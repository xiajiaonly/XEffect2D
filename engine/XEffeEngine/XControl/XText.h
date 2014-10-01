#ifndef _JIA_XTEXT_
#define _JIA_XTEXT_
//这个是文字控件的类
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.14
//--------------------------------
#include "XControlBasic.h"
#include "../XBasicWindow.h"

class _XText:public _XControlBasic
{
private:
	_XBool m_isInited;
	_XFontUnicode m_font;
public:
	_XBool init(const _XFontUnicode &font);
	_XBool init() {return init(XEE::systemFont);}
	_XBool init(const char * p)
	{
		if(!init(XEE::systemFont)) return XFalse;
		setString(p);
		return XTrue;
	}
	void setString(const char * str){m_font.setString(str);}
	const char *getCaptionString() const {return m_font.getString();}
	_XText()
		:m_isInited(XFalse)
	{
		m_ctrlType = CTRL_OBJ_XTEXT;
	}
	~_XText(){release();}
	void release();

	//下面是通用接口
	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);
	using _XObjectBasic::setSize;	//避免覆盖的问题
	void setSize(float x,float y);
	void setTextColor(const _XFColor& color){m_font.setColor(color);}	//设置字体的颜色
	_XFColor getTextColor() const {return m_font.getColor();}	//获取控件字体的颜色
	using _XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
protected:
	void draw();								//描绘按钮
	void drawUp(){};							//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState){return XFalse;}		//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState){return XFalse;}			//返回是否触发按键动作
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y){return XFalse;}				//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点
	_XBool canBeChoose(){return XFalse;}
public:
	_XBool isInRect(float x,float y){return m_font.isInRect(x,y);}		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order){return m_font.getBox(order);}			//获取四个顶点的坐标，目前先不考虑旋转和缩放
private://下面为了防止错误，重载赋值操作符，复制构造函数
	_XText(const _XText &temp);
	_XText& operator = (const _XText& temp);
};
#include "XText.inl"
#endif