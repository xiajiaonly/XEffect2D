//INLINE  函数
//INLINE  void XNodeLine::setOneNode(float x, float y,int nodeOrder)
//{
//	setOneNode(XVec2(x, y),nodeOrder);
//	m_needUpdateData = XTrue;
//}
//INLINE  void XNodeLine::addOneNode(float x,float y)
//{
//	addOneNode(XVec2(x, y));
//	m_needUpdateData = XTrue;
//}
//INLINE  void XNodeLine::addOneNode(float x, float y, int nodeOrder)
//{
//	addOneNode(XVec2(x,y),nodeOrder);
//	m_needUpdateData = XTrue;
//}
INLINE  int XNodeLine::getOrder() const
{
	return m_myOrder;
}
INLINE  void XNodeLine::setMyOrder(int temp)
{
	if (temp < 0 || temp >= 100000) return;
	m_myOrder = temp;
}
INLINE  void XNodeLine::setSpecialPointSpeed(float speed)	//设置逆向曲线的速度
{
	m_specialPoint.speed = speed;
}
INLINE  XBool XNodeLine::getSpecialPointIsEnd() const
{
	return m_specialPoint.isEnd;
}
INLINE  XVec2 XNodeLine::getSpecialPointPosition() const		//获取曲线中的点的位置
{
	return m_specialPoint.position * m_scale + m_offsetPosition;
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
INLINE  void XNodeLine::setPosition(const XVec2& p)	//设置曲线的偏移位置
{
	m_offsetPosition = p;
}
INLINE  void XNodeLine::release()		//释放资源
{
	//	if(m_isACopy == 0)
	//	{
	//		XMem::XDELETE_ARRAY(m_node);
	//	}
	m_node.clear();
}