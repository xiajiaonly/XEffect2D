#include "XProgressRing.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XBool _XProgressRing::initWithoutTex(float r0,float r1,const _XFontUnicode &font,bool withBG,bool withText)
{
	if(m_isInited) return XFalse;
	m_r0 = r0;
	m_r1 = r1;
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif

	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE);
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	m_withBG = withBG;
	m_withText = withText;

	m_position.set(0.0f,0.0f);
	m_caption.setPosition(m_position);
	m_nowValue = 0.0f;
	char tempC[20] = "100%";
	sprintf(tempC,"%d%%",(int)m_nowValue);
	m_caption.setString(tempC);

	float size = m_r0 > m_r1?m_r0:m_r1;
	m_nowMouseRect.set(m_position.x - size * m_size.x,m_position.y - size * m_size.y,
		m_position.x + size * m_size.x,m_position.y + size * m_size.y);

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
void _XProgressRing::draw()
{
	if(!m_isInited) return;
	if(m_withBG) drawFillRing(m_position.x,m_position.y,m_r0 * m_size.x,m_r1 * m_size.y,0.0f,360.0f,90,m_color.fR * 0.5f,m_color.fG * 0.5f,m_color.fB * 0.5f,m_color.fA * 0.5f);
	drawFillRing(m_position.x,m_position.y,m_r0 * m_size.x,m_r1 * m_size.y,0.0f,360.0f * 0.01f * m_nowValue,90,m_color.fR,m_color.fG,m_color.fB,m_color.fA);
	if(m_withText) m_caption.draw();
}
void _XProgressRing::release()
{
	_XCtrlManger.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
}