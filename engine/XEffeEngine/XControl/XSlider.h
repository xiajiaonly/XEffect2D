#ifndef _JIA_XSLIDER_
#define _JIA_XSLIDER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XButton.h"

enum _XSliderType
{
	SLIDER_TYPE_VERTICAL,		//垂直的
	SLIDER_TYPE_HORIZONTAL	//水平的
};
enum _XSliderState
{
	SLIDER_STATE_NORMAL,		//普通状态
	SLIDER_STATE_DOWN,			//鼠标按下
	SLIDER_STATE_ON,			//鼠标悬浮
	SLIDER_STATE_DISABLE		//无效状态
};
class _XSliderTexture
{
private:
	_XBool m_isInited;
	void releaseTex();
public:
	_XTextureData *sliderLineNormal;			//滑动条线的普通状态
	_XTextureData *sliderLineDown;				//滑动条线的按下状态
	_XTextureData *sliderLineOn;				//滑动条线的悬浮状态
	_XTextureData *sliderLineDisable;			//滑动条线的无效状态
	_XTextureData *sliderButtonNormal;			//滑动条按钮的普通状态
	_XTextureData *sliderButtonDown;			//滑动条按钮的按下状态
	_XTextureData *sliderButtonOn;				//滑动条按钮的悬浮状态
	_XTextureData *sliderButtonDisable;			//滑动条按钮的无效状态

	_XRect m_mouseRect;			//鼠标的响应范围
	_XRect m_mouseRectButton;			//鼠标的响应范围
	_XVector2 m_fontPosition;	//放置文字的位置

	_XSliderTexture();
	~_XSliderTexture(){release();}
	_XBool init(const char *LNormal,const char *LDown,const char *LOn,const char *LDisable,
		const char *BNormal,const char *BDown,const char *BOn,const char *BDisable,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};
class _XMultiList;
class _XMultiListBasic;
class _XMultiText;
class _XDirectoryList;
class _XSliderEx;
class _XImageList;
class _XSlider:public _XControlBasic,public _XBasicOprate
{
	friend _XMultiList;
	friend _XMultiListBasic;
	friend _XMultiText;
	friend _XDirectoryList;
	friend _XSliderEx;
	friend _XImageList;
private:
	_XBool m_isInited;
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

	_XBool m_needFont;						//是否需要字体
	_XFontUnicode m_caption;				//字体
	char m_fontString[MAX_FILE_NAME_LENGTH];//格式化字符串
	_XVector2 m_textPosition;				//文字的相对位置
	_XVector2 m_textSize;					//文字的尺寸
	_XFColor m_textColor;					//文字的颜色
	//下面是关于关联变量的接口
	float *m_pVariable;	//关联的变量
public:
	void setConnectVar(float * p) {m_pVariable = p;}	//关联变量
	void disConnectVar() {m_pVariable = NULL;}			//取消变量关联
private:
	_XVector2 m_upMousePoint;		//上次鼠标在滚动条的位置
	_XVector2 m_mouseFlyArea;		//鼠标拖曳是飞出的响应范围
	_XVector2 m_nowButtonPosition;//当前滑块按钮的位置
	float m_minValue;			//滑动条的最大值
	float m_maxValue;			//滑动条的最小值
	float m_nowValue;			//滑动条的当前值
	float m_upValue;			//滑动条的上一次值(定义这个变量的目的在于如果拖动滑块超出范围之后可以恢复到拖动之前得知，但是目前出于简单的考虑，不做这个动作)
	_XBool m_dataChanged;			//滑动条的数值是否有被修改
	float m_keyOneValue;		//按下一次按键的滑动量(注意这个值比需要>=0,否则将会造成错误)
	void updateButtonData();	//根据实时数据的变化更新滑块的信息

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//没有贴图的形式

