#ifndef _JIA_XTEXT_
#define _JIA_XTEXT_
//这个是文字控件的类
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.14
//--------------------------------
#include "XControlBasic.h"
namespace XE{
class XPropertyBox;
class XText :public XControlBasic
{
	friend XPropertyBox;
private:
	XBool m_isInited;
	XFontUnicode m_font;
public:
	XBool init(const XFontUnicode& font);
	XBool init() { return init(getDefaultFont()); }
	XBool init(const char * p)
	{
		if (!init(getDefaultFont())) return XFalse;
		setString(p);
		return XTrue;
	}
	void setAlignmentMode(XFontAlignmentModeX x, XFontAlignmentModeY y);
	void setString(const char * str) { m_font.setString(str); }
	void setMaxStrLen(int len) { m_font.setMaxStrLen(len); }
	const char *getCaptionString() const { return m_font.getString(); }
	XText()
		:m_isInited(XFalse)
	{
		m_ctrlType = CTRL_OBJ_TEXT;
	}
	~XText() { release(); }
	void release();

	//下面是通用接口
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);
	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(const XVec2& s);
	void setTextColor(const XFColor& color) { m_font.setColor(color); }	//设置字体的颜色
	const XFColor& getTextColor() const { return m_font.getColor(); }	//获取控件字体的颜色
	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);
protected:
	void draw();								//描绘按钮
	void drawUp() {};							//do nothing
	XBool mouseProc(const XVec2&, XMouseState) { return XFalse; }		//对于鼠标动作的响应函数
	XBool keyboardProc(int, XKeyState) { return XFalse; }			//返回是否触发按键动作
	void insertChar(const char *, int) { ; }
	XBool canGetFocus(const XVec2&) { return XFalse; }				//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&) { return XTrue; }	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点
	XBool canBeChoose() { return XFalse; }
public:
	XBool isInRect(const XVec2& p) { return m_font.isInRect(p); }		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order) { return m_font.getBox(order); }			//获取四个顶点的坐标，目前先不考虑旋转和缩放
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XText(const XText &temp);
	XText& operator = (const XText& temp);
};
#if WITH_INLINE_FILE
#include "XText.inl"
#endif
}
#endif