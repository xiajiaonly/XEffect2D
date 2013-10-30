//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBezierInterpolation.h"

_XBezierInterpolation::_XBezierInterpolation()
:m_p0(0)
,m_p1(0)
,m_p2(0)
,m_p3(0)
,m_speed(1.0f)
,m_nowPosition(0)
,m_isEnd(0)
,m_nowValue(0)
{
}

float _XBezierInterpolation::getBezierSplineValue(float temp)
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
	return m_p0 * Ftemp * Ftemp * Ftemp + 3.0f * m_p1 * Ftemp * Ftemp * temp + 3.0f * m_p2 * temp * temp * Ftemp + m_p3 * temp * temp * temp;
}

void _XBezierInterpolation::move(int timeDelay)
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