#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include"XSliderEx.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
void XSliderEx::ctrlProc(void*pClass,int id,int eventID)
{
	XSliderEx &pPar = *(XSliderEx *)pClass;
	if(id == pPar.m_chooseBtn.getControlID())
	{
		if(eventID != XButton::BTN_MOUSE_UP) return;
		if(pPar.m_secondarySld.getVisible()) pPar.m_secondarySld.disVisible();
		else 
		{//这里需要设置范围
			pPar.m_secondarySld.setVisible();
			float sum = (pPar.m_mainSld.getMaxValue() - pPar.m_mainSld.getMinValue()) * 0.02;
			float v = pPar.m_mainSld.getCurValue();
			float min = v - sum * 0.5f;
			float max = v + sum * 0.5f;
			if(min < pPar.m_mainSld.getMinValue())
			{
				min = pPar.m_mainSld.getMinValue();
				max = min + sum;
			}else
			if(max > pPar.m_mainSld.getMaxValue())
			{
				max = pPar.m_mainSld.getMaxValue();
				min = max - sum;
			}
			pPar.m_secondarySld.setRange(max,min);
			pPar.m_secondarySld.setCurValue(v);
		}
		return;
	}
	if(id == pPar.m_secondarySld.getControlID())
	{
		if(eventID == XSlider::SLD_MOUSE_MOVE || eventID == XSlider::SLD_VALUE_CHANGE)
			pPar.m_mainSld.setCurValue(pPar.m_secondarySld.getCurValue());
		return;
	}
	if(id == pPar.m_mainSld.getControlID())
	{
		switch(eventID)
		{
		case XSlider::SLD_MOUSE_ON:
			if(pPar.m_eventProc != NULL) pPar.m_eventProc(pPar.m_pClass,pPar.m_objectID,SLDEX_MOUSE_ON);
			else XCtrlManager.eventProc(pPar.m_objectID,SLDEX_MOUSE_ON);
			break;
		case XSlider::SLD_MOUSE_DOWN:
			if(pPar.m_secondarySld.getVisible()) pPar.m_secondarySld.disVisible();
			if(pPar.m_eventProc != NULL) pPar.m_eventProc(pPar.m_pClass,pPar.m_objectID,SLDEX_MOUSE_DOWN);
			else XCtrlManager.eventProc(pPar.m_objectID,SLDEX_MOUSE_DOWN);
			break;
		case XSlider::SLD_MOUSE_UP:
			if(pPar.m_eventProc != NULL) pPar.m_eventProc(pPar.m_pClass,pPar.m_objectID,SLDEX_MOUSE_UP);
			else XCtrlManager.eventProc(pPar.m_objectID,SLDEX_MOUSE_UP);
			break;
		case XSlider::SLD_MOUSE_MOVE:
			if(pPar.m_eventProc != NULL) pPar.m_eventProc(pPar.m_pClass,pPar.m_objectID,SLDEX_MOUSE_MOVE);
			else XCtrlManager.eventProc(pPar.m_objectID,SLDEX_MOUSE_MOVE);
			break;
		case XSlider::SLD_VALUE_CHANGE:
			if(pPar.m_eventProc != NULL) pPar.m_eventProc(pPar.m_pClass,pPar.m_objectID,SLDEX_VALUE_CHANGE);
			else XCtrlManager.eventProc(pPar.m_objectID,SLDEX_VALUE_CHANGE);
			break;
		}
	}
}
void XSliderEx::stateChangeProc(void *pClass)
{
	XSliderEx &pPar = *(XSliderEx *)pClass;
	pPar.stateChange();
}
XBool XSliderEx::initWithoutSkin(const XRect& area,float max,float min,
		XSliderType type,const XVector2 &fontPosition)
{
	if(m_isInited) return XFalse;	//防止重复初始化

	if(!m_mainSld.initWithoutSkin(area,max,min,type,fontPosition)) return XFalse;
	if(m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//水平模式
		m_secondarySld.initWithoutSkin(XRect(0.0f,0.0f,area.getWidth(),area.getHeight()),max,min,type);
		m_chooseBtn.initWithoutSkin("+",getDefaultFont(),1.0f,XRect(0.0f,0.0f,area.getHeight(),area.getHeight()),
			XVector2(area.getHeight() * 0.5f,area.getHeight() * 0.5f));
	}else
	{//垂直模式
		m_secondarySld.initWithoutSkin(XRect(0.0f,0.0f,area.getWidth(),area.getHeight()),max,min,type);
		m_chooseBtn.initWithoutSkin("+",getDefaultFont(),1.0f,XRect(0.0f,0.0f,area.getWidth(),area.getWidth()),
			XVector2(area.getWidth() * 0.5f,area.getWidth() * 0.5f));
	}
	m_mainSld.setEventProc(ctrlProc,this);
	m_mainSld.setStateChangeCB(stateChangeProc,this);
	m_secondarySld.disVisible();
	m_secondarySld.setEventProc(ctrlProc,this);
	m_secondarySld.setStateChangeCB(stateChangeProc,this);
	m_chooseBtn.disVisible();
	m_chooseBtn.setEventProc(ctrlProc,this);
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLDEX_INIT);
	else XCtrlManager.eventProc(m_objectID,SLDEX_INIT);

	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.decreaseAObject(&m_mainSld);
	XCtrlManager.decreaseAObject(&m_secondarySld);
	XCtrlManager.decreaseAObject(&m_chooseBtn);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_mainSld);
	XObjManager.decreaseAObject(&m_secondarySld);
	XObjManager.decreaseAObject(&m_chooseBtn);
