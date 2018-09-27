#ifndef _JIA_XSLIDER_
#define _JIA_XSLIDER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XButton.h"
#include "../XOprateDes.h"
#include "../XXml.h"
namespace XE{
enum XSliderType
{
	SLIDER_TYPE_VERTICAL,		//垂直的
	SLIDER_TYPE_HORIZONTAL		//水平的
};
enum XSliderState
{
	SLIDER_STATE_NORMAL,		//普通状态
	SLIDER_STATE_DOWN,			//鼠标按下
	SLIDER_STATE_ON,			//鼠标悬浮
	SLIDER_STATE_DISABLE		//无效状态
};
class XSliderSkin
{
private:
	XBool m_isInited;
	void releaseTex();
public:
	XTextureData *sliderLineNormal;			//滑动条线的普通状态
	XTextureData *sliderLineDown;				//滑动条线的按下状态
	XTextureData *sliderLineOn;				//滑动条线的悬浮状态
	XTextureData *sliderLineDisable;			//滑动条线的无效状态
	XTextureData *sliderButtonNormal;			//滑动条按钮的普通状态
	XTextureData *sliderButtonDown;			//滑动条按钮的按下状态
	XTextureData *sliderButtonOn;				//滑动条按钮的悬浮状态
	XTextureData *sliderButtonDisable;			//滑动条按钮的无效状态

	XRect m_mouseRect;			//鼠标的响应范围
	XRect m_mouseRectButton;			//鼠标的响应范围
	XVec2 m_fontPosition;	//放置文字的位置

	XSliderSkin();
	~XSliderSkin(){release();}
	XBool init(const char *LNormal,const char *LDown,const char *LOn,const char *LDisable,
		const char *BNormal,const char *BDown,const char *BOn,const char *BDisable,
		XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
private:
	bool loadFromFolder(const char *filename,XResPos resPos);	//从文件夹中载入资源
	bool loadFromPacker(const char *filename,XResPos resPos);	//从压缩包中载入资源
	bool loadFromWeb(const char *filename,XResPos resPos);		//从网页中读取资源
};
class XMultiList;
class XMultiListBasic;
class XMultiText;
class XDirectoryList;
class XSliderEx;
class XSliderInfinite;
class XImageList;
class XPropertyBox;
class XSubWindowEx;
class XSlider :public XControlBasic, public XBasicOprate
{
	friend XMultiList;
	friend XMultiListBasic;
	friend XMultiText;
	friend XDirectoryList;
	friend XSliderEx;
	friend XSliderInfinite;
	friend XImageList;
	friend XPropertyBox;
	friend XSubWindowEx;
private:
	XBool m_isInited;
	XSliderState m_curSliderState;			//滑动条的状态

	XSliderType m_typeVorH;						//滑动条的类型

	const XTextureData *m_sliderLineNormal;			//滑动条线的普通状态
	const XTextureData *m_sliderLineDown;			//滑动条线的按下状态
	const XTextureData *m_sliderLineOn;				//滑动条线的悬浮状态
	const XTextureData *m_sliderLineDisable;			//滑动条线的无效状态
	const XTextureData *m_sliderButtonNormal;		//滑动条按钮的普通状态
	const XTextureData *m_sliderButtonDown;			//滑动条按钮的按下状态
	const XTextureData *m_sliderButtonOn;			//滑动条按钮的悬浮状态
	const XTextureData *m_sliderButtonDisable;		//滑动条按钮的无效状态
public:
	enum XSliderEvent
	{
		SLD_INIT,
		SLD_RELEASE,
		SLD_MOUSE_ON,
		SLD_MOUSE_DOWN,
		SLD_MOUSE_UP,
		SLD_MOUSE_MOVE,
		SLD_VALUE_CHANGE,
	};
private:
	XRect m_buttonArea;		//滑动条中按钮的有效范围,这个范围是按钮的相对图片的响应范围，需要注意
	//XRect m_curButtonArea;		//当前鼠标的响应范围
	XSprite m_buttonSprite;	//用于显示贴图的精灵
	XSprite m_lineSprite;		//用于显示贴图的精灵

