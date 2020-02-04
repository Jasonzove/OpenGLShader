/*
===================================
*Copyright(C), huan.liu, 2019
*All rights reversed

*Author: huan.liu
*Address: huanliu_uestc@163.com

*FileName: obj_model.h
*Brief: obj model

*Data:2020/2/4
===================================
*/

#ifndef __OBJ_MODEL_H__
#define __OBJ_MODEL_H__

#include "macros.h"
#include "glew.h"

LH_NAMESPACE_BEGIN

struct VertexData //Opengl需要的数据
{
	float position[3];
	float texcoord[2];
	float normal[3];
};

struct VertexInfo //解码obj模型需要的数据结构，用于保存position,texcoord,normal数据
{
	float v[3];
	VertexInfo()
	{
		memset(v, 0, sizeof(float) * 3);
	}
};

struct VertexDefine //点的定义
{
	unsigned int positionIndex;
	unsigned int texcoordIndex;
	unsigned int normalIndex;
};

class ObjModel
{
public:
	void Init(const char* const& modelFilePath);
	void Bind(GLint posLoc, GLint texcoordLoc, GLint normalLoc);
	void Draw();
public:
	GLuint mVBO;
	GLuint mIBO;
	unsigned int mIndexCount;
};

#endif // !__OBJ_MODEL_H__

LH_NAMESPACE_END