#ifndef _JIA_XMATH_
#define _JIA_XMATH_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//这是一些基础的数学算法
#include "XOSDefine.h"
#include <math.h>

#define PI 3.1415926535897932384626433832795f
#define PI_HALF 1.5707963267948966192313216916395f								//(PI * 0.5)
#define PI2 6.283185307179586476925286766558f									//(PI * 2.0)
#define RADIAN2DEGREE	57.295779513082320876798154814114f						//(180.0 / PI)
#define DEGREE2RADIAN	0.01745329251994329576923690768488f						//(PI / 180.0)
#define SIN45 (0.70710678118654752440084436210485f)

#define XEE_Max(a,b) (((a) > (b)) ? (a) : (b))
#define XEE_Min(a,b) (((a) < (b)) ? (a) : (b))

//获取f的近似数据，近似精度为len
//f = 0.0123456 len = 2;
//返回 1.23 ex = 2
//也就是 0.0123456 = 1.23 * 10^-2 ,向下取整转换成科学计数法
inline float getApproximateData(float f,int len,int &ex)
{
	ex = 0;
	if(f == 0.0f || len <= 0 || len >= 6) return 0.0f;
	float tmp = pow(10.0f,len);
	if(int (f) != 0)
	{//带整数
		for(ex = 0;ex < 10;++ ex)
		{
			if(int(f) == 0)
			{
				ex = ex - 1;
				if(f < 0.0f) return ceil(f * tmp * 10.0f) / tmp;
				else return floor(f * tmp * 10.0f) / tmp;
			}
			f *= 0.1f;
		}
	}else
	{//纯小数
		for(ex = 0;ex < 10;++ ex)
		{
			if(int(f) != 0)
			{
				ex = -ex;
				if(f < 0.0f) return ceil(f * tmp) / tmp;
				else return floor(f * tmp) / tmp;
			}
			f *= 10.0f;
		}
	}
	return 0.0f;
}
//下面是一个sin的优化算法
inline double sinTay(double x)
{
    double x2 = x * x;
 
    const static double a1 = 0.99999748719198;
    const static double a3 = -0.166651680984443;
    const static double a4 = 0.00830951691334793;
    const static double a5 = -0.000184472198573026;
    return x*( a1+ ( a3 + ( a4 + a5*x2 ) * x2 ) * x2);  
}
inline double cosTay(double x) 
{
    double cos = 0.0;
    //compute cosine: sin(x + PI/2) = cos(x)
    x += 1.57079632;
    if(x > 3.14159265) x -= 6.28318531;
    if(x < 0) 
	{
        cos = 1.27323954 * x + 0.405284735 * x * x;
        if(cos < 0) cos = 0.225 * (cos *-cos - cos) + cos;
        else cos = 0.225 * (cos * cos - cos) + cos;
    }else
    {
        cos = 1.27323954 * x - 0.405284735 * x * x;
        if(cos < 0) cos = 0.225 * (cos *-cos - cos) + cos;
        else cos = 0.225 * (cos * cos - cos) + cos;
    }
    return cos;
}
//四舍五入取整
inline int toInt(float f) {return int(f + 0.5f);}
//向下取整
inline int toIntFloor(float f) {return floorf(f);}
//向上取整
inline int toIntCeil(float f) {return ceilf(f);}

//角度化简
extern float getAngleXY(float x,float y);	//用于获得x,y构成的角度
inline float angleAbbreviation(float angle)	//弧度角度化简
{
	angle = fmod(angle,PI2);
	if(angle < 0.0f) angle += (float)(PI2);
//	while(true)
//	{
//		if(angle >= PI2) angle -= (float)(PI2);
//		else if(angle < 0) angle += (float)(PI2);
//		else break;
//	}
	return angle;
}
inline float angleReduce(float angle)		//角度简化单位:度
{
	angle = fmod(angle,360.0f);
	if(angle < 0.0f) angle += 360.0f;
	return angle;
}
inline float maping1D(float in,float imin,float imax,float omin,float omax)//一维映射
{
	//if(fabs(xmax - xmin) < FLT_EPSILON) return ymin;		//数据非法
	if(fabs(imax - imin) < 1.192092896e-07F) return omin;	//数据非法
	float ret = (in - imin) / (imax - imin) * (omax - omin) + omin;
	if(ret < omin) return omin;
	if(ret > omax) return omax;
	return ret;
}
class _XVector2;
//0   1
//3   2
//注意:需要尽量保证输入输出都为凸多边形，否则会造成问题。关于凹多边形需要再仔细考虑
extern _XVector2 maping2D(const _XVector2 *inRect,const _XVector2 *outRect,const _XVector2 &in);

