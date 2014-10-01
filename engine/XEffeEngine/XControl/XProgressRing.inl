inline void _XProgressRing::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_caption.setPosition(m_position);
	float size = m_r0 > m_r1?m_r0:m_r1;
	m_nowMouseRect.set(m_position.x - size * m_size.x,m_position.y - size * m_size.y,
		m_position.x + size * m_size.x,m_position.y + size * m_size.y);
}
inline void _XProgressRing::setSize(float x,float y)
{
	m_size.set(x,y);
	m_caption.setSize(m_size);
	float size = m_r0 > m_r1?m_r0:m_r1;
	m_nowMouseRect.set(m_position.x - size * m_size.x,m_position.y - size * m_size.y,
		m_position.x + size * m_size.x,m_position.y + size * m_size.y);
}
inline void _XProgressRing::setTextColor(const _XFColor& color)	//设置字体的颜色
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
inline void _XProgressRing::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_caption.setColor(m_textColor * m_color);
	updateChildColor();
}
inline void _XProgressRing::setAlpha(float a)
{
	m_color.setA(a);
	m_caption.setColor(m_textColor * m_color);
	updateChildAlpha();
}
inline void _XProgressRing::setValue(float temp)
{
	if(!m_isInited) return;
	if(temp < 0.0f) temp = 0.0f;
	if(temp > 100.0f) temp = 100.0f;
	int tempI = temp;
	if(tempI == (int)m_nowValue) return;
	m_nowValue = temp;
	char tempC[20] = "100%";
	sprintf(tempC,"%d%%",tempI);
	m_caption.setString(tempC);
}
inline void _XProgressRing::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
//为了支持物件管理器管理控件，这里提供下面两个接口的支持
inline _XBool _XProgressRing::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XProgressRing::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0: return _XVector2(m_nowMouseRect.left,m_nowMouseRect.top);
	case 1: return _XVector2(m_nowMouseRect.right,m_nowMouseRect.top);
	case 2: return _XVector2(m_nowMouseRect.right,m_nowMouseRect.bottom);
	case 3: return _XVector2(m_nowMouseRect.left,m_nowMouseRect.bottom);
	}
	return _XVector2::zero;
}