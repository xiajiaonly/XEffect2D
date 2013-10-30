#ifndef _JIA_XBASICCLASS_
#define _JIA_XBASICCLASS_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:        2011.4.9
//--------------------------------
#include "XBasicFun.h"

class _XVector2I;

//单精度2D点
class _XVector2
{
public:
    float x;
    float y;

public:
    void reset();    //归零这个点
	void set(const _XVector2 &temp);
    void set(float a,float b);    //设置当前点的值
    void add(float a,float b);    //加上某个值

    _XVector2 operator + (const _XVector2& temp) const;
    _XVector2 operator - (const _XVector2& temp) const;
    _XVector2 operator * (const _XVector2& temp) const;
    _XVector2 operator / (const _XVector2& temp) const;
    _XVector2 operator + (const float& temp) const;
    _XVector2 operator - (const float& temp) const;
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

	_XVector2& operator = (const _XVector2I & temp);
	_XVector2(const _XVector2I & temp);

    float getLength(const _XVector2& P0) const;        //获取两个点之间的距离
    float getLength(float a,float b) const;		//获取两个点之间的距离
	float getLengthSqure(const _XVector2& P0) const;
	float getLengthSqure(float a,float b) const;
	float getLengthSqure() const;	//获得到原点的距离的平方
	float getLength() const;		//获得到原点的距离
    float getAngle(const _XVector2& P0) const;            //获取从当前点指向目标点的角度
    float getAngle(float a,float b) const;            //获取从当前点指向目标点的角度
	float getAngle() const;
    
    _XVector2(float a,float b);
    _XVector2();
};

//线段的类
class _XLine
{
private:
	_XVector2 m_startPosition;	//线段的起始点
	_XVector2 m_endPosition;	//线段的终点
	char m_isEnable;			//是否有效
public:
	double A,B,C;	//Ax + By + C = 0;
	double m_data0;	//A^2 + B^2
	double m_data1;	//sqrt(A^2 + B^2)
	double m_angle;
public:
	void set(const _XVector2 &startPosition,const _XVector2 &endPosition);
	void set(float startX,float startY,float endX,float endY);
	float getLineLength() const;	//获得线段的长度
	float getLineAngle() const;	//获得线段的角度
	_XVector2 getStartPosition() const;	//获得线段的起始位置
	_XVector2 getEndPosition() const;		//获得线段的终点位置
	void setStartPosition(const _XVector2& startPosition);
	void setStartPosition(float x,float y);
	void setEndPosition(const _XVector2& endPosition);
	void setEndPosition(float x,float y);
	int getIsEnable() const
	{
		return m_isEnable;
	}
	void setEnable(int temp)
	{
		if(temp == 0) m_isEnable = 0;
		else m_isEnable = 1;
	}
	_XLine()
	:m_isEnable(0)
	,A(0.0)
	,B(0.0)
	,C(0.0)
	,m_data0(0.0)
	,m_data1(0.0)
	,m_angle(0.0)
	,m_startPosition(0.0f,0.0f)
	,m_endPosition(1.0f,1.0f)
	{
	}
};

//+++++++匀速直线运动下的一组函数++++++++
//返回值 -2参数错误 -1不会发生碰撞 ret > 0
//匀速直线运动两球碰撞时间的计算
extern double getTwoLineTime(const _XVector2& L1S,const _XVector2& L1E,const _XVector2& L2S,const _XVector2& L2E,float distance,float timer);	//获取两条线段某个距离的最小解
//匀速直线运动球与固定点的碰撞时间计算
extern double getLineToPointTime(const _XVector2& L1S,const _XVector2& L1E,const _XVector2& point,float distance,float timer);
//匀速直线运动球与固定线段的碰撞时间计算
extern double getObjectToLineTime(const _XVector2& L1S,const _XVector2& L1E,const _XVector2& L2S,const _XVector2& L2E,float distance,float timer);	//获取两条线段某个距离的最小解
//运动球体与运动线段的碰撞时间计算
//运动线段与运动线段的碰撞时间计算
//+++++++匀加速直线运动的一组函数++++++++
//匀加速直线运动两物体碰撞时间的计算(时间单位统一为ms)
extern double getCrashTimerB2B(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,
							   const _XVector2& position2,const _XVector2& speed2,const _XVector2& a2,float distance,float time);
//匀加速直线运动物体与固定点碰撞的时间计算
extern double getCrashTimerB2P(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,
							   const _XVector2& position2,float distance,float time);
