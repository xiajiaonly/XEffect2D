#include "XStdHead.h"
#include "XCalendar.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
void XCalendar::ctrlProc(void*pClass,int ID,int eventID)
{
	if(eventID != XButton::BTN_MOUSE_DOWN) return;
	XCalendar &pPar = *(XCalendar *)pClass;
	if(ID == pPar.m_yearAddBtn.getControlID())
	{
		++pPar.m_curShowData.year;
		pPar.m_yearTxt.setString(XString::toString(pPar.m_curShowData.year).c_str());
		pPar.updateCurDate();
		return;
	}
	if(ID == pPar.m_yearDecBtn.getControlID())
	{
		--pPar.m_curShowData.year;
		pPar.m_yearTxt.setString(XString::toString(pPar.m_curShowData.year).c_str());
		pPar.updateCurDate();
		return;
	}
	if(ID == pPar.m_monthAddBtn.getControlID())
	{
		++pPar.m_curShowData.month;
		if(pPar.m_curShowData.month > 12)
		{
			pPar.m_curShowData.month = 1;
			++pPar.m_curShowData.year;
			pPar.m_yearTxt.setString(XString::toString(pPar.m_curShowData.year).c_str());
		}
		pPar.m_monthTxt.setString(XString::toString(pPar.m_curShowData.month).c_str());
		pPar.updateCurDate();
		return;
	}
	if(ID == pPar.m_monthDecBtn.getControlID())
	{
		--pPar.m_curShowData.month;
		if(pPar.m_curShowData.month <= 0)
		{
			pPar.m_curShowData.month = 12;
			--pPar.m_curShowData.year;
			pPar.m_yearTxt.setString(XString::toString(pPar.m_curShowData.year).c_str());
		}
		pPar.m_monthTxt.setString(XString::toString(pPar.m_curShowData.month).c_str());
		pPar.updateCurDate();
		return;
	}
}
XBool XCalendar::initWithoutSkin(const XFontUnicode &font)
{
	if(m_isInited) return XFalse;
	m_position.set(0.0f,0.0f);
	m_scale.set(1.0f,1.0f);
	m_rect.set(0.0f,0.0f,350.0f,280.0f);
	XTime::getTimeMs(m_todayDate);	//获取现在的时间
	m_curShowData.year = m_todayDate.year;
	m_curShowData.month = m_todayDate.month;
	m_curShowData.day = 1;
	m_yearAddBtn.initWithoutSkin("<<",font,XRect(0.0f,0.0f,32.0f,32.0f));
	m_yearAddBtn.setPosition(m_position.x + 10.0f * m_scale.x,m_position.y + 2.0f * m_scale.y);
	m_yearAddBtn.setScale(m_scale);
	m_yearAddBtn.setEventProc(ctrlProc,this);
	m_yearAddBtn.setWithAction(XFalse);
	m_yearDecBtn.initWithoutSkin(">>",font,XRect(0.0f,0.0f,32.0f,32.0f));
	m_yearDecBtn.setPosition(m_position.x + 112.0f * m_scale.x,m_position.y + 2.0f * m_scale.y);
	m_yearDecBtn.setScale(m_scale);
	m_yearDecBtn.setEventProc(ctrlProc,this);
	m_yearDecBtn.setWithAction(XFalse);
	if(!m_yearTxt.setACopy(font)) return XFalse;
	m_yearTxt.setColor(0.0f,0.0f,0.0f,1.0f);
	m_yearTxt.setPosition(m_position + XVector2(77.0f * m_scale.x,18.0f * m_scale.y));
	m_yearTxt.setScale(m_scale);
	m_yearTxt.setString(XString::toString(m_todayDate.year).c_str());
	//将这些物件从物件管理器中注销掉
	XCtrlManager.decreaseAObject(&m_yearAddBtn);	//注销这个物件
	XCtrlManager.decreaseAObject(&m_yearDecBtn);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_yearAddBtn);
	XObjManager.decreaseAObject(&m_yearDecBtn);
	XObjManager.decreaseAObject(&m_yearTxt);
