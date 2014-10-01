#ifndef _JIA_XTAB_
#define _JIA_XTAB_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.11
//--------------------------------
//这个是标签控件(尚未实现)
#include "XButton.h"

#define TAB_MODE (1)

#define TAB_TITLE_HEIGHT (32)	//标题的高度
#define TAB_TITLE_DISTANCE (3)	//标题之间的间距
#define TAB_BTN_WIDTH (20)

struct _XTabObject
{
	_XBool isEnable;					//是否有效
	_XBool isVisible;					//是否可见
	std::string srcTitle;				//原始标题没有限制长度的
	std::string title;					//标题
	std::vector<_XControlBasic *> objs;	//物件
	_XFontUnicode *pFont;

	_XBool needDraw;
	int len;	//显示的长度
	int offset;	//偏移
	std::vector<_XVector2> pos;		//物件的相对位置
	std::vector<_XVector2> size;	//物件的相对尺寸
	_XTabObject()
		:isEnable(XTrue)
		,isVisible(XTrue)
		,pFont(NULL)
	{}
};

//功能
//1、设置标签可见		x	//还需要测试
//2、设置标签是否可选	x	//还需要测试
//3、超长标签			x
//4、键盘操作标签		x

class _XTab:public _XControlBasic
{
private:
//	_XRect m_mouseRect;		//范围
//	_XRect m_nowMouseRect;	//范围
//	_XVector2 m_position;	//位置
//	_XVector2 m_size;		//大小
	_XRect m_titleRect;

	_XBool m_isInited;
	_XBool m_withoutTex;	//是否拥有贴图

	_XButton m_leftBtn;
	_XButton m_rightBtn;
	_XFColor m_textColor;		//文字的颜色

	std::vector<_XTabObject> m_tabObjects;	//所有的物件

	int m_nowChooseTabIndex;	//当前选择的标签编号
	int m_nowStartTabIndex;		//当前显示的第一个标签的编号
	int m_maxTitleLen;			//允许的最长标签
	float m_nowTextWidth;		//字体的宽度
	_XFontUnicode m_texFont;

	void updateTitle();	//更新标题的显示
	void updateBtnState();	//更新按钮的状态
	void updateObjState(bool flag = true);
	friend void tabBtnProc(void*,int);

	void (*m_funChooseTab)(void *,int,const std::string &);
	void *m_pClass;				//回调函数的参数
public:
	_XBool initWithoutTex(const _XRect &rect,const _XFontUnicode &font);
	_XBool initWithoutTex(const _XRect &rect) {return initWithoutTex(rect,XEE::systemFont);}
	_XBool initWithoutTex(const _XVector2 &pixelSize) 
	{
		return initWithoutTex(_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),XEE::systemFont);
	}
	void setChooseTabCB(void (* funChooseTab)(void *,int,const std::string &),void *pClass = NULL);
protected:
	void draw();
	void drawUp();
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	
	_XBool keyboardProc(int,_XKeyState);		//返回是否触发按键动作

	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);				//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点

public:
	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);
	using _XObjectBasic::setSize;	//避免覆盖的问题
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color);	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	_XBool setTabStr(const char *str,int index);
	_XBool setTabsStr(const char *str);		//;作为分隔符
	std::string getTabStr(int index);
	std::string getTabsStr();
	int getTabIndexByTitle(const std::string &title) const;
	void addATab(const std::string &title);
	void clearAllObj();	//清除所有的从属关系
	void addObjToTab(_XControlBasic *obj,int index);	//向一个标签中添加物件
	void addObjToTab(_XControlBasic *obj,const std::string &title);	//向一个标签中添加物件
	void setTabVisible(bool flag,int index);	//设置标签可见
	void setTabVisible(bool flag,const std::string &title);	//设置标签可见
	_XBool getTabVisible(int index) const;
	_XBool getTabVisible(const std::string &title) const;
	void setTabEnable(bool flag,int index);
	void setTabEnable(bool flag,const std::string &title);
	_XBool getTabEnable(int index) const;
	_XBool getTabEnable(const std::string &title) const;
	//下面的接口尚未实现
	void delObjFromTab(_XControlBasic *obj,int index);
	void delObjFromTab(_XControlBasic *obj,const std::string &title);
	void clearObjFromTab(int index);
	void clearObjFromTab(const std::string &title);

	_XTab()
		:m_isInited(XFalse)
		,m_withoutTex(XTrue)
		,m_nowChooseTabIndex(0)
		,m_nowStartTabIndex(0)
		,m_funChooseTab(NULL)
		,m_pClass(NULL)
	{
		m_ctrlType = CTRL_OBJ_XTAB;
	}
	~_XTab(){release();}
	void release();

	void disable();
	void enable();
	void setVisible();	//设置显示
	void disVisible();	//设置不显示
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	////virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	_XTab(const _XTab &temp);
	_XTab& operator = (const _XTab& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对控件动态支持而定义的相关属性和方法
private:
	_XMoveData m_actionMoveData;	//动态效果的变量
	int m_oldChooseTabIndex;
	void startAction();
	void update(int stepTime);
	//---------------------------------------------------------
};

#include "XTab.inl"

#endif