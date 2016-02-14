#ifndef _JIA_XPARAMETERCTRL_
#define _JIA_XPARAMETERCTRL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.9
//--------------------------------
#include "XControl\XButton.h"
//这是一个用于非线程变化的参数配置的控件
//而且数据没有范围
namespace XE{
enum XParamType
{
	TYPE_LINE,		//线程变化
	TYPE_EXPONENT,	//指数变化
};
class XParameterCtrl:public XControlBasic
{
private:
	bool m_isInited;
	XVector2 m_pixelSize;	//尺寸

	XFontUnicode m_font;

	XButton m_btn[2];
	XFColor m_textColor;

	XParamType m_paramType;
	float m_increaseRate;	//放大的系数
	float m_reduceRate;		//缩小的系数
	float m_curData;
	static void ctrlProc(void*,int,int);
	void increaseData();
	void reduceData();
public:
	enum XParamCtrlEvent
	{
		PC_VALUE_CHANGE,
	};
	void setCurValue(float value)
	{
		m_curData = value;
		m_font.setString(XString::toString(m_curData).c_str());
	}
	float getCurValue()const{return m_curData;}
	bool initWithoutSkin(float curValue,const XVector2 &pixelSize,const XFontUnicode &font);
	bool initWithoutSkin(float curValue,float width)
	{
		return initWithoutSkin(curValue,XVector2(width,MIN_FONT_CTRL_SIZE),getDefaultFont());
	}
	void setParamType(XParamType type,float iRate,float rRate)
	{
		m_paramType = type;
		m_reduceRate = rRate;
		m_increaseRate = iRate;
	}

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y)
	{
		m_position.set(x,y);
		m_btn[0].setPosition(m_position);
		m_btn[1].setPosition(m_position.x + m_pixelSize.x * m_scale.x - m_pixelSize.y * m_scale.y,m_position.y);
		m_font.setPosition(m_position + m_pixelSize * m_scale * 0.5f);	//字放在正中间
		m_curMouseRect.set(m_position.x,m_position.y,m_position.x + m_pixelSize.x * m_scale.x,m_position.y + m_pixelSize.y * m_scale.y);
	}

	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(float x,float y)
	{
		m_scale.set(x,y);
		m_btn[0].setScale(m_scale);
		m_btn[0].setPosition(m_position);
		m_btn[1].setScale(m_scale);
		m_btn[1].setPosition(m_position.x + m_pixelSize.x * m_scale.x - m_pixelSize.y * m_scale.y,m_position.y);
		m_font.setScale(m_scale);
		m_font.setPosition(m_position + m_pixelSize * m_scale * 0.5f);	//字放在正中间
		m_curMouseRect.set(m_position.x,m_position.y,m_position.x + m_pixelSize.x * m_scale.x,m_position.y + m_pixelSize.y * m_scale.y);
	}
	void setTextColor(const XFColor& color){m_textColor = color;}	//设置字体的颜色
	XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a)
	{
		m_color.setColor(r,g,b,a);
		m_font.setColor(m_textColor * m_color);
		m_btn[0].setColor(r,g,b,a);
		m_btn[1].setColor(r,g,b,a);
		updateChildColor();
	}
	void setAlpha(float a)
	{
		m_color.setA(a);
		m_font.setColor(m_textColor * m_color);
		m_btn[0].setColor(m_color);
		m_btn[1].setColor(m_color);
		updateChildColor();
	}

protected:
	void draw()
	{
		if(!m_isInited ||	//如果没有初始化直接退出
			!m_isVisible) return;	//如果不可见直接退出
		//描绘中间的背景
		XRender::drawFillBoxA(m_position,m_pixelSize * m_scale,XFColor::gray,true);
		m_font.draw();
		m_btn[0].draw();
		m_btn[1].draw();
	}
	void drawUp()
	{
		if(!m_isInited ||	//如果没有初始化直接退出
			!m_isVisible) return;	//如果不可见直接退出
		m_btn[0].drawUp();
		m_btn[1].drawUp();
	}
	void update(float stepTime)
	{
		if(!m_isInited) return;
		m_btn[0].update(stepTime);
		m_btn[1].update(stepTime);
	}
	XBool mouseProc(float x,float y,XMouseState mouseState)		//对于鼠标动作的响应函数
	{
		if(!m_isInited ||	//如果没有初始化直接退出
			!m_isActive ||		//没有激活的控件不接收控制
			!m_isVisible) return XFalse; 	//如果不可见直接退出
		if(m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
		XBool ret = m_btn[0].mouseProc(x,y,mouseState);
		XBool ret1 = m_btn[1].mouseProc(x,y,mouseState);
		return (ret || ret1);
	}
	XBool keyboardProc(int keyOrder,XKeyState keyState)			//返回是否触发按键动作
	{
		if(!m_isInited ||	//如果没有初始化直接退出
			!m_isActive ||		//没有激活的控件不接收控制
			!m_isVisible ||	//如果不可见直接退出
			!m_isEnable) return XFalse;		//如果无效则直接退出
		if(m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
		XBool ret = m_btn[0].keyboardProc(keyOrder,keyState);
		XBool ret1 = m_btn[1].keyboardProc(keyOrder,keyState);
		return (ret || ret1);
	}
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
	void setLostFocus()	//设置失去焦点
	{
		if(!m_isInited ||		//如果没有初始化直接退出
			!m_isActive ||		//没有激活的控件不接收控制
			!m_isVisible ||		//如果不可见直接退出
			!m_isEnable) return;		//如果无效则直接退出

		m_isBeChoose = XFalse;	//控件处于焦点状态
	}
public:
	XParameterCtrl()
		:m_isInited(false)
		,m_paramType(TYPE_EXPONENT)
		,m_increaseRate(1.1f)
		,m_reduceRate(0.9f)
		,m_curData(1.0f)
	{
		m_ctrlType = CTRL_OBJ_PARAMCTRL;
	}
	~XParameterCtrl(){release();}
	void release();
	XBool setACopy(const XParameterCtrl &temp){return XFalse;}	//尚未实现
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
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XParameterCtrl(const XParameterCtrl &temp);
	XParameterCtrl& operator = (const XParameterCtrl& temp);
};
}
#endif