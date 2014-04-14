#ifndef _JIA_XBEZIERSPLINE_
#define _JIA_XBEZIERSPLINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XBasicClass.h"

//3次贝塞尔曲线
class _XBezierSpline
{
private:
	_XVector2 m_pS;		//3次贝塞尔曲线中的起点
	_XVector2 m_pSP;		//起点偏移点
	_XVector2 m_pEP;		//终点偏移点
	_XVector2 m_pE;		//终点

	_XBool m_isEnd;				//是否已经移动到末尾
	float m_speed;				//曲线中点的移动速度
	_XVector2 m_nowValue;		//当前的贝塞尔取值
	float m_nowAngle;			//当前的角度
	float m_nowPosition;		//曲线中点的当前位置
public:
	_XVector2 getBezierSplineValue(float temp);	//temp属于(0,1)
	float getBezierSplineAngle(float temp);		//目前的方法比较原始，建议改进用数学公式推导	
	_XBool getIsEnd() const {return m_isEnd;}
	float getSpeed() const {return m_speed;}
	void setSpeed(float speed) {m_speed = speed;}
	_XVector2 getNowValue() const {return m_nowValue;}
	float getNowAngle() const {return m_nowAngle;}
	float getNowPosition() const {return m_nowPosition;}

	//贝塞尔曲线可以自动移动取值
	void move(int timeDelay);	//移动曲线中的点
	_XBezierSpline();
	void draw();

	void init(const _XVector2& pStart,const _XVector2& pSP,const _XVector2& pEP,const _XVector2& pEnd);
	void init(float pSx,float pSy,float pSPx,float pSPy,float pEPx,float pEPy,float pEx,float pEy);
	void reset();				//重置曲线移动点
};
inline void _XBezierSpline::init(const _XVector2& pStart,const _XVector2& pSP,const _XVector2& pEP,const _XVector2& pEnd)
{
	m_pS = pStart;
	m_pSP = pSP;
	m_pEP = pEP;
	m_pE = pEnd;
}
inline void _XBezierSpline::init(float pSx,float pSy,float pSPx,float pSPy,float pEPx,float pEPy,float pEx,float pEy)
{
	m_pS.set(pSx,pSy);
	m_pSP.set(pSPx,pSPy);
	m_pEP.set(pEPx,pEPy);
	m_pE.set(pEx,pEy);
}
inline void _XBezierSpline::reset()				//重置曲线移动点
{
	m_nowPosition = 0.0f;
	m_isEnd = XFalse;
	m_nowValue = m_pS;
}

//2次贝塞尔曲线
class _XBezierSpline2
{
private:
	_XVector2 m_pS;		//贝塞尔曲线中的起点
	_XVector2 m_pP;	//偏移点
	_XVector2 m_pE;		//终点

	_XBool m_isEnd;				//是否已经移动到末尾
	float m_speed;				//曲线中点的移动速度
	_XVector2 m_nowValue;		//当前的贝塞尔取值
	float m_nowAngle;			//当前的角度
	float m_nowPosition;		//曲线中点的当前位置
public:
	_XVector2 getBezierSplineValue(float temp);	//temp属于(0,1)
	float getBezierSplineAngle(float temp);		//目前的方法比较原始，建议改进用数学公式推导	
	_XBool getIsEnd() const {return m_isEnd;}
	float getSpeed() const {return m_speed;}
	void setSpeed(float speed) {m_speed = speed;}
	_XVector2 getNowValue() const {return m_nowValue;}
	float getNowAngle() const {return m_nowAngle;}
	float getNowPosition() const {return m_nowPosition;}

	//贝塞尔曲线可以自动移动取值
	void move(int timeDelay);	//移动曲线中的点
	_XBezierSpline2();
	void draw();

	void init(const _XVector2& pStart,const _XVector2& pP,const _XVector2& pEnd);
	void init(float pSx,float pSy,float pPx,float pPy,float pEx,float pEy);
	void reset();				//重置曲线移动点
};
inline void _XBezierSpline2::init(const _XVector2& pStart,const _XVector2& pP,const _XVector2& pEnd)
{
	m_pS = pStart;
	m_pP = pP;
	m_pE = pEnd;
}
inline void _XBezierSpline2::init(float pSx,float pSy,float pPx,float pPy,float pEx,float pEy)
{
	m_pS.set(pSx,pSy);
	m_pP.set(pPx,pPy);
	m_pE.set(pEx,pEy);
}
inline void _XBezierSpline2::reset()				//重置曲线移动点
{
	m_nowPosition = 0.0f;
	m_isEnd = XFalse;
	m_nowValue = m_pS;
}

#endif