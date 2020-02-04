#include <string>
#include <sstream>
#include <stdio.h>
#include <vector>
#include "obj_model.h"
#include "utils.h"

LH_NAMESPACE_BEGIN

void ObjModel::Init(const char* const& modelFilePath)
{
	std::vector<VertexInfo> positions;
	std::vector<VertexInfo> texcoords;
	std::vector<VertexInfo> normals;

	std::vector<VertexDefine> vertices;
	std::vector<unsigned int> objIndexes;
	//load model from file
	char* fileContent = Utils::LoadFileContent(modelFilePath);
	std::stringstream ssFileContent(fileContent);
	char szOneLine[256];
	std::string temp;
	while (!ssFileContent.eof())
	{
		memset(szOneLine, 0, 256);
		ssFileContent.getline(szOneLine, 256);
		if (strlen(szOneLine) > 0)
		{
			std::stringstream ssOneLine(szOneLine);
			if (szOneLine[0] == 'v')
			{
				if (szOneLine[1] == 't') //texcoord
				{
					VertexInfo vi;
					ssOneLine >> temp;
					ssOneLine >> vi.v[0];
					ssOneLine >> vi.v[1];
					texcoords.push_back(vi);
					//printf("texcoord: %f,%f\n", vi.v[0], vi.v[1]);//测试代码
				}
				else if (szOneLine[1] == 'n') //normal
				{
					VertexInfo vi;
					ssOneLine >> temp;
					ssOneLine >> vi.v[0];
					ssOneLine >> vi.v[1];
					ssOneLine >> vi.v[2];
					normals.push_back(vi);
					//printf("normal: %f,%f,%F\n", vi.v[0], vi.v[1], vi.v[2]);
				}
				else //position
				{
					VertexInfo vi;
					ssOneLine >> temp;
					ssOneLine >> vi.v[0];
					ssOneLine >> vi.v[1];
					ssOneLine >> vi.v[2];
					positions.push_back(vi);
					//printf("position: %f,%f, %f\n", vi.v[0], vi.v[2], vi.v[2]);
				}
			}
			else if (szOneLine[0] == 'f') //face
			{
				ssOneLine >> temp;
				std::string vertexStr;
				for (int i = 0; i < 3; ++i)
				{
					ssOneLine >> vertexStr;
					size_t pos1 = vertexStr.find_first_of('/');
					std::string positionIndexStr = vertexStr.substr(0, pos1);
					size_t pos2 = vertexStr.find_first_of('/', pos1 + 1);
					std::string textcoordIndexStr = vertexStr.substr(pos1 + 1, pos2 - (pos1 + 1));
					std::string normalIndexStr = vertexStr.substr(pos2 + 1, vertexStr.length() - (pos2 + 1));

					VertexDefine vd;
					vd.positionIndex = atoi(positionIndexStr.c_str());
					vd.texcoordIndex = atoi(textcoordIndexStr.c_str());
					vd.normalIndex = atoi(positionIndexStr.c_str());

					//去重
					int nCurrentIndex = -1;
					for (int j = 0; j < vertices.size(); ++j)
					{
						if (vertices[j].positionIndex == vd.positionIndex &&
							vertices[j].texcoordIndex == vd.texcoordIndex &&
							vertices[j].normalIndex == vd.normalIndex)
						{
							nCurrentIndex = j;
							break;
						}
					}

					if (nCurrentIndex == -1)
					{
						vertices.push_back(vd);//->vbo
						nCurrentIndex = vertices.size() - 1;
					}

					objIndexes.push_back(nCurrentIndex);//->ibo
				}
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