#include <windows.h>
#include "shader.h"

std::unordered_map<int, const char*> Shader::mShaderCodes = std::unordered_map<int, const char*>();

const char* const& Shader::GetShaderCode(int id)
{
	if (mShaderCodes.end() != mShaderCodes.find(id))
	{
		return mShaderCodes[id];
	}

	HMODULE handle = ::GetModuleHandle(TEXT("OpenGLShader.exe"));

	HRSRC rc = ::FindResource(handle, MAKEINTRESOURCE(id), TEXT("SHADER_FILE"));
	if (rc == nullptr)
	{
		throw std::exception("failed to find shader resource file!");
	}

	const char* data = nullptr;
	HGLOBAL rcData = ::LoadResource(handle, rc);
	data = static_cast<const char*>(::LockResource(rcData));
	if (data == nullptr)
	{
		throw std::exception("failed to load shader resource file!");
	}

	mShaderCodes[id] = data;

	return mShaderCodes[id];
}