#ifndef _JIA_XARCBALL_
#define _JIA_XARCBALL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.10.1
//--------------------------------
#include "XBasicClass.h"

class _XArcBall
{
protected:
	bool m_isInited;	//是否初始化
	_XVector3 m_clickVector;
	_XVector3 m_dragVector;
	float m_sizeWRate;	//尺寸的转换率
	float m_sizeHRate;

	void mapToSphere(const _XVector2 &point,_XVector3& vec) const;	//将2D坐标映射成3D坐标
public:
	_XArcBall(float width,float height)
		:m_isInited(false)
	{
		setSize(width,height);
	}
	_XArcBall()
		:m_isInited(false)
	{}
	~_XArcBall() {};
	bool setSize(float w,float h)
	{
		if(w <= 1.0f || h <= 1.0f) return false;	//数据不合法
		m_sizeWRate = 2.0f / (w - 1.0f);
		m_sizeHRate = 2.0f / (h - 1.0f);
		m_isInited = true;
		return true;
	}
	void click(const _XVector2 &point){mapToSphere(point,m_clickVector);}	//鼠标按下记录鼠标的状态
	void drag(const _XVector2 &point,_XVector4& rotate); //根据鼠标的拖动计算旋转的四元素
};
#endif