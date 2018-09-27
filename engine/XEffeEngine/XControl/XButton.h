#ifndef _JIA_XBUTTON_
#define _JIA_XBUTTON_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XControlBasic.h"
#include "XResourceManager.h"
//#include "../XMath/XMoveData.h"

//这是一个按钮的类，可以相应按钮事件等
namespace XE{
enum XButtonState
{
	BUTTON_STATE_NORMAL,		//按钮状态普通状态
	BUTTON_STATE_DOWN,		//按钮按下的状态
	BUTTON_STATE_ON,			//按钮鼠标处于悬浮的状态
	BUTTON_STATE_DISABLE		//按钮无效的状态
};
enum XButtonStyle
{
	BTN_STYLE_NORMAL,	//普通样式按钮
	BTN_STYLE_CHECK,	//check样式的按钮
};
enum XButtonSymbol
{
	BTN_SYMBOL_NULL,		//没有
	BTN_SYMBOL_LINE,		//减号
	BTN_SYMBOL_CIRCLE,		//圆形
	BTN_SYMBOL_RECT,		//矩形
	BTN_SYMBOL_CROSS,		//十字架
	BTN_SYMBOL_TRIANGLE,	//三角形
	BTN_SYMBOL_MASK_RIGHT,	//对号
	BTN_SYMBOL_MASK_WRONG,	//错号
	BTN_SYMBOL_LEFT,		//左
	BTN_SYMBOL_RIGHT,		//右
	BTN_SYMBOL_UP,			//上
	BTN_SYMBOL_DOWN,		//下

	BTN_SYMBOL_SAVE,		//保存
	BTN_SYMBOL_LOAD,		//读取
	BTN_SYMBOL_DEF,			//默认
	BTN_SYMBOL_DOWNLOAD,	//下载
	BTN_SYMBOL_UPDATE,		//上传
};
//控件事件的响应使用回调函数的方式
//建立事件	暂时先不用
//注销事件	暂时先不用
//悬浮事件
//按下事件
//弹起事件
class XButtonSkin
{
private:
	XBool m_isInited;
	//XSCounter *m_cp;		//引用计数器,尚未完成
	void releaseTex();
public:
	XTextureData *buttonNormal;			//普通状态下的按钮贴图
	XTextureData *buttonDown;				//按下状态下的按钮贴图
	XTextureData *buttonOn;				//悬浮状态下的按钮贴图
	XTextureData *buttonDisable;			//无效状态下的按钮贴图

	XRect m_mouseRect;			//鼠标的响应范围
	XVec2 m_fontPosition;	//放置文字的位置
	int m_areaPointSum;			//点整的数量
	XVec2 *m_pArea;			//点整的信息

	XButtonSkin();
	~XButtonSkin(){release();}
	XBool init(const char *normal,const char *down,const char *on,const char *disable,XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
private:
	bool loadFromFolder(const char *filename,XResPos resPos);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename,XResPos resPos);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename,XResPos resPos);		//从网页中读取资源
};

//存在的问题：标题没有显示居中
class XCombo;
class XMouseRightButtonMenu;
class XDirectoryList;
class XGroup;
class XSliderEx;
class XSliderInfinite;
class XPasswordPad;
class XTab;
class XCalendar;
class XButtonBar;
class XSubWindow;
class XSubWindowEx;
class XParameterCtrl;
class XButton:public XControlBasic
{
	friend XCombo;
	friend XMouseRightButtonMenu;
	friend XDirectoryList;
	friend XGroup;
	friend XSliderEx;
	friend XSliderInfinite;
	friend XPasswordPad;
	friend XTab;
	friend XCalendar;
	friend XButtonBar;
	friend XSubWindow;
	friend XSubWindowEx;
	friend XParameterCtrl;
private:
	XBool m_isInited;					//按钮是否被初始化
	XButtonState m_curButtonState;		//当前的按钮状态

	XFontUnicode m_caption;			//按钮的标题

	const XTextureData *m_buttonNormal;			//普通状态下的按钮贴图(无论如何，这个贴图一定要有)
	const XTextureData *m_buttonDown;			//按下状态下的按钮贴图
	const XTextureData *m_buttonOn;				//悬浮状态下的按钮贴图
	const XTextureData *m_buttonDisable;			//无效状态下的按钮贴图
	XSprite m_sprite;	//用于显示贴图的精灵
	XVec2 m_textPosition;	//文字显示的位置，是相对于控件的位置来定的
	XVec2 m_textSize;		//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	XFColor m_textColor;		//文字的颜色
	XVec2 m_upMousePoint;	//上次记录的鼠标位置

