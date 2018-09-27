#ifndef _JIA_XPARAMETERCTRL_
#define _JIA_XPARAMETERCTRL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.9
//--------------------------------
#include "XControl\XButton.h"
//这是一个用于非线程变化的参数配置的控件
//而且数据没有范围
namespace XE {
enum XParamType
{
	TYPE_LINE,		//线程变化
	TYPE_EXPONENT,	//指数变化
};
class XParameterCtrl :public XControlBasic
{
private:
	bool m_isInited;
	XVec2 m_pixelSize;	//尺寸

	XFontUnicode m_font;

	XButton m_btn[2];
	XFColor m_textColor;

	XParamType m_paramType;
	float m_increaseRate;	//放大的系数
	float m_reduceRate;		//缩小的系数
	float m_curData;
	static void ctrlProc(void*, int, int);
	void increaseData();
	void reduceData();
public:
	enum XParamCtrlEvent
	{
		PC_VALUE_CHANGE,
	};
	void setCurValue(float value);
	float getCurValue()const { return m_curData; }
	bool initWithoutSkin(float curValue, const XVec2& pixelSize, const XFontUnicode& font);
	bool initWithoutSkin(float curValue, float width)
	{
		return initWithoutSkin(curValue, XVec2(width, MIN_FONT_CTRL_SIZE), getDefaultFont());
	}
	void setParamType(XParamType type, float iRate, float rRate);

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(const XVec2& s);
	void setTextColor(const XFColor& color) { m_textColor = color; }	//设置字体的颜色
	const XFColor& getTextColor() const { return m_textColor; }	//获取控件字体的颜色

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p, XMouseState mouseState);		//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder, XKeyState keyState);			//返回是否触发按键动作
	void insertChar(const char *, int) { ; }
	XBool canGetFocus(const XVec2& p);				//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&) { return XTrue; }	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点
public:
	XParameterCtrl()
		:m_isInited(false)
		, m_paramType(TYPE_EXPONENT)
		, m_increaseRate(1.1f)
		, m_reduceRate(0.9f)
		, m_curData(1.0f)
	{
		m_ctrlType = CTRL_OBJ_PARAMCTRL;
	}
	~XParameterCtrl() { release(); }
	void release();
	XBool setACopy(const XParameterCtrl &temp) { return XFalse; }	//尚未实现
	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XParameterCtrl(const XParameterCtrl &temp);
	XParameterCtrl& operator = (const XParameterCtrl& temp);
};
#if WITH_INLINE_FILE
#include "XParameterCtrl.inl"
#endif
}
#endif