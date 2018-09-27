#ifndef _JIA_XMATRIX4X4_
#define _JIA_XMATRIX4X4_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//这个类尚未完成整理
//#include "XBasicFun.h"
#include "XMatrix3x3.h"
#include "XVector3.h"
#include "XVector4.h"
namespace XE{
//4*4的矩阵
class XMat4
{
public:
	float data[16];
	static const XMat4 identity;	//单位矩阵
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	void loadIdentity(void);
	XVec4 getRow(int position) const;
	XMat4() {loadIdentity();}
	XMat4(float e0, float e1, float e2, float e3,
		float e4, float e5, float e6, float e7,
		float e8, float e9, float e10, float e11,
		float e12, float e13, float e14, float e15)
	{
		data[0] = e0;	data[1] = e1; 	data[2] = e2;	data[3] = e3;
		data[4] = e4;	data[5] = e5; 	data[6] = e6;	data[7] = e7;
		data[8] = e8;	data[9] = e9; 	data[10] = e10;	data[11] = e11;
		data[12] = e12;	data[13] = e13;	data[14] = e14;	data[15] = e15;
	}
	XMat4 operator*(const XMat4 & rhs) const;	//CROSS product
	XMat4 inverse();			//求逆矩阵
	XMat3 toMatrix3x3()
	{
		return XMat3(data[0],data[1],data[2],
			data[4],data[5],data[6],
			data[8],data[9],data[10]);
	}
	void fromMatrix3x3(const XMat3 &m)
	{
		data[0] = m.data[0];data[1] = m.data[1];data[2] = m.data[2];data[3] = 0.0f;
		data[4] = m.data[3];data[5] = m.data[4];data[4] = m.data[5];data[5] = 0.0f;
		data[8] = m.data[6];data[9] = m.data[7];data[7] = m.data[8];data[8] = 0.0f;
		data[12] = 0.0f;data[13] = 0.0f;data[14] = 0.0f;data[15] = 1.0f;
	}