//匀加速直线运动物体与固定直线碰撞的时间计算
extern double getCrashTimerB2L(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,
							   const _XVector2& positionS,const _XVector2& positionE,float distance,float time);
//很奇怪下面这行对_XLine使用const说明居然会出错，不知道为什么。
extern double getCrashTimerB2L(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,const _XLine& pL,float distance,float time);
//extern double getCrashTimerB2L(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,_XLine& pL,float distance,float time);
extern void printfR();
//下面描写一些基本的公式
//点到直线距离的公式
extern double distancePointToLine(const _XVector2& point,const _XVector2& lineS,const _XVector2& lineE);//ax+by+c=0 x0,y0 |ax0+by0+c|/√(a^2+b^2)
extern double distancePointToLine(const _XVector2& point,double a,double b,double c);
//点到直线的垂足公式
extern _XVector2 curtatePointToLine(const _XVector2& point,const _XVector2& lineS,const _XVector2& lineE);	//设直线方程为ax+by+c=0,点坐标为(m,n)则垂足为((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
extern _XVector2 curtatePointToLine(const _XVector2& point,double a,double b,double c);
//两直线交点的公式
extern _XVector2 crossPointTineToLine(const _XVector2& line1S,const _XVector2& line1E,const _XVector2& line2S,const _XVector2& line2E);
extern _XVector2 crossPointTineToLine(double a0,double b0,double c0,double a1,double b1,double c1);
//两点直线公式
extern int twoPointLine(const _XVector2& lineS,const _XVector2& lineE,double *a,double *b,double *c);//(x-x1)/(x2-x1)=(y-y1)/(y2-y1)
//点到线段的最短距离
extern double minDistancePointToLine(const _XVector2& point,const _XVector2& lineS,const _XVector2& lineE);
//两线段的最短距离
extern _XVector2 minDistanceTwoLine(const _XVector2& line1S,const _XVector2& line1E,const _XVector2& line2S,const _XVector2& line2E);
//获取p是否在p1 p2 p3 p4围成的四边形内部
extern int getIsInRect(const _XVector2& p,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,const _XVector2& p4);
//判断点p是否在pR所围成的多边形内部
extern int getIsInRect(const _XVector2& p,const _XVector2 *pR,int pRSum);

//单精度3D点
class _XVector3
{
public:
    float x;
    float y;
    float z;

public:
    void reset();//重置这个点
    void add(float a,float b,float c);//设置这个点的值
    void set(float a,float b,float c);//这个点的值加上一个点的值
    _XVector3(float a,float b,float c);    
    _XVector3();
};

//单精度矩形
class _XRect
{
public:
    float left;
    float top;
    float right;
    float bottom;
public:
    void set(float xl =0,float yt =0,float xr =0,float yb =0);
    void set(const _XVector2& leftTop,const _XVector2& rightBottom);
    bool setLeft(float xl);
    bool setRight(float xr);
    bool setTop(float yt);
    bool setBottom(float yb);
    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
    _XRect(float xl =0.0f,float yt =0.0f,float xr =0.0f,float yb =0.0f);
    _XRect(const _XVector2& leftTop,const _XVector2& rightBottom);
    //_XRect()
    //    :left(0.0f)
    //    ,top(0.0f)
    //    ,right(0.0f)
    //    ,bottom(0.0f)
    //{
    //}
	//注意下面的函数式针对于不会旋转的矩形的碰撞判断，如果是旋转的需要进一步修改代码
    bool isCrash(const _XRect& R0) const;
    bool isCrash(const _XVector2& leftTop,const _XVector2& rightBottom) const;
    bool isCrash(float xl,float yt,float xr,float yb) const;
    bool isInRect(const _XVector2& p0) const;
    bool isInRect(float x,float y) const;
    float getArea() const;
    _XVector2 getCenter() const;
	float getXCenter() const {return (left + right) * 0.5f;}
	float getYCenter() const {return (top + bottom) * 0.5f;}
    float getWidth() const;
    float getHeight() const;
    void setCenter(float x,float y);
    void setCenter(const _XVector2& p);
};

//整形2D点，考虑到整形数的运算速度较快，所以才特别定义的
class _XVector2I
{
public:
    int x;
    int y;

public:
    void set(int a,int b);

    float getLength(const _XVector2I& P0) const;
    float getAngle(const _XVector2I& P0) const;

	_XVector2I& operator = (const _XVector2 & temp);
	_XVector2I(const _XVector2 & temp);