	XBool m_needFont;						//是否需要字体
	XFontUnicode m_caption;				//字体
	std::string m_fontString;//格式化字符串
	XVec2 m_textPosition;				//文字的相对位置
	XVec2 m_textSize;					//文字的尺寸
	XFColor m_textColor;					//文字的颜色
	//下面是关于关联变量的接口
	float *m_pVariable;	//关联的变量
	bool m_isInteger;	//数据是否为整数类型
public:
	void setIsInteger(bool flag) { m_isInteger = flag; }
	bool getIsInteger()const { return m_isInteger; }
	void setConnectVar(float* p)
	{
		m_pVariable = p;
		setCurValue(*p, true);
	}	//关联变量
	void disConnectVar() { m_pVariable = NULL; }			//取消变量关联
private:
	XVec2 m_upMousePoint;		//上次鼠标在滚动条的位置
	XVec2 m_mouseFlyArea;		//鼠标拖曳是飞出的响应范围
	XVec2 m_curButtonPosition;//当前滑块按钮的位置
	float m_minValue;			//滑动条的最大值
	float m_maxValue;			//滑动条的最小值
	float m_curValue;			//滑动条的当前值
	float m_upValue;			//滑动条的上一次值(定义这个变量的目的在于如果拖动滑块超出范围之后可以恢复到拖动之前得知，但是目前出于简单的考虑，不做这个动作)
	XBool m_dataChanged;			//滑动条的数值是否有被修改
	float m_keyOneValue;		//按下一次按键的滑动量(注意这个值比需要>=0,否则将会造成错误)
	void updateButtonData();	//根据实时数据的变化更新滑块的信息

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//没有贴图的形式

	void setTexture(const XSliderSkin &tex);
public:
	XBool init(const XVec2& position,	//控件所在的位置
		const XRect& Area,				//控件的鼠标响应范围
		const XRect& buttonArea,		//滑动块的鼠标响应范围(这个数据实际上没有起到作用)
		const XSliderSkin &tex, float maxValue = 100.0f, float minValue = 0.0f, XSliderType type = SLIDER_TYPE_HORIZONTAL);
	XBool initEx(const XVec2& position,	//对上面接口的简化
		const XSliderSkin &tex, float maxValue = 100.0f, float minValue = 0.0f, XSliderType type = SLIDER_TYPE_HORIZONTAL);
	XBool initPlus(const char* path, float maxValue = 100.0f, float minValue = 0.0f, XSliderType type = SLIDER_TYPE_HORIZONTAL,
		XResPos resPos = RES_SYS_DEF);
	//narrowBtn是否滑动块为窄边，窄边只有普通的一半大小
	//int initWithoutSkin(const XRect& area,const XRect& buttonArea,XSliderType type,float 
	//	maxValue,float minValue,const XVec2& fontPosition);
	XBool initWithoutSkin(const XRect& area, float maxValue = 100.0f, float minValue = 0.0f,
		XSliderType type = SLIDER_TYPE_HORIZONTAL, const XVec2& fontPosition = XVec2(0.0f, 16.0f),
		bool narrowBtn = false);
	XBool initWithoutSkin(const XVec2& pixelSize, float maxValue = 100.0f, float minValue = 0.0f,
		XSliderType type = SLIDER_TYPE_HORIZONTAL, const XVec2& fontPosition = XVec2(0.0f, 16.0f),
		bool narrowBtn = false)
	{
		return initWithoutSkin(XRect(XVec2::zero, pixelSize), maxValue, minValue, 
			type, fontPosition, narrowBtn);
	}
	XBool initWithoutSkin(float len, float maxValue = 100.0f, float minValue = 0.0f,
		XSliderType type = SLIDER_TYPE_HORIZONTAL, const XVec2& fontPosition = XVec2(0.0f, 16.0f),
		bool narrowBtn = false)
	{
		return initWithoutSkin(XRect(XVec2::zero, XVec2(len,32.0f)), maxValue, minValue,
			type, fontPosition, narrowBtn);
	}

