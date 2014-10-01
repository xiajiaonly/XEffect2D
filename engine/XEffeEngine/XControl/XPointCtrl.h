#ifndef _JIA_XPOINTCTRL_
#define _JIA_XPOINTCTRL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XControlBasic.h"

//这是一个基本原控件点，用于响应鼠标的位置操作
#define XPOINT_CTRL_LINE_SUM (10)
class _XPointCtrl:public _XControlBasic
{
private:
	_XBool m_isInited;		//是否初始化
//	_XVector2 m_position;	//控件的位置
//	_XVector2 m_size;		//大小
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
	void setRange(float l,float t,float r,float b);
	void disRange(){m_withRange = false;}
	_XVector2 getValue();
	void setMapRange(float l,float t,float r,float b);
	void disMapRange(){m_withMap = false;}
public:
	_XBool init(const _XVector2& position,const _XFontUnicode *font = NULL);
	void draw();
	void drawUp(){;}
	void setCallbackFun(void (* funDataChange)(void *,int),void *pClass = NULL);

	_XBool mouseProc(float x,float y,_XMouseState mouseState);
	_XBool keyboardProc(int,_XKeyState){return XTrue;}		//do nothing
	_XBool canGetFocus(float x,float y);	//do nothing	//事件可以穿透，如果返回XTrue则鼠标事件不能穿透
	_XBool canLostFocus(float,float){return XTrue;}	//do nothing

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	void insertChar(const char *,int){;}			//do nothing

//	void setLostFocus() {m_isBeChoose = XFalse;}							//do nothing
	void disable();	
	void enable(){m_isEnable = XTrue;}
	void release();

	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	_XBool setACopy(const _XPointCtrl & temp);

	//virtual void justForTest() {;}

	_XPointCtrl()
		:m_isInited(XFalse)
//		,m_position(0.0f,0.0f)
//		,m_size(1.0f,1.0f)
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
		m_ctrlType = CTRL_OBJ_POINTCTRL;
	}
	~_XPointCtrl() {release();}
};
#include "XPointCtrl.inl"

#endif