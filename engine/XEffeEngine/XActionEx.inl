inline _XBool _XActionEx::isName(const char *name)
{
	if(name == NULL) return XFalse;
	if(m_actionName == name) return XTrue;
	return XFalse;
}
inline void _XActionEx::setPause()			//设置动作暂停
{
	if(m_actionState == ACTION_STATE_ACTION)
		m_actionState = ACTION_STATE_PAUSE;
}
inline void _XActionEx::setResume()		//设置动作恢复
{
	if(m_actionState == ACTION_STATE_PAUSE)
		m_actionState = ACTION_STATE_ACTION;
}
inline void _XActionEx::release()
{
	if(!m_isInited) return;
	funRelease();
	if(m_type == ACTION_TYPE_COMPLEX) XDELETE_ARRAY(m_pAction);
	m_isInited = XFalse;
}

inline void _XActionExManager::move(int stepTime)		//遍历并推进所有的动作
{
	for(int i = 0;i < m_actionOrderX.size();++ i)
	{
		if(m_pActions[m_actionOrderX[i]].actionIsEnable) m_pActions[m_actionOrderX[i]].pAction->move(stepTime);	//这个不需要顺序
	}
}
inline void _XActionExManager::draw()					//遍历并描绘所有的动作
{
	for(int i = 0;i < m_actionOrderX.size();++ i)
	{
		if(m_pActions[m_actionOrderX[i]].actionIsEnable) m_pActions[m_actionOrderX[i]].pAction->draw();	//draw的顺序问题，这里估计需要进行一个映射
		m_pActions[m_actionOrderX[i]].pAction->keepDraw();
	}
}
inline void _XActionExManager::input(const _XInputEvent &inputEvent)					//遍历并描绘所有的动作
{
	for(int i = 0;i < m_actionOrderX.size();++ i)
	{
		if(m_pActions[m_actionOrderX[i]].actionIsEnable) m_pActions[m_actionOrderX[i]].pAction->input(inputEvent);	//draw的顺序问题，这里估计需要进行一个映射
	}
}
inline _XBool _XActionExManager::setStartAction(const char *actionName)
{
	int ret = getActionHandle(actionName);
	if(ret < 0) return XFalse;
	m_pActions[ret].pAction->setStart();
	return XTrue;
}
inline _XBool _XActionExManager::setStartAction(_XActionExHandle actionHandle)
{
	if(actionHandle >= 0 && actionHandle < m_pActions.size()
		&& m_pActions[actionHandle].actionIsEnable)
	{
		m_pActions[actionHandle].pAction->setStart();
		return XTrue;
	}
	return XFalse;
}
inline _XActionEx *_XActionExManager::getPAction(const char *actionName)
{
	int ret = getActionHandle(actionName);
	if(ret < 0) return NULL;
	return m_pActions[ret].pAction;
}
inline _XActionEx *_XActionExManager::getPAction(_XActionExHandle actionHandle)
{
	if(actionHandle >= 0 && actionHandle < m_pActions.size()
		&& m_pActions[actionHandle].actionIsEnable) return m_pActions[actionHandle].pAction;
	return NULL;
}
inline _XBool _XActionExManager::getIsActionEnd(const char *actionName)
{
	_XActionEx * temp = getPAction(actionName);
	if(temp == NULL) return XTrue;
	return temp->getIsEnd();
}
inline _XBool _XActionExManager::getIsActionEnd(_XActionExHandle actionHandle)
{
	if(actionHandle >= 0 && actionHandle < m_pActions.size()
		&& m_pActions[actionHandle].actionIsEnable) 
		return m_pActions[actionHandle].pAction->getIsEnd();
	return XTrue;
}
inline _XBool _XActionExManager::moveToBottom(const char *actionName)			//移动到最下面
{
	if(actionName == NULL) return XFalse;
	return moveToBottom(getActionHandle(actionName));
}
inline _XBool _XActionExManager::moveToTop(const char *actionName)				//移动到最上面
{
	if(actionName == NULL) return XFalse;
	return moveToTop(getActionHandle(actionName));
}
inline _XBool _XActionExManager::moveUp(const char *actionName)					//向上移动一层,与上一层进行交换
{
	if(actionName == NULL) return XFalse;
	return moveUp(getActionHandle(actionName));
}
inline _XBool _XActionExManager::moveDown(const char *actionName)					//向下移动一层
{
	if(actionName == NULL) return XFalse;
	return moveDown(getActionHandle(actionName));
}
inline _XBool _XActionExManager::moveUpTo(const char *s,const char *d)			//s移动到d上面
{
	if(s == NULL || d == NULL) return XFalse;
	return moveUpTo(getActionHandle(s),getActionHandle(d));
}
inline _XBool _XActionExManager::moveDownTo(const char *s,const char *d)			//s移动到d上面
{
	if(s == NULL || d == NULL) return XFalse;
	return moveDownTo(getActionHandle(s),getActionHandle(d));
}
inline int _XActionExManager::getLayerIndex(const char *actionName)			//获取当前的在哪一层
{
	if(actionName == NULL) return -1;
	return getLayerIndex(getActionHandle(actionName));
}
inline int _XActionExManager::getLayerIndex(_XActionExHandle actionHandle)
{
	if(actionHandle < 0 || actionHandle >= m_pActions.size()) return -1;
	return m_actionOrder[actionHandle];
}
inline _XBool _XActionExManager::releaseAction(_XActionSimpleHandle actionHandle)
{
	if(actionHandle < 0 || actionHandle >= m_pActions.size() ||
		!m_pActions[actionHandle].actionIsEnable) return XFalse;
	m_pActions[actionHandle].actionHandle = 0;
	m_pActions[actionHandle].actionIsEnable = XFalse;
	m_pActions[actionHandle].pAction->release();
	m_pActions[actionHandle].pAction = NULL;
	return XTrue;
}
inline _XBool _XActionExManager::popAction(_XActionExHandle actionHandle)
{
	if(actionHandle < 0 || actionHandle >= m_pActions.size() ||
		!m_pActions[actionHandle].actionIsEnable) return XFalse;
	m_pActions[actionHandle].actionHandle = 0;
	m_pActions[actionHandle].actionIsEnable = XFalse;
	m_pActions[actionHandle].pAction = NULL;
	return XTrue;
}