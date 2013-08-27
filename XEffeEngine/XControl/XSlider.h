#ifndef _JIA_XSLIDER_
#define _JIA_XSLIDER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XControlBasic.h"
#include "../XSprite.h"
#include "XButton.h"

enum _XSliderType
{
	_XSLIDER_TYPE_VERTICAL,		//垂直的
	_XSLIDER_TYPE_HORIZONTAL	//水平的
};

enum _XSliderState
{
	_XSLIDER_STATE_NORMAL,		//普通状态
	_XSLIDER_STATE_DOWN,		//普通状态
	_XSLIDER_STATE_ON,			//普通状态
	_XSLIDER_STATE_DISABLE		//普通状态
};

class _XSliderTexture
{
private:
	char m_isInited;
public:
	_XTextureData *sliderLineNormal;			//滑动条线的普通状态
	_XTextureData *sliderLineDown;				//滑动条线的按下状态
	_XTextureData *sliderLineOn;				//滑动条线的悬浮状态
	_XTextureData *sliderLineDisable;			//滑动条线的无效状态
	_XTextureData *sliderButtonNormal;			//滑动条按钮的普通状态
	_XTextureData *sliderButtonDown;			//滑动条按钮的按下状态
	_XTextureData *sliderButtonOn;				//滑动条按钮的悬浮状态
	_XTextureData *sliderButtonDisable;			//滑动条按钮的无效状态

	_XSliderTexture();
	~_XSliderTexture();
	int init(const char *LNormal,const char *LDown,const char *LOn,const char *LDisable,
		const char *BNormal,const char *BDown,const char *BOn,const char *BDisable,int resoursePosition = 0);
	void release();
};
class _XMultiList;
class _XMultiListBasic;
class _XMultiText;
class _XSlider:public _XControlBasic
{
	friend _XMultiList;
	friend _XMultiListBasic;
	friend _XMultiText;
private:
	char m_isInited;
	_XSliderState m_nowSliderState;			//滑动条的状态

	_XSliderType m_typeVorH;						//滑动条的类型

	const _XTextureData *m_sliderLineNormal;			//滑动条线的普通状态
	const _XTextureData *m_sliderLineDown;			//滑动条线的按下状态
	const _XTextureData *m_sliderLineOn;				//滑动条线的悬浮状态
	const _XTextureData *m_sliderLineDisable;			//滑动条线的无效状态
	const _XTextureData *m_sliderButtonNormal;		//滑动条按钮的普通状态
	const _XTextureData *m_sliderButtonDown;			//滑动条按钮的按下状态
	const _XTextureData *m_sliderButtonOn;			//滑动条按钮的悬浮状态
	const _XTextureData *m_sliderButtonDisable;		//滑动条按钮的无效状态

	void (*m_funInit)(void *,int);		//初始化
	void (*m_funRelease)(void *,int);		//资源释放
	void (*m_funMouseOn)(void *,int);		//鼠标悬浮
	void (*m_funMouseDown)(void *,int);	//鼠标按下
	void (*m_funMouseUp)(void *,int);		//鼠标弹起
	void (*m_funMouseMove)(void *,int);	//鼠标拖动花条时调用
	void (*m_funValueChange)(void *,int);	//滑动条的数值变化
	void *m_pClass;				//回调函数的参数

	_XRect m_buttonArea;		//滑动条中按钮的有效范围,这个范围是按钮的相对图片的响应范围，需要注意
	//_XRect m_nowButtonArea;		//当前鼠标的响应范围
	_XSprite m_buttonSprite;	//用于显示贴图的精灵
	_XSprite m_lineSprite;		//用于显示贴图的精灵

