#ifndef _JIA_XGROUP_
#define _JIA_XGROUP_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XControlBasic.h"
#include "../XFontUnicode.h"
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
	_XVector2 m_position;	//控件的位置
	_XVector2 m_size;		//大小
	_XRect m_rect;			//控件的范围
	_XRect m_drawRect;		//当前空间的范围

	_XFontUnicode m_caption;
	_XVector2 m_textSize;

	_XGroupState m_state;
	_XButton m_stateBotton;

	void (*m_funStateChange)(void *,int ID);	//状态发生改变时调用的函数
	void *m_pClass;

	friend void funXGroupStateBtn(void *pClass,int objectID);
	void updateData()		//更新内部数据
	{
		if(m_state == STATE_NORMAL)
		{
			m_caption.setPosition(m_position.x + (m_rect.left + XGROUP_STATE_BTN_SIZE + 5.0f) * m_size.x,m_position.y + m_rect.top * m_size.y);
		//	m_stateBotton.setPosition(m_position.x + (m_rect.right - XGROUP_STATE_BTN_SIZE) * m_size.x,m_position.y + m_rect.top * m_size.y);
			m_stateBotton.setPosition(m_position.x,m_position.y + m_rect.top * m_size.y);
			m_drawRect.set(m_rect.left * m_size.x + m_position.x,m_rect.top * m_size.y + m_position.y,
				m_rect.right * m_size.x + m_position.x,m_rect.bottom * m_size.y + m_position.y);
		}else
		{
			m_caption.setPosition(m_position.x + (XGROUP_STATE_BTN_SIZE + 3.0f) * m_size.x,m_position.y + 3.0f * m_size.y);
		//	m_stateBotton.setPosition(m_position.x + 3.0f * m_size.x + m_caption.getMaxPixelWidth(),m_position.y + 3.0f * m_size.y);
			m_stateBotton.setPosition(m_position.x,m_position.y + 3.0f * m_size.y);
			m_drawRect.left = m_rect.left * m_size.x + m_position.x;
			m_drawRect.top = m_rect.top * m_size.y + m_position.y;
			m_drawRect.right = m_drawRect.left + (6.0f + XGROUP_STATE_BTN_SIZE) * m_size.x + m_caption.getMaxPixelWidth();
			m_drawRect.bottom = m_drawRect.top + (6.0f + XGROUP_STATE_BTN_SIZE) * m_size.x;
		}
	}
public:
	_XBool init(const _XVector2& position,
		const _XRect &rect,
		const char *caption,const _XFontUnicode &font,float captionSize);
	void setState(_XGroupState state)
	{
		if(m_state == state) return;
		m_state = state;
		if(m_state == STATE_NORMAL)
		{
			m_stateBotton.setCaptionText("-");
			//隐藏所有子物体
			for(int i = 0;i < m_childList.size();++ i)
			{
				m_childList[i]->setVisiable();
			}
		}else
		{
			m_stateBotton.setCaptionText("+");
			//显示所有子物体
			for(int i = 0;i < m_childList.size();++ i)
			{
				m_childList[i]->disVisiable();
			}
		}
		updateData();
		if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
	}
	_XGroupState getState(){return m_state;}	//获取控件的状态
	void setCallbackFun(void (* funStateChange)(void *,int),
		void *pClass = NULL)
	{
		m_funStateChange = funStateChange;
		m_pClass = pClass;
	}
	void resetSize(_XVector2 size)
	{
		m_rect.right = size.x;
		m_rect.bottom = size.y;
		updateData();
	}
public:
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
		m_caption.setSize(m_textSize * m_size);
		m_stateBotton.setSize(m_size);
		updateData();
		updateChildSize();
	}

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a){updateChildColor();}	//do nothing
	void setAlpha(float a){updateChildAlpha();}							//do nothing

	void insertChar(const char *ch,int len){;}			//do nothing
	void setCaptionText(const char *caption)						//设置按钮的标题的文字
	{
		if(caption == NULL) return;
		m_caption.setString(caption);
	}
protected:
	void draw();
	void drawUp(){;}
	_XBool mouseProc(float x,float y,_XMouseState mouseState)
	{
	//	if(!m_isInited) return XTrue;	//如果没有初始化直接退出
	//	if(!m_isVisiable) return XTrue;	//如果不可见直接退出
	//	m_stateBotton.mouseProc(x,y,mouseState);
		return XTrue;
	}		//do nothing
	_XBool keyboardProc(int keyOrder,_XKeyState keyState)
	{
	//	if(!m_isInited) return XTrue;	//如果没有初始化直接退出
	//	if(!m_isVisiable) return XTrue;	//如果不可见直接退出
	//	m_stateBotton.keyboardProc(keyOrder,keyState);
		return XTrue;
	}		//do nothing
	_XBool canGetFocus(float x,float y){return XFalse;}	//do nothing	//事件可以穿透，如果返回XTrue则鼠标事件不能穿透
	_XBool canLostFocus(float x,float y){return XTrue;}	//do nothing
public:
	void setLostFocus() {;}							//do nothing
	void disable(){m_isEnable = XFalse;}	
	void enable(){m_isEnable = XTrue;}
	void release();
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	_XBool setACopy(const _XGroup & temp);

	void setVisiable()
	{
		m_isVisiable = XTrue;
		m_stateBotton.setVisiable();
		if(m_state == STATE_NORMAL)
		{//隐藏所有子物体
			for(int i = 0;i < m_childList.size();++ i)
			{
				m_childList[i]->setVisiable();
			}
		}else
		{//显示所有子物体
			for(int i = 0;i < m_childList.size();++ i)
			{
				m_childList[i]->disVisiable();
			}
		}
		//updateChildVisiable();
	}
	void disVisiable()
	{
		m_isVisiable = XFalse;
		m_stateBotton.disVisiable();
		updateChildVisiable();
	}	//设置控件不可见

	_XGroup()
		:m_isInited(XFalse)
		,m_position(0.0f,0.0f)
		,m_size(1.0f,1.0f)
		,m_state(STATE_NORMAL)
		,m_funStateChange(NULL)
		,m_pClass(NULL)
	{
	}
	~_XGroup()
	{
		release();
	}
	virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	_XGroup(const _XGroup &temp);
	_XGroup& operator = (const _XGroup& temp);
};

#endif