#include "XStdHead.h"
#include "XProgressRing.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
XBool XProgressRing::initWithoutSkin(float r0, float r1, const XFontUnicode& font,
	bool withBG, bool withText)
{
	if (m_isInited) return XFalse;
	m_r0 = r0;
	m_r1 = r1;
	if (!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif

	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE);
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	m_withBG = withBG;
	m_withText = withText;

	m_position.reset();
	m_caption.setPosition(m_position);
	m_curValue = 0.0f;
	char tempC[20] = "100%";
	sprintf_s(tempC, 20, "%d%%", (int)m_curValue);
	m_caption.setString(tempC);

	float size = m_r0 > m_r1 ? m_r0 : m_r1;
	m_curMouseRect.set(m_position - size * m_scale,
		m_position + size * m_scale);

	m_isInited = m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	return XTrue;
}
void XProgressRing::draw()
{
	if(!m_isInited) return;
	if(m_withBG) 
	{
		XRender::drawFillRing(m_position, m_r0 * m_scale.x, m_r1 * m_scale.y,
			0.0f, 360.0f, 90, m_color * 0.5f);
	}
	if(m_withVirtual)
	{
		if(m_curValue >= m_valueMD.getCurData())
		{
			XRender::drawFillRing(m_position, m_r0 * m_scale.x, m_r1 * m_scale.y,
				0.0f, 360.0f * 0.01f * m_curValue, 90, m_color * 0.75f);
			XRender::drawFillRing(m_position, m_r0 * m_scale.x, m_r1 * m_scale.y,
				0.0f, 360.0f * 0.01f * m_valueMD.getCurData(), 90, m_color);
		}else
		{
			XRender::drawFillRing(m_position, m_r0 * m_scale.x, m_r1 * m_scale.y,
				0.0f, 360.0f * 0.01f * m_valueMD.getCurData(), 90, m_color * 0.75f);
			XRender::drawFillRing(m_position, m_r0 * m_scale.x, m_r1 * m_scale.y,
				0.0f, 360.0f * 0.01f * m_curValue, 90, m_color);
		}
	}else
	{
		XRender::drawFillRing(m_position, m_r0 * m_scale.x, m_r1 * m_scale.y,
			0.0f, 360.0f * 0.01f * m_curValue, 90, m_color);
	}
	if (m_withText) m_caption.draw();
}
void XProgressRing::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = false;
}
#if !WITH_INLINE_FILE
#include "XProgressRing.inl"
#endif
}