//INLINE нд╪Ч
INLINE XBool XNumberEx::setNumber(const char *temp)
{
	if(temp == NULL) return XFalse;
	if(strlen(temp) >= MAX_NUMBER_LENGTH) return XFalse;
	strcpy(m_number,temp);
	return XTrue;
}
INLINE void XNumberEx::setColor(float r,float g,float b,float a)
{
	m_sprite.setColor(r,g,b,a);
}
INLINE void XNumberEx::setAlpha(float a)
{
	m_sprite.setAlpha(a);
}
INLINE void XNumberEx::setScale(const XVector2& scale)
{
	setScale(scale.x,scale.y);
}
INLINE void XNumberEx::setScale(float x,float y)
{
	m_showSize.set(x,y);
	m_sprite.setScale(x,y);
}
INLINE void XNumberEx::setAngle(float angle)
{
	m_angle = angle;
	m_sprite.setAngle(angle);
	m_angleSin = sin(m_angle * DEGREE2RADIAN) ;
	m_angleCos = cos(m_angle * DEGREE2RADIAN) ;
	XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set(tempPosition.x,tempPosition.y);
}
INLINE void XNumberEx::setRotateBasePoint(float x,float y)
{
	m_rotateBasicPoint.set(x,y);
	setPosition(m_setPosition);
}
INLINE void XNumberEx::setPosition(const XVector2& position)
{
	setPosition(position.x,position.y);
}
INLINE void XNumberEx::setPosition(float x,float y)
{
	m_setPosition.set(x,y);
	XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set(tempPosition.x,tempPosition.y);
}
INLINE XBool XNumberEx::release()
{
	if(!m_isInited) return XFalse;
	XMem::XDELETE_ARRAY(m_number);
	m_sprite.release();
	m_isInited = XFalse;
	return XTrue;
}