#include "Base.h"
#include "Shader.h"
#include "ffImage.h"
/*封装shader*/

unsigned int VBO = 0;
unsigned int VAO = 0;
unsigned int EBO = 0;
unsigned int _texture = 0;

ffImage* _pImage = NULL;

Shader _shader;
void framebuffer_size_callback(GLFWwindow* window,int width,int height){
	glViewport(0,0,width,height);
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window,true);
	}
}
void rend() {
	glBindTexture(GL_TEXTURE_2D,_texture);
	_shader.start();
	//针对哪个VAO进行绘图
	glBindVertexArray(VAO);
	//类型三角形，顶点起始索引，绘制多少顶点
	//glDrawArrays(GL_TRIANGLES,0,3);

	//6个索引，索引位置0开始读取
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
	_shader.end();
}
void initModel() {
	float vertices[] = {
	0.5f,0.5f,0.0f,  1.0f,0.0f,0.0f, 1.0f,1.0f,
	0.5f,-0.5f,0.0f, 0.0f,1.0f,0.0f, 1.0f,0.0f,
   -0.5f,-0.5f,0.0f, 0.0f,0.0f,1.0f, 0.0f,0.0f,
   -0.5f,0.5f,0.0f,  1.0f,1.0f,0.0f, 0.0f,1.0f
	};
	//hp1012
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	
	
	glGenBuffers(1, &EBO);
	//绑定EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//传输数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//vbo indx
	glGenBuffers(1, &VBO);//同时获取多个
	//绑定vbo的indx
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	//给vbo分配显存空间 传输数据
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_STATIC_DRAW);//顶点不变																	
	//告诉shader数据解析方式                 紧密排列都是顶点坐标可以设置为0
	//VAO记录了VBO的锚点信息                      步长            偏移量
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
	//激活锚点
	glEnableVertexAttribArray(0);
	//颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,8* sizeof(float), (void*)(3 * sizeof(float)));
	//纹理	               2个float                                            偏移量
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//激活锚点
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void initTexture() {
	_pImage = ffImage::readFromFile("res/container.jpg");
	
	glGenTextures(1,&_texture);//给图片赋值
	//绑定的纹理类型
	glBindBuffer(GL_TEXTURE_2D,_texture);
	//超出纹理范围的处理方法
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//放大或缩小采样的方法
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	if (_pImage)
	{
		//读入图片的数据  minmap纹理贴图远区分不开  纹理存储格式                    border 0?  当前读取图片如何存储像素   单体像素中的单通道像素格式byte
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _pImage->getWidth(), _pImage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _pImage->getData());
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
}

void initShader(const char*_vertexPath,const char * _fragPath) {
	_shader.initShader(_vertexPath, _fragPath);
}
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//核心模式3版本
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	//宽，高，窗口名字
	GLFWwindow* window = glfwCreateWindow(800,600,"OpenGL Core",NULL,NULL);
	if (window==NULL) {
		cout << "Failed create GLFW window" << endl;
		glfwTerminate();//退出
		return -1;
	}
	glfwMakeContextCurrent(window);//虚拟机和当前窗口绑定

	//函数指针??
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLFW"<<endl;
		return -1;
	}
	glViewport(0,0,800,600);
	//窗口大小发生变动
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	initModel();
	initTexture();
	initShader("vertexShader.glsl", "fragmentShader.glsl");
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f,0.3f,0.3f,0.1f);
		glClear(GL_COLOR_BUFFER_BIT);//清画布

		rend();

		glfwSwapBuffers(window);//双缓存
		glfwPollEvents();//分发事件
	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}