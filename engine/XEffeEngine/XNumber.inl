//inline нд╪Ч
inline _XBool _XNumber::setNumber(const char *temp)
{
	if(temp == NULL) return XFalse;
	if(strlen(temp) >= MAX_NUMBER_LENGTH) return XFalse;
	strcpy(m_string,temp);
	m_needUpdateData = XTrue;
	return XTrue;
}
inline void _XNumber::setColor(float r,float g,float b,float a)
{
	m_sprite.setColor(r,g,b,a);
	updateChildColor();
	m_alpha = m_sprite.getAlpha();
}
inline void _XNumber::setAlpha(float a)
{
	m_sprite.setAlpha(a);
	updateChildAlpha();
	m_alpha = m_sprite.getAlpha();
}
inline void _XNumber::setSize(float x,float y)
{
	m_showSize.set(x,y);
	updateChildSize();
	m_sprite.setSize(x,y);
	m_needUpdateData = 1;
}
inline void _XNumber::setAngle(float angle)
{
	m_angle = angle;
	updateChildAngle();
	m_sprite.setAngle(angle);
	m_angleSin = sin(m_angle * ANGLE_TO_RADIAN);
	m_angleCos = cos(m_angle * ANGLE_TO_RADIAN);

	_XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set(tempPosition.x,tempPosition.y);

	m_needUpdateData = 1;
}
inline void _XNumber::setRotateBasePoint(float x,float y)
{
	m_rotateBasicPoint.set(x,y);
	setPosition(m_setPosition);
}
inline void _XNumber::setPosition(float x,float y)
{
	m_setPosition.set(x,y);
	updateChildPos();
	_XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set(tempPosition.x,tempPosition.y);

	m_needUpdateData = 1;
}
inline void _XNumber::setPositionX(float x)
{
	setPosition(x,m_setPosition.y);
}
inline void _XNumber::setPositionY(float y)
{
	setPosition(m_setPosition.x,y);
}