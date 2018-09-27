#ifndef _JIA_XCOMBO_
#define _JIA_XCOMBO_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XButton.h"
#include "../XXml.h"
namespace XE{
//下拉菜单的类的贴图
class XComboSkin
{
private:
	XBool m_isInited;
	void releaseTex();
public:
	XTextureData *comboInputNormal;			//下拉菜单输入框的普通状态
	XTextureData *comboInputDisable;			//无效状态下的按钮贴图
	XButtonSkin downButtonTex;
	XButtonSkin downMenuUpButtonTex;
	XButtonSkin downMenuButtonTex;
	XButtonSkin downMenuDownButtonTex;

	XRect m_mouseRect;

	XComboSkin();
	~XComboSkin() {release();}
	XBool init(const char *inputNormal,const char *inputDisable,
		const char *downButtonNormal,const char *downButtonOn,const char *downButtonDown,const char *downButtonDisable,
		const char *downMenuUpNormal,const char *downMenuUpOn,const char *downMenuUpDown,const char *downMenuUpDisable,
		const char *downMenuNormal,const char *downMenuOn,const char *downMenuDown,const char *downMenuDisable,
		const char *downMenuDownNormal,const char *downMenuDownOn,const char *downMenuDownDown,const char *downMenuDownDisable,
		XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
private:
	bool loadFromFolder(const char *filename,XResPos resPos);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename,XResPos resPos);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename,XResPos resPos);		//从网页中读取资源
};

#define DEFAULT_COMBO_BT_SIZE (MIN_FONT_CTRL_SIZE)
#define DEFAULT_COMBO_MN_HEIGHT (MIN_FONT_CTRL_SIZE)
#define DEFAULT_COMBO_UD_HEIGHT (MIN_FONT_CTRL_SIZE * 0.75f)

class XPropertyLine;
class XCombo:public XControlBasic
{
	friend XPropertyLine;
private:
	//static const int m_comboMaxMenuLength = 4096;	//这里需要优化掉
	static const int m_comboLeftDistance = 5;
	static const int m_comboTopDistance = 0;
	//static const int m_defaultComboBtSize = MIN_FONT_CTRL_SIZE;
	//static const int m_defaultComboMNHeight = MIN_FONT_CTRL_SIZE;
	XBool m_isInited;
	XBool m_isDrawDownMenu;		//是否显示下拉菜单
	XBool m_withoutTex;	//没有贴图的形式
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了实现控件动态而定义的变量
	bool m_isDraw;	//是否是菜单出现
	XMoveData m_actionMoveData;
	//-----------------------------------------------------

	const XTextureData *m_comboInputNormal;			//下拉菜单输入框的普通状态
	const XTextureData *m_comboInputDisable;			//无效状态下的按钮贴图

	int m_curMenuSum;			//当前所有的选项数量(尚未实现，尚未生效)
	int m_menuSum;				//下拉菜单中的总菜单项数量
	int m_menuDrawSum;			//下拉菜单中显示的菜单的数量
	int m_menuStartDrawOrder;	//下拉菜单中开始显示的第一个菜单项的编号
	int m_menuTextWidth;		//下拉菜单中显示的文字的文字宽度
	int m_captionTextWidth;		//标题中显示的字符串的宽度

	int m_curChooseOrder;		//当前选择的菜单项的编号

	XSprite m_sprite;			//用于显示贴图的精灵
	XFontUnicode m_caption;	//文字的字体
	XVec2 m_textSize;		//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	XFColor m_textColor;		//文字的颜色
		
	std::string m_menuData;
	//char *m_menuData;			//下拉菜单的具体内容
	XButton *m_buttom;			//下拉菜单中的按钮，3个按钮为功能按钮，其他为菜单按钮

	void updateString();				//跟新字符串到具体的菜单内容
	void changeDownMenuState();		//改变下拉菜单的状态

	static void funCtrlProc(void*,int,int);

	XRect m_inputArea;			//输入框的响应范围
	XRect m_downButtonArea;	//下拉按钮的响应范围
	XRect m_downMenuUpArea;		//上翻页的响应范围
	XRect m_downMenuArea;		//选择项的响应范围
	XRect m_downMenuDownArea;	//下翻页的响应范围
	XRect m_allArea;			//整个空间的响应范围

