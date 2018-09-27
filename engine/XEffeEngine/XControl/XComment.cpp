#include "XStdHead.h"
#include "XComment.h"
namespace XE{
XBool XComment::init()
{
	if(m_isInited) return XFalse;
	m_font.setACopy(getDefaultFont());
	m_font.setColor(XFColor::black);
	m_font.setScale(0.5f);
	m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	//m_font.setMaxStrLen(128);
	m_font.setAlpha(m_alpha);

	m_isInited = XTrue;
	return XTrue;
}
void XComment::update(float stepTime)
{
	if(!m_needShow) return;
	if(m_timer < m_appearTime && m_timer + stepTime >= m_appearTime)//这里是为了设置位置，目前这个方法可能存在问题
	{//开始出现
		m_alphaMD.set(0.0f,1.0f,0.002f);
	}else if(m_timer < m_disappear && m_timer + stepTime >= m_disappear)
	{//开始消失
		m_alphaMD.set(1.0f,0.0f,0.002f);
	}
	if(!m_alphaMD.getIsEnd())
	{
		m_alphaMD.move(stepTime);
		setAlpha(m_alphaMD.getCurData());
	}
	m_timer += stepTime;
	if(m_timer >= m_maxTime) m_needShow = false;	//4秒之后取消显示
}
#if !WITH_INLINE_FILE
#include "XComment.inl"
#endif
}