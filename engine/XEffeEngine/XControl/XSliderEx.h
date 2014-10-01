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

class _XSliderEx:public _XControlBasic,public _XBasicOprate
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
	friend void stateChangeProc(void *);
protected:
	void update(int stepTime);
	void draw();//描绘滑动条
	void drawUp(){;}						//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}
	//void setLostFocus();	//设置失去焦点
public:
	_XBool initWithoutTex(const _XRect& area,float max = 100.0f,float min = 0.0f,
		_XSliderType type = SLIDER_TYPE_HORIZONTAL,const _XVector2 &fontPosition = _XVector2::zero);
	_XBool initWithoutTex(const _XVector2& pixelSize,float max = 100.0f,float min = 0.0f,
		_XSliderType type = SLIDER_TYPE_HORIZONTAL,const _XVector2 &fontPosition = _XVector2::zero)
	{
		return initWithoutTex(_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),max,min,type,fontPosition);
	}
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
	{
		m_ctrlType = CTRL_OBJ_SLIDEREX;
	}
	virtual ~_XSliderEx(){release();}

	void release();
	//下面是一些公共方法
	_XBool setFont(const char *caption,const _XFontUnicode &font,float captionSize,const _XVector2 &fontPosition);
	_XBool setFontEx(const char *caption,const _XFontUnicode &font,float captionSize);

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color){m_mainSld.setTextColor(color);}	//设置字体的颜色
	_XFColor getTextColor() const {return m_mainSld.getTextColor();}	//获取控件字体的颜色

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
	void setVisible();
	void disVisible();
	float getNowValue() const;	//获取滑块当前的值
	float getMaxValue() const; 
	float getMinValue() const;
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y); //点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);		//获取四个顶点的坐标，目前先不考虑旋转和缩放
	//virtual void justForTest() {;}
private:	//为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XSliderEx(const _XSliderEx &temp);
	_XSliderEx& operator = (const _XSliderEx& temp);
	//下面是关于关联变量的接口
public:
	void setConnectVar(float * p) {m_mainSld.setConnectVar(p);}	//关联变量
	void disConnectVar() {m_mainSld.disConnectVar();}			//取消变量关联
public:
	void setOprateState(void * data);
	void *getOprateState() const;
	virtual bool isSameState(void * data);
};
#include "XSliderEx.inl"

#endif