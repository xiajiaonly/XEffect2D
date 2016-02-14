//INLINE  函数的定义
INLINE  XObjectType XObjectManager::getObjectType(unsigned int objectID) const
{
//	if(!m_isInited) return OBJ_NULL;
	if(objectID >= m_objInfo.size()) return OBJ_NULL;
	return m_objInfo[objectID].m_pObject->m_objType;
}
INLINE  void *XObjectManager::getObject(unsigned int objectID) const
{
//	if(!m_isInited) return NULL;
	if(objectID >= m_objInfo.size()) return NULL;
	return m_objInfo[objectID].m_pObject;
}
INLINE  void XObjectManager::decreaseAObject(const void * object)
{
//	if(!m_isInited) return;
	decreaseAObject(findObjectID(object));
}
INLINE  int XObjectManager::getCurObjectSum() const
{
//	if(!m_isInited) return 0;
	return m_objInfo.size();
}
INLINE  int XObjectManager::getCurMouseOnObjectSum() const
{
//	if(!m_isInited) return 0;
	return m_curMouseOnObjectSum;
}
INLINE  int XObjectManager::getCurChooseObjectSum() const
{
//	if(!m_isInited) return 0;
	int ret = 0;
	for(unsigned int i = 0;i < m_objInfo.size();++ i)
	{
		if(m_objInfo[i].m_objBeSelect) ++ ret;
	}
	return ret;
}
INLINE  void XObjectManager::setOption()
{
	m_isOption = XTrue;
	setShow();
}
INLINE  void XObjectManager::disOption()
{
	m_isOption = XFalse;
	disShow();
	//取消掉所有的动作
	for(unsigned int i = 0;i < m_objInfo.size();++ i)
	{
		m_objInfo[i].m_objectKeyOption = OBJ_OPTION_NULL;
	}
}
INLINE  void XObjectManager::chengeCanSelectState()
{//改变物件是否可以被点选的标签
	if(!m_isShowUI) return;
	m_canSelect = !m_canSelect;
//	if(!m_canSelect) m_canSelect = XTrue;
//	else m_canSelect = XFalse;
}
#if OBJ_MANAGER_WITH_ID
INLINE  void XObjectManager::setShowObjID()
{
	if(!m_isShowUI) return;
	m_showObjID = 1 - m_showObjID;
	//if(m_showObjID == 0) m_showObjID = 1;
	//else m_showObjID = 0;
}
#endif