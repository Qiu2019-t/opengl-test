#include "Base.h"
#include "Shader.h"
#include "ffImage.h"
/*��װshader*/

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
	//����ĸ�VAO���л�ͼ
	glBindVertexArray(VAO);
	//���������Σ�������ʼ���������ƶ��ٶ���
	//glDrawArrays(GL_TRIANGLES,0,3);

	//6������������λ��0��ʼ��ȡ
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
	//��EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//��������
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//vbo indx
	glGenBuffers(1, &VBO);//ͬʱ��ȡ���
	//��vbo��indx
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	//��vbo�����Դ�ռ� ��������
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_STATIC_DRAW);//���㲻��																	
	//����shader���ݽ�����ʽ                 �������ж��Ƕ��������������Ϊ0
	//VAO��¼��VBO��ê����Ϣ                      ����            ƫ����
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
	//����ê��
	glEnableVertexAttribArray(0);
	//��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,8* sizeof(float), (void*)(3 * sizeof(float)));
	//����	               2��float                                            ƫ����
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//����ê��
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void initTexture() {
	_pImage = ffImage::readFromFile("res/container.jpg");
	
	glGenTextures(1,&_texture);//��ͼƬ��ֵ
	//�󶨵���������
	glBindBuffer(GL_TEXTURE_2D,_texture);
	//��������Χ�Ĵ�����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//�Ŵ����С�����ķ���
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	if (_pImage)
	{
		//����ͼƬ������  minmap������ͼԶ���ֲ���  ����洢��ʽ                    border 0?  ��ǰ��ȡͼƬ��δ洢����   ���������еĵ�ͨ�����ظ�ʽbyte
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//����ģʽ3�汾
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	//���ߣ���������
	GLFWwindow* window = glfwCreateWindow(800,600,"OpenGL Core",NULL,NULL);
	if (window==NULL) {
		cout << "Failed create GLFW window" << endl;
		glfwTerminate();//�˳�
		return -1;
	}
	glfwMakeContextCurrent(window);//������͵�ǰ���ڰ�

	//����ָ��??
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLFW"<<endl;
		return -1;
	}
	glViewport(0,0,800,600);
	//���ڴ�С�����䶯
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	initModel();
	initTexture();
	initShader("vertexShader.glsl", "fragmentShader.glsl");
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f,0.3f,0.3f,0.1f);
		glClear(GL_COLOR_BUFFER_BIT);//�廭��

		rend();

		glfwSwapBuffers(window);//˫����
		glfwPollEvents();//�ַ��¼�
	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}