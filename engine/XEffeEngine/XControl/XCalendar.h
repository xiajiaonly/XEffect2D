#ifndef _JIA_XCALENDAR_
#define _JIA_XCALENDAR_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.30
//--------------------------------
#include "XButton.h"
#include "XTimer.h"
//这是一个日历选择的类
//注意目前这个类的代码过于累赘，需要整体优化
//目前这个空间没有设计动作。
class _XCalendar:public _XControlBasic
{
private:
	_XBool m_isInited;
	_XRect m_rect;			//范围

	_XSystemTime m_todayDate;		//现在的日期
	_XSystemTime m_nowSetDate;		//当前设置的时间
	_XSystemTime m_nowShowData;		//当前显示的起始时间
	bool m_haveChoose;

	_XButton m_yearAddBtn;	//年加的button
	_XButton m_yearDecBtn;
	_XFontUnicode m_yearTxt;
	_XButton m_monthAddBtn;	//年加的button
	_XButton m_monthDecBtn;
	_XFontUnicode m_monthTxt;
	//_XFontUnicode m_titleTxt;
	_XFontUnicode m_titleFont[7];
	_XFontUnicode m_dateFont[42];
	friend void calendarCtrProc(void*,int);
	bool m_needShowToday;	//是否需要显示今天
	_XVector2 m_todayPos;	//今天的位置
	bool m_needShowChoose;	//是否需要显示选择的日期
	_XVector2 m_choosePos;	//选择日期的位置
public:
	_XBool initWithoutTex(const _XFontUnicode &font);
	_XBool initWithoutTex(){return initWithoutTex(XEE::systemFont);}
	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);
	using _XObjectBasic::setSize;	//避免覆盖的问题
	void setSize(float x,float y);

	using _XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	//下面是一些掉用接口
	_XSystemTime getTodayDate() const {return m_todayDate;}
	void setTodayDate(const _XSystemTime &t) {m_todayDate = t;}
	bool getHaveChoose() const{return m_haveChoose;}
	_XSystemTime getNowChooseDate() const {return m_nowSetDate;}
	void setNowChooseDate(const _XSystemTime &t) {m_nowSetDate = t;}
	_XSystemTime getNowShowDate() const {return m_nowShowData;}
	void setNowShowDate(const _XSystemTime &t) {m_nowShowData = t;}

protected:
	void updateNowDate();
	void draw();								//描绘按钮
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState){return XFalse;}			//返回是否触发按键动作
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);				//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点
public:
	_XCalendar()
		:m_isInited(XFalse)
		,m_haveChoose(false)
		,m_needShowToday(false)
		,m_needShowChoose(false)
	{
		m_ctrlType = CTRL_OBJ_CALENDAR;
	}
	~_XCalendar(){release();}
	void release();

	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	////virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	_XCalendar(const _XButton &temp);
	_XCalendar& operator = (const _XButton& temp);

};
#include "XCalendar.inl"
#endif