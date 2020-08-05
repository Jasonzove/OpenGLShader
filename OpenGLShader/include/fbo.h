/*
===================================
*Copyright(C), huan.liu, 2019
*All rights reversed

*Author: huan.liu
*Address: huanliu_uestc@163.com

*FileName: fbo.h
*Brief: class FBO

*Data:2020/2/6
===================================
*/

#ifndef __FBO_H__
#define __FBO_H__

#include <unordered_map>
#include <string>
#include <stack>
#include "macros.h"
#include "glew.h"

LH_NAMESPACE_BEGIN

class FBO
{
public:
	FBO();
	void AttachColorBuffer(const char* const& bufferName, const GLenum& attachment, const GLenum& dataType, const int& width, const int& height);
	void AttachDepthBuffer(const char* const& bufferName, const int& width, const int& height);
	void Bind();
	void UnBind();
	void Finish();

	GLuint GetBuffer(const char* const& bufferName);
public:
	GLuint mFBO;
	std::unordered_map<std::string, GLuint> mBuffers;
	std::stack<GLenum> mDrawBuffers;
};

LH_NAMESPACE_END

#endif // !__FBO_H__
