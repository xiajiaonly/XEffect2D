//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBezierSpline.h"

_XBezierSpline::_XBezierSpline()
:m_pS(0,0)
,m_pSP(0,0)
,m_pEP(0,0)
,m_pE(0,0)
,m_nowPosition(0)
,m_isEnd(XFalse)
,m_nowValue(0,0)
,m_speed(0)
{
}
_XVector2 _XBezierSpline::getBezierSplineValue(float temp)
{
	if(temp <= 0) return m_pS;
	if(temp >= 1) return m_pE;
	float Ftemp = 1.0f - temp;
	//二次贝塞尔曲线(临时测试用，或者封装2次贝塞尔曲线的类)
	//return _XVector2(m_pS.x * Ftemp * Ftemp + 2.0f * m_pSP.x * Ftemp * temp + m_pE.x * temp * temp,
	//	m_pS.y * Ftemp * Ftemp + 2.0f * m_pSP.y * Ftemp * temp + m_pE.y * temp * temp);
	return _XVector2(m_pS.x * Ftemp * Ftemp * Ftemp + 3.0f * m_pSP.x * Ftemp * Ftemp * temp + 3.0f * m_pEP.x * temp * temp * Ftemp + m_pE.x * temp * temp * temp,
		m_pS.y * Ftemp * Ftemp * Ftemp + 3.0f * m_pSP.y * Ftemp * Ftemp * temp + 3.0f * m_pEP.y * temp * temp * Ftemp + m_pE.y * temp * temp * temp);
	//求导公式如下
	//_XVector2(3.0f * m_pS.x * Ftemp * Ftemp + 3.0f * m_pSP.x * Ftemp * (2.0f * temp - Ftemp) + 3.0f * m_pEP.x * temp * (temp - 2.0f * Ftemp) + 3.0f * m_pE.x * temp * temp,
	//	3.0f * m_pS.y * Ftemp * Ftemp + 3.0f * m_pSP.y * Ftemp * (2.0f * temp - Ftemp) + 3.0f * m_pEP.y * temp * (temp - 2.0f * Ftemp) + 3.0f * m_pE.y * temp * temp);
}
float _XBezierSpline::getBezierSplineAngle(float temp)
{
	_XVector2 tempV;
	if(temp <= 0)
	{
		tempV = m_pS - m_pSP;
		return tempV.getAngle();
	}
	if(temp >= 1)
	{
		tempV = m_pE - m_pEP;
		return tempV.getAngle();
	}
	//求导公式如下
	if(temp < 0.99f) tempV = getBezierSplineValue(temp + 0.01f) - getBezierSplineValue(temp);
	else tempV = getBezierSplineValue(1.0f) - getBezierSplineValue(temp);
	return tempV.getAngle();
}
void _XBezierSpline::move(int timeDelay)
{
	if(!m_isEnd)
	{
		m_nowPosition += m_speed * timeDelay;
		if(m_nowPosition < 0) m_nowPosition = 0;
		if(m_nowPosition > 1)
		{
			m_isEnd = XTrue;
			m_nowPosition = 1.0f;
		}
		m_nowValue = getBezierSplineValue(m_nowPosition);
		m_nowAngle = getBezierSplineAngle(m_nowPosition);
	}
}
#include "XEffeEngine.h"
void _XBezierSpline::draw()
{
	drawLine(m_pS,m_pSP,1,1,0,0);
	drawLine(m_pEP,m_pE,1,1,0,0);
	//drawLine(m_pS,m_pE);
	for(int  i = 0;i < 50;++ i)
	{
		drawLine(getBezierSplineValue(i * 0.02f),getBezierSplineValue((i + 1) * 0.02f));
	}
	drawLine(m_nowValue,m_nowAngle * RADIAN_TO_ANGLE,20);
	//drawLine(m_nowValue,m_nowValue + _XVector2(1.0f,1.0f));
}

_XBezierSpline2::_XBezierSpline2()
:m_pS(0,0)
,m_pP(0,0)
,m_pE(0,0)
,m_nowPosition(0)
,m_isEnd(XFalse)
,m_nowValue(0,0)
,m_speed(0)
{
}
_XVector2 _XBezierSpline2::getBezierSplineValue(float temp)
{
	if(temp <= 0) return m_pS;
	if(temp >= 1) return m_pE;
	float Ftemp = 1.0f - temp;
	//二次贝塞尔曲线(临时测试用，或者封装2次贝塞尔曲线的类)
	return _XVector2(m_pS.x * Ftemp * Ftemp + 2.0f * m_pP.x * Ftemp * temp + m_pE.x * temp * temp,
		m_pS.y * Ftemp * Ftemp + 2.0f * m_pP.y * Ftemp * temp + m_pE.y * temp * temp);
	//求导公式如下
	//_XVector2(3.0f * m_pS.x * Ftemp * Ftemp + 3.0f * m_pSP.x * Ftemp * (2.0f * temp - Ftemp) + 3.0f * m_pEP.x * temp * (temp - 2.0f * Ftemp) + 3.0f * m_pE.x * temp * temp,
	//	3.0f * m_pS.y * Ftemp * Ftemp + 3.0f * m_pSP.y * Ftemp * (2.0f * temp - Ftemp) + 3.0f * m_pEP.y * temp * (temp - 2.0f * Ftemp) + 3.0f * m_pE.y * temp * temp);
}
float _XBezierSpline2::getBezierSplineAngle(float temp)
{
	_XVector2 tempV;
	if(temp <= 0)
	{
		tempV = m_pS - m_pP;
		return tempV.getAngle();
	}
	if(temp >= 1)
	{
		tempV = m_pE - m_pP;
		return tempV.getAngle();
	}
	//目前不是使用求导公式的方法
	if(temp < 0.99f) tempV = getBezierSplineValue(temp + 0.01f) - getBezierSplineValue(temp);
	else tempV = getBezierSplineValue(1.0f) - getBezierSplineValue(temp);
	return tempV.getAngle();
}
void _XBezierSpline2::move(int timeDelay)
{
	if(!m_isEnd)
	{
		m_nowPosition += m_speed * timeDelay;
		if(m_nowPosition < 0) m_nowPosition = 0;
		if(m_nowPosition > 1)
		{
			m_isEnd = XTrue;
			m_nowPosition = 1.0f;
		}
		m_nowValue = getBezierSplineValue(m_nowPosition);
		m_nowAngle = getBezierSplineAngle(m_nowPosition);
	}
}
#include "XEffeEngine.h"
void _XBezierSpline2::draw()
{
	drawLine(m_pS,m_pP,1,1,0,0);
	drawLine(m_pP,m_pE,1,1,0,0);
	//drawLine(m_pS,m_pE);
	for(int  i = 0;i < 50;++ i)
	{
		drawLine(getBezierSplineValue(i * 0.02f),getBezierSplineValue((i + 1) * 0.02f));
	}
	drawLine(m_nowValue,m_nowAngle * RADIAN_TO_ANGLE,20);
	//drawLine(m_nowValue,m_nowValue + _XVector2(1.0f,1.0f));
}