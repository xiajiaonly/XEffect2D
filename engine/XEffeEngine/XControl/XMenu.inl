INLINE XMenuItem *XMenuGroup::getChooseItem()	//获取当前组中被选择的菜单项
{
	if(m_chooseIndex >= 0 && m_chooseIndex < (int)(m_items.size()))
		return m_items[m_chooseIndex];
	return NULL;
}
INLINE void XMenuGroup::draw()
{
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->draw();
	}
}
INLINE void XMenuGroup::setOpen()	//设置打开
{//所有成员可见
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->m_isVisible = XTrue;
	}
}
INLINE void XMenuGroup::disOpen()
{
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->m_isVisible = XFalse;
		m_items[i]->disOpen();
	}
	resetChoose();
}
INLINE XBool XMenuGroup::isInRect(const XVec2& p)
{
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		if(m_items[i]->isInRect(p)) return XTrue;
	}
	return XFalse;
}
INLINE XBool XMenuGroup::mouseProc(const XVec2& p,XMouseState mouseState)
{
	XBool ret = XFalse;
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		if(m_items[i]->mouseProc(p,mouseState)) ret = XTrue;
	}
	return ret;
}
INLINE void XMenuGroup::postPross()
{
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->postPross();
	}
}
INLINE void XMenuGroup::openChoose()	//打开选择的项
{
	if(m_chooseIndex >= 0 && m_chooseIndex < (int)(m_items.size()))
		m_items[m_chooseIndex]->setOpen();
}
INLINE void XMenuGroup::closeChoose()	//关闭选择的项
{
	if(m_chooseIndex >= 0 && m_chooseIndex < (int)(m_items.size()))
		m_items[m_chooseIndex]->disOpen();
}
INLINE void XMenuGroup::resetChoose()
{
	m_chooseIndex = -1;
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->resetChoose();
	}
}
INLINE void XMenuGroup::setHotChooseActive(XBool flag)
{
	for(unsigned int i = 0; i < m_items.size();++ i)
	{
		m_items[i]->setHotChooseActive(flag);
	}
}
INLINE XMenuItem * XMenuGroup::getItem(const XVec2& p)
{
	XMenuItem * tmp = NULL;
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		if((tmp = m_items[i]->getItem(p)) != NULL) return tmp;
	}
	return NULL;
}
INLINE XMenuItem * XMenuGroup::getItemByNameInGroup(const std::string& name)
{
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		if(m_items[i]->m_originalName == name) return m_items[i];
	}
	return NULL;
}
INLINE void XMenuItem::setCheckState(XBool state)	//设置选择类菜单项的选择状态
{
	if(m_isChecked == state ||
		m_type != MENU_ITEM_TYPE_CHK) return;
	m_isChecked = state;
	if(m_isChecked) m_showName[0] = 'X';
	else m_showName[0] = '*';
	m_font.setString(m_showName.c_str());
}
INLINE void XMenuItem::postPross()
{
	if(m_child != NULL) m_child->postPross();
	//下面的情况的菜单项不能有快捷键
	if(m_levelIndex < 2 || m_type == MENU_ITEM_TYPE_DLT || m_child != NULL) m_hotKey = -1;

	if(m_levelIndex == 0) return;	//根层不需要处理
	updateStr();
}
INLINE XBool XMenuItem::setHotKey(char k)	//-1为无效
{//设置快捷键
	//一个菜单类中唯一
	m_hotKey = k;
	return XTrue;
}
INLINE XMenuItem *XMenuItem::getItemByHotKey(char k)
{//通过快捷键来查找菜单项
	if(k < 0) return NULL;
	if(m_hotKey == k) return this;
	if(m_child != NULL) return m_child->getItemByHotKey(k);
	return NULL;
}
INLINE XMenuItem *XMenuItem::getItemByHotChooseKey(char k)//从子菜单中寻找指定的热键的菜单项
{
	if(m_child == NULL) return NULL;
	return m_child->getItemByHotChooseKey(k);
}
INLINE void XMenuItem::setCallbackFun(void (*callbackFun)(void *,const std::string& ),void *p)	//设置回调函数，当菜单项被触发时会调用相关的回调函数
{
	if(m_type == MENU_ITEM_TYPE_DLT) return; //分隔符没有回调
	m_callbackFun = callbackFun;
	m_pClass = p;
}
INLINE void XMenuItem::setAllCallBackFun(void (*callbackFun)(void *,const std::string& ),void *p)
{
	if(m_type == MENU_ITEM_TYPE_DLT) return; //分隔符没有回调
	if(m_child == NULL)
	{
		m_callbackFun = callbackFun;
		m_pClass = p;
	}else
	{
		m_child->setAllCallBackFun(callbackFun,p);
	}
}
INLINE void XMenuItem::setOpen()	//设置打开
{
	if(!m_isEnable || m_isOpen) return;	//无效的物件不能打开
	if(m_type == MENU_ITEM_TYPE_DLT) return;
	m_isOpen = XTrue;
	if(m_child != NULL) m_child->setOpen();
}
INLINE void XMenuItem::disOpen()	//设置关闭
{
	if(m_type == MENU_ITEM_TYPE_DLT) return;
	m_isOpen = XFalse;
	if(m_child != NULL) m_child->disOpen();
}
INLINE XBool XMenuItem::isInRect(const XVec2& p)
{
	if(XRect(m_position,m_position + m_size * m_scaleSize).isInRect(p)) return XTrue;
	if(m_child != NULL && m_isOpen) return m_child->isInRect(p);
	return XFalse;
}
INLINE XMenuItem * XMenuItem::getItem(const XVec2& p)
{
	if(XRect(m_position,m_position + m_size * m_scaleSize).isInRect(p)) return this;
	if(m_child != NULL && m_isOpen) return m_child->getItem(p);
	return NULL;
}
INLINE void XMenuItem::resetChoose()
{
	m_isMouseOn = XFalse;
	if(m_child != NULL) m_child->resetChoose();
}
INLINE XBool XMenuItem::setHotChooseKey(char k)	//-1为无效
{//设置快选键
	if(k >= 'a' && k <= 'z') k += 'A' - 'a';	//必须是可以显示的字母
	if(k < 'A' || k > 'Z') return XFalse;
	//组中唯一
	if(m_owner != NULL && m_owner->getItemByHotChooseKey(k) != NULL) return XFalse;
	m_hotChooseKey = k;
	return XTrue;
}
INLINE void XMenuItem::disIcon()
{
	if(m_withIcon)
	{
		m_spriteIcon.release();
		m_withIcon = XFalse;
	}
}
INLINE void XMenuItem::setHotChooseActive(XBool flag)
{//设置热键激活状态
	if(m_child != NULL) m_child->setHotChooseActive(flag);
	m_isHotChooseActive = flag;
	if(m_hotChooseKey >= 0) updateStr();
}
INLINE XBool XMenu::getCheckState(const std::string& pos)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type != MENU_ITEM_TYPE_CHK) return XFalse;
	return tmpX->m_isChecked;
}
INLINE XBool XMenu::setCheckState(const std::string& pos,XBool state)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type != MENU_ITEM_TYPE_CHK) return XFalse;
	tmpX->setCheckState(state);
	return XTrue;
}
INLINE XBool XMenu::setDisable(const std::string& pos)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	tmpX->m_isEnable = XFalse;
	return XTrue;
}
INLINE XBool XMenu::setEnable(const std::string& pos)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	tmpX->m_isEnable = XTrue;
	return XTrue;
}
INLINE XBool XMenu::getEnable(const std::string& pos)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	return tmpX->m_isEnable;
}
INLINE XBool XMenu::setItemIcon(const std::string& pos,const char *filename,XResPos res)
{
	if(filename == NULL) return XFalse;
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	return tmpX->setIcon(filename,res);
}
INLINE XBool XMenu::setHotKey(const std::string& pos,char k)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	if(k >= 0 && m_rootItem.getItemByHotKey(k) != NULL) return XFalse;//必须检查唯一性 
	m_needPostProcess = XTrue;
	return tmpX->setHotKey(k);
}
INLINE char XMenu::getHotKey(const std::string& pos)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return -1;
	return tmpX->m_hotKey;
}
INLINE XBool XMenu::setHotChooseKey(const std::string& pos,char k)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	if(tmpX->setHotChooseKey(k))
	{
		m_needPostProcess = XTrue;
		return XTrue;
	}
	return XFalse;
}
INLINE char XMenu::getHotChooseKey(const std::string& pos)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return -1;
	return tmpX->m_hotChooseKey;
}
INLINE void XMenu::postProcess()
{
	if(!m_needPostProcess) return;
	m_rootItem.postPross();
	m_needPostProcess = XFalse;
}
//INLINE XBool XMenu::setCBFunction(const std::string& pos,void (*callbackFun)(void *,const std::string& ),void *p)
//{//设置菜单响应的回调函数
//	XMenuItem * tmp = getItemByName(pos);
//	if(tmp == NULL) return XFalse;
//	tmp->setCallbackFun(callbackFun,p);
//	return XTrue;
//}
INLINE void XMenu::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	postProcess();
	m_rootItem.draw();
}
INLINE const std::string XMenu::getItemFullStr(const XMenuItem * it)
{
	if(it == NULL) return XString::gNullStr;
	return it->getFullStr();
}
INLINE void XMenu::setPosition(const XVec2& p)
{
	m_position = p;
	m_rootItem.setPosition(m_position);
}
INLINE void XMenu::setScale(const XVec2& s)
{
	m_scale = s;
	m_rootItem.setScale(m_scale);
	m_rootItem.setPosition(m_position);
}
INLINE void XMenu::setColor(const XFColor& c)
{
	m_color = c;
	m_font.setColor(m_color);
	m_rootItem.setColor(m_color);
}
INLINE void XMenu::setAlpha(float a)
{
	m_color.setA(a);
	m_font.setAlpha(a);
	m_rootItem.setColor(m_color);
}
INLINE XVec2 XMenu::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{//这里并没有实际的意义
	if(!m_isInited || 
		m_rootItem.m_child == NULL)	return XVec2::zero;
	XMenuItem * it = m_rootItem.m_child->m_items[0];	//这里用第一个菜单项的范围，意义上不完整
	if(it == NULL)	return XVec2::zero;
	switch(order)
	{
	case 0:return it->m_position;
	case 1:return XVec2(it->m_position.x + it->m_size.x * it->m_scaleSize.x,it->m_position.y);
	case 2:return it->m_position + it->m_size * it->m_scaleSize;
	case 3:return XVec2(it->m_position.x,it->m_position.y + it->m_size.y * it->m_scaleSize.y);
	}
	return XVec2::zero;
}
INLINE XBool XMenu::canGetFocus(const XVec2& p)				//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(p);
}
INLINE void XMenu::setLostFocus()	//设置失去焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	m_isBeChoose = XFalse;	//控件处于焦点状态
	//菜单失去交单之后需要将菜单关闭
	closeAll();
	m_oldChooseItem = NULL;
	m_isClicked = XFalse;
}