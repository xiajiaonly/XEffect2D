#ifndef _JIA_XCOMMENT_
#define _JIA_XCOMMENT_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.9.1
//--------------------------------
#include "../XFont/XFontUnicode.h"
#include "XCtrlColorStyle.h"
#include "../XMath/XMoveData.h"
namespace XE{
//这是一个说明的类，用于鼠标悬浮于指定地方时的说明悬浮显示
class XComment
{
private:
	static const int m_appearTime = 1000;
	static const int m_disappear = 4000;
	static const int m_maxTime = 5000;
private:
	XBool m_isInited;		//是否初始化
	XFontUnicode m_font;
	XRect m_bgRect;		//背景的显示
	XBool m_haveString;	//是否有说明
	XBool m_visible;		//是否可见
	XBool m_needShow;		//是否需要显示

	int m_timer;
	float m_alpha;
	XMoveData m_alphaMD;	//透明度的动态变化
public:
	XBool init(const XFontUnicode& font);
	XBool init();
	void update(float stepTime);
	XBool setACopy(const XComment & temp);
	void setPosition(float x,float y);
	void draw();
	void setString(const char *str);
	void setVisible(){m_visible = XTrue;}
	void disVisible();
	void setShow();
	void disShow()
	{
		if(!m_needShow) return;
		if(m_timer <= m_appearTime)
		{
			m_needShow = XFalse;	//尚未出现，则直接消失
			return;
		}
		if(m_timer >= m_disappear) return;//正在消失，则什么也不变
		//进入消失阶段
		m_timer = m_disappear - 1;
	}
	void setAlpha(float a)
	{
		m_alpha = a;
		m_font.setAlpha(a);
	}
	XBool getIsShow(){return m_needShow;}
	XComment()
		:m_isInited(XFalse)
		,m_haveString(XFalse)
		,m_visible(XTrue)
		,m_needShow(XFalse)
		,m_alpha(1.0f)
	{}
	~XComment(){}
private:
	XComment(const XComment &rhs );		//禁止使用拷贝构造函数
	XComment &operator=( XComment );	//禁止使用赋值操作符
};
#if WITH_INLINE_FILE
#include "XComment.inl"
#endif
}
#endif