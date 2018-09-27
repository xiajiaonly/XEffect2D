#ifndef _JIA_XTAB_
#define _JIA_XTAB_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.11
//--------------------------------
//这个是标签控件(尚未实现)
#include "XButton.h"
#include "XCtrlManagerBase.h"
namespace XE{
#define TAB_TITLE_HEIGHT (MIN_FONT_CTRL_SIZE)	//标题的高度

class XTabObject
{
public:
	XBool isEnable;					//是否有效
	XBool isVisible;					//是否可见
	std::string srcTitle;				//原始标题没有限制长度的
	std::string title;					//标题
	std::vector<XControlBasic *> objs;	//物件
	XFontUnicode *pFont;

	XBool needDraw;
	int len;	//显示的长度
	int offset;	//偏移
	std::vector<XVec2> pos;		//物件的相对位置
	std::vector<XVec2> scale;	//物件的相对尺寸
	XTabObject()
		:isEnable(XTrue)
		,isVisible(XTrue)
		,pFont(NULL)
	{}
	virtual ~XTabObject(){}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(完成)
public:
	virtual XBool saveState(TiXmlNode &e);
	virtual XBool loadState(TiXmlNode *e);
	//---------------------------------------------------------
};

//功能
//1、设置标签可见		x	//还需要测试
//2、设置标签是否可选	x	//还需要测试
//3、超长标签			x
//4、键盘操作标签		x

class XTab:public XControlBasic
{
private:
	static const int m_tabTitleDistance = 3;	//标题之间的间距//使用这种方式代替宏
	static const int m_tabBtnWidth = 20;
//	XRect m_mouseRect;		//范围
//	XRect m_curMouseRect;	//范围
//	XVec2 m_position;	//位置
//	XVec2 m_scale;		//大小
	XRect m_titleRect;

	XBool m_isInited;
	XBool m_withoutTex;	//是否拥有贴图

	XButton m_leftBtn;
	XButton m_rightBtn;
	XFColor m_textColor;		//文字的颜色

	std::vector<XTabObject> m_tabObjects;	//所有的物件

	int m_curChooseTabIndex;	//当前选择的标签编号
	int m_curStartTabIndex;		//当前显示的第一个标签的编号
	int m_maxTitleLen;			//允许的最长标签
	float m_curTextWidth;		//字体的宽度
	XFontUnicode m_texFont;

	void updateTitle();	//更新标题的显示
	void updateBtnState();	//更新按钮的状态
	void updateObjState(bool flag = true);
	static void ctrlProc(void*,int,int);

	//std::string m_curChooseTabStr;
public:
	enum XTabEvent
	{
		TAB_CHOOSE,
	};
	const std::string& getCurChooseTabStr()const {return m_tabObjects[m_curChooseTabIndex].srcTitle;}
private:
//	void (*m_funChooseTab)(void *,int,const std::string& );
//	void *m_pClass;				//回调函数的参数
	void(*m_funUpDraw)(void *);
	void* m_pClass;				//回调函数的参数
public:
	XBool initWithoutSkin(const XRect& rect,const XFontUnicode& font);
	XBool initWithoutSkin(const XRect& rect) {return initWithoutSkin(rect,getDefaultFont());}
	XBool initWithoutSkin(const XVec2& pixelSize) 
	{
		return initWithoutSkin(XRect(XVec2::zero,pixelSize),getDefaultFont());
	}
//	void setChooseTabCB(void (* funChooseTab)(void *,int,const std::string& ),void *pClass = NULL);
	void setUpDrawCB(void(*funUpDraw)(void *), void *pClass = NULL);
protected:
	void draw();
	void drawUp();
	XBool mouseProc(const XVec2& p,XMouseState mouseState);
	XBool keyboardProc(int,XKeyState);		//返回是否触发按键动作
	void insertChar(const char *ch,int len){m_ctrlManager.insertChar(ch,len);}
	XBool canGetFocus(const XVec2& p);				//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&){return XTrue;}	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点

public:
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);
	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color);	//设置字体的颜色
	const XFColor& getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);

	XBool setTabStr(const char *str,unsigned int index);
	XBool setTabsStr(const char *str);		//;作为分隔符
	const std::string& getTabStr(unsigned int index)const;
	std::string getTabsStr();
	int getTabIndexByTitle(const std::string& title) const;
	void addATab(const std::string& title);
	void addATabs(const std::string& title);
	void delATab(const std::string& title);
	void clearAllObj();	//清除所有的从属关系
	void addObjToTab(XControlBasic *obj,unsigned int index);	//向一个标签中添加物件
	void addObjToTab(XControlBasic *obj,const std::string& title);	//向一个标签中添加物件
	void setTabVisible(bool flag,int index);	//设置标签可见
	void setTabVisible(bool flag,const std::string& title);	//设置标签可见
	XBool getTabVisible(unsigned int index) const;
	XBool getTabVisible(const std::string& title) const;
	void setTabEnable(bool flag,unsigned int index);
	void setTabEnable(bool flag,const std::string& title);
	XBool getTabEnable(unsigned int index) const;
	XBool getTabEnable(const std::string& title) const;

	void delObjFromTab(XControlBasic *obj,int index);
	void delObjFromTab(XControlBasic *obj,const std::string& title);
	//下面的接口尚未实现
	void clearObjFromTab(int index);
	void clearObjFromTab(const std::string& title);

	XTab()
		:m_isInited(XFalse)
		, m_withoutTex(XTrue)
		, m_curChooseTabIndex(0)
		, m_curStartTabIndex(0)
//		,m_funChooseTab(NULL)
//		,m_pClass(NULL)
		, m_funUpDraw(nullptr)
		, m_pClass(nullptr)
	{
		m_ctrlType = CTRL_OBJ_TAB;
	}
	~XTab(){release();}
	void release();

	void disable();
	void enable();
	void setVisible();	//设置显示
	void disVisible();	//设置不显示
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	////virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XTab(const XTab &temp);
	XTab& operator = (const XTab& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对控件动态支持而定义的相关属性和方法
private:
	XMoveData m_actionMoveData;	//动态效果的变量
	int m_oldChooseTabIndex;
	void startAction();
	void update(float stepTime);
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(完成)
public:
	virtual XBool saveState(TiXmlNode &e);
	virtual XBool loadState(TiXmlNode *e);
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//尝试使用控件管理器的方式来设计tab
	XCtrlManagerBase m_ctrlManager;	//加入一个控件管理器
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XTab.inl"
#endif
}
#endif