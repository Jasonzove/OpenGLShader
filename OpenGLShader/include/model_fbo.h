/*===================================
*Copyright(c): huan.liu, 2020
*All rights reversed

*File name: model_fbo.h
*Brief: 增加离屏渲染需要的类fbo

*Data:2020.08.11
*Version: 1.0
=====================================*/

#ifndef __MODEL_FBO_H__
#define __MODEL_FBO_H__

#include <unordered_map>
#include <stack>
#include "glew.h"

enum BufferType
{
	NORMALCOLOR,
	HDRCOLOE,
	DEPTH,
};

class FrameBufferObject
{
public:
	FrameBufferObject();
	~FrameBufferObject();
	//HDR/NORMAL buffer
	void AttachColorBuffer(
		const BufferType& bufferType, 
		const GLenum& attachment, 
		const GLenum& dataType,
		const int& width,
		const int& height);
	//depth buffer
	void AttachDepthBuffer(const BufferType& bufferType, const int& width, const int& height);

	void Finish();

	void Bind();

	void UnBind();

	GLuint GetBufferByType(const BufferType& bufferType);
private:
	std::unordered_map<int, GLuint> mBuffers; //key:BufferType
	std::stack<GLenum> mDrawBuffers;
	GLuint mFBO;
};

#endif
