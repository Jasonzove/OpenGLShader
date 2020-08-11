#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include "utils.h"

char* LoadFileContent(const char* const& filePath)
{
	if (filePath == nullptr)
	{
		printf("LoadFileContent():filePath is nullptr!\n");
		return nullptr;
	}
	FILE* pFile = nullptr;
	fopen_s(&pFile, filePath, "rb");
	if (pFile == nullptr)
	{
		printf("LoadFileContent():open file failed!\n");
		return nullptr;
	}

	fseek(pFile, 0, SEEK_END);
	int len = ftell(pFile);
	char* buffer = new char[len + 1];
	rewind(pFile);
	fread(buffer, len, 1, pFile);
	buffer[len] = '\0';
	fclose(pFile);

	//printf("%s\n", buffer);

	return buffer;
}

unsigned char* LoadBMP(const char* const& path, int& width, int& height)
{
	unsigned char* imageData = nullptr;
	FILE* pFile;
	fopen_s(&pFile, path, "rb");
	if (pFile)
	{
		BITMAPFILEHEADER bfh;
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, pFile);
		if (bfh.bfType == 0x4D42) //0x4D42:bmp
		{
			BITMAPINFOHEADER bih;
			fread(&bih, sizeof(BITMAPINFOHEADER), 1, pFile);
			width = bih.biWidth;
			height = bih.biHeight;
			int pixelCount = width * height * 3;
			imageData = new unsigned char[pixelCount];
			fseek(pFile, bfh.bfOffBits, SEEK_SET);
			fread(imageData, 1, pixelCount, pFile);

			//bgr -> rgb
			for (int i = 0; i < pixelCount; i += 3)
			{
				imageData[i] = imageData[i] ^ imageData[i + 2];
				imageData[i + 2] = imageData[i] ^ imageData[i + 2];
				imageData[i] = imageData[i] ^ imageData[i + 2];
			}
		}
		fclose(pFile);
	}
	return imageData;
}

void SavePixelDataToBMP(
	const char* const& filePath,
	unsigned char* const& pixelData,
	const int& width,
	const int& height)
{
	FILE* pFile;
	fopen_s(&pFile, filePath, "wb");
	if (pFile)
	{
		BITMAPFILEHEADER bfh;
		memset(&bfh, 0, sizeof(BITMAPFILEHEADER));
		bfh.bfType = 0x4D42;
		bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * 3;
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, pFile);

		BITMAPINFOHEADER bih;
		memset(&bih, 0, sizeof(BITMAPINFOHEADER));
		bih.biWidth = width;
		bih.biHeight = height;
		bih.biBitCount = 24;
		bih.biSize = sizeof(BITMAPINFOHEADER);
		fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, pFile);

		for (int i = 0; i < width * height * 3; i += 3)
		{
			pixelData[i] = pixelData[i] ^ pixelData[i + 2];
			pixelData[i + 2] = pixelData[i] ^ pixelData[i + 2];
			pixelData[i] = pixelData[i] ^ pixelData[i + 2];
		}
		fwrite(pixelData, width * height * 3, 1, pFile);
	}

	fclose(pFile);
	return;
}

GLuint CreateGPUBufferObject(GLenum targetType, GLsizeiptr size, GLenum usage, const void* data)
{
	GLuint object;
	glGenBuffers(1, &object);
	glBindBuffer(targetType, object);
	glBufferData(targetType, size, data, usage);
	glBindBuffer(targetType, 0);
	return object;
}

const unsigned long FROMAT_DXT1 = 0x31545844l;//DXT1 -> 1 T X D 的asci码
static unsigned char* DecodeDXT(const char* const& fileContent, int& width, int& height, int& size)
{
	height = *(unsigned long*)(fileContent + sizeof(unsigned long) * 3);
	width = *(unsigned long*)(fileContent + sizeof(unsigned long) * 4);
	size = *(unsigned long*)(fileContent + sizeof(unsigned long) * 5);
	unsigned long compressFormat = *(unsigned long*)(fileContent + sizeof(unsigned long) * 21);
	switch (compressFormat)
	{
	case FROMAT_DXT1:
		printf("DXT1\n");

		break;
	default:
		break;
	}

	unsigned char* pPixelData = new unsigned char[size];
	memcpy(pPixelData, (fileContent + sizeof(unsigned long) * 32), size);

	return pPixelData;
}

GLuint CreateTexture(const char* const& filePath)
{
	if (filePath == nullptr)
	{
		printf("CreateTexture(): filepath is nullpt!\n");
		return -1;
	}
	char* pFileContent = LoadFileContent(filePath);
	if (pFileContent == nullptr)
	{
		printf("CreateTexture(): pFileContent is nullpt!\n");
		return -1;
	}

	int width = 0;
	int height = 0;
	int dxt1size = 0;
	unsigned char* piexelData = nullptr;
	GLenum format = GL_RGB;
	if (*((unsigned short*)pFileContent) == 0x4D42)
	{
		piexelData = LoadBMP(filePath, width, height);
	}
	else if (memcmp(pFileContent, "DDS ", 4) == 0) //压缩格式,有alpha通道
	{
		piexelData = DecodeDXT(pFileContent, width, height, dxt1size);
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	}

	if (nullptr == piexelData)
	{
		printf("CreateTexture(): decode data failed!\n");
		return -1;
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //超过的变成1
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	if (format == GL_RGB)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, piexelData);
	}
	else if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
	{
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, dxt1size, piexelData);
	}

	delete piexelData;
	piexelData = nullptr;

	return textureId;
}

void CheckGLError(const char* const& pFile, const int& line)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		switch (error)
		{
		case GL_INVALID_ENUM:
			printf("GL ERROR: GL_INVALID_ENUM. %s:%d\n", pFile, line);
			break;
		case GL_INVALID_VALUE:
			printf("GL ERROR: GL_INVALID_VALUE. %s:%d\n", pFile, line);
			break;
		case GL_INVALID_OPERATION:
			printf("GL ERROR: GL_INVALID_OPERATION. %s:%d\n", pFile, line);
			break;
		case GL_STACK_OVERFLOW:
			printf("GL ERROR: GL_STACK_OVERFLOW. %s:%d\n", pFile, line);
			break;
		case GL_STACK_UNDERFLOW:
			printf("GL ERROR: GL_STACK_UNDERFLOW. %s:%d\n", pFile, line);
			break;
		case GL_OUT_OF_MEMORY:
			printf("GL ERROR: GL_OUT_OF_MEMORY. %s:%d\n", pFile, line);
			break;
		default:
			printf("GL ERROR: %0x. %s:%d\n", error, pFile, line);
			break;
		}
	}
}

float* CreatePerspective(float fov, float aspect, float zNear, float zFar)
{
	float* matrix = new float[16];
	float half = fov / 2.0f;
	float randiansOfHalf = (half / 180.0f) * 3.14f;
	float yscale = cos(randiansOfHalf) / sin(randiansOfHalf);
	float xscale = yscale / aspect;
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = xscale;
	matrix[5] = yscale;
	matrix[10] = (zNear + zFar) / (zNear - zFar);
	matrix[11] = -1.0f;
	matrix[14] = (2.0f * zNear * zFar) / (zNear - zFar);
	return matrix;
}