/*
===================================
*Copyright(C), huan.liu, 2019
*All rights reversed

*Author: huan.liu
*Address: huanliu_uestc@163.com

*FileName: macros.h
*Brief: macros

*Data:2020/2/2
===================================
*/

//Ãû¿Õ¼ä
#ifndef LH_NAMESPACE_BEGIN
#define LH_NAMESPACE_BEGIN\
	namespace LH{
#endif // !LH_NAMESPACE_BEGIN

#ifndef LH_NAMESPACE_END
#define LH_NAMESPACE_END }
#endif // !LH_NAMESPACE_END

//¶Ñ„h³ý
#ifndef DEL_PTR
#define DEL_PTR(ptr)\
do\
{\
	if (nullptr != ptr)\
	{\
		delete(ptr);\
		ptr = nullptr;\
	}\
} while (false)
#endif // !DEL_PTR



