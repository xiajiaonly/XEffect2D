#include "XCalendar.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

void calendarCtrProc(void*pClass,int ID)
{
	_XCalendar &pPar = *(_XCalendar *)pClass;
	if(ID == pPar.m_yearAddBtn.getControlID())
	{
		++pPar.m_nowShowData.year;
		pPar.m_yearTxt.setString(toString(pPar.m_nowShowData.year).c_str());
		pPar.updateNowDate();
	}else
	if(ID == pPar.m_yearDecBtn.getControlID())
	{
		--pPar.m_nowShowData.year;
		pPar.m_yearTxt.setString(toString(pPar.m_nowShowData.year).c_str());
		pPar.updateNowDate();
	}else
	if(ID == pPar.m_monthAddBtn.getControlID())
	{
		++pPar.m_nowShowData.month;
		if(pPar.m_nowShowData.month > 12)
		{
			pPar.m_nowShowData.month = 1;
			++pPar.m_nowShowData.year;
			pPar.m_yearTxt.setString(toString(pPar.m_nowShowData.year).c_str());
		}
		pPar.m_monthTxt.setString(toString(pPar.m_nowShowData.month).c_str());
		pPar.updateNowDate();
	}else
	if(ID == pPar.m_monthDecBtn.getControlID())
	{
		--pPar.m_nowShowData.month;
		if(pPar.m_nowShowData.month <= 0)
		{
			pPar.m_nowShowData.month = 12;
			--pPar.m_nowShowData.year;
			pPar.m_yearTxt.setString(toString(pPar.m_nowShowData.year).c_str());
		}
		pPar.m_monthTxt.setString(toString(pPar.m_nowShowData.month).c_str());
		pPar.updateNowDate();
	}
}
_XBool _XCalendar::initWithoutTex(const _XFontUnicode &font)
{
	if(m_isInited) return XFalse;
	m_position.set(0.0f,0.0f);
	m_size.set(1.0f,1.0f);
	m_rect.set(0.0f,0.0f,350.0f,280.0f);
	getTimeMs(m_todayDate);	//获取现在的时间
	m_nowShowData.year = m_todayDate.year;
	m_nowShowData.month = m_todayDate.month;
	m_nowShowData.day = 1;
	m_yearAddBtn.initWithoutTex("<<",font,_XRect(0.0f,0.0f,32.0f,32.0f));
	m_yearAddBtn.setPosition(m_position.x + 10.0f * m_size.x,m_position.y + 2.0f * m_size.y);
	m_yearAddBtn.setSize(m_size);
	m_yearAddBtn.setMouseDownCB(calendarCtrProc,this);
	m_yearAddBtn.setWithAction(XFalse);
	m_yearDecBtn.initWithoutTex(">>",font,_XRect(0.0f,0.0f,32.0f,32.0f));
	m_yearDecBtn.setPosition(m_position.x + 112.0f * m_size.x,m_position.y + 2.0f * m_size.y);
	m_yearDecBtn.setSize(m_size);
	m_yearDecBtn.setMouseDownCB(calendarCtrProc,this);
	m_yearDecBtn.setWithAction(XFalse);
	m_yearTxt.setACopy(font);
	m_yearTxt.setColor(0.0f,0.0f,0.0f,1.0f);
	m_yearTxt.setPosition(m_position + _XVector2(77.0f * m_size.x,18.0f * m_size.y));
	m_yearTxt.setSize(m_size);
	m_yearTxt.setString(toString(m_todayDate.year).c_str());
	//将这些物件从物件管理器中注销掉
	_XCtrlManger.decreaseAObject(&m_yearAddBtn);	//注销这个物件
	_XCtrlManger.decreaseAObject(&m_yearDecBtn);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_yearAddBtn);
	_XObjManger.decreaseAObject(&m_yearDecBtn);
	_XObjManger.decreaseAObject(&m_yearTxt);
