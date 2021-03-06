#ifndef _JIA_XPROGRESSRING_
#define _JIA_XPROGRESSRING_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.18
//--------------------------------
//这是一个环形进度条，目前尚未实现
#include "XControlBasic.h"
#include "XResourceManager.h"
namespace XE {
class XProgressRing :public XControlBasic
{
private:
	XBool m_isInited;	//是否初始化
	float m_r0;	//外径
	float m_r1;	//内径
	XBool m_withBG;
	XBool m_withText;
	XFontUnicode m_caption;
	XFColor m_textColor;		//文字的颜色

//	XVec2 m_position;
	float m_curValue;					//进度条的当前值
	bool m_withVirtual;			//是否使用虚拟的数据进行中间过渡
	XMoveData m_valueMD;
public:
	//r0:外径
	//r1:内径
	XBool initWithoutSkin(float r0, float r1, const XFontUnicode& font, 
		bool withBG = false, bool withText = true);
	XBool initWithoutSkin(float r0, float r1, bool withBG = false, bool withText = true)
	{
		return initWithoutSkin(r0, r1, getDefaultFont(), withBG, withText);
	}

	XProgressRing()
		:m_isInited(XFalse)
		, m_curValue(0.0f)
		, m_withText(XTrue)
		, m_withBG(XTrue)
		, m_textColor(1.0f, 1.0f)
		, m_withVirtual(false)
	{
		m_ctrlType = CTRL_OBJ_PROGRESSRING;
	}
	~XProgressRing() { release(); }
	void release();

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color);	//设置字体的颜色
	const XFColor& getTextColor() const { return m_textColor; }	//获取控件字体的颜色

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);

	void setValue(float temp);	//0 - 100之间
	float getValue() const { return m_curValue; }

	//是否显示中间过程
	bool getWithVirtual()const { return m_withVirtual; }
	void setWithVirtual(bool flag) { m_withVirtual = flag; }
protected:
	void update(float stepTime) { m_valueMD.move(stepTime); }
	void draw();
	void drawUp() {};						//do nothing
	XBool mouseProc(const XVec2&, XMouseState) { return XTrue; }	//do nothing
	XBool keyboardProc(int, XKeyState) { return XTrue; }	//do nothing
	void insertChar(const char *, int) { ; }
	XBool canGetFocus(const XVec2&) { return XFalse; }	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&) { return XTrue; }
public:
	XBool setACopy(const XProgressRing &temp);		//建立一个副本(尚未完成)
	//下面是内联函数
	void enable() { m_isEnable = XTrue; }
	void disable();
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	XProgressRing(const XProgressRing &temp);
	XProgressRing& operator = (const XProgressRing& temp);
};
#if WITH_INLINE_FILE
#include "XProgressRing.inl"
#endif
}
#endif