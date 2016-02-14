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
#include <complex>
#include <iostream>
#include <math.h>
namespace XE{
class XVector2;
#define PI (3.1415926535897932384626433832795f)
#define PI_HALF (1.5707963267948966192313216916395f)							//(PI * 0.5)
#define PI2 (6.283185307179586476925286766558f)									//(PI * 2.0)
#define RADIAN2DEGREE (57.295779513082320876798154814114f)						//(180.0 / PI)
#define DEGREE2RADIAN (0.01745329251994329576923690768488f)						//(PI / 180.0)
#define SIN45 (0.70710678118654752440084436210485f)
#define SQRT3 (1.7320508075688772935274463415059f)								//(3 ^ 0.5)
#define SQRT2 (1.4142135623730950488016887242097f)								//(2 ^ 0.5)

#define XEE_Max(a,b) (((a) > (b)) ? (a) : (b))
#define XEE_Min(a,b) (((a) < (b)) ? (a) : (b))

class XLine;
class XRect;
namespace XMath
{
	//获取f的近似数据，近似精度为len
	//f = 0.0123456 len = 2;
	//返回 1.23 ex = 2
	//也就是 0.0123456 = 1.23 * 10^-2 ,向下取整转换成科学计数法
	extern float getApproximateData(float f,int len,int &ex);
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
	//双线性插值的计算公式
	inline float bilinear(float a,float b,float c,float d,float rate0,float rate1)
	{
		return a * (1.0f - rate0) * (1.0f - rate1) + (1.0f - rate0) * rate1 * b
			+ rate0 * (1.0f - rate1) * c + rate0 * rate1 * d;
	}
	//四舍五入取整
	inline int toInt(float f) {return int(f + 0.5f);}
	//向下取整
	inline int toIntFloor(float f) {return (int)floorf(f);}
	//向上取整
	inline int toIntCeil(float f) {return (int)ceilf(f);}
	//需要进行适应缩放时，这里计算最小的缩放比例
	extern float getMinRate(const XVector2 &a,const XVector2 &b);
	//角度化简
	extern float getAngleXY(float x,float y);	//用于获得x,y构成的角度
	inline float angleAbbreviation(float angle)	//弧度角度化简
	{
		angle = fmod(angle,PI2);
		if(angle < 0.0f) angle += (float)(PI2);
		return angle;
	}
	inline float angleReduce(float angle)		//角度简化单位:度
	{
		angle = fmod(angle,360.0f);
		if(angle < 0.0f) angle += 360.0f;
		return angle;
	}
	inline float clamp(float value, float min, float max) 
	{
		return value < min ? min : value > max ? max : value;
	}
	//将数据in从imin到imax空间映射到omin到omax空间。
	inline float maping1D(float in,float imin,float imax,float omin,float omax)//一维映射
	{
		//if(fabs(xmax - xmin) < FLT_EPSILON) return ymin;		//数据非法
		if(fabs(imax - imin) < 1.192092896e-07F) return omin;	//数据非法
		//方案1
		//float ret = (in - imin) / (imax - imin) * (omax - omin) + omin;
		//if(ret < omin) return omin;
		//if(ret > omax) return omax;
		//return ret;
		//方案2
		if(in <= imin) return omin;
		if(in >= imax) return omax;
		return (in - imin) / (imax - imin) * (omax - omin) + omin;
	}
	//相对于maping1D，不做夹值，是超范围限制的
	inline float maping1DEx(float in,float imin,float imax,float omin,float omax)//一维映射
	{
		if(fabs(imax - imin) < 1.192092896e-07F) return omin;	//数据非法
		return (in - imin) / (imax - imin) * (omax - omin) + omin;
	}
	template<typename T> 
	bool isInArea(T value,T x,T y)//判断value是否在x和y的范围内
	{
		if(x < y) return (value >= x && value <= y);
		else return (value >= y && value <= x);
	}
	//0   1
	//3   2
	//注意:需要尽量保证输入输出都为凸多边形，否则会造成问题。关于凹多边形需要再仔细考虑
	//这个函数用于处理两个2D空间中的空间映射问题
	extern XVector2 maping2D(const XVector2 *inRect,const XVector2 *outRect,const XVector2 &in);
	//四边形向量场插值，由四个角点向量构成的向量场，计算其中某点的向量，使用线性插值
	extern XVector2 vectorFieldLineSlerp(const XVector2 *posRect,const XVector2 *vectorRect,const XVector2 &in);
	//四边形标量场插值,在2D空间的一个标量场，已知四边形角点的标量值，计算四边形内部任意点的标量值使用线性插值
	extern float scalarFieldLineSlerp(const XVector2 *posRect,const float *scalarRect,const XVector2 &in); 
	//线性插值，s:起点，e:终点，r：百分比[0 - 1.0]
	template<typename T> 
	T lineSlerp(const T &s,const T &e,float r) {return e * r + s * (1.0f  - r);}
	//插值 x[0 - 1]
	template<typename T> 
	T cSlerp(const T &u0,const T &u1,const T &u2,const T &u3,float x)
	{
		T p = (u3 - u2) - (u0 - u1);
		T q = (u0 - u1) - p;
		T r = u2 - u0;
		return x * (x * (x * p + q) + r) + u1;
	}
	//寻找比temp大的最小的符合2的n次方的尺寸
	inline int getMinWellSize2n(int temp) {return (int)powf(2.0, ceilf(logf((float)temp)/logf(2.0f)));}
	inline XBool isNPOT(int width, int height)		//判断是否不符合2的n次方
	{
		if(getMinWellSize2n(width) != width) return XTrue;
		if(getMinWellSize2n(height) != height) return XTrue;
		return XFalse;
	}
	extern void vector4Sort(XVector2* v,XVector2* u);//对任意序列的无旋转矩形进行顺时针排序
	//4个点必须要是已经排序了的
	extern int getEdgePoint(float angle,XVector2 * points,const XVector2* v,const XVector2* u);	//只能以中心点
	//获取一个矩形内的点与矩形的边在指定角度下围成的多边形(下面的方法可以设定以哪个点)
	extern int getEdgePointEx(XRect &rect,XVector2 &point,float angle,XVector2 * points);	//角度为12点钟方向开始逆时针[0 - PI2]
	extern int getEdgePointEx(XRect &vRect,XRect &uRect,XVector2 &vPoint,float angle,XVector2 * vPoints,XVector2 * uPoints);	//角度为12点钟方向开始逆时针[0 - PI2]
	extern int getEdgePointEx(XRect &rect,XVector2 &point,float angle,float * points);
	extern int getEdgePointEx(XRect &vRect,XRect &uRect,XVector2 &vPoint,float angle,float * vPoints,float * uPoints);	//角度为12点钟方向开始逆时针[0 - PI2]
	//+++++++匀速直线运动下的一组函数++++++++
	//返回值 -2参数错误 -1不会发生碰撞 ret > 0
	//匀速直线运动两球碰撞时间的计算
	extern double getTwoLineTime(const XVector2& L1S,const XVector2& L1E,const XVector2& L2S,const XVector2& L2E,float distance,float timer);	//获取两条线段某个距离的最小解
	//匀速直线运动球与固定点的碰撞时间计算
	extern double getLineToPointTime(const XVector2& L1S,const XVector2& L1E,const XVector2& point,float distance,float timer);
	//匀速直线运动球与固定线段的碰撞时间计算
	extern double getObjectToLineTime(const XVector2& L1S,const XVector2& L1E,const XVector2& L2S,const XVector2& L2E,float distance,float timer);	//获取两条线段某个距离的最小解
	//运动球体与运动线段的碰撞时间计算
	//运动线段与运动线段的碰撞时间计算
	//+++++++匀加速直线运动的一组函数++++++++
	//匀加速直线运动两物体碰撞时间的计算(时间单位统一为ms)
	extern double getCrashTimerB2B(const XVector2& position1,const XVector2& speed1,const XVector2& a1,
								   const XVector2& position2,const XVector2& speed2,const XVector2& a2,float distance,float time);
	//匀加速直线运动物体与固定点碰撞的时间计算
	extern double getCrashTimerB2P(const XVector2& position1,const XVector2& speed1,const XVector2& a1,
								   const XVector2& position2,float distance,float time);
	//匀加速直线运动物体与固定直线碰撞的时间计算
	extern double getCrashTimerB2L(const XVector2& position1,const XVector2& speed1,const XVector2& a1,
								   const XVector2& positionS,const XVector2& positionE,float distance,float time);
	//很奇怪下面这行对XLine使用const说明居然会出错，不知道为什么。
	extern double getCrashTimerB2L(const XVector2& position1,const XVector2& speed1,const XVector2& a1,const XLine& pL,float distance,float time);
	//extern double getCrashTimerB2L(const XVector2& position1,const XVector2& speed1,const XVector2& a1,XLine& pL,float distance,float time);
	extern void printfR();
	//下面描写一些基本的公式
	//点到直线距离的公式
	extern double distancePointToLine(const XVector2& point,const XVector2& lineS,const XVector2& lineE);//ax+by+c=0 x0,y0 |ax0+by0+c|/√(a^2+b^2)
	extern double distancePointToLine(const XVector2& point,double a,double b,double c);
	//点到直线的垂足公式
	extern XVector2 curtatePointToLine(const XVector2& point,const XVector2& lineS,const XVector2& lineE);	//设直线方程为ax+by+c=0,点坐标为(m,n)则垂足为((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
	extern XVector2 curtatePointToLine(const XVector2& point,double a,double b,double c);
	//两直线交点的公式
	extern XVector2 crossPointTineToLine(const XVector2& line1S,const XVector2& line1E,const XVector2& line2S,const XVector2& line2E);
	extern XVector2 crossPointTineToLine(double a0,double b0,double c0,double a1,double b1,double c1);
	//两点直线公式
	extern XBool twoPointLine(const XVector2& lineS,const XVector2& lineE,double *a,double *b,double *c);//(x-x1)/(x2-x1)=(y-y1)/(y2-y1)
	//点到线段的最短距离
	extern double minDistancePointToLine(const XVector2& point,const XVector2& lineS,const XVector2& lineE);
	//两线段的最短距离
	extern XVector2 minDistanceTwoLine(const XVector2& line1S,const XVector2& line1E,const XVector2& line2S,const XVector2& line2E);
	//获取p是否在p1 p2 p3 p4围成的四边形内部
	extern XBool getIsInRect(float x,float y,const XVector2& p1,const XVector2& p2,const XVector2& p3,const XVector2& p4);
	extern XBool getIsInRect(const XVector2& p,const XVector2& p1,const XVector2& p2,const XVector2& p3,const XVector2& p4);
	//判断点p是否在pR所围成的多边形内部
	extern XBool getIsInRect(float x,float y,const XVector2 *pR,int pRSum);
	extern XBool getIsInRect(const XVector2& p,const XVector2 *pR,int pRSum);
	//计算线段与矩形的交点，返回角点数量，o0为第一个角点，o1为第二个角点,返回交点的数量
	extern int getPointsLineToRect(const XVector2 &s,const XVector2 &e,const XRect &r,XVector2 &o0,XVector2 &o1);

