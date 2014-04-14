#ifndef _JIA_XBUTTON_
#define _JIA_XBUTTON_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XControlBasic.h"
#include "../XFontUnicode.h"
#include "../XSprite.h"

//这是一个按钮的类，可以相应按钮事件等

enum _XButtonState
{
	BUTTON_STATE_NORMAL,		//按钮状态普通状态
	BUTTON_STATE_DOWN,		//按钮按下的状态
	BUTTON_STATE_ON,			//按钮鼠标处于悬浮的状态
	BUTTON_STATE_DISABLE		//按钮无效的状态
};

//控件事件的响应使用回调函数的方式
//建立事件	暂时先不用
//注销事件	暂时先不用
//悬浮事件
//按下事件
//弹起事件
class _XButtonTexture
{
private:
	_XBool m_isInited;
	//_XSCounter *m_cp;		//引用计数器,尚未完成
	void releaseTex();
public:
	_XTextureData *buttonNormal;			//普通状态下的按钮贴图
	_XTextureData *buttonDown;				//按下状态下的按钮贴图
	_XTextureData *buttonOn;				//悬浮状态下的按钮贴图
	_XTextureData *buttonDisable;			//无效状态下的按钮贴图

	_XRect m_mouseRect;			//鼠标的响应范围
	_XVector2 m_fontPosition;	//放置文字的位置
	int m_areaPointSum;			//点整的数量
	_XVector2 *m_pArea;			//点整的信息

	_XButtonTexture();
	~_XButtonTexture();
	_XBool init(const char *normal,const char *down,const char *on,const char *disable,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};

//存在的问题：标题没有显示居中
class _XCombo;
class _XMouseRightButtonMenu;
class _XDirectoryList;
class _XGroup;
class _XSliderEx;
class _XButton:public _XControlBasic
{
	friend _XCombo;
	friend _XMouseRightButtonMenu;
	friend _XDirectoryList;
	friend _XGroup;
	friend _XSliderEx;
private:
	char m_isInited;					//按钮是否被初始化
	_XButtonState m_nowButtonState;		//当前的按钮状态

	_XFontUnicode m_caption;			//按钮的标题

	const _XTextureData *m_buttonNormal;			//普通状态下的按钮贴图(无论如何，这个贴图一定要有)
	const _XTextureData *m_buttonDown;			//按下状态下的按钮贴图
	const _XTextureData *m_buttonOn;				//悬浮状态下的按钮贴图
	const _XTextureData *m_buttonDisable;			//无效状态下的按钮贴图

	void (*m_funInit)(void *,int ID);
	void (*m_funRelease)(void *,int ID);
	void (*m_funMouseOn)(void *,int ID);
	void (*m_funMouseDown)(void *,int ID);
	void (*m_funMouseUp)(void *,int ID);
	void *m_pClass;				//回调函数的参数

	_XSprite m_sprite;	//用于显示贴图的精灵
	_XVector2 m_textPosition;	//文字显示的位置，是相对于控件的位置来定的
	_XVector2 m_textSize;		//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	_XFColor m_textColor;		//文字的颜色
	_XVector2 m_upMousePoint;	//上次记录的鼠标位置

	int m_hotKey;

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//没有贴图的形式
public:
	//需要注意的是这里的字体的位置，随着控件的缩放存在一些bug，需要实际使用中微调，不能做动态效果，以后需要改进
	_XBool init(const _XVector2& position,	//控件所在的位置
		const _XRect& Area,				//控件鼠标响应的区间，使用的是相对于图片左上角的坐标
		const _XButtonTexture &tex,		//控件的贴图信息
		const char *caption,const _XFontUnicode &font,float captionSize,_XVector2 textPosition);	//控件标题的相关信息
	_XBool initEx(const _XVector2& position,	//控件所在的位置
		const _XButtonTexture &tex,		//控件的贴图信息
		const char *caption,const _XFontUnicode &font,float captionSize);
	_XBool initPlus(const char * path,const char *caption,const _XFontUnicode &font,
		float captionSize,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);//这是经过最终优化的版本，估计以后尽量只是用这个版本
	_XBool initWithoutTex(const char *caption,const _XFontUnicode &font,
		float captionSize,const _XRect& area,_XVector2 textPosition);	//没有贴图的形式,直接用写屏绘图函数绘图(尚未实现，未完成工作之一)

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//避免覆盖的问题
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color) 
	{
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
	}	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a)
	{
		m_color.setColor(r,g,b,a);
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		updateChildColor();
	}
	void setAlpha(float a)
	{
		m_color.setA(a);
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		updateChildAlpha();
	}
