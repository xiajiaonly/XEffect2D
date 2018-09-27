#ifndef _JIA_XSLIDERINFINITE_
#define _JIA_XSLIDERINFINITE_
#include "XControl\XSlider.h"

namespace XE
{
class XSliderInfinite :public XControlBasic, public XBasicOprate
{
private:
	bool m_isInited;
	XVec2 m_size;	//像素大小
	XSlider m_mainSld;	//主的滑动条
	XButton m_addMinBtn;
	XButton m_desMinBtn;
	XButton m_addMaxBtn;
	XButton m_desMaxBtn;

	float m_timer;

	static void ctrlProc(void*, int, int);
	enum XChangeRangeType
	{
		TYPE_ADD_MIN,
		TYPE_DES_MIN,
		TYPE_ADD_MAX,
		TYPE_DES_MAX,
	};
	void changeRange(XChangeRangeType type);
public:
	enum XSliderInfiniteEvent
	{
		SLDI_MOUSE_ON,
		SLDI_MOUSE_DOWN,
		SLDI_MOUSE_UP,
		SLDI_MOUSE_MOVE,
		SLDI_VALUE_CHANGE,
	};
protected:
	void update(float stepTime);
	void draw();//描绘滑动条
	void drawUp();						//do nothing
	XBool mouseProc(const XVec2& p, XMouseState mouseState);	//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder, XKeyState keyState);
	void insertChar(const char *, int) { ; }
	XBool canGetFocus(const XVec2& p);	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&) { return XTrue; }
	void setLostFocus() { m_mainSld.setLostFocus(); }	//设置失去焦点
public:
	XBool initWithoutSkin(const XVec2& size, float maxValue = 100.0f,
		float minValue = 0.0f, XSliderType type = SLIDER_TYPE_HORIZONTAL);
	XSliderInfinite();
	~XSliderInfinite() { release(); }
	XBool setFont(const char *caption) { return m_mainSld.setFont(caption);}

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color) { m_mainSld.setTextColor(color); }	//设置字体的颜色
	const XFColor& getTextColor() const { return m_mainSld.getTextColor(); }	//获取控件字体的颜色

	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(const XFColor& c);//设置字体的颜色
	void setAlpha(float a);

	void disable();		//使控件无效
	void enable();		//使控件有效
	void setVisible();
	void disVisible();
	float getCurValue() const;	//获取滑块当前的值
	float getMaxValue() const;
	float getMinValue() const;
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p); //点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);		//获取四个顶点的坐标，目前先不考虑旋转和缩放

	void setCurValue(float temp, bool withEvent = false);			//设置滑块当前的值
	void setRange(float max, float min);

	void release();
private:	//为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	XSliderInfinite(const XSliderInfinite &temp);
	XSliderInfinite& operator = (const XSliderInfinite& temp);
public:
	void setConnectVar(float * p) { m_mainSld.setConnectVar(p); }	//关联变量
	void disConnectVar() { m_mainSld.disConnectVar(); }			//取消变量关联
public:
	virtual void setOprateState(void * data);
	virtual void *getOprateState() const;
	virtual void releaseOprateStateData(void *p);
	virtual void stateChange()
	{
		if (m_withUndoAndRedo) XOpManager.addAOperate(this);	//如果需要记录当前状态则将调用动作管理器的相关函数
		if (m_funStateChange != NULL) m_funStateChange(m_pStateClass);	//调用相关的回调函数
	}
	virtual bool isSameState(void * data);
	void setIsInteger(bool flag) { m_mainSld.setIsInteger(flag); }
	bool getIsInteger()const { return m_mainSld.getIsInteger(); }
};
#if WITH_INLINE_FILE
#include "XSliderInfinite.inl"
#endif
}
#endif