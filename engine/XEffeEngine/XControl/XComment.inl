INLINE XBool XComment::init(const XFontUnicode &font)
{
	if(m_isInited) return XFalse;
	if(!m_font.setACopy(font)) return XFalse;
	m_font.setColor(0.0f,0.0f,0.0f,1.0f);
	m_font.setScale(0.5f,0.5f);
	m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	//m_font.setMaxStrLen(128);
	m_font.setAlpha(m_alpha);

	m_isInited = XTrue;
	return XTrue;
}
INLINE XBool XComment::setACopy(const XComment & temp)
{
	m_isInited = temp.m_isInited;		//是否初始化
	m_font.setACopy(temp.m_font);
	m_bgRect = temp.m_bgRect;		//背景的显示
	m_haveString = temp.m_haveString;	//是否有说明
	m_visible = temp.m_visible;		//是否可见
	m_needShow = temp.m_needShow;		//是否需要显示
	m_alpha = temp.m_alpha;

	m_timer = temp.m_timer;
	return XTrue;
}
INLINE void XComment::setPosition(float x,float y)
{
	int w = (int)m_bgRect.getWidth();
	int h = (int)m_bgRect.getHeight();
	m_bgRect.set(x,y,x + w,y + h);
	m_font.setPosition(x + 2.0f,y + 2.0f);
}
INLINE void XComment::draw()
{
	if(m_isInited && m_haveString && m_visible && m_needShow && m_timer > m_appearTime)	//1秒之后才显示
	{
		XFColor tmp = XCCS::normalColor;
		tmp.fA = m_alpha;
		XRender::drawFillBoxA(XVector2(m_bgRect.left,m_bgRect.top),XVector2(m_bgRect.getWidth(),
			m_bgRect.getHeight()),tmp,true);
		m_font.draw();
	}
}
INLINE void XComment::setString(const char *str)
{
	if(!m_isInited) return;
	if(str == NULL) m_haveString = XFalse;
	else
	{
		m_haveString = true;
		m_font.setString(str);
		int w = m_font.getMaxPixelWidth();
		int h = m_font.getMaxPixelHeight();
		m_bgRect.right = m_bgRect.left + w + 4;
		m_bgRect.bottom = m_bgRect.top + h + 4;
	}
}
INLINE void XComment::disVisible()
{
	m_visible = XFalse;
	m_needShow = XFalse;
}
INLINE void XComment::setShow()
{
	m_needShow = XTrue;
	m_timer = 0;
}