    _XVector2I(int a,int b);
    _XVector2I();
};

//整形矩形
class _XIRect
{
private:
public:
    int left;
    int top;
    int right;
    int bottom;
public:
    void set(int xl =0,int yt =0,int xr =0,int yb =0);

    bool isCrash(_XIRect& R0);

    _XIRect(int xl =0,int yt =0,int xr =0,int yb =0);
    _XIRect(const _XVector2I& p0,const _XVector2I& p1);
   //_XIRect()
   //     :left(0)
   //     ,top(0)
   //     ,right(0)
   //     ,bottom(0)
   // {
   // }
    void set(const _XVector2I& p0,const _XVector2I& p1);
    void set(const _XVector2& p0,const _XVector2& p1);
    bool isInRect(const _XVector2I& p0) const;
    int getArea() const;
    _XVector2 getCenter() const;
    int getWidth() const;
    int getHeight() const;
    void setCenter(int x,int y);
    void setCenter(const _XVector2I& p);
};

//单字节的颜色 0 - 255
class _XCColor
{
public:
    unsigned char cR;    //0 - 255
    unsigned char cG;
    unsigned char cB;
    unsigned char cA;
    _XCColor()
        :cR(0)
        ,cG(0)
        ,cB(0)
        ,cA(0)
    {
    }
    void setColor(unsigned char r,unsigned char g,unsigned char b,unsigned char a);
};

//浮点数的颜色 0.0 - 1.0
class _XFColor
{
//private:
public:
    float fR;    //0 - 1
    float fG;
    float fB;
    float fA;
public:
    _XFColor()
        :fR(0.0f)
        ,fG(0.0f)
        ,fB(0.0f)
        ,fA(0.0f)
    {
    }
	_XFColor(float r,float g,float b,float a)
		:fR(r)
        ,fG(g)
        ,fB(b)
        ,fA(a)
	{
		if(r < 0.0f) r = 0;
		if(r > 1.0f) r = 1.0f;
		if(g < 0.0f) g = 0;
		if(g > 1.0f) g = 1.0f;
		if(b < 0.0f) b = 0;
		if(b > 1.0f) b = 1.0f;
		if(a < 0.0f) a = 0;
		if(a > 1.0f) a = 1.0f;
	}
    void setColor(float r,float g,float b,float a);
	_XFColor operator * (const _XFColor& temp) const
	{
		return _XFColor(fR * temp.fR,fG * temp.fG,fB * temp.fB,fA * temp.fA);
	}
	void setR(float r)
	{
		if(r < 0.0f) r = 0;
		if(r > 1.0f) r = 1.0f;
		fR = r;
	}
	void setG(float g)
	{
		if(g < 0.0f) g = 0;
		if(g > 1.0f) g = 1.0f;
		fG = g;
	}
	void setB(float b)
	{
		if(b < 0.0f) b = 0;
		if(b > 1.0f) b = 1.0f;
		fB = b;
	}
	void setA(float a)
	{
		if(a < 0.0f) a = 0;
		if(a > 1.0f) a = 1.0f;
		fA = a;
	}
	float getR() const {return fR;}
	float getG() const {return fG;}
	float getB() const {return fB;}
	float getA() const {return fA;}
};

#include "XOSDefine.h"

#ifdef OS_WINDOWS
#include "windows.h"

//下面的类用于线程互斥
class _XCritical 
{ 
public: 
    _XCritical(); 
    ~_XCritical();
    void Lock();

    void Unlock();

private: 
    CRITICAL_SECTION m_sec; 
};

typedef DWORD pthread_t;    
typedef _XCritical pthread_mutex_t;

extern void pthread_mutex_init(_XCritical &temp,int sum);
extern void pthread_mutex_lock(_XCritical &temp);
extern void pthread_mutex_unlock(_XCritical &temp);
extern void close(int temp);
extern int initSocket();
#endif

#include  <complex>
#include  <iostream>
#include  <math.h>
using   namespace   std;

//所调用的四次方程的函数的声明，返回有效接的数量，以及带复数部分的解
extern int quarticEquation(
					  double a,double b,double c,double d,double e,
					  complex<double> &x1,complex<double> &x2,
					  complex<double> &x3,complex<double> &x4
					  );
//上面函数的再封装，返回不带复数部分的解
extern int quarticEquation(
					  double a,double b,double c,double d,double e,
					  double *x);

#include "XBasicClass.inl"
#endif