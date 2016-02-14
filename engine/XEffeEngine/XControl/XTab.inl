INLINE void XTab::addObjToTab(XControlBasic *obj,const std::string &title)	//向一个标签中添加物件
{
	addObjToTab(obj,getTabIndexByTitle(title));
}
INLINE void XTab::delObjFromTab(XControlBasic *obj,const std::string &title)
{
	delObjFromTab(obj,getTabIndexByTitle(title));
}
INLINE std::string XTab::getTabStr(unsigned int index)
{
	if(index >= m_tabObjects.size()) return "";
	return m_tabObjects[index].srcTitle;
}
INLINE std::string XTab::getTabsStr()
{
	std::string ret = "";
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		ret += m_tabObjects[i].srcTitle + ";";
	}
	return ret;
}
INLINE XBool XTab::canGetFocus(float x,float y)				//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
INLINE void XTab::setLostFocus() 
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	m_isBeChoose = XFalse;	//控件处于焦点状态
}
INLINE XBool XTab::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XTab::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{	
	if(!m_isInited) return XVector2::zero;
	switch(order)
	{
	case 0:return XVector2(m_curMouseRect.left,m_curMouseRect.top);
	case 1:return XVector2(m_curMouseRect.right,m_curMouseRect.top);
	case 2:return XVector2(m_curMouseRect.right,m_curMouseRect.bottom);
	case 3:return XVector2(m_curMouseRect.left,m_curMouseRect.bottom);
	}
	return XVector2::zero;
}
INLINE void XTab::disable()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(m_isEnable) m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
INLINE void XTab::enable()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(!m_isEnable) m_isEnable = XTrue;
}
INLINE void XTab::setTabVisible(bool flag,const std::string &title)	//设置标签可见
{
	setTabVisible(flag,getTabIndexByTitle(title));
}
INLINE XBool XTab::getTabVisible(unsigned int index) const
{
	if(index >= m_tabObjects.size()) return XFalse;
	return m_tabObjects[index].isVisible;
}
INLINE XBool XTab::getTabVisible(const std::string &title) const
{
	return getTabVisible(getTabIndexByTitle(title));
}
INLINE void XTab::setTabEnable(bool flag,const std::string &title)
{
	setTabEnable(flag,getTabIndexByTitle(title));
}
INLINE XBool XTab::getTabEnable(unsigned int index) const
{
	if(index >= m_tabObjects.size()) return XFalse;
	return m_tabObjects[index].isEnable;
}
INLINE XBool XTab::getTabEnable(const std::string &title) const
{
	return getTabEnable(getTabIndexByTitle(title));
}
INLINE void XTab::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		for(unsigned int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->setColor(m_color);
		}
	}
}
INLINE void XTab::setAlpha(float a) 
{
	m_color.setA(a);
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		for(unsigned int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->setAlpha(a);
		}
	}
}
//INLINE void XTab::setChooseTabCB(void (* funChooseTab)(void *,int,const std::string &),void *pClass)
//{
//	m_funChooseTab = funChooseTab;
//	if(pClass == NULL) m_pClass = this;
//	else m_pClass = pClass;
//}
INLINE void XTab::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	m_leftBtn.drawUp();
	m_rightBtn.drawUp();
}
INLINE void XTab::startAction()
{
	if(m_withAction)
	{//这里测试一个动态效果
		m_isInAction = XTrue;
		m_actionMoveData.set(0.0f,1.0f,0.005f);
		m_oldChooseTabIndex = m_curChooseTabIndex;	//记录老的选择
	}
}
INLINE void XTab::setVisible()	//设置显示
{
	m_isVisible = XTrue;
	updateObjState();
	updateChildVisible();
}
INLINE void XTab::disVisible()	//设置不显示
{
	m_isVisible = XFalse;
	m_isBeChoose = XFalse;
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		for(unsigned int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			//m_tabObjects[i].objs[j]->disVisible();
			m_ctrlManager.setNeedDrawAndInput(false,false,m_tabObjects[i].objs[j]);
		}
	}
	updateChildVisible();
}