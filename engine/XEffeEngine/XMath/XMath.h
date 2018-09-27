#ifndef _JIA_XMATH_
#define _JIA_XMATH_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//这是一些基础的数学算法
//floor();	//向下取整
//ceil();	//向上取整
//round();	//四舍五入取整
#include "XCommonDefine.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <complex>
#include <iostream>
#include <math.h>
namespace XE{
class XVec2;
class XVec3;
#define PI (3.1415926535897932384626433832795f)
#define PI_HALF (1.5707963267948966192313216916395f)							//(PI * 0.5)
#define PI2 (6.283185307179586476925286766558f)									//(PI * 2.0)
#define RADIAN2DEGREE (57.295779513082320876798154814114f)						//(180.0 / PI)
#define DEGREE2RADIAN (0.01745329251994329576923690768488f)						//(PI / 180.0)
#define SIN45 (0.70710678118654752440084436210485f)
#define SQRT3 (1.7320508075688772935274463415059f)								//(3 ^ 0.5)
#define SQRT2 (1.4142135623730950488016887242097f)								//(2 ^ 0.5)
#define ONE_THIRD (0.333333f)	//1/3
#define TWO_THIRD (0.666666f)	//2/3

//#define XEE_Max(a,b) (((a) > (b)) ? (a) : (b))
//#define XEE_Min(a,b) (((a) < (b)) ? (a) : (b))

class XLine;
class XRect;
namespace XMath
{
	//获取f的近似数据，近似精度为len
	//f = 0.0123456 len = 2;
	//返回 1.23 ex = 2
	//也就是 0.0123456 = 1.23 * 10^-2 ,向下取整转换成科学计数法
	extern float getApproximateData(float f, int len, int &ex);
	//下面是一个sin的优化算法
	inline double sinTay(double x)
	{
		double x2 = x * x;

		const static double a1 = 0.99999748719198;
		const static double a3 = -0.166651680984443;
		const static double a4 = 0.00830951691334793;
		const static double a5 = -0.000184472198573026;
		return x*(a1 + (a3 + (a4 + a5*x2) * x2) * x2);
	}
	inline double cosTay(double x)
	{
		double cos = 0.0;
		//compute cosine: sin(x + PI/2) = cos(x)
		x += 1.57079632;
		if (x > 3.14159265) x -= 6.28318531;
		if (x < 0)
		{
			cos = 1.27323954 * x + 0.405284735 * x * x;
			if (cos < 0) cos = 0.225 * (cos *-cos - cos) + cos;
			else cos = 0.225 * (cos * cos - cos) + cos;
		}
		else
		{
			cos = 1.27323954 * x - 0.405284735 * x * x;
			if (cos < 0) cos = 0.225 * (cos *-cos - cos) + cos;
			else cos = 0.225 * (cos * cos - cos) + cos;
		}
		return cos;
	}
	//双线性插值的计算公式
	inline float bilinear(float a, float b, float c, float d, float rate0, float rate1)
	{
		return a * (1.0f - rate0) * (1.0f - rate1) + (1.0f - rate0) * rate1 * b
			+ rate0 * (1.0f - rate1) * c + rate0 * rate1 * d;
	}
	//四舍五入取整
	inline int toInt(float f) { return int(f + 0.5f); }
	//向下取整
	inline int toIntFloor(float f) { return (int)floorf(f); }
	//向上取整
	inline int toIntCeil(float f) { return (int)ceilf(f); }
	//需要进行适应缩放时，这里计算最小的缩放比例
	extern float getMinRate(const XVec2& a, const XVec2& b);
	//角度化简
	//extern float getAngleXY(float x,float y);	//用于获得x,y构成的角度
	inline float getAngleXY(float x, float y) { return atan2(y, x); }
	//返回范围0.0 - PI2
	inline float angleAbbreviation(float angle)	//弧度角度化简
	{
		angle = fmod(angle, PI2);
		if (angle < 0.0f) angle += (float)(PI2);
		return angle;
	}
	//返回范围0 - 360
	inline float angleReduce(float angle)		//角度简化单位:度
	{
		angle = fmod(angle, 360.0f);
		if (angle < 0.0f) angle += 360.0f;
		return angle;
	}
	template<typename T>
	bool isInArea(const T& value, const T& x, const T& y)	//判断value是否在x和y的范围内
	{
		if (x < y) return (value >= x && value <= y);
		else return (value >= y && value <= x);
	}
	template<typename T>
	bool isInAreaEx(const T& value, const T& maxV, const T& minV)	//判断value是否在b和s的范围内
	{
		return (value >= minV && value <= maxV);
	}
	template<class T>
	void swapTwoSum(T& sum0, T& sum1)
	{//交换两个变量的值
		std::swap(sum0, sum1);
	}
	//判断angle0是否在angle1 + maxArea到angle1 + minArea之间
	//minArea和maxArea必须为-180到+180度之间，所有参数单位为弧度
	inline bool isAngleInAreaR(float angle0, float angle1, float minArea, float maxArea)
	{
		if (minArea < 0.0f)
		{
			minArea += 360.0f;
			if (minArea > maxArea) swapTwoSum(minArea, maxArea);
			return !isInArea(angleReduce(angle0 - angle1), minArea, maxArea);
		}
		else
		{
			if (minArea > maxArea) swapTwoSum(minArea, maxArea);
			return isInArea(angleReduce(angle0 - angle1), minArea, maxArea);
		}
	}
	//两个角度差的绝对值返回范围为0-180
	inline float angleAbsDistance(float a, float b)
	{
		float ret = fmod(a - b, 360.0f);
		if (ret < 0.0f) ret += 360.0f;
		if (ret > 180.0f) ret = 360.0f - ret;
		return ret;
	}
	//计算两个角度之间的差值，范围-180 - 180
	inline float angleDistance(float a, float b)
	{
		float ret = fmod(a - b, 360.0f);
		if (ret < 0.0f) ret += 360.0f;
		if (ret > 180.0f) ret -= 360.0f;
		return ret;
	}
	//读数据进行夹值
	template<typename T>
	T clamp(T value, T min, T max)
	{
		return value < min ? min : value > max ? max : value;
	}
	//下面这组函数用于处理圆周的线性映射，比如无极编码器一周为16384，当转动在s和e的范围内时，
	//需要计算当前c角度相对于范围中点的角度时可以使用下面的函数
	extern float circleLineMapping(float in, float inStart, float inEnd, float inMin, float inMax, float outStart = 0.0f, float outEnd = 1.0f, float outMin = 0.0f, float outMax = 1.0f);
	//这个函数是上面这个函数的特殊形式，输出值以范围中点为0点，数值乘以系数
	inline float circleLineMappingEx(float in, float inStart, float inEnd, float inMin, float inMax, float outRate)
	{
		float ret = circleLineMapping(in, inStart, inEnd, inMin, inMax);
		float rate;
		if (inEnd >= inStart) rate = inEnd - inStart;
		else rate = inEnd - inMin + inMax - inStart;
		return (ret - 0.5f) * outRate * rate;
	}
	//对于上面这个问题的另一种处理方式，返回的数据是-180 到 +180，输入数据应该是 minAngle - maxAngle 之间
	inline float circleAngle(float middleAngle, float minAngle, float maxAngle, float curAngle)
	{//初步测试这个函数没有问题
		if (minAngle >= maxAngle) return 0.0f;	//非法的数据
		if (middleAngle < minAngle || middleAngle > maxAngle) return 0.0f;	//非法的数据
		if (curAngle < minAngle || curAngle > maxAngle) return 0.0f;		//非法的数据
		float half = (maxAngle - minAngle) * 0.5f;
		if (curAngle < middleAngle)
		{
			if (curAngle >= middleAngle - half)
			{
				return (curAngle - middleAngle) * 180.0f / half;
			}
			else
			{//另外半边
				return (curAngle - minAngle + maxAngle - middleAngle) * 180.0f / half;
			}
		}
		else
		{
			if (curAngle <= middleAngle + half)
			{
				return (curAngle - middleAngle) * 180.0f / half;
			}
			else
			{//另外半边
				return (minAngle - (maxAngle - curAngle) - middleAngle) * 180.0f / half;
			}
		}
	}
	//将数据in从imin到imax空间线性映射到omin到omax空间。注意：这个函数不能做逆方向的映射
	//输入输出区间都包含头和尾
	template<typename T> T mapping1D(T in, T imin, T imax, T omin, T omax)//一维映射
	//inline double mapping1D(double in, double imin, double imax, double omin, double omax)//一维映射
	{
		//if(fabs(xmax - xmin) < FLT_EPSILON) return ymin;		//数据非法
		if (abs(imax - imin) < 1.192092896e-07) return omin;	//数据非法
		//方案1
		//float ret = (in - imin) / (imax - imin) * (omax - omin) + omin;
		//if(ret < omin) return omin;
		//if(ret > omax) return omax;
		//return ret;
		//方案2
		if (in <= imin) return omin;
		if (in >= imax) return omax;
		return (in - imin) * (omax - omin) / (imax - imin) + omin;
	}
	//输入输出区间都包含头和尾
	inline int mapping1D(int in, int imin, int imax, int omin, int omax)//一维映射
	{
		if (imax - imin == 0) return omin;	//数据非法
		if (in <= imin) return omin;
		if (in >= imax) return omax;
		//下面修改为先乘再除，以便于对整数能有一个较好的结果,+1是为了包含大头
		return (in - imin) * (omax - omin + 1) / (imax - imin) + omin;
	}
	//相对于mapping1D，不做夹值，是超范围限制的，注意这个函数可以做逆方向的映射
	inline float mapping1DEx(float in, float imin, float imax, float omin, float omax)//一维映射
	{
		if (fabs(imax - imin) < 1.192092896e-07F) return omin;	//数据非法
		return (in - imin) * (omax - omin) / (imax - imin) + omin;
	}
	//贝塞尔计算
	inline float quadraticBezier(float x, float a, float b)
	{
		// adapted from BEZMATH.PS (1993)
		// by Don Lancaster, SYNERGETICS Inc.
		// http://www.tinaja.com/text/bezmath.html

		float epsilon = 0.00001f;
		a = (std::max)(0.0f, (std::min)(1.0f, a));
		b = (std::max)(0.0f, (std::min)(1.0f, b));
		if (a == 0.5f)
			a += epsilon;

		// solve t from x (an inverse operation)
		float om2a = 1.0f - 2.0f * a;
		float t = (sqrt(a * a + om2a * x) - a) / om2a;
		return (1.0f - 2.0f * b) * (t * t) + (2.0f * b) * t;
	}
	//与上面的函数功能一样，只是是非线性的映射，而是贝塞尔变形的映射方式
	//pa和pb为贝塞尔曲线的参数,输入输出值都会在范围内
	inline float mapping1DQBz(float in, float imin, float imax, float omin, float omax, float pa, float pb)
	{
		if (fabs(imax - imin) <= 1.192092896e-07F ||
			fabs(omin - omax) <= 1.192092896e-07F) return omin;	//数据非法
		return XMath::clamp(quadraticBezier((in - imin) / (imax - imin), pa, pb), 0.0f, 1.0f) *
			(omax - omin) + omin;
	}
	//0   1
	//3   2
	//注意:需要尽量保证输入输出都为凸多边形，否则会造成问题。关于凹多边形需要再仔细考虑
	//这个函数用于处理两个2D空间中的空间映射问题，四边形空间
	extern XVec2 mapping2D(const XVec2 *inRect, const XVec2 *outRect, const XVec2& in);
	//对上面算法的深入简化(上面的函数存在缺陷，应尽量避免使用)
	extern XVec2 mapping2DPlus(const XVec2 *inRect, const XVec2 *outRect, const XVec2& in);
	//0   1
	//3   2
	//上面一个空间映射的另一种实现方式，三角形映射，与三角片绘制的结果相吻合
	//三角空间的映射
	extern XVec2 mapping2DTriangle(const XVec2 *inRect, const XVec2 *outRect, const XVec2& in);
	//通过上面的两个三角形构成的四边形空间(这种方法好像没有mapping2D平滑稳定)
	extern XVec2 mapping2DEx(const XVec2 *inRect, const XVec2 *outRect, const XVec2& in);

