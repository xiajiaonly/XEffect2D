INLINE void XProgressRing::setPosition(const XVec2& p)
{
	m_position = p;
	m_caption.setPosition(m_position);
	float size = m_r0 > m_r1?m_r0:m_r1;
	m_curMouseRect.set(m_position - size * m_scale,
		m_position + size * m_scale);
}
INLINE void XProgressRing::setScale(const XVec2& s)
{
	m_scale = s;
	m_caption.setScale(m_scale);
	float size = m_r0 > m_r1?m_r0:m_r1;
	m_curMouseRect.set(m_position - size * m_scale,
		m_position + size * m_scale);
}
INLINE void XProgressRing::setTextColor(const XFColor& color)	//设置字体的颜色
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
INLINE void XProgressRing::setColor(const XFColor& c)
{
	m_color = c;
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
		sprintf_s(tempC,20,"%d%%",(int)(temp));
		m_caption.setString(tempC);
	}
	m_curValue = temp;
	float tmp = m_valueMD.getCurData();
	m_valueMD.setEx(tmp,m_curValue,0.5f);
}
INLINE void XProgressRing::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
//为了支持物件管理器管理控件，这里提供下面两个接口的支持
INLINE XBool XProgressRing::isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XProgressRing::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return XVec2::zero;
	switch(order)
	{
	case 0: return m_curMouseRect.getLT();
	case 1: return m_curMouseRect.getRT();
	case 2: return m_curMouseRect.getRB();
	case 3: return m_curMouseRect.getLB();
	}
	return XVec2::zero;
}