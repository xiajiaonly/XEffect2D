//inline 函数的定义
inline _XObjectType _XObjectManager::getObjectType(int objectID) const
{
//	if(!m_isInited) return OBJ_NULL;
	if(objectID < 0 || objectID >= m_objInfo.size()) return OBJ_NULL;
	return m_objInfo[objectID].m_pObject->m_objType;
}

inline void *_XObjectManager::getObject(int objectID) const
{
//	if(!m_isInited) return NULL;
	if(objectID < 0 || objectID >= m_objInfo.size()) return NULL;
	return m_objInfo[objectID].m_pObject;
}

inline void _XObjectManager::decreaseAObject(const void * object)
{
//	if(!m_isInited) return;
	decreaseAObject(findObjectID(object));
}

inline int _XObjectManager::getNowObjectSum() const
{
//	if(!m_isInited) return 0;
	return m_objInfo.size();
}

inline int _XObjectManager::getNowMouseOnObjectSum() const
{
//	if(!m_isInited) return 0;
	return m_nowMouseOnObjectSum;
}

inline int _XObjectManager::getNowChooseObjectSum() const
{
//	if(!m_isInited) return 0;
	int ret = 0;
	for(int i = 0;i < m_objInfo.size();++ i)
	{
		if(m_objInfo[i].m_objBeSelect) ++ ret;
	}
	return ret;
}
inline void _XObjectManager::setShow()	
{
	m_isShowUI = XTrue;
	m_mutiList.setVisible();
}	//设置显示
inline void _XObjectManager::disShow() 
{
	m_isShowUI = XFalse;
	m_mutiList.disVisible();
}	//设置不显示
inline void _XObjectManager::setOption()
{
	m_isOption = XTrue;
	setShow();
}
inline void _XObjectManager::disOption()
{
	m_isOption = XFalse;
	disShow();
	//取消掉所有的动作
	for(int i = 0;i < m_objInfo.size();++ i)
	{
		m_objInfo[i].m_objectKeyOption = OBJ_OPTION_NULL;
	}
}
inline void _XObjectManager::chengeCanSelectState()
{//改变物件是否可以被点选的标签
	if(!m_isShowUI) return;
	m_canSelect = !m_canSelect;
//	if(!m_canSelect) m_canSelect = XTrue;
//	else m_canSelect = XFalse;
}
#if OBJ_MANAGER_WITH_ID
inline void _XObjectManager::setShowObjID()
{
	if(!m_isShowUI) return;
	m_showObjID = 1 - m_showObjID;
	//if(m_showObjID == 0) m_showObjID = 1;
	//else m_showObjID = 0;
}
#endif