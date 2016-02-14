//INLINE  函数
INLINE  void XNodeLine::setOneNode(const XVector2& node,int nodeOrder)
{
	setOneNode(node.x,node.y,nodeOrder);
	m_needUpdateData = XTrue;
}
INLINE  void XNodeLine::addOneNode(const XVector2& node)
{
	addOneNode(node.x,node.y);
	m_needUpdateData = XTrue;
}
INLINE  void XNodeLine::addOneNode(const XVector2& node,int nodeOrder)
{
	addOneNode(node.x,node.y,nodeOrder);
	m_needUpdateData = XTrue;
}
INLINE  int XNodeLine::getOrder() const
{
	return m_myOrder;
}
INLINE  void XNodeLine::setMyOrder(int temp)
{
	if(temp < 0) return;
	if(temp >= 100000) return;
	m_myOrder = temp;
}
INLINE  void XNodeLine::setLeadMode(XLeadMode mode)
{
	if(mode == LEAD_MODE_FOREWORD)
	{
		m_leadMode = LEAD_MODE_FOREWORD;
	}else
	if(mode == LEAD_MODE_BACKWORD)
	{
		m_leadMode = LEAD_MODE_BACKWORD;
	}
}
INLINE  void XNodeLine::setSpecialPointSpeed(float speed)	//设置逆向曲线的速度
{
	m_specialPoint.speed = speed;
}
INLINE  XBool XNodeLine::getSpecialPointIsEnd() const
{
	return m_specialPoint.isEnd;
}
INLINE  XVector2 XNodeLine::getSpecialPointPosition() const		//获取曲线中的点的位置
{
	return XVector2(m_specialPoint.position.x * m_scale.x + m_offsetPosition.x,m_specialPoint.position.y * m_scale.y + m_offsetPosition.y);
}
INLINE  float XNodeLine::getSpecialPointAngle() const
{
	return m_specialPoint.angle;
}
//INLINE  int XNodeLine::setScale(float scale)			//设置曲线的尺寸
//{
//	if(size <= 0) return 0;
//	m_scale = scale;
//	return 1;
//}
INLINE  void XNodeLine::setPosition(float x,float y)	//设置曲线的偏移位置
{
	m_offsetPosition.set(x,y);
}
INLINE  void XNodeLine::release()		//释放资源
{
//	if(m_isACopy == 0)
//	{
//		XMem::XDELETE_ARRAY(m_node);
//	}
	m_nodeSum = 0;
}