//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XLine.h"
#include "XBasicClass.h"

void _XLine::set(const _XVector2& startPosition,const _XVector2 &endPosition)
{
	m_startPosition = startPosition;
	m_endPosition = endPosition;
	twoPointLine(m_startPosition,m_endPosition,&A,&B,&C);
	m_data0 = A * A + B * B;
	m_data1 = sqrt(m_data0);
	m_angle = m_startPosition.getAngle(m_endPosition);
}
void _XLine::set(float startX,float startY,float endX,float endY)
{
	m_startPosition.set(startX,startY);
	m_endPosition.set(endX,endY);
	twoPointLine(m_startPosition,m_endPosition,&A,&B,&C);
	m_data0 = A * A + B * B;
	m_data1 = sqrt(m_data0);
	m_angle = m_startPosition.getAngle(m_endPosition);
}
void _XLine::setStartPosition(const _XVector2& startPosition)
{
	m_startPosition = startPosition;
	twoPointLine(m_startPosition,m_endPosition,&A,&B,&C);
	m_data0 = A * A + B * B;
	m_data1 = sqrt(m_data0);
	m_angle = m_startPosition.getAngle(m_endPosition);
}
void _XLine::setStartPosition(float x,float y)
{
	m_startPosition.set(x,y);
	twoPointLine(m_startPosition,m_endPosition,&A,&B,&C);
	m_data0 = A * A + B * B;
	m_data1 = sqrt(m_data0);
	m_angle = m_startPosition.getAngle(m_endPosition);
}
void _XLine::setEndPosition(const _XVector2& endPosition)
{
	m_startPosition = endPosition;
	twoPointLine(m_startPosition,m_endPosition,&A,&B,&C);
	m_data0 = A * A + B * B;
	m_data1 = sqrt(m_data0);
	m_angle = m_startPosition.getAngle(m_endPosition);
}
void _XLine::setEndPosition(float x,float y)
{
	m_endPosition.set(x,y);
	twoPointLine(m_startPosition,m_endPosition,&A,&B,&C);
	m_data0 = A * A + B * B;
	m_data1 = sqrt(m_data0);
	m_angle = m_startPosition.getAngle(m_endPosition);
}