inline _XBool _XLineCtrl::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
inline _XVector2 _XLineCtrl::getBox(int order)
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
inline void _XLineCtrl::setString(const char * str)
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
inline void _XLineCtrl::setRange(float min,float max)
{
	m_withRange = true;
	m_range.set(min,max);
	updateData();
}
inline float _XLineCtrl::getValue() 
{
	if(m_withMap) return m_mapValue;
	if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U) return m_position.x;
	else return m_position.y;
}
inline void _XLineCtrl::setValue(float value)
{
	if(m_withMap) 
	{
		m_mapValue = value;
		if(m_type == LINE_CTRL_TYPE_X_D || m_type == LINE_CTRL_TYPE_X_U) m_position.x = maping1D(m_mapValue,m_mapRange.x,m_mapRange.y,m_range.x,m_range.y);
		else m_position.y = maping1D(m_mapValue,m_mapRange.x,m_mapRange.y,m_range.x,m_range.y);
		updateData();
	}
}
inline void _XLineCtrl::setMapRange(float min,float max)
{
	m_withMap = true;
	m_mapRange.set(min,max);
}
inline void _XLineCtrl::setCallbackFun(void (* funDataChange)(void *,int),void *pClass)
{
	m_funDataChange = funDataChange;
	m_pClass = pClass;
}
inline _XBool _XLineCtrl::canGetFocus(float x,float y)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
inline void _XLineCtrl::setPosition(float x,float y)
{
	m_position.set(x,y);
	updateData();
	updateChildPos();
}
inline void _XLineCtrl::setSize(float x,float y) 
{
	m_size.set(x,y);
	updateData();
	updateChildSize();
}
inline void _XLineCtrl::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	updateChildColor();
}
inline void _XLineCtrl::setAlpha(float a)
{
	m_color.setA(a);
	updateChildAlpha();
}
inline void _XLineCtrl::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}	