#ifndef _JIA_XCOLORCHOOSE_
#define _JIA_XCOLORCHOOSE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.9
//--------------------------------
#include "XControlBasic.h"
#include "../XXml.h"
namespace XE{
//这是一个用于选择颜色的控件
class XColorChoose:public XControlBasic
{
public:
	enum XColorChooseType
	{
		TYPE_WITH_TEXT,
		TYPE_NO_TEXT,
	};
	enum XColorChooseEvent
	{
		CLC_COLOR_CHANGE,	//选择的颜色发生了改变
	};
private:
	XBool m_isInited;
	XFColor m_curColor;	//当前的颜色
	XVector2 m_area;		//范围

	XFontUnicode m_font;
	XColorChooseType m_type;
	XBool m_needChooseColor;
public:
	void setType(XColorChooseType type){m_type = type;}
	XBool initWithoutSkin(const XVector2 &size,const XCColor &color);
protected:
	void draw();
	void drawUp(){}						
	XBool mouseProc(float x,float y,XMouseState mouseState);		//对于鼠标动作的响应函数
	XBool keyboardProc(int,XKeyState){return XFalse;}			//返回是否触发按键动作
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y)				//用于判断当前物件是否可以获得焦点
	{
		if(!m_isInited ||		//如果没有初始化直接退出
			!m_isActive ||		//没有激活的控件不接收控制
			!m_isVisible ||		//如果不可见直接退出
			!m_isEnable) return XFalse;		//如果无效则直接退出
		return m_curMouseRect.isInRect(x,y);
	}
	XBool canLostFocus(float,float){return XTrue;}	//应该是可以随时失去焦点的
	//void setLostFocus();	//设置失去焦点
public:
	void update(float stepTime);
	XColorChoose()
		:m_isInited(XFalse)
		,m_type(TYPE_WITH_TEXT)
		,m_needChooseColor(XFalse)
	{
		m_ctrlType = CTRL_OBJ_COLORCHOOSE;
	}
	~XColorChoose(){release();}	//do nothing
	void release();
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XColorChoose(const XColorChoose &temp);
	XColorChoose& operator = (const XColorChoose& temp);
private:
	void updateColor();
public:
	XBool isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	{
		if(!m_isInited) return XFalse;
		return m_curMouseRect.isInRect(x,y);
	}
	XVector2 getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	{
		if(!m_isInited) return XVector2::zero;
		switch(order)
		{
		case 0:return XVector2(m_curMouseRect.left,m_curMouseRect.top);
		case 1:return XVector2(m_curMouseRect.right,m_curMouseRect.top);
		case 2:return XVector2(m_curMouseRect.right,m_curMouseRect.bottom);
		case 3:return XVector2(m_curMouseRect.left,m_curMouseRect.bottom);
		}
		return XVector2::zero;
	}

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y)
	{
		m_position.set(x,y);
		m_font.setPosition(m_position.x + 2 * m_scale.x,m_position.y + m_area.y * m_scale.y * 0.5f);
		m_curMouseRect.set(m_position.x,m_position.y,
			m_position.x + m_area.x * m_scale.x,m_position.y + m_area.y * m_scale.y);
	}

	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(float x,float y)
	{
		m_scale.set(x,y);
		m_font.setPosition(m_position.x + 2 * m_scale.x,m_position.y + m_area.y * m_scale.y * 0.5f);
		m_font.setScale(m_scale);
		m_curMouseRect.set(m_position.x,m_position.y,
			m_position.x + m_area.x * m_scale.x,m_position.y + m_area.y * m_scale.y);
	}

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a){m_color.setColor(r,g,b,a);}
	void setAlpha(float a){m_color.setA(a);}

	XBool setACopy(const XColorChoose &temp);

	XFColor getCurColor() const {return m_curColor;}
	void setCurColor(const XFColor &c) 
	{
		m_curColor = c;
		updateColor();
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(完成)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		XFColor tmp = getCurColor();
		if(!XXml::addLeafNode(e,(m_ctrlName + "R").c_str(),XString::toString(tmp.fR))) return XFalse; 
		if(!XXml::addLeafNode(e,(m_ctrlName + "G").c_str(),XString::toString(tmp.fG))) return XFalse; 
		if(!XXml::addLeafNode(e,(m_ctrlName + "B").c_str(),XString::toString(tmp.fB))) return XFalse; 
		if(!XXml::addLeafNode(e,(m_ctrlName + "A").c_str(),XString::toString(tmp.fA))) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		XFColor tmp;
		if(XXml::getNextXmlAsFloat(e,(m_ctrlName + "R").c_str(),tmp.fR) == NULL) return XFalse;
		if(XXml::getNextXmlAsFloat(e,(m_ctrlName + "G").c_str(),tmp.fG) == NULL) return XFalse;
		if(XXml::getNextXmlAsFloat(e,(m_ctrlName + "B").c_str(),tmp.fB) == NULL) return XFalse;
		if(XXml::getNextXmlAsFloat(e,(m_ctrlName + "A").c_str(),tmp.fA) == NULL) return XFalse;
		setCurColor(tmp);
		return XTrue;
	}
	//---------------------------------------------------------
};
}
#endif