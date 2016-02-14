#ifndef _JIA_XWINDLINE_
#define _JIA_XWINDLINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.11
//--------------------------------
#include "XSmoothLine.h"
namespace XE{
class XWindLine:public XSmoothLine
{
private:
	std::deque<float> m_pointsLife;	//点的生命
	float m_maxLife;
	float m_wRate;
	float m_aRate;
	XFColor m_color;

	virtual void release();	//释放资源
	virtual void pushAPoint(const XVector2 &p,float life);	//这个点是已经经过插值运算了的
public:
	//life:点的生命，决定线的长度，w:线的宽度系数，a:线的透明系数
	bool init(int insertSum,float insertLength,float life,float w,float a);
	virtual void draw();
	virtual void addAPoint(const XVector2 & p);	//插入一个点
	virtual void move(float stepTime);	//生命衰减
	virtual void clear();
	XWindLine()
		:m_color(0.6f,0.6f,1.0f,1.0f)
	{}
	void setColor(const XFColor& c) {m_color = c;}
	XFColor getColor() {return m_color;}
};
#if WITH_INLINE_FILE
#include "XWindLine.inl"
#endif
}
#endif