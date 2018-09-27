INLINE void XComboSkin::releaseTex()
{
	XMem::XDELETE(comboInputNormal);
	XMem::XDELETE(comboInputDisable);
	
	downButtonTex.release();
	downMenuUpButtonTex.release();
	downMenuButtonTex.release();
	downMenuDownButtonTex.release();
}
INLINE void XComboSkin::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
INLINE XBool XCombo::initEx(const XComboSkin &tex,	//上面接口的简化版本
		const XVec2& position,		
		int menuSum,					
		int drawMenuSum,				
		const XFontUnicode& font,		
		float fontSize)
{
	return init(tex,position,tex.m_mouseRect,tex.downButtonTex.m_mouseRect,
		tex.downMenuUpButtonTex.m_mouseRect,tex.downMenuButtonTex.m_mouseRect,
		tex.downMenuDownButtonTex.m_mouseRect,menuSum,drawMenuSum,font,fontSize);
}
INLINE XBool XCombo::initPlus(const char * path,
		int menuSum,					//下拉菜单中的选项的数量
		int drawMenuSum,				//下拉菜单中显示的菜单项的数量
		const XFontUnicode& font,		//显示文字使用的字体
		float fontSize,
		XResPos resPos)
{
	if(m_isInited) return XFalse;				//防止重复初始化
	if(path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXCOMBO_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	return initEx(*(XComboSkin *)m_resInfo->m_pointer,XVec2::zero,menuSum,drawMenuSum,font,fontSize);
}
INLINE XBool XCombo::canGetFocus(const XVec2& p)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(p);
}
INLINE XBool XCombo::canLostFocus(const XVec2&)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XTrue;		//如果无效则直接退出

	if(m_isDrawDownMenu) return XFalse;
	return XTrue;
}
INLINE void XCombo::setLostFocus()	//设置失去焦点
{
	if (!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	if (m_isMouseInRect)
	{
		m_isMouseInRect = XFalse;
		m_comment.disShow();
	}
	if (m_isDrawDownMenu) changeDownMenuState();
	m_isBeChoose = XFalse;	//控件处于焦点状态
}
INLINE void XCombo::setColor(const XFColor& c) 	//设置按钮的颜色
{
	if(!m_isInited) return;
	m_color = c;
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setColor(m_color);
	}
}	//设置按钮的颜色
INLINE void XCombo::setAlpha(float a) 
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setColor(m_color);
	}
}
INLINE XBool XCombo::isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(p,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE void XCombo::setTextColor(const XFColor& color) 
{
	if(!m_isInited) return;
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setTextColor(m_textColor);
	}
}	//设置字体的颜色
INLINE XBool XCombo::setCurChooseOrder(int index)	//设置当前选项编号
{
	if(index < 0 || index >= m_menuSum) return XFalse;
	if(m_curChooseOrder == index) return XTrue;
	m_curChooseOrder = index;
	updateString();
	return XTrue;
}
INLINE void XCombo::disable()					//使无效
{
	m_isEnable = XFalse;
//	m_isBeChoose = XFalse;
}
INLINE void XCombo::enable()					//使能
{
	m_isEnable = XTrue;
}
INLINE const std::string XCombo::getCurChooseMenu() 	//获取当前选择的菜单的内容
{
	if(m_curChooseOrder < 0) return XString::gNullStr;
	if(m_curChooseOrder == 0)
	{
		int len = XString::getCharPosition(m_menuData.c_str(),'\n');
		if(len < 0) return m_menuData;
		return m_menuData.substr(0,len);
	//	m_menuData[len] = '\0';
	//	std::string ret = m_menuData;
	//	m_menuData[len] = '\n';
	//	return ret;
	}else
	{
		int s = XString::getCharPositionEx(m_menuData.c_str(),'\n',m_curChooseOrder);
		if(s < 0) return XString::gNullStr;
		int e = XString::getCharPositionEx(m_menuData.c_str(),'\n',m_curChooseOrder + 1);
		if(e < 0) return XString::gNullStr;
		return m_menuData.substr(s + 1,e - s - 1);
	//	m_menuData[e] = '\0';
	//	std::string ret = m_menuData.substr(s + 1,(int)(m_menuData.size()) - s - 1);
	//	m_menuData[e] = '\n';
	//	return ret;
	}
}