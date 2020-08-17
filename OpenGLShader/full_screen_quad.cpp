#include "full_screen_quad.h"
#include "utils.h"

void FullScreenQuad::Init()
{
	float pos[] = {
		-0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -1.0f, 0.0f, 1.0f,
	};
	mVBO = CreateGPUBufferObject(GL_ARRAY_BUFFER, sizeof(pos), GL_STATIC_DRAW, pos);

	//вСио╫г
	float ltPos[] = {
	-0.5f, 0.0f, -1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
	0.0f, 0.5f, -1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, -1.0f,  0.0f, 1.0f,
	};
	mLTVBO = CreateGPUBufferObject(GL_ARRAY_BUFFER, sizeof(ltPos), GL_STATIC_DRAW, ltPos);

	//вСоб╫г
	float lbPos[] = {
	-0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
	0.0f, -0.5f, -1.0f,	 1.0f, 0.0f,
	0.0f, 0.0f, -1.0f,	1.0f, 1.0f,
	-0.5f, 0.0f, -1.0f,	 0.0f, 1.0f,
	};
	mLBVBO = CreateGPUBufferObject(GL_ARRAY_BUFFER, sizeof(lbPos), GL_STATIC_DRAW, lbPos);

	//срио╫г
	float rtPos[] = {
	0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	0.5f, 0.0f, -1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -1.0f, 1.0f, 1.0f,
	0.0f, 0.5f, -1.0f, 0.0f, 1.0f,
	};
	mRTVBO = CreateGPUBufferObject(GL_ARRAY_BUFFER, sizeof(rtPos), GL_STATIC_DRAW, rtPos);

	//сроб╫г
	float rbPos[] = {
	0.0f, -0.5f, -1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -1.0f, 1.0f, 0.0f,
	0.5f, 0.0f, -1.0f, 1.0f, 1.0f,
	0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
	};
	mRBVBO = CreateGPUBufferObject(GL_ARRAY_BUFFER, sizeof(rbPos), GL_STATIC_DRAW, rbPos);
}

void FullScreenQuad::DrawWithTexture(
	const FullScreenType& type, 
	const GLint& posLocation, 
	const GLint& texcoordLocation,
	const GLint& pTextureLocation, 
	const GLuint& texture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(pTextureLocation, 0);

	GLuint vbo;
	switch (type)
	{
	case FULLSCREEN:
		vbo = mVBO;
		break;
	case LTFULLSCREEN:
		vbo = mLTVBO;
		break;
	case LBFULLSCREEN:
		vbo = mLBVBO;
		break;
	case RTFULLSCREEN:
		vbo = mRTVBO;
		break;
	case RBFULLSCREEN:
		vbo = mRBVBO;
		break;
	default:
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(texcoordLocation);
	glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glDrawArrays(GL_QUADS, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FullScreenQuad::Draw(const GLint& posLocation)
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
