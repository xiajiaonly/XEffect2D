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
class XVector2
{
public:
    float x;
    float y;
public:
	static const XVector2 zero;

    void reset();    //归零这个点
	void set(const XVector2 &temp);
    void set(float a,float b);    //设置当前点的值
	void set(float v) {set(v,v);}
    void add(float a,float b);    //加上某个值

    XVector2 operator + (const XVector2& temp) const;
    XVector2 operator - (const XVector2& temp) const;
	//下面这两个定义在数学上没有意义，只是程序上的表达
    XVector2 operator * (const XVector2& temp) const;	
    XVector2 operator / (const XVector2& temp) const;
    XVector2 operator + (const float& temp) const;
    XVector2 operator - (const float& temp) const;
 	//下面这两个定义在数学上没有意义，只是程序上的表达
	XVector2 operator * (const float& temp) const;
    XVector2 operator / (const float& temp) const;
    void operator += (const XVector2& temp);
    void operator -= (const XVector2& temp);
    void operator *= (const XVector2& temp);
    void operator /= (const XVector2& temp);
    void operator += (const float & temp);
    void operator -= (const float & temp);
    void operator *= (const float & temp);
    void operator /= (const float & temp);

//	XVector2& operator = (const XVector2I & temp);
//	XVector2(const XVector2I & temp);

    float getLength(const XVector2& P0) const;     //获取两个点之间的距离
    float getLength(float a,float b) const;			//获取两个点之间的距离
	float getLength() const;						//获得到原点的距离
	float getLengthSqure(const XVector2& P0) const;//获取两点之间距离的平方
	float getLengthSqure(float a,float b) const;
	float getLengthSqure() const;					//获得到原点的距离的平方
    float getAngle(const XVector2& P0) const;      //获取从当前点指向目标点的角度
    float getAngle(float a,float b) const;          //获取从当前点指向目标点的角度
	float getAngle() const;							//获取原点到当前点的角度

	float dot(const XVector2 &temp) const {return (x * temp.x + y * temp.y);}
	//点乘，也叫向量的内积、数量积。顾名思义，求下来的结果是一个数。 
	//点积： (x1 , y1 , z1 ) .( x2 , y2 , z2 ) = x1x2 + y1y2 + z1z2 
	//点积可以来计算两矢量的夹角,公式如下: 
	//cos (V dot W) =V.W / | V | | W | 
	//点乘的几何意义是：是一条边向另一条边的投影乘以另一条边的长度
	float cross(const XVector2 &temp)const{return x * temp.y - y * temp.x;}	//CROSS product
	void normalize();	//单位化 或者 归一化
	XVector2 perp(){return XVector2(y,-x);}	//反射
    
    XVector2::XVector2(float a,float b)
		:x(a),y(b)
	{}
	XVector2::XVector2()
		:x(0.0f),y(0.0f)
	{}
	XVector2(const XVector2 &temp)
		:x(temp.x),y(temp.y)
	{}
//	XVector2 &operator = (const XVector2 &temp);
	XVector2 &operator = (const float* temp);	//这种逻辑不严密
	XVector2(const float *temp) //这种逻辑不严密
	{
		if(temp == NULL)
		{
			x = 0.0f;
			y = 0.0f;
		}else
		{
			x = temp[0];
			y = temp[1];
		}
	}
	//-v
	XVector2 operator - () const;
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}
};

typedef XVector2 XPoint;

#if WITH_INLINE_FILE
#include "XVector2.inl"
#endif

inline XVector2 operator *(float a,const XVector2 &temp)
{//左乘
	return temp * a;
}
//inline float operator^(const XVector2 &v1,const XVector2 &v2)  // DOT product
//{
//	return v1.x*v2.x + v1.y*v2.y;
//}
namespace XMath{
//计算线段AB和BC的夹角,0 - PI2,前进方向的左边的角
inline float angleBetween(const XVector2 &A,const XVector2 &B,const XVector2 &C)
{//未经过测试
	XVector2 tmp1 = B - A;
	XVector2 tmp2 = C - B;
    double sin = tmp1.x * tmp2.y - tmp2.x * tmp1.y;  
    double cos = tmp1.x * tmp2.x + tmp1.y * tmp2.y;
	return XMath::angleAbbreviation(atan2(sin, cos));
}
//计算线段AB和BC的夹角,0 - PI,前进方向的左边的角
inline float getAngle(const XVector2 &A,const XVector2 &B,const XVector2 &C)
{
	return acos(((B - A).dot(C - B))/sqrt((B - A).getLengthSqure() * (C - B).getLengthSqure()));
}
inline XVector2 projector(const XVector2 &v1,const XVector2 &v2)
{//求v1向向量v2投影形成的向量
	return v2 * (v1.dot(v2) / v2.getLengthSqure());
}
inline XVector2 normalize(const XVector2 &v)
{
	float r = v.getLengthSqure();
	if(r != 0) return v * (1.0f / sqrt(r));
	return XVector2::zero;
}
}
}
#endif