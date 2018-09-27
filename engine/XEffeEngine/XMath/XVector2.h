#ifndef _JIA_XVECTOR2_
#define _JIA_XVECTOR2_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//#include "XBasicFun.h" 
#include "XMath.h"
#include "../XCommonDefine.h"

namespace XE{
//单精度2D点
class XVec2
{
public:
	float x;
	float y;
public:
	static const XVec2 zero;
	static const XVec2 zeroOne;
	static const XVec2 oneZero;
	static const XVec2 one;

	void reset();    //归零这个点
	void set(const XVec2& temp);
	void set(float a, float b);    //设置当前点的值
	void set(float v) { x = y = v; }
	void set(float* v);    //设置当前点的值
	void add(float a, float b);    //加上某个值

	XVec2 operator + (const XVec2& temp) const;
	XVec2 operator - (const XVec2& temp) const;
	//下面这两个定义在数学上没有意义，只是程序上的表达
	XVec2 operator * (const XVec2& temp) const;
	XVec2 operator / (const XVec2& temp) const;
	XVec2 operator + (const float& temp) const;
	XVec2 operator - (const float& temp) const;
	//下面这两个定义在数学上没有意义，只是程序上的表达
	XVec2 operator * (const float& temp) const;
	XVec2 operator / (const float& temp) const;
	void operator += (const XVec2& temp);
	void operator -= (const XVec2& temp);
	void operator *= (const XVec2& temp);
	void operator /= (const XVec2& temp);
	void operator += (const float& temp);
	void operator -= (const float& temp);
	void operator *= (const float& temp);
	void operator /= (const float& temp);

	//	XVec2& operator = (const XVector2I & temp);
	//	XVec2(const XVector2I & temp);

	float getLength(const XVec2& P0) const;     //获取两个点之间的距离
	float getLength(float a, float b) const;			//获取两个点之间的距离
	float getLength() const;						//获得到原点的距离
	float getLengthSqure(const XVec2& P0) const;//获取两点之间距离的平方
	float getLengthSqure(float a, float b) const;
	float getLengthSqure() const;					//获得到原点的距离的平方
	float getAngle(const XVec2& P0) const;      //获取从当前点指向目标点的角度
	float getAngle(float a, float b) const;          //获取从当前点指向目标点的角度
	float getAngle() const;							//获取原点到当前点的角度

	float dot(const XVec2& temp) const { return (x * temp.x + y * temp.y); }
	//点乘，也叫向量的内积、数量积。顾名思义，求下来的结果是一个数。 
	//点积： (x1 , y1 , z1 ) .( x2 , y2 , z2 ) = x1x2 + y1y2 + z1z2 
	//点积可以来计算两矢量的夹角,公式如下: 
	//cos (V dot W) =V.W / | V | | W | 
	//点乘的几何意义是：是一条边向另一条边的投影乘以另一条边的长度
	float cross(const XVec2& temp)const { return x * temp.y - y * temp.x; }	//CROSS product
	void normalize();	//单位化 或者 归一化
	XVec2 perp() { return XVec2(y, -x); }	//反射

	XVec2::XVec2(float a, float b)
		:x(a), y(b)
	{}
	XVec2::XVec2(float a)
		: x(a), y(a)
	{}
	XVec2::XVec2()
		: x(0.0f), y(0.0f)
	{}
	XVec2(const XVec2& temp)
		:x(temp.x), y(temp.y)
	{}
	//	XVec2& operator = (const XVec2& temp);
	XVec2& operator = (const float* temp);	//这种逻辑不严密
	XVec2(const float *temp) //这种逻辑不严密
	{
		if (temp == NULL)
		{
			x = y = 0.0f;
		}
		else
		{
			x = temp[0];
			y = temp[1];
		}
	}
	//-v
	XVec2 operator - () const;
	friend std::ostream& operator<<(std::ostream& os, const XVec2& vec)
	{
		os << vec.x << "," << vec.y;
		return os;
	}
	friend std::istream& operator >> (std::istream& is, XVec2& vec)
	{
		is >> vec.x;
		is.ignore(1);
		is >> vec.y;
		return is;
	}
	operator float* () const { return (float*) this; }
	operator const float* () const { return (const float*) this; }
	XVec2 rotation(float angle) const	//建议不要使用这个函数，而是使用旋转矩阵来实现
	{
		float c = cos(angle);
		float s = sin(angle);
		return XVec2(x * c - y * s, x * s + y * c);
	}
};

typedef XVec2 XPoint;

#if WITH_INLINE_FILE
#include "XVector2.inl"
#endif

inline XVec2 operator *(float a,const XVec2& temp)
{//左乘
	return temp * a;
}
//inline float operator^(const XVec2& v1,const XVec2& v2)  // DOT product
//{
//	return v1.x*v2.x + v1.y*v2.y;
//}
namespace XMath{
//计算线段AB和BC的夹角,0 - PI2,前进方向的左边的角
inline float angleBetween(const XVec2& A,const XVec2& B,const XVec2& C)
{//未经过测试
	XVec2 tmp1 = B - A;
	XVec2 tmp2 = C - B;
	return XMath::angleAbbreviation(atan2f(tmp1.cross(tmp2), tmp1.dot(tmp2)));
    //double sin = tmp1.cross(tmp2);
    //double cos = tmp1.dot(tmp2);
	//return XMath::angleAbbreviation(atan2f(sin, cos));
}
//计算线段AB和BC的夹角,0 - PI,前进方向的左边的角
inline float getAngle(const XVec2& A,const XVec2& B,const XVec2& C)
{
	XVec2 tmp1 = B - A;
	XVec2 tmp2 = C - B;
	return acos(tmp1.dot(tmp2) / sqrt(tmp1.getLengthSqure() * tmp2.getLengthSqure()));
}
inline XVec2 projector(const XVec2& v1,const XVec2& v2)
{//求v1向向量v2投影形成的向量
	return v2 * (v1.dot(v2) / v2.getLengthSqure());
}
inline XVec2 normalize(const XVec2& v)
{
	float r = v.getLengthSqure();
	if(r != 0) return v * (1.0f / sqrt(r));
	return XVec2::zero;
}
//angle单位弧度
//返回计算旋转之后坐标的参数
inline XVec2 getRotateRate(float angle)
{
	return XVec2(cos(angle), sin(angle));
}
//pos:位置
//RPos:区域的中心点
//w：区域的半宽
//h：区域的半高
inline bool isInArea(const XVec2& pos, const XVec2& RPos, float w, float h)
{
	return isInArea<float>(pos.x, RPos.x - w, RPos.x + w) &&
		isInArea<float>(pos.y, RPos.y - h, RPos.y + h);
}
}
}
#endif