/*===================================
*Copyright(c): huan.liu, 2020
*All rights reversed

*File name: GPUProgram.h
*Brief: program·â×°

*Data:2020.08.05
*Version: 1.0
=====================================*/

#ifndef __GPUPROGRAM_H__
#define __GPUPROGRAM_H__

#include <unordered_map>
#include <list>
#include "glew.h"

class GPUProgram
{
public:
	enum ShaderType
	{
		VERTEX_SHADER = 0,
		TESSLATION_CONTROL_SHADER,
		TESSLATION_EVALUATE_SHADER,
		GEOMETRY_SHADER,
		FRAGEMENT_SHADER,
		COMPUTE_SHADER,
	};

public:
	GPUProgram();
	~GPUProgram();
	void Bind();
	void UnBind();
	bool Link();
	void AttachShader(const ShaderType& shaderType, const char* const& shaderCode);
	GLuint ProgramId() const;
private:
	GLuint CompileShader_i(const ShaderType& shaderType, const char* const& shaderCode);
	void DetachShader_i();

private:
	GLuint mProgram;
	std::list<GLuint> mShaderIDs;
	std::unordered_map<ShaderType, const char*> mShaderCodes;
};

#endif