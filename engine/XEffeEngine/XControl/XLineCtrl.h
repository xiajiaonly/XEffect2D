#ifndef _JIA_XLINECTRL_
#define _JIA_XLINECTRL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
//这是一个基本原控件线
#include "XControlBasic.h"
#include "../XXml.h"
namespace XE{
class XLineCtrl:public XControlBasic
{
public:
	enum XLineCtrlType
	{
		LINE_CTRL_TYPE_X_D,
		LINE_CTRL_TYPE_X_U,
		LINE_CTRL_TYPE_Y_R,
		LINE_CTRL_TYPE_Y_L,
	};
private:
	static const int m_lineCtrlLineSum = 10;
//	XVec2 m_position;	//控件的位置
//	XVec2 m_scale;		//大小
	XVec2 m_pixelSize;	//像素大小
	XVec2 m_truePixelSize;	//真实的像素尺寸
	XFontUnicode m_font;
	char m_textStr[64];		//显示的字符串

	XLineCtrlType m_type;
	XBool m_isInited;		//是否初始化
	bool m_withFont;
	bool m_isDown;		//是否被鼠标拾取
	bool m_withRange;	//范围设置是否有效
	bool m_withMap;		//是否进行映射
	bool m_withString;			//是否有字符串
	XVec2 m_range;	//可以移动的范围
	XVec2 m_mapRange;	//映射的范围
	float m_mapValue;
	std::string m_showString;		//需要显示的字符串
	XFontUnicode m_stringFont;	//字符串的字体

	void updateData();
public:
	enum XLineCtrlEvent
	{
		LNCTRL_DATA_CHANGE,
	};
//	void (*m_funDataChange)(void *,int ID);
//	void *m_pClass;	

private:
	XCtrlMode m_ctrlMode;		//控件的模式
public:
	void setString(const char * str);

	void setCtrlMode(XCtrlMode mode){m_ctrlMode = mode;}
	void setRange(float minRange, float maxRange);
	void disRange(){m_withRange = false;}
	float getValue();
	void setValue(float value);
	void setMapRange(float min,float max);
	void disMapRange(){m_withMap = false;}
public:
	XBool init(const XVec2& position,XLineCtrlType type = LINE_CTRL_TYPE_X_D,const XFontUnicode *font = NULL);
	void draw();
	void drawUp(){;}
//	void setCallbackFun(void (* funDataChange)(void *,int),void *pClass = NULL);

	XBool mouseProc(const XVec2& p,XMouseState mouseState);
	XBool keyboardProc(int,XKeyState){return XTrue;}		//do nothing
	XBool canGetFocus(const XVec2& p);	//do nothing	//事件可以穿透，如果返回XTrue则鼠标事件不能穿透
	XBool canLostFocus(const XVec2&){return XTrue;}	//do nothing

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//避免覆盖的问题
	void setScale(const XVec2& s);

	using XObjectBasic::setColor;		//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);

	void insertChar(const char *,int){;}			//do nothing

	//void setLostFocus() {m_isBeChoose = XFalse;}							//do nothing
	void disable();	
	void enable(){m_isEnable = XTrue;}
	void release();

	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	XBool setACopy(const XLineCtrl & temp);

	//virtual void justForTest() {;}

	XLineCtrl()
		:m_isInited(XFalse)
	//	,m_position(0.0f)
	//	,m_scale(1.0f)
		,m_withFont(false)
		,m_pixelSize(10.0f)
		,m_isDown(false)
		,m_type(LINE_CTRL_TYPE_X_D)
		,m_range(0.0f,100.0f)
		,m_withRange(false)
		,m_withMap(false)		//是否进行映射
		,m_mapRange(0.0f,100.0f)	//映射的范围
		,m_mapValue(0.0f)
//		,m_funDataChange(NULL)
//		,m_pClass(NULL)
		,m_ctrlMode(CTRL_MODE_SIMPLE)
		,m_withString(false)
	{
		m_ctrlType = CTRL_OBJ_LINECTRL;
	}
	~XLineCtrl(){release();}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(完成)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		if(!XXml::addLeafNode(e,m_ctrlName.c_str(),XString::toString(getValue()))) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		float tmp;
		if(XXml::getXmlAsFloat(e,m_ctrlName.c_str(),tmp) == NULL) return XFalse;
		setValue(tmp);
		return XTrue;
	}
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XLineCtrl.inl"
#endif
}
#endif