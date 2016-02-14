INLINE void XSimpleLine::setLostFocus() 
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	m_isBeChoose = XFalse;	//控件处于焦点状态
}
INLINE XBool XSimpleLine::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XSimpleLine::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{	
	if(!m_isInited) return XVector2::zero;
	switch(order)
	{
	case 0:return XVector2(m_curMouseRect.left,m_curMouseRect.top);
	case 1:return XVector2(m_curMouseRect.right,m_curMouseRect.top);
	case 2:return XVector2(m_curMouseRect.right,m_curMouseRect.bottom);
	case 3:return XVector2(m_curMouseRect.left,m_curMouseRect.bottom);
	}
	return XVector2::zero;
}
INLINE void XSimpleLine::setScale(float x,float y)
{
	m_scale.set(x,y);
	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		m_curMouseRect.set(m_position.x - 5.0f * m_scale.x,m_position.y,
			m_position.x + 5.0f * m_scale.x,m_position.y + m_lineLen * m_scale.y);
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		m_curMouseRect.set(m_position.x,m_position.y - 5.0f * m_scale.y,
			m_position.x + m_lineLen * m_scale.x,m_position.y + 5.0f * m_scale.y);
		break;
	}
}
INLINE void XSimpleLine::setPosition(float x,float y)
{
	m_position.set(x,y);
	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		m_curMouseRect.set(m_position.x - 5.0f * m_scale.x,m_position.y,
			m_position.x + 5.0f * m_scale.x,m_position.y + m_lineLen * m_scale.y);
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		m_curMouseRect.set(m_position.x,m_position.y - 5.0f * m_scale.y,
			m_position.x + m_lineLen * m_scale.x,m_position.y + 5.0f * m_scale.y);
		break;
	}
}