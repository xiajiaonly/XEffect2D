//INLINE нд╪Ч
INLINE XBool XNumberEx::setNumber(const char *temp)
{
	if(temp == NULL) return XFalse;
	if(strlen(temp) >= MAX_NUMBER_LENGTH) return XFalse;
	strcpy_s(m_number,MAX_NUMBER_LENGTH,temp);
	return XTrue;
}
INLINE void XNumberEx::setColor(const XFColor& c)
{
	m_sprite.setColor(c);
}
INLINE void XNumberEx::setAlpha(float a)
{
	m_sprite.setAlpha(a);
}
INLINE void XNumberEx::setScale(const XVec2& scale)
{
	m_showSize = scale;
	m_sprite.setScale(m_showSize);
}
//INLINE void XNumberEx::setScale(float x,float y)
//{
//	setScale(XVec2(x,y));
//}
INLINE void XNumberEx::setAngle(float angle)
{
	m_angle = angle;
	m_sprite.setAngle(angle);
	m_angleSin = sin(m_angle * DEGREE2RADIAN) ;
	m_angleCos = cos(m_angle * DEGREE2RADIAN) ;
	m_position = m_setPosition + XVec2(-m_rotateBasicPoint.x * m_angleCos
		+ m_rotateBasicPoint.y * m_angleSin,
		-m_rotateBasicPoint.x * m_angleSin - m_rotateBasicPoint.y * m_angleCos);
}
INLINE void XNumberEx::setRotateBasePoint(const XVec2& r)
{
	m_rotateBasicPoint = r;
	setPosition(m_setPosition);
}
INLINE void XNumberEx::setPosition(const XVec2& position)
{
	m_setPosition = position;
	m_position = m_setPosition + XVec2(-m_rotateBasicPoint.x * m_angleCos
		+ m_rotateBasicPoint.y * m_angleSin,
		-m_rotateBasicPoint.x * m_angleSin - m_rotateBasicPoint.y * m_angleCos);
}
//INLINE void XNumberEx::setPosition(float x,float y)
//{
//	setPosition(XVec2(x,y));
//}
INLINE XBool XNumberEx::release()
{
	if(!m_isInited) return XFalse;
	XMem::XDELETE_ARRAY(m_number);
	m_sprite.release();
	m_isInited = XFalse;
	return XTrue;
}