protected:
	void draw();								//描绘按钮
	void drawUp(){};							//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);			//返回是否触发按键动作
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y);				//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float x,float y){return XTrue;}	//应该是可以随时失去焦点的
	void setLostFocus() 
	{
		if(m_isInited == 0) return;	//如果没有初始化直接退出
		if(m_isActive == 0) return;		//没有激活的控件不接收控制
		if(m_isVisiable == 0) return;	//如果不可见直接退出
		if(m_isEnable == 0) return;		//如果无效则直接退出

		if(m_nowButtonState != BUTTON_STATE_DISABLE) m_nowButtonState = BUTTON_STATE_NORMAL;
	}	//设置失去焦点
public:
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funMouseOn)(void *,int),
		void (* funMouseDown)(void *,int),
		void (* funMouseUp)(void *,int),
		void *pClass = NULL);
	void setTexture(const _XButtonTexture& tex);

	_XBool setACopy(const _XButton &temp);

	_XButton();
	~_XButton();
	void release();
	//下面是为了提升效率而定义的内联函数
	void setCaptionPosition(const _XVector2& textPosition);			//设置按钮的标题的位置，相对于按键左上角
	void setCaptionPosition(float x,float y);			//设置按钮的标题的位置，相对于按键左上角
	void setCaptionSize(const _XVector2& size);						//设置按钮的标题的尺寸
	void setCaptionSize(float x,float y);						//设置按钮的标题的尺寸
	void setCaptionText(const char *caption);						//设置按钮的标题的文字
	void setHotKey(int hotKey);	//设置按键的热键
	int getHotKey() const;			//获取当前按键的热键信息
	void setState(_XButtonState temp);		//设置按钮的状态
	_XButtonState getState() const; 
	void disable();
	void enable();
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	_XButton(const _XButton &temp);
	_XButton& operator = (const _XButton& temp);
};
inline void _XButton::setCaptionPosition(const _XVector2& textPosition)			//设置按钮的标题的位置，相对于按键左上角
{
	setCaptionPosition(textPosition.x,textPosition.y);
}
inline void _XButton::setCaptionPosition(float x,float y)			//设置按钮的标题的位置，相对于按键左上角
{
	m_textPosition.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
}
inline void _XButton::setCaptionSize(const _XVector2& size)						//设置按钮的标题的尺寸
{
	setCaptionSize(size.x,size.y);
}
inline void _XButton::setCaptionSize(float x,float y)						//设置按钮的标题的尺寸
{
	if(x < 0 || y < 0) return;
	m_textSize.set(x,y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
}
inline void _XButton::setHotKey(int hotKey)	//设置按键的热键
{
	m_hotKey = hotKey;
}
inline void _XButton::setCaptionText(const char *caption)						//设置按钮的标题的文字
{
	if(caption != NULL) m_caption.setString(caption);
}
inline int _XButton::getHotKey() const			//获取当前按键的热键信息
{
	return m_hotKey;
}
inline void _XButton::setState(_XButtonState temp)		//设置按钮的状态
{
	m_nowButtonState = temp;
}
 
inline _XButtonState _XButton::getState() const
{
	if(m_isEnable == 0) return BUTTON_STATE_DISABLE;
	return m_nowButtonState;
}
inline void _XButton::disable()
{
	m_isEnable = 0;
	m_nowButtonState = BUTTON_STATE_DISABLE;
}
inline void _XButton::enable()
{
	if(m_nowButtonState == BUTTON_STATE_DISABLE)
	{
		m_isEnable = 1;
		m_nowButtonState = BUTTON_STATE_NORMAL;
		mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	}
}

#endif