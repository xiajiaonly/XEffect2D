#ifndef _JIA_XCALENDAR_
#define _JIA_XCALENDAR_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.30
//--------------------------------
#include "XButton.h"
#include "XTimer.h"
#include "../XXml.h"
namespace XE{
//这是一个日历选择的类
//注意目前这个类的代码过于累赘，需要整体优化
//目前这个空间没有设计动作。
class XCalendar:public XControlBasic
{
private:
	XBool m_isInited;
	bool m_haveChoose;
	bool m_needShowToday;	//是否需要显示今天
	bool m_needShowChoose;	//是否需要显示选择的日期
	XVector2 m_todayPos;	//今天的位置
	XVector2 m_choosePos;	//选择日期的位置
	XRect m_rect;			//范围

	XSystemTime m_todayDate;		//现在的日期
	XSystemTime m_curSetDate;		//当前设置的时间
	XSystemTime m_curShowData;		//当前显示的起始时间

	XButton m_yearAddBtn;	//年加的button
	XButton m_yearDecBtn;
	XFontUnicode m_yearTxt;
	XButton m_monthAddBtn;	//年加的button
	XButton m_monthDecBtn;
	XFontUnicode m_monthTxt;
	//XFontUnicode m_titleTxt;
	XFontUnicode m_titleFont[7];
	XFontUnicode m_dateFont[42];
	static void ctrlProc(void*,int,int);
public:
	XBool initWithoutSkin(const XFontUnicode &font);
	XBool initWithoutSkin(){return initWithoutSkin(getDefaultFont());}
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);
	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(float x,float y);

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	//下面是一些调用接口
	XSystemTime getTodayDate() const {return m_todayDate;}
	void setTodayDate(const XSystemTime &t) {m_todayDate = t;}
	bool getHaveChoose() const{return m_haveChoose;}
	XSystemTime getCurChooseDate() const {return m_curSetDate;}
	void setCurChooseDate(const XSystemTime &t) {m_curSetDate = t;}
	XSystemTime getCurShowDate() const {return m_curShowData;}
	void setCurShowDate(const XSystemTime &t) {m_curShowData = t;}

protected:
	void updateCurDate();
	void draw();								//描绘按钮
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(float x,float y,XMouseState mouseState);		//对于鼠标动作的响应函数
	XBool keyboardProc(int,XKeyState){return XFalse;}			//返回是否触发按键动作
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);				//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(float,float){return XTrue;}	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点
public:
	XCalendar()
		:m_isInited(XFalse)
		,m_haveChoose(false)
		,m_needShowToday(false)
		,m_needShowChoose(false)
	{
		m_ctrlType = CTRL_OBJ_CALENDAR;
	}
	~XCalendar(){release();}
	void release();

	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	////virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XCalendar(const XButton &temp);
	XCalendar& operator = (const XButton& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存(完成)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		XSystemTime tmp = getCurChooseDate();
		if(!XXml::addLeafNode(e,(m_ctrlName + "Year").c_str(),XString::toString(tmp.year))) return XFalse; 
		if(!XXml::addLeafNode(e,(m_ctrlName + "Month").c_str(),XString::toString(tmp.month))) return XFalse; 
		if(!XXml::addLeafNode(e,(m_ctrlName + "Day").c_str(),XString::toString(tmp.day))) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		XSystemTime tmp;
		if(XXml::getXmlAsInt(e,(m_ctrlName + "Year").c_str(),tmp.year) == NULL) return XFalse;
		if(XXml::getXmlAsInt(e,(m_ctrlName + "Month").c_str(),tmp.month) == NULL) return XFalse;
		if(XXml::getXmlAsInt(e,(m_ctrlName + "Day").c_str(),tmp.day) == NULL) return XFalse;
		setCurChooseDate(tmp);
		return XTrue;
	}
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XCalendar.inl"
#endif
}
#endif