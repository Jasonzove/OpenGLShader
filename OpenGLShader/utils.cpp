#include <stdio.h>
#include <windows.h>
#include "utils.h"

LH_NAMESPACE_BEGIN

char* Utils::LoadFileContent(const char* const& filePath)
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

unsigned char* Utils::LoadBMP(const char* path, int& width, int& height)
{
	unsigned char* imageData = nullptr;
	FILE* pFile = fopen(path, "rb");
	if (pFile)
	{
		BITMAPFILEHEADER bfh;
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, pFile);
		if (bfh.bfType == 0x4D42)
		{
			BITMAPINFOHEADER bih;
			fread(&bih, sizeof(BITMAPINFOHEADER), 1, pFile);
			width = bih.biWidth;
			height = bih.biHeight;
			int pixelCount = width * height * 3;
			imageData = new unsigned char[pixelCount];
			fseek(pFile, bfh.bfOffBits, SEEK_SET);
			fread(imageData, 1, pixelCount, pFile);

			unsigned char temp;
			for (int i = 0; i < pixelCount; i += 3)
			{
				temp = imageData[i + 2];
				imageData[i + 2] = imageData[i];
				imageData[i] = temp;
			}
		}
		fclose(pFile);
	}
	return imageData;
}

GLuint Utils::CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void* data)
{
	GLuint object;
	glGenBuffers(1, &object);
	glBindBuffer(bufferType, object);
	glBufferData(bufferType, size, data, usage);
	glBindBuffer(bufferType, 0);
	return object;
}

LH_NAMESPACE_END