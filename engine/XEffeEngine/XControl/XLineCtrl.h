#ifndef _JIA_XLINECTRL_
#define _JIA_XLINECTRL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
//这是一个基本原控件线
#include "XControlBasic.h"
#include "../XFontUnicode.h"

#define XLINE_CTRL_LINE_SUM (10)

enum _XLineCtrlType
{
	LINE_CTRL_TYPE_X_D,
	LINE_CTRL_TYPE_X_U,
	LINE_CTRL_TYPE_Y_R,
	LINE_CTRL_TYPE_Y_L,
};
class _XLineCtrl:public _XControlBasic
{
private:
	_XBool m_isInited;		//是否初始化
	_XVector2 m_position;	//控件的位置
	_XVector2 m_size;		//大小
	_XVector2 m_pixelSize;	//像素大小
	_XVector2 m_truePixelSize;	//真实的像素尺寸
	_XFontUnicode m_font;
	char m_textStr[64];		//显示的字符串

	_XLineCtrlType m_type;
	bool m_withFont;
	bool m_isDown;		//是否被鼠标拾取
	void updateData();

	void (*m_funDataChange)(void *,int ID);
	void *m_pClass;	

	bool m_withRange;	//范围设置是否有效
	_XVector2 m_range;	//可以移动的范围
	bool m_withMap;		//是否进行映射
	_XVector2 m_mapRange;	//映射的范围
	float m_mapValue;

	_XCtrlMode m_ctrlMode;		//控件的模式
	
	bool m_withString;			//是否有字符串
	string m_showString;		//需要显示的字符串
	_XFontUnicode m_stringFont;	//字符串的字体
public:
	void setString(const char * str)
	{
		if(!m_withFont) return;
		if(str == NULL)
		{
			m_withString = false;
			return;
		}
		m_showString = str;
		m_stringFont.setString(m_showString.c_str());
		m_withString = true;
	}

	void setCtrlMode(_XCtrlMode mode){m_ctrlMode = mode;}
	void setRange(float min,float max)
	{
		m_withRange = true;
		m_range.set(min,max);
		updateData();
	}
	void disRange(){m_withRange = false;}
	float getValue() 
	{
		if(m_withMap) return m_mapValue;
		if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U) return m_position.x;
		else return m_position.y;
	}
	void setValue(float value)
	{
		if(m_withMap) 
		{
			m_mapValue = value;
			if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U) m_position.x = maping1D(m_mapValue,m_mapRange.x,m_mapRange.y,m_range.x,m_range.y);
			else m_position.y = maping1D(m_mapValue,m_mapRange.x,m_mapRange.y,m_range.x,m_range.y);
			updateData();
		}
	}
	void setMapRange(float min,float max)
	{
		m_withMap = true;
		m_mapRange.set(min,max);
	}
	void disMapRange(){m_withMap = false;}
public:
	_XBool init(const _XVector2& position,_XLineCtrlType type = LINE_CTRL_TYPE_X_D,const _XFontUnicode *font = NULL);
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
	_XBool setACopy(const _XLineCtrl & temp);

	virtual void justForTest() {;}

	_XLineCtrl()
		:m_isInited(false)
		,m_position(0.0f,0.0f)
		,m_size(1.0f,1.0f)
		,m_withFont(false)
		,m_pixelSize(10.0f,10.0f)
		,m_isDown(false)
		,m_type(LINE_CTRL_TYPE_X_D)
		,m_range(0.0f,100.0f)
		,m_withRange(false)
		,m_withMap(false)		//是否进行映射
		,m_mapRange(0.0f,100.0f)	//映射的范围
		,m_mapValue(0.0f)
		,m_funDataChange(NULL)
		,m_pClass(NULL)
		,m_ctrlMode(CTRL_MODE_SIMPLE)
		,m_withString(false)
	{
	}
	~_XLineCtrl()
	{
		release();
	}
};
inline _XBool _XLineCtrl::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
inline _XVector2 _XLineCtrl::getBox(int order)
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