#ifndef _JIA_XSLIDERRING_
#define _JIA_XSLIDERRING_
#include "XSlider.h"
namespace XE {
enum XSliderRingMode
{
	SLDR_MODE_VERTICAL,		//垂直的
	SLDR_MODE_HORIZONTAL,	//水平的
	SLDR_MODE_RING,			//环形的
};
enum XSliderRingState
{
	SLDR_STATE_NORMAL,		//普通状态
	SLDR_STATE_DOWN,			//鼠标按下
	SLDR_STATE_ON,			//鼠标悬浮
	SLDR_STATE_DISABLE		//无效状态
};
class XSliderRing :public XControlBasic
{
private:
	bool m_isInited;
	XVec2 m_size;
	float m_curValue;
	float m_maxValue;
	float m_minValue;
	XSliderRingMode m_mode;
	int m_isClockwise;	//是否为顺时针方向：-1:未知，0:逆时针方向，1:顺时针方向
	//是否是静态模式，静态模式时值是绝对的，动态模式是值是每次移动的量
	//bool m_isStaticMode;	//(尚未实现)
	XVec2 m_mouseFlyArea;		//鼠标拖曳是飞出的响应范围

	XVec2 m_oldMousePos;
	float m_upValue;			//滑动条的上一次值(定义这个变量的目的在于如果拖动滑块超出范围之后可以恢复到拖动之前得知，但是目前出于简单的考虑，不做这个动作)
	bool m_dataChanged;
	float m_keyOneValue;		//按下一次按键的滑动量(注意这个值比需要>=0,否则将会造成错误)

	XFColor m_textColor;	//文字的颜色
	bool m_withFont;		//是否有字体
	std::string m_fontString;	//文字内容
	XFontUnicode m_caption;	//显示的字体
	void updateFontStr();	//根据当前值更新字符串的内容

	XSliderRingState m_curSliderState;
	void updateCurValue(const XVec2& pos);
	float m_trimValue;	//微调的量
public:
	enum XSliderRingEvent
	{
		SLDR_MOUSE_DOWN,
		SLDR_MOUSE_UP,
		SLDR_MOUSE_MOVE,
		SLDR_VALUE_TRIM,		//微调事件，滚动滚轮时会触发微调事件，用于控制微小数据的变化
		SLDR_VALUE_CHANGE,
	};
public:
	bool initWithoutSkin(const XVec2& size,float maxValue = 100.0f,float minValue = 0.0f,
		XSliderRingMode mode = SLDR_MODE_HORIZONTAL);
	XBool setFont(const char* caption, const XFontUnicode& font);
	XBool setFont(const char* caption)
	{
		return setFont(caption, getDefaultFont());
	}

	using XObjectBasic::setPosition;
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color);
	const XFColor& getTextColor() const { return m_textColor; }

	using XObjectBasic::setColor;
	void setColor(const XFColor& c);
	void setAlpha(float a);
protected:
	void draw();
	void drawUp() {}
	XBool mouseProc(const XVec2& p, XMouseState mouseState);
	XBool keyboardProc(int keyOrder, XKeyState keyState);
	void insertChar(const char*, int) { ; }
	XBool canGetFocus(const XVec2& p);	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&) { return  !(m_curSliderState == SLDR_STATE_DOWN);}
	void setLostFocus();	//设置失去焦点
public:
	XSliderRing();
	~XSliderRing() { release(); }
	void release();
	void disable();		//使控件无效
	void enable();		//使控件有效
	float getCurValue()const { return m_curValue; }
	//返回两次变化角度之间的差值
	float getDistanceValue()const;
	float getTrimValue()const { return m_trimValue; }

	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
private:	//为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	XSliderRing(const XSliderRing &temp);
	XSliderRing& operator = (const XSliderRing& temp);
protected:
	void update(float stepTime) {}
};
#if WITH_INLINE_FILE
#include "XSliderRing.inl"
#endif
}
#endif