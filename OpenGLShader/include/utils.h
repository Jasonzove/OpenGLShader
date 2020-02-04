/*
===================================
*Copyright(C), huan.liu, 2019
*All rights reversed

*Author: huan.liu
*Address: huanliu_uestc@163.com

*FileName: utils.h
*Brief: tool class

*Data:2020/2/2
===================================
*/
#ifndef __UTILS_H__
#define __UTILS_H__

#include"macros.h"

LH_NAMESPACE_BEGIN

class Utils
{
public:
	static char* LoadFileContent(const char* const& filePath);

	static unsigned char* LoadBMP(const char* path, int& width, int& height);
};


LH_NAMESPACE_END

#endif
