#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<stdlib.h>
#include<string>
#include<fstream>
#include<sstream>//¶ÁÈ¡Á÷
using namespace std;
typedef unsigned int uint;
typedef unsigned char byte;
struct ffRGBA
{
	byte m_r;
	byte m_g;
	byte m_b;
	byte m_a;
	ffRGBA(byte r=255, byte g=255, byte b=255, byte a=255) {
		m_r = r;
		m_g = g;
		m_b = b;
		m_a = a;
	}
};

template<typename T>
struct tVec3
{
	T m_x;
	T m_y;
	T m_z;
	tVec3(T _x = 0, T _y = 0, T _z = 0) {
		m_x = _x;
		m_y = _y;
		m_z = _z;
	}
};
template<typename T>
struct tVec2
{
	T m_x;
	T m_y;
	tVec2(T _x = 0, T _y = 0) {
		m_x = _x;
		m_y = _y;
	}
};
/*
typedef tVec3<float> ffPosition;
typedef tVec3<float> ffNormal;
typedef tVec2<float> ffTexCoord;
typedef tVec3<float> ffTangent;
*/