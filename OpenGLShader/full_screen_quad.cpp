#include "full_screen_quad.h"
#include "utils.h"

void FullScreenQuad::Init()
{
	float pos[] = {
		-0.5f, -0.5f, -1.0f,
		0.5f, -0.5f, -1.0f,
		0.5f, 0.5f, -1.0f,
		-0.5f, 0.5f, -1.0f
	};
	mVBO = CreateGPUBufferObject(GL_ARRAY_BUFFER, sizeof(pos), GL_STATIC_DRAW, pos);
}

void FullScreenQuad::DrawWithTexture(const GLint& posLocation, const GLint& pTextureLocation, const GLuint& texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(pTextureLocation, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
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
