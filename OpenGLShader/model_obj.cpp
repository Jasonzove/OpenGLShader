#include "model_obj.h"

ObjMoel::ObjMoel() :mVBO(0), mEBO(0), mElementCount(0)
{

}

ObjMoel::~ObjMoel()
{

}

bool ObjMoel::Load(const char* const& pFilePath)
{

	return true;
}

void ObjMoel::Bind(const GLuint& posLocation)
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	//pos
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ObjMoel::Bind(const GLuint& posLocation, const GLuint& texcoordLocation)
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	//pos
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
	//texcoord
	glEnableVertexAttribArray(texcoordLocation);
	glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ObjMoel::Bind(const GLuint& posLocation, const GLuint& texcoordLocation, const GLuint& normalLocation)
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	//pos
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
	//texcoord
	glEnableVertexAttribArray(texcoordLocation);
	glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(3 * sizeof(float)));
	//normal
	glEnableVertexAttribArray(normalLocation);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(5 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ObjMoel::Draw()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glDrawElements(GL_TRIANGLES, mElementCount, GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
