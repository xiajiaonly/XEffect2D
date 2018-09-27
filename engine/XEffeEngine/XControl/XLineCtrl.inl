INLINE XBool XLineCtrl::isInRect(const XVec2& p)
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(p,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE XVec2 XLineCtrl::getBox(int order)
{
	if(!m_isInited) return XVec2::zero;

	switch(order)
	{
	case 0:return m_position - m_truePixelSize;
	case 1:return m_position + XVec2(m_truePixelSize.x, - m_truePixelSize.y);
	case 2:return m_position + m_truePixelSize;
	case 3:return m_position + XVec2(- m_truePixelSize.x,m_truePixelSize.y);
	}

	return XVec2::zero;
}
INLINE void XLineCtrl::setString(const char * str)
{
	if(!m_withFont) return;
	if(str == NULL)
	{
		m_withString = false;
		return;
	}
	m_showString = str;
	m_stringFont.setString(m_showString.c_str());
	m_withString = true;
}
INLINE void XLineCtrl::setRange(float minRange,float maxRange)
{
	m_withRange = true;
	m_range.set(minRange, maxRange);
	updateData();
}
INLINE float XLineCtrl::getValue() 
{
	if(m_withMap) return m_mapValue;
	if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U) return m_position.x;
	else return m_position.y;
}
INLINE void XLineCtrl::setValue(float value)
{
	if(m_withMap) 
	{
		m_mapValue = value;
		if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U) 
			m_position.x = XMath::mapping1D(m_mapValue,m_mapRange.x,m_mapRange.y,m_range.x,m_range.y);
		else 
			m_position.y = XMath::mapping1D(m_mapValue,m_mapRange.x,m_mapRange.y,m_range.x,m_range.y);
		updateData();
	}
}
INLINE void XLineCtrl::setMapRange(float min,float max)
{
	m_withMap = true;
	m_mapRange.set(min,max);
}
//INLINE void XLineCtrl::setCallbackFun(void (* funDataChange)(void *,int),void *pClass)
//{
//	m_funDataChange = funDataChange;
//	m_pClass = pClass;
//}
INLINE XBool XLineCtrl::canGetFocus(const XVec2& p)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(p);
}
INLINE void XLineCtrl::setPosition(const XVec2& p)
{
	m_position = p;
	updateData();
	updateChildPos();
}
INLINE void XLineCtrl::setScale(const XVec2& s)
{
	m_scale = s;
	updateData();
	updateChildScale();
}
INLINE void XLineCtrl::setColor(const XFColor& c)
{
	m_color = c;
	updateChildColor();
}
INLINE void XLineCtrl::setAlpha(float a)
{
	m_color.setA(a);
	updateChildAlpha();
}
INLINE void XLineCtrl::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}	