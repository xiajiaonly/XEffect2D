INLINE XBool XActionSimpleEx::isName(const char *name)
{
	if(name == NULL) return XFalse;
	return m_actionName == name;
}
INLINE void XActionSimpleEx::setPause()			//设置动作暂停
{
	if(m_actionState == ACTION_STATE_ACTION)
		m_actionState = ACTION_STATE_PAUSE;
}
INLINE void XActionSimpleEx::setResume()		//设置动作恢复
{
	if(m_actionState == ACTION_STATE_PAUSE)
		m_actionState = ACTION_STATE_ACTION;
}
INLINE void XActionSimpleEx::release()
{
	if(!m_isInited) return;
	if(m_funRelease != NULL) m_funRelease(m_pClass);
	if(m_type == ACTION_TYPE_COMPLEX) XMem::XDELETE_ARRAY(m_pAction);
	m_isInited = XFalse;
}
INLINE void XActionSimpleManager::move(float stepTime)		//遍历并推进所有的动作
{
	for(int i = 0;i < m_pActions.size();++ i)
	{
		m_pActions[i]->move(stepTime);	//这个不需要顺序
	}
}
INLINE void XActionSimpleManager::draw()					//遍历并描绘所有的动作
{
	for(int i = 0;i < m_pActions.size();++ i)
	{
		m_pActions[i]->draw();	//draw的顺序问题，这里估计需要进行一个映射
	}
}
INLINE XBool XActionSimpleManager::setStartAction(const char *actionName)
{
	int ret = getActionIndex(actionName);
	if(ret < 0) return XFalse;
	m_pActions[ret]->setStart();
	return XTrue;
}
INLINE XActionSimpleEx *XActionSimpleManager::getPAction(const char *actionName)
{
	int ret = getActionIndex(actionName);
	if(ret < 0) return NULL;
	return m_pActions[ret];
}
INLINE XBool XActionSimpleManager::getIsActionEnd(const char *actionName)
{
	XActionSimpleEx * temp = getPAction(actionName);
	if(temp == NULL) return XTrue;
	return temp->getIsEnd();
}