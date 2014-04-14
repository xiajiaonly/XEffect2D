#ifndef _JIA_XPOINTCTRL_
#define _JIA_XPOINTCTRL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XControlBasic.h"
#include "../XFontUnicode.h"

//这是一个基本原控件点，用于响应鼠标的位置操作
#define XPOINT_CTRL_LINE_SUM (10)
class _XPointCtrl:public _XControlBasic
{
private:
	_XBool m_isInited;		//是否初始化
	_XVector2 m_position;	//控件的位置
	_XVector2 m_size;		//大小
	_XVector2 m_pixelSize;	//像素大小
	_XVector2 m_truePixelSize;	//真实的像素尺寸
	_XFontUnicode m_font;
	char m_textStr[64];		//显示的字符串

	bool m_withFont;
	bool m_isDown;		//是否被鼠标拾取
	void updateData();

	void (*m_funDataChange)(void *,int ID);	//数据发生变化的时候才调用的回调函数
	void *m_pClass;	

	bool m_withRange;	//范围设置是否有效
	_XRect m_range;		//可以移动的范围
	bool m_withMap;		//是否进行映射
	_XRect m_mapRange;	//映射的范围
	_XVector2 m_mapValue;
	_XCtrlMode m_ctrlMode;		//控件的模式
public:
	void setCtrlMode(_XCtrlMode mode){m_ctrlMode = mode;}
	void setRange(float l,float t,float r,float b)
	{
		m_withRange = true;
		m_range.set(l,t,r,b);
		updateData();
	}
	void disRange(){m_withRange = false;}
	_XVector2 getValue() 
	{
		if(m_withMap) return m_mapValue;
		return m_position;
	}
	void setMapRange(float l,float t,float r,float b)
	{
		m_withMap = true;
		m_mapRange.set(l,t,r,b);
	}
	void disMapRange(){m_withMap = false;}
public:
	_XBool init(const _XVector2& position,const _XFontUnicode *font = NULL);
	void draw();
	void drawUp(){;}
	void setCallbackFun(void (* funDataChange)(void *,int),void *pClass = NULL)
	{
		m_funDataChange = funDataChange;
		m_pClass = pClass;
	}

	_XBool mouseProc(float x,float y,_XMouseState mouseState);
	_XBool keyboardProc(int keyOrder,_XKeyState keyState){return XTrue;}		//do nothing
	_XBool canGetFocus(float x,float y)
	{
		if(!m_isInited) return XFalse;	//如果没有初始化直接退出
		if(!m_isActive) return XFalse;		//没有激活的控件不接收控制
		if(!m_isVisiable) return XFalse;	//如果不可见直接退出
		if(!m_isEnable) return XFalse;		//如果无效则直接退出
		return isInRect(x,y);
	}	//do nothing	//事件可以穿透，如果返回XTrue则鼠标事件不能穿透
	_XBool canLostFocus(float x,float y){return XTrue;}	//do nothing

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y)
	{
		m_position.set(x,y);
		updateData();
		updateChildPos();
	}

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y) 
	{
		m_size.set(x,y);
		updateData();
		updateChildSize();
	}

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a){updateChildColor();}	//do nothing
	void setAlpha(float a){updateChildAlpha();}							//do nothing

	void insertChar(const char *ch,int len){;}			//do nothing

	void setLostFocus() {;}							//do nothing
	void disable(){m_isEnable = 0;}	
	void enable(){m_isEnable = 1;}
	void release();

	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	_XBool setACopy(const _XPointCtrl & temp);

	virtual void justForTest() {;}

	_XPointCtrl()
		:m_isInited(false)
		,m_position(0.0f,0.0f)
		,m_size(1.0f,1.0f)
		,m_withFont(false)
		,m_pixelSize(10.0f,10.0f)
		,m_isDown(false)
		,m_range(0.0f,0.0f,100.0f,100.0f)
		,m_withRange(false)
		,m_mapRange(0.0f,0.0f,100.0f,100.0f)
		,m_withMap(false)
		,m_mapValue(0.0f,0.0f)
		,m_funDataChange(NULL)
		,m_pClass(NULL)
		,m_ctrlMode(CTRL_MODE_SIMPLE)
	{
	}
	~_XPointCtrl()
	{
		release();
	}
};

inline _XBool _XPointCtrl::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
inline _XVector2 _XPointCtrl::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;

	if(order == 0) ret.set(m_position.x - m_truePixelSize.x,m_position.y - m_truePixelSize.y);else
	if(order == 1) ret.set(m_position.x + m_truePixelSize.x,m_position.y - m_truePixelSize.y);else
	if(order == 2) ret.set(m_position.x + m_truePixelSize.x,m_position.y + m_truePixelSize.y);else
	if(order == 3) ret.set(m_position.x - m_truePixelSize.x,m_position.y + m_truePixelSize.y);

	return ret;
}
#endif