#endif

	m_monthAddBtn.initWithoutTex("<<",font,_XRect(0.0f,0.0f,32.0f,32.0f));
	m_monthAddBtn.setPosition(m_position.x + 240.0f * m_size.x,m_position.y + 2.0f * m_size.y);
	m_monthAddBtn.setSize(m_size);
	m_monthAddBtn.setMouseDownCB(calendarCtrProc,this);
	m_monthAddBtn.setWithAction(XFalse);
	m_monthDecBtn.initWithoutTex(">>",font,_XRect(0.0f,0.0f,32.0f,32.0f));
	m_monthDecBtn.setPosition(m_position.x + 312.0f * m_size.x,m_position.y + 2.0f * m_size.y);
	m_monthDecBtn.setSize(m_size);
	m_monthDecBtn.setMouseDownCB(calendarCtrProc,this);
	m_monthDecBtn.setWithAction(XFalse);
	m_monthTxt.setACopy(font);
	m_monthTxt.setColor(0.0f,0.0f,0.0f,1.0f);
	m_monthTxt.setPosition(m_position + _XVector2(292.0f * m_size.x,18.0f * m_size.y));
	m_monthTxt.setSize(m_size);
	m_monthTxt.setString(toString(m_todayDate.month).c_str());
	//将这些物件从物件管理器中注销掉
	_XCtrlManger.decreaseAObject(&m_monthAddBtn);	//注销这个物件
	_XCtrlManger.decreaseAObject(&m_monthDecBtn);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_monthAddBtn);
	_XObjManger.decreaseAObject(&m_monthDecBtn);
	_XObjManger.decreaseAObject(&m_monthTxt);
