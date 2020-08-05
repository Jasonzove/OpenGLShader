/*
===================================
*Copyright(C), huan.liu, 2019
*All rights reversed

*Author: huan.liu
*Address: huanliu_uestc@163.com

*FileName: gpu_program.h
*Brief: GPU Program

*Data:2020/2/2
===================================
*/

#ifndef __GPU_PROGRAM_H__
#define __GPU_PROGRAM_H__

#include <stack>
#include <unordered_map>
#include <string>
#include "macros.h"
#include "glew.h"

LH_NAMESPACE_BEGIN

class GPUProgram
{
public:
	GPUProgram();
	virtual ~GPUProgram();

public:
	void AttachShader(GLuint shaderType, const int& shaderId);
	void Link();
	void DetectAttribute(const char* const& attributeName);
	void DetectUniform(const char* const& uniformName);
	GLint GetLocation(const char* const& name);

public:
	GLuint mProgram; //program id
	std::stack<GLuint> mAttachedShaders;
	std::unordered_map<std::string, GLint> mLocations;

private:
	GLuint CompileShader_i(GLuint shaderType, const int& shaderId);
};

LH_NAMESPACE_END

#endif // !__GPU_PROGRAM_H__
