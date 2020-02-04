#include <string>
#include <sstream>
#include <stdio.h>
#include "obj_model.h"
#include "utils.h"

LH_NAMESPACE_BEGIN

void ObjModel::Init(const char* const& modelFilePath)
{
	//load model from file
	char* fileContent = Utils::LoadFileContent(modelFilePath);
	std::stringstream ssFileContent(fileContent);
	char szOneLine[256];
	while (!ssFileContent.eof())
	{
		memset(szOneLine, 0, 256);
		ssFileContent.getline(szOneLine, 256);
		if (strlen(szOneLine) > 0)
		{
			if (szOneLine[0] == 'v')
			{
				if (szOneLine[1] == 't') //texcoord
				{
					printf("texcoord: %s\n", szOneLine);
				}
				else if (szOneLine[1] == 'n') //normal
				{
					printf("normal: %s\n", szOneLine);

				}
				else //position
				{
					printf("position: %s\n", szOneLine);

				}
			}
			else if (szOneLine[0] == 'f') //face
			{
				printf("face: %s\n", szOneLine);

			}
		}
	}

	//decode model
	//convert to opengl vbo & ibo
	DEL_PTR(fileContent); //此处设计不好，堆上分配的内存在LoadFileContent函数里面
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