	int m_hotKey;

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//没有贴图的形式
	XBool initProc(const XFontUnicode& font,const char *caption,float captionSize);	//公共的初始化过程
public:
	//需要注意的是这里的字体的位置，随着控件的缩放存在一些bug，需要实际使用中微调，不能做动态效果，以后需要改进
	XBool init(const XVec2& position,	//控件所在的位置
		const XRect& Area,				//控件鼠标响应的区间，使用的是相对于图片左上角的坐标
		const XButtonSkin &tex,		//控件的贴图信息
		const char *caption,const XFontUnicode& font,float captionSize,const XVec2& textPosition);	//控件标题的相关信息
	XBool initEx(const XVec2& position,	//上面一个接口的简化版本
		const XButtonSkin &tex,		
		const char *caption,const XFontUnicode& font,float captionSize = 1.0f);
	XBool initPlus(const char * path,const char *caption,const XFontUnicode& font,float captionSize = 1.0f,
		XResPos resPos = RES_SYS_DEF);//这是经过最终优化的版本，估计以后尽量只是用这个版本
	XBool initWithoutSkin(const char *caption,const XFontUnicode& font,
		float captionSize,const XRect& area,const XVec2& textPosition);	//没有贴图的形式,直接用写屏绘图函数绘图(尚未实现，未完成工作之一)
	XBool initWithoutSkin(const char *caption,const XFontUnicode& font,const XRect& area);	//这个接口是上个接口的简化版本
	XBool initWithoutSkin(const char *caption,const XRect& area) {return initWithoutSkin(caption,getDefaultFont(),area);}
	XBool initWithoutSkin(const char *caption,const XVec2& pixelSize) 
	{
		return initWithoutSkin(caption,getDefaultFont(),XRect(XVec2::zero,pixelSize));
	}
	XBool initWithoutSkin(const char *caption,float width) 
	{
		return initWithoutSkin(caption,getDefaultFont(), XRect(0.0f, 0.0f, width, MIN_FONT_CTRL_SIZE));
	}

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color);	//设置字体的颜色
	const XFColor& getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);
protected:
	void draw();								//描绘按钮
	void drawUp();							
	XBool mouseProc(const XVec2& p,XMouseState mouseState);		//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);			//返回是否触发按键动作
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);				//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&){return XTrue;}	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点
public:
	void setTexture(const XButtonSkin& tex);

	XBool setACopy(const XButton &temp);

	XButton();
	~XButton(){release();}
	void release();
	//下面是为了提升效率而定义的内联函数
	void setCaptionPosition(const XVec2& textPosition);			//设置按钮的标题的位置，相对于按键左上角
//	void setCaptionPosition(float x,float y);						//设置按钮的标题的位置，相对于按键左上角
	void setCaptionSize(const XVec2& size);						//设置按钮的标题的尺寸
//	void setCaptionSize(float x,float y);							//设置按钮的标题的尺寸
	void setCaptionText(const char *caption);						//设置按钮的标题的文字
	void setCaptionAlignmentModeX(XFontAlignmentModeX x){m_caption.setAlignmentModeX(x);}			//设置字体的对其方式
	void setCaptionAlignmentModeY(XFontAlignmentModeY y){m_caption.setAlignmentModeY(y);}	
	const char *getCaptionString() const {return m_caption.getString();}
	void setHotKey(int hotKey);	//设置按键的热键
	int getHotKey() const;			//获取当前按键的热键信息
	void setState(XButtonState temp);		//设置按钮的状态
	XButtonState getState() const; 
	void disable();
	void enable();
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	////virtual void justForTest() {;}
	//在不使用贴图的情况下才能调用这个函数
	//调用这个函数之后字体自动修改为居中对齐
	void setWidth(int width);	//设置按钮的宽度
	int getWidth()const{return m_mouseRect.getWidth();}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XButton(const XButton &temp);
	XButton& operator = (const XButton& temp);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了给btn添加icon而定义的接口，icon分两种:普通icon和无效icon
private:
	XBool m_withNormalIcon;		//是否拥有普通icon
	XSprite m_normalIcon;
	XBool m_withDisableIcon;		//是否拥有无效icon
	XSprite m_disableIcon;	
	XVec2 m_iconSize;		//icon的尺寸
	XVec2 m_iconPosition;	//icon的位置
public:
	void setNormalIcon(const char * filename,
		XResPos resPos = RES_SYS_DEF);
	void setNormalIcon(const XSprite &icon);
	void setDisableIcon(const char * filename,
		XResPos resPos = RES_SYS_DEF);
	void setDisableIcon(const XSprite &icon);
	void removeIcon();	//去掉所有的icon
//	void setIconPosition(float x, float y) { setIconPosition(XVec2(x, y)); }	//设置icon的位置
	void setIconPosition(const XVec2& p);	//设置icon的位置
//	void setIconSize(float x,float y) { setIconSize(XVec2(x, y)); }		//设置icon的缩放大小
	void setIconSize(const XVec2& p);	//设置icon的缩放大小
	void setIconSize(float x) { setIconSize(XVec2(x)); }
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对控件动态支持而定义的相关属性和方法
private:
	XMoveData m_actionMoveData;	//动态效果的变量
	XVec2 m_oldPos;				//动作播放时的位置
	XVec2 m_oldSize;			//动作播放时的大小
	XMoveData m_lightMD;
	XRect m_lightRect;
public:
protected:
	void update(float stepTime);
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面增加显示特殊符号
private:
	XButtonSymbol m_symbolType;	//显示的符号
public:
	void setSymbol(XButtonSymbol type){m_symbolType = type;}	//设置显示的符号
	XButtonSymbol getSymbol() const {return m_symbolType;}		//获取显示的符号
	void disSymbol(){m_symbolType = BTN_SYMBOL_NULL;}			//取消符号显示
	//---------------------------------------------------------
public:
	enum XButtonEvent
	{
		BTN_INIT,
		BTN_RELEASE,
		BTN_MOUSE_ON,
		BTN_MOUSE_DOWN,
		BTN_MOUSE_UP,
	};
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了支持check样式的按钮而定义的
private:
	XButtonStyle m_buttonStyle;	//按钮的样式
	bool m_isCheck;	//是否按下
public:
	void setStyle(XButtonStyle style){m_buttonStyle = style;}
	void setIsCheck(bool flag){m_isCheck = flag;}
	bool getIsCheck()const{return m_isCheck;}
	//--------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XButton.inl"
#endif
}
#endif