#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XLine.h"
namespace XE {
void XLine::set(const XVec2& startPosition, const XVec2& endPosition)
{
	m_startPosition = startPosition;
	m_endPosition = endPosition;
	XMath::twoPointLine(m_startPosition, m_endPosition, &A, &B, &C);
	m_data0 = A * A + B * B;
	m_data1 = sqrt(m_data0);
	m_angle = m_startPosition.getAngle(m_endPosition);
}
void XLine::set(float startX, float startY, float endX, float endY)
{
	m_startPosition.set(startX, startY);
	m_endPosition.set(endX, endY);
	XMath::twoPointLine(m_startPosition, m_endPosition, &A, &B, &C);
	m_data0 = A * A + B * B;
	m_data1 = sqrt(m_data0);
	m_angle = m_startPosition.getAngle(m_endPosition);
}
void XLine::setStartPosition(const XVec2& startPosition)
{
	m_startPosition = startPosition;
	XMath::twoPointLine(m_startPosition, m_endPosition, &A, &B, &C);
	m_data0 = A * A + B * B;
	m_data1 = sqrt(m_data0);
	m_angle = m_startPosition.getAngle(m_endPosition);
}
//void XLine::setStartPosition(float x,float y)
//{
//	m_startPosition.set(x,y);
//	XMath::twoPointLine(m_startPosition,m_endPosition,&A,&B,&C);
//	m_data0 = A * A + B * B;
//	m_data1 = sqrt(m_data0);
//	m_angle = m_startPosition.getAngle(m_endPosition);
//}
void XLine::setEndPosition(const XVec2& endPosition)
{
	m_startPosition = endPosition;
	XMath::twoPointLine(m_startPosition, m_endPosition, &A, &B, &C);
	m_data0 = A * A + B * B;
	m_data1 = sqrt(m_data0);
	m_angle = m_startPosition.getAngle(m_endPosition);
}
//void XLine::setEndPosition(float x,float y)
//{
//	m_endPosition.set(x,y);
//	XMath::twoPointLine(m_startPosition,m_endPosition,&A,&B,&C);
//	m_data0 = A * A + B * B;
//	m_data1 = sqrt(m_data0);
//	m_angle = m_startPosition.getAngle(m_endPosition);
//}
#if !WITH_INLINE_FILE
#include "XLine.inl"
#endif
}