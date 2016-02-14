#include "XStdHead.h"
#include "XText.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
XBool XText::init(const XFontUnicode &font)
{
	if(m_isInited) return XFalse;
	if(!m_font.setACopy(font)) return XFalse;
	m_position.set(0.0f,0.0f);
	m_scale.set(1.0f,1.0f);
	m_font.setPosition(0.0f,0.0f);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_font);
#endif
	m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);

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
void XText::release()
{
	if(!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = false;
}
#if !WITH_INLINE_FILE
#include "XText.inl"
#endif
}
