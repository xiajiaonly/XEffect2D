#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBezierSpline.h"
namespace XE{
XBezierSpline::XBezierSpline()
:m_curPosition(0.0f)
,m_isEnd(XTrue)
,m_curValue(0.0f,0.0f)
,m_speed(0.0f)
{}
float XBezierSpline::getBezierSplineAngle(float temp)
{
	XVector2 tempV;
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
void XBezierSpline::move(float timeDelay)
{
	if(m_isEnd) return;
	m_curPosition += m_speed * timeDelay;
	if(m_curPosition < 0.0f) m_curPosition = 0.0f;
	if(m_curPosition > 1.0f)
	{
		m_isEnd = XTrue;
		m_curPosition = 1.0f;
	}
	m_curValue = getBezierSplineValue(m_curPosition);
	m_curAngle = getBezierSplineAngle(m_curPosition);
}
void XBezierSpline::draw()
{
	XRender::drawLine(m_pS,m_pSP,1.0f,1.0f,0.0f,0.0f);
	XRender::drawLine(m_pEP,m_pE,1.0f,1.0f,0.0f,0.0f);
	//drawLine(m_pS,m_pE);
	for(int  i = 0;i < 50;++ i)
	{
		XRender::drawLine(getBezierSplineValue(i * 0.02f),getBezierSplineValue((i + 1.0f) * 0.02f));
	}
	XRender::drawLine(m_curValue,m_curAngle * RADIAN2DEGREE,20.0f);
	//drawLine(m_curValue,m_curValue + XVector2(1.0f,1.0f));
}

XBezierSpline2::XBezierSpline2()
:m_curPosition(0.0f)
,m_isEnd(XTrue)
,m_curValue(0.0f,0.0f)
,m_speed(0.0f)
{}
float XBezierSpline2::getBezierSplineAngle(float temp)
{
	XVector2 tempV;
	if(temp <= 0.0f)
	{
		tempV = m_pS - m_pP;
		return tempV.getAngle();
	}
	if(temp >= 1.0f)
	{
		tempV = m_pE - m_pP;
		return tempV.getAngle();
	}
	//目前不是使用求导公式的方法
	if(temp < 0.99f) tempV = getBezierSplineValue(temp + 0.01f) - getBezierSplineValue(temp);
	else tempV = getBezierSplineValue(1.0f) - getBezierSplineValue(temp);
	return tempV.getAngle();
}
void XBezierSpline2::move(float timeDelay)
{
	if(m_isEnd) return;
	m_curPosition += m_speed * timeDelay;
	if(m_curPosition < 0.0f) m_curPosition = 0.0f;
	if(m_curPosition > 1.0f)
	{
		m_isEnd = XTrue;
		m_curPosition = 1.0f;
	}
	m_curValue = getBezierSplineValue(m_curPosition);
	m_curAngle = getBezierSplineAngle(m_curPosition);
}
void XBezierSpline2::draw()
{
	XRender::drawLine(m_pS,m_pP,1.0f,1.0f,0.0f,0.0f);
	XRender::drawLine(m_pP,m_pE,1.0f,1.0f,0.0f,0.0f);
	//drawLine(m_pS,m_pE);
	for(int  i = 0;i < 50;++ i)
	{
		XRender::drawLine(getBezierSplineValue(i * 0.02f),getBezierSplineValue((i + 1.0f) * 0.02f));
	}
	XRender::drawLine(m_curValue,m_curAngle * RADIAN2DEGREE,20.0f);
	//drawLine(m_curValue,m_curValue + XVector2(1.0f,1.0f));
}
}