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
			float tmpMin = v - sum * 0.5f;
			float tmpMax = v + sum * 0.5f;
			if(tmpMin < pPar.m_mainSld.getMinValue())
			{
				tmpMin = pPar.m_mainSld.getMinValue();
				tmpMax = tmpMin + sum;
			}else
			if(tmpMax > pPar.m_mainSld.getMaxValue())
			{
				tmpMax = pPar.m_mainSld.getMaxValue();
				tmpMin = tmpMax - sum;
			}
			pPar.m_secondarySld.setRange(tmpMax, tmpMin, false);
			pPar.m_secondarySld.setCurValue(v);
		}
		return;
	}
	if(id == pPar.m_secondarySld.getControlID())
	{
		if (eventID == XSlider::SLD_MOUSE_MOVE || eventID == XSlider::SLD_VALUE_CHANGE)
			pPar.m_mainSld.setCurValue(pPar.m_secondarySld.getCurValue(), true);
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
XBool XSliderEx::initWithoutSkin(const XRect& area,float _max,float _min,
		XSliderType type,const XVec2& fontPosition)
{
	if(m_isInited) return XFalse;	//防止重复初始化
	m_comment.init();
	if(!m_mainSld.initWithoutSkin(area, _max, _min,type,fontPosition)) return XFalse;
	if(m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//水平模式
		m_secondarySld.initWithoutSkin(XRect(XVec2::zero,area.getSize()), _max, _min,type);
		m_chooseBtn.initWithoutSkin("", getDefaultFont(), 1.0f, XRect(XVec2::zero, XVec2(area.getHeight())),
			XVec2(area.getHeight()) * 0.5f);
	}else
	{//垂直模式
		m_secondarySld.initWithoutSkin(XRect(XVec2::zero, area.getSize()), _max, _min,type);
		m_chooseBtn.initWithoutSkin("", getDefaultFont(), 1.0f, XRect(XVec2::zero, XVec2(area.getWidth())),
			XVec2(area.getWidth()) * 0.5f);
	}
	m_chooseBtn.setSymbol(BTN_SYMBOL_CROSS);
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

	m_isVisible = m_isEnable = m_isActive = XTrue;

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
	setPosition(XVec2::zero);
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
	m_comment.update(steptime);
	m_mainSld.update(steptime);
	m_secondarySld.update(steptime);
	m_chooseBtn.update(steptime);
	//下面做状态判断，理论上应该是在update里面判断，但是由于原来的设计中控件类没有update函数，所以目前在draw中做这个判断，之后需要修整这个问题
	if(m_timer < 1000)
	{
		m_timer += steptime;
		if(m_timer >= 1000)
		{
			m_secondarySld.disVisible();
			m_chooseBtn.disVisible();
		}
	}
	//if(m_mainSld.getVisible() && m_mainSld.getActive() && m_mainSld.isInRect(x,y))
	if(m_mainSld.getVisible() && m_mainSld.isInRect(getMousePos()) && 
		m_mainSld.getEnable())
	{//显示微分按钮
		m_timer = 0;
		m_chooseBtn.setVisible();
		return;
	}
	if(m_chooseBtn.getVisible() && m_chooseBtn.isInRect(getMousePos()) && 
		m_mainSld.getEnable())
	{
		m_timer = 0;
		return;
	}
	if(m_secondarySld.getVisible() && m_secondarySld.isInRect(getMousePos()) ||
		m_secondarySld.m_curSliderState == SLIDER_STATE_DOWN)
	{
		m_timer = 0;
		return;
	}
}
void XSliderEx::setScale(const XVec2& s)
{
	if(s.x <= 0 || s.y <= 0||
		!m_isInited) return;	//如果没有初始化直接退出
	m_scale = s;

	m_mainSld.setScale(m_scale);
	m_secondarySld.setScale(m_scale);
	m_chooseBtn.setScale(m_scale);
	if(m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//水平模式
		m_chooseBtn.setPosition(m_mainSld.getBox(1) + XVec2(5.0f * m_scale.x,0.0F));
		m_secondarySld.setPosition(m_mainSld.getBox(3) + XVec2(0.0F,5.0f * m_scale.y));
	}else
	{
		m_chooseBtn.setPosition(m_mainSld.getBox(3) + XVec2(0.0F,5.0f * m_scale.y));
		m_secondarySld.setPosition(m_mainSld.getBox(0) - XVec2(5.0f * m_scale.y + m_secondarySld.getMouseRectWidth(), 0.0F));
	}
	m_curMouseRect = m_mainSld.getMouseRect();
	updateChildScale();
}
void XSliderEx::setPosition(const XVec2& p)
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	m_position = p;

	m_mainSld.setPosition(m_position);
	if(m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//水平模式
		m_chooseBtn.setPosition(m_mainSld.getBox(1) + XVec2(5.0f * m_scale.x, 0.0f));
		m_secondarySld.setPosition(m_mainSld.getBox(3) + XVec2(0.0f,5.0f * m_scale.y));
	}else
	{
		m_chooseBtn.setPosition(m_mainSld.getBox(3) + XVec2(0.0F, 5.0f * m_scale.y));
		m_secondarySld.setPosition(m_mainSld.getBox(0) - XVec2(5.0f * m_scale.y + m_secondarySld.getMouseRectWidth(), 0.0F));
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