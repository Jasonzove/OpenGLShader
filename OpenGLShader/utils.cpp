#include <stdio.h>
#include "utils.h"

LH_NAMESPACE_BEGIN

const char* Utils::LoadFileContent(const char* const& filePath)
{
	char* pFileContent = NULL;
	FILE* pFile = fopen(filePath, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		if (nLen > 0)
		{
			rewind(pFile);
			pFileContent = new char[nLen + 1];
			fread(pFileContent, 1, nLen, pFile);
			pFileContent[nLen] = '\0';
		}
		fclose(pFile);
	}
	return pFileContent;
}

LH_NAMESPACE_END