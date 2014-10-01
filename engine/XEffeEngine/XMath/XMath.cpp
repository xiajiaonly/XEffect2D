//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMath.h"
#include "XVector2.h"
#include "XLine.h"
#include "XRect.h"

float getAngleXY(float x,float y)
{
	float angle = 0.0f;
    if(x == 0.0f)
    {
        if(y == 0.0f) angle = 0.0f; else
        if(y > 0.0f) angle = PI_HALF; else
        angle = PI + PI_HALF;
    }else
    if(y == 0.0f)
    {
        if(x > 0.0f) angle = 0.0f;
        else angle = PI;
    }else
    {
        angle = atan(y / x);        //atan() return a value (-PI_HALF , PI_HALF)
        
        if(x < 0.0f) angle += PI;
        if(angle < 0.0f) angle += PI2;
    }
    return angle;
}
//对任意序列的无旋转矩形进行顺时针排序
void vector4Sort(_XVector2* v,_XVector2* u)
{
	_XBool isChange;
	for(int j = 0;j < 3;++ j)
	{
		isChange = XFalse;
		for(int i = 0;i < 3;++ i)
		{
			if(v[i].x > v[i + 1].x)
			{
				xChangeTwoSum<_XVector2>(v[i],v[i + 1]);
				xChangeTwoSum<_XVector2>(u[i],u[i + 1]);
				isChange = XTrue;
			}
		}
		if(!isChange) break;
	}
	if(v[2].y < v[3].y)
	{
		xChangeTwoSum<_XVector2>(v[2],v[3]);
		xChangeTwoSum<_XVector2>(u[2],u[3]);
	}
}
//4个点必须要是已经排序了的
int getEdgePoint(float angle,_XVector2 * points,const _XVector2* v,const _XVector2* u)
{
	int sum = 0;
	_XVector2 centerV((v[0].x + v[2].x) * 0.5f,(v[0].y + v[2].y) * 0.5f);	//提取必要的参数
	_XVector2 centerU((u[0].x + u[2].x) * 0.5f,(u[0].y + u[2].y) * 0.5f);	//提取必要的参数
	float wV = -(v[0].x - v[2].x);
	float hV = -(v[0].y - v[2].y);
	float tempAngle = _XVector2(fabs(wV),fabs(hV)).getAngle(); 
	//默认为顺时针垂直边模式及第一二个点是左边
	points[0] = centerV;
	points[1] = centerU;

	points[2].set(centerV.x,v[0].y); 
	points[3].set((u[0].x + u[3].x) * 0.5f,(u[0].y + u[3].y) * 0.5f); 
	if(angle <= PI_HALF - tempAngle)
	{
		points[4].set(centerV.x - hV * 0.5f * tan(angle),v[0].y);
		points[5].set(maping1D(points[4].x,v[3].x,v[0].x,u[3].x,u[0].x),
			maping1D(points[4].x,v[3].x,v[0].x,u[3].y,u[0].y));//@	//不好映射列
		sum = 3;
	}else
	if(angle <= PI_HALF + tempAngle)
	{
		points[4] = v[0];
		points[5] = u[0];

		points[6].set(v[0].x,centerV.y - wV * 0.5f * tan(PI_HALF - angle));
		points[7].set(maping1D(points[6].y,v[1].y,v[0].y,u[1].x,u[0].x),
			maping1D(points[6].y,v[1].y,v[0].y,u[1].y,u[0].y));	
		sum = 4;
	}else
	if(angle <= PI * 1.5f - tempAngle)
	{
		points[4] = v[0];
		points[5] = u[0];
		points[6] = v[1];
		points[7] = u[1];

		points[8].set(centerV.x - hV * 0.5f * tan(PI - angle),v[1].y);
		points[9].set(maping1D(points[8].x,v[2].x,v[1].x,u[2].x,u[1].x),
			maping1D(points[8].x,v[2].x,v[1].x,u[2].y,u[1].y));	
		sum = 5;
	}else
	if(angle <= PI * 1.5f + tempAngle)
	{
		points[4] = v[0];
		points[5] = u[0];
		points[6] = v[1];
		points[7] = u[1];
		points[8] = v[2];
		points[9] = u[2];
		points[10].set(v[2].x,centerV.y + wV * 0.5f * tan(PI * 1.5f - angle));
		points[11].set(maping1D(points[10].y,v[3].y,v[2].y,u[3].x,u[2].x),
			maping1D(points[10].y,v[3].y,v[2].y,u[3].y,u[2].y));	
		sum = 6;
	}else
	{
		points[4] = v[0];
		points[5] = u[0];
		points[6] = v[1];
		points[7] = u[1];
		points[8] = v[2];
		points[9] = u[2];
		points[10] = v[3];
		points[11] = u[3];
		points[12].set(centerV.x + hV * 0.5f * tan(PI2 - angle),v[3].y);
		points[13].set(maping1D(points[12].x,v[0].x,v[3].x,u[0].x,u[3].x),
			maping1D(points[12].x,v[0].x,v[3].x,u[0].y,u[3].y));	
		sum = 7;
	}
	//points[sum] = center;
	return sum;
}
int getEdgePointEx(_XRect &rect,_XVector2 &point,float angle,float * points)	//角度为12点钟方向开始逆时针[0 - PI]
{
	//如果点不在矩形内部则直接返回
	if(!rect.isInRect(point)) return 0;
	int ret = 0;
	//三个点
	float stateAngle = PI_HALF - _XVector2(point.x - rect.left,point.y - rect.top).getAngle();
	if(angle < stateAngle)
	{//符合三个点的情况
		ret = 3;
		points[0] = point.x;									points[1] = point.y;
		points[2] = point.x;									points[3] = rect.top;
		points[4] = point.x - (point.y - rect.top) * tan(angle);points[5] = rect.top;
		return ret;
	}
	//四个点
	stateAngle = PI_HALF + _XVector2(point.x - rect.left,rect.bottom - point.y).getAngle();
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
	stateAngle = PI_HALF * 3.0f - _XVector2(rect.right - point.x,rect.bottom - point.y).getAngle();
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
	stateAngle = PI_HALF * 3.0f + _XVector2(rect.right - point.x,point.y - rect.top).getAngle();
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
int getEdgePointEx(_XRect &vRect,_XRect &uRect,_XVector2 &vPoint,float angle,float * vPoints,float * uPoints)	//角度为12点钟方向开始逆时针[0 - PI2]
{
	int ret = getEdgePointEx(vRect,vPoint,angle,vPoints);
	if(ret <= 0) return 0;
	for(int i = 0;i < ret;++ i)
	{
		uPoints[i << 1] = maping1D(vPoints[i << 1],vRect.left,vRect.right,uRect.left,uRect.right);
		uPoints[(i << 1) + 1] = maping1D(vPoints[(i << 1) + 1],vRect.top,vRect.bottom,uRect.top,uRect.bottom);
	}
	return ret;
}
int getEdgePointEx(_XRect &rect,_XVector2 &point,float angle,_XVector2 * points)
{
	//如果点不在矩形内部则直接返回
	if(!rect.isInRect(point)) return 0;
	int ret = 0;
	//三个点
	float stateAngle = PI_HALF - _XVector2(point.x - rect.left,point.y - rect.top).getAngle();
	if(angle < stateAngle)
	{//符合三个点的情况
		ret = 3;
		points[0] = point;
		points[1].set(point.x,rect.top);
		points[2].set(point.x - (point.y - rect.top) * tan(angle),rect.top);
		return ret;
	}
	//四个点
	stateAngle = PI_HALF + _XVector2(point.x - rect.left,rect.bottom - point.y).getAngle();
	if(angle < stateAngle)
	{//符合四个点的情况
		ret = 4;
		points[0] = point;
		points[1].set(point.x,rect.top);
		points[2].set(rect.left,rect.top);
		points[3].set(rect.left,point.y - (point.x - rect.left) * tan(PI_HALF - angle));
		return ret;
	}
	//五个点
	stateAngle = PI_HALF * 3.0f - _XVector2(rect.right - point.x,rect.bottom - point.y).getAngle();
	if(angle < stateAngle)
	{//符合五个点的情况
		ret = 5;
		points[0] = point;
		points[1].set(point.x,rect.top);
		points[2].set(rect.left,rect.top);
		points[3].set(rect.left,rect.bottom);
		points[4].set(point.x - (rect.bottom - point.y) * tan(PI - angle),rect.bottom);
		return ret;
	}
	//六个点
	stateAngle = PI_HALF * 3.0f + _XVector2(rect.right - point.x,point.y - rect.top).getAngle();
	if(angle < stateAngle)
	{//符合六个点的情况
		ret = 6;
		points[0] = point;
		points[1].set(point.x,rect.top);
		points[2].set(rect.left,rect.top);
		points[3].set(rect.left,rect.bottom);
		points[4].set(rect.right,rect.bottom);
		points[5].set(rect.right,point.y + (rect.right - point.x) * tan(PI_HALF * 3.0f - angle));
		return ret;
	}
	//七个点
	if(angle < PI2)
	{
		ret = 7;
		points[0] = point;
		points[1].set(point.x,rect.top);
		points[2].set(rect.left,rect.top);
		points[3].set(rect.left,rect.bottom);
		points[4].set(rect.right,rect.bottom);
		points[5].set(rect.right,rect.top);
		points[6].set(point.x + (point.y - rect.top) * tan(PI2 - angle),rect.top);
		return ret;
	}
	return 0;
}
int getEdgePointEx(_XRect &vRect,_XRect &uRect,_XVector2 &vPoint,float angle,_XVector2 * vPoints,_XVector2 * uPoints)
{
	int ret = getEdgePointEx(vRect,vPoint,angle,vPoints);
	if(ret > 0)
	{
		for(int i = 0;i < ret;++ i)
		{
			uPoints[i].x = maping1D(vPoints[i].x,vRect.left,vRect.right,uRect.left,uRect.right);
			uPoints[i].y = maping1D(vPoints[i].y,vRect.top,vRect.bottom,uRect.top,uRect.bottom);
		}
		return ret;
	}
	return 0;
}

//折射与反射的计算
//折射：dx = h * sin(b * Ta/Tb);
//反射：dx = (h1 + h2) * sin(b);
//h为源到媒介点的距离或者媒介点到目标点的距离
//b为入射角
//Ta与Tb是两种媒介的光传导系数
//计算两个移动物体发生碰撞的时间
double getTwoLineTime(const _XVector2 &L1S,const _XVector2 &L1E,const _XVector2 &L2S,const _XVector2 &L2E,float distance,float timer)
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
double getLineToPointTime(const _XVector2 &L1S,const _XVector2 &L1E,const _XVector2 &point,float distance,float timer)
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
double getObjectToLineTime(const _XVector2 &L1S,const _XVector2 &L1E,const _XVector2 &L2S,const _XVector2 &L2E,float distance,float timer)	//获取两条线段某个距离的最小解
{
	if(timer <= 0) return -2.0;	//数值非法
	if(distance < 0) return -2.0;	//数值非法
	//算法1
	{
		double dx = (L1E.x - L1S.x) / timer;
		double dy = (L1E.y - L1S.y) / timer;
		double Ptx = 0.0;
		double Pty = 0.0;
		static double A,B,C;	//Ax + By + c = 0;
		twoPointLine(L2S,L2E,&A,&B,&C);
		double a = A * dx + B * dy;
		double b = A * L1S.x + B * L1S.y + C + distance * sqrt(A * A + B * B);
		if(a == 0) return -1.0;
		double t1 = -b/a;
		//b = 2.0 * (A * L1S.x + B * L1S.y + C) - b;
		b = A * L1S.x + B * L1S.y + C - distance * sqrt(A * A + B * B);
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
double getCrashTimerB2B(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,
						const _XVector2& position2,const _XVector2& speed2,const _XVector2& a2,float distance,float time)
{
	double e = (position1.x - position2.x) * (position1.x - position2.x) + (position1.y - position2.y) * (position1.y - position2.y) - distance * distance;
	if(e < 0) return -1.0f;	//防止结余误差
	double a = (0.25 * (a1.x - a2.x) * (a1.x - a2.x) + 0.25 * (a1.y - a2.y) * (a1.y - a2.y)) * 0.001 * 0.001 * 0.001 * 0.001;
	double b = ((a1.x - a2.x) * (speed1.x - speed2.x) + (a1.y - a2.y) * (speed1.y - speed2.y)) * 0.001 * 0.001 * 0.001;
	double c = ((speed1.x - speed2.x) * (speed1.x - speed2.x) + (position1.x - position2.x) * (a1.x - a2.x) + (speed1.y - speed2.y) * (speed1.y - speed2.y) + (position1.y - position2.y) * (a1.y - a2.y)) * 0.001 * 0.001;
	double d = (2.0 * (speed1.x - speed2.x) * (position1.x - position2.x) + 2.0 * (speed1.y - speed2.y) * (position1.y - position2.y)) * 0.001;
	if(a == 0.0) //则测试b必然也等于0
	{//求解一元二次方程
		if(c == 0) return -1.0;//平行线
		double temp = d * d - 4 * c * e;
		if(temp < 0)
		{//无解
			return -1.0;	//不会发生碰撞
		}else
		{//由于浮点数结余误差这里需要检验这个值，尚未进行
			double t2 = (-d - sqrt(temp)) / (2.0 * c);
			if(t2 > 0 && t2 <= time) return t2;
			else return -1.0;
		}
//		return -1.0;
	}else
	{//求解一元四次方程
		double r[4];
		int sum = quarticEquation(a,b,c,d,e,r);
		if(sum == 0) return -1.0;	//无解
		double temp = r[0];
		if(sum == 2)
		{
			if(r[0] < r[1]) temp = r[0]; 
			else temp = r[1];
		}else
		{
			for(int i = 1;i < sum;++ i)
			{
				if((r[i] < temp || temp < 0) && r[i] >= 0) temp = r[i] ;
			}
		}
		if(temp >= 0 && temp <= time) return temp;
		return -1.0;
	}
//	return -1.0;
}
double getCrashTimerB2P(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,
						const _XVector2& position2,float distance,float time)
{//实际上是getCrashTimerB2B这个函数的一种特殊参数的形式
	//化简
	double e = (position1.x - position2.x) * (position1.x - position2.x) + (position1.y - position2.y) * (position1.y - position2.y) - distance * distance;
	if(e < 0) return -1.0f;	//防止结余误差
	double a = (0.25 * a1.x * a1.x + 0.25 * a1.y * a1.y) * 0.001 * 0.001 * 0.001 * 0.001;
	double b = (a1.x * speed1.x  + a1.y * speed1.y) * 0.001 * 0.001 * 0.001;
	double c = (speed1.x * speed1.x + (position1.x - position2.x) * a1.x + speed1.y * speed1.y + (position1.y - position2.y) * a1.y) * 0.001 * 0.001;
	double d = (2.0 * speed1.x * (position1.x - position2.x) + 2.0 * speed1.y * (position1.y - position2.y)) * 0.001;
	if(a == 0.0) //则测试b必然也等于0
	{//求解一元二次方程
		if(c == 0) return -1.0;//平行线
		double temp = d * d - 4.0 * c * e;
		if(temp < 0)
		{//无解
			return -1.0;	//不会发生碰撞
		}else
		{
			double t2 = (-d - sqrt(temp)) / (2.0 * c);
			if(t2 > 0 && t2 <= time) return t2;
			else return -1.0;
		}
//		return -1.0;
	}else
	{//求解一元四次方程
		double r[4];
		int sum = quarticEquation(a,b,c,d,e,r);
		if(sum == 0) return -1.0;	//无解
		double temp = r[0];
		if(sum == 2)
		{
			if(r[0] < r[1]) temp = r[0]; 
			else temp = r[1];
		}else
		{
			for(int i = 1;i < sum;++ i)
			{
				if((r[i] < temp || temp < 0) && r[i] >= 0) temp = r[i] ;
			}
		}
		if(temp >= 0.0 && temp <= time) return temp;
		return -1.0;
	}
//	return -1.0;
}
double getCrashTimerB2L(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,
						const _XVector2& positionS,const _XVector2& positionE,float distance,float time)
{
	static double Ptx,Pty;
	static double A,B,C;
	twoPointLine(positionS,positionE,&A,&B,&C);
	static double a,b,c;
	double r[4];
	int order = 0;
	a = (A * 0.5 * a1.x + B * 0.5 * a1.y) * 0.001 * 0.001;
	b = (A * speed1.x + B * speed1.y) * 0.001;
	if(a == 0)
	{
		if(b == 0) return -1.0;
		c = A * position1.x + B * position1.y + C + distance * sqrt(A * A + B * B);
		double t1 = -c/b;
		c = A * position1.x + B * position1.y + C - distance * sqrt(A * A + B * B);
		double t2 = -c/b;
		if(t1 < t2 && t1 >= 0 && t1 <= time)
		{
			Ptx = position1.x + speed1.x * t1 * 0.001;
			Pty = position1.y + speed1.y * t1 * 0.001;
			//下面判断垂足是否在范围内
			if(B != 0.0)
			{
				double x = (B * B * Ptx - A * B * Pty - A * C) / (A * A + B * B);
				if((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x < positionE.x)) return t1;
			}else
			{
				double y = (A * A * Pty - A * B * Ptx - B * C) / (A * A + B * B);
				if((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y < positionE.y)) return t1;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
			}
		}
		if(t2 < t1 && t2 >= 0 && t2 <= time)
		{
			Ptx = position1.x + speed1.x * t2 * 0.001;
			Pty = position1.y + speed1.y * t2 * 0.001;
			//下面判断垂足是否在范围内
			if(B != 0.0)
			{
				double x = (B * B * Ptx - A * B * Pty - A * C) / (A * A + B * B);
				if((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x < positionE.x)) return t2;
			}else
			{
				double y = (A * A * Pty - A * B * Ptx - B * C) / (A * A + B * B);
				if((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y < positionE.y)) return t2;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
			}
		}
	}else
	{
		c = A * position1.x + B * position1.y + C + distance * sqrt(A * A + B * B);
		double temp = b * b - 4.0 * a * c;
		if(temp > 0)
		{
			r[order] = (-b + sqrt(temp)) / (2.0 * a); 
			++ order;
			r[order] = (-b - sqrt(temp)) / (2.0 * a); 
			++ order;
		}
		c = A * position1.x + B * position1.y + C - distance * sqrt(A * A + B * B);
		temp = b * b - 4.0 * a * c;
		if(temp > 0)
		{
			r[order] = (-b + sqrt(temp)) / (2.0 * a); 
			++ order;
			r[order] = (-b - sqrt(temp)) / (2.0 * a); 
			++ order;
		}
		if(order == 0) return -1.0;	//无解
		if(order == 4)
		{
			if((A * position1.x + B * position1.y + C) * a < 0)
			{
				if(r[3] < r[1]) temp = r[3]; 
				else temp = r[1];
			}else
			{
				if(r[2] < r[0]) temp = r[2]; 
				else temp = r[0];
			}
		}else
		{
			if(r[0] < r[1]) temp = r[0]; 
			else temp = r[1];
		}
		//这里需要计算解是否在范围内
		if(temp >= 0 && temp <= time)
		{
			Ptx = position1.x + speed1.x * temp * 0.001 + 0.5 * a1.x * temp * temp * 0.001 * 0.001;
			Pty = position1.y + speed1.y * temp * 0.001 + 0.5 * a1.y * temp * temp * 0.001 * 0.001;
			//下面判断垂足是否在范围内
			if(B != 0.0)
			{
				double x = (B * B * Ptx - A * B * Pty - A * C) / (A * A + B * B);
				if((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x < positionE.x)) return temp;
			}else
			{
				double y = (A * A * Pty - A * B * Ptx - B * C) / (A * A + B * B);
				if((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y < positionE.y)) return temp;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
			}
		}
	}
	return -1.0;
}
double getCrashTimerB2L(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,const _XLine &pL,float distance,float time)
{
	static double Ptx,Pty;
	double A = pL.A;
	double B = pL.B;
	double C = pL.C;
	double D = pL.m_data0;
	double E = pL.m_data1;
	double r[4];
	_XVector2 positionS = pL.getStartPosition();
	_XVector2 positionE = pL.getEndPosition();
	static double a,b,c,d;
	d = A * position1.x + B * position1.y + C;
	//如果点到直线的距离已经小于合法距离，则认为不会发生碰撞
	if(abs(d) - distance * E < 0)
	{//如果点到直线的距离已经小于许可范围
		//下面判断垂足是否在范围内
		if(B != 0.0)
		{
			double x = (B * B * position1.x - A * B * position1.y - A * C) / D;
			if((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x < positionE.x)) return -1.0f;
		}else
		{
			double y = (A * A * position1.x - A * B * position1.y - B * C) / D;
			if((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y < positionE.y)) return -1.0f;
		}
	}

	a = (A * 0.5 * a1.x + B * 0.5 * a1.y) * 0.001 * 0.001;
	b = (A * speed1.x + B * speed1.y) * 0.001;
	if(a == 0)
	{
		if(b == 0) return -1.0;
		c = d + distance * E;
		double t1 = -c/b;
		c = d - distance * E;
		double t2 = -c/b;
		if(t1 < t2 && t1 >= 0 && t1 <= time)
		{//由于浮点数结余误差这里需要检验这个值，尚未进行
			Ptx = position1.x + speed1.x * t1 * 0.001;
			Pty = position1.y + speed1.y * t1 * 0.001;
			//下面判断垂足是否在范围内
			if(B != 0.0)
			{
				double x = (B * B * Ptx - A * B * Pty - A * C) / D;
				if((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x < positionE.x)) return t1;
			}else
			{
				double y = (A * A * Pty - A * B * Ptx - B * C) / D;
				if((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y < positionE.y)) return t1;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
			}
		}
		if(t2 < t1 && t2 >= 0 && t2 <= time)
		{
			Ptx = position1.x + speed1.x * t2 * 0.001;
			Pty = position1.y + speed1.y * t2 * 0.001;
			//下面判断垂足是否在范围内
			if(B != 0.0)
			{
				double x = (B * B * Ptx - A * B * Pty - A * C) / D;
				if((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x < positionE.x)) return t2;
			}else
			{
				double y = (A * A * Pty - A * B * Ptx - B * C) / D;
				if((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y < positionE.y)) return t2;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
			}
		}
	}else
	{
		c = d + distance * E;
		double temp = b * b - 4.0 * a * c;
		int order = 0;
		if(temp > 0)
		{
			r[order] = (-b + sqrt(temp)) / (2.0 * a); 
			++ order;
			r[order] = (-b - sqrt(temp)) / (2.0 * a); 
			++ order;
		}
		c = d - distance * E;
		temp = b * b - 4.0 * a * c;
		if(temp > 0)
		{
			r[order] = (-b + sqrt(temp)) / (2.0 * a); 
			++ order;
			r[order] = (-b - sqrt(temp)) / (2.0 * a); 
			++ order;
		}
		if(order == 0) return -1.0;	//无解
		if(order == 4)
		{
			if(d * a < 0)
			{
				if(r[3] < r[1]) temp = r[3]; 
				else temp = r[1];
			}else
			{
				if(r[2] < r[0]) temp = r[2]; 
				else temp = r[0];
			}
			//temp = r[0];
			//for(int i = 1;i < order;++ i)
			//{
			//	if((r[i] < temp || temp < 0) && r[i] >= 0) temp = r[i];
			//}
		}else
		{
			if(r[0] < r[1]) temp = r[0]; 
			else temp = r[1];
		}
		//这里需要计算解是否在范围内
		if(temp >= 0 && temp <= time)
		{
			Ptx = position1.x + speed1.x * temp * 0.001 + 0.5 * a1.x * temp * temp * 0.001 * 0.001;
			Pty = position1.y + speed1.y * temp * 0.001 + 0.5 * a1.y * temp * temp * 0.001 * 0.001;
			//下面判断垂足是否在范围内
			if(B != 0.0)
			{
				double x = (B * B * Ptx - A * B * Pty - A * C) / D;
				if((x >= positionE.x && x <= positionS.x) || (x >= positionS.x && x <= positionE.x)) return temp;
			}else
			{
				double y = (A * A * Pty - A * B * Ptx - B * C) / D;
				if((y >= positionE.y && y <= positionS.y) || (y >= positionS.y && y <= positionE.y)) return temp;//((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
			}
		}
	}
	return -1.0;
}
//点到直线距离的公式
double distancePointToLine(const _XVector2& point,const _XVector2& lineS,const _XVector2& lineE)//ax+by+c=0 x0,y0 |ax0+by0+c|/√(a^2+b^2)
{
	double a = 0;
	double b = 0;
	double c = 0;
	twoPointLine(lineS,lineE,&a,&b,&c);
	if(a == 0 && b == 0) return 0.0;
	return abs(a * point.x + b * point.y + c) / sqrt(a * a + b * b);
}
double distancePointToLine(const _XVector2& point,double a,double b,double c)
{
	if(a == 0.0 && b == 0.0) return 0.0;
	return abs(a * point.x + b * point.y + c) / sqrt(a * a + b * b);
}
//点到直线的垂足公式
_XVector2 curtatePointToLine(const _XVector2& point,const _XVector2& lineS,const _XVector2& lineE)	//设直线方程为ax+by+c=0,点坐标为(m,n)则垂足为((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
{
	double a = 0.0;
	double b = 0.0;
	double c = 0.0;
	twoPointLine(lineS,lineE,&a,&b,&c);
	_XVector2 result(0.0f,0.0f);
	if(a == 0.0 && b == 0.0) return result;
	result.x = (b * b * point.x - a * b * point.y - a * c) / (a * a + b * b);
	result.y = (a * a * point.y - a * b * point.x - b * c) / (a * a + b * b);
	return result;
}
_XVector2 curtatePointToLine(const _XVector2& point,double a,double b,double c)
{
	_XVector2 result(0.0f,0.0f);
	if(a == 0.0 && b == 0.0) return result;
	result.x = (b * b * point.x - a * b * point.y - a * c) / (a * a + b * b);
	result.y = (a * a * point.y - a * b * point.x - b * c) / (a * a + b * b);
	return result;
}
//两直线交点的公式
_XVector2 crossPointTineToLine(const _XVector2& line1S,const _XVector2& line1E,const _XVector2& line2S,const _XVector2& line2E)
{
	double a0 = 0.0;
	double b0 = 0.0;
	double c0 = 0.0;
	double a1 = 0.0;
	double b1 = 0.0;
	double c1 = 0.0;
	twoPointLine(line1S,line1E,&a0,&b0,&c0);
	twoPointLine(line2S,line2E,&a1,&b1,&c1);
	_XVector2 result(0.0f,0.0f);
	if(a0 * b1 - a1 * b0 == 0.0) return result;
	result.x = (b0 * c1 - b1 * c0)/(a0 * b1 - a1 * b0) ;
	result.y = (a1 * c0 - a0 * c1)/(a0 * b1 - a1 * b0);
	return result;
}
_XVector2 crossPointTineToLine(double a0,double b0,double c0,double a1,double b1,double c1)	//y=(a1*c0 - a0*c1)/(a0*b1 - a1*b0),x=(b0*c1 - b1*c0)/(a0*b1 - a1*b0) 
{
	_XVector2 result(0.0f,0.0f);
	if(a0 * b1 - a1 * b0 == 0.0) return result;
	result.x = (b0 * c1 - b1 * c0)/(a0 * b1 - a1 * b0) ;
	result.y = (a1 * c0 - a0 * c1)/(a0 * b1 - a1 * b0);
	return result;
}
//两点直线公式
_XBool twoPointLine(const _XVector2& lineS,const _XVector2& lineE,double *a,double *b,double *c)//(x-x1)/(x2-x1)=(y-y1)/(y2-y1)
{
	if(a == NULL || b == NULL || c == NULL) return XFalse;
	(* a) = lineE.y - lineS.y;
	(* b) = lineS.x - lineE.x;
	(* c) = lineS.y * (lineE.x - lineS.x) - lineS.x * (lineE.y - lineS.y);
	return XTrue;
}
//点到线段的最短距离
double minDistancePointToLine(const _XVector2& point,const _XVector2& lineS,const _XVector2& lineE)
{
	//首先判断垂足是否在线段范围内
	_XVector2 tempPoint = curtatePointToLine(point,lineS,lineE);
	if(((tempPoint.x <= lineS.x && tempPoint.x >= lineE.x)
		|| (tempPoint.x >= lineS.x && tempPoint.x <= lineE.x))
		&& ((tempPoint.y <= lineS.y && tempPoint.y >= lineE.y)
		|| (tempPoint.y >= lineS.y && tempPoint.y <= lineE.y)))
	{//垂足在线段范围内
		return distancePointToLine(point,lineS,lineE);
	}else
	{//垂足不在线段范围内,返回两个端点中最近的端点距离
		double tempD1 = point.getLength(lineS);
		double tempD2 = point.getLength(lineE);
		if(tempD1 < tempD2) return tempD1;
		else return tempD2;
	}
//	return 0.0;
}
//两线段的最短距离
//_XVector2 minDistanceTwoLine(const _XVector2& line1S,const _XVector2& line1E,const _XVector2& line2S,const _XVector2& line2E)
_XVector2 minDistanceTwoLine(const _XVector2&,const _XVector2&,const _XVector2&,const _XVector2&)
{//尚未实现
	_XVector2 result(0.0,0.0);
	return result;
}
//使用射线法(这个方法可以扩展到任意多边形)
_XBool getIsInRect(const _XVector2& p,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,const _XVector2& p4)
{
	return getIsInRect(p.x,p.y,p1,p2,p3,p4);
}
_XBool getIsInRect(float x,float y,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,const _XVector2& p4)
{
	int crashSum = 0;	//射线相交的次数 
	static float temp;
	if(p1.y != p2.y && y >= min(p1.y,p2.y) && y < max(p1.y, p2.y))	//不平行	
	{
		temp = (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x; 
		if(temp > x) crashSum ++;
	}
	if(p2.y != p3.y && y >= min(p2.y,p3.y) && y < max(p2.y, p3.y))	//不平行	
	{
		temp = (y - p2.y) * (p3.x - p2.x) / (p3.y - p2.y) + p2.x; 
		if(temp > x) crashSum ++;
	}
	if(p3.y != p4.y && y >= min(p3.y,p4.y) && y < max(p3.y, p4.y))	//不平行	
	{
		temp = (y - p3.y) * (p4.x - p3.x) / (p4.y - p3.y) + p3.x; 
		if(temp > x) crashSum ++;
	}
	if(p4.y != p1.y && y >= min(p4.y,p1.y) && y < max(p4.y, p1.y))	//不平行	
	{
		temp = (y - p4.y) * (p1.x - p4.x) / (p1.y - p4.y) + p4.x; 
		if(temp > x) crashSum ++;
	}
	if(crashSum % 2 == 0) return XFalse;
	else return XTrue;
}
_XBool getIsInRect(const _XVector2& p,const _XVector2 *pR,int pRSum)
{
	return getIsInRect(p.x,p.y,pR,pRSum);
}
_XBool getIsInRect(float x,float y,const _XVector2 *pR,int pRSum)
{
	if(pR == NULL || pRSum <= 2) return XFalse;	//输入参数合理性检查
	int crashSum = 0;	//射线相交的次数 
	static float temp;
	for(int i = 0;i < pRSum - 1;++ i)
	{
		if(pR[i].y != pR[i + 1].y && y >= min(pR[i].y,pR[i + 1].y) && y < max(pR[i].y,pR[i + 1].y))
		{
			temp = (y - pR[i].y) * (pR[i + 1].x - pR[i].x) / (pR[i + 1].y - pR[i].y) + pR[i].x; 
			if(temp > x) crashSum ++;
		}
	}
	//判断闭合点
	if(pR[pRSum - 1].y != pR[0].y && y >= min(pR[pRSum - 1].y,pR[0].y) && y < max(pR[pRSum - 1].y,pR[0].y))
	{
		temp = (y - pR[pRSum - 1].y) * (pR[0].x - pR[pRSum - 1].x) / (pR[0].y - pR[pRSum - 1].y) + pR[pRSum - 1].x; 
		if(temp > x) crashSum ++;
	}
	if(crashSum % 2 == 0) return XFalse;
	else return XTrue;
}

//一次方程:ax + b = 0
int onceEquation(double a,double b,complex<double> &z)
{
	if(a == 0)
	{//没有意义
		z = 0;	//无效
		return 0;
	}else
	{
		z = -b / a;
		return 1;
	}
}
//二次方程:ax^2 + bx + c = 0
int quadraticEquation(double a,double b,double c,complex<double> &z1,complex<double> &z2)
{
	if(a == 0)
	{
		z2 = 0;	//无效
		return onceEquation(b,c,z1);	//实际上只有一个解
	}else
	{
		double delta;
		complex<double> temp1,temp2;

		delta = b*b - 4.0*a*c; 
		
		complex<double> temp(delta,0);

		temp1 = (-b)/(2.0*a);
		temp2 = sqrt(temp)/(2.0*a);

		z1 = temp1 + temp2;
		z2 = temp1 - temp2;
		return 2;
	}
}
//三次方程:ax^3 + bx^2 + cx + d = 0;
int cubicEquation(double a,double b,double c,double d,complex<double> &z1,complex<double> &z2,complex<double> &z3)
{
	static double p,q,delta; 
	static double M,N;	

	complex<double> temp1,temp2;
	if(a == 0)
	{
		return quadraticEquation(b,c,d,z1,z2);
	}else
	{  
		p = -pow(b/a,2.0)/3.0 + c/a;                  
		q = 2.0/27.0 * pow(b/a,3.0) - b * c/(a*a*3.0) + d/a;  

		delta = 0.25 * q * q + p * p * p / 27.0;

		complex<double>  omega1(-0.5, sqrt(3.0) * 0.5);
		complex<double>  omega2(-0.5, -sqrt(3.0) * 0.5);

		complex<double>  yy(b/(3.0*a),0.0);	

		M = -q * 0.5;

		if(delta < 0)
		{
			 N = sqrt(fabs(delta));
			 complex<double> s1(M,N);  
			 complex<double> s2(M,-N);	
			 
			 z1 = (pow(s1,1.0/3.0) + pow(s2,1.0/3.0)) - yy;
			 z2 = (pow(s1,1.0/3.0)*omega1 + pow(s2,1.0/3.0)*omega2) - yy;
			 z3 = (pow(s1,1.0/3.0)*omega2 + pow(s2,1.0/3.0)*omega1) - yy;
		}else
		{
			N = sqrt(delta);

			complex<double>  f1(M+N,0); 
			complex<double>  f2(M-N,0);

			 if(M+N >= 0)
				 temp1 = pow(f1,1.0/3.0);
			 else
				 temp1 = -norm(pow(sqrt(f1),1.0/3.0));

			 if(M-N >= 0)
				 temp2 = pow(f2,1.0/3.0);
			 else		
				 temp2 = -norm(pow(sqrt(f2),1.0/3.0));

			 z1 = temp1 + temp2 - yy;		 
			 z2 = omega1*temp1 + omega2*temp2 - yy;
			 z3 = omega2*temp1 + omega1*temp2 - yy; 
		}
		return 3;
	}
}
int quarticEquation(double a,double b,double c,double d,double e,double *x)
{
	static complex<double> r1,r2,r3,r4;
	int sum = quarticEquation(a,b,c,d,e,r1,r2,r3,r4);
	if(sum == 0) return 0;	//无解
	int rSum = 0;
	if(r1.imag() == 0) 
	{
		x[rSum] = r1.real();
		rSum ++;
	}
	if(r2.imag() == 0) 
	{
		x[rSum] = r2.real();
		rSum ++;
	}
	if(r3.imag() == 0) 
	{
		x[rSum] = r3.real();
		rSum ++;
	}
	if(r4.imag() == 0) 
	{
		x[rSum] = r4.real();
		rSum ++;
	}
	return rSum;
}
//四次方程:ax^4 + bx^3 + cx^2 + dx + e = 0
int quarticEquation(  double a,double b,double c,double d,double e,
					  complex<double> &x1,complex<double> &x2,
					  complex<double> &x3,complex<double> &x4
					  )
{
	static double b1,c1,d1;
	static double y;

	//输入一元四次方程的系数，得方程为a*x^4+b*x^3+c*x^2+d*x+e=0；

	if(a == 0)
	{
		x4 = 0; 
		return cubicEquation(b,c,d,e,x1,x2,x3);
	}else
	if(b == 0 && c == 0 && d == 0 && e == 0)
	{
		x1 = 0; 
		x2 = 0; 
		x3 = 0; 
		x4 = 0;
		return 4;
	}else 
	if(b == 0 && c == 0 && d == 0)
	{//有待进一步确定
		if(-e/a < 0)
		{
			return 0;	//无解
		}else
		{
			x4 = sqrt(-e/a);
			x1 = sqrt(x4);
			x2 = -sqrt(x4);
			x3 = sqrt(-x4); 
			x4 = -sqrt(-x4);
			return 4;
		}
	}else 
	if(b == 0 && d == 0 && e == 0)
	{
		return quadraticEquation(a,0.0,c,x1,x2);
	}else
	{
		//把任意系数的四次方程化为首项系数为1的四次方程；
		b = b/a;  
		c = c/a;  
		d = d/a;  
		e = e/a; 
		a = 1.0; 
	 
		//所调用的三次方程的系数;	
		b1 = -4.0*c;  
		c1 = 2.0*b*d - 8.0*e;  
		d1 = e*(4.0*c-b*b)-d*d;

	    cubicEquation(8.0,b1,c1,d1,x1,x2,x3);
		y = real(x1);
		//把三次方程的返回值赋给 y
		
		//第一次调用的二次方程的系数
		b1 = b*0.5 - sqrt(8.0*y + b*b - 4.0*c)*0.5;  
		c1 = y - (b*y - d)/sqrt(8.0*y + b*b - 4.0*c);
		quadraticEquation(1.0,b1,c1,x1,x2);	

		//第二次调用的二次方程的系数
		b1 = b*0.5 + sqrt(8.0*y + b*b - 4.0*c)*0.5;
		c1 = y + (b*y - d)/sqrt(8.0*y + b*b - 4.0*c);
	//	b1 = b - b1;
	//	c1 = 2.0 * y - c1;
		quadraticEquation(1.0f,b1,c1,x3,x4);
		return 4;
	}
}
//_XVector2 maping2D(const _XVector2 *inRect,const _XVector2 *outRect,const _XVector2 &in)
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
//		if(b == 0.0f) return _XVector2::zero;
//		double yt = -c/b;
//		double xt = cx + Kx2 * yt;
//		if(xt == 0.0f) return _XVector2::zero;
//		xt = (bx - Kx1 * yt) / xt;
//		if(xt >= 0.0f && xt <= 1.0f)
//		{//合法的值
//			_XVector2 ret;
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
//			if(xt == 0.0f) return _XVector2::zero;
//			xt = (bx - Kx1 * yt0) / xt;
//			if(xt >= 0.0f && xt <= 1.0f)
//			{//合法的值
//				_XVector2 ret;
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
//			if(xt == 0.0f) return _XVector2::zero;
//			xt = (bx - Kx1 * yt1) / xt;
//			if(xt >= 0.0f && xt <= 1.0f)
//			{//合法的值
//				_XVector2 ret;
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
//	return _XVector2::zero;
//}
//0   1
//3   2
_XVector2 maping2D(const _XVector2 *inRect,const _XVector2 *outRect,const _XVector2 &in)
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

	double temp = b * b - 4.0f * a * c;
	if(a == 0.0f)
	{
		if(b == 0.0f) return _XVector2::zero;
		double yt = -c/b;
		double xt = cx + Kx2 * yt;
		if(xt == 0.0f) return _XVector2::zero;
		xt = (bx - Kx1 * yt) / xt;
		if(xt >= 0.0f && xt <= 1.0f)
		{
			double x1 = outRect[0].x + (outRect[1].x - outRect[0].x) * xt;
			double y1 = outRect[0].y + (outRect[1].y - outRect[0].y) * xt;
			double x2 = outRect[3].x + (outRect[2].x - outRect[3].x) * xt;
			double y2 = outRect[3].y + (outRect[2].y - outRect[3].y) * xt;
			return _XVector2(x1 + (x2 - x1) * yt,y1 + (y2 - y1) * yt);
		}
	}
	if(temp >= 0.0f)
	{
		temp = sqrt(temp);
		double yt0 = (-b - temp) / (2.0f * a);
		double yt1 = (-b + temp) / (2.0f * a);

		if(yt0 >= 0.0f && yt0 <= 1.0f)
		{
			double xt = cx + Kx2 * yt0;
			if(xt == 0.0f) return _XVector2::zero;
			xt = (bx - Kx1 * yt0) / xt;
			if(xt >= 0.0f && xt <= 1.0f)
			{
				double x1 = outRect[0].x + (outRect[1].x - outRect[0].x) * xt;
				double y1 = outRect[0].y + (outRect[1].y - outRect[0].y) * xt;
				double x2 = outRect[3].x + (outRect[2].x - outRect[3].x) * xt;
				double y2 = outRect[3].y + (outRect[2].y - outRect[3].y) * xt;
				return _XVector2(x1 + (x2 - x1) * yt0,y1 + (y2 - y1) * yt0);
			}
		}
		if(yt1 >= 0.0f && yt1 <= 1.0f)
		{
			double xt = cx + Kx2 * yt1;
			if(xt == 0.0f) return _XVector2::zero;
			xt = (bx - Kx1 * yt1) / xt;
			if(xt >= 0.0f && xt <= 1.0f)
			{
				double x1 = outRect[0].x + (outRect[1].x - outRect[0].x) * xt;
				double y1 = outRect[0].y + (outRect[1].y - outRect[0].y) * xt;
				double x2 = outRect[3].x + (outRect[2].x - outRect[3].x) * xt;
				double y2 = outRect[3].y + (outRect[2].y - outRect[3].y) * xt;
				return _XVector2(x1 + (x2 - x1) * yt1,y1 + (y2 - y1) * yt1);
			}
		}
	}
	return _XVector2::zero;
}
#include <vector>
double getValueMatrix(const double *a,int n)
{//基本通过验证
	if(n==1) return a[0]; else 
	if(n==2) return a[0]*a[3]-a[1]*a[2]; else
	if(n==3) return a[0]*a[4]*a[8]+a[1]*a[5]*a[6]+a[2]*a[3]*a[7]-a[2]*a[4]*a[6]-a[1]*a[3]*a[8]-a[0]*a[5]*a[7];    
	else  
	{   
		double ret = 0.0;  
		std::vector<double> b;
		std::vector<double> c;
		for(int i = 0;i < n;++ i)
			b.push_back(a[i * n] * (((i%2) == 0)?1.0:-1.0));    

		std::vector<double> data;
		for(int i = 0;i < n;++ i)
			for(int j = 0;j < n - 1;++ j)
				data.push_back(a[i * n + j + 1]);
		for(int i = 0;i < n;++ i)
			for(int j = 0;j < n-1;++ j)
				c.push_back(data[i * (n - 1) + j]);
		for(int i=0;i<n;i++)
		{
			for(int j = i;j < n - 1;++ j)
			{
				for(int k = 0;k < n - 1;++ k)
				{
					data[j * (n - 1) + k] = data[(j + 1) * (n - 1) + k];
				}
			}
			ret += b[i] * getValueMatrix(&data[0],n - 1);
			for(int i = 0;i < n;++ i)
				for(int j = 0;j < n-1;++ j)
					data[i * (n - 1) + j] = c[i * (n - 1) + j];
		}
		return ret;
	}
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
unsigned short CRC16_XModen(unsigned char *buf,unsigned short len)
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