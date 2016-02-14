#ifndef _JIA_XBEZIERSPLINE_
#define _JIA_XBEZIERSPLINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//可以参考的资料
////set up texture map for bezier surface
//glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texpts[0][0][0]);
//glEnable(GL_MAP2_TEXTURE_COORD_2);
//glEnable(GL_MAP2_VERTEX_3);
//glEnable(GL_AUTO_NORMAL);
#include "XVector2.h"
//关于贝塞尔曲线的几个特点
//1、二次贝塞尔曲线，当pe(起点)、ps(终点)、pc(控制点)共线时，当pc为pe与ps的中点时线上点的分布是均匀的
//2、三次贝塞尔曲线，当pe、ps、pc0、pc1共线时，当pc0为pe与ps的1/3是，pc1为2/3时线上点的分布是均匀的
//3、如果需要获取贝塞尔曲线上的均匀分布的点 需要计算长度，计算量较大
namespace XE{
//Linear Bezier:
//B(t) = (1 - t) * P0 + t * P1;
//Quadratic Bezier:
//B(t) = (1 - t)^2 * P0 + 2 * t * (1 - t) * P1 + t^2 * P2;
//Cubic Bezier:
//B(t) = (1 - t)^3 * P0 + 3 * t * (1 - t)^2 * P1 + 3 * t^2(1 - t) * P2 + t^3 * P3;
//简单的不可移动的贝塞尔曲线的类
class XSimpleBezierSpline
{
protected:
	XVector2 m_pS;		//3次贝塞尔曲线中的起点
	XVector2 m_pSP;		//起点偏移点
	XVector2 m_pEP;		//终点偏移点
	XVector2 m_pE;		//终点
public:
	virtual void init(const XVector2& pStart,const XVector2& pSP,const XVector2& pEP,const XVector2& pEnd)
	{
		m_pS = pStart;
		m_pSP = pSP;
		m_pEP = pEP;
		m_pE = pEnd;
	}
	virtual void init(float pSx,float pSy,float pSPx,float pSPy,float pEPx,float pEPy,float pEx,float pEy)
	{
		m_pS.set(pSx,pSy);
		m_pSP.set(pSPx,pSPy);
		m_pEP.set(pEPx,pEPy);
		m_pE.set(pEx,pEy);
	}
	virtual XVector2 getBezierSplineValue(float temp)	//temp属于(0,1)
	{
		if(temp <= 0) return m_pS;
		if(temp >= 1) return m_pE;
		float Ftemp = 1.0f - temp;
		//二次贝塞尔曲线(临时测试用，或者封装2次贝塞尔曲线的类)
		//return XVector2(m_pS.x * Ftemp * Ftemp + 2.0f * m_pSP.x * Ftemp * temp + m_pE.x * temp * temp,
		//	m_pS.y * Ftemp * Ftemp + 2.0f * m_pSP.y * Ftemp * temp + m_pE.y * temp * temp);
		return XVector2(m_pS.x * Ftemp * Ftemp * Ftemp + 3.0f * m_pSP.x * Ftemp * Ftemp * temp + 3.0f * m_pEP.x * temp * temp * Ftemp + m_pE.x * temp * temp * temp,
			m_pS.y * Ftemp * Ftemp * Ftemp + 3.0f * m_pSP.y * Ftemp * Ftemp * temp + 3.0f * m_pEP.y * temp * temp * Ftemp + m_pE.y * temp * temp * temp);
		//求导公式如下
		//XVector2(3.0f * m_pS.x * Ftemp * Ftemp + 3.0f * m_pSP.x * Ftemp * (2.0f * temp - Ftemp) + 3.0f * m_pEP.x * temp * (temp - 2.0f * Ftemp) + 3.0f * m_pE.x * temp * temp,
		//	3.0f * m_pS.y * Ftemp * Ftemp + 3.0f * m_pSP.y * Ftemp * (2.0f * temp - Ftemp) + 3.0f * m_pEP.y * temp * (temp - 2.0f * Ftemp) + 3.0f * m_pE.y * temp * temp);
	}
};
//当三次贝塞尔曲线的控制点为直线的1/3时，点是均匀分布的
//可以移动的三次贝塞尔曲线的类
class XBezierSpline:public XSimpleBezierSpline
{
protected:
	XBool m_isEnd;				//是否已经移动到末尾
	float m_speed;				//曲线中点的移动速度
	XVector2 m_curValue;		//当前的贝塞尔取值
	float m_curAngle;			//当前的角度
	float m_curPosition;		//曲线中点的当前位置
public:
	float getBezierSplineAngle(float temp);		//目前的方法比较原始，建议改进用数学公式推导	
	XBool getIsEnd() const {return m_isEnd;}
	float getSpeed() const {return m_speed;}
	void setSpeed(float speed) {m_speed = speed;}
	XVector2 getCurValue() const {return m_curValue;}
	float getCurAngle() const {return m_curAngle;}
	float getCurPosition() const {return m_curPosition;}

