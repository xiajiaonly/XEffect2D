#ifndef _JIA_XRULER_
#define _JIA_XRULER_
#include "XMath\XMath.h"
#include "XFont\XFontUnicode.h"
namespace XE {
struct XRulerStyle
{
	bool m_withCross;		//是否描绘十字坐标系
	bool m_withEdgeLine;	//是否显示边线
	bool m_withEdgeRule;	//是否显示边标尺
	bool m_withNet;			//是否显示网格
	bool m_withMousePos;	//是否显示鼠标位置信息
	bool m_withFont;		//是否显示文字

	XRulerStyle()
		:m_withCross(true)
		, m_withEdgeLine(true)
		, m_withEdgeRule(true)
		, m_withNet(true)
		, m_withMousePos(true)
		, m_withFont(true)
	{}
};
class XRuler
{
private:
	bool m_isInited;

	XVec2 m_position;	//中心点的像素位置
	XVec2 m_size;		//实际大小
	XVec2 m_scale;		//实际到像素的缩放系数
	XVec2 m_pixelSize;	//整个的像素大小
	XVec2 m_posOffset;	//中心点的偏移位置,为实际值而不是像素值

	XRulerStyle m_style;
	XFontUnicode m_font;
public:
	XVec2 getPosition()const { return m_position; }
	void setPosition(const XVec2& pos) { m_position = pos; }
	XVec2 getPixelsSize()const { return m_size * m_scale; }
	void setPosOffset(const XVec2& offset) { m_posOffset = offset; }
	XVec2 getPosOffset()const { return m_posOffset; }
	void setScale(float s)
	{
		if (!m_isInited) return;
		m_scale.set(s);
		m_size = m_pixelSize / m_scale;
	}
	void setScale(const XVec2& s)
	{
		if (!m_isInited) return;
		m_scale = s;
		m_size = m_pixelSize / m_scale;
	}
	bool init();
	//size为像素范围
	bool init(const XVec2&pos, const XVec2&size);
	//size为像素范围
	bool init(const XVec2&pos, const XVec2&size, const XVec2&scale);
	void update() {}
	void draw();

	XVec2 mapScreenToLocal(const XVec2& s)	//像素坐标到空间坐标的映射
	{
		return (s - m_position) / m_scale - m_posOffset;
	}

	XRuler()
		:m_isInited(false)
	{}
};
}
#endif