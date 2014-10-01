#ifndef _JIA_XMOUSERIGHTBUTTONMENU_
#define _JIA_XMOUSERIGHTBUTTONMENU_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XButton.h"

//鼠标右键菜单的类，主要用于显示鼠标右键菜单
//右键菜单就是由许多的按钮组成，按纽可以使用不同的图案从而构成一个完成的整体
//这个菜单是不能通过程序来动态改变位置的，他的位置由鼠标右键确定（为了减小难度，目前暂行）
typedef _XButtonTexture _XMouseRightButtonMenuTexture;
class _XEdit;
class _XMouseRightButtonMenu:public _XControlBasic
{
	friend _XEdit;
private:
	_XBool m_isInited;
	_XButton *m_menu;		//按钮的指针
	int m_menuSum;		//按钮的数量
	int m_nowChoose;		//当前选择的值（-1什么也没有选择）//当使用键盘选择菜单时这个值才有效
	int m_lastChoose;		//最终选择的值

	void (*m_funChooseChange)(void *,int);	//改变选值时调用
	void (*m_funChooseOver)(void *,int);	//最终确定选值时调用
	void *m_pClass;

	_XRect m_allArea;		//整个空间的响应范围
	_XVector2 m_upMousePoint;	//上次记录的鼠标在范围内的位置

	_XResourceInfo *m_resInfo;
public:
	//这里的Area为单个菜单项的鼠标响应区域，默认所有菜单项都一样
	_XBool init(int menuSum,	//菜单中的物件数量
		const _XVector2& position,	//菜单的位置
		const _XRect& Area,	//菜单按键的响应范围
		const _XMouseRightButtonMenuTexture &tex,	//菜单的贴图
		const _XFontUnicode &font,float captionSize,const _XVector2& textPosition);		//菜单的字体
	_XBool initEx(int menuSum,	//对上面接口的简化
		const _XVector2& position,	
		const _XMouseRightButtonMenuTexture &tex,	
		const _XFontUnicode &font,float captionSize = 1.0f);
	_XBool initPlus(const char * path,int menuSum,	//菜单中的物件数量
		const _XFontUnicode &font,float captionSize = 1.0f,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(int menuSum,const _XRect& area,
		const _XFontUnicode &font,float captionSize, const _XVector2& textPosition);
	_XBool initWithoutTex(int menuSum,const _XRect& area,const _XVector2& textPosition)
	{
		return initWithoutTex(menuSum,area,XEE::systemFont,1.0f,textPosition);
	}
	_XBool initWithoutTex(int menuSum,const _XVector2& pixelSize,const _XVector2& textPosition)
	{
		return initWithoutTex(menuSum,_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			XEE::systemFont,1.0f,textPosition);
	}
protected:
	void draw();//描绘菜单
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);		//键盘时间的相应可以使用鼠标上下键选择menu项
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}
public:
	void setNowChoose(int){;}								//设置当前选择的菜单项
	_XBool setACopy(const _XMouseRightButtonMenu &temp);		//建立一个与目标实体共用资源的实体
public:
	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);	//设置菜单的位置

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);

	_XMouseRightButtonMenu();
	~_XMouseRightButtonMenu(){release();}
	//下面是是内联函数
	int getLastChoose() const;										//返回最终选择的值
	void setCallbackFun(void (* funChooseChange)(void *,int),
		void (* funChooseOver)(void *,int),
		void *pClass = NULL);	//设置菜单的回调函数
	void setHotKey(int hotKey,int order);							//设置菜单中某一项的热键
	void setText(const char *temp,int order);								//改变菜单中某一项的值
	void setTexture(const _XMouseRightButtonMenuTexture &tex,int order);	//改变菜单中某一项的贴图
	void release();	//释放资源
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	//下面的接口尚未实现
	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	//virtual void justForTest() {;}
private:	//下面为了防止意外调用发生的错误，而重载赋值构造函数和赋值操作符
	_XMouseRightButtonMenu(const _XMouseRightButtonMenu &temp);
	_XMouseRightButtonMenu& operator = (const _XMouseRightButtonMenu& temp);
};
#include "XMouseRightButtonMenu.inl"

#endif