#ifndef _JIA_XBUTTONEX_
#define _JIA_XBUTTONEX_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.12
//--------------------------------
//这是一个对XButton类的强化，可以支持各种形状的button
#include "XButton.h"

class _XButtonEx:public _XControlBasic
{
private:
	_XBool m_isInited;					//按钮是否被初始化
	_XButtonState m_nowButtonState;		//当前的按钮状态

	_XFontUnicode m_caption;			//按钮的标题

	const _XTextureData *m_buttonNormal;		//普通状态下的按钮贴图(无论如何，这个贴图一定要有)
	const _XTextureData *m_buttonDown;			//按下状态下的按钮贴图
	const _XTextureData *m_buttonOn;			//悬浮状态下的按钮贴图
	const _XTextureData *m_buttonDisable;		//无效状态下的按钮贴图

	void (*m_funInit)(void *,int ID);			//控件初始化的时候调用的函数
	void (*m_funRelease)(void *,int ID);		//控件资源释放的时候调用的函数
	void (*m_funMouseOn)(void *,int ID);		//鼠标悬浮的时候调用的函数
	void (*m_funMouseDown)(void *,int ID);		//鼠标按下的时候调用的函数
	void (*m_funMouseUp)(void *,int ID);		//鼠标弹起的时候嗲用的函数
	void *m_pClass;				//回调函数的参数

	_XSprite m_sprite;			//用于显示贴图的精灵
	_XVector2 m_textPosition;	//文字显示的位置，是相对于控件的位置来定的
	_XVector2 m_textSize;		//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	_XFColor m_textColor;		//文字的颜色
	_XVector2 m_upMousePoint;	//上次记录的鼠标位置
	_XVector2 *m_pArea;			//保存形状的点的序列
	_XVector2 *m_pNowArea;		//保存形状的点的序列
	int m_areaPointSum;			//保存形状的点的数量

	int m_hotKey;
	_XResourceInfo *m_resInfo;
public:
	//需要注意的是这里的字体的位置，随着控件的缩放存在一些bug，需要实际使用中微调，不能做动态效果，以后需要改进
	_XBool init(const _XVector2& position,			//控件的位置
		const _XVector2 *area,int pointSum,		//用于描述按钮的响应范围的序列点，以及点的数量
		const _XButtonTexture &tex,				//按钮的个中贴图信息
		const char *caption,float captionSize,_XVector2 captionPosition,	//按钮上现实的文字的相关信息
		const _XFontUnicode &font);				//按钮上使用的字体
	_XBool initEx(const _XVector2& position,			//控件的位置
		const _XButtonTexture &tex,				//按钮的个中贴图信息
		const char *caption,float captionSize,	//按钮上现实的文字的相关信息
		const _XFontUnicode &font);				//按钮上使用的字体
	_XBool initPlus(const char *path,				//按钮的个中贴图信息
		const char *caption,float captionSize,	//按钮上现实的文字的相关信息
		const _XFontUnicode &font,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);				//按钮上使用的字体

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
	}	//设置按钮的颜色
	void setAlpha(float a) 
	{
		m_color.setA(a);
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
	}	//设置按钮的颜色
protected:
	void draw();								//描绘按钮
	void drawUp(){};							//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);			//返回是否触发按键动作
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float x,float y);
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

	_XBool setACopy(const _XButtonEx &temp);

	_XButtonEx();
	~_XButtonEx();
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
	void release();	//资源释放函数
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	virtual void justForTest() {}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	_XButtonEx(const _XButtonEx &temp);
	_XButtonEx& operator = (const _XButtonEx& temp);
};

inline void _XButtonEx::setCaptionPosition(const _XVector2& textPosition)			//设置按钮的标题的位置，相对于按键左上角
{
	setCaptionPosition(textPosition.x,textPosition.y);
}

inline void _XButtonEx::setCaptionPosition(float x,float y)			//设置按钮的标题的位置，相对于按键左上角
{
	m_textPosition.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
}

inline void _XButtonEx::setCaptionSize(const _XVector2& size)						//设置按钮的标题的尺寸
{
	setCaptionSize(size.x,size.y);
}

inline void _XButtonEx::setCaptionSize(float x,float y)						//设置按钮的标题的尺寸
{
	if(x < 0 || y < 0) return;
	m_textSize.set(x,y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
}

inline void _XButtonEx::setHotKey(int hotKey)	//设置按键的热键
{
	m_hotKey = hotKey;
}

inline void _XButtonEx::setCaptionText(const char *caption)						//设置按钮的标题的文字
{
	if(caption != NULL)
	{
		m_caption.setString(caption);
	}
}

inline int _XButtonEx::getHotKey() const			//获取当前按键的热键信息
{
	return m_hotKey;
}

inline void _XButtonEx::setState(_XButtonState temp)		//设置按钮的状态
{
	m_nowButtonState = temp;
}
 
inline _XButtonState _XButtonEx::getState() const
{
	if(m_isEnable == 0) return BUTTON_STATE_DISABLE;
	return m_nowButtonState;
}

inline void _XButtonEx::disable()
{
	m_isEnable = 0;
	m_nowButtonState = BUTTON_STATE_DISABLE;
}

inline void _XButtonEx::enable()
{
	if(m_nowButtonState == BUTTON_STATE_DISABLE)
	{
		m_isEnable = 1;
		m_nowButtonState = BUTTON_STATE_NORMAL;
		mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	}
}

#endif