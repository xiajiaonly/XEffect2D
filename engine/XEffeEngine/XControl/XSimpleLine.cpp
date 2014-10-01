#include "XSimpleLine.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XBool _XSimpleLine::init(float lineLen,float lineWidth,_XSimpleLineType type)
{
	if(m_isInited) return XFalse;

	m_position.set(0.0f,0.0f);
	m_size.set(1.0f,1.0f);
	m_color.setColor(1.0f,1.0f,1.0f,1.0f);
	m_lineLen = lineLen;
	m_lineWidth = lineWidth;
	m_type = type;
	m_mouseRect.set(0.0f,0.0f,lineLen,lineWidth);

	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		m_nowMouseRect.set(m_position.x - 5.0f * m_size.x,m_position.y,
			m_position.x + 5.0f * m_size.x,m_position.y + m_lineLen * m_size.y);
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		m_nowMouseRect.set(m_position.x,m_position.y - 5.0f * m_size.y,
			m_position.x + m_lineLen * m_size.x,m_position.y + 5.0f * m_size.y);
		break;
	}

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
void _XSimpleLine::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		drawLine(m_position.x - 1,m_position.y,m_position.x - 1,m_position.y + m_lineLen,m_lineWidth,
			0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
		drawLine(m_position.x + 1,m_position.y,m_position.x + 1,m_position.y + m_lineLen,m_lineWidth,
			0.75f * m_color.fR,0.75f * m_color.fG,0.75f * m_color.fB,m_color.fA);
		drawLine(m_position.x,m_position.y,m_position.x,m_position.y + m_lineLen,m_lineWidth,
			0.0f,0.0f,0.0f,m_color.fA);
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		drawLine(m_position.x,m_position.y - 1,m_position.x + m_lineLen,m_position.y - 1,m_lineWidth,
			0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
		drawLine(m_position.x,m_position.y + 1,m_position.x + m_lineLen,m_position.y + 1,m_lineWidth,
			0.75f * m_color.fR,0.75f * m_color.fG,0.75f * m_color.fB,m_color.fA);
		drawLine(m_position.x,m_position.y,m_position.x + m_lineLen,m_position.y,m_lineWidth,
			0.0f,0.0f,0.0f,m_color.fA);
		break;
	}
}
void _XSimpleLine::release()
{
	if(!m_isInited) return;
	_XCtrlManger.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	m_isInited = false;
}