#pragma once
#include <unordered_map>

class Shader
{
public:
	static const char* const& GetShaderCode(int id);

public:
	static std::unordered_map<int, const char*> mShaderCodes;
};
