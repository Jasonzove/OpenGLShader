#include "obj_model.h"

LH_NAMESPACE_BEGIN

void ObjModel::Init(const char* const& modelFilePath)
{
	//load model from file
	//decode model
	//convert to opengl vbo & ibo
}

void ObjModel::Bind(GLint posLoc, GLint texcoordLoc, GLint normalLoc)
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);

	glEnableVertexAttribArray(texcoordLoc);
	glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(normalLoc);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 5));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ObjModel::Draw()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

LH_NAMESPACE_END