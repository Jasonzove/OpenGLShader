/*
===================================
*Copyright(C), huan.liu, 2019
*All rights reversed

*Author: huan.liu
*Address: huanliu_uestc@163.com

*FileName: shader.h
*Brief: shader class

*Data:2020/2/2
===================================
*/

#ifndef __SHADER_H__
#define __SHADER_H__

#include <unordered_map>
#include "macros.h"

LH_NAMESPACE_BEGIN

class Shader
{
public:
	static const char* GetShaderCode(const int& id);

public:
	static std::unordered_map<int, const char*> mShaderCodes;
};

LH_NAMESPACE_END

#endif // !__SHADER_H__
