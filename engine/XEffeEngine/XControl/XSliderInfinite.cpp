#include "XStdHead.h"
#include "XSliderInfinite.h"
#include "XControlManager.h"
namespace XE
{
void XSliderInfinite::ctrlProc(void*pClass, int id, int eventID)
{
	XSliderInfinite &ref = *(XSliderInfinite *)pClass;
	if (id == ref.m_mainSld.getControlID())
	{
		switch (eventID)
		{
		case XSlider::SLD_MOUSE_ON:
			if (ref.m_eventProc != NULL) 
				ref.m_eventProc(ref.m_pClass, ref.m_objectID, SLDI_MOUSE_ON);
			else 
				XCtrlManager.eventProc(ref.m_objectID, SLDI_MOUSE_ON);
			break;
		case XSlider::SLD_MOUSE_DOWN:
			if (ref.m_eventProc != NULL) 
				ref.m_eventProc(ref.m_pClass, ref.m_objectID, SLDI_MOUSE_DOWN);
			else 
				XCtrlManager.eventProc(ref.m_objectID, SLDI_MOUSE_DOWN);
			break;
		case XSlider::SLD_MOUSE_UP:
			if (ref.m_eventProc != NULL) 
				ref.m_eventProc(ref.m_pClass, ref.m_objectID, SLDI_MOUSE_UP);
			else 
				XCtrlManager.eventProc(ref.m_objectID, SLDI_MOUSE_UP);
			break;
		case XSlider::SLD_MOUSE_MOVE:
			if (ref.m_eventProc != NULL) 
				ref.m_eventProc(ref.m_pClass, ref.m_objectID, SLDI_MOUSE_MOVE);
			else 
				XCtrlManager.eventProc(ref.m_objectID, SLDI_MOUSE_MOVE);
			break;
		case XSlider::SLD_VALUE_CHANGE:
			if (ref.m_eventProc != NULL)
				ref.m_eventProc(ref.m_pClass, ref.m_objectID, SLDI_VALUE_CHANGE);
			else 
				XCtrlManager.eventProc(ref.m_objectID, SLDI_VALUE_CHANGE);
			break;
		}
		return;
	}
	if (id == ref.m_addMinBtn.getControlID())
	{
		if (eventID != XButton::BTN_MOUSE_UP) return;
		ref.changeRange(TYPE_ADD_MIN);
		return;
	}
	if (id == ref.m_desMinBtn.getControlID())
	{
		if (eventID != XButton::BTN_MOUSE_UP) return;
		ref.changeRange(TYPE_DES_MIN);
		return;
	}
	if (id == ref.m_addMaxBtn.getControlID())
	{
		if (eventID != XButton::BTN_MOUSE_UP) return;
		ref.changeRange(TYPE_ADD_MAX);
		return;
	}
	if (id == ref.m_desMaxBtn.getControlID())
	{
		if (eventID != XButton::BTN_MOUSE_UP) return;
		ref.changeRange(TYPE_DES_MAX);
		return;
	}
}
void XSliderInfinite::changeRange(XChangeRangeType type)
{
	float minValue = m_mainSld.getMinValue();
	float maxValue = m_mainSld.getMaxValue();
	float curValue = m_mainSld.getCurValue();
	switch (type)
	{
	case TYPE_ADD_MIN:
		minValue -= (maxValue - minValue) * 0.5f;
		break;
	case TYPE_DES_MIN:
		if(curValue - minValue >= (maxValue - minValue) * 0.5f)
			minValue += (maxValue - minValue) * 0.5f;
		else
			minValue += (curValue - minValue) * 0.5f;
		break;
	case TYPE_ADD_MAX:
		maxValue += (maxValue - minValue) * 0.5f;
		break;
	case TYPE_DES_MAX:
		if(maxValue - curValue >= (maxValue - minValue) * 0.5f)
			maxValue -= (maxValue - minValue) * 0.5f;
		else
			maxValue -= (maxValue - curValue) * 0.5f;
		break;
	}
	m_mainSld.setRange(minValue, maxValue, false);
}
XSliderInfinite::XSliderInfinite()
	:m_isInited(false)
	, m_timer(0.0f)
{
	m_ctrlType = CTRL_OBJ_SLIDERINFINITE;
}
void XSliderInfinite::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = false;
}
XBool XSliderInfinite::initWithoutSkin(const XVec2& size, float maxValue, float minValue,
	XSliderType type)
{
	if (m_isInited) return false;
	if (type == SLIDER_TYPE_VERTICAL)
	{
		if (!m_mainSld.initWithoutSkin(size, maxValue, minValue, type, XVec2(16.0f, 0.0f))) return XFalse;
	}
	else
	{
		if (!m_mainSld.initWithoutSkin(size, maxValue, minValue, type)) return XFalse;
	}
	float w = size.x;
	if (m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)	//水平模式
		w = size.y;

	m_addMinBtn.initWithoutSkin("", w);
	m_desMinBtn.initWithoutSkin("", w);
	m_addMaxBtn.initWithoutSkin("", w);
	m_desMaxBtn.initWithoutSkin("", w);

	m_addMinBtn.setSymbol(BTN_SYMBOL_CROSS);
	m_desMinBtn.setSymbol(BTN_SYMBOL_LINE);
	m_addMaxBtn.setSymbol(BTN_SYMBOL_CROSS);
	m_desMaxBtn.setSymbol(BTN_SYMBOL_LINE);

	m_mainSld.setEventProc(ctrlProc, this);
	m_addMinBtn.setEventProc(ctrlProc, this);
	m_addMinBtn.disVisible();
	m_desMinBtn.setEventProc(ctrlProc, this);
	m_desMinBtn.disVisible();
	m_addMaxBtn.setEventProc(ctrlProc, this);
	m_addMaxBtn.disVisible();
	m_desMaxBtn.setEventProc(ctrlProc, this);
	m_desMaxBtn.disVisible();

	stateChange();

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.decreaseAObject(&m_mainSld);
	XCtrlManager.decreaseAObject(&m_addMinBtn);
	XCtrlManager.decreaseAObject(&m_desMinBtn);
	XCtrlManager.decreaseAObject(&m_addMaxBtn);
	XCtrlManager.decreaseAObject(&m_desMaxBtn);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_mainSld);
	XObjManager.decreaseAObject(&m_addMinBtn);
	XObjManager.decreaseAObject(&m_desMinBtn);
	XObjManager.decreaseAObject(&m_addMaxBtn);
	XObjManager.decreaseAObject(&m_desMaxBtn);