#endif

	m_nowMouseRect.set(m_position.x + m_rect.left * m_size.x,m_position.y + m_rect.top * m_size.y,
		m_position.x + m_rect.right * m_size.x,m_position.y + m_rect.bottom * m_size.y);

	std::string tmpTitle[]={"日","一","二","三","四","五","六"};
	for(int i = 0;i < 7;++ i)
	{
		m_titleFont[i].setACopy(font);
		if(i == 0) m_titleFont[i].setColor(1.0f,0.0f,0.0f,1.0f);
		else m_titleFont[i].setColor(0.0f,0.0f,0.0f,1.0f);
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_titleFont[i]);
#endif
		m_titleFont[i].setPosition(m_position + _XVector2((25.0f + i * 50.0f) * m_size.x,53.0f * m_size.y));
		m_titleFont[i].setSize(m_size);
		m_titleFont[i].setString(tmpTitle[i].c_str());
	}
	for(int i = 0;i < 6;++ i)
	{
		for(int j = 0;j < 7;++ j)
		{
			_XFontUnicode &tmpFont = m_dateFont[i * 7 + j];
			tmpFont.setACopy(font);
#if WITH_OBJECT_MANAGER
			_XObjManger.decreaseAObject(&tmpFont);
#endif
			tmpFont.setPosition(m_position + _XVector2((25.0f + j * 50.0f) * m_size.x,(88.0f + 35.0f * i) * m_size.y));
			tmpFont.setSize(m_size);
		}
	}
	updateNowDate();

	m_isInited = XTrue;
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif

	return XTrue;
}
void _XCalendar::updateNowDate()
{
	int week = getDataWeek(m_nowShowData);
	int day = getDateDaysInYear(m_nowShowData) - week;
	_XSystemTime tmpT;
	m_needShowToday = false;
	m_needShowChoose = false;
	for(int i = 0;i < 6;++ i)
	{
		for(int j = 0;j < 7;++ j)
		{
			_XFontUnicode &tmpFont = m_dateFont[i * 7 + j];
			if(j == 0) tmpFont.setColor(1.0f,0.0f,0.0f,1.0f);
			else tmpFont.setColor(0.0f,0.0f,0.0f,1.0f);
			getDateData(m_nowShowData.year,day + i * 7 + j,tmpT,week);
			if(tmpT.month != m_nowShowData.month) 
			{
				if(j == 0) tmpFont.setColor(1.0f,0.5f,0.5f,1.0f);
				else tmpFont.setColor(0.5f,0.5f,0.5f,1.0f);
			}
			tmpFont.setString(toString(tmpT.day).c_str());
			if(tmpT.year == m_todayDate.year && tmpT.month == m_todayDate.month
				&& tmpT.day == m_todayDate.day)
			{//今天显示一个底色
				m_needShowToday = true;
				m_todayPos.set(50.0f * j,35.0f * i + 70.0f);
			//	drawFillBoxEx(_XVector2(m_nowMouseRect.left + 50.0f * j * m_size.x,
			//		m_nowMouseRect.top + (35.0f * i + 70.0f) * m_size.y),
			//		_XVector2(50 * m_size.x,35 * m_size.y),0.5f,0.5f,1.0f,true);//,true); 
			}
			if(m_haveChoose && tmpT.year == m_nowSetDate.year && 
				tmpT.month == m_nowSetDate.month && tmpT.day == m_nowSetDate.day)
			{//与选择的日期一致
				m_needShowChoose = true;
				m_choosePos.set(50.0f * j,35.0f * i + 70.0f);
			//	drawFillBoxEx(_XVector2(m_nowMouseRect.left + 50.0f * j * m_size.x,
			//		m_nowMouseRect.top + (35.0f * i + 70.0f) * m_size.y),
			//		_XVector2(50 * m_size.x,35 * m_size.y),0.5f,0.5f,0.5f,true);//,true); 
			}

		}
	}
}
void _XCalendar::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_yearAddBtn.setPosition(m_position.x + 10.0f * m_size.x,m_position.y + 2.0f * m_size.y);
	m_yearDecBtn.setPosition(m_position.x + 112.0f * m_size.x,m_position.y + 2.0f * m_size.y);
	m_yearTxt.setPosition(m_position + _XVector2(77.0f * m_size.x,18.0f * m_size.y));
	m_monthAddBtn.setPosition(m_position.x + 240.0f * m_size.x,m_position.y + 2.0f * m_size.y);
	m_monthDecBtn.setPosition(m_position.x + 312.0f * m_size.x,m_position.y + 2.0f * m_size.y);
	m_monthTxt.setPosition(m_position + _XVector2(292.0f * m_size.x,18.0f * m_size.y));
	m_nowMouseRect.set(m_position.x + m_rect.left * m_size.x,m_position.y + m_rect.top * m_size.y,
		m_position.x + m_rect.right * m_size.x,m_position.y + m_rect.bottom * m_size.y);
	for(int i = 0;i < 7;++ i)
	{
		m_titleFont[i].setPosition(m_position + _XVector2((25.0f + i * 50.0f) * m_size.x,53.0f * m_size.y));
	}
	for(int i = 0;i < 6;++ i)
	{
		for(int j = 0;j < 7;++ j)
		{
			_XFontUnicode &tmpFont = m_dateFont[i * 7 + j];
			tmpFont.setPosition(m_position + _XVector2((25.0f + j * 50.0f) * m_size.x,(88.0f + 35.0f * i) * m_size.y));
		}
	}
}
void _XCalendar::setSize(float x,float y)
{
	if(x < 0.0f || y < 0.0f) return;
	if(x == m_size.x && y == m_size.y) return;
	m_size.set(x,y);
	m_yearAddBtn.setPosition(m_position.x + 10.0f * m_size.x,m_position.y + 2.0f * m_size.y);
	m_yearAddBtn.setSize(m_size);
	m_yearDecBtn.setPosition(m_position.x + 112.0f * m_size.x,m_position.y + 2.0f * m_size.y);
	m_yearDecBtn.setSize(m_size);
	m_yearTxt.setPosition(m_position + _XVector2(77.0f * m_size.x,18.0f * m_size.y));
	m_yearTxt.setSize(m_size);
	m_monthAddBtn.setPosition(m_position.x + 240.0f * m_size.x,m_position.y + 2.0f * m_size.y);
	m_monthAddBtn.setSize(m_size);
	m_monthDecBtn.setPosition(m_position.x + 312.0f * m_size.x,m_position.y + 2.0f * m_size.y);
	m_monthDecBtn.setSize(m_size);
	m_monthTxt.setPosition(m_position + _XVector2(292.0f * m_size.x,18.0f * m_size.y));
	m_monthTxt.setSize(m_size);
//	m_titleTxt.setSize(m_size);
	m_nowMouseRect.set(m_position.x + m_rect.left * m_size.x,m_position.y + m_rect.top * m_size.y,
		m_position.x + m_rect.right * m_size.x,m_position.y + m_rect.bottom * m_size.y);
	for(int i = 0;i < 7;++ i)
	{
		m_titleFont[i].setPosition(m_position + _XVector2((25.0f + i * 50.0f) * m_size.x,53.0f * m_size.y));
		m_titleFont[i].setSize(m_size);
	}
	for(int i = 0;i < 6;++ i)
	{
		for(int j = 0;j < 7;++ j)
		{
			_XFontUnicode &tmpFont = m_dateFont[i * 7 + j];
			tmpFont.setPosition(m_position + _XVector2((25.0f + j * 50.0f) * m_size.x,(88.0f + 35.0f * i) * m_size.y));
			tmpFont.setSize(m_size);
		}
	}
}
void _XCalendar::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	static std::string tmpTitle[]={"日","一","二","三","四","五","六"};
	//显示背景
	drawFillBoxExA(_XVector2(m_nowMouseRect.left,m_nowMouseRect.top),
		_XVector2(m_nowMouseRect.getWidth(),
		m_nowMouseRect.getHeight()),0.75f * m_color.fR,0.75f * m_color.fG,0.75f * m_color.fB,m_color.fA,true); 
	//显示选项
	m_yearTxt.draw();
	m_monthTxt.draw();
	//显示标题
	for(int i = 0;i < 7;++ i)
		m_titleFont[i].draw();
	//显示特殊
	if(m_needShowToday)
	{
		drawFillBoxExA(_XVector2(m_nowMouseRect.left + m_todayPos.x * m_size.x,
			m_nowMouseRect.top + m_todayPos.y * m_size.y),
			_XVector2(50 * m_size.x,35 * m_size.y),0.5f * m_color.fR,0.5f * m_color.fG,1.0f * m_color.fB,m_color.fA,true);//,true); 
	}
	if(m_needShowChoose)
	{
		drawFillBoxExA(_XVector2(m_nowMouseRect.left + m_choosePos.x * m_size.x,
			m_nowMouseRect.top + m_choosePos.y * m_size.y),
			_XVector2(50 * m_size.x,35 * m_size.y),0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA,true);//,true); 
	}
	//显示日期
	for(int i = 0;i < 42;++ i)
		m_dateFont[i].draw();
	m_yearAddBtn.draw();
	m_yearDecBtn.draw();
	m_monthAddBtn.draw();
	m_monthDecBtn.draw();
}
_XBool _XCalendar::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	_XRect mouseRect;
	mouseRect.set(m_position.x + m_rect.left * m_size.x,
		m_position.y + (m_rect.top + 70.0f) * m_size.y,
		m_position.x + m_rect.right * m_size.x,
		m_position.y + m_rect.bottom * m_size.y);
	m_yearAddBtn.mouseProc(x,y,mouseState);
	m_yearDecBtn.mouseProc(x,y,mouseState);
	m_monthAddBtn.mouseProc(x,y,mouseState);
	m_monthDecBtn.mouseProc(x,y,mouseState);

	if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK) && 
		mouseRect.isInRect(x,y))
	{
		m_haveChoose = true;
		int dayX = (x - mouseRect.left) / (50.0f * m_size.x);
		int dayY = (y - mouseRect.top) / (35.0f * m_size.y);
		dayY = getDateDaysInYear(m_nowShowData) - getDataWeek(m_nowShowData) + dayY * 7 + dayX;
		getDateData(m_nowShowData.year,dayY,m_nowSetDate,dayX);
		updateNowDate();
		return XTrue;
	}
	return XFalse;
}
void _XCalendar::release()
{
	_XCtrlManger.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
}
void _XCalendar::setAlpha(float a)
{
	m_color.setA(a);
	m_yearAddBtn.setAlpha(a);
	m_yearDecBtn.setAlpha(a);
	m_yearTxt.setAlpha(a);
	m_monthAddBtn.setAlpha(a);
	m_monthDecBtn.setAlpha(a);
	m_monthTxt.setAlpha(a);
	for(int i = 0;i < 7;++ i)
	{
		m_titleFont[i].setAlpha(a);
	}
	for(int i = 0;i < 42;++ i)
	{
		m_dateFont[i].setAlpha(a);
	}
}
void _XCalendar::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_yearAddBtn.setColor(m_color);
	m_yearDecBtn.setColor(m_color);
	m_yearTxt.setColor(m_color);
	m_monthAddBtn.setColor(m_color);
	m_monthDecBtn.setColor(m_color);
	m_monthTxt.setColor(m_color);
	for(int i = 0;i < 7;++ i)
	{
		m_titleFont[i].setColor(m_color);
	}
	for(int i = 0;i < 42;++ i)
	{
		m_dateFont[i].setColor(m_color);
	}
}