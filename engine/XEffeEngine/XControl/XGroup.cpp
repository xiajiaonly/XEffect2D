#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XGroup.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE {
void XGroup::ctrlProc(void *pClass, int id, int eventID)
{
	if (eventID != XButton::BTN_MOUSE_DOWN) return;
	XGroup & pPar = *(XGroup *)pClass;
	if (id == pPar.m_stateBotton.getControlID())
	{
		if (pPar.m_state == STATE_NORMAL) pPar.setState(STATE_MINISIZE);
		else pPar.setState(STATE_NORMAL);
	}
}
XBool XGroup::init(const XVec2& position,
	const XRect& rect,
	const char *caption, const XFontUnicode& font, float captionSize)
{
	if (m_isInited) return XFalse;	//防止重复初始化

	m_position = position;
	m_rect = rect;
	m_textSize.set(captionSize);
	m_scale.set(1.0f);
	if (!m_caption.setACopy(font)) return XFalse;
	m_caption.setScale(captionSize);
	m_caption.setString(caption);
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);

	m_state = STATE_NORMAL;
	if (!m_stateBotton.initWithoutSkin("", font, captionSize, XRect(0.0f, m_groupStateBtnSize),
		XVec2(m_groupStateBtnSize * 0.5f))) return XFalse;
	m_stateBotton.setEventProc(ctrlProc, this);
	m_stateBotton.setSymbol(BTN_SYMBOL_LINE);
	updateData();

	//XCtrlManager.decreaseAObject(&m_stateBotton);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
	XObjManager.decreaseAObject(&m_stateBotton);
#endif
	//XCtrlManager.decreaseAObject(&m_stateBotton);
	XCtrlManager.addACtrl(this);
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	m_isVisible = m_isEnable = m_isActive = XTrue;

	m_isInited = XTrue;
	return XTrue;
}
void XGroup::draw()
{
	if (!m_isInited ||
		!m_isVisible) return;	//如果没有初始化直接退出
	if (m_withRect)
	{
		if (m_state == STATE_NORMAL)
		{
			int h = m_caption.getMaxPixelHeight();
			int w = m_caption.getMaxPixelWidth();

			XRender::drawLine(m_drawRect.left, m_drawRect.top + (h >> 1), m_drawRect.left, m_drawRect.bottom, 1, m_color);
			XRender::drawLine(m_drawRect.right, m_drawRect.top + (h >> 1), m_drawRect.right, m_drawRect.bottom, 1, m_color);
			XRender::drawLine(m_drawRect.getLB(), m_drawRect.getRB(), 1, m_color);

			//drawLine(m_drawRect.left,m_drawRect.top + (h >> 1),m_drawRect.left + 20.0f * m_scale.x ,m_drawRect.top + (h >> 1),1);
			if (m_drawRect.getWidth() > w + m_groupStateBtnSize + 10.0f)
				XRender::drawLine(m_drawRect.left + (m_groupStateBtnSize + 10.0f) * m_scale.x + w, m_drawRect.top + (h >> 1),
					m_drawRect.right, m_drawRect.top + (h >> 1), 1, m_color);
		}
		else
		{
			XRender::drawRect(m_drawRect, 1, m_color);
			//XRender::drawLine(m_drawRect.getLT(),m_drawRect.getLB(),1,m_color);
			//XRender::drawLine(m_drawRect.getRT(),m_drawRect.getRB(),1,m_color);
			//XRender::drawLine(m_drawRect.getLB(),m_drawRect.getRB(),1,m_color);
			//XRender::drawLine(m_drawRect.getLT(),m_drawRect.getRT(),1,m_color);
		}
	}
	m_caption.draw();
	//	m_stateBotton.draw();
}
XBool XGroup::setACopy(const XGroup & temp)
{
	if (&temp == this) return XTrue;	//防止自身赋值
	if (!temp.m_isInited ||
		!XControlBasic::setACopy(temp)) return XFalse;
	if (!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;
	m_position = temp.m_position;	//控件的位置
	m_scale = temp.m_scale;		//大小
	m_rect = temp.m_rect;			//控件的范围
	m_withRect = temp.m_withRect;			//控件的范围
//	m_funStateChange = temp.m_funStateChange;	//状态发生改变时调用的函数
//	m_pClass = temp.m_pClass;

	m_caption.setACopy(temp.m_caption);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_textSize = temp.m_textSize;

	return XTrue;
}
void XGroup::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = false;
}
void XGroup::setState(XGroupState state)
{
	if (m_state == state) return;
	m_state = state;
	//if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);	//注意这个调用顺序，如果放在可见状态改变之后会造成成员Action的问题
	if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, GRP_STATE_CHANGE);
	else XCtrlManager.eventProc(m_objectID, GRP_STATE_CHANGE);
	if (m_state == STATE_NORMAL)
	{
		m_stateBotton.setSymbol(BTN_SYMBOL_LINE);
		//隐藏所有子物体
		for (unsigned int i = 0; i < m_childList.size(); ++i)
		{
			m_childList[i]->setVisible();
		}
	}
	else
	{
		m_stateBotton.setSymbol(BTN_SYMBOL_CROSS);
		//显示所有子物体
		for (unsigned int i = 0; i < m_childList.size(); ++i)
		{
			m_childList[i]->disVisible();
		}
	}
	updateData();
}
#if !WITH_INLINE_FILE
#include "XGroup.inl"
#endif
}