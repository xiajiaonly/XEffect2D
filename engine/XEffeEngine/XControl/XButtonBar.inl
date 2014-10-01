inline _XBool _XButtonBar::setBtnEnable(const std::string &name)		//设置按钮使能
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->enable();
	return XTrue;
}
inline _XBool _XButtonBar::setBtnDisable(const std::string &name)	//设置按钮失效
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->disable();
	return XTrue;
}
inline _XBool _XButtonBar::setBtnCaption(const std::string &name,const std::string &caption)	//设置按钮的文字
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setCaptionText(caption.c_str());
	return XTrue;
}
inline _XBool _XButtonBar::setBtnCallbackFun(const std::string &name,
	void (* funInit)(void *,int),
	void (* funRelease)(void *,int),
	void (* funMouseOn)(void *,int),
	void (* funMouseDown)(void *,int),
	void (* funMouseUp)(void *,int),
	void *pClass)	//设置按钮的回调函数
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setCallbackFun(funInit,funRelease,funMouseOn,
		funMouseDown,funMouseUp,pClass);
	return XTrue;
}
inline _XBool _XButtonBar::setBtnMouseDownCB(const std::string &name,
	void (* funMouseDown)(void *,int),
	void *pClass)	//设置按钮被按下时的回调函数
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setMouseDownCB(funMouseDown,pClass);
	return XTrue;
}
inline _XBool _XButtonBar::setBtnNormalIcon(const std::string &name,const char * filename,
	_XResourcePosition resoursePosition)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setNormalIcon(filename,resoursePosition);
	return XTrue;
}
inline _XBool _XButtonBar::setBtnNormalIcon(const std::string &name,const _XSprite &icon)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setNormalIcon(icon);
	return XTrue;
}
inline _XBool _XButtonBar::setBtnDisableIcon(const std::string &name,const char * filename,
	_XResourcePosition resoursePosition)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setDisableIcon(filename,resoursePosition);
	return XTrue;
}
inline _XBool _XButtonBar::setBtnDisableIcon(const std::string &name,const _XSprite &icon)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setDisableIcon(icon);
	return XTrue;
}
inline _XBool _XButtonBar::removeBtnIcon(const std::string &name)	//去掉所有的icon
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->removeIcon();
	return XTrue;
}
inline _XBool _XButtonBar::setBtnIconPosition(const std::string &name,float x,float y)	//设置icon的位置
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setIconPosition(x,y);
	return XTrue;
}
inline _XBool _XButtonBar::setBtnIconSize(const std::string &name,float x,float y)	//设置icon的缩放大小
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setIconSize(x,y);
	return XTrue;
}
inline _XFColor _XButtonBar::getTextColor() const 
{
	if(m_buttons.size() <= 0) return _XFColor::white;
	else return m_buttons[0].button->getTextColor();
}
inline _XBool _XButtonBar::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XButtonBar::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
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
inline _XBool _XButtonBar::canGetFocus(float x,float y)				//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
inline int _XButtonBar::getBtnID(const std::string &name)	//获取指定按钮的ID
{
	int index = getIndexByName(name);
	if(index < 0) return -1;
	return m_buttons[index].button->getControlID();
}
inline _XBool _XButtonBar::setBtnComment(const std::string &name,const std::string &comment)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setComment(comment.c_str());
	return XTrue;
}
inline _XBool _XButtonBar::setBtnHotKey(const std::string &name,int hotKey)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setHotKey(hotKey);
	return XTrue;
}
