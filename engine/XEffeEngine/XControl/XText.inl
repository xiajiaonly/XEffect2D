INLINE void XText::setLostFocus() 
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	m_isBeChoose = XFalse;	//控件处于焦点状态
}
INLINE void XText::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_font.setPosition(x,y);
}
INLINE void XText::setScale(float x,float y)
{
	m_scale.set(x,y);
	m_font.setScale(x,y);
}
INLINE void XText::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_font.setColor(r,g,b,a);
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