#endif

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	m_isInited = XTrue;
	setPosition(0.0f,0.0f);
	return XTrue;
}
XBool XSliderEx::setACopy(const XSliderEx &temp)			//建立一个副本
{//
	if(&temp == this) return XFalse;
	if(!temp.m_isInited) return XFalse;
	if(!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;
	m_timer = temp.m_timer;
//	m_needDrawBtn = temp.m_needDrawBtn;
//	m_needDrawSld = temp.m_needDrawSld;
	if(!m_mainSld.setACopy(temp.m_mainSld)) return XFalse;		
	if(!m_secondarySld.setACopy(temp.m_secondarySld)) return XFalse;		
	if(!m_chooseBtn.setACopy(temp.m_chooseBtn)) return XFalse;		
	XCtrlManager.decreaseAObject(&m_mainSld);
	XCtrlManager.decreaseAObject(&m_secondarySld);
	XCtrlManager.decreaseAObject(&m_chooseBtn);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_mainSld);
	XObjManager.decreaseAObject(&m_secondarySld);
	XObjManager.decreaseAObject(&m_chooseBtn);
#endif
	return XTrue;
}
void XSliderEx::update(float steptime)
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	m_mainSld.update(steptime);
	m_secondarySld.update(steptime);
	m_chooseBtn.update(steptime);
	//下面做状态判断，理论上应该是在update里面判断，但是由于原来的设计中控件类没有update函数，所以目前在draw中做这个判断，之后需要修整这个问题
	if(m_timer < 1000)
	{
		m_timer += XEG.getLastFrameTime();
		if(m_timer >= 1000)
		{
			m_secondarySld.disVisible();
			m_chooseBtn.disVisible();
		}
	}
	//if(m_mainSld.getVisible() && m_mainSld.getActive() && m_mainSld.isInRect(x,y))
	if(m_mainSld.getVisible() && m_mainSld.isInRect(getMousePos().x,getMousePos().y) && 
		m_mainSld.getEnable())
	{//显示微分按钮
		m_timer = 0;
		m_chooseBtn.setVisible();
		return;
	}
	if(m_chooseBtn.getVisible() && m_chooseBtn.isInRect(getMousePos().x,getMousePos().y) && 
		m_mainSld.getEnable())
	{
		m_timer = 0;
		return;
	}
	if(m_secondarySld.getVisible() && m_secondarySld.isInRect(getMousePos().x,getMousePos().y) ||
		m_secondarySld.m_curSliderState == SLIDER_STATE_DOWN)
	{
		m_timer = 0;
		return;
	}
}
void XSliderEx::setScale(float x,float y)
{
	if(x <= 0 || y <= 0||
		!m_isInited) return;	//如果没有初始化直接退出
	m_scale.set(x,y);

	m_mainSld.setScale(x,y);
	m_secondarySld.setScale(x,y);
	m_chooseBtn.setScale(x,y);
	if(m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//水平模式
		m_chooseBtn.setPosition(m_mainSld.getBox(1).x + 5.0f * m_scale.x,m_mainSld.getBox(1).y);
		m_secondarySld.setPosition(m_mainSld.getBox(3).x,m_mainSld.getBox(3).y + 5.0f * m_scale.y);
	}else
	{
		m_chooseBtn.setPosition(m_mainSld.getBox(3).x,m_mainSld.getBox(3).y + 5.0f * m_scale.y);
		m_secondarySld.setPosition(m_mainSld.getBox(0).x - 5.0f * m_scale.y - m_secondarySld.getMouseRectWidth(),m_mainSld.getBox(0).y);
	}
	m_curMouseRect = m_mainSld.getMouseRect();
	updateChildScale();
}
void XSliderEx::setPosition(float x,float y)
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	m_position.set(x,y);

	m_mainSld.setPosition(x,y);
	if(m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//水平模式
		m_chooseBtn.setPosition(m_mainSld.getBox(1).x + 5.0f * m_scale.x,m_mainSld.getBox(1).y);
		m_secondarySld.setPosition(m_mainSld.getBox(3).x,m_mainSld.getBox(3).y + 5.0f * m_scale.y);
	}else
	{
		m_chooseBtn.setPosition(m_mainSld.getBox(3).x,m_mainSld.getBox(3).y + 5.0f * m_scale.y);
		m_secondarySld.setPosition(m_mainSld.getBox(0).x - 5.0f * m_scale.y - m_secondarySld.getMouseRectWidth(),m_mainSld.getBox(0).y);
	}
	m_curMouseRect = m_mainSld.getMouseRect();
	updateChildPos();
}
void XSliderEx::release()
{
	if(!m_isInited) return;
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLDEX_RELEASE);
	else XCtrlManager.eventProc(m_objectID,SLDEX_RELEASE);

	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = XFalse;
}
#if !WITH_INLINE_FILE
#include "XSliderEx.inl"
#endif
}