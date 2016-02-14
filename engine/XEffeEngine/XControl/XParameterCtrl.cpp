#include "XStdHead.h"
#include "XParameterCtrl.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
void XParameterCtrl::ctrlProc(void*pClass,int id,int eventID)
{
	if(eventID != XButton::BTN_MOUSE_DOWN) return;
	XParameterCtrl &pPar = *(XParameterCtrl *)pClass;
	if(id == pPar.m_btn[0].getControlID())
	{
		pPar.increaseData();
		return;
	}
	if(id == pPar.m_btn[1].getControlID())
	{
		pPar.reduceData();
		return;
	}
}
bool XParameterCtrl::initWithoutSkin(float curValue,const XVector2 &pixelSize,const XFontUnicode &font)
{
	if(m_isInited) return false;
	if(pixelSize.y <= 0.0f || pixelSize.x <= pixelSize.y * 2.0f) return false;
	m_pixelSize = pixelSize;
	m_curData = curValue;
	if(!m_font.setACopy(font)) XFalse;
	m_mouseRect.set(0,0,m_pixelSize.x,m_pixelSize.y);
	m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE);
	m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	m_font.setString(XString::toString(m_curData).c_str());
	m_textColor = XFColor::black;
	m_font.setColor(XFColor::black);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_font);
#endif

	m_btn[0].initWithoutSkin("+",m_pixelSize.y);
	m_btn[0].setEventProc(ctrlProc,this);
	m_btn[1].initWithoutSkin("-",m_pixelSize.y);
	m_btn[1].setEventProc(ctrlProc,this);

	XCtrlManager.decreaseAObject(&m_btn[0]);	//在物件管理器中注册当前物件
	XCtrlManager.decreaseAObject(&m_btn[1]);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_btn[0]);
	XObjManager.decreaseAObject(&m_btn[1]);
#endif

	setPosition(0.0f,0.0f);
	setScale(1.0f,1.0f);
	m_isInited = true;
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	return true;
}
void XParameterCtrl::release()
{
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
}
void XParameterCtrl::increaseData()
{
	switch(m_paramType)
	{
	case TYPE_LINE:
		m_curData += m_increaseRate;
		break;
	case TYPE_EXPONENT:
		m_curData *= m_increaseRate;
		break;
	}
	m_font.setString(XString::toString(m_curData).c_str());
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,PC_VALUE_CHANGE);
	else XCtrlManager.eventProc(m_objectID,PC_VALUE_CHANGE);
}
void XParameterCtrl::reduceData()
{
	switch(m_paramType)
	{
	case TYPE_LINE:
		m_curData += m_reduceRate;
		break;
	case TYPE_EXPONENT:
		m_curData *= m_reduceRate;
		break;
	}
	m_font.setString(XString::toString(m_curData).c_str());
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,PC_VALUE_CHANGE);
	else XCtrlManager.eventProc(m_objectID,PC_VALUE_CHANGE);
}
}