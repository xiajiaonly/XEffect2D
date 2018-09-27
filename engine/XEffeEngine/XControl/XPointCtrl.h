#ifndef _JIA_XPOINTCTRL_
#define _JIA_XPOINTCTRL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XControlBasic.h"
#include "../XXml.h"
#include "XMath\XPressDataMode.h"
#define XPCTRL_BY_KEYMOVE	//是否可以通过键盘连续移动
namespace XE{
enum XPointCtrlStrMode
{
	PCTRL_MODE_RB,	//文字再右下角
	PCTRL_MODE_RT,	//文字再右上角
	PCTRL_MODE_LB,	//文字再左下角
	PCTRL_MODE_LT,	//文字再左上角
};
class XPointCtrl:public XControlBasic
{
private:
	static const int m_pointCtrlLineSum = 10;//这是一个基本原控件点，用于响应鼠标的位置操作
//	XVec2 m_position;	//控件的位置
//	XVec2 m_scale;		//大小
	XVec2 m_pixelSize;	//像素大小
	XVec2 m_truePixelSize;	//真实的像素尺寸
	XFontUnicode m_font;
	char m_textStr[64];		//显示的字符串

	XBool m_isInited;		//是否初始化
	bool m_withFont;
	bool m_isDown;		//是否被鼠标拾取
	bool m_withRange;	//范围设置是否有效
	bool m_withMap;		//是否进行映射
	bool m_autoString;	//自动更新显示的文字
	XRect m_range;		//可以移动的范围
	XRect m_mapRange;	//映射的范围
	XVec2 m_mapValue;
#ifdef XPCTRL_BY_KEYMOVE
	XKeyPressModel m_moveKeyBoard[4];	//通过鼠标按键来移动
#endif
	void updateData();
public:
	enum XPointCtrlEvent
	{
		PITCTRL_DATA_CHANGE,
	};
private:
	XCtrlMode m_ctrlMode;		//控件的模式
	XPointCtrlStrMode m_PCtrlMode;
	bool m_withFitRect;
	XRect m_fitRect;
public:
	//保存文字再rect指定的框内
	void setFitRect(const XRect& r)
	{
		m_withFitRect = true;
		m_fitRect = r;
		fitMode(m_fitRect);
	}
	void disFitRect() { m_withFitRect = false; }
	void fitMode(const XRect& rect);
	XPointCtrlStrMode getPCtrlMode()const { return m_PCtrlMode; }
	bool setPCtrlMode(XPointCtrlStrMode mode);
	void setCtrlMode(XCtrlMode mode){m_ctrlMode = mode;}
	void setRange(float l,float t,float r,float b);
	void disRange(){m_withRange = false;}
	const XVec2& getValue()const;
	void setValue(const XVec2& value);
	void setMapRange(float l,float t,float r,float b);
	void disMapRange(){m_withMap = false;}
public:
	XBool init(const XVec2& position,const XFontUnicode *font = NULL);
	void draw();
	void drawUp(){;}
	virtual void update(float steptime);

	XBool mouseProc(const XVec2& p,XMouseState mouseState);
	XBool keyboardProc(int,XKeyState);		//do nothing
	XBool canGetFocus(const XVec2& p);	//do nothing	//事件可以穿透，如果返回XTrue则鼠标事件不能穿透
	XBool canLostFocus(const XVec2&){return XTrue;}	//do nothing
	virtual void setLostFocus() //设置失去焦点
	{ 
		m_isDown = false;
		m_isBeChoose = XFalse; 
	}

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(const XVec2& s);

	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);

	void insertChar(const char *,int){;}			//do nothing

//	void setLostFocus() {m_isBeChoose = XFalse;}							//do nothing
	void disable();	
	void enable(){m_isEnable = XTrue;}
	void release();

	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	XBool setACopy(const XPointCtrl & temp);

	XBool setFontString(const char *str);

	//virtual void justForTest() {;}

	XPointCtrl()
		:m_isInited(XFalse)
		, m_withFont(false)
		, m_pixelSize(10.0f)
		, m_isDown(false)
		, m_range(0.0f, 0.0f, 100.0f, 100.0f)
		, m_withRange(false)
		, m_mapRange(0.0f, 0.0f, 100.0f, 100.0f)
		, m_withMap(false)
		, m_mapValue(0.0f)
//		,m_funDataChange(NULL)
//		,m_pClass(NULL)
		, m_ctrlMode(CTRL_MODE_SIMPLE)
		, m_autoString(true)
		, m_withFitRect(false)
	{
		m_ctrlType = CTRL_OBJ_POINTCTRL;
	}
	~XPointCtrl() {release();}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(完成)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		if(!XXml::addLeafNode(e,(m_ctrlName + "X").c_str(),XString::toString(getValue().x))) return XFalse; 
		if(!XXml::addLeafNode(e,(m_ctrlName + "Y").c_str(),XString::toString(getValue().y))) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		XVec2 tmp;
		if(XXml::getXmlAsFloat(e,(m_ctrlName + "X").c_str(),tmp.x) == NULL) return XFalse;
		if(XXml::getXmlAsFloat(e,(m_ctrlName + "Y").c_str(),tmp.y) == NULL) return XFalse;
		setValue(tmp);
		return XTrue;
	}
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XPointCtrl.inl"
#endif
}
#endif