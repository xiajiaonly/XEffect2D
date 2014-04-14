//inline нд╪Ч
inline _XBool _XNumberEx::setNumber(const char *temp)
{
	if(temp == NULL) return 0;
	if(strlen(temp) >= MAX_NUMBER_LENGTH) return 0;
	strcpy(m_number,temp);
	return 1;
}

inline void _XNumberEx::setColor(float r,float g,float b,float a)
{
	m_sprite.setColor(r,g,b,a);
}

inline void _XNumberEx::setAlpha(float a)
{
	m_sprite.setAlpha(a);
}

inline void _XNumberEx::setSize(const _XVector2& size)
{
	setSize(size.x,size.y);
}

inline void _XNumberEx::setSize(float x,float y)
{
	m_showSize.set(x,y);
	m_sprite.setSize(x,y);
}

inline void _XNumberEx::setAngle(float angle)
{
	m_angle = angle;
	m_sprite.setAngle(angle);
	m_angleSin = sin(m_angle * ANGLE_TO_RADIAN) ;
	m_angleCos = cos(m_angle * ANGLE_TO_RADIAN) ;
	_XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set(tempPosition.x,tempPosition.y);
}

inline void _XNumberEx::setRotateBasePoint(float x,float y)
{
	m_rotateBasicPoint.set(x,y);
	setPosition(m_setPosition);
}
inline void _XNumberEx::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}
inline void _XNumberEx::setPosition(float x,float y)
{
	m_setPosition.set(x,y);
	_XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set(tempPosition.x,tempPosition.y);
}
inline _XBool _XNumberEx::release()
{
	if(!m_isInited) return XFalse;
	XDELETE_ARRAY(m_number);
	m_sprite.release();
	m_isInited = XFalse;
	return XTrue;
}
inline void _XMoveNumber::addArmNumber(float number)
{
	m_armNumber += number;
}
inline void _XMoveNumber::setArmNumber(float number)
{
	m_armNumber = number;
}
inline int _XMoveNumber::getNowNumber()
{
	return m_nowNumber;
}
inline int _XMoveNumber::getArmNumber()
{
	return (int)m_armNumber;
}
inline void _XMoveNumber::reset()
{
	m_nowNumber = 0;
	m_armNumber = 0.0f;
}
