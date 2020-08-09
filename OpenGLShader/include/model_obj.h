/*===================================
*Copyright(c): huan.liu, 2020
*All rights reversed

*File name: model_obj.h
*Brief: obj模型类封装

*Data:2020.08.07
*Version: 1.0
=====================================*/

#ifndef __MODEL_OBJ_H__
#define __MODEL_OBJ_H__

#include "glew.h"

struct VertexData
{
	float position[3];
	float texcoord[2];
	float normal[3];
};

class ObjMoel
{
public:
	ObjMoel();
	~ObjMoel();
	bool Load(const char* const& pFilePath);
	void Bind(const GLuint& posLocation);
	void Bind(const GLuint& posLocation, const GLuint& texcoordLocation);
	void Bind(const GLuint& posLocation, const GLuint& texcoordLocation, const GLuint& normalLocation);
	void Draw();
private:
	GLuint mVBO;
	GLuint mEBO;
	unsigned int mElementCount;
};

#endif
