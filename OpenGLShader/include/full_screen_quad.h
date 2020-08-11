/*===================================
*Copyright(c): huan.liu, 2020
*All rights reversed

*File name: full_screen_quad.h
*Brief: È«ÆÁËÄ±ßÐÎ

*Data:2020.08.12
*Version: 1.0
=====================================*/

#ifndef __FULL_SCREEN_QUAD_H__
#define __FULL_SCREEN_QUAD_H__

#include "glew.h"

class FullScreenQuad
{
public:
	void Init();
	void Draw(const GLint& posLocation);
	void DrawWithTexture(const GLint& posLocation, const GLint& pTextureLocation, const GLuint& texture);

private:
	GLuint mVBO;
};

#endif
