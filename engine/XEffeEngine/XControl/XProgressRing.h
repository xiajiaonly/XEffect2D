#ifndef _JIA_XPROGRESSRING_
#define _JIA_XPROGRESSRING_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.18
//--------------------------------
//这是一个环形进度条，目前尚未实现
#include "XControlBasic.h"
#include "XResourcePack.h"
#include "XResourceManager.h"

class _XProgressRing:public _XControlBasic
{
private:
	_XBool m_isInited;	//是否初始化
	float m_r0;	//外径
	float m_r1;	//内径
	_XBool m_withBG;
	_XBool m_withText;
	_XFontUnicode m_caption;
	_XFColor m_textColor;		//文字的颜色

//	_XVector2 m_position;
	float m_nowValue;					//进度条的当前值
public:
	_XBool initWithoutTex(float r0,float r1,const _XFontUnicode &font,bool withBG = false,bool withText = true);
	_XBool initWithoutTex(float r0,float r1,bool withBG = false,bool withText = true)
	{
		return initWithoutTex(r0,r1,XEE::systemFont,withBG,withText);
	}

	_XProgressRing()
		:m_isInited(XFalse)
		,m_nowValue(0.0f)
		,m_withText(XTrue)
		,m_withBG(XTrue)
		,m_textColor(1.0f,1.0f,1.0f,1.0f)
	{
		m_ctrlType = CTRL_OBJ_PROGRESSRING;
	}
	~_XProgressRing(){release();}
	void release();

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//避免覆盖的问题
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color);	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	void setValue(float temp);
	float getValue() const{return m_nowValue;}
protected:
	void draw();
	void drawUp(){};						//do nothing
	_XBool mouseProc(float,float,_XMouseState){return XTrue;}	//do nothing
	_XBool keyboardProc(int,_XKeyState){return XTrue;}	//do nothing
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float,float){return XFalse;}	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}
public:
	_XBool setACopy(const _XProgressRing &temp);		//建立一个副本(尚未完成)
	//下面是内联函数
	void enable(){m_isEnable = XTrue;}
	void disable();
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XProgressRing(const _XProgressRing &temp);
	_XProgressRing& operator = (const _XProgressRing& temp);
};
#include "XProgressRing.inl"
#endif