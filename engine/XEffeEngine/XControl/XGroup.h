#ifndef _JIA_XGROUP_
#define _JIA_XGROUP_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XButton.h"

//目前尚未实现，但是建议添加的功能
//1、锁定：锁定之后组内的成员状态不会被改变
//2、回退：回退到上一次操作的数值，如果可能可以回退一直到最初的状态
//3、键盘操作：所有的内容都可以通过键盘操作来完成

enum _XGroupState
{
	STATE_NORMAL,		//常规状态
	STATE_MINISIZE,		//最小化状态
};
#define XGROUP_STATE_BTN_SIZE (32.0f)
//这是一个群组的空间，比较假单的一个控件
class _XGroup:public _XControlBasic
{
private:
	_XBool m_isInited;
//	_XVector2 m_position;	//控件的位置
//	_XVector2 m_size;		//大小
	_XRect m_rect;			//控件的范围
	_XRect m_drawRect;		//当前空间的范围

	_XFontUnicode m_caption;
	_XVector2 m_textSize;

	_XGroupState m_state;
	_XButton m_stateBotton;

	void (*m_funStateChange)(void *,int ID);	//状态发生改变时调用的函数
	void *m_pClass;

	friend void funXGroupStateBtn(void *pClass,int objectID);
	void updateData();		//更新内部数据
public:
	_XBool init(const _XVector2& position,
		const _XRect &rect,
		const char *caption,const _XFontUnicode &font,float captionSize = 1.0f);
	_XBool initWithoutTex(const _XVector2& position,
		const _XRect &rect,
		const char *caption) {return init(position,rect,caption,XEE::systemFont,1.0f);}
	_XBool initWithoutTex(const _XVector2& position,
		const _XVector2 &pixelSize,
		const char *caption) 
	{
		return init(position,_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			caption,XEE::systemFont,1.0f);
	}
	void setState(_XGroupState state);
	_XGroupState getState(){return m_state;}	//获取控件的状态
	void setCallbackFun(void (* funStateChange)(void *,int),
		void *pClass = NULL);
	void resetSize(const _XVector2 &size);
public:
	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
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
	void update(int stepTime)
	{
		//m_stateBotton.update(stepTime);
	}
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//do nothing
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);		//do nothing
	_XBool canGetFocus(float,float){return XFalse;}	//do nothing	//事件可以穿透，如果返回XTrue则鼠标事件不能穿透
	_XBool canLostFocus(float,float){return XTrue;}	//do nothing
public:
	void setLostFocus() {;}							//do nothing
	void disable()
	{
		m_isEnable = XFalse;
		m_isBeChoose = XFalse;
	}	
	void enable(){m_isEnable = XTrue;}
	void release();
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	_XBool setACopy(const _XGroup & temp);

	void setVisible();
	void disVisible();	//设置控件不可见

	_XGroup()
		:m_isInited(XFalse)
//		,m_position(0.0f,0.0f)
//		,m_size(1.0f,1.0f)
		,m_state(STATE_NORMAL)
		,m_funStateChange(NULL)
		,m_pClass(NULL)
	{
		m_ctrlType = CTRL_OBJ_GROUP;
	}
	~_XGroup(){release();}
	//virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	_XGroup(const _XGroup &temp);
	_XGroup& operator = (const _XGroup& temp);
};
#include "XGroup.inl"

#endif