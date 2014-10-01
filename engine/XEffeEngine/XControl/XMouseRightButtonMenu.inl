inline _XBool _XMouseRightButtonMenu::initEx(int menuSum,	//对上面接口的简化
	const _XVector2& position,	
	const _XMouseRightButtonMenuTexture &tex,	
	const _XFontUnicode &font,float captionSize)
{
	return init(menuSum,position,tex.m_mouseRect,tex,font,captionSize,tex.m_fontPosition);
}
inline _XBool _XMouseRightButtonMenu::initPlus(const char * path,int menuSum,	//菜单中的物件数量
	const _XFontUnicode &font,float captionSize,
	_XResourcePosition resoursePosition)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XBUTTON_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(menuSum,_XVector2::zero,*(_XButtonTexture *)m_resInfo->m_pointer,font,captionSize);
}
inline void _XMouseRightButtonMenu::draw()
{
	if(!m_isInited ||		//没有初始化时不显示
		!m_isVisible) return;	//不可见时不显示
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].draw();
	}
}
inline void _XMouseRightButtonMenu::update(int stepTime)
{
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].update(stepTime);
	}
}
inline void _XMouseRightButtonMenu::drawUp()
{
	if(!m_isInited ||		//没有初始化时不显示
		!m_isVisible) return;	//不可见时不显示
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].drawUp();
	}
}
inline _XBool _XMouseRightButtonMenu::canGetFocus(float x,float y)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
inline _XBool _XMouseRightButtonMenu::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_allArea.isInRect(x,y);
}
inline _XVector2 _XMouseRightButtonMenu::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0:return _XVector2(m_allArea.left,m_allArea.top);
	case 1:return _XVector2(m_allArea.right,m_allArea.top);
	case 2:return _XVector2(m_allArea.right,m_allArea.bottom);
	case 3:return _XVector2(m_allArea.left,m_allArea.bottom);
	}
	return _XVector2::zero;
}
inline int _XMouseRightButtonMenu::getLastChoose() const										//返回最终选择的值
{
	return m_lastChoose;
}
inline void _XMouseRightButtonMenu::setCallbackFun(void (* funChooseChange)(void *,int),void (* funChooseOver)(void *,int),void *pClass)	//设置菜单的回调函数
{
	m_funChooseChange = funChooseChange;
	m_funChooseOver = funChooseOver;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline void _XMouseRightButtonMenu::setHotKey(int hotKey,int order)							//设置菜单中某一项的热键
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	if(order < 0 || order >= m_menuSum) return;
	m_menu[order].setHotKey(hotKey);
}
inline void _XMouseRightButtonMenu::setText(const char *temp,int order)								//改变菜单中某一项的值
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	if(order < 0 || order >= m_menuSum) return;
	m_menu[order].setCaptionText(temp);
}
inline void _XMouseRightButtonMenu::setTexture(const _XMouseRightButtonMenuTexture &tex,int order)	//改变菜单中某一项的贴图
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	if(order < 0 || order >= m_menuSum) return;
	m_menu[order].setTexture(tex);
}
inline void _XMouseRightButtonMenu::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setColor(r,g,b,a);
	}
	updateChildColor();
}
inline void _XMouseRightButtonMenu::setAlpha(float a)
{
	m_color.setA(a);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setAlpha(a);
	}
	updateChildAlpha();
}
