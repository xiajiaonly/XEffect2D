#ifndef _JIA_XWINDLINE_
#define _JIA_XWINDLINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.11
//--------------------------------
#include "XSmoothLine.h"

class _XWindLine:public _XSmoothLine
{
private:
	std::deque<float> m_pointsLife;	//点的生命
	float m_maxLife;
	float m_wRate;
	float m_aRate;

	virtual void release()	//释放资源
	{
		if(!m_isInited) return;
		_XSmoothLine::release();
		m_pointsLife.clear();
	}
	virtual void pushAPoint(const _XVector2 &p,float life)	//这个点是已经经过插值运算了的
	{
		_XSmoothLine::pushAPoint(p);
		m_pointsLife.push_back(life);
	}
public:
	//life:点的生命，决定线的长度，w:线的宽度系数，a:线的透明系数
	bool init(int insertSum,float insertLength,float life,float w,float a)
	{
		if(!_XSmoothLine::init(insertSum,insertLength)) return false;
		m_maxLife = life;
		m_wRate = w;
		m_aRate = a;
		return true;
	}
	virtual void draw();
	virtual void addAPoint(const _XVector2 & p);	//插入一个点
	virtual void move(int stepTime);	//生命衰减
	virtual void clear() 
	{
		for(int i = 0;i < m_points.size();++ i)
		{
			XDELETE(m_points[i]);
		}
		m_points.clear();
		m_pointsLife.clear();
	}
};

#endif