	void setTexture(const _XSliderTexture &tex);
public:
	_XBool init(const _XVector2& position,	//控件所在的位置
		const _XRect& Area,				//控件的鼠标响应范围
		const _XRect& buttonArea,		//滑动块的鼠标响应范围(这个数据实际上没有起到作用)
		const _XSliderTexture &tex,float max = 100.0f,float min = 0.0f,_XSliderType type = SLIDER_TYPE_HORIZONTAL);
	_XBool initEx(const _XVector2& position,	//对上面接口的简化
		const _XSliderTexture &tex,float max = 100.0f,float min = 0.0f,_XSliderType type = SLIDER_TYPE_HORIZONTAL);
	_XBool initPlus(const char * path,float max = 100.0f,float min = 0.0f,_XSliderType type = SLIDER_TYPE_HORIZONTAL,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	//int initWithoutTex(const _XRect& area,const _XRect& buttonArea,_XSliderType type,float max,float min,const _XVector2 &fontPosition);
	_XBool initWithoutTex(const _XRect& area,float max = 100.0f,float min = 0.0f,
		_XSliderType type = SLIDER_TYPE_HORIZONTAL,const _XVector2 &fontPosition = _XVector2::zero);
	_XBool initWithoutTex(const _XVector2& pixelSize,float max = 100.0f,float min = 0.0f,
		_XSliderType type = SLIDER_TYPE_HORIZONTAL,const _XVector2 &fontPosition = _XVector2::zero)
	{
		return initWithoutTex(_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),max,min,type,fontPosition);
	}

	_XBool setFont(const char *caption,const _XFontUnicode &font,float captionSize,const _XVector2 &fontPosition);
	_XBool setFontEx(const char *caption,const _XFontUnicode &font,float captionSize);

	using _XObjectBasic::setPosition;	//避免覆盖的问题
//	using _XControlBasic::getPosition;	//避免覆盖的问题
	void setPosition(float x,float y);		//设置滑动条的位置

	using _XObjectBasic::setSize;		//避免覆盖的问题
//	using _XControlBasic::getSize;	//避免覆盖的问题
	void setSize(float x,float y);				//设置滑动条的缩放比例

	void setTextColor(const _XFColor& color);	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;	//避免覆盖的问题
//	using _XControlBasic::getColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
protected:
	void draw();							//描绘滑动条
	void drawUp();
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return !(m_nowSliderState == SLIDER_STATE_DOWN);}
	void setLostFocus();	//设置失去焦点
public:
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funMouseOn)(void *,int),
		void (* funMouseDown)(void *,int),
		void (* funMouseUp)(void *,int),
		void (*funValueChange)(void *,int),
		void (*funMouseMove)(void *,int),
		void *pClass = NULL);
	void setDataChangeCB(void (*funValueChange)(void *,int),
		void (*funMouseMove)(void *,int),
		void *pClass = NULL);
	void setNowValue(float temp,bool cbFun = true);			//设置滑块当前的值,cbFun是否触发数值变化的回调函数
	void setRange(float max,float min);

	_XBool setACopy(const _XSlider &temp);			//建立一个副本

	_XSlider();
	~_XSlider(){release();}
	void release();
	//内联函数
	void disable();		//使控件无效
	void enable();		//使控件有效
	float getNowValue() const;	//获取滑块当前的值
	float getMaxValue() const; 
	float getMinValue() const;
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	//virtual void justForTest() {;}
private:	//为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	_XSlider(const _XSlider &temp);
	_XSlider& operator = (const _XSlider& temp);
public:
	virtual void setOprateState(void * data);
	virtual void *getOprateState() const;
	virtual bool isSameState(void * data);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对控件动态支持而定义的相关属性和方法
private:
	_XMoveData m_actionMoveData;	//动态效果的变量
	_XVector2 m_oldPos;				//动作播放时的位置
	_XVector2 m_oldSize;			//动作播放时的大小
	_XMoveData m_lightMD;
	_XRect m_lightRect;
protected:
	void update(int stepTime);
	//---------------------------------------------------------
};
#include "XSlider.inl"

#endif