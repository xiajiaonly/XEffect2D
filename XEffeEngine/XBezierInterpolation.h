#ifndef _JIA_XBEZIERINTERPOLATION_
#define _JIA_XBEZIERINTERPOLATION_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//#include "XBasicClass.h"

class _XBezierInterpolation
{
private:
	float m_p0;		//3次贝塞尔曲线中的第一个点
	float m_p1;
	float m_p2;
	float m_p3;
public:
	float getBezierSplineValue(float temp);	//temp属于(0,1)

	//贝塞尔曲线可以自动移动取值
	float m_nowValue;			//当前的贝塞尔取值
	float m_nowPosition;		//曲线中点的当前位置
	float m_speed;				//曲线中点的移动速度
	int m_isEnd;				//是否已经移动到末尾
	void move(int timeDelay);	//移动曲线中的点
	_XBezierInterpolation();
	void init(float p0,float p1,float p2,float p3);
	void reset();				//重置曲线移动点
};

inline void _XBezierInterpolation::init(float p0,float p1,float p2,float p3)
{
	m_p0 = p0;
	m_p1 = p1;
	m_p2 = p2;
	m_p3 = p3;
}

inline void _XBezierInterpolation::reset()				//重置曲线移动点
{
	m_nowPosition = 0;
	m_isEnd = 0;
	m_nowValue = m_p0;
}

#endif