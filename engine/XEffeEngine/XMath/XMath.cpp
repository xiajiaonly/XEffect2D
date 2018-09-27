#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMath.h"
#include "XVector2.h"
#include "XVector3.h"
#include "XLine.h"
#include "XMatrix2x2.h"
#if WITH_LOG
#else
#include <vector>
#endif
#include "XRect.h"
namespace XE{
namespace XMath
{
	float getMinRate(const XVec2& d, const XVec2& s) { return (std::min)(d.x / s.x, d.y / s.y); }
	float getApproximateData(float f, int len, int &ex)
	{
		ex = 0;
		if (f == 0.0f || len <= 0 || len >= 6) return 0.0f;
		float tmp = pow(10.0f, len);
		if (int(f) != 0)
		{//带整数
			for (ex = 0; ex < 10; ++ex)
			{
				if (int(f) == 0)
				{
					ex = ex - 1;
					if (f < 0.0f) return ceil(f * tmp * 10.0f) / tmp;
					else return floor(f * tmp * 10.0f) / tmp;
				}
				f *= 0.1f;
			}
		}
		else
		{//纯小数
			for (ex = 0; ex < 10; ++ex)
			{
				if (int(f) != 0)
				{
					ex = -ex;
					if (f < 0.0f) return ceil(f * tmp) / tmp;
					else return floor(f * tmp) / tmp;
				}
				f *= 10.0f;
			}
		}
		return 0.0f;
	}
	//float getAngleXY(float x,float y)
	//{
	//	float angle = 0.0f;
	//	if(x == 0.0f)
	//	{
	//		if(y == 0.0f) angle = 0.0f; else
	//		if(y > 0.0f) angle = PI_HALF; else
	//		angle = PI + PI_HALF;
	//	}else
	//	if(y == 0.0f)
	//	{
	//		if(x > 0.0f) angle = 0.0f;
	//		else angle = PI;
	//	}else
	//	{
	//		angle = atan(y / x);        //atan() return a value (-PI_HALF , PI_HALF)
 //       
	//		if(x < 0.0f) angle += PI;
	//		if(angle < 0.0f) angle += PI2;
	//	}
	//	return angle;
	//}
	//对任意序列的无旋转矩形进行顺时针排序
	void vector4Sort(XVec2* v, XVec2* u)
	{
		XBool isChange;
		for (int j = 0; j < 3; ++j)
		{
			isChange = XFalse;
			for (int i = 0; i < 3; ++i)
			{
				if (v[i].x > v[i + 1].x)
				{
					XMath::swapTwoSum<XVec2>(v[i], v[i + 1]);
					XMath::swapTwoSum<XVec2>(u[i], u[i + 1]);
					isChange = XTrue;
				}
			}
			if (!isChange) break;
		}
		if (v[2].y < v[3].y)
		{
			XMath::swapTwoSum<XVec2>(v[2], v[3]);
			XMath::swapTwoSum<XVec2>(u[2], u[3]);
		}
	}
	//4个点必须要是已经排序了的
	int getEdgePoint(float angle, XVec2 * points, const XVec2* v, const XVec2* u)
	{
		int sum = 0;
		XVec2 centerV = (v[0] + v[2]) * 0.5f;	//提取必要的参数
		XVec2 centerU = (u[0] + u[2]) * 0.5f;	//提取必要的参数
		float wV = -(v[0].x - v[2].x);
		float hV = -(v[0].y - v[2].y);
		float tempAngle = XVec2(fabs(wV), fabs(hV)).getAngle();
		//默认为顺时针垂直边模式及第一二个点是左边
		points[0] = centerV;
		points[1] = centerU;

		points[2].set(centerV.x, v[0].y);
		points[3] = (u[0] + u[3]) * 0.5f;
		if (angle <= PI_HALF - tempAngle)
		{
			points[4].set(centerV.x - hV * 0.5f * tan(angle), v[0].y);
			points[5].set(mapping1D(points[4].x, v[3].x, v[0].x, u[3].x, u[0].x),
				mapping1D(points[4].x, v[3].x, v[0].x, u[3].y, u[0].y));//@	//不好映射列
			sum = 3;
		}
		else
		if (angle <= PI_HALF + tempAngle)
		{
			points[4] = v[0];
			points[5] = u[0];

			points[6].set(v[0].x, centerV.y - wV * 0.5f * tan(PI_HALF - angle));
			points[7].set(mapping1D(points[6].y, v[1].y, v[0].y, u[1].x, u[0].x),
				mapping1D(points[6].y, v[1].y, v[0].y, u[1].y, u[0].y));
			sum = 4;
		}
		else
		if (angle <= PI * 1.5f - tempAngle)
		{
			points[4] = v[0];
			points[5] = u[0];
			points[6] = v[1];
			points[7] = u[1];

			points[8].set(centerV.x - hV * 0.5f * tan(PI - angle), v[1].y);
			points[9].set(mapping1D(points[8].x, v[2].x, v[1].x, u[2].x, u[1].x),
				mapping1D(points[8].x, v[2].x, v[1].x, u[2].y, u[1].y));
			sum = 5;
		}
		else
		if (angle <= PI * 1.5f + tempAngle)
		{
			points[4] = v[0];
			points[5] = u[0];
			points[6] = v[1];
			points[7] = u[1];
			points[8] = v[2];
			points[9] = u[2];
			points[10].set(v[2].x, centerV.y + wV * 0.5f * tan(PI * 1.5f - angle));
			points[11].set(mapping1D(points[10].y, v[3].y, v[2].y, u[3].x, u[2].x),
				mapping1D(points[10].y, v[3].y, v[2].y, u[3].y, u[2].y));
			sum = 6;
		}
		else
		{
			points[4] = v[0];
			points[5] = u[0];
			points[6] = v[1];
			points[7] = u[1];
			points[8] = v[2];
			points[9] = u[2];
			points[10] = v[3];
			points[11] = u[3];
			points[12].set(centerV.x + hV * 0.5f * tan(PI2 - angle), v[3].y);
			points[13].set(mapping1D(points[12].x, v[0].x, v[3].x, u[0].x, u[3].x),
				mapping1D(points[12].x, v[0].x, v[3].x, u[0].y, u[3].y));
			sum = 7;
		}
		//points[sum] = center;
		return sum;
	}
	int getEdgePointEx(const XRect& rect, const XVec2& point,float angle,float * points)	//角度为12点钟方向开始逆时针[0 - PI]
	{
		//如果点不在矩形内部则直接返回
		if(!rect.isInRect(point)) return 0;
		int ret = 0;
		//三个点
		float stateAngle = PI_HALF - (point - rect.getLT()).getAngle();
		if(angle < stateAngle)
		{//符合三个点的情况
			ret = 3;
			points[0] = point.x;									points[1] = point.y;
			points[2] = point.x;									points[3] = rect.top;
			points[4] = point.x - (point.y - rect.top) * tan(angle);points[5] = rect.top;
			return ret;
		}
		//四个点
		stateAngle = PI_HALF + XVec2(point.x - rect.left,rect.bottom - point.y).getAngle();
		if(angle < stateAngle)
		{//符合四个点的情况
			ret = 4;
			points[0] = point.x;	points[1] = point.y;
			points[2] = point.x;	points[3] = rect.top;
			points[4] = rect.left;	points[5] = rect.top;
			points[6] = rect.left;	points[7] = point.y - (point.x - rect.left) * tan(PI_HALF - angle);
			return ret;
		}
		//五个点
		stateAngle = PI_HALF * 3.0f - (rect.getRB() - point).getAngle();
		if(angle < stateAngle)
		{//符合五个点的情况
			ret = 5;
			points[0] = point.x;	points[1] = point.y;
			points[2] = point.x;	points[3] = rect.top;
			points[4] = rect.left;	points[5] = rect.top;
			points[6] = rect.left;	points[7] = rect.bottom;
			points[8] = point.x - (rect.bottom - point.y) * tan(PI - angle);points[9] = rect.bottom;
			return ret;
		}
		//六个点
		stateAngle = PI_HALF * 3.0f + XVec2(rect.right - point.x,point.y - rect.top).getAngle();
		if(angle < stateAngle)
		{//符合六个点的情况
			ret = 6;
			points[0] = point.x;	points[1] = point.y;
			points[2] = point.x;	points[3] = rect.top;
			points[4] = rect.left;	points[5] = rect.top;
			points[6] = rect.left;	points[7] = rect.bottom;
			points[8] = rect.right;	points[9] = rect.bottom;
			points[10] = rect.right;points[11] = point.y + (rect.right - point.x) * tan(PI_HALF * 3.0f - angle);
			return ret;
		}
		//七个点
		if(angle < PI2)
		{
			ret = 7;
			points[0] = point.x;	points[1] = point.y;
			points[2] = point.x;	points[3] = rect.top;
			points[4] = rect.left;	points[5] = rect.top;
			points[6] = rect.left;	points[7] = rect.bottom;
			points[8] = rect.right;	points[9] = rect.bottom;
			points[10] = rect.right;points[11] = rect.top;
			points[12] = point.x + (point.y - rect.top) * tan(PI2 - angle);points[13] = rect.top;
			return ret;
		}
		return 0;
	}
	int getEdgePointEx(const XRect& vRect,const XRect& uRect,const XVec2& vPoint,
		float angle,float * vPoints,float * uPoints)	//角度为12点钟方向开始逆时针[0 - PI2]
	{
		int ret = getEdgePointEx(vRect,vPoint,angle,vPoints);
		if(ret <= 0) return 0;
		for(int i = 0;i < ret;++ i)
		{
			uPoints[i << 1] = mapping1D(vPoints[i << 1],vRect.left,vRect.right,uRect.left,uRect.right);
			uPoints[(i << 1) + 1] = mapping1D(vPoints[(i << 1) + 1],vRect.top,vRect.bottom,uRect.top,uRect.bottom);
		}
		return ret;
	}
	int getEdgePointEx(const XRect& rect,const XVec2& point,float angle,XVec2 * points)
	{
		//如果点不在矩形内部则直接返回
		if(!rect.isInRect(point)) return 0;
		int ret = 0;
		//三个点
		float stateAngle = PI_HALF - (point - rect.getLT()).getAngle();
		if(angle < stateAngle)
		{//符合三个点的情况
			ret = 3;
			points[0] = point;
			points[1].set(point.x,rect.top);
			points[2].set(point.x - (point.y - rect.top) * tan(angle),rect.top);
			return ret;
		}
		//四个点
		stateAngle = PI_HALF + XVec2(point.x - rect.left,rect.bottom - point.y).getAngle();
		if(angle < stateAngle)
		{//符合四个点的情况
			ret = 4;
			points[0] = point;
			points[1].set(point.x,rect.top);
			points[2] = rect.getLT();
			points[3].set(rect.left,point.y - (point.x - rect.left) * tan(PI_HALF - angle));
			return ret;
		}
		//五个点
		stateAngle = PI_HALF * 3.0f - (rect.getRB() - point).getAngle();
		if(angle < stateAngle)
		{//符合五个点的情况
			ret = 5;
			points[0] = point;
			points[1].set(point.x,rect.top);
			points[2] = rect.getLT();
			points[3] = rect.getLB();
			points[4].set(point.x - (rect.bottom - point.y) * tan(PI - angle),rect.bottom);
			return ret;
		}
		//六个点
		stateAngle = PI_HALF * 3.0f + XVec2(rect.right - point.x,point.y - rect.top).getAngle();
		if(angle < stateAngle)
		{//符合六个点的情况
			ret = 6;
			points[0] = point;
			points[1].set(point.x,rect.top);
			points[2] = rect.getLT();
			points[3] = rect.getLB();
			points[4] = rect.getRB();
			points[5].set(rect.right,point.y + (rect.right - point.x) * tan(PI_HALF * 3.0f - angle));
			return ret;
		}
		//七个点
		if(angle < PI2)
		{
			ret = 7;
			points[0] = point;
			points[1].set(point.x,rect.top);
			points[2] = rect.getLT();
			points[3] = rect.getLB();
			points[4] = rect.getRB();
			points[5] = rect.getRT();
			points[6].set(point.x + (point.y - rect.top) * tan(PI2 - angle),rect.top);
			return ret;
		}
		return 0;
	}
	int getEdgePointEx(const XRect& vRect, const XRect& uRect, const XVec2& vPoint, float angle, XVec2 * vPoints, XVec2 * uPoints)
	{
		int ret = getEdgePointEx(vRect, vPoint, angle, vPoints);
		if (ret <= 0) return 0;
		for (int i = 0; i < ret; ++i)
		{
			uPoints[i].x = mapping1D(vPoints[i].x, vRect.left, vRect.right, uRect.left, uRect.right);
			uPoints[i].y = mapping1D(vPoints[i].y, vRect.top, vRect.bottom, uRect.top, uRect.bottom);
		}
		return ret;
	}