template<typename T> 
T lineSlerp(const T &s,const T &e,float r)//线性插值，s:起点，e:终点，r：百分比[0 - 1.0]
{
	return e * r + s * (1.0f  - r);
}
//插值 x[0 - 1]
template<typename T> 
T cSlerp(const T &u0,const T &u1,const T &u2,const T &u3,float x)
{
	T p = (u3 - u2) - (u0 - u1);
	T q = (u0 - u1) - p;
	T r = u2 - u0;
	return x * (x * (x * p + q) + r) + u1;
}
///////////////////////////////////////////////////////////
inline int getMinWellSize2n(int temp)			//寻找比temp大的最小的符合2的n次方的尺寸
{
	return (int)powf(2.0, ceilf(logf((float)temp)/logf(2.0f)));
	//int i = 1;
	//while(true)
	//{
	//	if(i > temp) break;
	//	i = i << 1;
	//}
	//return i;
}
inline _XBool isNPOT(int width, int height)		//判断是否不符合2的n次方
{
	if((int)powf(2.0f,ceilf(logf((float)width)/logf(2.0f))) != width) return XTrue;
	if((int)powf(2.0f,ceilf(logf((float)height)/logf(2.0f))) != height) return XTrue;
	return XFalse;
}

class _XLine;
class _XRect;

extern void vector4Sort(_XVector2* v,_XVector2* u);//对任意序列的无旋转矩形进行顺时针排序
//4个点必须要是已经排序了的
extern int getEdgePoint(float angle,_XVector2 * points,const _XVector2* v,const _XVector2* u);	//只能以中心点
//获取一个矩形内的点与矩形的边在指定角度下围成的多边形(下面的方法可以设定以哪个点)
class _XRect;
extern int getEdgePointEx(_XRect &rect,_XVector2 &point,float angle,_XVector2 * points);	//角度为12点钟方向开始逆时针[0 - PI2]
extern int getEdgePointEx(_XRect &vRect,_XRect &uRect,_XVector2 &vPoint,float angle,_XVector2 * vPoints,_XVector2 * uPoints);	//角度为12点钟方向开始逆时针[0 - PI2]
extern int getEdgePointEx(_XRect &rect,_XVector2 &point,float angle,float * points);
extern int getEdgePointEx(_XRect &vRect,_XRect &uRect,_XVector2 &vPoint,float angle,float * vPoints,float * uPoints);	//角度为12点钟方向开始逆时针[0 - PI2]

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
extern _XBool twoPointLine(const _XVector2& lineS,const _XVector2& lineE,double *a,double *b,double *c);//(x-x1)/(x2-x1)=(y-y1)/(y2-y1)
//点到线段的最短距离
extern double minDistancePointToLine(const _XVector2& point,const _XVector2& lineS,const _XVector2& lineE);
//两线段的最短距离
extern _XVector2 minDistanceTwoLine(const _XVector2& line1S,const _XVector2& line1E,const _XVector2& line2S,const _XVector2& line2E);
//获取p是否在p1 p2 p3 p4围成的四边形内部
extern _XBool getIsInRect(float x,float y,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,const _XVector2& p4);
extern _XBool getIsInRect(const _XVector2& p,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,const _XVector2& p4);
//判断点p是否在pR所围成的多边形内部
extern _XBool getIsInRect(const _XVector2& p,const _XVector2 *pR,int pRSum);
extern _XBool getIsInRect(float x,float y,const _XVector2 *pR,int pRSum);

#include  <complex>
#include  <iostream>
#include  <math.h>
using namespace std;
//所调用的四次方程的函数的声明，返回有效解的数量，以及带复数部分的解
extern int quarticEquation(
					  double a,double b,double c,double d,double e,
					  complex<double> &x1,complex<double> &x2,
					  complex<double> &x3,complex<double> &x4
					  );
//上面函数的再封装，返回不带复数部分的解
extern int quarticEquation(
					  double a,double b,double c,double d,double e,
					  double *x);
//计算大小为s的方阵的值，data为方正的数据
//0,1,2
//3,4,5
//6,7,8
extern double getValueMatrix(const double *d,int s);
//下面是CRC16计算
//p为字符串
//len为字符串长度
extern unsigned short CRC16_Modbus(unsigned char *p,unsigned short len);
extern unsigned short CRC16_XModen(unsigned char *buf,unsigned short length);
#endif