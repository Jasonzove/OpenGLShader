#include "fbo.h"

LH_NAMESPACE_BEGIN

FBO::FBO()
{
	glGenFramebuffers(1, &mFBO);
}

void FBO::AttachColorBuffer(const char* const& bufferName, const GLenum& attachment, const GLenum& dataType, const int& width, const int& height)
{
	GLuint colorBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, dataType, width, height, GL_FALSE, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, colorBuffer, 0);
	
	mDrawBuffers.push(attachment);
	mBuffers.insert(std::pair<std::string, GLuint>(bufferName, colorBuffer));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::AttachDepthBuffer(const char* const& bufferName, const int& width, const int& height)
{
	GLuint depthBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, GL_FALSE, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_TEXTURE_2D, depthBuffer, 0);
	mBuffers.insert(std::pair<std::string, GLuint>(bufferName, depthBuffer));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}

void FBO::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Finish()
{
	int nCount = static_cast<int>(mDrawBuffers.size());
	if (nCount > 0)
	{
		GLenum* buffers = new GLenum[nCount];
		int i = 0;
		while (!mDrawBuffers.empty())
		{
			buffers[++i] = mDrawBuffers.top();
			mDrawBuffers.pop();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glDrawBuffers(nCount, buffers);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

GLuint FBO::GetBuffer(const char* const& bufferName)
{
	auto iter = mBuffers.find(bufferName);
	if (iter != mBuffers.end())
	{
		return iter->second;

	}

	return 0;
}

LH_NAMESPACE_END