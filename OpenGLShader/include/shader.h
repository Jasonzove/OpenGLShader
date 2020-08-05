/*===================================
*Copyright(c): huan.liu, 2020
*All rights reversed

*File name: shader.h
*Brief: ��ȡshaderԴ�벢����

*Data:2020.08.05
*Version: 1.0
=====================================*/

#ifndef __SHADER_H__
#define __SHADER_H__

#include <unordered_map>

class Shader
{
public:
	static const char* GetShaderCode(const int& id);

public:
	static std::unordered_map<int, const char*> mShaderCodes;
};

#endif