#ifndef _JIA_XSUBWINDOW_
#define _JIA_XSUBWINDOW_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.13
//--------------------------------
#include "XButton.h"
#include "XCtrlManagerBase.h"
#include "XDragObjectBase.h"
#include "XAutoShrinkBase.h"
namespace XE{
//这是一个子窗口的类，尚未实现，只是为了建立这个概念
//说明
//1、期望能实现窗口在边界自动收缩的功能
//2、期望能实现窗口失去焦点时自动虚化的功能
//3、期望能实现窗口层次切换的功能
struct XSubWindowObj
{
	XControlBasic *obj;	//控件的指针
	XVec2 pos;
	XVec2 scale;
	//下面还需要包含其他属性(尚未添加)
};
class XSubWindow:public XControlBasic,public XDragObjectBase,public XAutoShrinkBase
{
public:
	enum XSubWindowState
	{//子窗口的状态
		STATE_NORMAL,	//普通状态
		STATE_MINISIZE,	//最小化
	};
	enum XSubWindowType
	{
		TYPE_NORMAL,	//普通窗口
		TYPE_NO_TITLE,	//无标题的窗口
		TYPE_TOP,		//最顶端的窗口
	};
	enum XSubWindowEvent
	{
		SWD_STATE_CHANGE,	//窗口状态发生改变
		SWD_LOSE_FOCUS,		//窗口失去焦点
		SWD_RELEASE,		//窗口注销
	};
private:
	XBool m_isInited;			//是否进行初始化
	XBool m_withTitle;			//窗口是否有标题
	XBool m_withBackGround;	//是否有窗口背景
	XSubWindowType m_windowType;
	XSubWindowState m_windowState;
	std::string m_titleString;	//窗口的标题
	XButton m_titleButton[3];	//窗口的三个按钮，最小化，关闭和待定

	XRect m_area;	//窗口的范围
	XFontUnicode m_titleFont;

	std::vector<XSubWindowObj> m_objects;	//物件的序列
	static void ctrlProc(void *,int,int);

	void setMiniSize();
	void setNormalSize();
	void updateMouseRect();

	XCtrlManagerBase m_ctrlManager;	//加入一个控件管理器
public:
	XSubWindow()
		:m_isInited(XFalse)
		, m_withTitle(XTrue)
		, m_titleString("SubWindow")
		, m_withBackGround(XTrue)
		, m_windowType(TYPE_NORMAL)
		, m_windowState(STATE_NORMAL)
	{
		m_ctrlType = CTRL_OBJ_SUBWINDOW;
	}
	~XSubWindow() {release();}
	void release();
	XBool initWithoutSkin(const XVec2& area,const char * title);	//无皮肤的初始化
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);		//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);			//返回是否触发按键动作
	void insertChar(const char *ch,int len){m_ctrlManager.insertChar(ch,len);}
	XBool canGetFocus(const XVec2& p);				//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&){return true;}	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点
	void setFocus();
public:
	XBool setACopy(const XSubWindow &){}	//这个不能复制
	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);
	virtual const XVec2& getPosition() const {return m_position;}

	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(const XVec2& s);

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);

	void setTitle(const char * title);
	void changeStage(XSubWindowState state);

	XBool addACtrlObj(XControlBasic *obj);	//向窗口中添加一个控件
	XBool desACtrlObj(XControlBasic* obj);	//从窗口中减少一个控件
	int getObjectIndex(XControlBasic *obj);
private:
	virtual XBool getIsInDragRect(const XVec2& p);	//判断位置是否在拖拽区域内
	virtual XVec2 correctPos(const XVec2& pos);	//更具传入的拖拽位置，修正该位置，当位置出于非法状态时，修正成合法的位置

	virtual XShrinkDirection calShrinkDir();	//计算当前可能的收缩方向
	virtual XBool getIsInShrinkRect(){return m_curMouseRect.isInRect(m_curMousePosition);}	//判断位置是否在拖拽区域内
	virtual void updateShrinkPosition();
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(尚未完成)
public:
	virtual XBool saveState(TiXmlNode &e);
	virtual XBool loadState(TiXmlNode *e);
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XSubWindow.inl"
#endif
}
#endif