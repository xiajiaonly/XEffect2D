#ifndef _JIA_XVECTOR2_
#define _JIA_XVECTOR2_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "XBasicFun.h"
//#include <math.h>
//#include "stdio.h"

//单精度2D点
class _XVector2
{
public:
    float x;
    float y;
public:
	static const _XVector2 zero;

    void reset();    //归零这个点
	void set(const _XVector2 &temp);
    void set(float a,float b);    //设置当前点的值
	void set(float v) {set(v,v);}
    void add(float a,float b);    //加上某个值

    _XVector2 operator + (const _XVector2& temp) const;
    _XVector2 operator - (const _XVector2& temp) const;
	//下面这两个定义在数学上没有意义，只是程序上的表达
    _XVector2 operator * (const _XVector2& temp) const;	
    _XVector2 operator / (const _XVector2& temp) const;
    _XVector2 operator + (const float& temp) const;
    _XVector2 operator - (const float& temp) const;
 	//下面这两个定义在数学上没有意义，只是程序上的表达
   _XVector2 operator * (const float& temp) const;
    _XVector2 operator / (const float& temp) const;
    void operator += (const _XVector2& temp);
    void operator -= (const _XVector2& temp);
    void operator *= (const _XVector2& temp);
    void operator /= (const _XVector2& temp);
    void operator += (const float & temp);
    void operator -= (const float & temp);
    void operator *= (const float & temp);
    void operator /= (const float & temp);

//	_XVector2& operator = (const _XVector2I & temp);
//	_XVector2(const _XVector2I & temp);

    float getLength(const _XVector2& P0) const;     //获取两个点之间的距离
    float getLength(float a,float b) const;			//获取两个点之间的距离
	float getLength() const;						//获得到原点的距离
	float getLengthSqure(const _XVector2& P0) const;//获取两点之间距离的平方
	float getLengthSqure(float a,float b) const;
	float getLengthSqure() const;					//获得到原点的距离的平方
    float getAngle(const _XVector2& P0) const;      //获取从当前点指向目标点的角度
    float getAngle(float a,float b) const;          //获取从当前点指向目标点的角度
	float getAngle() const;							//获取原点到当前点的角度

	float dot(const _XVector2 &temp) const {return (x * temp.x + y * temp.y);}
	//点乘，也叫向量的内积、数量积。顾名思义，求下来的结果是一个数。 
	//点积： (x1 , y1 , z1 ) .( x2 , y2 , z2 ) = x1x2 + y1y2 + z1z2 
	//点积可以来计算两矢量的夹角,公式如下: 
	//cos (V dot W) =V.W / | V | | W | 
	//点乘的几何意义是：是一条边向另一条边的投影乘以另一条边的长度
	void normalize();	//单位化 或者 归一化
	_XVector2 perp(){return _XVector2(y,-x);}	//反射
    
    _XVector2::_XVector2(float a,float b)
		:x(a),y(b)
	{}
	_XVector2::_XVector2()
		:x(0.0f),y(0.0f)
	{}
	_XVector2(const _XVector2 &temp)
		:x(temp.x),y(temp.y)
	{}
//	_XVector2 &operator = (const _XVector2 &temp);
	_XVector2 &operator = (const float* temp);	//这种逻辑不严密
	_XVector2(const float *temp) //这种逻辑不严密
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
	_XVector2 operator - () const;
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}
};

typedef _XVector2 _XPoint;
#include "XVector2.inl"

inline _XVector2 operator *(float a,const _XVector2 &temp)
{//左乘
	return temp * a;
}
//inline float operator^(const _XVector2 &v1,const _XVector2 &v2)  // DOT product
//{
//	return v1.x*v2.x + v1.y*v2.y;
//}
//计算线段AB和BC的夹角,0 - PI,前进方向的左边的角
inline float getAngle(const _XVector2 &A,const _XVector2 &B,const _XVector2 &C)
{
	return acos(((B - A).dot(C - B))/sqrt((B - A).getLengthSqure() * (C - B).getLengthSqure()));
}
inline _XVector2 projector(const _XVector2 &v1,const _XVector2 &v2)
{//求v1向向量v2投影形成的向量
	return v2 * (v1.dot(v2) / v2.getLengthSqure());
}
#endif