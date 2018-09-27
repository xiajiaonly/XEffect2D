INLINE XBool XPointCtrl::isInRect(const XVec2& p)
{
	if (!m_isInited) return XFalse;
	return XMath::getIsInRect(p, getBox(0), getBox(1), getBox(2), getBox(3));
}
INLINE XVec2 XPointCtrl::getBox(int order)
{
	if (!m_isInited) return XVec2::zero;

	switch (order)
	{
	case 0:return m_position - m_truePixelSize;
	case 1:return m_position + XVec2(m_truePixelSize.x, -m_truePixelSize.y);
	case 2:return m_position + m_truePixelSize;
	case 3:return m_position + XVec2(-m_truePixelSize.x, m_truePixelSize.y);
	}

	return XVec2::zero;
}
INLINE void XPointCtrl::setRange(float l,float t,float r,float b)
{
	m_withRange = true;
	m_range.set(l, t, r, b);
	updateData();
}
INLINE const XVec2& XPointCtrl::getValue() const
{
	if(m_withMap) return m_mapValue;
	return m_position;
}
INLINE void XPointCtrl::setValue(const XVec2& value)
{
	if (m_withMap)
	{
		m_position.x = XMath::mapping1D(value.x, m_mapRange.left, m_mapRange.right,
			m_range.left, m_range.right);
		m_position.y = XMath::mapping1D(value.y, m_mapRange.top, m_mapRange.bottom,
			m_range.top, m_range.bottom);
	}
	else
	{
		m_position = value;
	}
	updateData();
}
INLINE void XPointCtrl::setMapRange(float l, float t, float r, float b)
{
	m_withMap = true;
	m_mapRange.set(l, t, r, b);
}
//INLINE void XPointCtrl::setCallbackFun(void (* funDataChange)(void *,int),void *pClass)
//{
//	m_funDataChange = funDataChange;
//	m_pClass = pClass;
//}
INLINE XBool XPointCtrl::canGetFocus(const XVec2& p)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(p);
}
INLINE void XPointCtrl::setPosition(const XVec2& p)
{
	m_position = p;
	updateData();
	updateChildPos();
}
INLINE void XPointCtrl::setScale(const XVec2& s)
{
	m_scale = s;
	updateData();
	updateChildScale();
}
INLINE void XPointCtrl::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}	
INLINE void XPointCtrl::setColor(const XFColor& c)
{
	m_color = c;
	m_font.setColor(m_color);
	updateChildColor();
}
INLINE void XPointCtrl::setAlpha(float a)
{
	m_color.setA(a);
	m_font.setAlpha(a);
	updateChildAlpha();
}
INLINE XBool XPointCtrl::setFontString(const char *str)
{
	if (!m_withFont) return false;
	if (str == NULL)
	{
		m_autoString = true;
	}
	else
	{
		m_autoString = false;
		m_font.setString(str);
	}
	return true;
}