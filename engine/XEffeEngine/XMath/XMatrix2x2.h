#ifndef _JIA_XMATRIX2X2_
#define _JIA_XMATRIX2X2_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//#include "stdio.h"
//#include "stdlib.h"
//#include "string.h"
#include "XVector2.h"
namespace XE{
class XMat2
{
public:
	float data[4];
	static const XMat2 identity;	//单位矩阵
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	void loadIdentity();
	XMat2(float d0,float d1,float d2,float d3)
	{
		data[0] = d0;
		data[1] = d1;
		data[2] = d2;
		data[3] = d3;
	}
	XMat2() { loadIdentity(); }

	float& operator () (int x,int y);
	float operator () (int x,int y) const;
	XVec2 operator *(const XVec2& v);	//v必须是列向量，否则没有意义，OpenGL中默认为列向量 -- 应用与坐标系变化中的缩放和旋转

	float getValue();//计算二阶行列式的值

	XMat2 operator*(const XMat2 & rhs) const;
	XMat2 inverse();	//求逆矩阵
	XMat2 anti();		//求反置矩阵
//	void anti();
};
#if WITH_INLINE_FILE
#include "XMatrix2x2.inl"
#endif
//v * m
inline XVec2 operator *(const XVec2& v,const XMat2 & m)	//v必须是行向量，否则没有意义，DirectX中默认为行向量
{
	return XVec2(v.x * m.data[0] + v.y * m.data[2],
		v.x * m.data[1] + v.y * m.data[3]);
}
namespace XMath{
//沿v轴缩放size
inline XMat2 getMatrix2D(float size,const XVec2& v)		//通过旋转的角度和缩放的尺寸计算旋转矩阵
{
	float t = size - 1.0f;
	return XMat2(1.0f + t * v.x * v.x, t * v.x * v.y,
		t * v.x * v.y, 1.0f + t * v.y * v.y);
}
//angle [0 - 2PI]
inline XMat2 getMatrix2D(float angle)		//通过旋转的角度计算旋转矩阵
{
	return XMat2(cos(angle), sin(angle), -sin(angle), cos(angle));
}
inline XMat2 getMatrix2D(const XVec2& s)	//缩放产生的矩阵
{
	return XMat2(s.x, 0.0f, 0.0f, s.y);
}
//inline float getAngle(const XMat2 &m);	//通过旋转矩阵计算旋转的角度(尚未实现)
//inline XVec2 getSize(const XMat2 &m);	//通过旋转矩阵计算缩放的尺寸(尚未实现)
}
}
#endif