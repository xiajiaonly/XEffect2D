#ifndef _JIA_XVECTOR3_
#define _JIA_XVECTOR3_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "math.h"
namespace XE{
class XVec3
{
public:
	float x;
	float y;
	float z;
public:
	bool isZero() const { return x == 0.0f && y == 0.0f && z == 0.0f; }
	static const XVec3 zero;
	void reset();//重置这个点
	void add(float a, float b, float c);//设置这个点的值
	void set(float a, float b, float c);//这个点的值加上一个点的值
	XVec3(float a, float b, float c)
		:x(a), y(b), z(c)
	{}
	XVec3(float a)
		:x(a), y(a), z(a)
	{}
	XVec3(const XVec3&  temp)
		:x(temp.x), y(temp.y), z(temp.z)
	{}
	XVec3()
		:x(0.0f), y(0.0f), z(0.0f)
	{}
	XVec3 multi(const XVec3&  tmp) const
	{
		return XVec3(x * tmp.x, y * tmp.y, z * tmp.z);
	}
	XVec3 operator + (const XVec3&  temp) const;
	XVec3 operator - (const XVec3&  temp) const;
	//叉乘的几何意义
	//|a * b| = |a|*|b|*sin(夹角)，叉乘的结果垂直于a，b，方向为左手定则，大小符合前面的公式 
	XVec3 operator * (const XVec3&  temp) const;	//CROSS product
	XVec3 operator / (const XVec3&  temp) const;
	XVec3 operator + (const float& temp) const;
	XVec3 operator - (const float& temp) const;
	XVec3 operator * (const float& temp) const;
	XVec3 operator / (const float& temp) const;
	void operator += (const XVec3&  temp);
	void operator -= (const XVec3&  temp);
	void operator *= (const XVec3&  temp);	//CROSS product
	void operator /= (const XVec3&  temp);
	void operator += (const float& temp);
	void operator -= (const float& temp);
	void operator *= (const float& temp);
	void operator /= (const float& temp);

	float getLength(const XVec3&  P0) const;
	float getLength(float a, float b, float c) const;
	float getLength() const;
	float getLengthSqure(const XVec3&  P0) const;
	float getLengthSqure(float a, float b, float c) const;
	float getLengthSqure() const;

	float dot(const XVec3&  temp) const;//点乘，集合意义:a.dot(b) = |a|*|b|*cos(夹角)
	void normalize();	//单位化
	//重定义复制操作符和拷贝构造函数
	XVec3& operator = (const XVec3& temp);
	XVec3& operator = (const float* temp);	//逻辑不严密
	XVec3(const float *temp)	//这种逻辑不严密
	{
		if (temp == NULL)
		{
			x = y = z = 0.0f;
		}
		else
		{
			x = temp[0];
			y = temp[1];
			z = temp[2];
		}
	}
	XVec3 operator - () const; //-v
	friend std::ostream& operator<<(std::ostream& os, const XVec3&  vec)
	{
		os << vec.x << "," << vec.y << "," << vec.z;
		return os;
	}
	friend std::istream& operator >> (std::istream& is, XVec3& vec)
	{
		is >> vec.x;
		is.ignore(1);
		is >> vec.y;
		is.ignore(1);
		is >> vec.z;
		return is;
	}
	operator float* () const { return (float*) this; }
	operator const float* () const { return (const float*) this; }
};
typedef XVec3 XPoint3;
#if WITH_INLINE_FILE
#include "XVector3.inl"
#endif
namespace XMath{
inline XVec3 normalize(const XVec3&  v) 
{
	float d = v.getLengthSqure();
	if (d == 0.0f) return v * (1.0f / 0.0000001f);
	else return v * (1.0f / sqrt(d));
	//float d = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	//if(d == 0.0f) d = 0.0000001f;
	//return v * (1.0f/d);
}
//计算空间三个点构成的面的法线
inline XVec3 getNormal(const XVec3& p1,const XVec3& p2,const XVec3& p3)
{//计算三角面的法线
	//XVec3 w = p0 - p1;
	//XVec3 v = p2 - p1;
	//XVec3 n = XVec3(w.y * v.z - w.z * v.y,w.x * v.z - w.z * v.x,w.x * v.y - w.y * v.x);
	//return n / n.getLength();
	return normalize((p3 - p2) * (p1 - p2));
//	float a = ( (p2.y-p1.y)*(p3.z-p1.z)-(p2.z-p1.z)*(p3.y-p1.y) );    
//	float b = ( (p2.z-p1.z)*(p3.x-p1.x)-(p2.x-p1.x)*(p3.z-p1.z) );    
//	float c = ( (p2.x-p1.x)*(p3.y-p1.y)-(p2.y-p1.y)*(p3.x-p1.x) );    
//	return normalize(XVec3(a,b,c));
}
//计算空间三个点构成的面的法线
inline XVec3 getNormal(const float *p0,const float *p1,const float *p2)
{
//	XVec3 w(p0[0] - p1[0],p0[1] - p1[1],p0[2] - p1[2]);
//	XVec3 v(p2[0] - p1[0],p2[1] - p1[1],p2[2] - p1[2]);
//	XVec3 n(w.y * v.z - w.z * v.y,w.x * v.z - w.z * v.x,w.x * v.y - w.y * v.x);
//	return n / n.getLength();
	return normalize(XVec3(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]) *
		XVec3(p0[0] - p1[0], p0[1] - p1[1], p0[2] - p1[2]));
}
inline XVec3 operator *(float a,const XVec3& temp)
{//左乘
	return temp * a;
}
//点乘
//inline float operator^(const XVec3& v1,const XVec3&  v2)  // DOT product
//{
//	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
//}
//计算线段AB和BC的夹角,0 - PI,前进方向的左边的角
inline float getAngle(const XVec3& A,const XVec3& B,const XVec3& C)
{
	XVec3 tmp1 = B - A;
	XVec3 tmp2 = C - B;
	return acos(tmp1.dot(tmp2) / sqrt(tmp1.getLengthSqure() * tmp2.getLengthSqure()));
}
inline XVec3 projector(const XVec3& v1,const XVec3& v2)
{//求v1向向量v2投影形成的向量
	return v2 * (v1.dot(v2) / v2.getLengthSqure());
}
}
}
#endif