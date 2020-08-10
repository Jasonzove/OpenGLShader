#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

#include "model_obj.h"
#include "utils.h"

ObjMoel::ObjMoel() :mVBO(0), mEBO(0), mElementCount(0)
{

}

ObjMoel::~ObjMoel()
{

}

bool ObjMoel::Load(const char* const& pFilePath)
{
	typedef struct FloatData
	{
		FloatData()
		{
			memset(v, 0, 3 * sizeof(float));
		}

		float v[3];
	}FloatData;

	typedef struct VertexDefine
	{
		int positionIndex;
		int texcoordIndex;
		int normalIndex;
	}VertexDefine;

	char* pFileContent = LoadFileContent(pFilePath);
	if (pFileContent == nullptr)
	{
		//日志
		printf("ObjMoel::Load():LoadFileContent failed!\n");
		return false;
	}

	std::stringstream ssFileContent(pFileContent);
	std::string temp;
	char szOneLine[256];
	std::vector<FloatData> positions, normals, texcoords; //放数据的位置
	std::vector<VertexDefine> vertexDefines; //顶点数据索引
	std::vector<unsigned int> indexes; //面索引

	while (!ssFileContent.eof())
	{
		memset(szOneLine, 0, 256 * sizeof(char));
		ssFileContent.getline(szOneLine, 256);
		//printf(">%s\n", szOneLine);
		if (strlen(szOneLine) > 0)
		{
			if (szOneLine[0] == 'v')
			{
				std::stringstream ssOneLine(szOneLine);
				FloatData data;
				memset(&data, 0, sizeof(FloatData));
				if (szOneLine[1] == 't')
				{
					ssOneLine >> temp;
					ssOneLine >> data.v[0];
					ssOneLine >> data.v[1];
					texcoords.push_back(data);
					//printf("texcoord:%f,%f\n", data.v[0], data.v[1]);
				}
				else if (szOneLine[1] == 'n')
				{
					ssOneLine >> temp;
					ssOneLine >> data.v[0];
					ssOneLine >> data.v[1];
					ssOneLine >> data.v[2];
					normals.push_back(data);
					//printf("normal:%f,%f,%f\n", data.v[0], data.v[1],data.v[2]);
				}
				else
				{
					ssOneLine >> temp;
					ssOneLine >> data.v[0];
					ssOneLine >> data.v[1];
					ssOneLine >> data.v[2];
					positions.push_back(data);
					//printf("position:%f,%f,%f\n", data.v[0], data.v[1], data.v[2]);
				}
			}
			else if (szOneLine[0] == 'f')
			{
				std::stringstream ssOneLine(szOneLine);
				ssOneLine >> temp;
				std::string vertexString;
				for (int i = 0; i < 3; ++i)
				{
					ssOneLine >> vertexString;
					int pos = vertexString.find_first_of('/');
					std::string positionIndex = vertexString.substr(0, pos);
					int pos2 = vertexString.find_first_of('/', pos + 1);
					std::string texcoordIndex = vertexString.substr(pos + 1, pos2 - pos - 1);
					std::string normalIndex = vertexString.substr(pos2 + 1, vertexString.length() - pos2 - 1);
					//printf("%s,%s,%s\n", positionIndex.c_str(), texcoordIndex.c_str(), normalIndex.c_str());
					VertexDefine vd;
					vd.positionIndex = atoi(positionIndex.c_str());
					vd.texcoordIndex = atoi(texcoordIndex.c_str());
					vd.normalIndex = atoi(normalIndex.c_str());

					int currentVertexIndex = -1;
					for (int i = 0; i < vertexDefines.size(); ++i)
					{
						if (vertexDefines[i].positionIndex == vd.positionIndex &&
							vertexDefines[i].texcoordIndex == vd.texcoordIndex &&
							vertexDefines[i].normalIndex == vd.normalIndex)
						{
							currentVertexIndex = i;
							break;
						}
					}

					//没有找到重复点
					if (-1 == currentVertexIndex)
					{
						vertexDefines.push_back(vd);
						currentVertexIndex = vertexDefines.size() - 1;
					}

					indexes.push_back(currentVertexIndex);
				}
			}
		}
	}

	//vbo
	int vertexCount = vertexDefines.size();
	auto vertexes = new VertexData[vertexCount];
	for (int i = 0; i < vertexCount; ++i)
	{
		memcpy(vertexes[i].position, positions[vertexDefines[i].positionIndex - 1].v, 3 * sizeof(float));
		memcpy(vertexes[i].texcoord, texcoords[vertexDefines[i].texcoordIndex - 1].v, 2 * sizeof(float));
		memcpy(vertexes[i].normal, normals[vertexDefines[i].normalIndex - 1].v, 3 * sizeof(float));
	}
	mVBO = CreateGPUBufferObject(GL_ARRAY_BUFFER, vertexCount * sizeof(VertexData), GL_STATIC_DRAW, vertexes);

	//ebo
	mElementCount = indexes.size();
	auto elements = new unsigned int[mElementCount];
	memcpy(elements, indexes.data(), mElementCount * sizeof(unsigned int));
	mEBO = CreateGPUBufferObject(GL_ELEMENT_ARRAY_BUFFER, mElementCount * sizeof(unsigned int), GL_STATIC_DRAW, elements);

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
