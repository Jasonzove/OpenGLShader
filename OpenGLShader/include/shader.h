#pragma once
#include <unordered_map>

class Shader
{
public:
	static const char* GetShaderCode(const int& id);

public:
	static std::unordered_map<int, const char*> mShaderCodes;
};
