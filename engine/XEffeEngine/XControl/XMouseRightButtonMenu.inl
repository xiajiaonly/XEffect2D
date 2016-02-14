INLINE XBool XMouseRightButtonMenu::initEx(int menuSum,	//对上面接口的简化
	const XVector2& position,	
	const XMouseRightButtonMenuSkin &tex,	
	const XFontUnicode &font,float captionSize)
{
	return init(menuSum,position,tex.m_mouseRect,tex,font,captionSize,tex.m_fontPosition);
}
INLINE XBool XMouseRightButtonMenu::initPlus(const char * path,int menuSum,	//菜单中的物件数量
	const XFontUnicode &font,float captionSize,
	XResourcePosition resoursePosition)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXBUTTON_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(menuSum,XVector2::zero,*(XButtonSkin *)m_resInfo->m_pointer,font,captionSize);
}
INLINE void XMouseRightButtonMenu::draw()
{
	if(!m_isInited ||		//没有初始化时不显示
		!m_isVisible) return;	//不可见时不显示
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].draw();
	}
}
INLINE void XMouseRightButtonMenu::update(float stepTime)
{
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].update(stepTime);
	}
	if(m_isInAction)
	{//这里对工作进行描述(尚未完成)
		m_actionMoveData.move(stepTime);
		float action = m_actionMoveData.getCurData();
		if(m_actionMoveData.getIsEnd()) 
		{//动作完成
			m_isInAction = false;
			for(int i = 0;i < m_menuSum;++ i)
			{
				m_menu[i].setWithAction(XTrue);
			}
			action = 1.0f;	//这里需要恢复
			if(!m_isDraw)
				m_isVisible = XFalse;
		}
		if(m_isDraw)
		{
			for(int i = 0;i < m_menuSum;++ i)
			{
				m_menu[i].setAlpha(action * m_color.fA);
				m_menu[i].setPosition(m_position.x + (100.0f + i * 50.0f) * m_scale.y * (1.0f - action),
					m_position.y + m_mouseRect.getHeight() * i * m_scale.y);
			}
		}else
		{
			for(int i = 0;i < m_menuSum;++ i)
			{
				m_menu[i].setAlpha((1.0f - action) * m_color.fA);
				m_menu[i].setPosition(m_position.x - (100.0f + i * 50.0f) * m_scale.y * action,
					m_position.y + m_mouseRect.getHeight() * i * m_scale.y);
			}
		}
	}
}
INLINE void XMouseRightButtonMenu::drawUp()
{
	if(!m_isInited ||		//没有初始化时不显示
		!m_isVisible) return;	//不可见时不显示
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].drawUp();
	}
}
INLINE XBool XMouseRightButtonMenu::canGetFocus(float x,float y)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
INLINE XBool XMouseRightButtonMenu::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_allArea.isInRect(x,y);
}
INLINE XVector2 XMouseRightButtonMenu::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return XVector2::zero;
	switch(order)
	{
	case 0:return XVector2(m_allArea.left,m_allArea.top);
	case 1:return XVector2(m_allArea.right,m_allArea.top);
	case 2:return XVector2(m_allArea.right,m_allArea.bottom);
	case 3:return XVector2(m_allArea.left,m_allArea.bottom);
	}
	return XVector2::zero;
}
INLINE int XMouseRightButtonMenu::getLastChoose() const										//返回最终选择的值
{
	return m_lastChoose;
}
//INLINE void XMouseRightButtonMenu::setCallbackFun(void (* funChooseChange)(void *,int),void (* funChooseOver)(void *,int),void *pClass)	//设置菜单的回调函数
//{
//	m_funChooseChange = funChooseChange;
//	m_funChooseOver = funChooseOver;
//	if(pClass != NULL) m_pClass = pClass;
//	else m_pClass = this;
//}
INLINE void XMouseRightButtonMenu::setHotKey(int hotKey,int order)							//设置菜单中某一项的热键
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	if(order < 0 || order >= m_menuSum) return;
	m_menu[order].setHotKey(hotKey);
}
INLINE void XMouseRightButtonMenu::setText(const char *temp,int order)								//改变菜单中某一项的值
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	if(order < 0 || order >= m_menuSum) return;
	m_menu[order].setCaptionText(temp);
}
INLINE void XMouseRightButtonMenu::setTexture(const XMouseRightButtonMenuSkin &tex,int order)	//改变菜单中某一项的贴图
{
	if(!m_isInited) return;	//如果没有初始化直接退出
	if(order < 0 || order >= m_menuSum) return;
	m_menu[order].setTexture(tex);
}
INLINE void XMouseRightButtonMenu::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setColor(r,g,b,a);
	}
	updateChildColor();
}
INLINE void XMouseRightButtonMenu::setAlpha(float a)
{
	m_color.setA(a);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setAlpha(a);
	}
	updateChildAlpha();
}