	//贝塞尔曲线可以自动移动取值
	void move(float timeDelay);	//移动曲线中的点
	XBezierSpline();
	void draw();

	virtual void init(const XVector2& pStart,const XVector2& pSP,const XVector2& pEP,const XVector2& pEnd);
	virtual void init(float pSx,float pSy,float pSPx,float pSPy,float pEPx,float pEPy,float pEx,float pEy);
	void reset();				//重置曲线移动点
	//下面是为了实现匀速直线运动而定义的接口
private:
	double m_length;	//线的长度
	double m_startSpeed;
	double m_endSpeed;
//	XVector2 m_ts,m_tp,m_te;
	void calculate()
	{
//		m_ts = (m_pSP - m_pS) * 3.0f;
//		m_tp = (m_pEP - m_pSP) * 6.0f;
//		m_te = (m_pE - m_pEP) * 3.0f;
		m_startSpeed = speed(0.0f);
		m_endSpeed = speed(1.0f);
		m_length = getLength(1.0f);
	}
public:
	XVector2 getBezierSplineValueEx(float temp){return getBezierSplineValue(getT(temp));}
	float getBezierSplineAngleEx(float temp){return getBezierSplineAngle(getT(temp));}//目前的方法比较原始，建议改进用数学公式推导	
	double speedX(double t)  
	{  
		double it = 1.0 - t;  
		//return -3.0*m_pS.x*it*it + 3.0*m_pSP.x*it*it - 6.0*m_pSP.x*it*t + 6.0*m_pEP.x*it*t - 3.0*m_pEP.x*t*t + 3.0*m_pE.x*t*t;  
		return 3.0 * ((m_pSP.x - m_pS.x) * it * it + 2.0 * (m_pEP.x - m_pSP.x) * it * t + (m_pE.x - m_pEP.x) * t * t);  
	}
	double speedY(double t)  
	{  
		double it = 1.0 - t;  
		return 3.0 * ((m_pSP.y - m_pS.y) * it * it + 2.0 * (m_pEP.y - m_pSP.y) * it * t + (m_pE.y - m_pEP.y) * t * t);  
	} 
	double speed(double t)  
	{  
		double sx = speedX(t);  
		double sy = speedY(t);  
		return sqrt(sx*sx+sy*sy);  
		//对上面的算法进行优化
//		double it = 1.0 - t;  
//		return (m_ts * (it * it) + m_tp * (it * t) + m_te * (t * t)).getLength();  
	}  
	double getLength(double t)  
	{//算法有问题  
		//在总长度范围内，使用simpson算法的分割数  
		#define TOTAL_SIMPSON_STEP  (512)  
		//分割份数  
		int stepCounts = (int)(TOTAL_SIMPSON_STEP * t);  
		if(stepCounts & 1) ++ stepCounts; else    //保证stepCounts为偶数
		if(stepCounts == 0) return 0.0;  

		double sum1 = 0.0,sum2 = 0.0;  
		double dStepH = t / stepCounts;  
		double dStep = dStepH * 2.0;  
		double data = 0.0;
		for(int i = 0; i < stepCounts;i += 2,data += dStep)  
		{  
			sum1 += speed(data + dStepH);  
			if(i != 0)
				sum2 += speed(data); 
		}  
		return (m_startSpeed + m_endSpeed + 2.0f * sum2 + 4.0f * sum1) * dStep / 6.0;  
	}
	double getT(double t)  
	{  
		double len = t * m_length; //如果按照匀速增长,此时对应的曲线长度  
		double t1 = t;  
		double t2 = t1 - (getLength(t1) - len) / speed(t1);
		double oldT = abs(t1 - t2);//由于下面的循环不是逻辑上收缩的，所以定义这个变量防止无法跳出
		while(oldT > 0.001)  
		{  
			t1 = t2;  
			t2 = t1 - (getLength(t1) - len) / speed(t1);  
			t = abs(t1 - t2);
			if(oldT <= t) break;
			oldT = t;
		} 
		return t2;  
	}  
};
inline void XBezierSpline::init(const XVector2& pStart,const XVector2& pSP,const XVector2& pEP,const XVector2& pEnd)
{
	XSimpleBezierSpline::init(pStart,pSP,pEP,pEnd);
	calculate();
}
inline void XBezierSpline::init(float pSx,float pSy,float pSPx,float pSPy,float pEPx,float pEPy,float pEx,float pEy)
{
	XSimpleBezierSpline::init(pSx,pSy,pSPx,pSPy,pEPx,pEPy,pEx,pEy);
	calculate();
}
inline void XBezierSpline::reset()				//重置曲线移动点
{
	m_curPosition = 0.0f;
	m_isEnd = XFalse;
	m_curValue = m_pS;
}
//简单的不可移动的贝塞尔曲线的类
class XSimpleBezierSpline2
{
protected:
	XVector2 m_pS;		//贝塞尔曲线中的起点
	XVector2 m_pP;	//偏移点
	XVector2 m_pE;		//终点
public:
	virtual XVector2 getBezierSplineValue(float temp)	//temp属于(0,1)
	{
		if(temp <= 0) return m_pS;
		if(temp >= 1) return m_pE;
		float Ftemp = 1.0f - temp;
		//二次贝塞尔曲线(临时测试用，或者封装2次贝塞尔曲线的类)
		return XVector2(m_pS.x * Ftemp * Ftemp + 2.0f * m_pP.x * Ftemp * temp + m_pE.x * temp * temp,
			m_pS.y * Ftemp * Ftemp + 2.0f * m_pP.y * Ftemp * temp + m_pE.y * temp * temp);
		//求导公式如下
		//XVector2(3.0f * m_pS.x * Ftemp * Ftemp + 3.0f * m_pSP.x * Ftemp * (2.0f * temp - Ftemp) + 3.0f * m_pEP.x * temp * (temp - 2.0f * Ftemp) + 3.0f * m_pE.x * temp * temp,
		//	3.0f * m_pS.y * Ftemp * Ftemp + 3.0f * m_pSP.y * Ftemp * (2.0f * temp - Ftemp) + 3.0f * m_pEP.y * temp * (temp - 2.0f * Ftemp) + 3.0f * m_pE.y * temp * temp);
	}
	virtual void init(const XVector2& pStart,const XVector2& pP,const XVector2& pEnd)
	{
		m_pS = pStart;
		m_pP = pP;
		m_pE = pEnd;
	}
	virtual void init(float pSx,float pSy,float pPx,float pPy,float pEx,float pEy)
	{
		m_pS.set(pSx,pSy);
		m_pP.set(pPx,pPy);
		m_pE.set(pEx,pEy);
	}
};
//当二次贝塞尔曲线的控制点为直线的中点时，点是均匀分布的
//可以移动的二次贝塞尔曲线的类
class XBezierSpline2:public XSimpleBezierSpline2
{
protected:
	XBool m_isEnd;				//是否已经移动到末尾
	float m_speed;				//曲线中点的移动速度
	XVector2 m_curValue;		//当前的贝塞尔取值
	float m_curAngle;			//当前的角度
	float m_curPosition;		//曲线中点的当前位置
public:
	/*float getLength(float tmp)	//tmp为[0 - 1]
	{
		const float csX = m_pP.x - m_pS.x;
        const float csY = m_pP.y - m_pS.y;
        const float nvX = m_pE.x - m_pP.x - csX;
        const float nvY = m_pE.y - m_pP.y - csY;
                        
        const float c0 = 4.0f * (csX * csX + csY * csY);
        const float c1 = 8.0f * (csX * nvX + csY * nvY);
        const float c2 = 4.0f * (nvX * nvX + nvY * nvY);
                        
        float ft = 0.0f;
        float f0 = 0.0f;
                        
		if(c2 == 0.0f)
		{
			if(c1 == 0.0f)
			{
				ft = sqrt(c0) * tmp;
				return ft;
			}else
			{
				ft = (2 / 3) * (c1 * tmp + c0) * sqrt(c1 * tmp + c0) / c1;
				f0 = (2 / 3) * c0 * sqrt(c0) / c1;
				return (ft - f0);
			}
		}else
        {
			const float sqrt_0 = sqrt(c2 * tmp * tmp + c1 * tmp + c0);
			const float sqrt_c0 = sqrt(c0);
			const float sqrt_c2 = sqrt(c2);
                                                
			ft = 0.25 * (2 * c2 * tmp + c1) * sqrt_0 / c2;
			if((0.5 * c1 + c2 * tmp) / sqrt_c2 + sqrt_0 >= 1e-10)
				ft +=0.5 * log((0.5 * c1 + c2 * tmp) / sqrt_c2 + sqrt_0) / sqrt_c2 * (c0 - 0.25 * c1 * c1 / c2);
                                
			f0 = 0.25 * (c1) * sqrt_c0 / c2;
			if((0.5 * c1) / sqrt_c2 + sqrt_c0 >= 1e-10)
				f0 += 0.5 * log((0.5 * c1) / sqrt_c2 + sqrt_c0) / sqrt_c2 * (c0 - 0.25 * c1 * c1 / c2);
                                
			return ft - f0;
        }
	}*/
private:
	double m_length;	//线的长度
	double ax,ay,bx,by;
	double A,B,C;
	//下面是一些提速的临时变量
	double D,E,sqrtA,sqrtC,powA;
public:
	void calculate()
	{
		ax = m_pS.x - 2.0*m_pP.x + m_pE.x;  
		ay = m_pS.y - 2.0*m_pP.y + m_pE.y;  
		bx = 2.0*m_pP.x - 2.0*m_pS.x;  
		by = 2.0*m_pP.y - 2.0*m_pS.y;  

		A = 4.0 * (ax*ax+ay*ay);  
		B = 4.0 * (ax*bx+ay*by);  
		C = bx*bx+by*by; 
		sqrtA = sqrt(A);
		sqrtC = sqrt(C);
		D = log(B + 2.0 * sqrtA * sqrtC); 
		powA = 1.0 / (8.0 * pow(A,1.5));
		E = (B * B - 4.0 * A * C);
		D = D * E;
		m_length = getLength(1.0f);
	}
public:
	double getLength(double t)  //计算[0 - 1]之间的长度
	{  
		double temp1 = sqrt(C + t * (B + A * t));  
		double temp2 = 2.0 * sqrtA * (2.0 * A * t * temp1 + B * (temp1 - sqrtC));  
		double temp3 = D - E * log(B + 2.0 * A * t + 2.0 * sqrtA * temp1);  
		return (temp2 + temp3) * powA;  
	} 
	//使用牛顿切线法求解
	float speed(float t){return sqrt(A * t * t + B * t + C);}
	double getT(float t)  //计算长度到百分比之间的转换
	{  
		float l = t * m_length;
		float t1 = t,t2;  
		//int timer = 0;
		while(true)  
		{  
			t2 = t1 - (getLength(t1) - l) / speed(t1);  
			if(abs(t1 - t2) < 0.001) break;  //误差限制
			t1 = t2;  
		//	++ timer;
		} 
		//printf("%d\n",timer);
		return t2;  
	} 
/*	float getTimeByDistance(float distance)
    {
		if(distance <= 0.0f) return 0.0f;
                       
		float arcLength;
		float diffArcLength;
		float curveLength = getLength(1.0f);
		float time = distance / curveLength;
		if(distance >= curveLength) return 1;
                        
        const float csX = m_pP.x - m_pS.x;
        const float csY = m_pP.y - m_pS.y;
        const float ecX = m_pE.x - m_pP.x;
        const float ecY = m_pE.y - m_pP.y;
        const float nvX = ecX - csX;
        const float nvY = ecY - csY;
        
        const float c0 = 4.0f * (csX * csX + csY * csY);
        const float c1 = 8.0f * (csX * nvX + csY * nvY);
        const float c2 = 4.0f * (nvX * nvX + nvY * nvY);
                        
        const float c025 = c0 - 0.25 * c1 * c1 / c2;
        const float f0Base = 0.25 * c1 * sqrt(c0) / c2;
        const float exp2 = 0.5 * c1 / sqrt(c2) + sqrt(c0);
        
        const float c00sqrt = sqrt(c0);
        const float c20sqrt = sqrt(c2);
        float c22sqrt;
                        
        float exp1;
        float ft;
        float ftBase;
                        
        float f0;
        int maxIterations = 100;
        
        if(c2 == 0)
        {
            if(c1 == 0)
            {
                do{
                    arcLength = c00sqrt * time;
                    diffArcLength = sqrt(abs((c2 * time * time + c1 * time + c0))) || 1e-10; 
                    time = time - (arcLength - distance) / diffArcLength;
                }while (abs(arcLength - distance) > 1e-10 && maxIterations--);
            }else
            {
                do{
                    arcLength = (2 / 3) * (c1 * time + c0) * sqrt(c1 * time + c0) / c1 - (2 / 3) * c0 * c00sqrt / c1; 
                    diffArcLength = sqrt(abs((c2 * time * time + c1 * time + c0))) || 1e-10;
                    time = time - (arcLength - distance) / diffArcLength;
                } while (abs(arcLength - distance) > 1e-10 && maxIterations--);
            }
        }else
        {
            do{
                c22sqrt = sqrt(abs(c2 * time * time + c1 * time + c0));
                exp1 = (0.5 * c1 + c2 * time) / c20sqrt + c22sqrt;
                ftBase = 0.25 * (2 * c2 * time + c1) * c22sqrt / c2;
                if(exp1 < 1e-10)  ft = ftBase;
                else  ft = ftBase + 0.5 * log((0.5 * c1 + c2 * time) / c20sqrt + c22sqrt) / c20sqrt * c025;
                                        
                if(exp2 < 1e-10) f0 = f0Base;
                else f0 = f0Base + 0.5 * log((0.5 * c1) / c20sqrt + c00sqrt) / c20sqrt * c025;
                                        
                arcLength = ft - f0;
                diffArcLength = c22sqrt || 1e-10; 
                time = time - (arcLength - distance) / diffArcLength;
            }while(abs(arcLength - distance) > 1e-10 && maxIterations--);
        }
                        
        return time;
    }*/
	float getBezierSplineAngle(float temp);		//目前的方法比较原始，建议改进用数学公式推导	
	XBool getIsEnd() const {return m_isEnd;}
	float getSpeed() const {return m_speed;}
	void setSpeed(float speed) {m_speed = speed;}
	XVector2 getCurValue() const {return m_curValue;}
	float getCurAngle() const {return m_curAngle;}
	float getCurPosition() const {return m_curPosition;}

