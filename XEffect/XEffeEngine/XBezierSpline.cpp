//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBezierSpline.h"

_XBezierSpline::_XBezierSpline()
:m_p0(0,0)
,m_p1(0,0)
,m_p2(0,0)
,m_p3(0,0)
,m_nowPosition(0)
,m_isEnd(0)
,m_nowValue(0,0)
,m_speed(0)
{
}

_XVector2 _XBezierSpline::getBezierSplineValue(float temp)
{
	if(temp <= 0) 
	{
		return m_p0;
	}
	if(temp >= 1) 
	{
		return m_p3;
	}
	float Ftemp = 1.0f - temp;
	return _XVector2(m_p0.x * Ftemp * Ftemp * Ftemp + 3.0f * m_p1.x * Ftemp * Ftemp * temp + 3.0f * m_p2.x * temp * temp * Ftemp + m_p3.x * temp * temp * temp,
		m_p0.y * Ftemp * Ftemp * Ftemp + 3.0f * m_p1.y * Ftemp * Ftemp * temp + 3.0f * m_p2.y * temp * temp * Ftemp + m_p3.y * temp * temp * temp);
}

void _XBezierSpline::move(int timeDelay)
{
	if(m_isEnd == 0)
	{
		m_nowPosition += m_speed * timeDelay;
		if(m_nowPosition < 0) m_nowPosition = 0;
		if(m_nowPosition > 1)
		{
			m_isEnd = 1;
			m_nowPosition = 1;
		}
		m_nowValue = getBezierSplineValue(m_nowPosition);
	}
}