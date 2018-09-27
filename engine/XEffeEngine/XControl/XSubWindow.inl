INLINE XBool XSubWindow::isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XSubWindow::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return XVec2::zero;
	switch(order)
	{
	case 0:return m_curMouseRect.getLT();
	case 1:return m_curMouseRect.getRT();
	case 2:return m_curMouseRect.getRB();
	case 3:return m_curMouseRect.getLB();
	}
	return XVec2::zero;
}
INLINE void XSubWindow::setTitle(const char * title)
{
	if(!m_isInited) return;
	if(title == NULL) m_titleString = "";
	else m_titleString = title;
	m_titleFont.setString(XString::getCanShowString(m_titleString.c_str(),(int)(m_area.getWidth() - 96.0f),15).c_str());
}
INLINE XBool XSubWindow::canGetFocus(const XVec2& p)				//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_windowType == TYPE_NO_TITLE) return XFalse;
//	XRect tmpRect;
//	tmpRect.set(m_curMouseRect.left,m_curMouseRect.top,
//		m_curMouseRect.right,m_curMouseRect.top + 32.0f * m_scale.y);
//	return tmpRect.isInRect(x,y);
	//如果所有的子物体中有能获取到焦点的物体，则自身能获取到焦点,当有漂浮在外面的控件时这个可能防止焦点穿透
	for (unsigned int i = 0;i < m_objects.size();++i)
	{
		if (m_objects[i].obj->canGetFocus(p)) return true;
	}
	return m_curMouseRect.isInRect(p);
}
INLINE XSubWindow::XShrinkDirection XSubWindow::calShrinkDir()	//计算当前可能的收缩方向
{
	if(m_isInShrink) return DIR_NULL;	//如果已处于收缩状态，则不能再进行收缩
	//这里需要根据窗的位置计算窗口可能的搜索方向
	//if(m_position.x < 2.0f) return DIR_LEFT;
	//1、鼠标靠边
	//2、控件靠边
	if(/*m_curMousePosition.x < 5.0f && */m_position.x < 5.0f) return DIR_LEFT;
	if(/*m_curMousePosition.x > XEE::windowWidth - 5.0f && */
		m_position.x > getWindowWidth() - m_curMouseRect.getWidth() - 5.0f) return DIR_RIGHT;
	if(/*m_curMousePosition.y < 5.0f && */m_position.y < 5.0f) return DIR_UP;
	return DIR_NULL;
}
INLINE void XSubWindow::update(float stepTime)
{
	m_titleButton[0].update(stepTime);
	m_titleButton[1].update(stepTime);
	m_titleButton[2].update(stepTime);

	updateShrinkState(stepTime);
	m_ctrlManager.update(stepTime);
}
INLINE XBool XSubWindow::getIsInDragRect(const XVec2& p)	//判断位置是否在拖拽区域内
{
	return XRect(m_curMouseRect.getLT(),
		m_curMouseRect.getRT() + XVec2(-96.0f, 32.0f) * m_scale).isInRect(p);
}
INLINE XVec2 XSubWindow::correctPos(const XVec2& pos)	//更具传入的拖拽位置，修正该位置，当位置出于非法状态时，修正成合法的位置
{
	XVec2 ret = pos;
	//if(pos.x < -(m_curMouseRect.getWidth() - 96.0f * m_scale.x - 5.0f)) ret.x = -(m_curMouseRect.getWidth() - 96.0f * m_scale.x - 5.0f);
	if(pos.x < 0.0f) ret.x = 0.0f;
	if(pos.x > getWindowWidth() - 5.0f) ret.x = getWindowWidth() - 5.0f;
	//if(pos.x > XEE::windowWidth - m_curMouseRect.getWidth()) ret.x = XEE::windowWidth - m_curMouseRect.getWidth();
	if(pos.y < 0.0f) ret.y = 0.0f;
	if(pos.y > getSceneHeight() - 5.0f) ret.y = getSceneHeight() - 5.0f;
	return ret;
}
	INLINE void XSubWindow::updateShrinkPosition()
	{
		switch(m_curShrinkDir)
		{
		case DIR_UP:		//向上收缩
			setPosition(m_position.x,(5.0f - m_curMouseRect.getHeight()) * m_shrinkRate);
			break;
		case DIR_LEFT:		//向左收缩
			setPosition((5.0f - m_curMouseRect.getWidth()) * m_shrinkRate,m_position.y);
			break;
		case DIR_RIGHT:		//向右收缩
			setPosition(getWindowWidth() - 5.0f * m_shrinkRate - m_curMouseRect.getWidth() * (1.0f - m_shrinkRate),m_position.y);
			break;
		case DIR_DOWN:		//向下收缩
			setPosition(m_position.x,getSceneHeight() - 5.0f * m_shrinkRate - m_curMouseRect.getHeight() * (1.0f - m_shrinkRate));
			break;
		}
	}
	INLINE XBool XSubWindow::keyboardProc(int keyOrder, XKeyState keyState)
	{
		if (!m_isInited ||	//如果没有初始化直接退出
			!m_isActive ||		//没有激活的控件不接收控制
			!m_isVisible) return XFalse; 	//如果不可见直接退出
		if (m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
		if (m_isSilent) return XFalse;
		m_ctrlManager.keyProc(keyOrder, keyState);
		return XFalse;
	}