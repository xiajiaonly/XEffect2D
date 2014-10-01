#include "XComment.h"
#include "../XBasicWindow.h"
_XBool _XComment::init()
{
	if(m_isInited) return XFalse;
	m_font.setACopy(XEE::systemFont);
	m_font.setColor(0.0f,0.0f,0.0f,1.0f);
	m_font.setSize(0.5f,0.5f);
	m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	//m_font.setMaxStrLen(128);

	m_isInited = XTrue;
	return XTrue;
}