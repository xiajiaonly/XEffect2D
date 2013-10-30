//inline 函数
inline void _XFontUnicode::setRotateBasePoint(float x,float y)
{
	m_rotateBasicPoint.set(x,y);
	setPosition(m_setPosition);
}

inline void _XFontUnicode::setColor(const _XFColor& color)
{
	m_sprite.setColor(color);
}

inline void _XFontUnicode::setColor(float r,float g,float b,float a)
{
	m_sprite.setColor(r,g,b,a);
}

inline void _XFontUnicode::setClipRect(_XRect temp)
{
	setClipRect(temp.left,temp.top,temp.right,temp.bottom);
}

inline void _XFontUnicode::setClipRect(float left,float top,float right,float bottom)
{
	m_clipRect.set(left,top,right,bottom);
	m_isCliped = 1;
	m_needUpdateData = 1;
}

inline void _XFontUnicode::disClip()
{
	if(m_isCliped != 0) 
	{
		m_isCliped = 0;
		m_needUpdateData = 1;
	}
}


inline int _XFontUnicode::getStringLengthPix()
{
	return getMaxPixelWidth();
	//int stringLength = 0;
	//for(int i =0;i < STRING_MAX_SIZE;++ i)
	//{
	//	if(m_string[i] == '\0') break;	//字符串结束
	//	if((unsigned char)m_string[i] < 128)
	//	{//ASCII
	//		stringLength += m_size.x * m_showSize.x * 0.5f;
	//		stringLength += m_distance;
	//	}else
	//	{//非ASCII
	//		if(m_isPassword == 0)
	//		{//非密码模式
	//			stringLength += m_size.x * m_showSize.x;
	//			stringLength += m_distance;
	//		}else
	//		{//密码模式
	//			stringLength += m_size.x * m_showSize.x * 0.5f;
	//			stringLength += m_distance;
	//		}
	//		++ i;
	//	}
	//}
	//return stringLength;
}