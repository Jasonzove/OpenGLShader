#include "texture_2d.h"

LH_NAMESPACE_BEGIN

Texture2D::Texture2D()
{

}

Texture2D::~Texture2D()
{

}

void Texture2D::GenTexture_i()
{
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mImageData);
	glBindTexture(GL_TEXTURE_2D, 0);
}


LH_NAMESPACE_END