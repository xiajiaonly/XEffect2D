INLINE void XText::setLostFocus() 
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	m_isBeChoose = XFalse;	//控件处于焦点状态
}
INLINE void XText::setPosition(const XVec2& p)
{
	m_position = p;
	m_font.setPosition(m_position);
}
INLINE void XText::setScale(const XVec2& s)
{
	m_scale = s;
	m_font.setScale(m_scale);
}
INLINE void XText::setColor(const XFColor& c)
{
	m_color = c;
	m_font.setColor(m_color);
}
INLINE void XText::setAlpha(float a)
{
	m_color.setA(a);
	m_font.setAlpha(a);
}
INLINE void XText::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	m_font.draw();
}								//描绘按钮
INLINE void XText::setAlignmentMode(XFontAlignmentModeX x, XFontAlignmentModeY y)
{
	if (!m_isInited) return;
	m_font.setAlignmentModeX(x);
	m_font.setAlignmentModeY(y);
}