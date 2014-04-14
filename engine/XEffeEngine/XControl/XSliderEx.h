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

class _XSliderEx:public _XControlBasic
{
private:
	_XBool m_isInited;
	_XSlider m_mainSld;			//主体的滑动条
	_XSlider m_secondarySld;	//辅助的滑动条
	_XButton m_chooseBtn;		//切换辅助滑动条的按钮

	int m_timer;	//计时器，用于判断各种案件状态是否需要显示

	void (*m_funInit)(void *,int);		//初始化
	void (*m_funRelease)(void *,int);		//资源释放
	void (*m_funMouseOn)(void *,int);		//鼠标悬浮
	void (*m_funMouseDown)(void *,int);	//鼠标按下
	void (*m_funMouseUp)(void *,int);		//鼠标弹起
	void (*m_funMouseMove)(void *,int);	//鼠标拖动花条时调用
	void (*m_funValueChange)(void *,int);	//滑动条的数值变化
	void *m_pClass;				//回调函数的参数

	friend void sliderExBtnProc(void *pClass,int id);
	friend void sliderExSldProc(void *pClass,int id);
	friend void sliderExSldOnProc(void *pClass,int id);
	friend void sliderExSldUpProc(void *pClass,int id);
	friend void sliderExSldMoveProc(void *pClass,int id);
	friend void sliderExSldChangeProc(void *pClass,int id);
protected:
	void update(int stepTime);
	void draw();//描绘滑动条
	void drawUp(){;}						//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float x,float y){return XTrue;}
	void setLostFocus();	//设置失去焦点
public:
	_XBool initWithoutTex(const _XRect& area,float max = 100.0f,float min = 0.0f,
		_XSliderType type = SLIDER_TYPE_HORIZONTAL,const _XVector2 &fontPosition = _XVector2(0.0f,0.0f));
	_XSliderEx()
		:m_isInited(XFalse)
		,m_timer(0)
		,m_funInit(NULL)		//初始化
		,m_funRelease(NULL)		//资源释放
		,m_funMouseOn(NULL)		//鼠标悬浮
		,m_funMouseDown(NULL)	//鼠标按下
		,m_funMouseUp(NULL)		//鼠标弹起
		,m_funMouseMove(NULL)	//鼠标拖动花条时调用
		,m_funValueChange(NULL)	//滑动条的数值变化
		,m_pClass(NULL)				//回调函数的参数
	{}
	virtual ~_XSliderEx()
	{release();}

	void release();
	//下面是一些公共方法
	_XBool setFont(const char *caption,const _XFontUnicode &font,float captionSize,const _XVector2 &fontPosition);
	_XBool setFontEx(const char *caption,const _XFontUnicode &font,float captionSize);

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a);//设置字体的颜色
	void setAlpha(float a);

	_XBool setACopy(const _XSliderEx &temp);			//建立一个副本

	//内联函数
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funMouseOn)(void *,int),
		void (* funMouseDown)(void *,int),
		void (* funMouseUp)(void *,int),
		void (*funValueChange)(void *,int),
		void (*funMouseMove)(void *,int),
		void *pClass = NULL);
	void setNowValue(float temp);			//设置滑块当前的值
	void setRange(float max,float min);

	void disable();		//使控件无效
	void enable();		//使控件有效
	void setVisiable();
	void disVisiable();
	float getNowValue() const;	//获取滑块当前的值
	float getMaxValue() const; 
	float getMinValue() const;
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y); //点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);		//获取四个顶点的坐标，目前先不考虑旋转和缩放
	virtual void justForTest() {;}
private:	//为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XSliderEx(const _XSliderEx &temp);
	_XSliderEx& operator = (const _XSliderEx& temp);
};
inline void _XSliderEx::disable()//使控件无效
{
	m_mainSld.disable();
	m_secondarySld.disable();
	m_chooseBtn.disable();
}
inline void _XSliderEx::enable()//使控件有效
{
	m_mainSld.enable();
	m_secondarySld.enable();
	m_chooseBtn.enable();
}
inline float _XSliderEx::getNowValue() const	//获取滑块当前的值
{
	return m_mainSld.getNowValue();
}
inline float _XSliderEx::getMaxValue() const
{
	return m_mainSld.getMaxValue();
}
inline float _XSliderEx::getMinValue() const
{
	return m_mainSld.getMinValue();
}
inline void _XSliderEx::setCallbackFun(void (* funInit)(void *,int),
	void (* funRelease)(void *,int),
	void (* funMouseOn)(void *,int),
	void (* funMouseDown)(void *,int),
	void (* funMouseUp)(void *,int),
	void (* funValueChange)(void *,int),
	void (* funMouseMove)(void *,int),
	void *pClass)
{
	m_funInit = funInit;
	m_funRelease = funRelease;
	m_funMouseOn = funMouseOn;
	m_funMouseDown = funMouseDown;		//有效
	m_funMouseUp = funMouseUp;
	m_funValueChange = funValueChange;
	m_funMouseMove = funMouseMove;
	m_pClass = pClass;
}
inline void _XSliderEx::setNowValue(float temp)
{
	m_mainSld.setNowValue(temp);
}
inline void _XSliderEx::setRange(float max,float min)
{
	m_mainSld.setRange(max,min);
}
inline _XBool _XSliderEx::isInRect(float x,float y) //点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
inline _XVector2 _XSliderEx::getBox(int order)		//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(!m_isInited) return ret;
	return m_mainSld.getBox(order);
	return ret;
}
inline void _XSliderEx::setColor(float r,float g,float b,float a)
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);
	m_mainSld.setColor(m_color);
	m_secondarySld.setColor(m_color);
	m_chooseBtn.setColor(m_color);
	updateChildColor();
}//设置字体的颜色
inline void _XSliderEx::setAlpha(float a)
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_mainSld.setColor(m_color);
	m_secondarySld.setColor(m_color);
	m_chooseBtn.setColor(m_color);
	updateChildAlpha();
}
inline void _XSliderEx::release()
{
	if(!m_isInited) return;
	if(m_funRelease != NULL) m_funRelease(m_pClass,getControlID());
	m_isInited = false;
}
//下面是一些公共方法
inline _XBool _XSliderEx::setFont(const char *caption,const _XFontUnicode &font,float captionSize,const _XVector2 &fontPosition)
{
	return m_mainSld.setFont(caption,font,captionSize,fontPosition);
}
inline _XBool _XSliderEx::setFontEx(const char *caption,const _XFontUnicode &font,float captionSize)
{
	return m_mainSld.setFontEx(caption,font,captionSize);
}
inline _XBool _XSliderEx::keyboardProc(int keyOrder,_XKeyState keyState)
{
	return m_mainSld.keyboardProc(keyOrder,keyState);
}
inline void _XSliderEx::setLostFocus() 
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	if(!m_isActive) return;		//没有激活的控件不接收控制
	if(!m_isVisiable) return;	//如果不可见直接退出
	if(!m_isEnable) return;		//如果无效则直接退出

	m_mainSld.setLostFocus();
	m_secondarySld.setLostFocus();
	m_chooseBtn.setLostFocus();
}
inline void _XSliderEx::setVisiable()
{
	m_isVisiable = XTrue;
	m_mainSld.setVisiable();
	//m_secondarySld.setVisiable();
	//m_chooseBtn.setVisiable();
	updateChildVisiable();
}
inline void _XSliderEx::disVisiable()
{
	m_isVisiable = XFalse;
	m_mainSld.disVisiable();
	m_secondarySld.disVisiable();
	m_chooseBtn.disVisiable();
	updateChildVisiable();
}
#endif