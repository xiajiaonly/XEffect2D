#ifndef _JIA_XRADIOS_
#define _JIA_XRADIOS_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//这是一个单选框的类，这个类和MFC是有区别的。
//1、单选框不能自动组队需要在初始化的时侯设定，一组单选框中的单选项数量
//2、单个的单选框实际上是就是单个的复选框，实际上是没有区别的

//控件的激活方式需要修改

//整体的移动和缩放，统一使用相对于组空间基本点的移动和缩放
#include "XCheck.h"

typedef _XCheckTexture _XRadiosTexture;

class _XRadios:public _XControlBasic
{
private:
	_XBool m_isInited;	//空间时都已经初始化

	int m_radioSum;		//选项的数量
	int m_nowChoose;	//当前所选的项的编号
	_XCheck *m_radio;	//所有单选项的指针
	_XVector2 *m_checkPosition;	//单选项的相对位置
	_XVector2 m_distance;	//设置的每个单选项之间的距离

	void (*m_funStateChange)(void *,int);			//控件状态改变时调用
	void *m_pClass;				//回调函数的参数

	const _XCheckTexture *m_texture;

	_XFontUnicode m_caption;
	float m_captionSize;
	_XFColor m_textColor;	//显示的字体的颜色
	_XVector2 m_textPosition;

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//没有贴图的形式
public:
	_XBool init(int radioSum,			//选项的数量
		const _XVector2& distance,	//每个单选项之间的距离
		const _XVector2& position,	//控件的位置
		const _XRect &Area,			//选择图标的鼠标响应范围
		const _XRadiosTexture* tex,const _XFontUnicode &font,float captionSize,
		const _XVector2& textPosition);		//单选框初始化
	_XBool initEx(int radioSum,			//选项的数量
		const _XVector2& distance,	//每个单选项之间的距离
		const _XVector2& position,	//控件的位置
		const _XRadiosTexture* tex,const _XFontUnicode &font,float captionSize);
	_XBool initPlus(int radioSum,			//选项的数量
		const _XVector2& distance,	//每个单选项之间的距离
		const char *path,const _XFontUnicode &font,float captionSize,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(int radioSum,
		const _XVector2& distance,
		const _XRect &area,
		const _XFontUnicode &font,float captionSize,
		const _XVector2& textPosition);
protected:
	void draw();
	void drawUp(){};						//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);		//当处于激活状态的时候，可以通过上下左右4个按键改变选择的值
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float x,float y){return XTrue;}
public:
	void disable();
	void enable();
	void setChoosed(int temp);									//设置当前选择的单选项
	void setRadioPosition(const _XVector2& position,int order);			//设置单选项中某一项的位置(这里使用相对坐标)
	void setRadioPosition(float x,float y,int order);			//设置单选项中某一项的位置(这里使用相对坐标)
	
	void setDistance(const _XVector2& distance)
	{
		if(m_isInited == 0) return;
		m_distance = distance;
		//更新数据
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_checkPosition[i].set(m_distance.x * i,m_distance.y * i);
			m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y);
		}
	}
	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);				//设置缩放比例

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);		//设置位置

	void setTextColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].setTextColor(m_textColor * m_color);
		}
	}	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a) 
	{
		if(m_isInited == 0) return;
		m_color.setColor(r,g,b,a);
		m_caption.setColor(m_textColor * m_color);
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].setColor(m_color);
		}
	}	//设置按钮的颜色
	void setAlpha(float a) 
	{
		if(m_isInited == 0) return;
		m_color.setA(a);
		m_caption.setColor(m_textColor * m_color);
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].setColor(m_color);
		}
	}	//设置按钮的颜色

	_XBool setACopy(const _XRadios &temp);			//设置一个副本
	_XBool setRadioSum(int radioSum);

	_XRadios();
	~_XRadios();
    //下面是内联函数
	void release();	//释放分配的资源
	void setCallbackFun(void (* funStateChange)(void *,int),void *pClass = NULL);
	int getNowChoose();	//返回当前单选项中选择的项的编号
	void setRadioText(const char *temp,int order);	//设置单选项中某一项的文字
	void setRadiosText(const char * temp);			//设置多项的值，每项之间用'\n'隔开,如果总项数不匹配，则自动匹配
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	virtual void justForTest() {}
private:	//为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XRadios(const _XRadios &temp);
	_XRadios& operator = (const _XRadios& temp);
};
inline void _XRadios::setCallbackFun(void (* funStateChange)(void *,int),void *pClass)
{
	if(funStateChange != NULL) m_funStateChange = funStateChange;
	if(pClass == NULL) m_pClass = this;
	else m_pClass = pClass;
}
inline int _XRadios::getNowChoose()	//返回当前单选项中选择的项的编号
{
	return m_nowChoose;
}
inline void _XRadios::setRadioText(const char *temp,int order)	//设置单选项中某一项的文字
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(temp == NULL) return;
	if(order < 0 || order >= m_radioSum) return;
	m_radio[order].setText(temp);
}
inline void _XRadios::setRadioPosition(const _XVector2& position,int order)
{
	setRadioPosition(position.x,position.y,order);
}

#endif