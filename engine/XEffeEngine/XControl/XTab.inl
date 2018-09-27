INLINE void XTab::addObjToTab(XControlBasic *obj, const std::string& title)	//向一个标签中添加物件
{
	addObjToTab(obj, getTabIndexByTitle(title));
}
INLINE void XTab::delObjFromTab(XControlBasic *obj,const std::string& title)
{
	delObjFromTab(obj,getTabIndexByTitle(title));
}
INLINE const std::string& XTab::getTabStr(unsigned int index)const
{
	if(index >= m_tabObjects.size()) return XString::gNullStr;
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
INLINE XBool XTab::canGetFocus(const XVec2& p)				//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	//如果所有的子物体中有能获取到焦点的物体，则自身能获取到焦点,当有漂浮在外面的控件时这个可能防止焦点穿透
	for (int i = 0;i < m_tabObjects[m_curChooseTabIndex].objs.size();++i)
	{
		if (m_tabObjects[m_curChooseTabIndex].objs[i]->canGetFocus(p)) return true;
	}
	return m_curMouseRect.isInRect(p);
}
INLINE void XTab::setLostFocus() 
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	m_isBeChoose = XFalse;	//控件处于焦点状态
}
INLINE XBool XTab::isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XTab::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
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
INLINE void XTab::setTabVisible(bool flag,const std::string& title)	//设置标签可见
{
	setTabVisible(flag,getTabIndexByTitle(title));
}
INLINE XBool XTab::getTabVisible(unsigned int index) const
{
	if(index >= m_tabObjects.size()) return XFalse;
	return m_tabObjects[index].isVisible;
}
INLINE XBool XTab::getTabVisible(const std::string& title) const
{
	return getTabVisible(getTabIndexByTitle(title));
}
INLINE void XTab::setTabEnable(bool flag,const std::string& title)
{
	setTabEnable(flag,getTabIndexByTitle(title));
}
INLINE XBool XTab::getTabEnable(unsigned int index) const
{
	if(index >= m_tabObjects.size()) return XFalse;
	return m_tabObjects[index].isEnable;
}
INLINE XBool XTab::getTabEnable(const std::string& title) const
{
	return getTabEnable(getTabIndexByTitle(title));
}
INLINE void XTab::setColor(const XFColor& c)
{
	m_color = c;
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
//INLINE void XTab::setChooseTabCB(void (* funChooseTab)(void *,int,const std::string& ),void *pClass)
//{
//	m_funChooseTab = funChooseTab;
//	if(pClass == NULL) m_pClass = this;
//	else m_pClass = pClass;
//}
INLINE void XTab::setUpDrawCB(void(*funUpDraw)(void *), void *pClass)
{
	m_funUpDraw = funUpDraw;
	if (pClass == NULL) m_pClass = this;
	else m_pClass = pClass;
}
INLINE void XTab::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	m_leftBtn.drawUp();
	m_rightBtn.drawUp();
//	m_ctrlManager.drawUp();
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
INLINE void XTab::delATab(const std::string& title)
{
	if (!m_isInited) return;
	int index = getTabIndexByTitle(title);
	if (index < 0) return;
	XMem::XDELETE(m_tabObjects[index].pFont);
	m_tabObjects.erase(m_tabObjects.begin() + index);
}