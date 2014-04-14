#ifndef _JIA_XMATRIX4X4_
#define _JIA_XMATRIX4X4_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//这个类尚未完成整理

#include "XBasicFun.h"
#include "XMatrix3x3.h"
#include "XVector3.h"
#include "XVector4.h"
//4*4的矩阵
class _XMatrix4x4
{
public:
	float data[16];
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	void loadIdentity(void);
	_XVector4 getRow(int position) const;
	_XMatrix4x4() {loadIdentity();}
	_XMatrix4x4(float e0, float e1, float e2, float e3,
				float e4, float e5, float e6, float e7,
				float e8, float e9, float e10, float e11,
				float e12, float e13, float e14, float e15)
	{
		data[0] = e0;	data[1] = e1; 	data[2] = e2;	data[3] = e3;
		data[4] = e4;	data[5] = e5; 	data[6] = e6;	data[7] = e7;
		data[8] = e8;	data[9] = e9; 	data[10] = e10;	data[11] = e11;
		data[12] = e12;	data[13] = e13;	data[14] = e14;	data[15] = e15;
	}
	_XMatrix4x4 operator*(const _XMatrix4x4 & rhs) const;	//CROSS product
	_XMatrix4x4 inverse();
	_XMatrix3x3 toMatrix3x3()
	{
		return _XMatrix3x3(data[0],data[1],data[2],
			data[4],data[5],data[6],
			data[8],data[9],data[10]);
	}
	void fromMatrix3x3(const _XMatrix3x3 &m)
	{
		data[0] = m.data[0];data[1] = m.data[1];data[2] = m.data[2];data[3] = 0.0f;
		data[4] = m.data[3];data[5] = m.data[4];data[4] = m.data[5];data[5] = 0.0f;
		data[8] = m.data[6];data[9] = m.data[7];data[7] = m.data[8];data[8] = 0.0f;
		data[12] = 0.0f;data[13] = 0.0f;data[14] = 0.0f;data[15] = 1.0f;
	}

	_XVector3 operator*(const _XVector3 &v) const;	//v必须是列向量，否则没有意义，OpenGL中默认为列向量
	_XVector4 operator*(const _XVector4 &v) const;	//v必须是列向量，否则没有意义，OpenGL中默认为列向量
	//取x行y列的元素
	float& operator () (int x,int y);
	float operator () (int x,int y) const;
	_XMatrix4x4 getRotate(const _XMatrix3x3 &mxtr);
	//下面的函数用于求解三元一次方程组
	//aX + bY + cZ = d;
	//eX + fY + gZ = h;
	//iX + jY + kZ = l;
	//对应的4阶行列式为
	//a,b,c,d,
	//e,f,g,h,
	//i,j,k,l,
	//0,0,0,1,
	_XVector3 resolveEquation();	//求解三元一次方程组
	_XMatrix4x4 anti() //反置
	{
		return _XMatrix4x4(data[0],data[4],data[8],data[12],
		data[1],data[5],data[9],data[13],
		data[2],data[6],data[10],data[14],
		data[3],data[7],data[11],data[15]);
	}
};
#include "XMatrix4x4.inl"
inline _XVector4 operator*(const _XVector4 &v,const _XMatrix4x4 &m)//v必须是行向量，否则没有意义，DirectX中默认为行向量
{
	return _XVector4(
		m.data[0] * v.x + m.data[4] * v.y + m.data[8] * v.z + m.data[12] * v.w,
		m.data[1] * v.x + m.data[5] * v.y + m.data[9] * v.z + m.data[13] * v.w,
		m.data[2] * v.x + m.data[6] * v.y + m.data[10] * v.z + m.data[14] * v.w,
		m.data[3] * v.x + m.data[7] * v.y + m.data[11] * v.z + m.data[15] * v.w);
}
//从四元素转换成旋转矩阵
inline _XMatrix4x4 toMatrix4x4(const _XVector4 &v)
{//尚未经过验证
	return _XMatrix4x4(2.0f * (v.x * v.x + v.w * v.w) - 1.0f,2.0f * (v.x * v.y + v.z * v.w),2.0f * (v.x * v.z - v.y * v.w),0.0f,
		2.0f * (v.x * v.y - v.z * v.w),2.0f * (v.y * v.y + v.w * v.w) - 1.0f,2.0f * (v.y * v.z + v.x * v.w),0.0f,
		2.0f * (v.x * v.z + v.y * v.w),2.0f * (v.y * v.z - v.x * v.w),2.0f * (v.z * v.z + v.w * v.w) - 1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f);
}
//m * v
inline _XVector3 transform4x4(const _XMatrix4x4& m,const _XVector3& v) 
{
	return _XVector3((m.data[0]*v.x + m.data[1]*v.y + m.data[2]*v.z),
	                 (m.data[4]*v.x + m.data[5]*v.y + m.data[6]*v.z),
	                 (m.data[8]*v.x + m.data[9]*v.y + m.data[10]*v.z));
}
//v * m
inline _XVector3 transform4x4(const _XVector3& v, const _XMatrix4x4& m) 
{
	return _XVector3((m.data[0]*v.x + m.data[4]*v.y + m.data[8]*v.z),
	               (m.data[1]*v.x + m.data[5]*v.y + m.data[9]*v.z),
	               (m.data[2]*v.x + m.data[6]*v.y + m.data[10]*v.z));
}
#endif