#include "XStdHead.h"
#include "XProgressRing.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
XBool XProgressRing::initWithoutSkin(float r0,float r1,const XFontUnicode &font,bool withBG,bool withText)
{
	if(m_isInited) return XFalse;
	m_r0 = r0;
	m_r1 = r1;
	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif

	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE);
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	m_withBG = withBG;
	m_withText = withText;

	m_position.set(0.0f,0.0f);
	m_caption.setPosition(m_position);
	m_curValue = 0.0f;
	char tempC[20] = "100%";
	sprintf(tempC,"%d%%",(int)m_curValue);
	m_caption.setString(tempC);

	float size = m_r0 > m_r1?m_r0:m_r1;
	m_curMouseRect.set(m_position.x - size * m_scale.x,m_position.y - size * m_scale.y,
		m_position.x + size * m_scale.x,m_position.y + size * m_scale.y);

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
void XProgressRing::draw()
{
	if(!m_isInited) return;
	if(m_withBG) XRender::drawFillRing(m_position.x,m_position.y,m_r0 * m_scale.x,m_r1 * m_scale.y,0.0f,360.0f,90,m_color.fR * 0.5f,m_color.fG * 0.5f,m_color.fB * 0.5f,m_color.fA * 0.5f);
	XRender::drawFillRing(m_position.x,m_position.y,m_r0 * m_scale.x,m_r1 * m_scale.y,0.0f,360.0f * 0.01f * m_curValue,90,m_color);
	if(m_withText) m_caption.draw();
}
void XProgressRing::release()
{
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
}
#if !WITH_INLINE_FILE
#include "XProgressRing.inl"
#endif
}