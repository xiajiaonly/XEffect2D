INLINE XBool XLineCtrl::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE XVector2 XLineCtrl::getBox(int order)
{
	if(!m_isInited) return XVector2::zero;

	switch(order)
	{
	case 0:return XVector2(m_position.x - m_truePixelSize.x,m_position.y - m_truePixelSize.y);
	case 1:return XVector2(m_position.x + m_truePixelSize.x,m_position.y - m_truePixelSize.y);
	case 2:return XVector2(m_position.x + m_truePixelSize.x,m_position.y + m_truePixelSize.y);
	case 3:return XVector2(m_position.x - m_truePixelSize.x,m_position.y + m_truePixelSize.y);
	}

	return XVector2::zero;
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
INLINE void XLineCtrl::setRange(float min,float max)
{
	m_withRange = true;
	m_range.set(min,max);
	updateData();
}
INLINE float XLineCtrl::getValue() 
{
	if(m_withMap) return m_mapValue;
	if(m_type == LINE_CTRL_TYPEX_D || m_type == LINE_CTRL_TYPEX_U) return m_position.x;
	else return m_position.y;
}
INLINE void XLineCtrl::setValue(float value)
{
	if(m_withMap) 
	{
		m_mapValue = value;
		if(m_type == LINE_CTRL_TYPEX_D || m_type == LINE_CTRL_TYPEX_U) 
			m_position.x = XMath::maping1D(m_mapValue,m_mapRange.x,m_mapRange.y,m_range.x,m_range.y);
		else 
			m_position.y = XMath::maping1D(m_mapValue,m_mapRange.x,m_mapRange.y,m_range.x,m_range.y);
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
INLINE XBool XLineCtrl::canGetFocus(float x,float y)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
INLINE void XLineCtrl::setPosition(float x,float y)
{
	m_position.set(x,y);
	updateData();
	updateChildPos();
}
INLINE void XLineCtrl::setScale(float x,float y) 
{
	m_scale.set(x,y);
	updateData();
	updateChildScale();
}
INLINE void XLineCtrl::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
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