INLINE XBool XButtonBar::setBtnEnable(const std::string &name)		//设置按钮使能
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->enable();
	return XTrue;
}
INLINE XBool XButtonBar::setBtnDisable(const std::string &name)	//设置按钮失效
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->disable();
	return XTrue;
}
INLINE XBool XButtonBar::setBtnCaption(const std::string &name,const std::string &caption)	//设置按钮的文字
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setCaptionText(caption.c_str());
	return XTrue;
}
INLINE XBool XButtonBar::setEventProc(const std::string &name,
		void (* eventProc)(void *,int,int),void *pClass)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setEventProc(eventProc,pClass);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnNormalIcon(const std::string &name,const char * filename,
	XResourcePosition resoursePosition)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setNormalIcon(filename,resoursePosition);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnNormalIcon(const std::string &name,const XSprite &icon)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setNormalIcon(icon);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnDisableIcon(const std::string &name,const char * filename,
	XResourcePosition resoursePosition)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setDisableIcon(filename,resoursePosition);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnDisableIcon(const std::string &name,const XSprite &icon)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setDisableIcon(icon);
	return XTrue;
}
INLINE XBool XButtonBar::removeBtnIcon(const std::string &name)	//去掉所有的icon
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->removeIcon();
	return XTrue;
}
INLINE XBool XButtonBar::setBtnIconPosition(const std::string &name,float x,float y)	//设置icon的位置
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setIconPosition(x,y);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnIconSize(const std::string &name,float x,float y)	//设置icon的缩放大小
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setIconSize(x,y);
	return XTrue;
}
INLINE XFColor XButtonBar::getTextColor() const 
{
	if(m_buttons.size() <= 0) return XFColor::white;
	else return m_buttons[0].button->getTextColor();
}
INLINE XBool XButtonBar::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XButtonBar::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
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
INLINE XBool XButtonBar::canGetFocus(float x,float y)				//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
INLINE int XButtonBar::getBtnID(const std::string &name)	//获取指定按钮的ID
{
	int index = getIndexByName(name);
	if(index < 0) return -1;
	return m_buttons[index].button->getControlID();
}
INLINE XBool XButtonBar::setBtnComment(const std::string &name,const std::string &comment)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setComment(comment.c_str());
	return XTrue;
}
INLINE XBool XButtonBar::setBtnHotKey(const std::string &name,int hotKey)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setHotKey(hotKey);
	return XTrue;
}