	//四边形向量场插值，由四个角点向量构成的向量场，计算其中某点的向量，使用线性插值
	extern XVec2 vectorFieldLineSlerp(const XVec2 *posRect, const XVec2 *vectorRect, const XVec2& in);
	//四边形标量场插值,在2D空间的一个标量场，已知四边形角点的标量值，计算四边形内部任意点的标量值使用线性插值
	extern float scalarFieldLineSlerp(const XVec2 *posRect, const float *scalarRect, const XVec2& in);
	//线性插值，s:起点，e:终点，r：百分比[0 - 1.0]
	template<typename T>
	T lineSlerp(const T& s, const T& e, float r) { return e * r + s * (1.0f - r); }
	//插值 x[0 - 1]
	template<typename T>
	T cSlerp(const T& u0, const T& u1, const T& u2, const T& u3, float x)
	{
		T p = (u3 - u2) - (u0 - u1);
		T q = (u0 - u1) - p;
		T r = u2 - u0;
		return x * (x * (x * p + q) + r) + u1;
	}
	//寻找比temp大的最小的符合2的n次方的尺寸
	inline int getMinWellSize2n(int temp) { return (int)powf(2.0, ceilf(logf((float)temp) / logf(2.0f))); }
	inline XBool isNPOT(int width, int height)		//判断是否不符合2的n次方
	{
		if (getMinWellSize2n(width) != width ||
			getMinWellSize2n(height) != height) return XTrue;
		return XFalse;
	}
	inline int fit4(int value)	//输出符合4字节对齐的值
	{
		if (value % 4 == 0) return value;
		return value + 4 - (value % 4);
	}
	extern void vector4Sort(XVec2* v, XVec2* u);//对任意序列的无旋转矩形进行顺时针排序
	//4个点必须要是已经排序了的
	extern int getEdgePoint(float angle, XVec2 * points, const XVec2* v,
		const XVec2* u);	//只能以中心点
	//获取一个矩形内的点与矩形的边在指定角度下围成的多边形(下面的方法可以设定以哪个点)
	extern int getEdgePointEx(const XRect& rect, const XVec2& point,
		float angle, XVec2 * points);	//角度为12点钟方向开始逆时针[0 - PI2]
	extern int getEdgePointEx(const XRect& vRect, const XRect& uRect, const XVec2& vPoint,
		float angle, XVec2 * vPoints, XVec2 * uPoints);	//角度为12点钟方向开始逆时针[0 - PI2]
	extern int getEdgePointEx(const XRect& rect, const XVec2& point,
		float angle, float * points);
	extern int getEdgePointEx(const XRect& vRect, const XRect& uRect, const XVec2& vPoint,
		float angle, float * vPoints, float * uPoints);	//角度为12点钟方向开始逆时针[0 - PI2]
	//+++++++匀速直线运动下的一组函数++++++++
	//返回值 -2参数错误 -1不会发生碰撞 ret > 0
	//匀速直线运动两球碰撞时间的计算
	extern double getTwoLineTime(const XVec2& L1S, const XVec2& L1E, const XVec2& L2S, const XVec2& L2E, float distance, float timer);	//获取两条线段某个距离的最小解
	//匀速直线运动球与固定点的碰撞时间计算
	extern double getLineToPointTime(const XVec2& L1S, const XVec2& L1E, const XVec2& point, float distance, float timer);
	//匀速直线运动球与固定线段的碰撞时间计算
	extern double getObjectToLineTime(const XVec2& L1S, const XVec2& L1E, const XVec2& L2S, const XVec2& L2E, float distance, float timer);	//获取两条线段某个距离的最小解
	//运动球体与运动线段的碰撞时间计算
	//运动线段与运动线段的碰撞时间计算
	//+++++++匀加速直线运动的一组函数++++++++
	//匀加速直线运动两物体碰撞时间的计算(时间单位统一为ms)
	extern double getCrashTimerB2B(const XVec2& position1, const XVec2& speed1, const XVec2& a1,
		const XVec2& position2, const XVec2& speed2, const XVec2& a2, float distance, float time);
	//匀加速直线运动物体与固定点碰撞的时间计算
	extern double getCrashTimerB2P(const XVec2& position1, const XVec2& speed1, const XVec2& a1,
		const XVec2& position2, float distance, float time);
	//匀加速直线运动物体与固定直线碰撞的时间计算
	extern double getCrashTimerB2L(const XVec2& position1, const XVec2& speed1, const XVec2& a1,
		const XVec2& positionS, const XVec2& positionE, float distance, float time);
	//很奇怪下面这行对XLine使用const说明居然会出错，不知道为什么。
	extern double getCrashTimerB2L(const XVec2& position1, const XVec2& speed1, const XVec2& a1, const XLine& pL, float distance, float time);
	//extern double getCrashTimerB2L(const XVec2& position1,const XVec2& speed1,const XVec2& a1,XLine& pL,float distance,float time);
	extern void printfR();
	//下面描写一些基本的公式
	//点到直线距离的公式
	extern double distancePointToLine(const XVec2& point, const XVec2& lineS, const XVec2& lineE);//ax+by+c=0 x0,y0 |ax0+by0+c|/√(a^2+b^2)
	extern double distancePointToLine(const XVec2& point, double a, double b, double c);
	//点到直线的垂足公式
	extern XVec2 curtatePointToLine(const XVec2& point, const XVec2& lineS, const XVec2& lineE);	//设直线方程为ax+by+c=0,点坐标为(m,n)则垂足为((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
	extern XVec2 curtatePointToLine(const XVec2& point, double a, double b, double c);
	//两直线交点的公式
	extern XVec2 crossPointLineToLine(const XVec2& line1S, const XVec2& line1E, const XVec2& line2S, const XVec2& line2E);
	extern XVec2 crossPointLineToLine(double a0, double b0, double c0, double a1, double b1, double c1);
	//两点直线公式
	extern XBool twoPointLine(const XVec2& lineS, const XVec2& lineE, double *a, double *b, double *c);//(x-x1)/(x2-x1)=(y-y1)/(y2-y1)
	//点到线段的最短距离
	extern double minDistancePointToLine(const XVec2& point, const XVec2& lineS, const XVec2& lineE);
	//两线段的最短距离
	extern XVec2 minDistanceTwoLine(const XVec2& line1S, const XVec2& line1E, const XVec2& line2S, const XVec2& line2E);
	//获取p是否在p1 p2 p3 p4围成的四边形内部
	extern XBool getIsInRect(float x, float y, const XVec2& p1, const XVec2& p2, const XVec2& p3, const XVec2& p4);
	extern XBool getIsInRect(const XVec2& p, const XVec2& p1, const XVec2& p2, const XVec2& p3, const XVec2& p4);
	//判断点p是否在pR所围成的多边形内部
	extern XBool getIsInPolygon(float x, float y, const XVec2 *pR, int pRSum);
	extern XBool getIsInPolygon(const XVec2& p, const XVec2 *pR, int pRSum);
	inline XBool getIsInPolygon(const XVec2& p, const std::vector<XVec2> &points)
	{
		return getIsInPolygon(p, &points[0], (int)(points.size()));
	}
	//判断点是否在三角形内部
	extern XBool getIsInTriangle(const XVec2& p, const XVec2 *points);
	inline XBool getIsInTriangle(const XVec2& p, const std::vector<XVec2> &points)
	{
		if (points.size() < 3) return false;
		return getIsInTriangle(p, &points[0]);
	}
	//计算线段与矩形的交点，返回交点数量，o0为第一个交点，o1为第二个交点,返回交点的数量
	extern int getPointsLineToRect(const XVec2& s, const XVec2& e, const XRect& r, XVec2& o0, XVec2& o1);

