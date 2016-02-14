INLINE XBool XPointCtrl::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE XVector2 XPointCtrl::getBox(int order)
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
INLINE void XPointCtrl::setRange(float l,float t,float r,float b)
{
	m_withRange = true;
	m_range.set(l,t,r,b);
	updateData();
}
INLINE XVector2 XPointCtrl::getValue() 
{
	if(m_withMap) return m_mapValue;
	return m_position;
}
INLINE void XPointCtrl::setValue(const XVector2 &value)
{
	if(m_withMap)
	{
		m_position.x = XMath::maping1D(value.x,m_mapRange.left,m_mapRange.right,m_range.left,m_range.right);
		m_position.x = XMath::maping1D(value.y,m_mapRange.left,m_mapRange.right,m_range.left,m_range.right);
	}else
	{
		m_position = value;
	}
	updateData();
}
INLINE void XPointCtrl::setMapRange(float l,float t,float r,float b)
{
	m_withMap = true;
	m_mapRange.set(l,t,r,b);
}
//INLINE void XPointCtrl::setCallbackFun(void (* funDataChange)(void *,int),void *pClass)
//{
//	m_funDataChange = funDataChange;
//	m_pClass = pClass;
//}
INLINE XBool XPointCtrl::canGetFocus(float x,float y)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
INLINE void XPointCtrl::setPosition(float x,float y)
{
	m_position.set(x,y);
	updateData();
	updateChildPos();
}
INLINE void XPointCtrl::setScale(float x,float y) 
{
	m_scale.set(x,y);
	updateData();
	updateChildScale();
}
INLINE void XPointCtrl::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}	
INLINE void XPointCtrl::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_font.setColor(m_color);
	updateChildColor();
}
INLINE void XPointCtrl::setAlpha(float a)
{
	m_color.setA(a);
	m_font.setAlpha(a);
	updateChildAlpha();
}