	//折射与反射的计算
	//折射：dx = h * sin(b * Ta/Tb);
	//反射：dx = (h1 + h2) * sin(b);
	//h为源到媒介点的距离或者媒介点到目标点的距离
	//b为入射角
	//Ta与Tb是两种媒介的光传导系数
	//计算两个移动物体发生碰撞的时间
	double getTwoLineTime(const XVec2& L1S,const XVec2& L1E,const XVec2& L2S,const XVec2& L2E,float distance,float timer)
	{
		if(timer <= 0) return -2.0;	//数值非法
		if(distance < 0) return -2.0;	//数值非法
		//再进行一次优化
		double dx = (L1E.x - L1S.x - (L2E.x - L2S.x)) / timer;
		double dy = (L1E.y - L1S.y - (L2E.y - L2S.y)) / timer;
		double a = dx * dx + dy * dy;
		double b = 2.0 * (L1S.x - L2S.x) * dx + 2.0 * (L1S.y - L2S.y) * dy;
		double c = (L1S.x - L2S.x) * (L1S.x - L2S.x) + (L1S.y - L2S.y) * (L1S.y - L2S.y) - distance * distance;
		if(a == 0) return -1.0;//平行线
		double temp = b * b - 4 * a * c;
		if(temp < 0)
		{//无解
			return -1.0;	//不会发生碰撞
		}else
		{
			double t2 = (-b - sqrt(temp)) / (2.0 * a);
			if(t2 > 0 && t2 <= timer) return t2;
		//	double t1 = (-b + sqrt(temp)) / (2.0 * a);
		//	if(t1 > 0 && t1 <= timer) return t1;
			return -1.0;
		}
	//	return -1.0;
	}
	//计算一个移动物体与点发生碰撞的时间
	double getLineToPointTime(const XVec2& L1S,const XVec2& L1E,const XVec2& point,float distance,float timer)
	{
		if(timer <= 0) return -2.0;	//数值非法
		if(distance < 0) return -2.0;	//数值非法
		//再进行一次优化
		double dx = (L1E.x - L1S.x) / timer;
		double dy = (L1E.y - L1S.y) / timer;
		double a = dx * dx + dy * dy;
		double b = 2.0 * (L1S.x - point.x) * dx + 2.0 * (L1S.y - point.y) * dy;
		double c = (L1S.x - point.x) * (L1S.x - point.x) + (L1S.y - point.y) * (L1S.y - point.y) - distance * distance;
		if(a == 0.0) return -1.0;//平行线
		double temp = b * b - 4 * a * c;
		if(temp < 0)
		{//无解
			return -1.0;	//不会发生碰撞
		}else
		{
			double t2 = (-b - sqrt(temp)) / (2.0 * a);
			if(t2 > 0 && t2 <= timer) return t2;
		//	double t1 = (-b + sqrt(temp)) / (2.0 * a);
		//	if(t1 > 0 && t1 <= timer) return t1;
			return -1.0;
		}
	//	return -1.0;
	}
	//计算一个移动物体与一条线段发生碰撞的时间(不考虑端点)
	double getObjectToLineTime(const XVec2& L1S,const XVec2& L1E,const XVec2& L2S,const XVec2& L2E,float distance,float timer)	//获取两条线段某个距离的最小解
	{
		if(timer <= 0) return -2.0;	//数值非法
		if(distance < 0) return -2.0;	//数值非法
		//算法1
		{
			double dx = (L1E.x - L1S.x) / timer;
			double dy = (L1E.y - L1S.y) / timer;
			double Ptx = 0.0;
			double Pty = 0.0;
			double A,B,C;	//Ax + By + c = 0;
			twoPointLine(L2S,L2E,&A,&B,&C);
			double a = A * dx + B * dy;
			double b = A * L1S.x + B * L1S.y + C + distance * _hypot(A,B);
			if(a == 0) return -1.0;
			double t1 = -b/a;
			//b = 2.0 * (A * L1S.x + B * L1S.y + C) - b;
			b = A * L1S.x + B * L1S.y + C - distance * _hypot(A,B);
			double t2 = -b/a;
			if(t1 < t2 && t1 >= 0 && t1 <= timer)
			{
				Ptx = L1S.x + dx * t1;
				Pty = L1S.y + dy * t1;
				//下面判断垂足是否在范围内
				if(B != 0.0)
				{
					double x = (B * B * Ptx - A * B * Pty - A * C) / (A * A + B * B);
					if((x >= L2E.x && x <= L2S.x) || (x >= L2S.x && x < L2E.x)) return t1;
				}else
				{
					double y = (A * A * Pty - A * B * Ptx - B * C) / (A * A + B * B);
					if((y >= L2E.y && y <= L2S.y) || (y >= L2S.y && y < L2E.y)) return t1;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
				}
			}
			if(t2 < t1 && t2 >= 0 && t2 <= timer)
			{
				Ptx = L1S.x + dx * t2;
				Pty = L1S.y + dy * t2;
				//下面判断垂足是否在范围内
				if(B != 0.0)
				{
					double x = (B * B * Ptx - A * B * Pty - A * C) / (A * A + B * B);
					if((x >= L2E.x && x <= L2S.x) || (x >= L2S.x && x < L2E.x)) return t2;
				}else
				{
					double y = (A * A * Pty - A * B * Ptx - B * C) / (A * A + B * B);
					if((y >= L2E.y && y <= L2S.y) || (y >= L2S.y && y < L2E.y)) return t2;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
				}
			}
			return -1.0;
		}
	}
	//double r[4];
	//void printfR()
	//{
	//	printf("-------\n");
	//	printf("%f\n",r[0]);
	//	printf("%f\n",r[1]);
	//	printf("%f\n",r[2]);
	//	printf("%f\n",r[3]);
	//}
	double getCrashTimerB2B(const XVec2& position1, const XVec2& speed1, const XVec2& a1,
		const XVec2& position2, const XVec2& speed2, const XVec2& a2, float distance, float time)
	{
		double e = (position1.x - position2.x) * (position1.x - position2.x) + (position1.y - position2.y) * (position1.y - position2.y) - distance * distance;
		if (e < 0) return -1.0f;	//防止结余误差
		double a = (0.25 * (a1.x - a2.x) * (a1.x - a2.x) + 0.25 * (a1.y - a2.y) * (a1.y - a2.y)) * 0.001 * 0.001 * 0.001 * 0.001;
		double b = ((a1.x - a2.x) * (speed1.x - speed2.x) + (a1.y - a2.y) * (speed1.y - speed2.y)) * 0.001 * 0.001 * 0.001;
		double c = ((speed1.x - speed2.x) * (speed1.x - speed2.x) + (position1.x - position2.x) * (a1.x - a2.x) + (speed1.y - speed2.y) * (speed1.y - speed2.y) + (position1.y - position2.y) * (a1.y - a2.y)) * 0.001 * 0.001;
		double d = (2.0 * (speed1.x - speed2.x) * (position1.x - position2.x) + 2.0 * (speed1.y - speed2.y) * (position1.y - position2.y)) * 0.001;
		if (a == 0.0) //则测试b必然也等于0
		{//求解一元二次方程
			if (c == 0) return -1.0;//平行线
			double temp = d * d - 4 * c * e;
			if (temp < 0)
			{//无解
				return -1.0;	//不会发生碰撞
			}
			else
			{//由于浮点数结余误差这里需要检验这个值，尚未进行
				double t2 = (-d - sqrt(temp)) / (2.0 * c);
				if (t2 > 0 && t2 <= time) return t2;
				else return -1.0;
			}
			//		return -1.0;
		}
		else
		{//求解一元四次方程
			double r[4];
			int sum = quarticEquation(a, b, c, d, e, r);
			if (sum == 0) return -1.0;	//无解
			double temp = r[0];
			if (sum == 2)
			{
				if (r[0] < r[1]) temp = r[0];
				else temp = r[1];
			}
			else
			{
				for (int i = 1; i < sum; ++i)
				{
					if ((r[i] < temp || temp < 0) && r[i] >= 0) temp = r[i];
				}
			}
			if (temp >= 0 && temp <= time) return temp;
			return -1.0;
		}
	//	return -1.0;
	}
	double getCrashTimerB2P(const XVec2& position1, const XVec2& speed1, const XVec2& a1,
		const XVec2& position2, float distance, float time)
	{//实际上是getCrashTimerB2B这个函数的一种特殊参数的形式
		//化简
		double e = (position1.x - position2.x) * (position1.x - position2.x) + (position1.y - position2.y) * (position1.y - position2.y) - distance * distance;
		if (e < 0) return -1.0f;	//防止结余误差
		double a = (0.25 * a1.x * a1.x + 0.25 * a1.y * a1.y) * 0.001 * 0.001 * 0.001 * 0.001;
		double b = (a1.x * speed1.x + a1.y * speed1.y) * 0.001 * 0.001 * 0.001;
		double c = (speed1.x * speed1.x + (position1.x - position2.x) * a1.x + speed1.y * speed1.y + (position1.y - position2.y) * a1.y) * 0.001 * 0.001;
		double d = (2.0 * speed1.x * (position1.x - position2.x) + 2.0 * speed1.y * (position1.y - position2.y)) * 0.001;
		if (a == 0.0) //则测试b必然也等于0
		{//求解一元二次方程
			if (c == 0) return -1.0;//平行线
			double temp = d * d - 4.0 * c * e;
			if (temp < 0)
			{//无解
				return -1.0;	//不会发生碰撞
			}
			else
			{
				double t2 = (-d - sqrt(temp)) / (2.0 * c);
				if (t2 > 0 && t2 <= time) return t2;
				else return -1.0;
			}
			//		return -1.0;
		}
		else
		{//求解一元四次方程
			double r[4];
			int sum = quarticEquation(a, b, c, d, e, r);
			if (sum == 0) return -1.0;	//无解
			double temp = r[0];
			if (sum == 2)
			{
				if (r[0] < r[1]) temp = r[0];
				else temp = r[1];
			}
			else
			{
				for (int i = 1; i < sum; ++i)
				{
					if ((r[i] < temp || temp < 0) && r[i] >= 0) temp = r[i];
				}
			}
			if (temp >= 0.0 && temp <= time) return temp;
			return -1.0;
		}
	//	return -1.0;
	}
	double getCrashTimerB2L(const XVec2& position1, const XVec2& speed1, const XVec2& a1,
		const XVec2& positionS, const XVec2& positionE, float distance, float time)
	{
		double Ptx, Pty;
		double A, B, C;
		twoPointLine(positionS, positionE, &A, &B, &C);
		double a, b, c;
		double r[4];
		int order = 0;
		a = (A * 0.5 * a1.x + B * 0.5 * a1.y) * 0.001 * 0.001;
		b = (A * speed1.x + B * speed1.y) * 0.001;
		if (a == 0)
		{
			if (b == 0) return -1.0;
			c = A * position1.x + B * position1.y + C + distance * _hypot(A, B);
			double t1 = -c / b;
			c = A * position1.x + B * position1.y + C - distance * _hypot(A, B);
			double t2 = -c / b;
			if (t1 < t2 && t1 >= 0 && t1 <= time)
			{
				Ptx = position1.x + speed1.x * t1 * 0.001;
				Pty = position1.y + speed1.y * t1 * 0.001;
				//下面判断垂足是否在范围内
				if (B != 0.0)
				{
					double x = (B * B * Ptx - A * B * Pty - A * C) / (A * A + B * B);
					if ((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x < positionE.x)) return t1;
				}
				else
				{
					double y = (A * A * Pty - A * B * Ptx - B * C) / (A * A + B * B);
					if ((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y < positionE.y)) return t1;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
				}
			}
			if (t2 < t1 && t2 >= 0 && t2 <= time)
			{
				Ptx = position1.x + speed1.x * t2 * 0.001;
				Pty = position1.y + speed1.y * t2 * 0.001;
				//下面判断垂足是否在范围内
				if (B != 0.0)
				{
					double x = (B * B * Ptx - A * B * Pty - A * C) / (A * A + B * B);
					if ((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x < positionE.x)) return t2;
				}
				else
				{
					double y = (A * A * Pty - A * B * Ptx - B * C) / (A * A + B * B);
					if ((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y < positionE.y)) return t2;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
				}
			}
		}
		else
		{
			c = A * position1.x + B * position1.y + C + distance * _hypot(A, B);
			double temp = b * b - 4.0 * a * c;
			double tmp = 0.5 / a;
			if (temp >= 0.0)
			{
				temp = sqrt(temp);
				r[order] = (-b + temp) * tmp;
				++order;
				r[order] = (-b - temp) * tmp;
				++order;
			}
			c = A * position1.x + B * position1.y + C - distance * _hypot(A, B);
			temp = b * b - 4.0 * a * c;
			if (temp >= 0.0)
			{
				temp = sqrt(temp);
				r[order] = (-b + temp) * tmp;
				++order;
				r[order] = (-b - temp) * tmp;
				++order;
			}
			if (order == 0) return -1.0;	//无解
			if (order == 4)
			{
				if ((A * position1.x + B * position1.y + C) * a < 0)
				{
					if (r[3] < r[1]) temp = r[3];
					else temp = r[1];
				}
				else
				{
					if (r[2] < r[0]) temp = r[2];
					else temp = r[0];
				}
			}
			else
			{
				if (r[0] < r[1]) temp = r[0];
				else temp = r[1];
			}
			//这里需要计算解是否在范围内
			if (temp >= 0 && temp <= time)
			{
				Ptx = position1.x + speed1.x * temp * 0.001 + 0.5 * a1.x * temp * temp * 0.001 * 0.001;
				Pty = position1.y + speed1.y * temp * 0.001 + 0.5 * a1.y * temp * temp * 0.001 * 0.001;
				//下面判断垂足是否在范围内
				if (B != 0.0)
				{
					double x = (B * B * Ptx - A * B * Pty - A * C) / (A * A + B * B);
					if ((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x < positionE.x)) return temp;
				}
				else
				{
					double y = (A * A * Pty - A * B * Ptx - B * C) / (A * A + B * B);
					if ((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y < positionE.y)) return temp;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
				}
			}
		}
		return -1.0;
	}
	double getCrashTimerB2L(const XVec2& position1, const XVec2& speed1, const XVec2& a1,
		const XLine &pL, float distance, float time)
	{
		double Ptx, Pty;
		double A = pL.A;
		double B = pL.B;
		double C = pL.C;
		double D = pL.m_data0;
		double E = pL.m_data1;
		double r[4];
		XVec2 positionS = pL.getStartPosition();
		XVec2 positionE = pL.getEndPosition();
		double a, b, c, d;
		d = A * position1.x + B * position1.y + C;
		//如果点到直线的距离已经小于合法距离，则认为不会发生碰撞
		if (abs(d) - distance * E < 0)
		{//如果点到直线的距离已经小于许可范围
			//下面判断垂足是否在范围内
			if (B != 0.0)
			{
				double x = (B * B * position1.x - A * B * position1.y - A * C) / D;
				if ((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x < positionE.x)) return -1.0f;
			}
			else
			{
				double y = (A * A * position1.x - A * B * position1.y - B * C) / D;
				if ((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y < positionE.y)) return -1.0f;
			}
		}

		a = (A * 0.5 * a1.x + B * 0.5 * a1.y) * 0.001 * 0.001;
		b = (A * speed1.x + B * speed1.y) * 0.001;
		if (a == 0)
		{
			if (b == 0) return -1.0;
			c = d + distance * E;
			double t1 = -c / b;
			c = d - distance * E;
			double t2 = -c / b;
			if (t1 < t2 && t1 >= 0 && t1 <= time)
			{//由于浮点数结余误差这里需要检验这个值，尚未进行
				Ptx = position1.x + speed1.x * t1 * 0.001;
				Pty = position1.y + speed1.y * t1 * 0.001;
				//下面判断垂足是否在范围内
				if (B != 0.0)
				{
					double x = (B * B * Ptx - A * B * Pty - A * C) / D;
					if ((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x < positionE.x)) return t1;
				}
				else
				{
					double y = (A * A * Pty - A * B * Ptx - B * C) / D;
					if ((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y < positionE.y)) return t1;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
				}
			}
			if (t2 < t1 && t2 >= 0 && t2 <= time)
			{
				Ptx = position1.x + speed1.x * t2 * 0.001;
				Pty = position1.y + speed1.y * t2 * 0.001;
				//下面判断垂足是否在范围内
				if (B != 0.0)
				{
					double x = (B * B * Ptx - A * B * Pty - A * C) / D;
					if ((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x < positionE.x)) return t2;
				}
				else
				{
					double y = (A * A * Pty - A * B * Ptx - B * C) / D;
					if ((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y < positionE.y)) return t2;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
				}
			}
		}
		else
		{
			c = d + distance * E;
			double temp = b * b - 4.0 * a * c;
			int order = 0;
			double tmp = 0.5 / a;
			if (temp >= 0.0)
			{
				temp = sqrt(temp);
				r[order] = (-b + temp) * tmp;
				++order;
				r[order] = (-b - temp) * tmp;
				++order;
			}
			c = d - distance * E;
			temp = b * b - 4.0 * a * c;
			if (temp >= 0.0)
			{
				temp = sqrt(temp);
				r[order] = (-b + temp) * tmp;
				++order;
				r[order] = (-b - temp) * tmp;
				++order;
			}
			if (order == 0) return -1.0;	//无解
			if (order == 4)
			{
				if (d * a < 0)
				{
					if (r[3] < r[1]) temp = r[3];
					else temp = r[1];
				}
				else
				{
					if (r[2] < r[0]) temp = r[2];
					else temp = r[0];
				}
				//temp = r[0];
				//for(int i = 1;i < order;++ i)
				//{
				//	if((r[i] < temp || temp < 0) && r[i] >= 0) temp = r[i];
				//}
			}
			else
			{
				if (r[0] < r[1]) temp = r[0];
				else temp = r[1];
			}
			//这里需要计算解是否在范围内
			if (temp >= 0 && temp <= time)
			{
				Ptx = position1.x + speed1.x * temp * 0.001 + 0.5 * a1.x * temp * temp * 0.001 * 0.001;
				Pty = position1.y + speed1.y * temp * 0.001 + 0.5 * a1.y * temp * temp * 0.001 * 0.001;
				//下面判断垂足是否在范围内
				if (B != 0.0)
				{
					double x = (B * B * Ptx - A * B * Pty - A * C) / D;
					if ((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x <= positionE.x)) return temp;
				}
				else
				{
					double y = (A * A * Pty - A * B * Ptx - B * C) / D;
					if ((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y <= positionE.y)) return temp;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
				}
			}
		}
		return -1.0;
	}
	//点到直线距离的公式
	double distancePointToLine(const XVec2& point, const XVec2& lineS, const XVec2& lineE)//ax+by+c=0 x0,y0 |ax0+by0+c|/√(a^2+b^2)
	{
		double a = 0.0;
		double b = 0.0;
		double c = 0.0;
		twoPointLine(lineS, lineE, &a, &b, &c);
		double tmp = a * a + b * b;
		if (tmp == 0.0) return 0.0;
		tmp = 1.0 / tmp;
		return abs(a * point.x + b * point.y + c) * sqrt(tmp);
	}
	double distancePointToLine(const XVec2& point,double a,double b,double c)
	{
		double tmp = a * a + b * b;
		if(tmp == 0.0) return 0.0;
		tmp = 1.0 / tmp;
		return abs(a * point.x + b * point.y + c) * sqrt(tmp);
	}
	//点到直线的垂足公式
	XVec2 curtatePointToLine(const XVec2& point, const XVec2& lineS, const XVec2& lineE)	//设直线方程为ax+by+c=0,点坐标为(m,n)则垂足为((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
	{
		double a = 0.0;
		double b = 0.0;
		double c = 0.0;
		twoPointLine(lineS, lineE, &a, &b, &c);
		double tmp = a * a + b * b;
		if (tmp == 0.0) return XVec2::zero;
		tmp = 1.0 / tmp;
		return XVec2(b * b * point.x - a * b * point.y - a * c,
			a * a * point.y - a * b * point.x - b * c) * tmp;
	}
	XVec2 curtatePointToLine(const XVec2& point, double a, double b, double c)
	{
		double tmp = a * a + b * b;
		if (tmp == 0.0) return XVec2::zero;
		tmp = 1.0 / tmp;
		return XVec2(b * b * point.x - a * b * point.y - a * c,
			a * a * point.y - a * b * point.x - b * c) * tmp;
	}
	//两直线交点的公式
	XVec2 crossPointLineToLine(const XVec2& line1S, const XVec2& line1E, const XVec2& line2S, const XVec2& line2E)
	{
		double a0 = 0.0;
		double b0 = 0.0;
		double c0 = 0.0;
		double a1 = 0.0;
		double b1 = 0.0;
		double c1 = 0.0;
		twoPointLine(line1S, line1E, &a0, &b0, &c0);
		twoPointLine(line2S, line2E, &a1, &b1, &c1);
		double tmp = a0 * b1 - a1 * b0;
		if (tmp == 0.0) return XVec2::zero;
		tmp = 1.0 / tmp;
		return XVec2(b0 * c1 - b1 * c0, a1 * c0 - a0 * c1) * tmp;
	}
	XVec2 crossPointLineToLine(double a0, double b0, double c0, double a1, double b1, double c1)	//y=(a1*c0 - a0*c1)/(a0*b1 - a1*b0),x=(b0*c1 - b1*c0)/(a0*b1 - a1*b0) 
	{
		double tmp = a0 * b1 - a1 * b0;
		if (tmp == 0.0) return XVec2::zero;
		tmp = 1.0 / tmp;
		return XVec2(b0 * c1 - b1 * c0, a1 * c0 - a0 * c1) * tmp;
	}
	//两点直线公式
	XBool twoPointLine(const XVec2& lineS, const XVec2& lineE, double *a, double *b, double *c)//(x-x1)/(x2-x1)=(y-y1)/(y2-y1)
	{
		if (a == NULL || b == NULL || c == NULL) return XFalse;
		(*a) = lineE.y - lineS.y;
		(*b) = lineS.x - lineE.x;
		//(* c) = lineS.y * (lineE.x - lineS.x) - lineS.x * (lineE.y - lineS.y);
		(*c) = -lineS.y * (*b) - lineS.x * (*a);
		return XTrue;
	}
	//点到线段的最短距离
	double minDistancePointToLine(const XVec2& point, const XVec2& lineS, const XVec2& lineE)
	{
		//首先判断垂足是否在线段范围内
		XVec2 tempPoint = curtatePointToLine(point, lineS, lineE);
		//下面的判断存在问题，当出现水平线或者垂直线时，由于浮点数取点误差会造成误判
		//if (((tempPoint.x <= lineS.x && tempPoint.x >= lineE.x)
		//	|| (tempPoint.x >= lineS.x && tempPoint.x <= lineE.x))
		//	&& ((tempPoint.y <= lineS.y && tempPoint.y >= lineE.y)
		//		|| (tempPoint.y >= lineS.y && tempPoint.y <= lineE.y)))
		//下面时对上面算法的改进
		if((lineS.x == lineE.x  && XMath::isInArea(tempPoint.y, lineS.y, lineE.y)) ||
			(lineS.y == lineE.y  && XMath::isInArea(tempPoint.x, lineS.x, lineE.x)) ||
			(XMath::isInArea(tempPoint.y, lineS.y, lineE.y) && XMath::isInArea(tempPoint.x, lineS.x, lineE.x)))
		{//垂足在线段范围内
			return distancePointToLine(point, lineS, lineE);
		}
		else
		{//垂足不在线段范围内,返回两个端点中最近的端点距离
			return (std::min)(point.getLength(lineS), point.getLength(lineE));
		}
		//	return 0.0;
	}
	//两线段的最短距离
	//XVec2 minDistanceTwoLine(const XVec2& line1S,const XVec2& line1E,const XVec2& line2S,const XVec2& line2E)
	XVec2 minDistanceTwoLine(const XVec2&, const XVec2&, const XVec2&, const XVec2&)
	{//尚未实现
		return XVec2::zero;
	}
	//使用射线法(这个方法可以扩展到任意多边形)
	XBool getIsInRect(const XVec2& p, const XVec2& p1, const XVec2& p2, const XVec2& p3, const XVec2& p4)
	{
		int crashSum = 0;	//射线相交的次数 
		if (p1.y != p2.y && p.y >= (std::min)(p1.y, p2.y) && p.y < (std::max)(p1.y, p2.y) && 	//不平行	
			(p.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x > p.x)
			++crashSum;
		if (p2.y != p3.y && p.y >= (std::min)(p2.y, p3.y) && p.y < (std::max)(p2.y, p3.y) &&	//不平行	
			(p.y - p2.y) * (p3.x - p2.x) / (p3.y - p2.y) + p2.x > p.x)
			++crashSum;
		if (p3.y != p4.y && p.y >= (std::min)(p3.y, p4.y) && p.y < (std::max)(p3.y, p4.y) &&	//不平行	
			(p.y - p3.y) * (p4.x - p3.x) / (p4.y - p3.y) + p3.x > p.x)
			++crashSum;
		if (p4.y != p1.y && p.y >= (std::min)(p4.y, p1.y) && p.y < (std::max)(p4.y, p1.y) &&	//不平行	
			(p.y - p4.y) * (p1.x - p4.x) / (p1.y - p4.y) + p4.x > p.x)
			++crashSum;
		//return (crashSum % 2 != 0);
		return ((crashSum & 0x01) != 0);
	}
	XBool getIsInRect(float x, float y, const XVec2& p1, const XVec2& p2, const XVec2& p3, const XVec2& p4)
	{
		return getIsInRect(XVec2(x, y), p1, p2, p3, p4);
	}
	XBool getIsInPolygon(const XVec2& p, const XVec2 *pR, int pRSum)
	{
		if (pR == NULL || pRSum <= 2) return XFalse;	//输入参数合理性检查
		int crashSum = 0;	//射线相交的次数 
		for (int i = 0; i < pRSum - 1; ++i)
		{
			const XVec2& ref = pR[i + 1];
			if (pR[i].y != ref.y && p.y >= (std::min)(pR[i].y, ref.y) && p.y < (std::max)(pR[i].y, ref.y) &&
				(p.y - pR[i].y) * (ref.x - pR[i].x) / (ref.y - pR[i].y) + pR[i].x > p.x)
				++crashSum;
		}
		//判断闭合点
		const XVec2& ref = pR[pRSum - 1];
		if (ref.y != pR[0].y && p.y >= (std::min)(ref.y, pR[0].y) && p.y < (std::max)(ref.y, pR[0].y) &&
			(p.y - ref.y) * (pR[0].x - ref.x) / (pR[0].y - ref.y) + ref.x > p.x)
			++crashSum;
		//return (crashSum % 2 != 0);
		return ((crashSum & 0x01) != 0);
	}
	XBool getIsInPolygon(float x,float y,const XVec2 *pR,int pRSum)
	{
		return getIsInPolygon(XVec2(x,y),pR,pRSum);
	}
	XBool getIsInTriangle(const XVec2& p, const XVec2* points)
	{
		if (points == nullptr) return XFalse;
		XVec2 PA = points[0] - p;
		XVec2 PB = points[1] - p;
		XVec2 PC = points[2] - p;
		double t1 = PA.cross(PB);
		return t1 * PB.cross(PC) >= 0.0f && t1 * PC.cross(PA) >= 0.0f;
	}
	#define COHEN_Left 1
	#define COHEN_Right 2
	#define COHEN_Bottom 4
	#define COHEN_Top 8
	inline int cohenCode(const XVec2& p,const XRect& r)
	{//点相对于矩形的关系
		int c = 0;
		if(p.x < r.left) c |= COHEN_Left;
		if(p.x > r.right) c |= COHEN_Right;
		if(p.y > r.bottom) c |= COHEN_Bottom;
		if(p.y < r.top)	c |= COHEN_Top;
		return c;
	}
	int getPointsLineToRect(const XVec2& s, const XVec2& e, const XRect& r, XVec2& ps, XVec2& pe)
	{//cohenProcess
		int intCode;
		float x = 0, y = 0;

		int intCode1 = cohenCode(s, r);
		int intCode2 = cohenCode(e, r);
		ps = s;
		pe = e;
		int i = 0;//计算交点的数量，最多有2个交点
		while (intCode1 != 0 || intCode2 != 0)
		{
			if ((intCode1 & intCode2) != 0)
				return 0;	//没有交点
			++i;
			intCode = intCode1;
			if (intCode1 == 0)
				intCode = intCode2;
			if ((COHEN_Left & intCode) != 0)
			{
				x = r.left;
				y = ps.y + (pe.y - ps.y) * (r.left - ps.x) / (pe.x - ps.x);
			}
			else if ((COHEN_Right & intCode) != 0)
			{
				x = r.right;
				y = ps.y + (pe.y - ps.y) * (r.right - ps.x) / (pe.x - ps.x);
			}
			else if ((COHEN_Bottom & intCode) != 0)
			{
				y = r.bottom;
				x = ps.x + (pe.x - ps.x) * (r.bottom - ps.y) / (pe.y - ps.y);
			}
			else if ((COHEN_Top & intCode) != 0)
			{
				y = r.top;
				x = ps.x + (pe.x - ps.x) * (r.top - ps.y) / (pe.y - ps.y);
			}

			if (intCode == intCode1)
			{
				if (i == 1)
				{
					ps.set(x, y);
					intCode1 = cohenCode(ps, r);
				}
				else
				{
					ps = pe;
					intCode1 = intCode2;
					pe.set(x, y);
					intCode2 = cohenCode(pe, r);
				}
			}
			else
			{
				if (i == 1)
				{
					pe = ps;
					intCode2 = intCode1;
					ps.set(x, y);
					intCode1 = cohenCode(ps, r);
				}
				else
				{
					pe.set(x, y);
					intCode2 = cohenCode(pe, r);
				}
			}
		}
		return i;	//有交点
	}
	//一次方程:ax + b = 0
	int onceEquation(double a, double b, std::complex<double> &z)
	{
		if (a == 0.0)
		{//没有意义
			z = 0;	//无效
			return 0;
		}
		else
		{
			z = -b / a;
			return 1;
		}
	}
	//二次方程:ax^2 + bx + c = 0
	int quadraticEquation(double a, double b, double c, std::complex<double>& z1,
		std::complex<double>& z2)
	{
		if (a == 0)
		{
			z2 = 0;	//无效
			return onceEquation(b, c, z1);	//实际上只有一个解
		}
		else
		{
			double delta;
			std::complex<double> temp1, temp2;

			delta = b*b - 4.0*a*c;

			std::complex<double> temp(delta, 0);

			temp1 = (-b) / (2.0*a);
			temp2 = sqrt(temp) / (2.0*a);

			z1 = temp1 + temp2;
			z2 = temp1 - temp2;
			return 2;
		}
	}
	//三次方程:ax^3 + bx^2 + cx + d = 0;
	int cubicEquation(double a, double b, double c, double d, std::complex<double>& z1,
		std::complex<double>& z2, std::complex<double>& z3)
	{
		double p, q, delta;
		double M, N;

		std::complex<double> temp1, temp2;
		if (a == 0)
		{
			return quadraticEquation(b, c, d, z1, z2);
		}
		else
		{
			p = -pow(b / a, 2.0) / 3.0 + c / a;
			q = 2.0 / 27.0 * pow(b / a, 3.0) - b * c / (a*a*3.0) + d / a;

			delta = 0.25 * q * q + p * p * p / 27.0;

			std::complex<double> omega1(-0.5, sqrt(3.0) * 0.5);
			std::complex<double> omega2(-0.5, -sqrt(3.0) * 0.5);

			std::complex<double> yy(b / (3.0*a), 0.0);

			M = -q * 0.5;

			if (delta < 0)
			{
				N = sqrt(fabs(delta));
				std::complex<double> s1(M, N);
				std::complex<double> s2(M, -N);

				z1 = (pow(s1, 1.0 / 3.0) + pow(s2, 1.0 / 3.0)) - yy;
				z2 = (pow(s1, 1.0 / 3.0)*omega1 + pow(s2, 1.0 / 3.0)*omega2) - yy;
				z3 = (pow(s1, 1.0 / 3.0)*omega2 + pow(s2, 1.0 / 3.0)*omega1) - yy;
			}
			else
			{
				N = sqrt(delta);

				std::complex<double>  f1(M + N, 0);
				std::complex<double>  f2(M - N, 0);

				if (M + N >= 0)
					temp1 = pow(f1, 1.0 / 3.0);
				else
					temp1 = -norm(pow(sqrt(f1), 1.0 / 3.0));

				if (M - N >= 0)
					temp2 = pow(f2, 1.0 / 3.0);
				else
					temp2 = -norm(pow(sqrt(f2), 1.0 / 3.0));

				z1 = temp1 + temp2 - yy;
				z2 = omega1*temp1 + omega2*temp2 - yy;
				z3 = omega2*temp1 + omega1*temp2 - yy;
			}
			return 3;
		}
	}
	int quarticEquation(double a, double b, double c, double d, double e, double *x)
	{
		std::complex<double> r1, r2, r3, r4;
		int sum = quarticEquation(a, b, c, d, e, r1, r2, r3, r4);
		if (sum == 0) return 0;	//无解
		int rSum = 0;
		if (r1.imag() == 0)
		{
			x[rSum] = r1.real();
			++rSum;
		}
		if (r2.imag() == 0)
		{
			x[rSum] = r2.real();
			++rSum;
		}
		if (r3.imag() == 0)
		{
			x[rSum] = r3.real();
			++rSum;
		}
		if (r4.imag() == 0)
		{
			x[rSum] = r4.real();
			++rSum;
		}
		return rSum;
	}
	//四次方程:ax^4 + bx^3 + cx^2 + dx + e = 0
	int quarticEquation(double a, double b, double c, double d, double e,
		std::complex<double>& x1, std::complex<double>& x2,
		std::complex<double>& x3, std::complex<double>& x4)
	{
		double b1, c1, d1;
		double y;

		//输入一元四次方程的系数，得方程为a*x^4+b*x^3+c*x^2+d*x+e=0；

		if (a == 0)
		{
			x4 = 0;
			return cubicEquation(b, c, d, e, x1, x2, x3);
		}
		else if (b == 0 && c == 0 && d == 0 && e == 0)
		{
			x1 = 0;
			x2 = 0;
			x3 = 0;
			x4 = 0;
			return 4;
		}
		else if (b == 0 && c == 0 && d == 0)
		{//有待进一步确定
			if (-e / a < 0)
			{
				return 0;	//无解
			}
			else
			{
				x4 = sqrt(-e / a);
				x1 = sqrt(x4);
				x2 = -sqrt(x4);
				x3 = sqrt(-x4);
				x4 = -sqrt(-x4);
				return 4;
			}
		}
		else if (b == 0 && d == 0 && e == 0)
		{
			return quadraticEquation(a, 0.0, c, x1, x2);
		}
		else
		{
			//把任意系数的四次方程化为首项系数为1的四次方程；
			a = 1.0 / a;
			b *= a;
			c *= a;
			d *= a;
			e *= a;
			a = 1.0;

			//所调用的三次方程的系数;	
			b1 = -4.0*c;
			c1 = 2.0*b*d - 8.0*e;
			d1 = e*(4.0*c - b*b) - d*d;

			cubicEquation(8.0, b1, c1, d1, x1, x2, x3);
			y = real(x1);
			//把三次方程的返回值赋给 y

			//第一次调用的二次方程的系数
			b1 = b*0.5 - sqrt(8.0*y + b*b - 4.0*c)*0.5;
			c1 = y - (b*y - d) / sqrt(8.0*y + b*b - 4.0*c);
			quadraticEquation(1.0, b1, c1, x1, x2);

			//第二次调用的二次方程的系数
			b1 = b*0.5 + sqrt(8.0*y + b*b - 4.0*c)*0.5;
			c1 = y + (b*y - d) / sqrt(8.0*y + b*b - 4.0*c);
			//	b1 = b - b1;
			//	c1 = 2.0 * y - c1;
			quadraticEquation(1.0f, b1, c1, x3, x4);
			return 4;
		}
	}
	float circleLineMapping(float in, float inStart, float inEnd, float inMin,
		float inMax, float outStart, float outEnd, float outMin, float outMax)
	{
		if (inStart <= inEnd)
		{
			if (outStart <= outEnd)
			{//正常的线性映射
				return mapping1D(in, inStart, inEnd, outStart, outEnd);
			}
			else
			{//结果反序的映射
				float ret = mapping1D(in, inStart, inEnd, outStart, outMax + inEnd - outMin);
				if (ret >= outMax) return ret - outMax + outMin;
				return ret;
			}
		}
		else
		{
			if (in < inEnd) in = in - inMin + inMax;
			if (outStart <= outEnd)
			{//正常的线性映射 X
				return mapping1D(in, inStart, inMax + inEnd - inMin, outStart, outEnd);
			}
			else
			{//X
				float ret = mapping1D(in, inStart, inMax + inEnd - inMin, outStart, outMax + inEnd - outMin);
				if (ret >= outMax) return ret - outMax + outMin;
				return ret;
			}
		}
	}
	//XVec2 mapping2D(const XVec2 *inRect,const XVec2 *outRect,const XVec2& in)
	//{
	//	//0   1
	//	//3   2
	//	//检查数据的合法性
	//	//第一步计算系数
	//	double ax1 = inRect[0].x;
	//	double kx1 = inRect[1].x - inRect[0].x;
	//	double ay1 = inRect[0].y;
	//	double ky1 = inRect[1].y - inRect[0].y;
	//	double ax2 = inRect[3].x;
	//	double kx2 = inRect[2].x - inRect[3].x;
	//	double ay2 = inRect[3].y;
	//	double ky2 = inRect[2].y - inRect[3].y;
	//
	//	double bx = in.x - ax1;
	//	double Kx1 = ax2 - ax1;
	//	double by = in.y - ay1;
	//	double Ky1 = ay2 - ay1;
	//	double cx = kx1;
	//	double Kx2 = kx2 - kx1;
	//	double cy = ky1;
	//	double Ky2 = ky2 - ky1;
	//
	//	double a = Kx1 * Ky2 - Ky1 * Kx2;
	//	double b = by * Kx2 - cx * Ky1 + cy * Kx1 - bx * Ky2;
	//	double c = cx * by - bx * cy;
	//
	//	double temp = b * b - 4.0f * a * c;
	//	if(a == 0.0f)
	//	{
	//		if(b == 0.0f) return XVec2::zero;
	//		double yt = -c/b;
	//		double xt = cx + Kx2 * yt;
	//		if(xt == 0.0f) return XVec2::zero;
	//		xt = (bx - Kx1 * yt) / xt;
	//		if(xt >= 0.0f && xt <= 1.0f)
	//		{//合法的值
	//			XVec2 ret;
	//			double x1 = outRect[0].x + (outRect[1].x - outRect[0].x) * xt;
	//			double y1 = outRect[0].y + (outRect[1].y - outRect[0].y) * xt;
	//			double x2 = outRect[3].x + (outRect[2].x - outRect[3].x) * xt;
	//			double y2 = outRect[3].y + (outRect[2].y - outRect[3].y) * xt;
	//			ret.x = x1 + (x2 - x1) * yt;
	//			ret.y = y1 + (y2 - y1) * yt;
	//			return ret;
	//		}
	//	}
	//	if(temp >= 0.0f)
	//	{//有解
	//		temp = sqrt(temp);
	//		double yt0 = (-b - temp) / (2.0f * a);
	//		double yt1 = (-b + temp) / (2.0f * a);
	//
	//		if(yt0 >= 0.0f && yt0 <= 1.0f)
	//		{
	//			double xt = cx + Kx2 * yt0;
	//			if(xt == 0.0f) return XVec2::zero;
	//			xt = (bx - Kx1 * yt0) / xt;
	//			if(xt >= 0.0f && xt <= 1.0f)
	//			{//合法的值
	//				XVec2 ret;
	//				double x1 = outRect[0].x + (outRect[1].x - outRect[0].x) * xt;
	//				double y1 = outRect[0].y + (outRect[1].y - outRect[0].y) * xt;
	//				double x2 = outRect[3].x + (outRect[2].x - outRect[3].x) * xt;
	//				double y2 = outRect[3].y + (outRect[2].y - outRect[3].y) * xt;
	//				ret.x = x1 + (x2 - x1) * yt0;
	//				ret.y = y1 + (y2 - y1) * yt0;
	//				return ret;
	//			}
	//		}
	//		if(yt1 >= 0.0f && yt1 <= 1.0f)
	//		{
	//			double xt = cx + Kx2 * yt1;
	//			if(xt == 0.0f) return XVec2::zero;
	//			xt = (bx - Kx1 * yt1) / xt;
	//			if(xt >= 0.0f && xt <= 1.0f)
	//			{//合法的值
	//				XVec2 ret;
	//				double x1 = outRect[0].x + (outRect[1].x - outRect[0].x) * xt;
	//				double y1 = outRect[0].y + (outRect[1].y - outRect[0].y) * xt;
	//				double x2 = outRect[3].x + (outRect[2].x - outRect[3].x) * xt;
	//				double y2 = outRect[3].y + (outRect[2].y - outRect[3].y) * xt;
	//				ret.x = x1 + (x2 - x1) * yt1;
	//				ret.y = y1 + (y2 - y1) * yt1;
	//				return ret;
	//			}
	//		}
	//	}
	//	return XVec2::zero;
	//}
	//0   1
	//3   2
	XVec2 mapping2D(const XVec2* inRect,const XVec2* outRect,const XVec2& in)
	{
		double bx = in.x - inRect[0].x;
		double Kx1 = inRect[3].x - inRect[0].x;
		double by = in.y - inRect[0].y;
		double Ky1 = inRect[3].y - inRect[0].y;
		double cx = inRect[1].x - inRect[0].x;
		double Kx2 = (inRect[2].x - inRect[3].x) - (inRect[1].x - inRect[0].x);
		double cy = inRect[1].y - inRect[0].y;
		double Ky2 = (inRect[2].y - inRect[3].y) - (inRect[1].y - inRect[0].y);

		double a = Kx1 * Ky2 - Ky1 * Kx2;
		double b = by * Kx2 - cx * Ky1 + cy * Kx1 - bx * Ky2;
		double c = cx * by - bx * cy;

		if(a == 0.0)
		{
			if(b == 0.0) return XVec2::zero;
			double yt = -c/b;
			double xt = cx + Kx2 * yt;
			if(xt == 0.0) return XVec2::zero;
			xt = (bx - Kx1 * yt) / xt;
			if(xt >= 0.0 && xt <= 1.0)
			{
				double x1 = outRect[0].x + (outRect[1].x - outRect[0].x) * xt;
				double y1 = outRect[0].y + (outRect[1].y - outRect[0].y) * xt;
				double x2 = outRect[3].x + (outRect[2].x - outRect[3].x) * xt;
				double y2 = outRect[3].y + (outRect[2].y - outRect[3].y) * xt;
				return XVec2(x1 + (x2 - x1) * yt,y1 + (y2 - y1) * yt);
			}
		}
		double temp = b * b - 4.0f * a * c;
		if(temp >= 0.0)
		{
			temp = sqrt(temp);
			double yt0 = (-b - temp) / (2.0 * a);
			double yt1 = (-b + temp) / (2.0 * a);

			if(yt0 >= 0.0 && yt0 <= 1.0)
			{
				double xt = cx + Kx2 * yt0;
				if(xt == 0.0) return XVec2::zero;
				xt = (bx - Kx1 * yt0) / xt;
				if(xt >= 0.0 && xt <= 1.0)
				{
					double x1 = outRect[0].x + (outRect[1].x - outRect[0].x) * xt;
					double y1 = outRect[0].y + (outRect[1].y - outRect[0].y) * xt;
					double x2 = outRect[3].x + (outRect[2].x - outRect[3].x) * xt;
					double y2 = outRect[3].y + (outRect[2].y - outRect[3].y) * xt;
					return XVec2(x1 + (x2 - x1) * yt0,y1 + (y2 - y1) * yt0);
				}
			}
			if(yt1 >= 0.0 && yt1 <= 1.0)
			{
				double xt = cx + Kx2 * yt1;
				if(xt == 0.0) return XVec2::zero;
				xt = (bx - Kx1 * yt1) / xt;
				if(xt >= 0.0 && xt <= 1.0)
				{
					double x1 = outRect[0].x + (outRect[1].x - outRect[0].x) * xt;
					double y1 = outRect[0].y + (outRect[1].y - outRect[0].y) * xt;
					double x2 = outRect[3].x + (outRect[2].x - outRect[3].x) * xt;
					double y2 = outRect[3].y + (outRect[2].y - outRect[3].y) * xt;
					return XVec2(x1 + (x2 - x1) * yt1,y1 + (y2 - y1) * yt1);
				}
			}
		}
		return XVec2::zero;
	}
	XVec2 mapping2DPlus(const XVec2 *inRect, const XVec2 *outRect, const XVec2& in)
	{
		double bx = in.x - inRect[0].x;
		double Kx1 = inRect[3].x - inRect[0].x;
		double by = in.y - inRect[0].y;
		double Ky1 = inRect[3].y - inRect[0].y;
		double cx = inRect[1].x - inRect[0].x;
		double Kx2 = (inRect[2].x - inRect[3].x) - (inRect[1].x - inRect[0].x);
		double cy = inRect[1].y - inRect[0].y;
		double Ky2 = (inRect[2].y - inRect[3].y) - (inRect[1].y - inRect[0].y);

		double a = Kx1 * Ky2 - Ky1 * Kx2;
		double b = by * Kx2 - cx * Ky1 + cy * Kx1 - bx * Ky2;
		double c = cx * by - bx * cy;

		double yt = 0.0;
		if (a == 0.0)
		{
			if (b == 0.0) return XVec2::zero;
			yt = -c / b;
		}
		else
		{
			double temp = b * b - 4.0f * a * c;
			if (temp < 0.0) return XVec2::zero;
			yt = (-b - sqrt(temp)) / (2.0 * a);
		}
		double xt = cx + Kx2 * yt;
		if (xt == 0.0) return XVec2::zero;
		xt = (bx - Kx1 * yt) / xt;

		double x1 = outRect[0].x + (outRect[1].x - outRect[0].x) * xt;
		double y1 = outRect[0].y + (outRect[1].y - outRect[0].y) * xt;
		double x2 = outRect[3].x + (outRect[2].x - outRect[3].x) * xt;
		double y2 = outRect[3].y + (outRect[2].y - outRect[3].y) * xt;
		return XVec2(x1 + (x2 - x1) * yt, y1 + (y2 - y1) * yt);
	}
	XVec2 mapping2DTriangle(const XVec2* inTriangle, const XVec2* outTriangle, const XVec2& in)
	{//下面的方法不对
		//从一个顶点过这个点与另一条边的交点
		XVec2 tmpA = inTriangle[1] - inTriangle[0];
		XVec2 tmpB = inTriangle[2] - inTriangle[0];
		float tmpRate = tmpB.cross(tmpA);
		if (tmpRate == 0.0f) return XVec2::zero;	//非法的值
		//float b = (in - inTriangle[0]).cross(tmpA) / tmpRate;
		//float a = (in - inTriangle[0]).cross(tmpB) / (-tmpRate);
		return outTriangle[0] + (outTriangle[1] - outTriangle[0]) * (in - inTriangle[0]).cross(tmpB) / (-tmpRate) +
			(outTriangle[2] - outTriangle[0]) * (in - inTriangle[0]).cross(tmpA) / tmpRate;
	}
	XVec2 mapping2DEx(const XVec2 *inRect,const XVec2 *outRect,const XVec2& in)
	{
		if(XMath::getIsInTriangle(in,inRect))
			return XMath::mapping2DTriangle(inRect,outRect,in);
		else
		{
			XVec2 outT[3],inT[3];
			outT[0] = outRect[0];
			outT[1] = outRect[2];
			outT[2] = outRect[3];
			inT[0] = inRect[0];
			inT[1] = inRect[2];
			inT[2] = inRect[3];
			return mapping2DTriangle(inT,outT,in);
		}
	}
	XVec2 vectorFieldLineSlerp(const XVec2* posRect, const XVec2* vectorRect, const XVec2& in)
	{
		//方案1：比较符合逻辑，但是计算量较大，需要多次开方
		//float l0 = in.getLength(posRect[0]);
		//float l1 = in.getLength(posRect[1]);
		//float l2 = in.getLength(posRect[2]);
		//float l3 = in.getLength(posRect[3]);
		//float r0 = l1 * l2 * l3;
		//float r1 = l0 * l2 * l3;
		//float r2 = l0 * l1 * l3;
		//float r3 = l0 * l1 * l2;
		//return (vectorRect[0] * r0 + vectorRect[1] * r1 + vectorRect[2] * r2 + vectorRect[3] * r3) / (r0 + r1 + r2 + r3);
		//方案2：避开开方的计算,优化计算量
		double l0 = in.getLengthSqure(posRect[0]);
		double l1 = in.getLengthSqure(posRect[1]);
		double l2 = in.getLengthSqure(posRect[2]);
		double l3 = in.getLengthSqure(posRect[3]);
		double r0 = l1 * l2 * l3;
		double r1 = l0 * l2 * l3;
		double r2 = l0 * l1 * l3;
		double r3 = l0 * l1 * l2;
		return (vectorRect[0] * r0 + vectorRect[1] * r1 + vectorRect[2] * r2 + vectorRect[3] * r3) / (r0 + r1 + r2 + r3);
	}
	float scalarFieldLineSlerp(const XVec2* posRect,const float* scalarRect,const XVec2& in)
	{
		//方案1：比较符合逻辑，但是计算量较大，需要多次开方
		//float l0 = in.getLength(posRect[0]);
		//float l1 = in.getLength(posRect[1]);
		//float l2 = in.getLength(posRect[2]);
		//float l3 = in.getLength(posRect[3]);
		//float r0 = l1 * l2 * l3;
		//float r1 = l0 * l2 * l3;
		//float r2 = l0 * l1 * l3;
		//float r3 = l0 * l1 * l2;
		//return (scalarRect[0] * r0 + scalarRect[1] * r1 + scalarRect[2] * r2 + scalarRect[3] * r3) / (r0 + r1 + r2 + r3);
		//方案2：避开开方的计算,优化计算量
		double l0 = in.getLengthSqure(posRect[0]);
		double l1 = in.getLengthSqure(posRect[1]);
		double l2 = in.getLengthSqure(posRect[2]);
		double l3 = in.getLengthSqure(posRect[3]);
		double r0 = l1 * l2 * l3;
		double r1 = l0 * l2 * l3;
		double r2 = l0 * l1 * l3;
		double r3 = l0 * l1 * l2;
		return (scalarRect[0] * r0 + scalarRect[1] * r1 + scalarRect[2] * r2 + scalarRect[3] * r3) / (r0 + r1 + r2 + r3);
	}
	double getValueMatrix(const double* a, int n)
	{//基本通过验证,计算多维行列式的值
		if (n == 1) return a[0]; 
		if (n == 2) return a[0] * a[3] - a[1] * a[2]; 
		if (n == 3) return a[0] * a[4] * a[8] + a[1] * a[5] * a[6] + a[2] * a[3] * a[7] - a[2] * a[4] * a[6] - a[1] * a[3] * a[8] - a[0] * a[5] * a[7];
		double ret = 0.0;
		std::vector<double> b;
		std::vector<double> c;
		for (int i = 0; i < n; ++i)
			b.push_back(a[i * n] * (((i % 2) == 0) ? 1.0 : -1.0));

		std::vector<double> data;
		int index = 0;
		for (int i = 0; i < n; ++i)
		{
			index = i * n + 1;
			for (int j = 0; j < n - 1; ++j, ++index)
			{
				data.push_back(a[index]);
			}
		}
		for (int i = 0; i < n; ++i)
		{
			index = i * (n - 1);
			for (int j = 0; j < n - 1; ++j, ++index)
			{
				c.push_back(data[index]);//这里可以用内存拷贝进行优化
			}
		}
		for (int i = 0; i < n; ++i)
		{
			for (int j = i; j < n - 1; ++j)
			{
				index = j * (n - 1);
				for (int k = 0; k < n - 1; ++k, ++index)
				{
					data[index] = data[index + n - 1];//这里可以用内存拷贝进行优化
				}
			}
			ret += b[i] * getValueMatrix(&data[0], n - 1);
			for (int j = 0; j < n; ++j)
			{
				index = j * (n - 1);
				for (int k = 0; k < n - 1; ++k)
				{
					data[index] = c[index];	//这里可以用内存拷贝进行优化
				}
			}
		}
		return ret;
	}
	//modbus的CRC16校验查表算法
	unsigned short CRC16_Modbus(unsigned char * p,unsigned short len)     
	{
		static const unsigned short wCRCTable[] = {
			0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
			0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
			0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
			0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
			0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
			0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
			0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
			0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
			0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
			0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
			0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
			0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
			0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
			0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
			0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
			0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
			0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
			0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
			0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
			0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
			0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
			0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
			0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
			0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
			0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
			0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
			0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
			0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
			0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
			0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
			0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
			0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 };
		unsigned char nTemp;
		unsigned short wCRCWord = 0xFFFF;
		while (len--)
		{
			nTemp = *p++ ^ wCRCWord;
			wCRCWord >>= 8;
			wCRCWord  ^= wCRCTable[(nTemp & 0xFF)];
		}
		return wCRCWord;
	}
	//modbus的CRC16校验计算
	//unsigned short crcPoly16 = 0xa001;// 该位为简式书写 实际为0x11021
	//unsigned short calccrc(unsigned char crcbuf,unsigned short crc)
	//{
	//	crc = crc ^ crcbuf;
	//	for(int i = 0;i < 8;++ i)
	//	{
	//		unsigned char chk = crc & 1;
	//		crc = crc>>1;
	//		crc = crc&0x7fff;
	//		if(chk == 1)
	//			crc=crc^crcPoly16;
	//		crc=crc&0xffff;
	//	}
	//	return crc;
	//}
	//unsigned short CRC16T(unsigned char *buf,unsigned short len)
	//{
	//	unsigned short crc = 0xffff;	//0xffff
	//	for(unsigned short i = 0;i < len;++ i)
	//	{
	//		crc = calccrc(buf[i],crc);
	//	}
	//	return crc;
	//}
	////XMODEN 计算
	//unsigned short crcPoly16 = 0x1021
	//unsigned short CRC16T(unsigned char *buf,unsigned short len)
	//{  
	//	unsigned short crc = 0x00;          // initial value   
	//	for (int index = 0 ; index < len; index++) 
	//	{  
	//		unsigned char b = buf[index];  
	//		for (int i = 0; i < 8; i++) 
	//		{  
	//			boolean bit = ((b   >> (7-i) & 1) == 1);  
	//			boolean c15 = ((crc >> 15    & 1) == 1);  
	//			crc <<= 1;  
	//			if(c15 ^ bit) crc ^= crcPoly16;  
	//		}  
	//	} 
	//	return crc;     
	//}
	//XMODEL的官方代码
	//int calcrc(char *ptr, int count)
	//{
	//    int crc = 0;
	//    char i;
	//    while (--count >= 0)
	//    {
	//        crc = crc ^ (int) *ptr++ << 8;
	//        i = 8;
	//        do
	//        {
	//            if (crc & 0x8000)
	//                crc = crc << 1 ^ 0x1021;
	//            else
	//                crc = crc << 1;
	//        } while(--i);
	//    }
	//    return (crc);
	//}
	//XMODEN 查表
	unsigned short CRC16XModen(unsigned char *buf,unsigned short len)
	{
		static const unsigned short crc16tab[256]= {
			 0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
			 0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
			 0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
			 0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
			 0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
			 0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
			 0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
			 0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
			 0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
			 0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
			 0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
			 0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
			 0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
			 0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
			 0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
			 0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
			 0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
			 0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
			 0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
			 0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
			 0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
			 0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
			 0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
			 0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
			 0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
			 0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
			 0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
			 0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
			 0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
			 0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
			 0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
			 0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0};
		unsigned char nTemp;
		unsigned short crc = 0;
		for(int counter = 0; counter < len;++ counter)
		{
			nTemp = (crc>>8) ^ *(char *)buf++;
			crc = (crc<<8) ^ crc16tab[(nTemp & 0x00FF)];
		}
		return crc;
	}
#ifdef SIN_BOOK
	float sinData[628] = { 0 };

	void initSinData()
	{
		for (int i = 0; i < 628; ++i)
		{
			sinData[i] = (float)(sin(PI2 * i / 628.0));
		}
	}
	float xSin(float x)
	{
		int temp = (int)(x * 100.0f);
		while (true)
		{
			if (temp < 0) temp += 628;
			else break;
		}
		if (temp >= 628) temp = temp % 628;

		return sinData[temp];
	}
#endif

#ifdef SQRT_BOOK
	float sqrtData[1000000] = { 0 };

	void initSqrtData()
	{
		sinData[0] = 0;
		for (int i = 1; i < 1000000; ++i)
		{
			sqrtData[i] = sqrt((double)i);
		}
	}

	float xSqrt(float x)
	{
		if (x < 0 || x >= 1000000) return 0;
		return sqrtData[(int)x];
	}
#endif
	//static char codeTable36[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static char codeTable36[37] = "C015PQREB6$YDWUV4SZ8GX73JFHAKL92NiMT";
	std::string to36Encode(const unsigned char *data,int len)
	{//这个函数是不可逆的
		if(data == NULL || len <= 0) return XString::gNullStr;
		unsigned int tmp = data[0];
		//int cur;
		std::string ret;
		for(int i = 1;i < len;++ i)
		{
		//	cur = tmp % 36;
		//	if(cur < 10) ret.push_back(cur + '0');
		//	else ret.push_back(cur - 10 + 'A');
			ret.push_back(codeTable36[tmp % 36]);
			tmp = tmp / 36 + data[i] * 64;
		}
		//cur = tmp % 36;
		//if(cur < 10) ret.push_back(cur + '0');
		//else ret.push_back(cur - 10 + 'A');
		ret.push_back(codeTable36[tmp % 36]);
		return ret;
	}
	//static char codeTable64[65] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+=";
	//static char codeTable64[65] = "0123456789abcdefghijk#mnopqrstuvwxyzABCDEFGHIJKLMN$PQRSTUVWXYZ+=";
	static char codeTable64[65] = "uvVg7oHjzwmpkIAEsfRn2CxT3QGeZ8b1D#XP6+d5F=KcaNUMrqSYW0Jy49$BhitL";
	inline int getCodeTableIndex(char ch)
	{
		for(int i = 0;i < 64;++ i)
		{
			if(codeTable64[i] == ch) return i;
		}
		return 0;
	}
	std::string to64Encode(const unsigned char *data,int len)						//将长数据转换成64进制字符串0-9 A-Z a-z +=
	{//注意如果有多出来的部分需要清除掉
		std::string ret = "";
		unsigned int tmp = 0;
		unsigned int tmpX = data[0];
		//int cur = 0;
		int curLen = 0;
		for(int i = 0;;)
		{
			if (curLen < 6)
			{//数据不够处理一次则考虑是否需要加入新的数据
				if (i >= len)
				{//所有数据处理完毕，准备处理尾数或者退出
					if (curLen == 0) break;
					ret.push_back(codeTable64[tmp & 0x3f]);	//取低6个字节
					break;
				}
				else
				{//还有新的数据可以加入
					if (curLen != 0)
					{
						tmpX = data[i];
						tmp += (tmpX << curLen);
					}
					else tmp = data[i];
				
					++ i;
					curLen += 8;
				}
			}
			ret.push_back(codeTable64[tmp & 0x3f]);	//取低6个字节
			tmp = tmp >> 6;
			curLen -= 6;
		}
		return ret;
	}
	void to64Decode(unsigned char *data,int len,const std::string& code)	//将64进制字符串转换成数据
	{
		unsigned int cur;
		unsigned int tmpData = 0;
		int offset = 0;
		int index = 0;
		for(int i = 0;i < code.size();++ i)
		{
			cur = getCodeTableIndex(code[i]);
			if(offset == 0) tmpData += cur;
			else tmpData = tmpData + (cur << offset);
			offset += 6;
			if(offset >= 8 || i == int(code.size()) - 1)
			{
				data[index] = tmpData & 0xff;
				tmpData = tmpData >> 8;
				offset -= 8;
				++ index;
				if(index >= len) return;	//超出范围
			}
		}
	}
//	inline double determinant(double v1, double v2, double v3, double v4)  // 行列式  
//	{  
//		return (v1*v4-v2*v3);  
//	}
	bool isLineIntersect(const XVec2& s0,const XVec2& e0,const XVec2& s1,const XVec2& e1)
	{
		//double delta = determinant(e0.x - s0.x, s1.x - e1.x, e0.y - s0.y,s1.y - e1.y);  
		double delta = XMat2(e0.x - s0.x, s1.x - e1.x, e0.y - s0.y,s1.y - e1.y).getValue();  
		if (delta <= (1e-6) && delta >= -(1e-6))  // delta=0，表示两线段重合或平行  
		{  
			return false;  
		}  
		delta = 1.0 / delta;
		//double namenda = determinant(s1.x - s0.x, s1.x - e1.x, s1.y - s0.y, s1.y - e1.y) * delta;  
		double namenda = XMat2(s1.x - s0.x, s1.x - e1.x, s1.y - s0.y, s1.y - e1.y).getValue() * delta;  
		if (namenda > 1.0 || namenda < 0.0)  
		{  
			return false;  
		}  
		//double miu = determinant(e0.x - s0.x, s1.x - s0.x, e0.y - s0.y, s1.y - s0.y) * delta;  
		double miu = XMat2(e0.x - s0.x, s1.x - s0.x, e0.y - s0.y, s1.y - s0.y).getValue() * delta;  
		if (miu > 1.0 || miu < 0.0)  
		{  
			return false;  
		}  
		return true;  
	}
	bool isPolygonIntersect(const std::vector<XVec2> &polygon0, const std::vector<XVec2> &polygon1)
	{
		if (polygon0.size() < 3 || polygon1.size() < 3) return false;	//传入的参数不合法
		//首先判断一个多边形的点是否在另一个多边形内部
		for (int i = 0; i < polygon0.size(); ++i)
		{
			if (getIsInPolygon(polygon0[i], polygon1)) return true;
		}
		for (int i = 0; i < polygon1.size(); ++i)
		{
			if (getIsInPolygon(polygon1[i], polygon0)) return true;
		}
		//判断两个多边形的边是否相交
		for (int i = 0; i < polygon0.size(); ++i)
		{
			for (int j = 0; j < polygon1.size(); ++j)
			{
				if (isLineIntersect(polygon0[i], polygon0[(i + 1) % polygon0.size()],
					polygon1[j], polygon1[(j + 1) % polygon1.size()])) return true;
			}
		}
		return false;
	}
	double getPolygonArea(const std::vector<XVec2> &polygon)
	{
		double ret = 0.0;
		if (polygon.size() < 3) return ret;
		std::vector<XVec2> tmp(3);
		tmp[0] = polygon[0];
		for (int i = 1; i < polygon.size() - 1; ++i)
		{
			tmp[1] = polygon[i];
			tmp[2] = polygon[i + 1];
			ret += getTriangleArea(tmp);
		}
		return ret;
	}
	double getTriangleArea(const std::vector<XVec2> &triangle)
	{
		if (triangle.size() != 3) return 0.0;
		return fabs((triangle[0].x - triangle[2].x) * (triangle[1].y - triangle[0].y) - 
			(triangle[0].x - triangle[1].x) * (triangle[2].y - triangle[0].y)) * 0.5;
	}
	int getGCM(int x, int y)
	{//更相减损法
		if (x <= 0 || y <= 0) return 0;
		int z = y;
		while (x%y != 0)
		{
			z = x%y;
			x = y;
			y = z;
		}
		return z;
	}
	bool calMPEquation(std::vector<double> &inData, int xSum, std::vector<double> &outData)
	{
		if (inData.size() != xSum * (xSum + 1)) return false;	//输入参数不合法
		if (xSum <= 0) return false;
		outData.resize(xSum);

		if (xSum == 1)
		{//已经只有一个参数
			if (inData[0] == 0.0f) return false;
			outData[0] = inData[1] / inData[0];
			return true;
		}
		else
		{
			std::vector<double> tmpIn;
			tmpIn.resize(xSum * (xSum - 1));
			std::vector<double> tmpOut;
			tmpOut.resize(xSum - 1);
			//准备数据进行迭代
			for (int i = 0, index = 0; i < xSum - 1; ++i, index += xSum + 1)
			{
				float a = inData[index];
				float b = inData[index + xSum + 1];
				for (int j = 0; j < xSum; ++j)
				{
					tmpIn[i * xSum + j] = inData[index + 1 + j] * b - inData[index + xSum + 1 + 1 + j] * a;
				}
			}
			if (!calMPEquation(tmpIn, xSum - 1, tmpOut)) return false;
			for (int i = 1; i < xSum; ++i)
			{
				outData[i] = tmpOut[i - 1];
			}
			for (int i = 0, index = 0; i < xSum; ++i, index += xSum + 1)
			{
				if (inData[index] != 0)
				{//计算第一个未知数
					outData[0] = inData[index + xSum];
					for (int j = 0; j < xSum - 1; ++j)
					{
						outData[0] -= inData[index + j + 1] * outData[j + 1];
					}
					outData[0] /= inData[index];
					break;
				}
			}
			return true;
		}
		return true;
	}
	void resampleLines(const std::vector<XVec2> &inLines,
		std::vector<XVec2> &outLines, float len, bool isLoop)
	{
		if (inLines.size() <= 0 || len <= 0.0f) return;
		outLines.clear();
		outLines.push_back(inLines[0]);
		float curLen = 0.0f;
		float tmpLen;
		if (isLoop)
		{
			for (int i = 0; i < inLines.size(); ++i)
			{
				int index = (i + 1) % inLines.size();
				tmpLen = inLines[index].getLength(inLines[i]);
				if (curLen + tmpLen >= len)
				{//存在中间点
					float rate = (len - curLen) / tmpLen;
					outLines.push_back(inLines[i] + (inLines[index] - inLines[i]) * rate);
					curLen = tmpLen - (len - curLen);
					while (curLen >= len)
					{
						rate = 1.0f - (curLen - len) / tmpLen;
						outLines.push_back(inLines[i] + (inLines[index] - inLines[i]) * rate);
						curLen -= len;
					}
				}
				else
				{
					curLen += tmpLen;
				}
			}
		}
		else
		{
			for (int i = 0; i < inLines.size() - 1; ++i)
			{
				tmpLen = inLines[i + 1].getLength(inLines[i]);
				if (curLen + tmpLen >= len)
				{//存在中间点
					float rate = (len - curLen) / tmpLen;
					outLines.push_back(inLines[i] + (inLines[i + 1] - inLines[i]) * rate);
					curLen = tmpLen - (len - curLen);
					while (curLen >= len)
					{
						rate = 1.0f - (curLen - len) / tmpLen;
						outLines.push_back(inLines[i] + (inLines[i + 1] - inLines[i]) * rate);
						curLen -= len;
					}
				}
				else
				{
					curLen += tmpLen;
				}
			}
		}
	}
	void resampleLinesP(const std::vector<XVec2> &inLines,
		std::vector<XVec2> &outLines, int pointSum, bool isLoop)
	{
		if (inLines.size() <= 0 || pointSum <= 1) return;
		if (inLines.size() == 1)
		{
			outLines.resize(pointSum);
			std::fill(outLines.begin(), outLines.end(), inLines[0]);
		}
		double len = 0.0f;
		for (int i = 0; i < inLines.size() - 1; ++i)
		{
			len += inLines[i].getLength(inLines[i + 1]);
		}
		if (isLoop)
		{
			len += inLines[0].getLength(inLines[inLines.size() - 1]);
			len /= pointSum;
		}
		else
		{
			len /= pointSum - 1.0;
		}
		resampleLines(inLines, outLines, len, isLoop);
	}
	double getTheFirstData(double data, int &fData)
	{
		bool isN = false;
		if (data < 0.0)
		{
			isN = true;
			data = -data;
		}
		if (data < 1.0)
		{//纯小数
			double tmp = 1.0;
			while (data < 1.0)
			{
				data *= 10.0;
				tmp *= 10.0;
			}
			fData = floor(data);
			data = fData / tmp;	//取整
		}
		else
		{//非纯小数
			double tmp = 1.0;
			while (data >= 1.0)
			{
				data /= 10.0;
				tmp *= 10.0;
			}
			fData = floor(data * 10.0);
			data = fData * tmp * 0.1;	//取整
		}
		if (isN) return -data;
		return data;
	}
	XVec2 polar2Cart(float len, float angle)//angle的单位是弧度
	{
		return getRotateRate(angle) * len;
	}
	XVec2 get2DOrderIndex(int order)
	{//目前这个算法有问题，暂时用于替代使用吧
		const XVec2 Order40[4] = { XVec2(0,-1),XVec2::oneZero,XVec2::zeroOne,XVec2(-1,0) };
		const XVec2 Order4X[8] = { XVec2(1,-1),XVec2::one,XVec2(-1,1),XVec2(-1,-1) };
		if (order == 0) return XVec2::zero;
		int index = 0;
		int sum = 0;
		while (true)
		{
			if (sum >= order) break;
			++index;
			sum += index * 8;
		}
		//下面计算具体的排序
		if (order > sum - 4)
		{
			return XVec2(index, index) * Order4X[3 - (sum - order)];
		}
		sum -= index * 8;
		if (order <= sum + 4)
		{
			if (order % 2 == 0) return XVec2(index, 0) * Order40[order - sum - 1];
			else return XVec2(0, index) * Order40[order - sum - 1];
		}
		int h = (order - 4 - sum - 1) / 8 + 1;
		sum += 4 + h * 8;
		int tmpIndex = (sum - order) >> 1;
		if (order % 2 == 1)
		{
			if (tmpIndex % 2 == 1)
				return XVec2(h, index) * Order4X[3 - tmpIndex];
			else
				return XVec2(index, h) * Order4X[3 - tmpIndex];
		}
		else
		{
			if (tmpIndex % 2 == 1)
				return XVec2(index, h) * Order4X[3 - tmpIndex];
			else
				return XVec2(h, index) * Order4X[3 - tmpIndex];
		}
		return XVec2::zero;
	}
	XVec3 get3DOrderIndex(int order) 
	{ 
		return XVec3::zero; 
	}
	//判断两条线段是否相交
	bool isLineSegmentIntersect(const XVec2& s0, const XVec2& e0, const XVec2& s1, const XVec2& e1)
	{
		XVec2 pos = XMath::crossPointLineToLine(s0, e0, s1, e1);
		//下面判断交点是否为线段内
		return ((s0.x == e0.x || XMath::isInArea(pos.x, s0.x, e0.x)) && (s0.y == e0.y || XMath::isInArea(pos.y, s0.y, e0.y)) &&
			(s1.x == e1.x || XMath::isInArea(pos.x, s1.x, e1.x)) && (s1.y == e1.y || XMath::isInArea(pos.y, s1.y, e1.y)));
	}
	//计算两条线段的交点
	bool getCrossPoint2LineSegment(const XVec2& s0, const XVec2& e0, const XVec2& s1, const XVec2& e1, XVec2& o)
	{
		o = XMath::crossPointLineToLine(s0, e0, s1, e1);
		//下面判断交点是否为线段内
		return ((s0.x == e0.x || XMath::isInArea(o.x, s0.x, e0.x)) && (s0.y == e0.y || XMath::isInArea(o.y, s0.y, e0.y)) &&
			(s1.x == e1.x || XMath::isInArea(o.x, s1.x, e1.x)) && (s1.y == e1.y || XMath::isInArea(o.y, s1.y, e1.y)));
	}
	//计算两条线段的交点
	float minDistance2LineSegment(const XVec2& s0, const XVec2& e0, const XVec2& s1, const XVec2& e1)
	{
		return 0.0f;
	}
	bool getRandomArrayEx(std::vector<int> &randArray, int num, int _min, int _max)
	{
		if (num < 0 || num > _max - _min + 1) return false;	//检查参数非法
		int allSum = _max - _min + 1;
		randArray.resize(allSum);
		int* p = &randArray[0];
		//方案1：
		//std::fill(randArray.begin(), randArray.end(), _min - 1);
		//for (int i = 0; i < allSum; ++i)
		//{
		//	int index = XRand::random(allSum);
		//	if (p[i] < _min) p[i] = i + _min;
		//	if (p[index] < _min) p[index] = index + _min;
		//	//if (index == i) continue;
		//	std::swap(p[i], p[index]);
		//}
		//方案2：
		//mode0	//两种方式效率基本上一致
		//auto it = randArray.begin();
		//for(int i = _min;i <= _max;++ i)
		//	*(it++) = i;
		//for (int i = 0; i < allSum; ++i)
		//	std::swap(randArray[i], randArray[XRand::random(allSum)]);
		//mode1
		for (int i = _min; i <= _max; ++i)
			*(p++) = i;
		p = &randArray[0];
		for (int i = allSum - 1; i > 0; --i)
			std::swap(p[i], p[XRand::random(allSum)]);

		randArray.resize(num);
		return true;
	}
	bool getRandomArray(std::vector<int> &randArray, int num, int _min, int _max)
	{
		if (num < 0 || num > _max - _min + 1) return false;	//检查参数非法
		int allSum = _max - _min + 1;
		randArray.resize(num);
		std::vector<int> tabTmp(allSum);
		//tabTmp.resize(allSum);

		int* p = &randArray[0];
		int* t = &tabTmp[0];
		std::fill(tabTmp.begin(), tabTmp.end(), _min - 1);
		//memset(t, 255, sizeof(int) * allSum);

		int nTop = _max, nRandResult;
		for (int i = 0; i < num; ++i)
		{
			nRandResult = _min + XRand::random(nTop + 1 - _min);
			p[i] = t[nRandResult - _min] < _min ? nRandResult : t[nRandResult - _min];
			if (nRandResult != nTop)
				t[nRandResult - _min] = t[nTop - _min] < _min ? nTop : t[nTop - _min];

			nTop = nTop - 1;
		}
		return true;
	}
}
void X2DOrderTable::_init(std::vector<XTableInfo> &table, int size)
{
	//table.resize((size + 2)*(size + 1) >> 1);
	for (int x = 0,index = 0; x < size; ++x)
	{
		for (int y = 0; y <= x; ++y,++index)
		{
			table.push_back(XTableInfo(x, y));
		}
	}
	//使用STD进行排序
	std::sort(table.begin(), table.end());
	//对表格进行排序，使用冒泡法（建议使用快速排序）
	//for (int i = 0; i < table.size(); ++i)
	//{
	//	for (int j = 0; j < table.size() - 1 - i; ++j)
	//	{
	//		if (table[j].order > table[j + 1].order)
	//		{
	//			std::swap(table[j], table[j + 1]);
	//		}
	//	}
	//}
	//计算序列
	for (int i = 0; i < table.size(); ++i)
	{
		if (table[i].x == table[i].y && table[i].x == 0)
		{
			table[i].sum = 1;
			table[i].order = 0;
			continue;
		}
		if (table[i].x == table[i].y || table[i].y == 0)
		{
			table[i].sum = 4;
			table[i].order = table[i].sum + table[i - 1].order;
			continue;
		}
		table[i].sum = 8;
		table[i].order = table[i].sum + table[i - 1].order;
	}
}
int X2DOrderTable::getTableSize()const
{
	if (!m_isInited) return 0;
	if (m_isFullTable) return m_fullTable.size();
	else
	{
		if (m_table.size() == 0) return 0;
		return m_table[m_table.size() - 1].order + 1;
	}
}
void X2DOrderTable::initFullTable(int size)
{
	if (m_isInited || size <= 0) return;

	std::vector<XTableInfo> table;
	_init(table, size);
	//下面计算全数据
	for (int i = 0; i < table.size(); ++i)
	{
		XFullTableInfo tmp;
		switch (table[i].sum)
		{
		case 1:
			tmp.x = 0;
			tmp.y = 0;			
			m_fullTable.push_back(tmp);
			break;
		case 4:
			for (int j = 0;j < table[i].sum;++j)
			{
				if (j % 2 == 1)
				{
					tmp.x = table[i].x * m_order4[j].x;
					tmp.y = table[i].y * m_order4[j].y;
					m_fullTable.push_back(tmp);
				}
				else
				{
					tmp.x = table[i].y * m_order4[j].x;
					tmp.y = table[i].x * m_order4[j].y;
					m_fullTable.push_back(tmp);
				}
			}
			break;
		case 8:
			for (int j = 0;j < table[i].sum;++j)
			{
				int tmpI = j >> 1;
				if (((j & 0x1) + (j & 0x2)) % 2 == 1)
				{
					tmp.x = table[i].x * m_order4[tmpI].x;
					tmp.y = table[i].y * m_order4[tmpI].y;
				}
				else
				{
					tmp.x = table[i].y * m_order4[tmpI].x;
					tmp.y = table[i].x * m_order4[tmpI].y;
				}
				m_fullTable.push_back(tmp);
			}
			break;
		}
	}
	m_isFullTable = true;
	m_isInited = true;
}
const XVec2 X2DOrderTable::m_order4[4] = { XVec2(1,-1),XVec2(1,1),XVec2(-1,1),XVec2(-1,-1) };
XVec2 X2DOrderTable::get2DOrderIndex(int index)
{
	if (!m_isInited) return XVec2::zero;
	if (m_isFullTable)
	{
		if (index < 0 || index >= m_fullTable.size()) return XVec2::zero;
		return XVec2(m_fullTable[index].x, m_fullTable[index].y);
	}
	else
	{
		if (index == 0 || m_table.size() == 0 || index > m_table[m_table.size() - 1].order) return XVec2::zero;
		int s = 1;
		int e = m_table.size() - 1;
		int curIndex = (s + e) >> 1;
		while (true)
		{//使用二分法来计算
			if (m_table[curIndex].order >= index && m_table[curIndex - 1].order < index) break;	//找到
			if (m_table[curIndex].order < index) //上半部分
				s = curIndex + 1;
			else //下半部分
				e = curIndex - 1;
			curIndex = (s + e) >> 1;
		}
		//for (int i = 0; i < m_table.size(); ++i)
		//{
		//	if (m_table[i].order >= index)
		//	{
		//		curIndex = i;
		//		break;
		//	}
		//}
		if (curIndex < 0) return XVec2::zero;
		XTableInfo &ref = m_table[curIndex];
		if (ref.sum == 4)
		{
			int tmp = (index - 1 - m_table[curIndex - 1].order);
			if (tmp % 2 == 1)
			{
				return XVec2(ref.x, ref.y) * m_order4[tmp];
			}
			else
			{
				return XVec2(ref.y, ref.x) * m_order4[tmp];
			}
		}
		else
		{
			int tmp = (index - 1 - m_table[curIndex - 1].order);
			if (tmp % 2 == 1)
			{
				tmp = tmp >> 1;
				if (tmp % 2 == 1)
					return XVec2(ref.y, ref.x) * m_order4[tmp];
				else
					return XVec2(ref.x, ref.y) * m_order4[tmp];
			}
			else
			{
				tmp = tmp >> 1;
				if (tmp % 2 == 1)
					return XVec2(ref.x, ref.y) * m_order4[tmp];
				else
					return XVec2(ref.y, ref.x) * m_order4[tmp];
			}
		}
	}
	return XVec2::zero;
}
}