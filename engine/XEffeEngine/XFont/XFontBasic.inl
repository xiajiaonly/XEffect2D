INLINE XBool XFontBasic::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE XBool XFontBasic::setString(const char *p)	//设置显示的字符串
{
	if(p == NULL) return XFalse;
	if(strcmp(p,m_string) == 0) return XTrue;	//如果是相同的字符串则不设置
	if(strlen(p) < m_maxStringLen && m_maxStringLen >= 0)
	{
		strcpy(m_string,p);
	}else
	{
		assert(m_maxStringLen > 0);
		memcpy(m_string,p,m_maxStringLen);
		m_string[m_maxStringLen - 1] = '\0';	//给字符串一个结束符
		if(XString::isAtUnicodeEnd(m_string,m_maxStringLen - 3))
		{
			m_string[m_maxStringLen - 3] = '.';
			m_string[m_maxStringLen - 2] = '.';
			m_string[m_maxStringLen - 1] = '\0';
		}else
		{
			m_string[m_maxStringLen - 4] = '.';
			m_string[m_maxStringLen - 3] = '.';
			m_string[m_maxStringLen - 2] = '\0';
		}
		//strlen(p) >= m_maxStringLen
	}
	m_needUpdateData = XTrue;
	return XTrue;
}
INLINE void XFontBasic::setPosition(float x,float y)			//设置字体显示的位置
{
	m_setPosition.set(x,y);
	updateChildPos();
	XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set((int)(tempPosition.x),(int)(tempPosition.y));

	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setDistance(float distance)			//设置字与字之间的距离
{
	m_distance = distance;
	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setDistance(float x,float y)
{
	m_distance = x;
	m_distanceY = y;
	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setAngle(float angle)	//设置字体显示的角度
{
	m_angle = angle;
	updateChildAngle();
	m_sprite.setAngle(angle);
	m_angleSin = sin(angle * DEGREE2RADIAN);
	m_angleCos = cos(angle * DEGREE2RADIAN);
	XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set((int)(tempPosition.x),(int)(tempPosition.y));

	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setScale(float x,float y)	//设置字体的显示大小
{
	m_scale.set(x,y);
	updateChildScale();
	m_sprite.setScale(x,y);
	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setRotateBasePoint(float x,float y)
{
	m_rotateBasicPoint.set(x,y);
	setPosition(m_setPosition);
}
INLINE void XFontBasic::setColor(float r,float g,float b,float a)
{
	m_sprite.setColor(r,g,b,a);
	updateChildColor();
}
INLINE void XFontBasic::setAlpha(float a) 
{
	m_sprite.setAlpha(a);
	updateChildAlpha();
}
INLINE void XFontBasic::setVisible() 
{
	m_isVisible = XTrue;
	updateChildVisible();
}					//设置物件可见
INLINE void XFontBasic::disVisible() 
{
	m_isVisible = XFalse;
	updateChildVisible();
}
INLINE void XFontBasic::setClipRect(float left,float top,float right,float bottom)
{
	m_clipRect.set(left,top,right,bottom);
	m_isCliped = XTrue;
	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::disClip()
{
	if(!m_isCliped) return; 
	m_isCliped = XFalse;
	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setAlignmentModeX(XFontAlignmentModeX mode)
{
	if(mode == m_alignmentModeX) return;
	m_alignmentModeX = mode;
	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setAlignmentModeY(XFontAlignmentModeY mode)
{
	if(mode == m_alignmentModeY) return;
	m_alignmentModeY = mode;
	m_needUpdateData = XTrue;
}
INLINE int XFontBasic::getMaxPixelWidth()
{
	if(m_needUpdateData)
	{
		updateData();
		m_needUpdateData = XFalse;
	}
	return m_maxPixelWidth;
}
INLINE int XFontBasic::getMaxPixelHeight()
{
	if(m_needUpdateData)
	{
		updateData();
		m_needUpdateData = XFalse;
	}
	return m_maxPixelHeight;
}
INLINE void XFontBasic::setIsPassWord(XBool temp)
{
	if((m_isPassword && !temp) || (!m_isPassword && temp))
	{
		m_isPassword = temp;
		m_needUpdateData = XTrue;
	}
}