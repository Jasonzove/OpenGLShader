#include "GPUProgram.h"

GPUProgram::GPUProgram()
{
	mProgram = glCreateProgram();
}

GPUProgram::~GPUProgram()
{

}

void GPUProgram::Bind()
{
	glUseProgram(mProgram);
}

void GPUProgram::UnBind()
{
	glUseProgram(0);
}

bool GPUProgram::Link()
{
	//compile all sahders
	for (auto& shader : mShaderCodes)
	{
		GLuint shaderId = CompileShader_i(shader.first, shader.second);
		if (shaderId == 0)
		{
			printf("GPUProgram::Link():CompileShader_i failed!\n");
			return false;
		}
		mShaderIDs.push_back(shaderId);
	}

	//attach shader
	for (auto& shaderId : mShaderIDs)
	{
		glAttachShader(mProgram, shaderId);
	}

	//link
	glLinkProgram(mProgram);

	//detach sahder
	DetachShader_i();

	//log
	GLint linkResult = GL_TRUE;
	glGetProgramiv(mProgram, GL_LINK_STATUS, &linkResult);
	if (linkResult != GL_TRUE)
	{
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;
		glGetProgramInfoLog(mProgram, 1024, &logLen, szLog);
		printf("GPUProgram::Link():glLinkProgram failed!\n");
		glDeleteProgram(mProgram);
		mProgram = 0;
		return false;
	}

	return true;
}

void GPUProgram::AttachShader(const ShaderType& shaderType, const char* const& shaderCode)
{
	if (shaderCode == nullptr)
	{
		printf("GPUProgram::AttachShader():shader code is nullptr!\n");
		return;
	}

	if (mShaderCodes.find(shaderType) != mShaderCodes.end())
	{
		printf("GPUProgram::AttachShader():the same type shader finded in mShadercodes!\n");
		return;
	}

	mShaderCodes.insert(std::make_pair(shaderType, shaderCode));
}

void GPUProgram::SetUniformfv(const char* const& pLocation, const float* const pData, const int& size)
{
	GLint location;
	location = GetLocation(pLocation, LocationType::UNIFORM);

	switch (size)
	{
	case 1:
		glUniform1fv(location, 1, pData);
		break;
	case 2:
		glUniform2fv(location, 1, pData);
		break;
	case 4:
		glUniform4fv(location, 1, pData);
		break;
	case 16:
		glUniformMatrix4fv(location, 1, GL_FALSE, pData);
	default:
		break;
	}
}

void GPUProgram::SetTexture(const char* const& pTexLocation, const int& id, const int& size)
{
	GLint location = GetLocation(pTexLocation, LocationType::UNIFORM);

	switch (size)
	{
	case 1:
		glUniform1i(location, id);
		break;
	default:
		break;
	}
}

void GPUProgram::SetAttribPointer(const char* const& pLocation, const int& size, const int& stride, const void* const& pointer)
{
	GLint location;
	location = GetLocation(pLocation, LocationType::ATTRIBUTE);

	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride, pointer);
}

GLint GPUProgram::GetLocation(const char* const& pLocation, const LocationType& type)
{
	auto iter = mLocations.find(pLocation);
	if (iter != mLocations.end())
	{
		return iter->second;
	}

	GLint location = -1;
	switch (type)
	{
	case ATTRIBUTE:
		location = glGetAttribLocation(mProgram,pLocation);
		break;
	case UNIFORM:
		location = glGetUniformLocation(mProgram, pLocation);
		break;
	default:
		break;
	}

	if (-1 == location)
	{
		printf("can not get location:%s\n", pLocation);
		return -1;
	}

	mLocations.insert(std::make_pair(pLocation, location));
	return location;
}

GLuint GPUProgram::ID() const
{
	return mProgram;
}

GLuint GPUProgram::CompileShader_i(const ShaderType& shaderType, const char* const& shaderCode)
{
	if (shaderCode == nullptr)
	{
		printf("GPUProgram::CompileShader_i():shaderCode is nullptr!\n");
		return 0;
	}

	GLenum GLShaderType = GL_VERTEX_SHADER;
	switch (shaderType)
	{
	case VERTEX_SHADER:
		GLShaderType = GL_VERTEX_SHADER;
		break;
	case TESSLATION_CONTROL_SHADER:
		GLShaderType = GL_TESS_CONTROL_SHADER;
		break;
	case TESSLATION_EVALUATE_SHADER:
		GLShaderType = GL_TESS_EVALUATION_SHADER;
		break;
	case GEOMETRY_SHADER:
		GLShaderType = GL_GEOMETRY_SHADER;
		break;
	case FRAGEMENT_SHADER:
		GLShaderType = GL_FRAGMENT_SHADER;
		break;
	case COMPUTE_SHADER:
		GLShaderType = GL_COMPUTE_SHADER;
		break;
	default:
		break;
	}

	GLuint shader = glCreateShader(GLShaderType);
	if (shader == 0)
	{
		glDeleteShader(shader);
		printf("GPUProgram::CompileShader_i():glCreateShader failed!\n");
		return 0;
	}
	glShaderSource(shader, 1, &shaderCode, nullptr); //´«ÈëÏÔ¿¨
	glCompileShader(shader);
	GLint compileResult = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (compileResult != GL_TRUE)
	{
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;
		glGetShaderInfoLog(shader, 1024, &logLen, szLog);
		printf("GPUProgram::CompileShader_i():glCompileShader failed!\nlog\n:%s\ncode\n:%s\n", szLog, shaderCode);
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void GPUProgram::DetachShader_i()
{
	for (auto& shaderId : mShaderIDs)
	{
		glDetachShader(mProgram, shaderId);
		glDeleteShader(shaderId);
	}
}
