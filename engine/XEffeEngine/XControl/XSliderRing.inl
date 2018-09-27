INLINE void XSliderRing::updateFontStr()
{
	char str[1024];
	sprintf_s(str, 1024, m_fontString.c_str(), m_curValue);
	m_caption.setString(str);
}
INLINE XBool XSliderRing::keyboardProc(int keyOrder, XKeyState keyState)
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if (m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if (m_isSilent) return XFalse;

	return XFalse;
}
INLINE XBool XSliderRing::isInRect(const XVec2& p)
{
	if (!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XSliderRing::getBox(int order)
{
	if (!m_isInited) return XVec2::zero;
	switch (order)
	{
	case 0:return m_curMouseRect.getLT();
	case 1:return m_curMouseRect.getRT();
	case 2:return m_curMouseRect.getRB();
	case 3:return m_curMouseRect.getLB();
	}
	return XVec2::zero;
}
INLINE void XSliderRing::setTextColor(const XFColor& color)
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
INLINE void XSliderRing::setColor(const XFColor& c)
{
	if (!m_isInited) return;
	m_color = c;
	m_caption.setColor(m_textColor * m_color);
	updateChildColor();
}
INLINE void XSliderRing::setAlpha(float a)
{
	if (!m_isInited) return;
	m_color.setA(a);
	m_caption.setColor(m_textColor * m_color);
	updateChildAlpha();

}
INLINE XBool XSliderRing::canGetFocus(const XVec2& p)	//用于判断当前物件是否可以获得焦点
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(p);
}
INLINE void XSliderRing::setLostFocus()	//设置失去焦点
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出
	if (m_curSliderState != SLDR_STATE_DISABLE) m_curSliderState = SLDR_STATE_NORMAL;
	m_isBeChoose = XFalse;
}
INLINE void XSliderRing::disable()		//使控件无效
{
	m_curSliderState = SLDR_STATE_DISABLE;
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
INLINE void XSliderRing::enable()		//使控件有效
{
	if (m_curSliderState == SLDR_STATE_DISABLE)
	{
		m_curSliderState = SLDR_STATE_NORMAL;
		//还需要调用鼠标函数
		mouseProc(getMousePos(), MOUSE_MOVE);
	}
	m_isEnable = XTrue;
}