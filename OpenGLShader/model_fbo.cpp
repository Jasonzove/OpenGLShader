#include "model_fbo.h"

FrameBufferObject::FrameBufferObject()
{
	glGenFramebuffers(1, &mFBO);
}

FrameBufferObject::~FrameBufferObject()
{

}

void FrameBufferObject::AttachColorBuffer(const BufferType& bufferType, const GLenum& attachment, const GLenum& dataType, const int& width, const int& height)
{
	GLuint colorBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (dataType == GL_RGBA16F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, dataType, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, nullptr);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, dataType, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, colorBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mDrawBuffers.push(attachment);
	mBuffers.insert(std::make_pair(bufferType, colorBuffer));
}

void FrameBufferObject::AttachDepthBuffer(const BufferType& bufferType, const int& width, const int& height)
{
	GLuint depthBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mBuffers.insert(std::make_pair(bufferType, depthBuffer));
}

void FrameBufferObject::Finish()
{
	if (mDrawBuffers.empty())
	{
		printf("FrameBufferObject::Finish():mDrawBuffers is empty()!\n");
		return;
	}
	int bufferSize = mDrawBuffers.size();
	GLenum* buffers = new GLenum[bufferSize];
	int i = 0;
	while (!mDrawBuffers.empty())
	{
		buffers[i++] = mDrawBuffers.top();
		mDrawBuffers.pop();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glDrawBuffers(bufferSize, buffers);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}

void FrameBufferObject::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBufferObject::GetBufferByType(const BufferType& bufferType)
{
	auto iter = mBuffers.find(bufferType);
	if (iter != mBuffers.end())
	{
		return iter->second;
	}

	return 0;
}
