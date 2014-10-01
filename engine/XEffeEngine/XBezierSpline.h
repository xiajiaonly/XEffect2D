#ifndef _JIA_XBEZIERSPLINE_
#define _JIA_XBEZIERSPLINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XBasicClass.h"
//Linear Bezier:
//B(t) = (1 - t) * P0 + t * P1;
//Quadratic Bezier:
//B(t) = (1 - t)^2 * P0 + 2 * t * (1 - t) * P1 + t^2 * P2;
//Cubic Bezier:
//B(t) = (1 - t)^3 * P0 + 3 * t * (1 - t)^2 * P1 + 3 * t^2(1 - t) * P2 + t^3 * P3;
//3次贝塞尔曲线
class _XBezierSpline
{
private:
	_XVector2 m_pS;		//3次贝塞尔曲线中的起点
	_XVector2 m_pSP;		//起点偏移点
	_XVector2 m_pEP;		//终点偏移点
	_XVector2 m_pE;		//终点

	_XBool m_isEnd;				//是否已经移动到末尾
	float m_speed;				//曲线中点的移动速度
	_XVector2 m_nowValue;		//当前的贝塞尔取值
	float m_nowAngle;			//当前的角度
	float m_nowPosition;		//曲线中点的当前位置
public:
	_XVector2 getBezierSplineValue(float temp);	//temp属于(0,1)
	float getBezierSplineAngle(float temp);		//目前的方法比较原始，建议改进用数学公式推导	
	_XBool getIsEnd() const {return m_isEnd;}
	float getSpeed() const {return m_speed;}
	void setSpeed(float speed) {m_speed = speed;}
	_XVector2 getNowValue() const {return m_nowValue;}
	float getNowAngle() const {return m_nowAngle;}
	float getNowPosition() const {return m_nowPosition;}

	//贝塞尔曲线可以自动移动取值
	void move(int timeDelay);	//移动曲线中的点
	_XBezierSpline();
	void draw();

	void init(const _XVector2& pStart,const _XVector2& pSP,const _XVector2& pEP,const _XVector2& pEnd);
	void init(float pSx,float pSy,float pSPx,float pSPy,float pEPx,float pEPy,float pEx,float pEy);
	void reset();				//重置曲线移动点
	//下面是为了实现匀速直线运动而定义的接口
private:
	double m_length;	//线的长度
	double m_startSpeed;
	double m_endSpeed;
	void calculate()
	{
		m_length = getLength(1.0f);
		m_startSpeed = speed(0.0f);
		m_endSpeed = speed(1.0f);
	}
public:
	_XVector2 getBezierSplineValueEx(float temp){return getBezierSplineValue(getT(temp));}
	float getBezierSplineAngleEx(float temp){return getBezierSplineAngle(getT(temp));}//目前的方法比较原始，建议改进用数学公式推导	
	double speedX(double t)  
	{  
		double it = 1.0 - t;  
		return -3.0*m_pS.x*it*it + 3.0*m_pSP.x*it*it - 6.0*m_pSP.x*it*t + 6.0*m_pEP.x*it*t - 3.0*m_pEP.x*t*t + 3.0*m_pE.x*t*t;  
	}
	double speedY(double t)  
	{  
		double it = 1.0 - t;  
		return -3.0*m_pS.y*it*it + 3.0*m_pSP.y*it*it - 6.0*m_pSP.y*it*t + 6.0*m_pEP.y*it*t - 3.0*m_pEP.y*t*t + 3.0*m_pE.y*t*t;  
	} 
	double speed(double t)  
	{  
		double sx = speedX(t);  
		double sy = speedY(t);  
		return sqrt(sx*sx+sy*sy);  
	}  
	double getLength(double t)  
	{  
		//在总长度范围内，使用simpson算法的分割数  
		#define TOTAL_SIMPSON_STEP  (10000)  
		//分割份数  
		int stepCounts = (int)(TOTAL_SIMPSON_STEP*t);  
		if(stepCounts & 1) stepCounts++;    //偶数  
		if(stepCounts == 0) return 0.0;  

		int halfCounts = (stepCounts >> 1);  
		double sum1 = 0.0,sum2 = 0.0;  
		double dStep = t/stepCounts;  
		for(int i=0; i<halfCounts; i++)  
		{  
			sum1 += speed(((i << 1) + 1) * dStep);  
		}  
		for(int i=1; i<halfCounts; i++)  
		{  
			sum2 += speed((i << 1) * dStep);  
		}  
		return (m_startSpeed + m_endSpeed + 2.0f * sum2 + 4.0f * sum1) * dStep / 3.0;  
	}
	double getT(double t)  
	{  
		double len = t * m_length; //如果按照匀速增长,此时对应的曲线长度  
		double t1=t, t2;  
		while(true)  
		{  
			t2 = t1 - (getLength(t1)-len)/speed(t1);  
			if(abs(t1-t2)<0.0001) break;  
			t1=t2;  
		} 
		return t2;  
	}  
};
inline void _XBezierSpline::init(const _XVector2& pStart,const _XVector2& pSP,const _XVector2& pEP,const _XVector2& pEnd)
{
	m_pS = pStart;
	m_pSP = pSP;
	m_pEP = pEP;
	m_pE = pEnd;
	calculate();
}
inline void _XBezierSpline::init(float pSx,float pSy,float pSPx,float pSPy,float pEPx,float pEPy,float pEx,float pEy)
{
	m_pS.set(pSx,pSy);
	m_pSP.set(pSPx,pSPy);
	m_pEP.set(pEPx,pEPy);
	m_pE.set(pEx,pEy);
	calculate();
}
inline void _XBezierSpline::reset()				//重置曲线移动点
{
	m_nowPosition = 0.0f;
	m_isEnd = XFalse;
	m_nowValue = m_pS;
}

