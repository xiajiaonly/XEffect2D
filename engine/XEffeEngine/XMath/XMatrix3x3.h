#ifndef _JIA_XMATRIX3X3_
#define _JIA_XMATRIX3X3_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//#include "stdio.h"
//#include "stdlib.h"
//#include "string.h"
#include "XVector2.h"
#include "XVector3.h"
#include "XVector4.h"
namespace XE{
class XMat3
{
public:
	float data[9];
	static const XMat3 identity;	//单位矩阵
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	float& operator () (int x,int y);
	float operator () (int x,int y) const;
	
	void loadIdentity();
	XMat3(float d0,float d1,float d2,float d3,float d4,
		float d5,float d6,float d7,float d8)
	{
		data[0] = d0;data[1] = d1;data[2] = d2;
		data[3] = d3;data[4] = d4;data[5] = d5;
		data[6] = d6;data[7] = d7;data[8] = d8;
	}
	XMat3() {loadIdentity();}
	XMat3 operator*(const XMat3 & rhs) const;
	//点A * mat转换成B时满足B * mat.inverse() = A
	XMat3 inverse();	//求逆矩阵
	float getValue();	//计算三阶行列式的值	
	//下面的函数用于求解二元一次方程组
	//aX + bY = c;
	//dX + eY = f;
	//对应的4阶行列式为
	//a,b,c,
	//d,e,f,
	//0,0,1,
	XVec2 resolveEquation();				//求解二元一次方程组
	XVec3 operator *(const XVec3& v);	//v必须是列向量，否则没有意义，OpenGL中默认为列向量
	XMat3 anti();		//求反置矩阵
//	void anti();	//尚未实现
};
#if WITH_INLINE_FILE
#include "XMatrix3x3.inl"
#endif
namespace XMath{
//v * m
inline XVec3 transform3x3(const XVec3&  v, const XMat3& m)
{
	return XVec3((m.data[0] * v.x + m.data[3] * v.y + m.data[6] * v.z),
		(m.data[1] * v.x + m.data[4] * v.y + m.data[7] * v.z),
		(m.data[2] * v.x + m.data[5] * v.y + m.data[8] * v.z));
}
//m * v
inline XVec3 transform3x3(const XMat3& m, const XVec3&  v)
{
	return XVec3((m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z),
		(m.data[3] * v.x + m.data[4] * v.y + m.data[5] * v.z),
		(m.data[6] * v.x + m.data[7] * v.y + m.data[8] * v.z));
}
inline XMat3 getMatrix3D(const XVec3& s)	//缩放产生的矩阵
{
	return XMat3(s.x, 0.0f, 0.0f,
		0.0f, s.y, 0.0f,
		0.0f, 0.0f, s.z);
}
//沿v轴缩放size
inline XMat3 getMatrix3D(float size, const XVec3& v)		//通过旋转的角度和缩放的尺寸计算旋转矩阵
{
	float t = size - 1.0f;
	return XMat3(1.0f + t * v.x * v.x, t * v.x * v.y, t * v.x * v.z,
		t * v.x * v.y, 1.0f + t * v.y * v.y, t * v.y * v.z,
		t * v.x * v.z, t * v.y * v.z, 1.0f + t * v.z * v.z);
}
//绕v轴旋转angle的旋转矩阵[0 - 360]
inline XMat3 getRotate3D(float angle, XVec3 v)
{
	float sinA = sin(angle * DEGREE2RADIAN);
	float cosA = cos(angle * DEGREE2RADIAN);
	float tCosA = 1.0f - cosA;
	v.normalize();
	return XMat3(tCosA * v.x * v.x + cosA, tCosA * v.x * v.y + v.z * sinA, tCosA * v.x * v.z - sinA * v.y,
		tCosA * v.x * v.y - sinA * v.z, tCosA * v.y * v.y + cosA, tCosA * v.y * v.z + sinA * v.x,
		tCosA * v.x * v.z + sinA * v.y, tCosA * v.y * v.z - v.x * sinA, tCosA * v.z * v.z + cosA);
}
//绕x轴旋转angle[0 - 360]
inline XMat3 getRotate3DX(float angle)
{
	float sinA = sin(angle * DEGREE2RADIAN);
	float cosA = cos(angle * DEGREE2RADIAN);
	return XMat3(1.0f, 0.0f, 0.0f,
		0.0f, cosA, sinA,
		0.0f, -sinA, cosA);
}
//绕y轴旋转angle[0 - 360]
inline XMat3 getRotate3DY(float angle)
{
	float sinA = sin(angle * DEGREE2RADIAN);
	float cosA = cos(angle * DEGREE2RADIAN);
	return XMat3(cosA, 0.0f, -sinA,
		0.0f, 1.0f, 0.0f,
		sinA, 0.0f, cosA);
}
//绕z轴旋转angle[0 - 360]
inline XMat3 getRotate3DZ(float angle)
{
	float sinA = sin(angle * DEGREE2RADIAN);
	float cosA = cos(angle * DEGREE2RADIAN);
	return XMat3(cosA, sinA, 0.0f,
		-sinA, cosA, 0.0f,
		0.0f, 0.0f, 1.0f);
}
//欧拉角到旋转矩阵[0 - 360]
inline XMat3 getRotate3D(const XVec3& a)
{//通过验证
	XMat3 ret = getRotate3DZ(a.z);
	ret = ret * getRotate3DX(a.x);
	ret = ret * getRotate3DY(a.y);
	//ret = getRotate3DY(a.y) * ret;
	//ret = getRotate3DX(a.x) * ret;
	return ret;
}
//旋转向量:用单位向量来表示转轴，用模来表示角度,OpenCV中常用到这个概念，在摄像机标定时有这个概念
//旋转向量到旋转矩阵之间的转换,单位：弧度
inline XMat3 getRVec2RMat(const XVec3& a)
{
	//float angle = a.getLength();
	//XVec3 dir = normalize(a);
	//return getRotate3D(angle * RADIAN2DEGREE, dir);
	return getRotate3D(a.getLength() * RADIAN2DEGREE, a);
}
//欧拉角到旋转矩阵,角度单位：角度[0 - 360]
inline XMat3 getRotateMatrix(const XVec3& a)
{//通过验证
	float sh,ch, sp,cp, sb,cb;
	sp = sin(a.x * DEGREE2RADIAN);
	cp = cos(a.x * DEGREE2RADIAN);
	sh = sin(a.y * DEGREE2RADIAN);
	ch = cos(a.y * DEGREE2RADIAN);
	sb = sin(a.z * DEGREE2RADIAN);
	cb = cos(a.z * DEGREE2RADIAN);

	XMat3 ret;

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
inline XMat3 getRotateMatByLookAt(const XVec3& eyePos,
	const XVec3& LookAtPos, const XVec3& up)
{//通过验证
	XVec3 zaxis = normalize(eyePos - LookAtPos);
	XVec3 xaxis = normalize(up * zaxis);
	XVec3 yaxis = zaxis * xaxis;

	return XMat3(-xaxis.x, -xaxis.y, -xaxis.z,
		yaxis.x, yaxis.y, yaxis.z,
		-zaxis.x, -zaxis.y, -zaxis.z);
}
//通过旋转矩阵计算欧拉角:单位弧度，[0 - PI2]
inline XVec3 getEulerian(const XMat3& m)
{//通过验证
	XVec3 ret;
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
inline XMat3 toMatrix3x3(const XVec4& v)
{//经过验证
	float n = (v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w);
	float s = (n > 0.0f) ? (2.0f / n) : 0.0f;
	return XMat3(1.0f - (v.y * v.y * s + v.z * v.z *s), v.x * v.y * s - v.w * v.z * s, v.x * v.z * s + v.w * v.y * s,
		v.x * v.y * s + v.w * v.z * s, 1.0f - (v.x * v.x * s + v.z * v.z * s), v.y * v.z * s - v.w * v.x * s,
		v.x * v.z * s - v.w * v.y * s, v.y * v.z * s + v.w * v.x * s, 1.0f - (v.x * v.x * s + v.y * v.y * s));
}
extern XVec4 toVector4(const XMat3 &m);//从旋转矩阵到四元数的转换
}
//v * m
inline XVec3 operator *(const XVec3& v, const XMat3 & m)	//v必须是行向量，否则没有意义，DirectX中默认为行向量
{
	return XMath::transform3x3(v, m);
}
}
#endif