#endif

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	m_isInited = XTrue;
	setPosition(XVec2::zero);
	return true;
}
void XSliderInfinite::update(float steptime)
{
	if (!m_isInited) return;	//如果没有初始化直接退出
	m_mainSld.update(steptime);
	m_addMinBtn.update(steptime);
	m_desMinBtn.update(steptime);
	m_addMaxBtn.update(steptime);
	m_desMaxBtn.update(steptime);
	if (m_timer < 1000.0f)
	{
		m_timer += steptime;
		if (m_timer >= 1000.0f)
		{
			m_addMinBtn.disVisible();
			m_desMinBtn.disVisible();
			m_addMaxBtn.disVisible();
			m_desMaxBtn.disVisible();
		}
	}
	if (m_mainSld.getVisible() && m_mainSld.isInRect(getMousePos()) &&
		m_mainSld.getEnable())
	{//显示微分按钮
		m_timer = 0.0f;
		m_addMinBtn.setVisible();
		m_desMinBtn.setVisible();
		m_addMaxBtn.setVisible();
		m_desMaxBtn.setVisible();
		return;
	}
	if (m_mainSld.getEnable() &&
		(m_addMinBtn.getVisible() && m_addMinBtn.isInRect(getMousePos())) ||
		(m_desMinBtn.getVisible() && m_desMinBtn.isInRect(getMousePos())) ||
		(m_addMaxBtn.getVisible() && m_addMaxBtn.isInRect(getMousePos())) ||
		(m_desMaxBtn.getVisible() && m_desMaxBtn.isInRect(getMousePos())))
	{
		m_timer = 0.0f;
		return;
	}
}
XBool XSliderInfinite::mouseProc(const XVec2& p, XMouseState mouseState)	//对于鼠标动作的响应函数
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if (m_isSilent) return XFalse;

	m_mainSld.mouseProc(p, mouseState);
	m_addMinBtn.mouseProc(p, mouseState);
	m_desMinBtn.mouseProc(p, mouseState);
	m_addMaxBtn.mouseProc(p, mouseState);
	m_desMaxBtn.mouseProc(p, mouseState);
	if (mouseState == MOUSE_LEFT_BUTTON_UP && m_mainSld.isInRect(p))
		m_isBeChoose = XTrue;
	return XTrue;
}
void XSliderInfinite::setPosition(const XVec2& p)
{
	if (!m_isInited) return;	//如果没有初始化直接退出
	m_position = p;

	m_mainSld.setPosition(m_position);
	if (m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//水平模式
		m_addMinBtn.setPosition(m_mainSld.getBox(3) + XVec2(0.0f, 2.0f * m_scale.y));
		m_desMinBtn.setPosition(m_addMinBtn.getBox(1) + XVec2(2.0f * m_scale.y, 0.0f));
		m_addMaxBtn.setPosition(m_mainSld.getBox(2) + XVec2(-m_addMaxBtn.getRectWidth(), 2.0f * m_scale.y));
		m_desMaxBtn.setPosition(m_addMaxBtn.getBox(0) + XVec2(-m_desMaxBtn.getRectWidth() - 2.0f * m_scale.y, 0.0f));
	}
	else
	{
		m_addMinBtn.setPosition(m_mainSld.getBox(0) - XVec2(2.0f * m_scale.x + m_addMinBtn.getRectWidth(),0.0f));
		m_desMinBtn.setPosition(m_addMinBtn.getBox(3) + XVec2(0.0f, 2.0f * m_scale.y));
		m_addMaxBtn.setPosition(m_mainSld.getBox(3) - XVec2(m_addMaxBtn.getRectWidth() + 2.0f * m_scale.x, m_addMaxBtn.getRectHeight()));
		m_desMaxBtn.setPosition(m_addMaxBtn.getBox(0) - XVec2(0.0f, m_addMaxBtn.getRectHeight() + 2.0f * m_scale.y));
	}
	m_curMouseRect = m_mainSld.getMouseRect();
	updateChildPos();
}
void XSliderInfinite::setScale(const XVec2& s)
{
	if (s.x <= 0 || s.y <= 0 ||
		!m_isInited) return;	//如果没有初始化直接退出
	m_scale = s;

	m_mainSld.setScale(m_scale);
	m_addMinBtn.setScale(m_scale);
	m_desMinBtn.setScale(m_scale);
	m_addMaxBtn.setScale(m_scale);
	m_desMaxBtn.setScale(m_scale);
	if (m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//水平模式
		m_addMinBtn.setPosition(m_mainSld.getBox(3) + XVec2(0.0f, 2.0f * m_scale.y));
		m_desMinBtn.setPosition(m_addMinBtn.getBox(1) + XVec2(2.0f * m_scale.y, 0.0f));
		m_addMaxBtn.setPosition(m_mainSld.getBox(2) + XVec2(-m_addMaxBtn.getRectWidth(), 2.0f * m_scale.y));
		m_desMaxBtn.setPosition(m_addMaxBtn.getBox(0) + XVec2(-m_desMaxBtn.getRectWidth() - 2.0f * m_scale.y, 0.0f));
	}
	else
	{
		m_addMinBtn.setPosition(m_mainSld.getBox(0) - XVec2(2.0f * m_scale.x + m_addMinBtn.getRectWidth(), 0.0f));
		m_desMinBtn.setPosition(m_addMinBtn.getBox(3) + XVec2(2.0f * m_scale.x, 0.0f));
		m_addMaxBtn.setPosition(m_mainSld.getBox(3) - XVec2(m_addMaxBtn.getRectWidth() + 2.0f * m_scale.x, m_addMaxBtn.getRectHeight()));
		m_desMaxBtn.setPosition(m_addMaxBtn.getBox(3) - XVec2(0.0f, m_addMaxBtn.getRectHeight() + 2.0f * m_scale.y));
	}
	m_curMouseRect = m_mainSld.getMouseRect();
	updateChildScale();
}
XVec2 XSliderInfinite::getBox(int order)		//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if (!m_isInited) return XVec2::zero;
	if (m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{
		if (m_addMinBtn.getVisible())
		{
			switch (order)
			{
			case 0:return m_mainSld.getBox(0);
			case 1:return m_mainSld.getBox(1);
			case 2:return m_addMaxBtn.getBox(2);
			case 3:return m_addMinBtn.getBox(3);
			}
		}
		else
		{
			return m_mainSld.getBox(order);
		}
	}
	else
	{//这里可能会有问题
		if (m_addMinBtn.getVisible())
		{
			switch (order)
			{
			case 0:return m_addMinBtn.getBox(0);
			case 1:return m_mainSld.getBox(1);
			case 2:return m_mainSld.getBox(2);
			case 3:return m_addMaxBtn.getBox(3);
			}
		}
		else
		{
			return m_mainSld.getBox(order);
		}
	}

	return XVec2::zero;
}
#if !WITH_INLINE_FILE
#include "XSliderInfinite.inl"
#endif
}