//2次贝塞尔曲线
class _XBezierSpline2
{
private:
	_XVector2 m_pS;		//贝塞尔曲线中的起点
	_XVector2 m_pP;	//偏移点
	_XVector2 m_pE;		//终点

	_XBool m_isEnd;				//是否已经移动到末尾
	float m_speed;				//曲线中点的移动速度
	_XVector2 m_nowValue;		//当前的贝塞尔取值
	float m_nowAngle;			//当前的角度
	float m_nowPosition;		//曲线中点的当前位置
public:
	_XVector2 getBezierSplineValue(float temp);	//temp属于(0,1)
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
	void calculate()
	{
		m_length = getLength(1.0f);
		ax = m_pS.x - 2.0*m_pP.x + m_pE.x;  
		ay = m_pS.y - 2.0*m_pP.y + m_pE.y;  
		bx = 2.0*m_pP.x - 2.0*m_pS.x;  
		by = 2.0*m_pP.y - 2.0*m_pS.y;  

		A = 4.0 * (ax*ax+ay*ay);  
		B = 4.0 * (ax*bx+ay*by);  
		C = bx*bx+by*by; 	
	}
public:
	double getLength(double t)  
	{  
		double temp1 = sqrt(C+t*(B+A*t));  
		double temp2 = (2.0*A*t*temp1+B*(temp1-sqrt(C)));  
		double temp3 = log(B+2.0*sqrt(A)*sqrt(C));  
		double temp4 = log(B+2.0*A*t+2.0*sqrt(A)*temp1);  
		double temp5 = 2.0*sqrt(A)*temp2;  
		double temp6 = (B*B-4.0*A*C)*(temp3-temp4);  
		return (temp5+temp6)/(8.0*pow(A,1.5));  
	} 
	//使用牛顿切线法求解
	float speed(float t)
	{
		return sqrt(A*t*t + B*t+C);  
	}
	double getT(float t)  
	{  
		float l = t * m_length;
		float t1 = t,t2;  
		while(true)  
		{  
			t2 = t1 - (getLength(t1) - l) / speed(t1);  
			if(abs(t1 - t2) < 0.0001) break;  //误差限制
			t1 = t2;  
		} 
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
	_XBool getIsEnd() const {return m_isEnd;}
	float getSpeed() const {return m_speed;}
	void setSpeed(float speed) {m_speed = speed;}
	_XVector2 getNowValue() const {return m_nowValue;}
	float getNowAngle() const {return m_nowAngle;}
	float getNowPosition() const {return m_nowPosition;}

	//贝塞尔曲线可以自动移动取值
	void move(int timeDelay);	//移动曲线中的点
	_XBezierSpline2();
	void draw();

	void init(const _XVector2& pStart,const _XVector2& pP,const _XVector2& pEnd);
	void init(float pSx,float pSy,float pPx,float pPy,float pEx,float pEy);
	void reset();				//重置曲线移动点
};
inline void _XBezierSpline2::init(const _XVector2& pStart,const _XVector2& pP,const _XVector2& pEnd)
{
	m_pS = pStart;
	m_pP = pP;
	m_pE = pEnd;
}
inline void _XBezierSpline2::init(float pSx,float pSy,float pPx,float pPy,float pEx,float pEy)
{
	m_pS.set(pSx,pSy);
	m_pP.set(pPx,pPy);
	m_pE.set(pEx,pEy);
}
inline void _XBezierSpline2::reset()				//重置曲线移动点
{
	m_nowPosition = 0.0f;
	m_isEnd = XFalse;
	m_nowValue = m_pS;
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
    if(stepCounts & 1) stepCounts++;    //偶数  
    if(stepCounts==0) return 0.0;  
    int halfCounts = stepCounts/2;  
    double sum1=0.0, sum2=0.0;  
    double dStep = t/stepCounts;  
    for(int i=0; i<halfCounts; i++)  
    {  
        sum1 += beze_speed((2*i+1)*dStep);  
    }  
    for(int i=1; i<halfCounts; i++)  
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

				nIndex++;  
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
            for(int i=0; i<STEP; i++)  
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
#endif