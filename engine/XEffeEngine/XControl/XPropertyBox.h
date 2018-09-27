#ifndef _JIA_XPROPERTYBOX_
#define _JIA_XPROPERTYBOX_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
#include "XSlider.h"
#include "XText.h"
#include "XEdit.h"
#include "XCheck.h"
#include "XCombo.h"
#include "XCtrlManagerBase.h"
namespace XE{
//属性框
enum XPropertyLineType
{// 属性行的类型
	PROPERTY_BLANK,	//空白行，空行，什么都没有
	PROPERTY_TITLE,	//只显示名字的行
	PROPERTY_EDIT,	//输入框
	PROPERTY_CHECK,	//选择项
	PROPERTY_COMBO,	//选项
};
class XPropertyLine
{//属性行
public:
	std::string name;	//属性行的名称
	XPropertyLineType type;
	XFontUnicode *pNameFont;
	XControlBasic *pCtrl;

	XBool getIsFocus();
	void setFocus(bool flag);
	XBool canGetFocus(const XVec2& p);	//是否可以获得焦点
	void draw();
	void disable();
	void enable();
	XPropertyLine()
		:type(PROPERTY_BLANK)
		,pNameFont(NULL)
		,pCtrl(NULL)
	{}
	void setAlpha(float a);
	void setColor(const XFColor& c);
};
class XPropertyBox :public XControlBasic
{
private:
	XBool m_isInited;		//是否已经初始化

	XBool m_haveTitle;		//是否拥有标题
	std::string m_titleStr;	//标题名称
	XText m_titleTxt;		//用于显示标题的字体
	XFontUnicode m_font;	//使用到的字体

	float m_width;		//总的宽度
	float m_nameWidth;	//属性名称的宽度，默认是对半开
	int m_maxLineSum;	//显示的最多属性行数

	XBool m_needShowSlider;	//是否需要显示滑动条
	XSlider m_slider;			//右侧的滑动条
	int m_curStartLineIndex;	//当前的起始行编号

	std::vector<XPropertyLine> m_propertyLines;	//属性行

	void setCurStartLineIndex(unsigned int index);//设置当前起始行的编号
public:
	void setTitle(const char * title);	//设置标题
	void disTitle();					//取消标题
	XBool initWithoutSkin(int width, int maxLineSum);
	void setNameWidth(int width);

protected:
	void draw();								//描绘按钮
	void drawUp();
	XBool mouseProc(const XVec2& p, XMouseState mouseState);		//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder, XKeyState keyState);			//返回是否触发按键动作
	void insertChar(const char *ch, int len) { m_ctrlManager.insertChar(ch, len); }
	XBool canGetFocus(const XVec2& p);				//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&) { return XTrue; }	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点
public:	//向属性框中增加一个属性项
	XBool addAPropertyLine(const char * name, XPropertyLineType type, int menuSum = 0, int showMenuSum = 0);
	XBool setPropertyName(unsigned int index, const char * name);
	const std::string& getPropertyName(unsigned int index)const;
	XBool setPropertyCtrlPrama(unsigned int index, int prama);
	XBool setPropertyStr(unsigned int index, const char * str);
	std::string getPropertyStr(unsigned int index)const;
	XBool setPropertyValue(unsigned int index, float value);		//尚未实现
	float getPropertyValue(unsigned int index);		//尚未实现
	XBool setPropertyIndex(unsigned int index, int order);
	int getPropertyIndex(unsigned int index)const;
	XBool setPropertyState(unsigned int index, XBool state);
	XBool getPropertyState(unsigned int index)const;
	XBool setPropertyMenuStr(unsigned int index, int menuIndex, const char *str);
	XBool setPropertyMenuStr(unsigned int index, const char *str);
	void updateSliderState();	//更新滑动条的状态

	XPropertyBox()
		:m_isInited(XFalse)
		, m_haveTitle(XFalse)
		, m_curStartLineIndex(0)
	{
		m_ctrlType = CTRL_OBJ_PROPERTYBOX;
	}
	~XPropertyBox() { release(); }
	void release();

	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(const XVec2& s);

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);

	static void ctrlProc(void*, int, int);

	void disable();
	void enable();

	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	////virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XPropertyBox(const XPropertyBox &temp);
	XPropertyBox& operator = (const XPropertyBox& temp);
protected:
	void update(float stepTime);
	void updateRect();
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(尚未完成)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if (!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		//遍历所有的控件，保存其状态
		TiXmlElement elmNode(m_ctrlName.c_str());
		XBool flag = XTrue;
		for (unsigned int i = 0; i < m_propertyLines.size(); ++i)
		{
			if (m_propertyLines[i].pCtrl != NULL &&
				!m_propertyLines[i].pCtrl->saveState(elmNode)) flag = XFalse;
		}
		if (e.InsertEndChild(elmNode) == NULL) flag = XFalse;
		return flag;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if (!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		//遍历所有的空间，读取其状态
		TiXmlNode * keyNode = e->FirstChildElement(m_ctrlName.c_str());
		if (keyNode == NULL) return XFalse;
		XBool flag = XTrue;
		for (unsigned int i = 0; i < m_propertyLines.size(); ++i)
		{
			if (m_propertyLines[i].pCtrl != NULL &&
				!m_propertyLines[i].pCtrl->loadState(keyNode)) flag = XFalse;
		}
		return flag;
	}
	//---------------------------------------------------------
private:
	XCtrlManagerBase m_ctrlManager;	//加入一个控件管理器
};
enum XSerialInfoPpb
{
	S_PPB_PORT,
	S_PPB_BITRATE,
	S_PPB_MODE,
	S_PPB_WITHEVENT,
	S_PPB_WITHOLP,
	S_PPB_DATABIT,
	S_PPB_STOPBIT,
	S_PPB_DTR,
	S_PPB_RTS,
	S_PPB_WAIT_RECV,
};
extern void setAsSerialPortInfo(XPropertyBox &pb, const char* title);
struct XSerialPortInfo;
extern bool getSerialPortInfo(const XPropertyBox& pb, XSerialPortInfo& tmpInfo);
extern void setSerialPortInfo(XPropertyBox& pb, const XSerialPortInfo& tmpInfo);

#if WITH_INLINE_FILE
#include "XPropertyBox.inl"
#endif
}
#endif