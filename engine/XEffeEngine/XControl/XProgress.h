#ifndef _JIA_XPROGRESS_
#define _JIA_XPROGRESS_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//这是一个用于进度条的类，进度条使用三层贴图的解构，就是背景层，移动层和遮罩或者光反射层，其中移动层是必须要有的。

#include "XControlBasic.h"
#include "../XNumber.h"
#include "../XSprite.h"

class _XProgressTexture
{
private:
	_XBool m_isInited;
public:
	_XTextureData *progressBackGround;			//进度条的背景贴图
	_XTextureData *progressMove;				//进度条用移动变化的贴图
	_XTextureData *progressUpon;				//进度条中上层的遮罩或者立体光效

	_XRect m_mouseRect;			//鼠标的响应范围
	_XVector2 m_fontPosition;	//放置文字的位置

	_XProgressTexture();
	~_XProgressTexture();
	_XBool init(const char *backgound,const char *move,const char *upon,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};

//进度条的数值设定范围为0.0 至 100.0
class _XProgress:public _XControlBasic
{
private:
	float m_nowValue;					//进度条的当前值

	_XBool m_isInited;					//进度条是否被初始化
	char m_mode;	//0普通模式 1竖屏模式
	_XNumber m_caption;					//进度条的标题

	const _XTextureData *m_progressBackGround;	//进度条的背景贴图
	const _XTextureData *m_progressMove;			//进度条用移动变化的贴图
	const _XTextureData *m_progressUpon;			//进度条中上层的遮罩或者立体光效

	_XSprite m_spriteBackground;	//用于显示背景的精灵
	_XSprite m_spriteMove;			//用于显示贴图的精灵
//	_XVector2 m_movePosition;			//动作条相对于背景的位置
	_XSprite m_spriteUpon;			//用于显示上层遮罩的精灵
//	_XVector2 m_uponPosition;			//遮罩相对于背景的位置
	_XVector2 m_textPosition;			//文字显示的位置
	_XVector2 m_textSize;				//文字的尺寸
	_XFColor m_textColor;		//文字的颜色

	_XBool m_isShowFont;				//是否显示数字文字
	_XBool m_isACopy;			//这个实体是否是一个副本

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//没有贴图的形式
public:
	_XBool init(const _XVector2& position,//控件所在的位置
		const _XRect &Area,	//进度条区域的范围
		const _XProgressTexture &tex,	//控件的贴图
		_XNumber* font,float captionSize,const _XVector2& textPosition,	//控件的文字
		int mode = 0);
	_XBool initEx(const _XVector2& position,//控件所在的位置
		const _XProgressTexture &tex,	//控件的贴图
		_XNumber* font,float captionSize,	//控件的文字
		int mode = 0);
	_XBool initPlus(const char * path,
		_XNumber* font,float captionSize,	//控件的文字
		int mode = 0,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(const _XRect &area,
		_XNumber* font,float captionSize,	//控件的文字
		const _XVector2& textPosition,int mode = 0);

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color) 
	{
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
	}	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a) 
	{
		m_color.setColor(r,g,b,a);
		m_spriteBackground.setColor(m_color);
		m_spriteMove.setColor(m_color);
		m_spriteUpon.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
	}	//设置按钮的颜色
	void setAlpha(float a) 
	{
		m_color.setA(a);
		m_spriteBackground.setColor(m_color);
		m_spriteMove.setColor(m_color);
		m_spriteUpon.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
	}	//设置按钮的颜色

protected:
	void draw();
	void drawUp(){};						//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState){return XTrue;}	//do nothing
	_XBool keyboardProc(int keyOrder,_XKeyState keyState){return XTrue;}	//do nothing
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y){return XFalse;}	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float x,float y){return XTrue;}
public:
	void setValue(float temp);
	float getValue() const;

	_XBool setACopy(const _XProgress &temp);		//建立一个副本
	_XProgress();
	~_XProgress();
	void release();
	//下面是内联函数
	void enable();
	void disable();
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
private://为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XProgress(const _XProgress &temp);
	_XProgress& operator = (const _XProgress& temp);

	virtual void justForTest() {}
};
inline float _XProgress::getValue() const
{
	return m_nowValue;
}
inline void _XProgress::enable()
{
	m_isEnable = XTrue;
}
inline void _XProgress::disable()
{
	m_isEnable = XFalse;
}

#endif