	XVec3 operator*(const XVec3& v) const;	//v必须是列向量，否则没有意义，OpenGL中默认为列向量
	XVec4 operator*(const XVec4& v) const;	//v必须是列向量，否则没有意义，OpenGL中默认为列向量
	//取x行y列的元素
	float& operator () (int x,int y);
	float operator () (int x,int y) const;
	XMat4 getRotate(const XMat3 &mxtr);
	//下面的函数用于求解三元一次方程组
	//aX + bY + cZ = d;
	//eX + fY + gZ = h;
	//iX + jY + kZ = l;
	//对应的4阶行列式为
	//a,b,c,d,
	//e,f,g,h,
	//i,j,k,l,
	//0,0,0,1,
	XVec3 resolveEquation();	//求解三元一次方程组
	XMat4 anti()			//反置矩阵
	{
		return XMat4(data[0], data[4], data[8], data[12],
			data[1], data[5], data[9], data[13],
			data[2], data[6], data[10], data[14],
			data[3], data[7], data[11], data[15]);
	}
	friend std::ostream& operator<<(std::ostream& os, const XMat4& m)
	{
		os << m.data[0] << "," << m.data[1] << "," << m.data[2] << "," << m.data[3] << std::endl;
		os << m.data[4] << "," << m.data[5] << "," << m.data[6] << "," << m.data[7] << std::endl;
		os << m.data[8] << "," << m.data[9] << "," << m.data[10] << "," << m.data[11] << std::endl;
		os << m.data[12] << "," << m.data[14] << "," << m.data[14] << "," << m.data[15] << std::endl;
		return os;
	}
	friend std::istream& operator>>(std::istream& is, XMat4& m) 
	{
		is >> m.data[0]; is.ignore(1);
		is >> m.data[1]; is.ignore(1);
		is >> m.data[2]; is.ignore(1);
		is >> m.data[3]; is.ignore(1);
		is >> m.data[4]; is.ignore(1);
		is >> m.data[5]; is.ignore(1);
		is >> m.data[6]; is.ignore(1);
		is >> m.data[7]; is.ignore(1);
		is >> m.data[8]; is.ignore(1);
		is >> m.data[9]; is.ignore(1);
		is >> m.data[10]; is.ignore(1);
		is >> m.data[11]; is.ignore(1);
		is >> m.data[12]; is.ignore(1);
		is >> m.data[13]; is.ignore(1);
		is >> m.data[14]; is.ignore(1);
		is >> m.data[15];
		return is;
	}
};
#if WITH_INLINE_FILE
#include "XMatrix4x4.inl"
#endif
//#if _MSC_VER
//#pragma message("Warning : Pre mult and Post mult is Anti.")
//#elif __GNUC__
//#warning("Warning : Pre mult and Post mult is Anti. ")
//#endif
inline XVec4 operator*(const XVec4& v,const XMat4 &m)//v必须是行向量，否则没有意义，DirectX中默认为行向量
{//注意这里pre_mult和post_mult反了
	return XVec4(
		m.data[0] * v.x + m.data[4] * v.y + m.data[8] * v.z + m.data[12] * v.w,
		m.data[1] * v.x + m.data[5] * v.y + m.data[9] * v.z + m.data[13] * v.w,
		m.data[2] * v.x + m.data[6] * v.y + m.data[10] * v.z + m.data[14] * v.w,
		m.data[3] * v.x + m.data[7] * v.y + m.data[11] * v.z + m.data[15] * v.w);
}
//#if _MSC_VER
//#pragma message("Warning : Pre mult and Post mult is Anti.")
//#elif __GNUC__
//#warning("Warning : Pre mult and Post mult is Anti. ")
//#endif
inline XVec3 operator*(const XVec3& v,const XMat4 &m)//preMult
{//注意这里pre_mult和post_mult反了
	float d = 1.0f / (m.data[3] * v.x + m.data[7] * v.y + m.data[11] * v.z + m.data[15]);
	return XVec3((m.data[0] * v.x + m.data[4] * v.y + m.data[8] * v.z + m.data[12]) * d,
		(m.data[1] * v.x + m.data[5] * v.y + m.data[9] * v.z + m.data[13]) * d,
		(m.data[2] * v.x + m.data[6] * v.y + m.data[10] * v.z + m.data[14]) * d);
}
namespace XMath{
//从四元素转换成旋转矩阵
inline XMat4 toMatrix4x4(const XVec4& v)
{//尚未经过验证
	return XMat4(2.0f * (v.x * v.x + v.w * v.w) - 1.0f, 2.0f * (v.x * v.y + v.z * v.w), 2.0f * (v.x * v.z - v.y * v.w), 0.0f,
		2.0f * (v.x * v.y - v.z * v.w), 2.0f * (v.y * v.y + v.w * v.w) - 1.0f, 2.0f * (v.y * v.z + v.x * v.w), 0.0f,
		2.0f * (v.x * v.z + v.y * v.w), 2.0f * (v.y * v.z - v.x * v.w), 2.0f * (v.z * v.z + v.w * v.w) - 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}
//m * v
inline XVec3 transform4x4(const XMat4& m,const XVec3&  v) 
{
	return XVec3((m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z),
		(m.data[4] * v.x + m.data[5] * v.y + m.data[6] * v.z),
		(m.data[8] * v.x + m.data[9] * v.y + m.data[10] * v.z));
}
//v * m
inline XVec3 transform4x4(const XVec3&  v, const XMat4& m) 
{
	return XVec3((m.data[0] * v.x + m.data[4] * v.y + m.data[8] * v.z),
		(m.data[1] * v.x + m.data[5] * v.y + m.data[9] * v.z),
		(m.data[2] * v.x + m.data[6] * v.y + m.data[10] * v.z));
}
}
}
#endif