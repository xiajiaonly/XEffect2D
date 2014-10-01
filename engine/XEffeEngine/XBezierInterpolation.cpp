//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBezierInterpolation.h"

_XBezierInterpolation::_XBezierInterpolation()
:m_p0(0.0f)
,m_p1(0.0f)
,m_p2(0.0f)
,m_p3(0.0f)
,m_speed(1.0f)
,m_nowPosition(0.0f)
,m_isEnd(XTrue)
,m_nowValue(0.0f)
{}
float _XBezierInterpolation::getBezierSplineValue(float temp)
{
	if(temp <= 0.0f) return m_p0;
	if(temp >= 1.0f) return m_p3;
	float Ftemp = 1.0f - temp;
	return m_p0 * Ftemp * Ftemp * Ftemp + 3.0f * m_p1 * Ftemp * Ftemp * temp + 3.0f * m_p2 * temp * temp * Ftemp + m_p3 * temp * temp * temp;
}
void _XBezierInterpolation::move(int timeDelay)
{
	if(m_isEnd) return;
	m_nowPosition += m_speed * timeDelay;
	if(m_nowPosition < 0.0f) m_nowPosition = 0;
	if(m_nowPosition > 1.0f)
	{
		m_isEnd = XTrue;
		m_nowPosition = 1.0f;
	}
	m_nowValue = getBezierSplineValue(m_nowPosition);
}