#endif

	m_monthAddBtn.initWithoutSkin("<<",font,XRect(0.0f,0.0f,32.0f,32.0f));
	m_monthAddBtn.setPosition(m_position.x + 240.0f * m_scale.x,m_position.y + 2.0f * m_scale.y);
	m_monthAddBtn.setScale(m_scale);
	m_monthAddBtn.setEventProc(ctrlProc,this);
	m_monthAddBtn.setWithAction(XFalse);
	m_monthDecBtn.initWithoutSkin(">>",font,XRect(0.0f,0.0f,32.0f,32.0f));
	m_monthDecBtn.setPosition(m_position.x + 312.0f * m_scale.x,m_position.y + 2.0f * m_scale.y);
	m_monthDecBtn.setScale(m_scale);
	m_monthDecBtn.setEventProc(ctrlProc,this);
	m_monthDecBtn.setWithAction(XFalse);
	if(!m_monthTxt.setACopy(font)) return XFalse;
	m_monthTxt.setColor(0.0f,0.0f,0.0f,1.0f);
	m_monthTxt.setPosition(m_position + XVector2(292.0f * m_scale.x,18.0f * m_scale.y));
	m_monthTxt.setScale(m_scale);
	m_monthTxt.setString(XString::toString(m_todayDate.month).c_str());
	//将这些物件从物件管理器中注销掉
	XCtrlManager.decreaseAObject(&m_monthAddBtn);	//注销这个物件
	XCtrlManager.decreaseAObject(&m_monthDecBtn);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_monthAddBtn);
	XObjManager.decreaseAObject(&m_monthDecBtn);
	XObjManager.decreaseAObject(&m_monthTxt);
#endif

	m_curMouseRect.set(m_position.x + m_rect.left * m_scale.x,m_position.y + m_rect.top * m_scale.y,
		m_position.x + m_rect.right * m_scale.x,m_position.y + m_rect.bottom * m_scale.y);

	std::string tmpTitle[]={"日","一","二","三","四","五","六"};
	for(int i = 0;i < 7;++ i)
	{
		if(!m_titleFont[i].setACopy(font)) return XFalse;
		if(i == 0) m_titleFont[i].setColor(1.0f,0.0f,0.0f,1.0f);
		else m_titleFont[i].setColor(0.0f,0.0f,0.0f,1.0f);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_titleFont[i]);
