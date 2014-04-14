#ifndef _JIA_XLINE_
#define _JIA_XLINE_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "XBasicFun.h"
#include "XVector2.h"
//线段的类
class _XLine
{
private:
	_XVector2 m_startPosition;	//线段的起始点
	_XVector2 m_endPosition;	//线段的终点
	_XBool m_isEnable;			//是否有效
public:
	double A,B,C;	//Ax + By + C = 0;
	double m_data0;	//A^2 + B^2
	double m_data1;	//sqrt(A^2 + B^2)
	double m_angle;
public:
	void set(const _XVector2 &startPosition,const _XVector2 &endPosition);
	void set(float startX,float startY,float endX,float endY);
	float getLineLength() const;	//获得线段的长度
	float getLineAngle() const;	//获得线段的角度
	_XVector2 getStartPosition() const;	//获得线段的起始位置
	_XVector2 getEndPosition() const;		//获得线段的终点位置
	void setStartPosition(const _XVector2& startPosition);
	void setStartPosition(float x,float y);
	void setEndPosition(const _XVector2& endPosition);
	void setEndPosition(float x,float y);
	_XBool getIsEnable() const
	{
		return m_isEnable;
	}
	void setEnable(_XBool temp)
	{
		if(temp) m_isEnable = XTrue;
		else m_isEnable = XFalse;
	}
	_XLine()
	:m_isEnable(XFalse)
	,A(0.0)
	,B(0.0)
	,C(0.0)
	,m_data0(0.0)
	,m_data1(0.0)
	,m_angle(0.0)
	,m_startPosition(0.0f,0.0f)
	,m_endPosition(1.0f,1.0f)
	{}
};
#include "XLine.inl"
#endif