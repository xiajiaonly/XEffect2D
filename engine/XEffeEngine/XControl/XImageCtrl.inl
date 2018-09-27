INLINE void XImageCtrl::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_connectWithTex)
	{
		m_sprite.draw(m_connectTex);
	}else
	{
		if(m_onlyWithTexture) return m_sprite.draw(m_texID);
		else m_sprite.draw();
	}
}
INLINE XBool XImageCtrl::canGetFocus(const XVec2& p)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(p);
}
INLINE void XImageCtrl::setPosition(const XVec2& p)
{
	m_position = p;
	m_sprite.setPosition(m_position);
}
INLINE void XImageCtrl::setScale(const XVec2& s)
{
	m_scale = s;
	m_sprite.setScale(m_scale);
}
INLINE void XImageCtrl::fitScale(float x,float y)
{
	setScale(x / m_w,y / m_h);
}
INLINE void XImageCtrl::setColor(const XFColor& c)
{
	m_color = c;
	return m_sprite.setColor(m_color);
}
INLINE void XImageCtrl::setAlpha(float a)
{
	m_color.setA(a);
	return m_sprite.setAlpha(a);
}