#endif
		m_titleFont[i].setPosition(m_position + XVector2((25.0f + i * 50.0f) * m_scale.x,53.0f * m_scale.y));
		m_titleFont[i].setScale(m_scale);
		m_titleFont[i].setString(tmpTitle[i].c_str());
	}
	for(int i = 0;i < 6;++ i)
	{
		for(int j = 0;j < 7;++ j)
		{
			XFontUnicode &tmpFont = m_dateFont[i * 7 + j];
			if(!tmpFont.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&tmpFont);
#endif
			tmpFont.setPosition(m_position + XVector2((25.0f + j * 50.0f) * m_scale.x,(88.0f + 35.0f * i) * m_scale.y));
			tmpFont.setScale(m_scale);
		}
	}
	updateCurDate();

	m_isInited = XTrue;
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	return XTrue;
}
void XCalendar::updateCurDate()
{
	int week = XTime::getDateWeek(m_curShowData);
	int day = XTime::getDateDaysInYear(m_curShowData) - week;
	XSystemTime tmpT;
	m_needShowToday = false;
	m_needShowChoose = false;
	for(int i = 0;i < 6;++ i)
	{
		for(int j = 0;j < 7;++ j)
		{
			XFontUnicode &tmpFont = m_dateFont[i * 7 + j];
			if(j == 0) tmpFont.setColor(1.0f,0.0f,0.0f,1.0f);
			else tmpFont.setColor(0.0f,0.0f,0.0f,1.0f);
			XTime::getDateData(m_curShowData.year,day + i * 7 + j,tmpT,week);
			if(tmpT.month != m_curShowData.month) 
			{
				if(j == 0) tmpFont.setColor(1.0f,0.5f,0.5f,1.0f);
				else tmpFont.setColor(0.5f,0.5f,0.5f,1.0f);
			}
			tmpFont.setString(XString::toString(tmpT.day).c_str());
			if(tmpT.year == m_todayDate.year && tmpT.month == m_todayDate.month
				&& tmpT.day == m_todayDate.day)
			{//今天显示一个底色
				m_needShowToday = true;
				m_todayPos.set(50.0f * j,35.0f * i + 70.0f);
			//	drawFillBoxEx(XVector2(m_curMouseRect.left + 50.0f * j * m_scale.x,
			//		m_curMouseRect.top + (35.0f * i + 70.0f) * m_scale.y),
			//		XVector2(50 * m_scale.x,35 * m_scale.y),0.5f,0.5f,1.0f,true);//,true); 
			}
			if(m_haveChoose && tmpT.year == m_curSetDate.year && 
				tmpT.month == m_curSetDate.month && tmpT.day == m_curSetDate.day)
			{//与选择的日期一致
				m_needShowChoose = true;
				m_choosePos.set(50.0f * j,35.0f * i + 70.0f);
			//	drawFillBoxEx(XVector2(m_curMouseRect.left + 50.0f * j * m_scale.x,
			//		m_curMouseRect.top + (35.0f * i + 70.0f) * m_scale.y),
			//		XVector2(50 * m_scale.x,35 * m_scale.y),0.5f,0.5f,0.5f,true);//,true); 
			}

		}
	}
}
void XCalendar::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_yearAddBtn.setPosition(m_position.x + 10.0f * m_scale.x,m_position.y + 2.0f * m_scale.y);
	m_yearDecBtn.setPosition(m_position.x + 112.0f * m_scale.x,m_position.y + 2.0f * m_scale.y);
	m_yearTxt.setPosition(m_position + XVector2(77.0f * m_scale.x,18.0f * m_scale.y));
	m_monthAddBtn.setPosition(m_position.x + 240.0f * m_scale.x,m_position.y + 2.0f * m_scale.y);
	m_monthDecBtn.setPosition(m_position.x + 312.0f * m_scale.x,m_position.y + 2.0f * m_scale.y);
	m_monthTxt.setPosition(m_position + XVector2(292.0f * m_scale.x,18.0f * m_scale.y));
	m_curMouseRect.set(m_position.x + m_rect.left * m_scale.x,m_position.y + m_rect.top * m_scale.y,
		m_position.x + m_rect.right * m_scale.x,m_position.y + m_rect.bottom * m_scale.y);
	for(int i = 0;i < 7;++ i)
	{
		m_titleFont[i].setPosition(m_position + XVector2((25.0f + i * 50.0f) * m_scale.x,53.0f * m_scale.y));
	}
	for(int i = 0;i < 6;++ i)
	{
		for(int j = 0;j < 7;++ j)
		{
			XFontUnicode &tmpFont = m_dateFont[i * 7 + j];
			tmpFont.setPosition(m_position + XVector2((25.0f + j * 50.0f) * m_scale.x,(88.0f + 35.0f * i) * m_scale.y));
		}
	}
}
void XCalendar::setScale(float x,float y)
{
	if(x < 0.0f || y < 0.0f) return;
	if(x == m_scale.x && y == m_scale.y) return;
	m_scale.set(x,y);
	m_yearAddBtn.setPosition(m_position.x + 10.0f * m_scale.x,m_position.y + 2.0f * m_scale.y);
	m_yearAddBtn.setScale(m_scale);
	m_yearDecBtn.setPosition(m_position.x + 112.0f * m_scale.x,m_position.y + 2.0f * m_scale.y);
	m_yearDecBtn.setScale(m_scale);
	m_yearTxt.setPosition(m_position + XVector2(77.0f * m_scale.x,18.0f * m_scale.y));
	m_yearTxt.setScale(m_scale);
	m_monthAddBtn.setPosition(m_position.x + 240.0f * m_scale.x,m_position.y + 2.0f * m_scale.y);
	m_monthAddBtn.setScale(m_scale);
	m_monthDecBtn.setPosition(m_position.x + 312.0f * m_scale.x,m_position.y + 2.0f * m_scale.y);
	m_monthDecBtn.setScale(m_scale);
	m_monthTxt.setPosition(m_position + XVector2(292.0f * m_scale.x,18.0f * m_scale.y));
	m_monthTxt.setScale(m_scale);
//	m_titleTxt.setSize(m_scale);
	m_curMouseRect.set(m_position.x + m_rect.left * m_scale.x,m_position.y + m_rect.top * m_scale.y,
		m_position.x + m_rect.right * m_scale.x,m_position.y + m_rect.bottom * m_scale.y);
	for(int i = 0;i < 7;++ i)
	{
		m_titleFont[i].setPosition(m_position + XVector2((25.0f + i * 50.0f) * m_scale.x,53.0f * m_scale.y));
		m_titleFont[i].setScale(m_scale);
	}
	for(int i = 0;i < 6;++ i)
	{
		for(int j = 0;j < 7;++ j)
		{
			XFontUnicode &tmpFont = m_dateFont[i * 7 + j];
			tmpFont.setPosition(m_position + XVector2((25.0f + j * 50.0f) * m_scale.x,(88.0f + 35.0f * i) * m_scale.y));
			tmpFont.setScale(m_scale);
		}
	}
}
void XCalendar::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	static const std::string tmpTitle[]={"日","一","二","三","四","五","六"};
	//显示背景
	XRender::drawFillBoxExA(XVector2(m_curMouseRect.left,m_curMouseRect.top),
		XVector2(m_curMouseRect.getWidth(),
		m_curMouseRect.getHeight()),XCCS::normalColor * m_color,true); 
	//显示选项
	m_yearTxt.draw();
	m_monthTxt.draw();
	//显示标题
	for(int i = 0;i < 7;++ i)
		m_titleFont[i].draw();
	//显示特殊
	if(m_needShowToday)
	{
		XRender::drawFillBoxExA(XVector2(m_curMouseRect.left + m_todayPos.x * m_scale.x,
			m_curMouseRect.top + m_todayPos.y * m_scale.y),
			XVector2(50 * m_scale.x,35 * m_scale.y),XCCS::downColor * m_color,true);//,true); 
	}
	if(m_needShowChoose)
	{
		XRender::drawFillBoxExA(XVector2(m_curMouseRect.left + m_choosePos.x * m_scale.x,
			m_curMouseRect.top + m_choosePos.y * m_scale.y),
			XVector2(50 * m_scale.x,35 * m_scale.y),XCCS::downColor * m_color,true);//,true); 
	}
	//显示日期
	for(int i = 0;i < 42;++ i)
		m_dateFont[i].draw();
	m_yearAddBtn.draw();
	m_yearDecBtn.draw();
	m_monthAddBtn.draw();
	m_monthDecBtn.draw();
}
XBool XCalendar::mouseProc(float x,float y,XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	XRect mouseRect;
	mouseRect.set(m_position.x + m_rect.left * m_scale.x,
		m_position.y + (m_rect.top + 70.0f) * m_scale.y,
		m_position.x + m_rect.right * m_scale.x,
		m_position.y + m_rect.bottom * m_scale.y);
	m_yearAddBtn.mouseProc(x,y,mouseState);
	m_yearDecBtn.mouseProc(x,y,mouseState);
	m_monthAddBtn.mouseProc(x,y,mouseState);
	m_monthDecBtn.mouseProc(x,y,mouseState);

	if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK) && 
		mouseRect.isInRect(x,y))
	{
		m_haveChoose = true;
		int dayX = (x - mouseRect.left) / (50.0f * m_scale.x);
		int dayY = (y - mouseRect.top) / (35.0f * m_scale.y);
		dayY = XTime::getDateDaysInYear(m_curShowData) - XTime::getDateWeek(m_curShowData) + dayY * 7 + dayX;
		XTime::getDateData(m_curShowData.year,dayY,m_curSetDate,dayX);
		updateCurDate();
		return XTrue;
	}
	return XFalse;
}
void XCalendar::release()
{
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
}
void XCalendar::setAlpha(float a)
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
void XCalendar::setColor(float r,float g,float b,float a)
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
#if !WITH_INLINE_FILE
#include "XCalendar.inl"
#endif
}