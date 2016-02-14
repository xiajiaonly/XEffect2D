#include "XStdHead.h"
#include "XSimpleLine.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
XBool XSimpleLine::init(float lineLen,float lineWidth,XSimpleLineType type)
{
	if(m_isInited) return XFalse;

	m_position.set(0.0f,0.0f);
	m_scale.set(1.0f,1.0f);
	m_color.setColor(1.0f,1.0f,1.0f,1.0f);
	m_lineLen = lineLen;
	m_lineWidth = lineWidth;
	m_type = type;
	m_mouseRect.set(0.0f,0.0f,lineLen,lineWidth);

	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		m_curMouseRect.set(m_position.x - 5.0f * m_scale.x,m_position.y,
			m_position.x + 5.0f * m_scale.x,m_position.y + m_lineLen * m_scale.y);
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		m_curMouseRect.set(m_position.x,m_position.y - 5.0f * m_scale.y,
			m_position.x + m_lineLen * m_scale.x,m_position.y + 5.0f * m_scale.y);
		break;
	}

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
void XSimpleLine::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		XRender::drawLine(m_position.x - 1,m_position.y,m_position.x - 1,m_position.y + m_lineLen,m_lineWidth,
			XCCS::downColor * m_color);
		XRender::drawLine(m_position.x + 1,m_position.y,m_position.x + 1,m_position.y + m_lineLen,m_lineWidth,
			XCCS::normalColor * m_color);
		XRender::drawLine(m_position.x,m_position.y,m_position.x,m_position.y + m_lineLen,m_lineWidth,
			XCCS::darkColor * m_color);
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		XRender::drawLine(m_position.x,m_position.y - 1,m_position.x + m_lineLen,m_position.y - 1,m_lineWidth,
			XCCS::downColor * m_color);
		XRender::drawLine(m_position.x,m_position.y + 1,m_position.x + m_lineLen,m_position.y + 1,m_lineWidth,
			XCCS::normalColor * m_color);
		XRender::drawLine(m_position.x,m_position.y,m_position.x + m_lineLen,m_position.y,m_lineWidth,
			XCCS::darkColor * m_color);
		break;
	}
}
void XSimpleLine::release()
{
	if(!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = false;
}
#if !WITH_INLINE_FILE
#include "XSimpleLine.inl"
#endif
}
