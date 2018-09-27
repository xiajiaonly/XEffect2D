//INLINE 文件
INLINE XBool XNumber::setNumber(const char *temp)
{
	if(temp == NULL) return XFalse;
	if(strlen(temp) >= MAX_NUMBER_LENGTH) return XFalse;
	strcpy_s(m_string,MAX_NUMBER_LENGTH,temp);
	m_needUpdateData = XTrue;
	return XTrue;
}
INLINE void XNumber::setColor(const XFColor& c)
{
	m_sprite.setColor(c);
	updateChildColor();
	m_alpha = m_sprite.getAlpha();
}
INLINE void XNumber::setAlpha(float a)
{
	m_sprite.setAlpha(a);
	updateChildAlpha();
	m_alpha = m_sprite.getAlpha();
}
INLINE void XNumber::setScale(const XVec2& s)
{
	m_scale = s;
	updateChildScale();
	m_sprite.setScale(s);
	m_needUpdateData = XTrue;
}
INLINE void XNumber::setAngle(float angle)
{
	m_angle = angle;
	updateChildAngle();
	m_sprite.setAngle(angle);
	m_angleSin = sin(m_angle * DEGREE2RADIAN);
	m_angleCos = cos(m_angle * DEGREE2RADIAN);

	int x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	int y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set(x, y);

	m_needUpdateData = XTrue;
}
INLINE void XNumber::setRotateBasePoint(const XVec2& r)
{
	m_rotateBasicPoint = r;
	setPosition(m_setPosition);
}
INLINE void XNumber::setPosition(const XVec2& p)
{
	m_setPosition = p;
	updateChildPos();
	int x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	int y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set(x, y);

	m_needUpdateData = XTrue;
}
INLINE void XNumber::setPositionX(float x)
{
	setPosition(x,m_setPosition.y);
}
INLINE void XNumber::setPositionY(float y)
{
	setPosition(m_setPosition.x,y);
}
INLINE XBool XNumber::isInRect(const XVec2& p)
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(p, getBox(0), getBox(1), getBox(2), getBox(3));
}
INLINE void XNumber::setAlignmentMode(XNumberAlignmentMode mode)
{
	if(mode != m_alignmentMode)
	{
		m_alignmentMode = mode;
		m_needUpdateData = XTrue;
	}
}
INLINE void XNumber::setVisible() 
{
	m_isVisible = XTrue;
	updateChildVisible();
}					//设置物件可见
INLINE void XNumber::disVisible() 
{
	m_isVisible = XFalse;
	updateChildVisible();
}
INLINE int XNumber::getMaxPixelWidth()
{
	if(m_needUpdateData)
	{
		updateData();
		m_needUpdateData = XFalse;
	}
	return m_maxPixelWidth;
}
INLINE int XNumber::getMaxPixelHeight()
{
	if(m_needUpdateData)
	{
		updateData();
		m_needUpdateData = XFalse;
	}
	return m_maxPixelHeight;
}