INLINE void XProgressRing::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_caption.setPosition(m_position);
	float size = m_r0 > m_r1?m_r0:m_r1;
	m_curMouseRect.set(m_position.x - size * m_scale.x,m_position.y - size * m_scale.y,
		m_position.x + size * m_scale.x,m_position.y + size * m_scale.y);
}
INLINE void XProgressRing::setScale(float x,float y)
{
	m_scale.set(x,y);
	m_caption.setScale(m_scale);
	float size = m_r0 > m_r1?m_r0:m_r1;
	m_curMouseRect.set(m_position.x - size * m_scale.x,m_position.y - size * m_scale.y,
		m_position.x + size * m_scale.x,m_position.y + size * m_scale.y);
}
INLINE void XProgressRing::setTextColor(const XFColor& color)	//设置字体的颜色
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
INLINE void XProgressRing::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_caption.setColor(m_textColor * m_color);
	updateChildColor();
}
INLINE void XProgressRing::setAlpha(float a)
{
	m_color.setA(a);
	m_caption.setColor(m_textColor * m_color);
	updateChildAlpha();
}
INLINE void XProgressRing::setValue(float temp)
{
	if(!m_isInited) return;
	if(temp < 0.0f) temp = 0.0f;
	if(temp > 100.0f) temp = 100.0f;
//	int tempI = (int)temp;
//	if(tempI == (int)m_curValue) return;
	if((int)(m_curValue) != (int)(temp)) 
	{
		char tempC[20] = "100%";
		sprintf(tempC,"%d%%",(int)(temp));
		m_caption.setString(tempC);
	}
	m_curValue = temp;
}
INLINE void XProgressRing::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
//为了支持物件管理器管理控件，这里提供下面两个接口的支持
INLINE XBool XProgressRing::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XProgressRing::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return XVector2::zero;
	switch(order)
	{
	case 0: return XVector2(m_curMouseRect.left,m_curMouseRect.top);
	case 1: return XVector2(m_curMouseRect.right,m_curMouseRect.top);
	case 2: return XVector2(m_curMouseRect.right,m_curMouseRect.bottom);
	case 3: return XVector2(m_curMouseRect.left,m_curMouseRect.bottom);
	}
	return XVector2::zero;
}