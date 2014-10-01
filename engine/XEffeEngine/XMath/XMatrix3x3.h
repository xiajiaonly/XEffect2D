#ifndef _JIA_XMATRIX3X3_
#define _JIA_XMATRIX3X3_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "XVector2.h"
#include "XVector3.h"
#include "XVector4.h"
class _XMatrix3x3
{
public:
	float data[9];
	static const _XMatrix3x3 identity;	//单位矩阵
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	float& operator () (int x,int y);
	float operator () (int x,int y) const;
	
	void loadIdentity();
	_XMatrix3x3(float d0,float d1,float d2,float d3,float d4,
		float d5,float d6,float d7,float d8)
	{
		data[0] = d0;data[1] = d1;data[2] = d2;
		data[3] = d3;data[4] = d4;data[5] = d5;
		data[6] = d6;data[7] = d7;data[8] = d8;
	}
	_XMatrix3x3() {loadIdentity();}
	_XMatrix3x3 operator*(const _XMatrix3x3 & rhs) const;
	_XMatrix3x3 inverse();
	float getValue();	//计算三阶行列式的值	
	//下面的函数用于求解二元一次方程组
	//aX + bY = c;
	//dX + eY = f;
	//对应的4阶行列式为
	//a,b,c,
	//d,e,f,
	//0,0,1,
	_XVector2 resolveEquation();				//求解二元一次方程组
	_XVector3 operator *(const _XVector3 &v);	//v必须是列向量，否则没有意义，OpenGL中默认为列向量
	_XMatrix3x3 anti();
//	void anti();	//尚未实现
};
#include "XMatrix3x3.inl"
//v * m
inline _XVector3 transform3x3(const _XVector3& v,const _XMatrix3x3& m) 
{
	return _XVector3((m.data[0]*v.x + m.data[3]*v.y + m.data[6]*v.z),
	                 (m.data[1]*v.x + m.data[4]*v.y + m.data[7]*v.z),
	                 (m.data[2]*v.x + m.data[5]*v.y + m.data[8]*v.z));
}
inline _XVector3 operator *(const _XVector3 &v,const _XMatrix3x3 & m)	//v必须是行向量，否则没有意义，DirectX中默认为行向量
{
	return transform3x3(v,m);
}
//m * v
inline _XVector3 transform3x3(const _XMatrix3x3& m,const _XVector3& v) 
{
	return _XVector3((m.data[0]*v.x + m.data[1]*v.y + m.data[2]*v.z),
	                 (m.data[3]*v.x + m.data[4]*v.y + m.data[5]*v.z),
	                 (m.data[6]*v.x + m.data[7]*v.y + m.data[8]*v.z));
}
inline _XMatrix3x3 getMatrix3D(const _XVector3 &s)	//缩放产生的矩阵
{
	return _XMatrix3x3(s.x,0,0,
						0,s.y,0,
						0,0,s.z);
}
//沿v轴缩放size
inline _XMatrix3x3 getMatrix3D(float size,const _XVector3 &v)		//通过旋转的角度和缩放的尺寸计算旋转矩阵
{
	float t = size - 1.0f;
	return _XMatrix3x3(1.0f + t * v.x * v.x,t * v.x * v.y,t * v.x * v.z,
						t * v.x * v.y,1.0f + t * v.y * v.y,t * v.y * v.z,
						t * v.x * v.z,t * v.y * v.z,1.0f + t * v.z * v.z);
}
//绕v轴旋转angle的旋转矩阵
inline _XMatrix3x3 getRotate3D(float angle,_XVector3 v)
{
	float sinA = sin(angle * DEGREE2RADIAN);
	float cosA = cos(angle * DEGREE2RADIAN);
	float tCosA = 1 - cosA;
	v = normalize(v);
	return _XMatrix3x3( tCosA * v.x * v.x + cosA,		tCosA * v.x * v.y + v.z * sinA, tCosA * v.x * v.z - sinA * v.y,
						tCosA * v.x * v.y - sinA * v.z,	tCosA * v.y * v.y + cosA,		tCosA * v.y * v.z + sinA * v.x,
						tCosA * v.x * v.z + sinA * v.y,	tCosA * v.y * v.z - v.x * sinA,	tCosA * v.z * v.z + cosA);
}
//绕x轴旋转angle
inline _XMatrix3x3 getRotate3DX(float angle)
{
	float sinA = sin(angle * DEGREE2RADIAN);
	float cosA = cos(angle * DEGREE2RADIAN);
	return _XMatrix3x3(	1,	0,		0,
						0,	cosA,	sinA,
						0,	-sinA,	cosA);
}
//绕y轴旋转angle
inline _XMatrix3x3 getRotate3DY(float angle)
{
	float sinA = sin(angle * DEGREE2RADIAN);
	float cosA = cos(angle * DEGREE2RADIAN);
	return _XMatrix3x3(	cosA,0,-sinA,
						0,1,0,
						sinA,0,cosA);
}
//绕z轴旋转angle
inline _XMatrix3x3 getRotate3DZ(float angle)
{
	float sinA = sin(angle * DEGREE2RADIAN);
	float cosA = cos(angle * DEGREE2RADIAN);
	return _XMatrix3x3(	cosA,sinA,0,
						-sinA,cosA,0,
						0,0,1);
}
//欧拉角到旋转矩阵
inline _XMatrix3x3 getRotate3D(const _XVector3 &a)
{//通过验证
	_XMatrix3x3 ret = getRotate3DZ(a.z);
	ret = ret * getRotate3DX(a.x);
	ret = ret * getRotate3DY(a.y);
	return ret;
}
//欧拉角到旋转矩阵
inline _XMatrix3x3 getRotateMatrix(const _XVector3 &a)
{//通过验证
	float sh,ch, sp,cp, sb,cb;
	sp = sin(a.x * DEGREE2RADIAN);
	cp = cos(a.x * DEGREE2RADIAN);
	sh = sin(a.y * DEGREE2RADIAN);
	ch = cos(a.y * DEGREE2RADIAN);
	sb = sin(a.z * DEGREE2RADIAN);
	cb = cos(a.z * DEGREE2RADIAN);

	_XMatrix3x3 ret;

	ret(0,0) = ch * cb + sh * sp * sb;
	ret(0,1) = -ch * sb + sh * sp * cb;
	ret(0,2) = sh * cp;

	ret(1,0) = sb * cp;
	ret(1,1) = cb * cp;
	ret(1,2) = -sp;

	ret(2,0) = -sh * cb + ch * sp * sb;
	ret(2,1) = sb * sh + ch * sp * cb;
	ret(2,2) = ch * cp;
	return ret;
}
//通过旋转矩阵计算欧拉角
inline _XVector3 getEulerian(const _XMatrix3x3& m)
{//通过验证
	_XVector3 ret;
	float sp = -(m(1,2));
	if(fabs(sp) > 0.9999f)
	{
		ret.x = PI_HALF * sp;	//pitch
		ret.y = atan2(-(m(2,0)),m(0,0));	//heading
		ret.z = 0.0f;		//bank
	}else
	{
		ret.x = asin(sp);
		ret.y = atan2(m(0,2),m(2,2));
		ret.z = atan2(m(1,0),m(1,1));
	}
	return ret;
}
//四元素转换为旋转矩阵
inline _XMatrix3x3 toMatrix3x3(const _XVector4 &v)
{//经过验证
	float n = (v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w);
	float s = (n > 0.0f) ? (2.0f / n) : 0.0f;
	return _XMatrix3x3(1.0f - (v.y * v.y * s + v.z * v.z *s),	v.x * v.y * s - v.w * v.z * s,			v.x * v.z * s + v.w * v.y * s,
						v.x * v.y * s + v.w * v.z * s,			1.0f - (v.x * v.x * s + v.z * v.z * s),	v.y * v.z * s - v.w * v.x * s,
						v.x * v.z * s - v.w * v.y * s,			v.y * v.z * s + v.w * v.x * s,			1.0f - (v.x * v.x * s + v.y * v.y * s));
}
extern _XVector4 toVector4(const _XMatrix3x3 &m);//从旋转矩阵到四元数的转换

#endif