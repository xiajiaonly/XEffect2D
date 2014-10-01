inline void _XTab::addObjToTab(_XControlBasic *obj,const std::string &title)	//向一个标签中添加物件
{
	addObjToTab(obj,getTabIndexByTitle(title));
}
inline std::string _XTab::getTabStr(int index)
{
	if(index < 0 || index >= m_tabObjects.size()) return "";
	return m_tabObjects[index].srcTitle;
}
inline std::string _XTab::getTabsStr()
{
	std::string ret = "";
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		ret += m_tabObjects[i].srcTitle + ";";
	}
	return ret;
}
inline _XBool _XTab::canGetFocus(float x,float y)				//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
#if TAB_MODE == 1
	//return isInRect(x,y);
	return m_titleRect.isInRect(x,y);
#else
	return m_titleRect.isInRect(x,y);
#endif
}
inline void _XTab::setLostFocus() 
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	m_isBeChoose = XFalse;	//控件处于焦点状态
}
inline _XBool _XTab::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XTab::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
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
inline void _XTab::disable()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(m_isEnable) m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
inline void _XTab::enable()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(!m_isEnable) m_isEnable = XTrue;
}
inline void _XTab::setTabVisible(bool flag,const std::string &title)	//设置标签可见
{
	setTabVisible(flag,getTabIndexByTitle(title));
}
inline _XBool _XTab::getTabVisible(int index) const
{
	if(index < 0 || index >= m_tabObjects.size()) return XFalse;
	return m_tabObjects[index].isVisible;
}
inline _XBool _XTab::getTabVisible(const std::string &title) const
{
	return getTabVisible(getTabIndexByTitle(title));
}
inline void _XTab::setTabEnable(bool flag,const std::string &title)
{
	setTabEnable(flag,getTabIndexByTitle(title));
}
inline _XBool _XTab::getTabEnable(int index) const
{
	if(index < 0 || index >= m_tabObjects.size()) return XFalse;
	return m_tabObjects[index].isEnable;
}
inline _XBool _XTab::getTabEnable(const std::string &title) const
{
	return getTabEnable(getTabIndexByTitle(title));
}
inline void _XTab::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->setColor(m_color);
		}
	}
}
inline void _XTab::setAlpha(float a) 
{
	m_color.setA(a);
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->setAlpha(a);
		}
	}
}
inline void _XTab::setChooseTabCB(void (* funChooseTab)(void *,int,const std::string &),void *pClass)
{
	m_funChooseTab = funChooseTab;
	if(pClass == NULL) m_pClass = this;
	else m_pClass = pClass;
}
inline void _XTab::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	m_leftBtn.drawUp();
	m_rightBtn.drawUp();
#if TAB_MODE == 1
	//显示其子物体
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->drawUp();
		}
	}
#endif
}
inline void _XTab::startAction()
{
	if(m_withAction)
	{//这里测试一个动态效果
		m_isInAction = XTrue;
		m_actionMoveData.set(0.0f,1.0f,0.005f);
		m_oldChooseTabIndex = m_nowChooseTabIndex;	//记录老的选择
	}
}
inline void _XTab::setVisible()	//设置显示
{
	m_isVisible = XTrue;
	updateObjState();
	updateChildVisible();
}
inline void _XTab::disVisible()	//设置不显示
{
	m_isVisible = XFalse;
	m_isBeChoose = XFalse;
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->disVisible();
		}
	}
	updateChildVisible();
}