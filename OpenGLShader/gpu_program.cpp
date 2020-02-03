#include "gpu_program.h"
#include "shader.h"
#include "macros.h"

LH_NAMESPACE_BEGIN

GPUProgram::GPUProgram()
{
	mProgram = glCreateProgram();
}

GPUProgram::~GPUProgram()
{

}

void GPUProgram::AttachShader(GLuint shaderType, const int& shaderId)
{
	GLuint shader = CompileShader_i(shaderType, shaderId);
	if (0 != shaderId)
	{
		glAttachShader(mProgram, shader);
		mAttachedShaders.push(shader);
	}
}

void GPUProgram::Link()
{
	glLinkProgram(mProgram);
	GLint nResult;
	glGetProgramiv(mProgram, GL_LINK_STATUS, &nResult);
	if (GL_FALSE == nResult)
	{
		printf("create gpu program failed!, Link Error:\n");
		GLint logLength;
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &logLength);
		char* log = new char[logLength];
		memset(log, 0, logLength);
		GLsizei writed = 0;
		glGetProgramInfoLog(mProgram, logLength, &writed, log);
		printf("%s\n", log);
		DEL_PTR(log);
		glDeleteProgram(mProgram);
		mProgram = 0;
	}

	while (!mAttachedShaders.empty())
	{
		GLuint shader = mAttachedShaders.top();
		glDetachShader(mProgram, shader);
		glDeleteShader(shader);
		mAttachedShaders.pop();
	}
}

GLuint GPUProgram::CompileShader_i(GLuint shaderType, const int& shaderId)
{
	GLuint shader = glCreateShader(shaderType);
	const char* shaderCode = Shader::GetShaderCode(shaderId);
	if (nullptr == shaderCode)
	{
		printf("cannot load shader code, shaderId:%d\n", shaderId);
		return 0;
	}

	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	GLint nResult;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &nResult);
	if (GL_FALSE == nResult)
	{
		printf("compile shader failed!\n\nCODE:\n%s\n\n", shaderCode);
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		char* log = new char[logLength];
		memset(log, 0, logLength);
		GLsizei writed = 0;
		glGetShaderInfoLog(shader, logLength, &writed, log);
		printf("ERROR:\n%s\n", log);
		DEL_PTR(log);
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

LH_NAMESPACE_END