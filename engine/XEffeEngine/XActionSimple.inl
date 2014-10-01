inline _XBool _XActionSimpleEx::isName(const char *name)
{
	if(name == NULL) return XFalse;
	if(strcmp(name,m_actionName) == 0) return XTrue;
	return XFalse;
}
inline void _XActionSimpleEx::setPause()			//设置动作暂停
{
	if(m_actionState == ACTION_STATE_ACTION)
		m_actionState = ACTION_STATE_PAUSE;
}
inline void _XActionSimpleEx::setResume()		//设置动作恢复
{
	if(m_actionState == ACTION_STATE_PAUSE)
		m_actionState = ACTION_STATE_ACTION;
}
inline void _XActionSimpleEx::release()
{
	if(!m_isInited) return;
	if(m_funRelease != NULL) m_funRelease(m_pClass);
	if(m_type == ACTION_TYPE_COMPLEX) XDELETE_ARRAY(m_pAction);
	m_isInited = XFalse;
}
inline void _XActionSimpleManager::move(int stepTime)		//遍历并推进所有的动作
{
	for(int i = 0;i < MAX_ACTION_SUM;++ i)
	{
		if(m_pActions[i].actionIsEnable) m_pActions[i].pAction->move(stepTime);	//这个不需要顺序
	}
}
inline void _XActionSimpleManager::draw()					//遍历并描绘所有的动作
{
	for(int i = 0;i < MAX_ACTION_SUM;++ i)
	{
		if(m_pActions[i].actionIsEnable) m_pActions[i].pAction->draw();	//draw的顺序问题，这里估计需要进行一个映射
	}
}
inline _XBool _XActionSimpleManager::setStartAction(const char *actionName)
{
	int ret = getActionHandle(actionName);
	if(ret < 0) return XFalse;
	m_pActions[ret].pAction->setStart();
	return XTrue;
}
inline _XBool _XActionSimpleManager::setStartAction(_XActionSimpleHandle actionHandle)
{
	if(actionHandle >= 0 && actionHandle < MAX_ACTION_SUM
		&& m_pActions[actionHandle].actionIsEnable)
	{
		m_pActions[actionHandle].pAction->setStart();
		return XTrue;
	}
	return XFalse;
}
inline _XActionSimpleEx *_XActionSimpleManager::getPAction(const char *actionName)
{
	int ret = getActionHandle(actionName);
	if(ret < 0) return NULL;
	return m_pActions[ret].pAction;
}
inline _XActionSimpleEx *_XActionSimpleManager::getPAction(_XActionSimpleHandle actionHandle)
{
	if(actionHandle >= 0 && actionHandle < MAX_ACTION_SUM
		&& m_pActions[actionHandle].actionIsEnable) return m_pActions[actionHandle].pAction;
	return NULL;
}
inline _XBool _XActionSimpleManager::getIsActionEnd(const char *actionName)
{
	_XActionSimpleEx * temp = getPAction(actionName);
	if(temp == NULL) return XTrue;
	return temp->getIsEnd();
}
inline _XBool _XActionSimpleManager::getIsActionEnd(_XActionSimpleHandle actionHandle)
{
	if(actionHandle >= 0 && actionHandle < MAX_ACTION_SUM
		&& m_pActions[actionHandle].actionIsEnable) 
		return m_pActions[actionHandle].pAction->getIsEnd();
	return XTrue;
}