	XBool setFont(const char* caption, const XFontUnicode& font, float captionSize = 1.0f, const XVec2& fontPosition = XVec2(0.0f, 16.0f));
	XBool setFont(const char* caption, const XVec2& fontPosition)
	{
		return setFont(caption, getDefaultFont(), 1.0f, fontPosition);
	}
	XBool setFont(const char *caption)
	{
		return setFont(caption, getDefaultFont(), 1.0f, m_textPosition);
	}

	using XObjectBasic::setPosition;	//避免覆盖的问题
//	using XControlBasic::getPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);		//设置滑动条的位置

	using XObjectBasic::setScale;		//避免覆盖的问题
//	using XControlBasic::getSize;	//避免覆盖的问题
	void setScale(const XVec2& s);				//设置滑动条的缩放比例

	void setTextColor(const XFColor& color);	//设置字体的颜色
	const XFColor& getTextColor() const { return m_textColor; }	//获取控件字体的颜色

	using XObjectBasic::setColor;	//避免覆盖的问题
//	using XControlBasic::getColor;	//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);
protected:
	void draw();							//描绘滑动条
	void drawUp();
	XBool mouseProc(const XVec2& p, XMouseState mouseState);	//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder, XKeyState keyState);
	void insertChar(const char*, int) { ; }
	XBool canGetFocus(const XVec2& p);	//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&) { return !(m_curSliderState == SLIDER_STATE_DOWN); }
	void setLostFocus();	//设置失去焦点
	float getMapValue(float v);
public:
	void setCurValue(float temp, bool withEvent = false);			//设置滑块当前的值,cbFun是否触发数值变化的回调函数
	void setRange(float maxValue, float minValue, bool withEvent = true);

	XBool setACopy(const XSlider &temp);			//建立一个副本

	XSlider();
	virtual ~XSlider()
	{
		release();
		if (gFrameworkData.pOperateManager != NULL)
			gFrameworkData.pOperateManager->decreaseObj(this);
	}
	void release();
	//内联函数
	void disable();		//使控件无效
	void enable();		//使控件有效
	float getCurValue() const;	//获取滑块当前的值
	float getMaxValue() const;
	float getMinValue() const;
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	//virtual void justForTest() {;}
private:	//为了防止意外调用造成的错误，这里重载赋值操作符和赋值构造函数
	XSlider(const XSlider &temp);
	XSlider& operator = (const XSlider& temp);
public:
	virtual void setOprateState(void * data);
	virtual void *getOprateState() const;
	virtual void releaseOprateStateData(void *p);
	virtual void stateChange()
	{
		if (m_withUndoAndRedo) XOpManager.addAOperate(this);	//如果需要记录当前状态则将调用动作管理器的相关函数
		if (m_funStateChange != NULL) m_funStateChange(m_pStateClass);	//调用相关的回调函数
	}
	virtual bool isSameState(void * data);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对控件动态支持而定义的相关属性和方法
private:
	XMoveData m_actionMoveData;	//动态效果的变量
	XVec2 m_oldPos;				//动作播放时的位置
	XVec2 m_oldSize;			//动作播放时的大小
	XMoveData m_lightMD;
	XRect m_lightRect;
protected:
	void update(float stepTime);
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(完成)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if (!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		if (!XXml::addLeafNode(e, m_ctrlName.c_str(), XString::toString(getCurValue()))) return XFalse;
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if (!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		float tmp;
		if (XXml::getXmlAsFloat(e, m_ctrlName.c_str(), tmp) == NULL) return XFalse;
		setCurValue(tmp, true);
		return XTrue;
	}
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XSlider.inl"
#endif
}
#endif