	//所调用的四次方程的函数的声明，返回有效解的数量，以及带复数部分的解
	extern int quarticEquation(
		double a, double b, double c, double d, double e,
		std::complex<double> &x1, std::complex<double> &x2,
		std::complex<double> &x3, std::complex<double> &x4);
	//上面函数的再封装，返回不带复数部分的解
	extern int quarticEquation(
		double a, double b, double c, double d, double e,
		double *x);
	//计算大小为s的方阵的值，data为方正的数据
	//0,1,2
	//3,4,5
	//6,7,8
	extern double getValueMatrix(const double *d, int s);
	//下面是CRC16计算
	//p为字符串
	//len为字符串长度
	extern unsigned short CRC16_Modbus(unsigned char *p, unsigned short len);
	extern unsigned short CRC16XModen(unsigned char *buf, unsigned short length);
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
		template<class type>void qSort(type *a, int low, int high)
		{//代码来自百度
			if (low >= high) return;
			int first = low;
			int last = high;
			type key = a[first];	//用字表的第一个记录作为枢轴
			while (first < last)
			{
				while (first < last && a[last] >= key)
					--last;
				a[first] = a[last];	//将比第一个小的移到低端
				while (first < last && a[first] <= key)
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
		i = 0x5f375a86 - (i >> 1); // gives initial guess y0
		x = *(float*)&i; // convert bits BACK to float
		x = x*(1.5f - xhalf*x*x); // Newton step, repeating increases accuracy
		return x;
	}
	//下面这个函数是不可逆的
	extern std::string to36Encode(const unsigned char *data, int len);				//将一串数据转换成36进制的字符串，0-9 + A-Z,用于字符串加密
	extern std::string to64Encode(const unsigned char *data, int len);				//将长数据转换成64进制字符串0-9 A-Z a-z +=
	extern void to64Decode(unsigned char *data, int len, const std::string& code);	//将64进制字符串转换成数据
	//下面需要实现一些图形学的基本算法
	//判断两条直线是否有交点
	extern bool isLineIntersect(const XVec2& s0, const XVec2& e0, const XVec2& s1, const XVec2& e1);
	//判断两个多边形是否相交
	extern bool isPolygonIntersect(const std::vector<XVec2> &polygon0, const std::vector<XVec2> &polygon1);
	//获取凸多边形的面积
	extern double getPolygonArea(const std::vector<XVec2> &polygon);
	//计算任意三角形的面积
	extern double getTriangleArea(const std::vector<XVec2> &triangle);
	//判断两条贝塞尔曲线的交点
	//http://stackoverflow.com/questions/4039229/checking-if-two-cubic-b%C3%A9zier-curves-intersect
	//http://www.tsplines.com/technology/edu/CurveIntersection.pdf
	//http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/Bezier/bezier-sub.html
	//获取两个正整数的最大公约数
	extern int getGCM(int a, int b);
	//求解N元依次方程组
	//inData：方程系数，n元一次方程应该有n*（n+1）个系数
	//xSum：未知数的数量
	//outData：结果
	//函数返回求解是否成功,目前这个方法没有经过仔细的测试，只进行了初步测试
	extern bool calMPEquation(std::vector<double> &inData, int xSum, std::vector<double> &outData);
	//对曲线进行重新采样
	//inLines：由若干点构成的线
	//outLines：重新采样之后的线
	//len：重新采样的点间距
	//isLoop：线是否闭合
	extern void resampleLines(const std::vector<XVec2> &inLines,
		std::vector<XVec2> &outLines, float len, bool isLoop);
	extern void resampleLinesP(const std::vector<XVec2> &inLines,
		std::vector<XVec2> &outLines, int pointSum, bool isLoop);
	//返回第一位有效位之后的值，如：53.123，返回50，如果0.0123，返回0.01
	//fData返回最高位的数值
	extern double getTheFirstData(double data, int &fData);
	//极坐标到直角坐标系(笛卡尔坐标系)的转换
	extern XVec2 polar2Cart(float len, float angle);//angle的单位是弧度
	//在点阵中按照距离排序，返回指定序列的点阵坐标，如果距离相同，则从正上方开始顺时针排序
	//编号从0开始，0为自身点
	//由于目前没有对下面的方法找到学术的算法，而定义查表的方式来给出近似解
	extern XVec2 get2DOrderIndex(int order);
	extern XVec3 get3DOrderIndex(int order);

	//下面是一些需要但是尚未实现的接口
	//判断两条线段是否相交
	extern bool isLineSegmentIntersect(const XVec2& s0, const XVec2& e0, const XVec2& s1, const XVec2& e1);
	//计算两条线段的交点
	extern bool getCrossPoint2LineSegment(const XVec2& s0, const XVec2& e0, const XVec2& s1, const XVec2& e1, XVec2& o);
	//计算两条线段的最小距离(尚未实现)
	extern float minDistance2LineSegment(const XVec2& s0, const XVec2& e0, const XVec2& s1, const XVec2& e1);
	//计算线段与四边形的交点(尚未实现)
	//从min到max之间取随机不重复序列的num个数放在randArray中
	extern bool getRandomArray(std::vector<int> &randArray, int num, int _min, int _max);
	//上面一种功能的另一种实现
	extern bool getRandomArrayEx(std::vector<int> &randArray, int num, int _min, int _max);
}
class X2DOrderTable
{
private:
	class XTableInfo
	{
	public:
		int x;
		int y;
		int sum;
		int order;
	public:
		//XTableInfo() {}
		XTableInfo(int _x, int _y)
			:x(_x)
			, y(_y)
		{
			order = x * x + y * y;
		}
		bool operator < (const XTableInfo &m) const
		{
			return order < m.order;
		}
	};
	std::vector<XTableInfo> m_table;
	struct XFullTableInfo
	{
		short x;
		short y;
	};
	std::vector<XFullTableInfo> m_fullTable;
	int m_isInited;
	bool m_isFullTable;

	void _init(std::vector<XTableInfo> &table, int size);
	static const XVec2 m_order4[4];
public:
	X2DOrderTable()
		:m_isInited(false)
		, m_isFullTable(false)
	{}
	//更少的内存空间
	void initTable(int size)
	{
		if (m_isInited || size <= 0) return;
		_init(m_table, size);
		m_isInited = true;
	}
	//更快的速度
	int getTableSize()const;
	void initFullTable(int size);
	XE::XVec2 get2DOrderIndex(int index);
};
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