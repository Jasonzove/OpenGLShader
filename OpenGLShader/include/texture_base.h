/*
===================================
*Copyright(C), huan.liu, 2019
*All rights reversed

*Author: huan.liu
*Address: huanliu_uestc@163.com

*FileName: texture_base.h
*Brief: class texture base

*Data:2020/2/5
===================================
*/

#ifndef __TEXTURE_BASE_H__
#define __TEXTURE_BASE_H__

#include "macros.h"
#include "glew.h"

LH_NAMESPACE_BEGIN

class TextureBase
{
public:
	TextureBase();
	virtual ~TextureBase();

public:
	void Init(const char* const& bmpFilePath);

private:
	virtual void GenTexture_i();

public:
	unsigned char* mImageData;
	int mWidth;
	int mHeight;
	GLuint mTexture;
};

LH_NAMESPACE_END

#endif // !__TEXTURE_BASE_H__

