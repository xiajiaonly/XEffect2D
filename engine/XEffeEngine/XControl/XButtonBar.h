#ifndef _JIA_XBUTTONBAR_
#define _JIA_XBUTTONBAR_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.9.6
//--------------------------------
#include "XButton.h"

struct _XButtonBarCore
{
	_XBool isEnable;	//是否有效
	int width;
	std::string name;
	_XButton *button;
	_XButtonBarCore()
		:button(NULL)
	{}
	~_XButtonBarCore(){}
};
class _XButtonBar:public _XControlBasic
{
private:
	_XBool m_isInited;	//是否初始化
	float m_insertPoint;	//插入按钮的位置
	int m_barHeight;	//高度
	std::vector<_XButtonBarCore> m_buttons;
public:
	_XBool initWithoutTex(int height = 32);
	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);
	using _XObjectBasic::setSize;	//避免覆盖的问题
	void setSize(float x,float y);
	void setTextColor(const _XFColor& color);	//设置字体的颜色
	_XFColor getTextColor() const;	//获取控件字体的颜色
	
	using _XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	int getIndexByName(const std::string &name);
	_XBool addAButton(const std::string &name,const std::string &caption = "");
	_XBool addAButton(const std::string& name,int width,const std::string &caption = "");
	//+++++++++++++++++++++++++++++++++++++++
	//下面是对按钮的相关操作的封装
	_XBool setBtnEnable(const std::string &name);		//设置按钮使能
	_XBool setBtnDisable(const std::string &name);	//设置按钮失效
	_XBool setBtnCaption(const std::string &name,const std::string &caption);	//设置按钮的文字
	_XBool setBtnCallbackFun(const std::string &name,
		void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funMouseOn)(void *,int),
		void (* funMouseDown)(void *,int),
		void (* funMouseUp)(void *,int),
		void *pClass = NULL);	//设置按钮的回调函数
	_XBool setBtnMouseDownCB(const std::string &name,
		void (* funMouseDown)(void *,int),
		void *pClass = NULL);	//设置按钮被按下时的回调函数
	//下面是与icon相关的函数
	_XBool setBtnNormalIcon(const std::string &name,const char * filename,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool setBtnNormalIcon(const std::string &name,const _XSprite &icon);
	_XBool setBtnDisableIcon(const std::string &name,const char * filename,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool setBtnDisableIcon(const std::string &name,const _XSprite &icon);
	_XBool removeBtnIcon(const std::string &name);	//去掉所有的icon
	_XBool setBtnIconPosition(const std::string &name,float x,float y);	//设置icon的位置
	_XBool setBtnIconSize(const std::string &name,float x,float y);	//设置icon的缩放大小
	_XBool setBtnIconSize(const std::string &name,float x){return setBtnIconSize(name,x,x);}
	//---------------------------------------
	int getBtnID(const std::string &name);	//获取指定按钮的ID
	_XBool setBtnComment(const std::string &name,const std::string &comment);
	_XBool setBtnHotKey(const std::string &name,int hotKey);
protected:
	void draw();
	void drawUp();
	void update(int stepTime);
public:
	void release();
	_XButtonBar()
		:m_isInited(XFalse)
		,m_barHeight(32)
	{
		m_ctrlType = CTRL_OBJ_BTNBAR;
	}
	~_XButtonBar() {release();}

	virtual _XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	virtual _XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	void disable();
	void enable();
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);			//返回是否触发按键动作
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);				//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}	//应该是可以随时失去焦点的

private://下面为了防止错误，重载赋值操作符，复制构造函数
	_XButtonBar(const _XButtonBar &temp);
	_XButtonBar& operator = (const _XButtonBar& temp);
};
#include "XButtonBar.inl"
#endif