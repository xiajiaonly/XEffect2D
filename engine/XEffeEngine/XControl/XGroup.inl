INLINE void XGroup::updateData()		//更新内部数据
{
	if(m_state == STATE_NORMAL)
	{
		m_caption.setPosition(m_position.x + (m_rect.left + m_groupStateBtnSize + 5.0f) * m_scale.x,m_position.y + m_rect.top * m_scale.y);
	//	m_stateBotton.setPosition(m_position.x + (m_rect.right - m_groupStateBtnSize) * m_scale.x,m_position.y + m_rect.top * m_scale.y);
		m_stateBotton.setPosition(m_position.x,m_position.y + m_rect.top * m_scale.y);
		m_drawRect.set(m_rect.getLT() * m_scale + m_position, m_rect.getRB() * m_scale + m_position);
	}else
	{
		m_caption.setPosition(m_position.x + (m_groupStateBtnSize + 3.0f) * m_scale.x,m_position.y + 3.0f * m_scale.y);
	//	m_stateBotton.setPosition(m_position.x + 3.0f * m_scale.x + m_caption.getMaxPixelWidth(),m_position.y + 3.0f * m_scale.y);
		m_stateBotton.setPosition(m_position.x,m_position.y + 3.0f * m_scale.y);
		m_drawRect.left = m_rect.left * m_scale.x + m_position.x;
		m_drawRect.top = m_rect.top * m_scale.y + m_position.y;
		m_drawRect.right = m_drawRect.left + (6.0f + m_groupStateBtnSize) * m_scale.x + m_caption.getMaxPixelWidth();
		m_drawRect.bottom = m_drawRect.top + (6.0f + m_groupStateBtnSize) * m_scale.x;
	}
}
//INLINE void XGroup::setCallbackFun(void (* funStateChange)(void *,int),
//		void *pClass)
//{
//	m_funStateChange = funStateChange;
//	m_pClass = pClass;
//}
INLINE void XGroup::resetSize(const XVec2& size)
{
	m_rect.right = size.x;
	m_rect.bottom = size.y;
	updateData();
}
INLINE void XGroup::setPosition(const XVec2& p)
{
	m_position = p;
	updateData();
	updateChildPos();
}
INLINE void XGroup::setScale(const XVec2& s)
{
	m_scale = s;
	m_caption.setScale(m_textSize * m_scale);
	m_stateBotton.setScale(m_scale);
	updateData();
	updateChildScale();
}
INLINE void XGroup::setCaptionText(const char *caption)						//设置按钮的标题的文字
{
	if(caption == NULL) return;
	m_caption.setString(caption);
}
INLINE XBool XGroup::mouseProc(const XVec2&,XMouseState)
{
//	if(!m_isInited) return XTrue;	//如果没有初始化直接退出
//	if(!m_isVisible) return XTrue;	//如果不可见直接退出
//	m_stateBotton.mouseProc(x,y,mouseState);
	return XTrue;
}		//do nothing
INLINE XBool XGroup::keyboardProc(int,XKeyState)
{
//	if(!m_isInited) return XTrue;	//如果没有初始化直接退出
//	if(!m_isVisible) return XTrue;	//如果不可见直接退出
//	m_stateBotton.keyboardProc(keyOrder,keyState);
	return XTrue;
}
INLINE XBool XGroup::isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(p,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE void XGroup::setVisible()
{
	m_isVisible = XTrue;
	m_stateBotton.setVisible();
	if(m_state == STATE_NORMAL)
	{//隐藏所有子物体
		for(unsigned int i = 0;i < m_childList.size();++ i)
		{
			m_childList[i]->setVisible();
		}
	}else
	{//显示所有子物体
		for(unsigned int i = 0;i < m_childList.size();++ i)
		{
			m_childList[i]->disVisible();
		}
	}
	//updateChildVisible();
}
INLINE void XGroup::disVisible()
{
	m_isVisible = XFalse;
	m_isBeChoose = XFalse;
	m_comment.disVisible();
	m_stateBotton.disVisible();
	updateChildVisible();
}
INLINE XVec2 XGroup::getBox(int order)
{
	if(!m_isInited) return XVec2::zero;
	switch(order)
	{
	case 0: return m_drawRect.getLT();
	case 1: return m_drawRect.getRT();
	case 2: return m_drawRect.getRB();
	case 3: return m_drawRect.getLB();
	}
	return XVec2::zero;
}
INLINE void XGroup::setColor(const XFColor& c)
{
	m_color = c;
	m_caption.setColor(m_color);
	m_stateBotton.setColor(m_color);
	updateChildColor();
}
INLINE void XGroup::setAlpha(float a)
{
	m_color.setA(a);
	m_caption.setAlpha(a);
	m_stateBotton.setAlpha(a);
	updateChildAlpha();
}