#ifndef _JIA_XGROUP_
#define _JIA_XGROUP_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XButton.h"
namespace XE{
//目前尚未实现，但是建议添加的功能
//1、锁定：锁定之后组内的成员状态不会被改变
//2、回退：回退到上一次操作的数值，如果可能可以回退一直到最初的状态
//3、键盘操作：所有的内容都可以通过键盘操作来完成

//这是一个群组的空间，比较假单的一个控件
class XGroup:public XControlBasic
{
public:
	enum XGroupState
	{
		STATE_NORMAL,		//常规状态
		STATE_MINISIZE,		//最小化状态
	};
private:
	static const int m_groupStateBtnSize = 32;
	XBool m_isInited;
	bool m_withRect;
//	XVec2 m_position;	//控件的位置
//	XVec2 m_scale;		//大小
	XRect m_rect;			//控件的范围
	XRect m_drawRect;		//当前空间的范围

	XFontUnicode m_caption;
	XVec2 m_textSize;

	XGroupState m_state;
	XButton m_stateBotton;
public:
	enum XGroupEvent
	{
		GRP_STATE_CHANGE,
	};
	void setWithRect(bool flag){m_withRect = flag;}
	bool getWithRect()const{return m_withRect;}
private:
//	void (*m_funStateChange)(void *,int ID);	//状态发生改变时调用的函数
//	void *m_pClass;

	static void ctrlProc(void *,int,int);
	void updateData();		//更新内部数据
public:
	XBool init(const XVec2& position,
		const XRect& rect,
		const char *caption,const XFontUnicode& font,float captionSize = 1.0f);
	XBool initWithoutSkin(const XVec2& position,
		const XRect& rect,
		const char *caption) {return init(position,rect,caption,getDefaultFont(),1.0f);}
	XBool initWithoutSkin(const XVec2& position,
		const XVec2& pixelSize,
		const char *caption) 
	{
		return init(position,XRect(XVec2::zero,pixelSize),
			caption,getDefaultFont(),1.0f);
	}
	void setState(XGroupState state);
	XGroupState getState(){return m_state;}	//获取控件的状态
//	void setCallbackFun(void (* funStateChange)(void *,int),
//		void *pClass = NULL);
	void resetSize(const XVec2& size);
public:
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(const XVec2& s);

	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);

	void insertChar(const char *,int){;}			//do nothing
	void setCaptionText(const char *caption);					//设置按钮的标题的文字
protected:
	void draw();
	void drawUp()
	{
	//	if(!m_isInited ||
	//		!m_isVisible) return ;	//如果没有初始化直接退出
	//	m_stateBotton.drawUp();
	}
	void update(float)
	{
		//m_stateBotton.update(stepTime);
	}
	XBool mouseProc(const XVec2& p,XMouseState mouseState);		//do nothing
	XBool keyboardProc(int keyOrder,XKeyState keyState);		//do nothing
	XBool canGetFocus(const XVec2&){return XFalse;}	//do nothing	//事件可以穿透，如果返回XTrue则鼠标事件不能穿透
	XBool canLostFocus(const XVec2&){return XTrue;}	//do nothing
public:
	void drawBG()//描绘一个浅色的背景
	{
		XRender::drawFillRectA(m_drawRect.getLT(), m_drawRect.getSize(), m_color * 0.5f, true);
	}
	void setLostFocus() 
	{
		if (!m_isInited ||		//如果没有初始化直接退出
			!m_isActive ||		//没有激活的控件不接收控制
			!m_isVisible ||		//如果不可见直接退出
			!m_isEnable) return;		//如果无效则直接退出

		m_isBeChoose = XFalse;	//控件处于焦点状态
	}							//do nothing
	void disable()
	{
		m_isEnable = XFalse;
		m_isBeChoose = XFalse;
	}	
	void enable(){m_isEnable = XTrue;}
	void release();
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	XBool setACopy(const XGroup & temp);

	void setVisible();
	void disVisible();	//设置控件不可见

	XGroup()
		:m_isInited(XFalse)
//		,m_position(0.0f)
//		,m_scale(1.0f)
		,m_state(STATE_NORMAL)
//		,m_funStateChange(NULL)
//		,m_pClass(NULL)
		,m_withRect(true)
	{
		m_ctrlType = CTRL_OBJ_GROUP;
	}
	~XGroup(){release();}
	//virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XGroup(const XGroup &temp);
	XGroup& operator = (const XGroup& temp);
};
#if WITH_INLINE_FILE
#include "XGroup.inl"
#endif
}
#endif