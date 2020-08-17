/*===================================
*Copyright(c): huan.liu, 2020
*All rights reversed

*File name: full_screen_quad.h
*Brief: 全屏四边形

*Data:2020.08.12
*Version: 1.0
=====================================*/

#ifndef __FULL_SCREEN_QUAD_H__
#define __FULL_SCREEN_QUAD_H__

#include "glew.h"

enum FullScreenType
{
	FULLSCREEN = 0,
	LTFULLSCREEN, //左上
	LBFULLSCREEN, 
	RTFULLSCREEN,
	RBFULLSCREEN,
};

class FullScreenQuad
{
public:
	void Init();
	void Draw(const GLint& posLocation);
	void DrawWithTexture(
		const FullScreenType& type, 
		const GLint& posLocation, 
		const GLint& texcoordLocation, 
		const GLint& pTextureLocation, 
		const GLuint& texture);

private:
	GLuint mVBO;
	GLuint mLTVBO, mLBVBO, mRTVBO, mRBVBO;
};

#endif
