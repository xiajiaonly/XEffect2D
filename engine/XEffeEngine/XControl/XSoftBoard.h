#ifndef _JIA_XSOFTBOARD_
#define _JIA_XSOFTBOARD_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
#include "XButton.h"
#include "XAutoShrinkBase.h"
#include "XDragObjectBase.h"
#include "XControlManager.h"
namespace XE{
enum XSoftBoardType
{//软键盘的类型
	TYPE_NORMAL,
	TYPE_MINI,
};
class XSoftBoard:public XControlBasic,public XDragObjectBase,public XAutoShrinkBase
{
public:
	static const int m_ctrlGap = 4;	//控件之间的间隙
	static const int m_numberBtnSum = 10;
	static const int m_letterBtnSum = 26;
	static const int m_symbolBtnSum = 11;
	static const int m_specialBtnSum = 28;
	static const std::string m_numberBtnStrs[m_numberBtnSum];	
	static const std::string m_numberBtnUpStrs[m_numberBtnSum];	
	static const std::string m_letterBtnStrs[m_letterBtnSum];	
	static const std::string m_letterBtnUpStrs[m_letterBtnSum];	
	static const std::string m_symbolBtnStrs[m_symbolBtnSum];	
	static const std::string m_symbolBtnUpStrs[m_symbolBtnSum];	
	static const std::string m_specialBtnStrs[m_specialBtnSum];	
private:
	bool m_isInited;
	bool m_isShiftDown;		//是否shift按下
	bool m_isCapsDown;		//是否caps按下
	XSoftBoardType m_softBoardType;

	XE::XButton m_numberBtn[m_numberBtnSum];	
	XE::XButton m_letterBtn[m_letterBtnSum];	
	XE::XButton m_symbolBtn[m_symbolBtnSum];	
	XE::XButton m_specialBtn[m_specialBtnSum];	

	static void btnEventProc(void *,int,int);	//按键状态的按钮
	void pressCapsBtn();	//caps按键被按下
	void pressShiftBtn();	//shift按键被按下

