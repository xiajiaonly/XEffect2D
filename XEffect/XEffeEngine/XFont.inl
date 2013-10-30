//inline º¯Êý
inline void _XFont::setColor(float r,float g,float b,float a)
{
	m_sprite.setColor(r,g,b,a);
}

inline void _XFont::setClipRect(_XRect temp)
{
	setClipRect(temp.left,temp.top,temp.right,temp.bottom);
}

inline void _XFont::setClipRect(float left,float top,float right,float bottom)
{
	m_clipRect.set(left,top,right,bottom);
	m_isCliped = 1;
	m_needUpdateData = 1;
}

inline void _XFont::disClip()
{
	if(m_isCliped != 0) 
	{
		m_isCliped = 0;

		m_needUpdateData = 1;
	}
}
