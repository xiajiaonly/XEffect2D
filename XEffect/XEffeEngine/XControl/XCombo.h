#ifndef _JIA_XCOMBO_
#define _JIA_XCOMBO_
#include "XButton.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//下拉菜单的类的贴图
class _XComboTexture
{
private:
	char m_isInited;
public:
	_XTextureData *comboInputNormal;			//下拉菜单输入框的普通状态
	_XTextureData *comboInputDisable;			//无效状态下的按钮贴图
	_XButtonTexture downButtonTex;
	_XButtonTexture downMenuUpButtonTex;
	_XButtonTexture downMenuButtonTex;
	_XButtonTexture downMenuDownButtonTex;

	_XComboTexture();
	~_XComboTexture();
	int init(const char *inputNormal,const char *inputDisable,
		const char *downButtonNormal,const char *downButtonOn,const char *downButtonDown,const char *downButtonDisable,
		const char *downMenuUpNormal,const char *downMenuUpOn,const char *downMenuUpDown,const char *downMenuUpDisable,
		const char *downMenuNormal,const char *downMenuOn,const char *downMenuDown,const char *downMenuDisable,
		const char *downMenuDownNormal,const char *downMenuDownOn,const char *downMenuDownDown,const char *downMenuDownDisable,
		int resoursePosition = 0);
	void release();
};

#define COMBO_MAX_MENU_LENGTH 512
#define COMBO_LEFT_DISTANSE 10
#define COMBO_TOP_DISTANSE 5

class _XCombo:public _XControlBasic
{
private:
	char m_isInited;

	const _XTextureData *m_comboInputNormal;			//下拉菜单输入框的普通状态
	const _XTextureData *m_comboInputDisable;			//无效状态下的按钮贴图

	int m_nowMenuSum;			//当前所有的选项数量(尚未实现，尚未生效)
	int m_menuSum;				//下拉菜单中的总菜单项数量
	int m_menuDrawSum;			//下拉菜单中显示的菜单的数量
	int m_menuStartDrawOrder;	//下拉菜单中开始显示的第一个菜单项的编号
	int m_menuTextWidth;		//下拉菜单中显示的文字的文字宽度

	char m_isDrawDownMenu;		//是否显示下拉菜单
	int m_nowChooseOrder;		//当前选择的菜单项的编号

	_XSprite m_sprite;			//用于显示贴图的精灵
	_XFontUnicode m_caption;	//文字的字体
	_XVector2 m_textSize;		//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	_XFColor m_textColor;		//文字的颜色
		
	char *m_menuData;			//下来菜单的具体内容
	_XButton *m_buttom;			//下拉菜单中的按钮，3个按钮为功能按钮，其他为菜单按钮

	void updateString();				//跟新字符串到具体的菜单内容

	void (*m_funDataChange)(void *,int ID);
	void *m_pClass;				//回调函数的参数

	friend void funComboMenuState(void *pClass,int ID);
	friend void funComboMenuStart(void *pClass,int ID);
	friend void funComboMenuEnd(void *pClass,int ID);
	friend void funComboMenu(void *pClass,int ID);

	_XRect m_inputArea;			//输入框的响应范围
	_XRect m_downButtonArea;	//下拉按钮的响应范围
	_XRect m_downMenuUpArea;		//上翻页的响应范围
	_XRect m_downMenuArea;		//选择项的响应范围
	_XRect m_downMenuDownArea;	//下翻页的响应范围
	_XRect m_allArea;			//整个空间的响应范围
public:
	int init(const _XComboTexture &tex,	//控件的贴图
		const _XVector2& position,		//控件的位置
		const _XRect& inputArea,		//输入框的有效范围
		const _XRect& downButtonArea,	//下拉按钮的响应范围
		const _XRect& downMenuUpArea,	//上翻页的响应范围
		const _XRect& downMenuArea,		//选择项的响应范围
		const _XRect& downMenuDownArea,	//下翻页的响应范围
		int menuSum,					//下拉菜单中的选项的数量
		int drawMenuSum,				//下拉菜单中显示的菜单项的数量
		const _XFontUnicode &font,		//显示文字使用的字体
		float fontSize);				//字体的大小
protected:			
	void draw();
	void drawUp();
	int mouseProc(float x,float y,_XMouseState mouseState);	//对于鼠标动作的响应函数
	int keyboardProc(int keyOrder,_XKeyState keyState){return 1;};	//do nothing
	int canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	int canLostFocus(float x,float y);
public:
	int setMenuStr(const char *str,int order);			//设置某个下拉菜单的文字
	void setShowMenuSum(int sum);	//动态改变显示的选项数量(尚未实现)

	void setSize(const _XVector2& size);			//设置控件的尺寸
	void setSize(float x,float y);			//设置控件的尺寸
	_XVector2 getSize() const {return m_size;}
	void setPosition(const _XVector2& position);	//设置空间的位置
	void setPosition(float x,float y);		//设置空间的位置
	_XVector2 getPosition() const {return m_position;}
	void setTextColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
		for(int i = 0;i < m_menuDrawSum + 3;++ i)
		{
			m_buttom[i].setTextColor(m_textColor);
		}
	}	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色
	void setColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_color = color;
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		for(int i = 0;i < m_menuDrawSum + 3;++ i)
		{
			m_buttom[i].setColor(m_color);
		}
	}	//设置按钮的颜色
	_XFColor getColor() const {return m_color;}	//获取控件的颜色
	//副本拥有自己的行为,但是与母体使用同样的资源,如果母体资源被改变,副本的资源也会相应的被改变
	int setACopy(const _XCombo &temp);	//建立一个副本
	void release();						//释放资源
	_XCombo();
	~_XCombo();

	//下面是内联函数
	void setCallbackFun(void (* funDataChange)(void *,int),void *pClass);
	void disable();					//使无效
	void enable();					//使能
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	int isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XCombo(const _XCombo &temp);
	_XCombo& operator = (const _XCombo& temp);
};

inline void _XCombo::setCallbackFun(void (* funDataChange)(void *,int),void *pClass)
{
	if(funDataChange != NULL) m_funDataChange = funDataChange;
	m_pClass = pClass;
}

inline void _XCombo::disable()					//使无效
{
	m_isEnable = 0;
}

inline void _XCombo::enable()					//使能
{
	m_isEnable = 1;
}

inline void _XCombo::setSize(const _XVector2& size)			//设置控件的尺寸
{
	setSize(size.x,size.y);
}

inline void _XCombo::setPosition(const _XVector2& position)	//设置空间的位置
{
	setPosition(position.x,position.y);
}
#endif