	//贝塞尔曲线可以自动移动取值
	void move(float timeDelay);	//移动曲线中的点
	XBezierSpline2();
	void draw();

	void reset();				//重置曲线移动点
};
inline void XBezierSpline2::reset()				//重置曲线移动点
{
	m_curPosition = 0.0f;
	m_isEnd = XFalse;
	m_curValue = m_pS;
}

/*
#include <stdio.h>  
  
#include <math.h>  
  
#include <windows.h>  
  
  
  
//四个控制点  
  
POINT P0={50,50}, P1={300,600}, P2={600,600}, P3={800,200};  
  
  
  
//曲线总长度  
  
double total_length = 0.0;  
  
  
  
//曲线分割的份数  
  
const int STEP = 70;  
  
  
  
//用于保存绘制点数据的数组  
  
POINT pixels[STEP];  
  
  
  
  
  
//-------------------------------------------------------------------------------------  
//x坐标方程  
double beze_x(double t)  
{  
    double it = 1-t;  
    return it*it*it*P0.x + 3*it*it*t*P1.x + 3*it*t*t*P2.x + t*t*t*P3.x;  
}  
//-------------------------------------------------------------------------------------  
//y坐标方程  
double beze_y(double t)  
{  
    double it = 1-t;  
    return it*it*it*P0.y + 3*it*it*t*P1.y + 3*it*t*t*P2.y + t*t*t*P3.y;  
}  
//-------------------------------------------------------------------------------------  
//x坐标速度方程  
double beze_speed_x(double t)  
{  
    double it = 1-t;  
    return -3*P0.x*it*it + 3*P1.x*it*it - 6*P1.x*it*t + 6*P2.x*it*t - 3*P2.x*t*t + 3*P3.x*t*t;  
}  
//-------------------------------------------------------------------------------------  
//y坐标速度方程  
double beze_speed_y(double t)  
{  
    double it = 1-t;  
    return -3*P0.y*it*it + 3*P1.y*it*it - 6*P1.y*it*t + 6*P2.y*it*t - 3*P2.y*t*t + 3*P3.y*t*t;  
}  
//-------------------------------------------------------------------------------------  
//速度方程  
double beze_speed(double t)  
{  
    double sx = beze_speed_x(t);  
    double sy = beze_speed_y(t);  
    return sqrt(sx*sx+sy*sy);  
}  
//-------------------------------------------------------------------------------------  
//长度方程,使用Simpson积分算法  
double beze_length(double t)  
{  
    //在总长度范围内，使用simpson算法的分割数  
    #define TOTAL_SIMPSON_STEP  (10000)  
    //分割份数  
    int stepCounts = (int)(TOTAL_SIMPSON_STEP*t);  
    if(stepCounts & 1) ++ stepCounts;    //偶数  
    if(stepCounts==0) return 0.0;  
    int halfCounts = stepCounts/2;  
    double sum1=0.0, sum2=0.0;  
    double dStep = t/stepCounts;  
    for(int i=0; i<halfCounts; ++i)  
    {  
        sum1 += beze_speed((2*i+1)*dStep);  
    }  
    for(int i=1; i<halfCounts; ++i)  
    {  
        sum2 += beze_speed((2*i)*dStep);  
    }  
    return (beze_speed(0.0)+beze_speed(1.0)+2*sum2+4*sum1)*dStep/3.0;  
}  
//-------------------------------------------------------------------------------------  
//根据t推导出匀速运动自变量t'的方程(使用牛顿切线法)  
double beze_even(double t)  
{  
    double len = t*total_length; //如果按照匀速增长,此时对应的曲线长度  
    double t1=t, t2;  
    do  
    {  
        t2 = t1 - (beze_length(t1)-len)/beze_speed(t1);  
        if(abs(t1-t2)<0.0000001) break;  
        t1=t2;  
    }while(true);  
    return t2;  
}  

//-------------------------------------------------------------------------------------  
LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    switch (message)   
    {  
    case WM_TIMER:  
        {  
            static unsigned int nIndex = 0;  
            if(nIndex>=0 && nIndex<=STEP)  
            {  
                double t = (double)nIndex/STEP;  
                //求得匀速运动对应的t值  
                t = beze_even(t);  

				//根据贝塞尔曲线函数，求得取得此时的x,y坐标  
                double x = beze_x(t);  
                double y = beze_y(t);  

				//取整  
                pixels[nIndex].x = (int)(x+0.5);  
                pixels[nIndex].y = (int)(y+0.5);  

				++ nIndex;  
                InvalidateRect(hWnd, 0, 0);  
            }  
            else  
            {  
                KillTimer(hWnd, 101);  
            }  
        }  
        break;  
    case WM_PAINT:  
        {  
            PAINTSTRUCT ps;  
            HDC hdc = BeginPaint(hWnd, &ps);  
            ::MoveToEx(hdc, P0.x, P0.y, 0);  
            LineTo(hdc, P1.x, P1.y);  
            LineTo(hdc, P2.x, P2.y);  
            LineTo(hdc, P3.x, P3.y);  
            for(int i=0; i<STEP; ++i)  
            {  
                const POINT &pt = pixels[i];  
                if(pt.x==0 && pt.y==0) break;  
                ::MoveToEx(hdc, pt.x-2, pt.y, 0);  
                ::LineTo(hdc, pt.x+3, pt.y);  
                ::MoveToEx(hdc, pt.x, pt.y-2, 0);  
                ::LineTo(hdc, pt.x, pt.y+3);  
            }  
            EndPaint(hWnd, &ps);  
        }  
        break;  
    case WM_DESTROY:  
        PostQuitMessage(0);  
        break;  
    default:  
        return DefWindowProc(hWnd, message, wParam, lParam);  
    }  
    return 0;  
}  

//-------------------------------------------------------------------------------------  
int APIENTRY WinMain(HINSTANCE hInstance,  
                     HINSTANCE hPrevInstance,  
                     LPTSTR    lpCmdLine,  
                     int       nCmdShow)  
{  
    //计算总长度  
    total_length = beze_length(1.0);  
  
    //注册窗口类  
    WNDCLASSEX wcex;  
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));  

	wcex.cbSize = sizeof(WNDCLASSEX);   
    wcex.style          = CS_HREDRAW | CS_VREDRAW;  
    wcex.lpfnWndProc    = (WNDPROC)_WndProc;  
    wcex.hInstance      = hInstance;  
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);  
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);  
    wcex.lpszClassName  = "BezierClass";  
    RegisterClassEx(&wcex);  

	//创建窗口  
    HWND hWnd = CreateWindow("BezierClass", "BezierDemo", WS_OVERLAPPEDWINDOW,  
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);  
    ShowWindow(hWnd, nCmdShow);  
    UpdateWindow(hWnd);  

	//清空绘制点数据  
    ZeroMemory(&pixels, sizeof(pixels));  

	//设定定时刷新计时器  
    SetTimer(hWnd, 101, 10, 0);  

	//消息循环  
    MSG msg;  
    while(GetMessage(&msg, NULL, 0, 0))   
    {  
        TranslateMessage(&msg);  
        DispatchMessage(&msg);  
    }  
    return (int) msg.wParam;  
  
}  
*/
}
#endif