	_XVector2 m_upMousePoint;		//上次鼠标在滚动条的位置
	_XVector2 m_mouseFlyArea;		//鼠标拖曳是飞出的响应范围
	_XVector2 m_nowButtonPosition;//当前滑块按钮的位置
	float m_minValue;			//滑动条的最大值
	float m_maxValue;			//滑动条的最小值
	float m_nowValue;			//滑动条的当前值
	float m_upValue;			//滑动条的上一次值(定义这个变量的目的在于如果拖动滑块超出范围之后可以恢复到拖动之前得知，但是目前出于简单的考虑，不做这个动作)
	char m_dataChanged;			//滑动条的数值是否有被修改
	float m_keyOneValue;		//按下一次按键的滑动量(注意这个值比需要>=0,否则将会造成错误)
	void updateButtonData();	//根据实时数据的变化更新滑块的信息
public:
	int init(const _XVector2& position,	//控件所在的位置
		const _XRect& Area,				//控件的鼠标响应范围
		const _XRect& buttonArea,		//滑动块的鼠标响应范围(这个数据实际上没有起到作用)
		const _XSliderTexture &tex,_XSliderType type,float max,float min);
	void setPosition(const _XVector2& position);		//设置滑动条的位置
	void setPosition(float x,float y);		//设置滑动条的位置
	_XVector2 getPosition() const
	{
		return m_position;
	}
	void setSize(const _XVector2& size);				//设置滑动条的缩放比例
	void setSize(float x,float y);				//设置滑动条的缩放比例
	_XVector2 getSize() const
	{
		return m_size;
	}
	void setColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_color = color;
		m_buttonSprite.setColor(m_color);
		m_lineSprite.setColor(m_color);
	}	//设置字体的颜色
	_XFColor getColor() const {return m_color;}	//获取控件字体的颜色
protected:
	void draw();							//描绘滑动条
	void drawUp(){};						//do nothing
	int mouseProc(float x,float y,_XMouseState mouseState);	//对于鼠标动作的响应函数
	int keyboardProc(int keyOrder,_XKeyState keyState);
	int canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	int canLostFocus(float x,float y);
	void setLostFocus() 
	{
		if(m_isInited == 0) return;	//如果没有初始化直接退出
		if(m_isActive == 0) return;		//没有激活的控件不接收控制
		if(m_isVisiable == 0) return;	//如果不可见直接退出
		if(m_isEnable == 0) return;		//如果无效则直接退出

		if(m_nowSliderState != _XSLIDER_STATE_DISABLE) m_nowSliderState = _XSLIDER_STATE_NORMAL;
	}	//设置失去焦点
public:
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funMouseOn)(void *,int),
		void (* funMouseDown)(void *,int),
		void (* funMouseUp)(void *,int),
		void (*funValueChange)(void *,int),
		void (*funMouseMove)(void *,int),
		void *pClass);
	void setNowValue(float temp);			//设置滑块当前的值
	void setRange(float max,float min);

	int setACopy(const _XSlider &temp);			//建立一个副本

	_XSlider();
	~_XSlider();
	void release();
	//内联函数
	void disable();		//使控件无效
	void enable();		//使控件有效
	float getNowValue() const;	//获取滑块当前的值
	float getMaxValue() const; 
	float getMinValue() const;
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	int isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
private:	//为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XSlider(const _XSlider &temp);
	_XSlider& operator = (const _XSlider& temp);
};

inline void _XSlider::disable()//使控件无效
{
	m_nowSliderState = _XSLIDER_STATE_DISABLE;
}

inline void _XSlider::enable()//使控件有效
{
	if(m_nowSliderState == _XSLIDER_STATE_DISABLE)
	{
		m_nowSliderState = _XSLIDER_STATE_NORMAL;
		//还需要调用鼠标函数
		mouseProc(m_upMousePoint.x,m_upMousePoint.y,_XMOUSE_MOVE);
	}
}

inline float _XSlider::getNowValue() const	//获取滑块当前的值
{
	return m_nowValue;
}

inline float _XSlider::getMaxValue() const
{
	return m_maxValue;
}

inline float _XSlider::getMinValue() const
{
	return m_minValue;
}

inline void _XSlider::setSize(const _XVector2& size)
{
	setSize(size.x,size.y);
}

inline void _XSlider::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}
#endif