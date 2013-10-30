#ifndef _JIA_XBEZIERSPLINE_
#define _JIA_XBEZIERSPLINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XBasicClass.h"

class _XBezierSpline
{
private:
	_XVector2 m_p0;		//3次贝塞尔曲线中的第一个点
	_XVector2 m_p1;
	_XVector2 m_p2;
	_XVector2 m_p3;
public:
	_XVector2 getBezierSplineValue(float temp);	//temp属于(0,1)

	//贝塞尔曲线可以自动移动取值
	_XVector2 m_nowValue;			//当前的贝塞尔取值
	float m_nowPosition;		//曲线中点的当前位置
	float m_speed;				//曲线中点的移动速度
	int m_isEnd;				//是否已经移动到末尾
	void move(int timeDelay);	//移动曲线中的点
	_XBezierSpline();

	void init(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3);
	void init(float p0x,float p0y,float p1x,float p1y,float p2x,float p2y,float p3x,float p3y);
	void reset();				//重置曲线移动点
};

inline void _XBezierSpline::init(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3)
{
	m_p0 = p0;
	m_p1 = p1;
	m_p2 = p2;
	m_p3 = p3;
}

inline void _XBezierSpline::init(float p0x,float p0y,float p1x,float p1y,float p2x,float p2y,float p3x,float p3y)
{
	m_p0.set(p0x,p0y);
	m_p1.set(p1x,p1y);
	m_p2.set(p2x,p2y);
	m_p3.set(p3x,p3y);
}

inline void _XBezierSpline::reset()				//重置曲线移动点
{
	m_nowPosition = 0;
	m_isEnd = 0;
	m_nowValue = m_p0;
}

#endif