	//所调用的四次方程的函数的声明，返回有效解的数量，以及带复数部分的解
	extern int quarticEquation(
						  double a,double b,double c,double d,double e,
						  std::complex<double> &x1,std::complex<double> &x2,
						  std::complex<double> &x3,std::complex<double> &x4
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
	extern unsigned short CRC16XModen(unsigned char *buf,unsigned short length);

	template<class type> void swapTwoSum(type &sum0, type &sum1)
	{//交换两个变量的值
		type temp = sum0;
		sum0 = sum1;
		sum1 = temp;
	}
	inline float rectResize(float w, float h, float Rx, float Ry)	//根据Rx与Ry构成的框，将w和h的框放入其中，如果能放进去，则返回1.0f,否则返回自适应大小
	{//该函数用于窗口缩放适应，返回缩放的比例
		if (w <= 0.0f || h <= 0.0f || Rx <= 0.0f || Ry <= 0.0f) return 1.0f;
		if (w <= Rx && h <= Ry) return 1.0f;
		float size = Rx / w;
		if (size < Ry / h) return size;
		else return Ry / h;
	}
	inline float squareFloat(float x) { return x * x; }	//求x的平方
	//#define SIN_BOOK	//sin函数是否使用查字典方式
#ifdef SIN_BOOK
	extern void initSinData();
	extern float xSin(float x);
#endif
	//#define SQRT_BOOK	//开方是否使用查字典方式

#ifdef SQRT_BOOK
	extern void initSqrtData();
	extern float xSqrt(float x);
#endif
	namespace QSort
	{
		template<class type>void qSort(type *a,int low,int high)
		{//代码来自百度
			if (low >= high) return;
			int first = low;
			int last = high;
			type key = a[first];	//用字表的第一个记录作为枢轴
			while (first<last)
			{
				while(first < last && a[last] >= key)
					--last;
				a[first] = a[last];	//将比第一个小的移到低端
				while(first < last && a[first] <= key)
					++first;
				a[last] = a[first];	//将比第一个大的移到高端
			}
			a[first] = key;	//枢轴记录到位
			qSort(a, low, first - 1);
			qSort(a, first + 1, high);
		}
	}
	//通过快速排序对数组进行排序
	template<class type> void quickSort(type *datas, int sum)
	{//初步通过验证
		if (datas == NULL || sum <= 1) return;
		QSort::qSort(datas, 0, sum - 1);
	}
	//quake3中的优化算法计算1.0/sqrt(x)
	inline float invSqrt(float x)
	{
		float xhalf = 0.5f*x;
		int i = *(int*)&x; // get bits for floating VALUE 
		i = 0x5f375a86- (i>>1); // gives initial guess y0
		x = *(float*)&i; // convert bits BACK to float
		x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
		return x;
	} 
}
//基本数学知识
//关于矩阵的基本知识
//v向量
//m矩阵
//mt,转置矩阵，为矩阵m的行列对调
//m-1,矩阵m的逆，v*m = v1 则 v = v1 * m-1
//矩阵满足结合律：v*m1 * m2 = v * (m1 * m2)
//(m1 + m2) * m3 = m1 * m3 + m2 * m3
//k * (m1 * m2) = k * m1 * m2 =  m1 * k * m2	//k是系数
//(m1 * m2)t = m1t * m2t
//正交矩阵，当逆等于转置矩阵时。
}
#endif