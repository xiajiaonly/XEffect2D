#ifndef _JIA_XBUTTONBAR_
#define _JIA_XBUTTONBAR_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.9.6
//--------------------------------
#include "XButton.h"
namespace XE{
struct XButtonBarCore
{
	XBool isEnable;	//是否有效
	int width;
	std::string name;
	XButton *button;
	XButtonBarCore()
		:button(NULL)
	{}
	~XButtonBarCore(){}
};
class XButtonBar:public XControlBasic
{
private:
	XBool m_isInited;	//是否初始化
	float m_insertPoint;	//插入按钮的位置
	int m_barHeight;	//高度
	std::vector<XButtonBarCore> m_buttons;
public:
	XBool initWithoutSkin(int height = 32);
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);
	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(float x,float y);
	void setTextColor(const XFColor& color);	//设置字体的颜色
	XFColor getTextColor() const;	//获取控件字体的颜色
	
	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	int getIndexByName(const std::string &name);
	XBool addAButton(const std::string &name,const std::string &caption = "");
	XBool addAButton(const std::string& name,int width,const std::string &caption = "");
	//+++++++++++++++++++++++++++++++++++++++
	//下面是对按钮的相关操作的封装
	XBool setBtnEnable(const std::string &name);		//设置按钮使能
	XBool setBtnDisable(const std::string &name);	//设置按钮失效
	XBool setBtnCaption(const std::string &name,const std::string &caption);	//设置按钮的文字
	XBool setEventProc(const std::string &name,
		void (* eventProc)(void *,int,int),void *pClass = NULL);
	//下面是与icon相关的函数
	XBool setBtnNormalIcon(const std::string &name,const char * filename,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool setBtnNormalIcon(const std::string &name,const XSprite &icon);
	XBool setBtnDisableIcon(const std::string &name,const char * filename,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool setBtnDisableIcon(const std::string &name,const XSprite &icon);
	XBool removeBtnIcon(const std::string &name);	//去掉所有的icon
	XBool setBtnIconPosition(const std::string &name,float x,float y);	//设置icon的位置
	XBool setBtnIconSize(const std::string &name,float x,float y);	//设置icon的缩放大小
	XBool setBtnIconSize(const std::string &name,float x){return setBtnIconSize(name,x,x);}
	//---------------------------------------
	int getBtnID(const std::string &name);	//获取指定按钮的ID
	XBool setBtnComment(const std::string &name,const std::string &comment);
	XBool setBtnHotKey(const std::string &name,int hotKey);
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
public:
	void release();
	XButtonBar()
		:m_isInited(XFalse)
		,m_barHeight(32)
	{
		m_ctrlType = CTRL_OBJ_BTNBAR;
	}
	~XButtonBar() {release();}

	virtual XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	virtual XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	void disable();
	void enable();
	XBool mouseProc(float x,float y,XMouseState mouseState);		//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);			//返回是否触发按键动作
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);				//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(float,float){return XTrue;}	//应该是可以随时失去焦点的

private://下面为了防止错误，重载赋值操作符，复制构造函数
	XButtonBar(const XButtonBar &temp);
	XButtonBar& operator = (const XButtonBar& temp);
};
#if WITH_INLINE_FILE
#include "XButtonBar.inl"
#endif
}
#endif