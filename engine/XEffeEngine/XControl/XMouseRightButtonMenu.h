#ifndef _JIA_XMOUSERIGHTBUTTONMENU_
#define _JIA_XMOUSERIGHTBUTTONMENU_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XButton.h"
namespace XE{
//鼠标右键菜单的类，主要用于显示鼠标右键菜单
//右键菜单就是由许多的按钮组成，按纽可以使用不同的图案从而构成一个完成的整体
//这个菜单是不能通过程序来动态改变位置的，他的位置由鼠标右键确定（为了减小难度，目前暂行）
typedef XButtonSkin XMouseRightButtonMenuSkin;
class XEdit;
class XMouseRightButtonMenu:public XControlBasic
{
	friend XEdit;
private:
	XBool m_isInited;
	XButton *m_menu;		//按钮的指针
	int m_menuSum;		//按钮的数量
	int m_curChoose;		//当前选择的值（-1什么也没有选择）//当使用键盘选择菜单时这个值才有效
	int m_lastChoose;		//最终选择的值
public:
	enum XMouseRightButtonMenuEvent
	{
		MRBM_CHOOSE_CHANGE,
		MRBM_CHOOSE_OVER,
	};
//	void (*m_funChooseChange)(void *,int);	//改变选值时调用
//	void (*m_funChooseOver)(void *,int);	//最终确定选值时调用
//	void *m_pClass;
private:
	XRect m_allArea;		//整个空间的响应范围
	XVec2 m_upMousePoint;	//上次记录的鼠标在范围内的位置

	XResourceInfo *m_resInfo;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了实现控件动态而定义的变量
	bool m_isDraw;			//是否是菜单出现
	XMoveData m_actionMoveData;
	void playOutAction();	//播放退出的动画
	//-----------------------------------------------------
public:
	//这里的Area为单个菜单项的鼠标响应区域，默认所有菜单项都一样
	XBool init(int menuSum,	//菜单中的物件数量
		const XVec2& position,	//菜单的位置
		const XRect& Area,	//菜单按键的响应范围
		const XMouseRightButtonMenuSkin &tex,	//菜单的贴图
		const XFontUnicode& font,float captionSize,const XVec2& textPosition);		//菜单的字体
	XBool initEx(int menuSum,	//对上面接口的简化
		const XVec2& position,	
		const XMouseRightButtonMenuSkin &tex,	
		const XFontUnicode& font,float captionSize = 1.0f);
	XBool initPlus(const char * path,int menuSum,	//菜单中的物件数量
		const XFontUnicode& font,float captionSize = 1.0f,
		XResPos resPos = RES_SYS_DEF);
	XBool initWithoutSkin(int menuSum,const XRect& area,
		const XFontUnicode& font,float captionSize, const XVec2& textPosition);
	XBool initWithoutSkin(int menuSum,const XRect& area,const XVec2& textPosition)
	{
		return initWithoutSkin(menuSum,area,getDefaultFont(),1.0f,textPosition);
	}
	XBool initWithoutSkin(int menuSum,const XVec2& pixelSize,const XVec2& textPosition)
	{
		return initWithoutSkin(menuSum,XRect(XVec2::zero,pixelSize),
			getDefaultFont(),1.0f,textPosition);
	}
protected:
	void draw();//描绘菜单
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);	//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);		//键盘时间的相应可以使用鼠标上下键选择menu项
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&){return XTrue;}
public:
	void setCurChoose(int){;}								//设置当前选择的菜单项
	XBool setACopy(const XMouseRightButtonMenu &temp);		//建立一个与目标实体共用资源的实体
	void setMnuState(bool state,int index)//设置按键状态
	{
		if(index < 0 || index >= m_menuSum) return;
		if(state) m_menu[index].enable();
		else m_menu[index].disable();
	}
public:
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);	//设置菜单的位置

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(const XVec2& s);

	XMouseRightButtonMenu();
	~XMouseRightButtonMenu(){release();}
	//下面是是内联函数
	int getLastChoose() const;										//返回最终选择的值
	void setCallbackFun(void (* funChooseChange)(void *,int),
		void (* funChooseOver)(void *,int),
		void *pClass = NULL);	//设置菜单的回调函数
	void setHotKey(int hotKey,int order);							//设置菜单中某一项的热键
	void setText(const char *temp,int order);								//改变菜单中某一项的值
	void setTexture(const XMouseRightButtonMenuSkin &tex,int order);	//改变菜单中某一项的贴图
	void release();	//释放资源
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	//下面的接口尚未实现
	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);

	//virtual void justForTest() {;}
private:	//下面为了防止意外调用发生的错误，而重载赋值构造函数和赋值操作符
	XMouseRightButtonMenu(const XMouseRightButtonMenu &temp);
	XMouseRightButtonMenu& operator = (const XMouseRightButtonMenu& temp);
};
#if WITH_INLINE_FILE
#include "XMouseRightButtonMenu.inl"
#endif
}
#endif