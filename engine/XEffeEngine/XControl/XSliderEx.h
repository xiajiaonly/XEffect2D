#ifndef _JIA_XSLIDEREX_
#define _JIA_XSLIDEREX_
//基本完成部分测试
//这是一个带微调开关的slider的类，这是一个复合类。
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.6
//--------------------------------
#include "XSlider.h"
namespace XE{
class XSliderEx:public XControlBasic,public XBasicOprate
{
private:
	XBool m_isInited;
	XSlider m_mainSld;			//主体的滑动条
	XSlider m_secondarySld;	//辅助的滑动条
	XButton m_chooseBtn;		//切换辅助滑动条的按钮

	int m_timer;	//计时器，用于判断各种案件状态是否需要显示
public:
	enum XSliderExEvent
	{
		SLDEX_INIT,
		SLDEX_RELEASE,
		SLDEX_MOUSE_ON,
		SLDEX_MOUSE_DOWN,
		SLDEX_MOUSE_UP,
		SLDEX_MOUSE_MOVE,
		SLDEX_VALUE_CHANGE,
	};
private:
	static void stateChangeProc(void *);
	static void ctrlProc(void*,int,int);
protected:
	void update(float stepTime);
	void draw();//描绘滑动条
	void drawUp();						//do nothing
	XBool mouseProc(const XVec2& p,XMouseState mouseState);	//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&){return XTrue;}
	void setLostFocus(){m_mainSld.setLostFocus();}	//设置失去焦点
public:
	XBool initWithoutSkin(const XRect& area,float max = 100.0f,float min = 0.0f,
		XSliderType type = SLIDER_TYPE_HORIZONTAL,const XVec2& fontPosition = XVec2(0.0f,16.0f));
	XBool initWithoutSkin(const XVec2& pixelSize,float max = 100.0f,float min = 0.0f,
		XSliderType type = SLIDER_TYPE_HORIZONTAL,const XVec2& fontPosition = XVec2(0.0f,16.0f))
	{
		return initWithoutSkin(XRect(XVec2::zero,pixelSize),max,min,type,fontPosition);
	}
	XSliderEx()
		:m_isInited(XFalse)
		,m_timer(0)
	{
		m_ctrlType = CTRL_OBJ_SLIDEREX;
	}
	virtual ~XSliderEx()
	{
		release();
		if(gFrameworkData.pOperateManager != NULL)
		gFrameworkData.pOperateManager->decreaseObj(this);
	}

	void release();
	//下面是一些公共方法
	XBool setFont(const char *caption,const XFontUnicode& font,float captionSize = 1.0f,const XVec2& fontPosition = XVec2(0.0f, 16.0f));
	XBool setFont(const char *caption,const XVec2& fontPosition)
	{
		return setFont(caption,getDefaultFont(),1.0f,fontPosition);
	}
	XBool setFont(const char *caption)
	{
		return m_mainSld.setFont(caption);
	}

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color){m_mainSld.setTextColor(color);}	//设置字体的颜色
	const XFColor& getTextColor() const {return m_mainSld.getTextColor();}	//获取控件字体的颜色

	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(const XFColor& c);//设置字体的颜色
	void setAlpha(float a);

	XBool setACopy(const XSliderEx &temp);			//建立一个副本

	void setCurValue(float temp, bool withEvent = false);			//设置滑块当前的值
	void setRange(float max,float min);

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
	//virtual void justForTest() {;}
private:	//为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	XSliderEx(const XSliderEx &temp);
	XSliderEx& operator = (const XSliderEx& temp);
	//下面是关于关联变量的接口
public:
	void setConnectVar(float * p) {m_mainSld.setConnectVar(p);}	//关联变量
	void disConnectVar() {m_mainSld.disConnectVar();}			//取消变量关联
public:
	virtual void setOprateState(void * data);
	virtual void *getOprateState() const;
	virtual void releaseOprateStateData(void *p);
	virtual void stateChange()
	{
		if(m_withUndoAndRedo) XOpManager.addAOperate(this);	//如果需要记录当前状态则将调用动作管理器的相关函数
		if(m_funStateChange != NULL) m_funStateChange(m_pStateClass);	//调用相关的回调函数
	}
	virtual bool isSameState(void * data);
	void setIsInteger(bool flag){m_mainSld.setIsInteger(flag);}
	bool getIsInteger()const{return m_mainSld.getIsInteger();}
};
#if WITH_INLINE_FILE
#include "XSliderEx.inl"
#endif
}
#endif