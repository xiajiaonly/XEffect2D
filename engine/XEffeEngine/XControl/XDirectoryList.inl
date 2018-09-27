INLINE void XDirListSkin::release()
{
	if(!m_isInited) return;
	XMem::XDELETE(dirListNormal);
	XMem::XDELETE(dirListDisable);
	m_isInited = XFalse;
}
INLINE void XDirListOneLine::release()
{
	if(!m_isEnable) return;
	XMem::XDELETE_ARRAY(m_string);
	XMem::XDELETE(m_check);
	m_isEnable = XFalse;
}
INLINE void XDirectoryList::setCanChangePath(XBool flag)
{
	if((flag && m_canChangePath) || (!flag && !m_canChangePath)) return;
	m_canChangePath = flag;
	if(flag) m_edit.enable();
	else m_edit.disable();
}
INLINE XBool XDirectoryList::initEx(const XVec2& position,	//对上面接口的简化
		XDirListSkin & tex,
		const XFontUnicode& font,
		float fontSize,
		const XCheck &check,
		const XButton &button,
		const XEdit &edit,
		const XSlider &vSlider,
		const XSlider &hSlider)
{
	return init(position,tex.m_mouseRect,tex,font,fontSize,check,button,edit,vSlider,hSlider);
}
INLINE const char * XDirectoryList::getSelectFileName() const	//获取全路径
{
	if(m_haveSelect) return m_lineData[m_selectLineOrder]->m_file->allPath.c_str();
	else return NULL;
}
INLINE int XDirectoryList::getSelectLineOrder() const
{
	if(m_haveSelect) return m_selectLineOrder;
	else return -1;
}
INLINE XBool XDirectoryList::canGetFocus(const XVec2& p)//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(p);
}
INLINE void XDirectoryList::setPosition(const XVec2& p)
{
	m_position = p;
	updateShowPosition();
	if(!m_withoutTex) m_spriteBackGround.setPosition(m_position + XVec2(0.0f,m_edit.getMouseRect().getHeight()) * m_scale);
	m_button.setPosition(m_position + XVec2(m_mouseRect.getWidth(),0.0f) * m_scale);
	m_verticalSlider.setPosition(m_position + XVec2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()) * m_scale);
	m_horizontalSlider.setPosition(m_position + XVec2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()) * m_scale);
	m_edit.setPosition(m_position);
	m_curMouseRect.set(m_position, m_position + XVec2(m_mouseRect.getWidth() + m_verticalSlider.getMouseRect().getWidth(),
		m_mouseRect.getHeight() + m_edit.getMouseRect().getHeight() + m_horizontalSlider.getMouseRect().getHeight()) * m_scale);
	updateChildPos();
}
INLINE void XDirectoryList::setScale(const XVec2& s)
{//左上角对其
	m_scale = s;
	updateShowPosition();
	if(!m_withoutTex) 
	{
		m_spriteBackGround.setPosition(m_position + XVec2(0.0f,m_edit.getMouseRect().getHeight()) * m_scale);
		m_spriteBackGround.setScale(m_scale);
	}
	m_button.setPosition(m_position + XVec2(m_mouseRect.getWidth(),0.0f) * m_scale);
	m_button.setScale(m_scale);
	m_verticalSlider.setPosition(m_position + XVec2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()) * m_scale);
	m_verticalSlider.setScale(m_scale);
	m_horizontalSlider.setPosition(m_position + XVec2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()) * m_scale);
	m_horizontalSlider.setScale(m_scale);
	m_edit.setPosition(m_position);
	m_edit.setScale(m_scale);
	m_curMouseRect.set(m_position, m_position + XVec2(m_mouseRect.getWidth() + m_verticalSlider.getMouseRect().getWidth(),
		m_mouseRect.getHeight() + m_edit.getMouseRect().getHeight() + m_horizontalSlider.getMouseRect().getHeight()) * m_scale);
	updateChildScale();
}
INLINE XBool XDirectoryList::isInRect(const XVec2& p)						//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XBool XDirectoryList::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return XTrue;	//如果不可见直接退出
	if(m_isSilent) return XFalse;
	if(m_needShowVSlider) m_verticalSlider.keyboardProc(keyOrder,keyState);
	if(m_needShowHSlider) m_horizontalSlider.keyboardProc(keyOrder,keyState);
	m_edit.keyboardProc(keyOrder,keyState);
	m_button.keyboardProc(keyOrder,keyState);
	return XTrue;
}
INLINE XVec2 XDirectoryList::getBox(int order)		//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return XVec2::zero;
	switch(order)
	{
	case 0: return m_curMouseRect.getLT();
	case 1: return m_curMouseRect.getRT();
	case 2: return m_curMouseRect.getRB();
	case 3: return m_curMouseRect.getLB();
	}
	return XVec2::zero;
}
INLINE void XDirectoryList::setColor(const XFColor& c)
{
	m_color = c;
	m_caption.setColor(m_color);
	m_spriteBackGround.setColor(m_color);
	m_verticalSlider.setColor(m_color);
	m_horizontalSlider.setColor(m_color);
	m_button.setColor(m_color);
	m_edit.setColor(m_color);
	m_check.setColor(m_color);
	for(unsigned int i = 0;i < m_lineData.size();++ i)
	{
		m_lineData[i]->m_font.setColor(m_color);
		if(m_lineData[i]->m_check != NULL) m_lineData[i]->m_check->setColor(m_color); 
	}
	updateChildColor();
}
INLINE void XDirectoryList::setAlpha(float a)
{
	m_color.setA(a);
	m_caption.setAlpha(a);
	m_spriteBackGround.setAlpha(a);
	m_verticalSlider.setAlpha(a);
	m_horizontalSlider.setAlpha(a);
	m_button.setAlpha(a);
	m_edit.setAlpha(a);
	m_check.setAlpha(a);
	for(unsigned int i = 0;i < m_lineData.size();++ i)
	{
		m_lineData[i]->m_font.setAlpha(a);
		if(m_lineData[i]->m_check != NULL) m_lineData[i]->m_check->setAlpha(a); 
	}
	updateChildAlpha();
}