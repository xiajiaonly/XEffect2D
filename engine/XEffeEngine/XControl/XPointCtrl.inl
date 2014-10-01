inline _XBool _XPointCtrl::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
inline _XVector2 _XPointCtrl::getBox(int order)
{
	if(!m_isInited) return _XVector2::zero;

	switch(order)
	{
	case 0:return _XVector2(m_position.x - m_truePixelSize.x,m_position.y - m_truePixelSize.y);
	case 1:return _XVector2(m_position.x + m_truePixelSize.x,m_position.y - m_truePixelSize.y);
	case 2:return _XVector2(m_position.x + m_truePixelSize.x,m_position.y + m_truePixelSize.y);
	case 3:return _XVector2(m_position.x - m_truePixelSize.x,m_position.y + m_truePixelSize.y);
	}

	return _XVector2::zero;
}
inline void _XPointCtrl::setRange(float l,float t,float r,float b)
{
	m_withRange = true;
	m_range.set(l,t,r,b);
	updateData();
}
inline _XVector2 _XPointCtrl::getValue() 
{
	if(m_withMap) return m_mapValue;
	return m_position;
}
inline void _XPointCtrl::setMapRange(float l,float t,float r,float b)
{
	m_withMap = true;
	m_mapRange.set(l,t,r,b);
}
inline void _XPointCtrl::setCallbackFun(void (* funDataChange)(void *,int),void *pClass)
{
	m_funDataChange = funDataChange;
	m_pClass = pClass;
}
inline _XBool _XPointCtrl::canGetFocus(float x,float y)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
inline void _XPointCtrl::setPosition(float x,float y)
{
	m_position.set(x,y);
	updateData();
	updateChildPos();
}
inline void _XPointCtrl::setSize(float x,float y) 
{
	m_size.set(x,y);
	updateData();
	updateChildSize();
}
inline void _XPointCtrl::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}	
inline void _XPointCtrl::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_font.setColor(m_color);
	updateChildColor();
}
inline void _XPointCtrl::setAlpha(float a)
{
	m_color.setA(a);
	m_font.setAlpha(a);
	updateChildAlpha();
}
