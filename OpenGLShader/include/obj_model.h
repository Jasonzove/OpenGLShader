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

struct VertexData
{
	float position[3];
	float texcoord[2];
	float normal[3];
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