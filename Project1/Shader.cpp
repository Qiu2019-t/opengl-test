#include "Shader.h"

void Shader::initShader(const char*_vertexPath, const char * _fragPath) {
	//��ȡ����Shader�Ĵ���
	std::string _vertexCode("");
	std::string _fragCode("");
	//�ļ���ȡ��
	std::ifstream _vShaderFile;
	std::ifstream _fShaderFile;
	_vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);//�ļ��𻵶�ȡʧ��
	_fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{	//���ļ�
		_vShaderFile.open(_vertexPath);
		_fShaderFile.open(_fragPath);
		//�����ļ��Ķ�ȡ��
		stringstream _vShaderStream, _fShaderStream;
		//��ȡ�ļ��Ļ������ݵ�������
		_vShaderStream << _vShaderFile.rdbuf();
		_fShaderStream << _fShaderFile.rdbuf();
		//�ر��ļ�������
		_vShaderFile.close();
		_fShaderFile.close();
		//ת����������string
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
	//shader�ı���������
	unsigned int _vertexID = 0, _fragID = 0;
	char _infoLog[512];//��ȡ������Ϣ
	int _successFlag = 0;
	//����
	//������ɫ��
	_vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertexID, 1, &_vShaderStr, NULL);//�������
	glCompileShader(_vertexID);
	//��ӡ�������(����еĻ�)
	glGetShaderiv(_vertexID, GL_COMPILE_STATUS, &_successFlag);//�����Ƿ�ɹ�
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
	//����
	m_shaderProgram = glCreateProgram();//����shader����
	glAttachShader(m_shaderProgram, _vertexID);//�������õ�����shader
	glAttachShader(m_shaderProgram, _fragID);
	glLinkProgram(m_shaderProgram);//����shader��һ��
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &_successFlag);
	if (!_successFlag) {
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, _infoLog);
		string errStr(_infoLog);
		cout << _infoLog << endl;
	}
	//ɾ����ɫ�����Ѿ����ӵ������У����±���õ�shaderProgram
	glDeleteShader(_vertexID);
	glDeleteShader(_fragID);
}
