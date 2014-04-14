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
        if(y > 0.0f) angle = (float)(PI_HALF); else
        angle = (float)(PI + PI_HALF);
    }else
    if(y == 0.0f)
    {
        if(x > 0.0f) angle = 0.0f;
        else angle = (float)(PI);
    }else
    {
        angle = atan(y / x);        //atan() return a value (-PI_HALF , PI_HALF)
        
        if(x < 0.0f) angle += (float)(PI);
        if(angle < 0.0f) angle += (float)(PI2);
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
	if(ret > 0)
	{
		for(int i = 0;i < ret;++ i)
		{
			uPoints[i << 1] = maping1D(vPoints[i << 1],vRect.left,vRect.right,uRect.left,uRect.right);
			uPoints[(i << 1) + 1] = maping1D(vPoints[(i << 1) + 1],vRect.top,vRect.bottom,uRect.top,uRect.bottom);
		}
		return ret;
	}
	return 0;
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
double getTwoLineTime(_XVector2 L1S,_XVector2 L1E,_XVector2 L2S,_XVector2 L2E,float distance,float timer)
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
double getLineToPointTime(_XVector2 L1S,_XVector2 L1E,_XVector2 point,float distance,float timer)
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
double getObjectToLineTime(_XVector2 L1S,_XVector2 L1E,_XVector2 L2S,_XVector2 L2E,float distance,float timer)	//获取两条线段某个距离的最小解
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
_XVector2 minDistanceTwoLine(const _XVector2& line1S,const _XVector2& line1E,const _XVector2& line2S,const _XVector2& line2E)
{//尚未实现
	_XVector2 result(0.0,0.0);
	return result;
}
//使用射线法(这个方法可以扩展到任意多边形)
_XBool getIsInRect(const _XVector2& p,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,const _XVector2& p4)
{
	int crashSum = 0;	//射线相交的次数 
	static float temp;
	if(p1.y != p2.y && p.y >= min(p1.y,p2.y) && p.y < max(p1.y, p2.y))	//不平行	
	{
		temp = (p.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x; 
		if(temp > p.x) crashSum ++;
	}
	if(p2.y != p3.y && p.y >= min(p2.y,p3.y) && p.y < max(p2.y, p3.y))	//不平行	
	{
		temp = (p.y - p2.y) * (p3.x - p2.x) / (p3.y - p2.y) + p2.x; 
		if(temp > p.x) crashSum ++;
	}
	if(p3.y != p4.y && p.y >= min(p3.y,p4.y) && p.y < max(p3.y, p4.y))	//不平行	
	{
		temp = (p.y - p3.y) * (p4.x - p3.x) / (p4.y - p3.y) + p3.x; 
		if(temp > p.x) crashSum ++;
	}
	if(p4.y != p1.y && p.y >= min(p4.y,p1.y) && p.y < max(p4.y, p1.y))	//不平行	
	{
		temp = (p.y - p4.y) * (p1.x - p4.x) / (p1.y - p4.y) + p4.x; 
		if(temp > p.x) crashSum ++;
	}
	if(crashSum % 2 == 0) return XFalse;
	else return XTrue;
}
_XBool getIsInRect(const _XVector2& p,const _XVector2 *pR,int pRSum)
{
	if(pR == NULL || pRSum <= 2) return XFalse;	//输入参数合理性检查
	int crashSum = 0;	//射线相交的次数 
	static float temp;
	for(int i = 0;i < pRSum - 1;++ i)
	{
		if(pR[i].y != pR[i + 1].y && p.y >= min(pR[i].y,pR[i + 1].y) && p.y < max(pR[i].y,pR[i + 1].y))
		{
			temp = (p.y - pR[i].y) * (pR[i + 1].x - pR[i].x) / (pR[i + 1].y - pR[i].y) + pR[i].x; 
			if(temp > p.x) crashSum ++;
		}
	}
	//判断闭合点
	if(pR[pRSum - 1].y != pR[0].y && p.y >= min(pR[pRSum - 1].y,pR[0].y) && p.y < max(pR[pRSum - 1].y,pR[0].y))
	{
		temp = (p.y - pR[pRSum - 1].y) * (pR[0].x - pR[pRSum - 1].x) / (pR[0].y - pR[pRSum - 1].y) + pR[pRSum - 1].x; 
		if(temp > p.x) crashSum ++;
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