	XResourceInfo *m_resInfo;
public:
	XBool init(const XComboSkin &tex,	//控件的贴图
		const XVec2& position,		//控件的位置
		const XRect& inputArea,		//输入框的有效范围
		const XRect& downButtonArea,	//下拉按钮的响应范围
		const XRect& downMenuUpArea,	//上翻页的响应范围
		const XRect& downMenuArea,		//选择项的响应范围
		const XRect& downMenuDownArea,	//下翻页的响应范围
		int menuSum,					//下拉菜单中的选项的数量
		int drawMenuSum,				//下拉菜单中显示的菜单项的数量
		const XFontUnicode& font,		//显示文字使用的字体
		float fontSize = 1.0f);				//字体的大小
	XBool initEx(const XComboSkin &tex,	//上面接口的简化版本
		const XVec2& position,		
		int menuSum,					
		int drawMenuSum,				
		const XFontUnicode& font,		
		float fontSize = 1.0f);
	XBool initPlus(const char * path,
		int menuSum,					//下拉菜单中的选项的数量
		int drawMenuSum,				//下拉菜单中显示的菜单项的数量
		const XFontUnicode& font,		//显示文字使用的字体
		float fontSize = 1.0f,
		XResPos resPos = RES_SYS_DEF);
	//int initWithoutSkin(const XRect& inputArea,		//输入框的有效范围
	//	const XRect& downButtonArea,	//下拉按钮的响应范围
	//	const XRect& downMenuUpArea,	//上翻页的响应范围
	//	const XRect& downMenuArea,		//选择项的响应范围
	//	const XRect& downMenuDownArea,	//下翻页的响应范围
	//	int menuSum,					//下拉菜单中的选项的数量
	//	int drawMenuSum,				//下拉菜单中显示的菜单项的数量
	//	const XFontUnicode& font,		//显示文字使用的字体
	//	float fontSize);
	XBool initWithoutSkin(int inputLen,
		int menuSum,					//下拉菜单中的选项的数量
		int drawMenuSum,				//下拉菜单中显示的菜单项的数量
		const XFontUnicode& font,		//显示文字使用的字体
		float fontSize = 1.0f);
	XBool initWithoutSkin(int inputLen,
		int menuSum,					//下拉菜单中的选项的数量
		int drawMenuSum)				//下拉菜单中显示的菜单项的数量
	{
		return initWithoutSkin(inputLen,menuSum,drawMenuSum,getDefaultFont(),1.0f);
	}

protected:			
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);	//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2& p);
	void setLostFocus();	//设置失去焦点
public:
	int getCurChooseOrder()const {return m_curChooseOrder;}	//获取当前选项编号
	const std::string getCurChooseMenu(); 	//获取当前选择的菜单的内容
	XBool setCurChooseOrder(int index);	//设置当前选项编号
	XBool setMenuStr(const char *str,int order);			//设置某个下拉菜单的文字
	XBool setMenuStr(const char *str);			//设置多项的值，每项之间用';'隔开,如果总项数不匹配，则自动匹配
	std::string getMenuStr() const;
	std::string getMenuStr(int order);
//	void setShowMenuSum(int sum);	//动态改变显示的选项数量(尚未实现)
//	void setMenuSum(int sum);		//设置总的菜单数量(尚未实现)
	//重新设置下拉菜单的菜单数量
	bool resetMenuSum(int menuSum,int showMenuSum);	//这个函数尚未经过严格测试

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(const XVec2&s);			//设置控件的尺寸

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2&p);		//设置空间的位置

	void setTextColor(const XFColor& color);	//设置字体的颜色
	const XFColor& getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(const XFColor& c); 	//设置按钮的颜色
	void setAlpha(float a);	//设置按钮的颜色

	//副本拥有自己的行为,但是与母体使用同样的资源,如果母体资源被改变,副本的资源也会相应的被改变
	XBool setACopy(const XCombo &temp);	//建立一个副本
	void release();						//释放资源
	XCombo();
	~XCombo(){release();}

	void disable();					//使无效
	void enable();					//使能
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	//virtual void justForTest() {}
	//下面这个函数只有在不使用贴图的时候才有效
	void setInputLen(int len);
	int getInputLen()const{return m_inputArea.getWidth();}
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	XCombo(const XCombo &temp);
	XCombo& operator = (const XCombo& temp);
public:
	enum XComboEvent
	{
		CMB_DATA_CHANGE,
	};
	enum XComboStyle
	{
		CMB_STYLE_DOWN,	//向下的菜单方式
		CMB_STYLE_UP,	//向上的菜单方式
	};
private:
	XComboStyle m_curStyle;	//当前的样式
	void updateMenuBtnPosition();
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(完成)
public:
	void setStyle(XComboStyle style)
	{
		if(m_curStyle == style) return;
		m_curStyle = style;
		if(!m_isInAction)
			updateMenuBtnPosition();
	}
	XComboStyle getStyle()const{return m_curStyle;}
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		int tmp = getCurChooseOrder();
		if(!XXml::addLeafNode(e,m_ctrlName.c_str(),XString::toString(tmp))) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		int tmp;
		if(XXml::getXmlAsInt(e,m_ctrlName.c_str(),tmp) == NULL) return XFalse;
		setCurChooseOrder(tmp);
		return XTrue;
	}
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XCombo.inl"
#endif
}
#endif