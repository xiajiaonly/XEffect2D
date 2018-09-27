INLINE void XPropertyBox::setTitle(const char * title)	//设置标题
{
	if(title == NULL) return;
	m_haveTitle = XTrue;
	m_titleStr = title;
	m_titleTxt.setString(XString::getCanShowString(title,(int)(m_width - 10.0f + MIN_FONT_CTRL_SIZE),16).c_str());
	m_titleTxt.setVisible();
	updateRect();
	if(m_haveTitle)
		m_slider.setPosition(m_position + XVec2(m_width, MIN_FONT_CTRL_SIZE) * m_scale);
	else
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y);
}
INLINE void XPropertyBox::disTitle()					//取消标题
{
	m_haveTitle = XFalse;
	m_titleTxt.disVisible();
	updateRect();
	if(m_haveTitle)
		m_slider.setPosition(m_position + XVec2(m_width, MIN_FONT_CTRL_SIZE) * m_scale);
	else
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y);
}
INLINE XBool XPropertyBox::isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XPropertyBox::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
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
INLINE XBool XPropertyBox::canGetFocus(const XVec2& p)				//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	//如果所有的子物体中有能获取到焦点的物体，则自身能获取到焦点,当有漂浮在外面的控件时这个可能防止焦点穿透
	for (unsigned int i = 0;i < m_propertyLines.size();++i)
	{
		if (m_propertyLines[i].canGetFocus(p)) return true;
	}
	return m_curMouseRect.isInRect(p);
}
INLINE void XPropertyBox::setLostFocus() 
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出
	for (unsigned int i = 0;i < m_propertyLines.size();++i)
	{
		m_propertyLines[i].setFocus(false);
	}
	m_isBeChoose = XFalse;	//控件处于焦点状态
}
INLINE void XPropertyBox::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_haveTitle)
		m_titleTxt.drawUp();
	m_slider.drawUp();	
	//下面描绘所有属性框的上面(尚未完成)
	//for (unsigned int i = 0; i < m_propertyLines.size(); ++i)
	//{
	//	m_propertyLines[i].drawUp();
	//}
	m_ctrlManager.drawUp();
}
INLINE void XPropertyLine::draw()
{
	switch(type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
		break;
	case PROPERTY_TITLE:	//只显示名字的行
	case PROPERTY_EDIT:		//输入框
	case PROPERTY_CHECK:	//选择项
	case PROPERTY_COMBO:	//选项
		pNameFont->draw();	//显示字体
		break;
	}
}
INLINE XBool XPropertyLine::getIsFocus()
{
	switch(type)
	{
	case PROPERTY_BLANK:return XFalse;	//空白行，空行，什么都没有
	case PROPERTY_TITLE:return XFalse;	//只显示名字的行
	case PROPERTY_EDIT:		//输入框
	case PROPERTY_CHECK:	//选择项
	case PROPERTY_COMBO:	//选项
		return pCtrl->isFocus();
	}
	return XFalse;
}
INLINE void XPropertyLine::setFocus(bool flag)
{
	switch(type)
	{
	case PROPERTY_BLANK:break;	//空白行，空行，什么都没有
	case PROPERTY_TITLE:break;	//只显示名字的行
	case PROPERTY_EDIT:		//输入框
		if(flag) ((XEdit *)pCtrl)->setFocus();
		else ((XEdit *)pCtrl)->setLostFocus();
		break;
	case PROPERTY_CHECK:	//选择项
		if(flag) ((XCheck *)pCtrl)->setFocus();
		else ((XCheck *)pCtrl)->setLostFocus();
		break;
	case PROPERTY_COMBO:	//选项
		if(flag) ((XCombo *)pCtrl)->setFocus();
		else ((XCombo *)pCtrl)->setLostFocus();
		break;
	}
}
INLINE XBool XPropertyLine::canGetFocus(const XVec2& p)
{
	switch(type)
	{
	case PROPERTY_BLANK:return XFalse;	//空白行，空行，什么都没有
	case PROPERTY_TITLE:return XFalse;	//只显示名字的行
	case PROPERTY_EDIT:		//输入框
		return ((XEdit *)pCtrl)->canGetFocus(p);
	case PROPERTY_CHECK:	//选择项
		return ((XCheck *)pCtrl)->canGetFocus(p);
	case PROPERTY_COMBO:	//选项
		return ((XCombo *)pCtrl)->canGetFocus(p);
	}
	return XFalse;
}
INLINE void XPropertyLine::disable()
{
	switch(type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		break;
	case PROPERTY_EDIT:		//输入框
		((XEdit *)pCtrl)->disable();
		break;
	case PROPERTY_CHECK:	//选择项
		((XCheck *)pCtrl)->disable();
		break;
	case PROPERTY_COMBO:	//选项
		((XCombo *)pCtrl)->disable();
		break;
	}
}
INLINE void XPropertyLine::enable()
{
	switch(type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		break;
	case PROPERTY_EDIT:		//输入框
		((XEdit *)pCtrl)->enable();
		break;
	case PROPERTY_CHECK:	//选择项
		((XCheck *)pCtrl)->enable();
		break;
	case PROPERTY_COMBO:	//选项
		((XCombo *)pCtrl)->enable();
		break;
	}
}
INLINE void XPropertyBox::setColor(const XFColor& c)
{
	m_color = c;
	m_slider.setColor(c);
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].setColor(c);
	}
	m_titleTxt.setColor(XFColor(0.0f,c.a));
}
INLINE void XPropertyBox::setAlpha(float a)
{
	m_color.setA(a);
	m_slider.setAlpha(a);
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].setAlpha(a);
	}
	m_titleTxt.setAlpha(a);
}
INLINE void XPropertyLine::setAlpha(float a)
{
	if(pNameFont != NULL) pNameFont->setAlpha(a);
	switch(type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		break;
	case PROPERTY_EDIT:	//输入框
		if(pCtrl != NULL) ((XEdit *)pCtrl)->setAlpha(a);
		break;
	case PROPERTY_CHECK:	//选择项
		if(pCtrl != NULL) ((XCheck *)pCtrl)->setAlpha(a);
		break;
	case PROPERTY_COMBO:	//选项
		if(pCtrl != NULL) ((XCombo *)pCtrl)->setAlpha(a);
		break;
	}
}
INLINE void XPropertyLine::setColor(const XFColor& c)
{
	if(pNameFont != NULL) pNameFont->setColor(XFColor(0.0f,c.a));
	switch(type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		break;
	case PROPERTY_EDIT:	//输入框
		if(pCtrl != NULL) ((XEdit *)pCtrl)->setColor(c);
		break;
	case PROPERTY_CHECK:	//选择项
		if(pCtrl != NULL) ((XCheck *)pCtrl)->setColor(c);
		break;
	case PROPERTY_COMBO:	//选项
		if(pCtrl != NULL) ((XCombo *)pCtrl)->setColor(c);
		break;
	}
}
INLINE const std::string& XPropertyBox::getPropertyName(unsigned int index)const
{
	if (index >= m_propertyLines.size()) return XString::gNullStr;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:return XString::gNullStr;	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		return m_propertyLines[index].name;
	case PROPERTY_EDIT:		//输入框
	case PROPERTY_CHECK:	//选择项
	case PROPERTY_COMBO:	//选项
		return m_propertyLines[index].name;
	}
	return XString::gNullStr;
}
INLINE XBool XPropertyBox::setPropertyStr(unsigned int index, const char * str)
{
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		return XFalse;
	case PROPERTY_EDIT:		//输入框
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XEdit *)m_propertyLines[index].pCtrl)->setString(str);
		return XTrue;
	case PROPERTY_CHECK:	//选择项
	case PROPERTY_COMBO:	//选项
		return XFalse;
	}
	return XFalse;
}
INLINE XBool XPropertyBox::setPropertyCtrlPrama(unsigned int index, int prama)
{
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		return XFalse;
	case PROPERTY_EDIT:		//输入框
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XEdit *)m_propertyLines[index].pCtrl)->setEditType((XEdit::XEditType)prama);
		return XTrue;
	case PROPERTY_CHECK:	//选择项
	case PROPERTY_COMBO:	//选项
		return XFalse;
	}
	return XFalse;
}
INLINE std::string XPropertyBox::getPropertyStr(unsigned int index)const
{
	if (index >= m_propertyLines.size()) return XString::gNullStr;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		return XString::gNullStr;
	case PROPERTY_EDIT:		//输入框
		if (m_propertyLines[index].pCtrl == NULL) return XString::gNullStr;
		return ((XEdit *)m_propertyLines[index].pCtrl)->getString();
	case PROPERTY_CHECK:	//选择项
	case PROPERTY_COMBO:	//选项
		return XString::gNullStr;
	}
	return XString::gNullStr;
}
INLINE XBool XPropertyBox::setPropertyIndex(unsigned int index, int order)
{
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
	case PROPERTY_EDIT:		//输入框
	case PROPERTY_CHECK:	//选择项
		return XFalse;
	case PROPERTY_COMBO:	//选项
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XCombo *)m_propertyLines[index].pCtrl)->setCurChooseOrder(order);
		return XTrue;
	}
	return XFalse;
}
INLINE int XPropertyBox::getPropertyIndex(unsigned int index)const
{
	if (index >= m_propertyLines.size()) return -1;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
	case PROPERTY_EDIT:		//输入框
	case PROPERTY_CHECK:	//选择项
		return -1;
	case PROPERTY_COMBO:	//选项
		if (m_propertyLines[index].pCtrl == NULL) return -1;
		return ((XCombo *)m_propertyLines[index].pCtrl)->getCurChooseOrder();
	}
	return -1;
}
INLINE XBool XPropertyBox::setPropertyState(unsigned int index, XBool state)
{
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
	case PROPERTY_EDIT:		//输入框
	case PROPERTY_COMBO:	//选项
		return XFalse;
	case PROPERTY_CHECK:	//选择项
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XCheck *)m_propertyLines[index].pCtrl)->setState(state);
		return XTrue;
	}
	return XFalse;
}
INLINE XBool XPropertyBox::getPropertyState(unsigned int index)const
{//这个函数的设计不严谨，需要慎重考虑
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
	case PROPERTY_EDIT:		//输入框
	case PROPERTY_COMBO:	//选项
		return XFalse;
	case PROPERTY_CHECK:	//选择项
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		return ((XCheck *)m_propertyLines[index].pCtrl)->getState();
	}
	return XFalse;
}
INLINE XBool XPropertyBox::setPropertyMenuStr(unsigned int index, int menuIndex, const char *str)
{
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
	case PROPERTY_EDIT:		//输入框
	case PROPERTY_CHECK:	//选择项
		return XFalse;
	case PROPERTY_COMBO:	//选项
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XCombo *)m_propertyLines[index].pCtrl)->setMenuStr(str, menuIndex);
		return XTrue;
	}
	return XFalse;
}
INLINE XBool XPropertyBox::setPropertyMenuStr(unsigned int index, const char *str)
{
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
	case PROPERTY_EDIT:		//输入框
	case PROPERTY_CHECK:	//选择项
		return XFalse;
	case PROPERTY_COMBO:	//选项
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XCombo *)m_propertyLines[index].pCtrl)->setMenuStr(str);
		return XTrue;
	}
	return XFalse;
}
INLINE void XPropertyBox::setPosition(const XVec2& p)
{
	m_position = p;
	m_titleTxt.setPosition(m_position + XVec2(5.0f * m_scale.x, 0.0f));
	if (m_haveTitle)
		m_slider.setPosition(m_position + XVec2(m_width, MIN_FONT_CTRL_SIZE) * m_scale);
	else
		m_slider.setPosition(m_position + XVec2(m_width * m_scale.x, 0.0f));
	//下面更新所有属性栏的位置
	setCurStartLineIndex(m_curStartLineIndex);
	updateRect();
}
INLINE XBool XPropertyBox::mouseProc(const XVec2& p, XMouseState mouseState)		//对于鼠标动作的响应函数
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible) return XFalse; 	//如果不可见直接退出
	if (m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if (m_isSilent) return XFalse;
	m_slider.mouseProc(p, mouseState);
	m_ctrlManager.mouseProc(p, mouseState);
	return m_curMouseRect.isInRect(p);
}
INLINE XBool XPropertyBox::keyboardProc(int keyOrder, XKeyState keyState)			//返回是否触发按键动作
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if (m_withAction && m_isInAction) return XTrue;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if (m_isSilent) return XFalse;
	m_slider.keyboardProc(keyOrder, keyState);
	m_ctrlManager.keyProc(keyOrder, keyState);
	return XTrue;
}
INLINE void XPropertyBox::update(float stepTime)
{
	m_comment.update(stepTime);
	m_slider.update(stepTime);
	m_ctrlManager.update(stepTime);
}

