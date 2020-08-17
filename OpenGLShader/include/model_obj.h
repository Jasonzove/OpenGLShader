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

typedef enum BindType
{
	NONE = 0X00,
	BINDTEXCOORD = 0X01,
	BINDNORMAL = 0X02,
}BindType;

class ObjMoel
{
public:
	ObjMoel();
	~ObjMoel();
	bool Load(const char* const& pFilePath);
	void Bind(const GLuint& posLocation, const BindType& bindType, const GLuint& texcoordLocation = 0, const GLuint& normalLocation = 0);
	void Draw();
private:
	GLuint mVBO;
	GLuint mEBO;
	unsigned int mElementCount;
};

#endif
