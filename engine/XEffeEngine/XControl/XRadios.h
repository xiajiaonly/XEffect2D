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

class _XRadios:public _XControlBasic,public _XBasicOprate
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
	_XBool initEx(int radioSum,			//对上面接口的简化
		const _XVector2& distance,	
		const _XVector2& position,	
		const _XRadiosTexture* tex,const _XFontUnicode &font,float captionSize = 1.0);
	_XBool initPlus(int radioSum,			//选项的数量
		const _XVector2& distance,	//每个单选项之间的距离
		const char *path,const _XFontUnicode &font,float captionSize = 1.0f,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(int radioSum,
		const _XVector2& distance,
		const _XRect &area,
		const _XFontUnicode &font,float captionSize,
		const _XVector2& textPosition);
	_XBool initWithoutTex(int radioSum,
		const _XVector2& distance,
		const _XRect &area,
		const _XFontUnicode &font,
		const _XVector2& textPosition)
	{
		return initWithoutTex(radioSum,distance,area,font,1.0f,textPosition);
	}
	_XBool initWithoutTex(int radioSum,
		const _XVector2& distance,
		const _XRect &area,
		const _XVector2& textPosition)
	{
		return initWithoutTex(radioSum,distance,area,XEE::systemFont,1.0f,textPosition);
	}
	_XBool initWithoutTex(int radioSum,
		const _XVector2& distance,
		const _XVector2 &pixelSize,
		const _XVector2& textPosition)
	{
		return initWithoutTex(radioSum,distance,_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			XEE::systemFont,1.0f,textPosition);
	}
protected:
	void draw();
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);		//当处于激活状态的时候，可以通过上下左右4个按键改变选择的值
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}
public:
	void disable();
	void enable();
	void setChoosed(int temp);									//设置当前选择的单选项
	void setRadioPosition(const _XVector2& position,int order);			//设置单选项中某一项的位置(这里使用相对坐标)
	void setRadioPosition(float x,float y,int order);			//设置单选项中某一项的位置(这里使用相对坐标)
	
	void setDistance(const _XVector2& distance);
	using _XObjectBasic::setSize;		//避免覆盖的问题
	void setSize(float x,float y);				//设置缩放比例

	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);		//设置位置

	void setTextColor(const _XFColor& color);//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(float r,float g,float b,float a);//设置按钮的颜色
	void setAlpha(float a);//设置按钮的颜色

	_XBool setACopy(const _XRadios &temp);			//设置一个副本
	_XBool setRadioSum(int radioSum);

	_XRadios();
	~_XRadios(){release();}
    //下面是内联函数
	void release();	//释放分配的资源
	void setCallbackFun(void (* funStateChange)(void *,int),void *pClass = NULL);
	int getNowChoose() const;	//返回当前单选项中选择的项的编号
	void setRadioText(const char *temp,int order);	//设置单选项中某一项的文字
	void setRadiosText(const char * temp);			//设置多项的值，每项之间用';'隔开,如果总项数不匹配，则自动匹配
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放

	//virtual void justForTest() {}
private:	//为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XRadios(const _XRadios &temp);
	_XRadios& operator = (const _XRadios& temp);
public:
	void setOprateState(void * data)
	{
		int index = *(int *)data;
		setChoosed(index);
	}
	void *getOprateState() const
	{
		int *data = createMem<int>();
		*data =  getNowChoose();
		return data;
	}
	virtual bool isSameState(void * data)
	{
		if(data == NULL) return false;
		return(*(int*)data == getNowChoose());
	}
};
#include "XRadios.inl"
#endif