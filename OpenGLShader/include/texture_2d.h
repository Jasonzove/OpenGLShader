/*
===================================
*Copyright(C), huan.liu, 2019
*All rights reversed

*Author: huan.liu
*Address: huanliu_uestc@163.com

*FileName: texture_2d.h
*Brief: class texture 2D

*Data:2020/2/5
===================================
*/

#ifndef __TEXTURE_2D_H__
#define __TEXTURE_2D_H__

#include "macros.h"
#include "texture_base.h"

LH_NAMESPACE_BEGIN

class Texture2D : public TextureBase
{
public:
	Texture2D();
	~Texture2D();
private:
	virtual void GenTexture_i();
};

LH_NAMESPACE_END

#endif // !__TEXTURE_2D_H__

