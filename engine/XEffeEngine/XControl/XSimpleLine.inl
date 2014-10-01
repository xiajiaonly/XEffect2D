inline void _XSimpleLine::setLostFocus() 
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	m_isBeChoose = XFalse;	//控件处于焦点状态
}
inline _XBool _XSimpleLine::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XSimpleLine::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{	
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0:return _XVector2(m_nowMouseRect.left,m_nowMouseRect.top);
	case 1:return _XVector2(m_nowMouseRect.right,m_nowMouseRect.top);
	case 2:return _XVector2(m_nowMouseRect.right,m_nowMouseRect.bottom);
	case 3:return _XVector2(m_nowMouseRect.left,m_nowMouseRect.bottom);
	}
	return _XVector2::zero;
}
inline void _XSimpleLine::setSize(float x,float y)
{
	m_size.set(x,y);
	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		m_nowMouseRect.set(m_position.x - 5.0f * m_size.x,m_position.y,
			m_position.x + 5.0f * m_size.x,m_position.y + m_lineLen * m_size.y);
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		m_nowMouseRect.set(m_position.x,m_position.y - 5.0f * m_size.y,
			m_position.x + m_lineLen * m_size.x,m_position.y + 5.0f * m_size.y);
		break;
	}
}
inline void _XSimpleLine::setPosition(float x,float y)
{
	m_position.set(x,y);
	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		m_nowMouseRect.set(m_position.x - 5.0f * m_size.x,m_position.y,
			m_position.x + 5.0f * m_size.x,m_position.y + m_lineLen * m_size.y);
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		m_nowMouseRect.set(m_position.x,m_position.y - 5.0f * m_size.y,
			m_position.x + m_lineLen * m_size.x,m_position.y + 5.0f * m_size.y);
		break;
	}
}