	//自己管理自己的控件
	XCtrlManagerBase m_ctrlManager;	//加入一个控件管理器
	void addACtrl(XControlBasic *obj)
	{
		XCtrlManager.decreaseAObject(obj);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(obj);
#endif
		m_ctrlManager.addACtrl(obj);
	}
	void resetKeyState();//退出时需要还原那些已经按下的按键
public:
	void setVisible()
	{
		pressShiftBtn();
		m_isVisible = XTrue;
		m_comment.setVisible();
		updateChildVisible();
	}
	void disVisible()
	{
		m_isVisible = XFalse;
		m_isBeChoose = XFalse;
		m_comment.disVisible();
		updateChildVisible();
		resetKeyState();
	}
	bool initWithoutSkin();
protected:
	void draw();
	void drawUp(){}
	void update(float stepTime)
	{
		updateShrinkState(stepTime);
		m_ctrlManager.update(stepTime);
		if(m_isInited && m_isVisible && ((XEE::getCapsLockState() && !m_isCapsDown) || (!XEE::getCapsLockState() && m_isCapsDown)))	//XEE::getCapsLockState() != m_isCapsDown
			pressCapsBtn();
	}
	XBool mouseProc(float x,float y,XMouseState mouseState)		//对于鼠标动作的响应函数
	{
		if(!m_isInited ||	//如果没有初始化直接退出
			!m_isActive ||		//没有激活的控件不接收控制
			!m_isVisible) return XFalse; 	//如果不可见直接退出
		//if(m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
		m_curMousePosition.set(x,y);
		mouseEventProc(x,y,mouseState);
		m_ctrlManager.mouseProc(x,y,mouseState);
		return isInRect(x,y);
		//return XFalse;
	}
	XBool keyboardProc(int keyOrder,XKeyState keyState)			//返回是否触发按键动作
	{
		return XFalse;
	//	if(!m_isInited ||	//如果没有初始化直接退出
	//		!m_isActive ||		//没有激活的控件不接收控制
	//		!m_isVisible) return XFalse; 	//如果不可见直接退出
	//	if(m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	//	m_ctrlManager.keyProc(keyOrder,keyState);
	//	return XFalse;
	}
	void insertChar(const char *ch,int len){m_ctrlManager.insertChar(ch,len);}
	XBool canGetFocus(float x,float y)				//用于判断当前物件是否可以获得焦点
	{
		if(!m_isInited ||		//如果没有初始化直接退出
			!m_isActive ||		//没有激活的控件不接收控制
			!m_isVisible ||		//如果不可见直接退出
			!m_isEnable) return XFalse;		//如果无效则直接退出
		return m_curMouseRect.isInRect(x,y);
	}
	XBool canLostFocus(float,float){return true;}	//应该是可以随时失去焦点的
public:
	void setSoftBoardType(XSoftBoardType type);	//设置键盘的类型
	XBool setACopy(const XSubWindow &){}	//这个不能复制
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
	void setPosition(float x,float y);
	XVector2 getPosition()const{return m_position;}

	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(float x,float y);

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a){m_color.setColor(r,g,b,a);}
	void setAlpha(float a){m_color.setA(a);}
private:
	virtual XBool getIsInDragRect(float x,float y){return m_curMouseRect.isInRect(x,y);}	//判断位置是否在拖拽区域内
	virtual XVector2 correctPos(const XVector2 &pos)	//更具传入的拖拽位置，修正该位置，当位置出于非法状态时，修正成合法的位置
	{
		XVector2 ret = pos;
		if(pos.x < 0.0f) ret.x = 0.0f;
		if(pos.x > getSceneWidth() - m_curMouseRect.getWidth()) ret.x = getSceneWidth() - m_curMouseRect.getWidth();
		if(pos.y < 0.0f) ret.y = 0.0f;
		if(pos.y > getSceneHeight() - m_curMouseRect.getHeight()) ret.y = getSceneHeight() - m_curMouseRect.getHeight();
		return ret;
	}
	virtual XShrinkDirection calShrinkDir()	//计算当前可能的收缩方向
	{
		if(m_isInShrink) return DIR_NULL;	//如果已处于收缩状态，则不能再进行收缩
		//这里需要根据窗的位置计算窗口可能的搜索方向
		//if(m_position.x < 2.0f) return DIR_LEFT;
		//1、鼠标靠边
		//2、控件靠边
		if(m_position.x < 5.0f) return DIR_LEFT;
		if(m_position.x > getSceneWidth() - m_curMouseRect.getWidth() - 5.0f) return DIR_RIGHT;
		if(m_position.y < 5.0f) return DIR_UP;
		if(m_position.y > getSceneHeight() - m_curMouseRect.getHeight() - 5.0f) return DIR_DOWN;
		return DIR_NULL;
	}
	virtual XBool getIsInShrinkRect(){return m_curMouseRect.isInRect(m_curMousePosition);}	//判断位置是否在拖拽区域内
	virtual void updateShrinkPosition()
	{
		switch(m_curShrinkDir)
		{
		case DIR_UP:		//向上收缩
			setPosition(m_position.x,(5.0f - m_curMouseRect.getHeight()) * m_shrinkRate);
			break;
		case DIR_LEFT:		//向左收缩
			setPosition((5.0f - m_curMouseRect.getWidth()) * m_shrinkRate,m_position.y);
			break;
		case DIR_RIGHT:		//向右收缩
			setPosition(getSceneWidth() - 5.0f * m_shrinkRate - m_curMouseRect.getWidth() * (1.0f - m_shrinkRate),m_position.y);
			break;
		case DIR_DOWN:		//向下收缩
			setPosition(m_position.x,getSceneHeight() - 5.0f * m_shrinkRate - m_curMouseRect.getHeight() * (1.0f - m_shrinkRate));
			break;
		}
	}
public:
	XSoftBoard()
		:m_isInited(false)
		,m_isShiftDown(false)
		,m_isCapsDown(false)
		,m_softBoardType(TYPE_MINI)
	{
		m_ctrlType = CTRL_OBJ_SOFTBOARD;
		m_ctrlSpecialType = CTRL_TYPE_MOUSE;
		m_withAutoShrink = true;
	}
	virtual ~XSoftBoard(){release();}
	void release()
	{
		resetKeyState();
		XCtrlManager.decreaseAObject(this);	//注销这个物件
	#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(this);
	#endif
		m_ctrlManager.clearAllObject();	//这里需要清空所有的物件
	}
};
}

#endif