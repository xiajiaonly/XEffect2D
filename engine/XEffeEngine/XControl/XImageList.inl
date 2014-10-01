inline _XBool _XImageList::canGetFocus(float x,float y)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
	!m_isActive ||		//没有激活的控件不接收控制
	!m_isVisible ||	//如果不可见直接退出
	!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
inline void _XImageList::disable()		//使控件无效
{
	m_leftBtn.disable();	//左边的按钮
	m_rightBtn.disable();	//右边的按钮
	m_imageSld.disable();	//滑动条
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
inline void _XImageList::enable()		//使控件有效
{
	m_leftBtn.enable();	//左边的按钮
	m_rightBtn.enable();	//右边的按钮
	m_imageSld.enable();	//滑动条
	m_isEnable = XTrue;
}
inline void _XImageList::setVisible()
{
	m_leftBtn.setVisible();	//左边的按钮
	m_rightBtn.setVisible();	//右边的按钮
	m_imageSld.setVisible();	//滑动条
	m_isVisible = XTrue;
}
inline void _XImageList::disVisible()
{
	m_leftBtn.disVisible();	//左边的按钮
	m_rightBtn.disVisible();	//右边的按钮
	m_imageSld.disVisible();	//滑动条
	m_isVisible = XFalse;
	m_isBeChoose = XFalse;
}
inline void _XImageList::setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funSelectChange)(void *,int),
		void *pClass)
{
	m_funInit = funInit;
	m_funRelease = funRelease;
	m_funSelectChange = funSelectChange;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}	
//inline void _XImageList::setLostFocus()	//设置失去焦点
//{
//	if(!m_isInited ||	//如果没有初始化直接退出
//		!m_isActive ||		//没有激活的控件不接收控制
//		!m_isVisible ||	//如果不可见直接退出
//		!m_isEnable) return;		//如果无效则直接退出
//
//	//m_leftBtn.setLostFocus();
//	//m_rightBtn.setLostFocus();
//	//m_imageSld.setLostFocus();
//	m_isBeChoose = XFalse;
//}
inline void _XImageList::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_leftBtn.setPosition(m_position);
	m_rightBtn.setPosition(m_position.x + (m_buttonWidth + m_imageSize.x * m_showImageSum) * m_size.x,m_position.y);
	m_imageSld.setPosition(m_position.x + m_buttonWidth * m_size.x,m_position.y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	updateState(false);
}
inline _XBool _XImageList::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XImageList::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
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
inline void _XImageList::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	m_leftBtn.setColor(m_color);
	m_rightBtn.setColor(m_color);
	m_imageSld.setColor(m_color);
	for(int i = 0;i < m_imageList.size();++ i)
	{
		m_imageList[i]->setColor(m_color);
	}
}
inline void _XImageList::setAlpha(float a) 
{
	m_color.setA(a);
	m_leftBtn.setAlpha(a);
	m_rightBtn.setAlpha(a);
	m_imageSld.setAlpha(a);
	for(int i = 0;i < m_imageList.size();++ i)
	{
		m_imageList[i]->setAlpha(a);
	}
}
inline void _XImageList::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	m_leftBtn.drawUp();
	m_rightBtn.drawUp();
	m_imageSld.drawUp();
}
inline void _XImageList::update(int stepTime)
{
	m_leftBtn.update(stepTime);
	m_rightBtn.update(stepTime);
	m_imageSld.update(stepTime);
}