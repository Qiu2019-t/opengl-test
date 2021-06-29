#include "Shader.h"

void Shader::initShader(const char*_vertexPath, const char * _fragPath) {
	//读取两个Shader的代码
	std::string _vertexCode("");
	std::string _fragCode("");
	//文件读取器
	std::ifstream _vShaderFile;
	std::ifstream _fShaderFile;
	_vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);//文件损坏读取失败
	_fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{	//打开文件
		_vShaderFile.open(_vertexPath);
		_fShaderFile.open(_fragPath);
		//两个文件的读取流
		stringstream _vShaderStream, _fShaderStream;
		//读取文件的缓冲内容到数据流
		_vShaderStream << _vShaderFile.rdbuf();
		_fShaderStream << _fShaderFile.rdbuf();
		//关闭文件处理器
		_vShaderFile.close();
		_fShaderFile.close();
		//转换数据流到string
		_vertexCode = _vShaderStream.str();
		_fragCode = _fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		string errstr = "read shader fail";
		cout << errstr << endl;
	}
	const char* _vShaderStr = _vertexCode.c_str();
	const char* _fShaderStr = _fragCode.c_str();
	//shader的编译与链接
	unsigned int _vertexID = 0, _fragID = 0;
	char _infoLog[512];//存取错误信息
	int _successFlag = 0;
	//编译
	//顶点着色器
	_vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertexID, 1, &_vShaderStr, NULL);//传入代码
	glCompileShader(_vertexID);
	//打印编译错误(如果有的话)
	glGetShaderiv(_vertexID, GL_COMPILE_STATUS, &_successFlag);//编译是否成功
	if (!_successFlag) {
		glGetShaderInfoLog(_vertexID, 512, NULL, _infoLog);
		string errStr(_infoLog);
		cout << _infoLog << endl;
	}

	_fragID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragID, 1, &_fShaderStr, NULL);
	glCompileShader(_fragID);
	glGetShaderiv(_fragID, GL_COMPILE_STATUS, &_successFlag);
	if (!_successFlag) {
		glGetShaderInfoLog(_fragID, 512, NULL, _infoLog);
		string errStr(_infoLog);
		cout << _infoLog << endl;
	}
	//链接
	m_shaderProgram = glCreateProgram();//创建shader程序
	glAttachShader(m_shaderProgram, _vertexID);//加入编译好的两个shader
	glAttachShader(m_shaderProgram, _fragID);
	glLinkProgram(m_shaderProgram);//两个shader放一起
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &_successFlag);
	if (!_successFlag) {
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, _infoLog);
		string errStr(_infoLog);
		cout << _infoLog << endl;
	}
	//删除着色器，已经链接到程序中，留下编译好的shaderProgram
	glDeleteShader(_vertexID);
	glDeleteShader(_fragID);
}
