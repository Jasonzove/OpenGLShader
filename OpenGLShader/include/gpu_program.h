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

public:
	GLuint mProgram; //program id
	std::stack<GLuint> mAttachedShaders;

private:
	GLuint CompileShader_i(GLuint shaderType, const int& shaderId);
};

LH_NAMESPACE_END

#endif // !__GPU_PROGRAM_H__
