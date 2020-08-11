/*===================================
*Copyright(c): huan.liu, 2020
*All rights reversed

*File name: utils.h
*Brief: ¹¤¾ß

*Data:2020.08.07
*Version: 1.0
=====================================*/

#ifndef __UTILS_H__
#define __UTILS_H__

#include "glew.h"

char* LoadFileContent(const char* const& filePath);

unsigned char* LoadBMP(const char* const& path, int& width, int& height);

void SavePixelDataToBMP(
	const char* const& filePath,
	unsigned char* const& pixelData,
	const int& width,
	const int& height);

GLuint CreateGPUBufferObject(GLenum targetType, GLsizeiptr size, GLenum usage, const void* data = nullptr);

GLuint CreateTexture(const char* const& filePath);

float* CreatePerspective(float fov, float aspect, float zNear, float zFar);

void CheckGLError(const char* const& pFile, const int& line);

#define GL_CHECK(x) \
do\
{\
	x;\
	CheckGLError(__FILE__, __LINE__);\
} while (false);

#endif

