#ifndef _JIA_XBUTTONEX_
#define _JIA_XBUTTONEX_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.12
//--------------------------------
//这是一个对XButton类的强化，可以支持各种形状的button
#include "XButton.h"
namespace XE{
class XImageList;
class XButtonEx:public XControlBasic
{
	friend XImageList;
private:
	XBool m_isInited;					//按钮是否被初始化
	XButtonState m_curButtonState;		//当前的按钮状态

	XFontUnicode m_caption;			//按钮的标题

	const XTextureData *m_buttonNormal;		//普通状态下的按钮贴图(无论如何，这个贴图一定要有)
	const XTextureData *m_buttonDown;			//按下状态下的按钮贴图
	const XTextureData *m_buttonOn;			//悬浮状态下的按钮贴图
	const XTextureData *m_buttonDisable;		//无效状态下的按钮贴图

	XSprite m_sprite;			//用于显示贴图的精灵
	XVector2 m_textPosition;	//文字显示的位置，是相对于控件的位置来定的
	XVector2 m_textSize;		//文字显示的尺寸，这个尺寸会与空间的缩放尺寸叠加
	XFColor m_textColor;		//文字的颜色
	XVector2 m_upMousePoint;	//上次记录的鼠标位置
	XVector2 *m_pArea;			//保存形状的点的序列
	XVector2 *m_pCurArea;		//保存形状的点的序列
	int m_areaPointSum;			//保存形状的点的数量

	int m_hotKey;
	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//没有贴图的形式
	float *m_colorRate;
public:
	//需要注意的是这里的字体的位置，随着控件的缩放存在一些bug，需要实际使用中微调，不能做动态效果，以后需要改进
	XBool init(const XVector2& position,			//控件的位置
		const XVector2 *area,int pointSum,		//用于描述按钮的响应范围的序列点，以及点的数量
		const XButtonSkin &tex,				//按钮的个中贴图信息
		const char *caption,float captionSize,const XVector2 &captionPosition,	//按钮上显示的文字的相关信息
		const XFontUnicode &font);				//按钮上使用的字体
	XBool initEx(const XVector2& position,	//这是对上面接口的简化
		const XButtonSkin &tex,				
		const char *caption,float captionSize,	
		const XFontUnicode &font);
	XBool initPlus(const char *path,				//按钮的个中贴图信息
		const char *caption,float captionSize,	//按钮上现实的文字的相关信息
		const XFontUnicode &font,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);				//按钮上使用的字体
	XBool initWithoutSkin(const XVector2 *area,int pointSum,const char *caption,	//按钮上现实的文字的相关信息
		const XFontUnicode &font,const XVector2 &captionPosition,float captionSize = 1.0f);
	XBool initWithoutSkin(const XVector2 *area,int pointSum,const char *caption)	//按钮上现实的文字的相关信息
	{
		if(area == NULL || pointSum <= 0) return XFalse;
		XVector2 tmp(0.0f,0.0f);
		for(int i = 0;i < pointSum;++ i)
		{
			tmp += area[i];
		}
		tmp /= (float)(pointSum);
		initWithoutSkin(area,pointSum,caption,getDefaultFont(),tmp,1.0f);
	}

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(float x,float y);

	void setTextColor(const XFColor& color);	//设置字体的颜色
	XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);	//设置按钮的颜色
	void setAlpha(float a);	//设置按钮的颜色
protected:
	void draw();								//描绘按钮
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(float x,float y,XMouseState mouseState);		//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);			//返回是否触发按键动作
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(float,float){return XTrue;}
	void setLostFocus();	//设置失去焦点
public:
	void setTexture(const XButtonSkin& tex);

	XBool setACopy(const XButtonEx &temp);

	XButtonEx();
	~XButtonEx(){release();}
	//下面是为了提升效率而定义的内联函数
	void setCaptionPosition(const XVector2& textPosition);			//设置按钮的标题的位置，相对于按键左上角
	void setCaptionPosition(float x,float y);			//设置按钮的标题的位置，相对于按键左上角
	void setCaptionSize(const XVector2& size);						//设置按钮的标题的尺寸
	void setCaptionSize(float x,float y);						//设置按钮的标题的尺寸
	void setCaptionText(const char *caption);						//设置按钮的标题的文字
	const char *getCaptionString() const {return m_caption.getString();}
	void setHotKey(int hotKey);	//设置按键的热键
	int getHotKey() const;			//获取当前按键的热键信息
	void setState(XButtonState temp);		//设置按钮的状态
	XButtonState getState() const; 
	void disable();
	void enable();
	void release();	//资源释放函数
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	//virtual void justForTest() {}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XButtonEx(const XButtonEx &temp);
	XButtonEx& operator = (const XButtonEx& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对控件动态支持而定义的相关属性和方法
private:
	XMoveData m_actionMoveData;	//动态效果的变量
	XVector2 m_oldPos;				//动作播放时的位置
	XVector2 m_oldSize;			//动作播放时的大小
	XVector2 m_centerPos;			//多边形的中点
	XMoveData m_lightMD;
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面增加新的事件处理方式来处理控件的事件
public:
	enum XButtonExEvent
	{
		BTNEX_INIT,
		BTNEX_RELEASE,
		BTNEX_MOUSE_ON,
		BTNEX_MOUSE_DOWN,
		BTNEX_MOUSE_UP,
	};
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XButtonEx.inl"
#endif
}
#endif