#ifndef _JIA_XCHECK_
#define _JIA_XCHECK_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//这是一个用于实现复选框的类

#include "XControlBasic.h"
#include "../XFontUnicode.h"
#include "../XSprite.h"

#define XCHECK_LEFT_ADD_LENGTH 15	//在字符后面加上这个像素长度作为响应范围，为了使控制更加合理

class _XCheckTexture
{
private:
	_XBool m_isInited;
	void releaseTex();
public:
	_XTextureData *checkChoosed;				//选择按钮选中的贴图
	_XTextureData *checkDischoose;				//选择按钮未选中的贴图
	_XTextureData *checkDisableChoosed;			//无效状态下选择按钮选中的贴图
	_XTextureData *checkDisableDischoose;		//无效状态下选择按钮未选中的贴图

	_XRect m_mouseRect;			//鼠标的响应范围
	_XVector2 m_fontPosition;	//放置文字的位置

	_XCheckTexture();
	~_XCheckTexture();
	_XBool init(const char *choosed,const char *disChoose,const char *disableChoosed,const char *disableDischoose,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};
class _XRadios;
class _XMultiListBasic;
class _XDirectoryList;
class _XCheck:public _XControlBasic
{
	friend _XRadios;
	friend _XMultiListBasic;
	friend _XDirectoryList;
private:
	_XBool m_isInited;					//进度条是否被初始化
	_XFontUnicode m_caption;			//进度条的标题

	const _XTextureData *m_checkChoosed;			//选择按钮选中的贴图
	const _XTextureData *m_checkDischoose;		//选择按钮未选中的贴图
	const _XTextureData *m_checkDisableChoosed;	//无效状态下选择按钮选中的贴图
	const _XTextureData *m_checkDisableDischoose;	//无效状态下选择按钮未选中的贴图

	void (*m_funInit)(void *,int ID);				//控件初始化的时候调用，（这个目前没有实际生效）
	void (*m_funRelease)(void *,int ID);				//控件注销的时候调用，（这个目前没有实际生效）
	void (*m_funMouseOn)(void *,int ID);				//鼠标悬浮时调用，（这个目前没有实际生效）
	void (*m_funMouseDown)(void *,int ID);			//鼠标按下时调用，（这个目前没有实际生效）		
	void (*m_funMouseUp)(void *,int ID);				//鼠标弹起时调用
	void (*m_funStateChange)(void *,int ID);			//控件状态改变时调用
	void *m_pClass;				//回调函数的参数

	_XSprite m_sprite;			//用于显示贴图的精灵
	_XVector2 m_textPosition;		//文字的相对位置
	_XVector2 m_textSize;			//文字的尺寸
	_XFColor m_textColor;		//文字的颜色

	_XBool m_state;				//复选框的选择状态
	_XRect m_mouseClickArea;	//字体的鼠标响应范围
	_XBool m_withCaption;			//是否拥有文字
		//下面是关于关联变量的接口
	_XBool *m_pVariable;	//关联的变量

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//没有贴图的形式
public:
	void setConnectVar(_XBool * p) {m_pVariable = p;}	//关联变量
	void disConnectVar() {m_pVariable = NULL;}			//取消变量关联

public:
	_XBool getWithCaption() const {return m_withCaption;}
	void setWithCaption(_XBool withCaption)
	{
		m_withCaption = withCaption;
	}
	_XBool init(const _XVector2 & position,	//控件的位置
		const _XRect &Area,					//控件的图片的响应范围
		const _XCheckTexture &tex,			//控件的贴图
		const char *caption,const _XFontUnicode &font,float captionSize,	//显示的字体的相关信息
		const _XVector2 &textPosition);		//字体的位置
	_XBool initEx(const _XVector2 & position,	//控件的位置
		const _XCheckTexture &tex,			//控件的贴图
		const char *caption,const _XFontUnicode &font,float captionSize);	//显示的字体的相关信息
	_XBool initPlus(const char * path,			//控件的贴图
		const char *caption,const _XFontUnicode &font,
		float captionSize,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//显示的字体的相关信息
	_XBool initWithoutTex(const char *caption,				//文字
		const _XFontUnicode &font,float captionSize,	//字体以及尺寸
		const _XRect& area,_XVector2 captionPosition);	//空间的大小以及文字的相对位置
protected:
	void draw();
	void drawUp(){};						//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//对于鼠标动作的响应函数(返回动作是否造成改变)
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);		//对键盘动作的响应
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float x,float y){return XTrue;}
public:
	void setCallbackFun(void (* funInit)(void *,int),
							 void (* funRelease)(void *,int),
							 void (* funMouseOn)(void *,int),
							 void (* funMouseDown)(void *,int),
							 void (* funMouseUp)(void *,int),
							 void (*funStateChange)(void *,int),void *pClass = NULL);

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);//设置控件的位置

	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color) 
	{
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
	}	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a) 	//设置按钮的颜色
	{
		m_color.setColor(r,g,b,a);
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		updateChildColor();
	}	//设置按钮的颜色
	void setAlpha(float a) 
	{
		m_color.setA(a);
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		updateChildAlpha();
	}	//设置按钮的颜色

	void setText(const char *temp);				//设置复选框的文字
	_XBool setACopy(const _XCheck &temp);			//复制一个副本
	_XCheck();
	~_XCheck();
	void release();
	void disable();
	void enable();
	_XBool getState() const;
	void setState(_XBool temp);
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	virtual void justForTest() {;}
private:	//这里重载复制构造函数和赋值操作符，防止意外调用的错误
	_XCheck(const _XCheck &temp);
	_XCheck& operator = (const _XCheck& temp);
};

inline void _XCheck::disable()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(m_isEnable)m_isEnable = XFalse;
}
inline void _XCheck::enable()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(!m_isEnable) m_isEnable = XTrue;
}
inline _XBool _XCheck::getState() const
{
	return m_state;
}
inline void _XCheck::setState(_XBool temp)
{
	//理论上状态不论何时都是可以改变的
//	if(!m_isVisiable) return;	//如果不可见直接退出
//	if(!m_isEnable) return;
	if((m_state && temp) || (!m_state && !temp)) return;
	m_state = temp;
//	if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
}
#endif