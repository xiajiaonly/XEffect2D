INLINE void XPropertyBox::setTitle(const char * title)	//设置标题
{
	if(title == NULL) return;
	m_haveTitle = XTrue;
	m_titleStr = title;
	m_titleTxt.setString(XString::getCanShowString(title,(int)(m_width - 10.0f + MIN_FONT_CTRL_SIZE),16).c_str());
	m_titleTxt.setVisible();
	updateRect();
	if(m_haveTitle)
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y + MIN_FONT_CTRL_SIZE * m_scale.y);
	else
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y);
}
INLINE void XPropertyBox::disTitle()					//取消标题
{
	m_haveTitle = XFalse;
	m_titleTxt.disVisible();
	updateRect();
	if(m_haveTitle)
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y + MIN_FONT_CTRL_SIZE * m_scale.y);
	else
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y);
}
INLINE XBool XPropertyBox::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XPropertyBox::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
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
INLINE XBool XPropertyBox::canGetFocus(float x,float y)				//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return m_curMouseRect.isInRect(x,y);
}
INLINE void XPropertyBox::setLostFocus() 
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出
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
	//下面依次显示所有的属性项
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].drawUp();
	}
}
INLINE void XPropertyLine::draw()
{
	switch(type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
		break;
	case PROPERTY_TITLE:	//只显示名字的行
		pNameFont->draw();	//显示字体
		break;
	case PROPERTY_EDIT:		//输入框
	case PROPERTY_CHECK:	//选择项
	case PROPERTY_COMBO:	//选项
		pNameFont->draw();	//显示字体
		pCtrl->draw();
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
INLINE XBool XPropertyLine::canLostFocus(float x,float y)
{
	switch(type)
	{
	case PROPERTY_BLANK:return XTrue;	//空白行，空行，什么都没有
	case PROPERTY_TITLE:return XTrue;	//只显示名字的行
	case PROPERTY_EDIT:		//输入框
		return ((XEdit *)pCtrl)->canLostFocus(x,y);
	case PROPERTY_CHECK:	//选择项
		return ((XCheck *)pCtrl)->canLostFocus(x,y);
	case PROPERTY_COMBO:	//选项
		return ((XCombo *)pCtrl)->canLostFocus(x,y);
	}
	return XTrue;
}
INLINE XBool XPropertyLine::canGetFocus()
{
	switch(type)
	{
	case PROPERTY_BLANK:return XFalse;	//空白行，空行，什么都没有
	case PROPERTY_TITLE:return XFalse;	//只显示名字的行
	case PROPERTY_EDIT:		//输入框
		return ((XEdit *)pCtrl)->getVisible() && ((XEdit *)pCtrl)->getEnable();
	case PROPERTY_CHECK:	//选择项
		return ((XCheck *)pCtrl)->getVisible() && ((XCheck *)pCtrl)->getEnable();
	case PROPERTY_COMBO:	//选项
		return ((XCombo *)pCtrl)->getVisible() && ((XCombo *)pCtrl)->getEnable();
	}
	return XFalse;
}
INLINE void XPropertyLine::drawUp()
{
	switch(type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		break;
	case PROPERTY_EDIT:		//输入框
	case PROPERTY_CHECK:	//选择项
	case PROPERTY_COMBO:	//选项
		pCtrl->drawUp();
		break;
	}
}
INLINE XBool XPropertyLine::mouseProc(float x,float y,XMouseState mouseState)
{
	switch(type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		break;
	case PROPERTY_EDIT:		//输入框
		((XEdit *)pCtrl)->mouseProc(x,y,mouseState);
		break;
	case PROPERTY_CHECK:	//选择项
		((XCheck *)pCtrl)->mouseProc(x,y,mouseState);
		break;
	case PROPERTY_COMBO:	//选项
		((XCombo *)pCtrl)->mouseProc(x,y,mouseState);
		break;
	}
	return XTrue;
}
INLINE XBool XPropertyLine::keyboardProc(int keyOrder,XKeyState keyState)
{
	switch(type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		break;
	case PROPERTY_EDIT:		//输入框
		((XEdit *)pCtrl)->keyboardProc(keyOrder,keyState);
		break;
	case PROPERTY_CHECK:	//选择项
		((XCheck *)pCtrl)->keyboardProc(keyOrder,keyState);
		break;
	case PROPERTY_COMBO:	//选项
		((XCombo *)pCtrl)->keyboardProc(keyOrder,keyState);
		break;
	}
	return XTrue;
}
INLINE void XPropertyLine::update(float stepTime)
{
	switch(type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		break;
	case PROPERTY_EDIT:		//输入框
		((XEdit *)pCtrl)->update(stepTime);
		break;
	case PROPERTY_CHECK:	//选择项
		((XCheck *)pCtrl)->update(stepTime);
		break;
	case PROPERTY_COMBO:	//选项
		((XCombo *)pCtrl)->update(stepTime);
		break;
	}
}
INLINE void XPropertyLine::insertChar(const char *ch,int len)
{
	switch(type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
	case PROPERTY_TITLE:	//只显示名字的行
		break;
	case PROPERTY_EDIT:		//输入框
		((XEdit *)pCtrl)->insertChar(ch,len);
		break;
	case PROPERTY_CHECK:	//选择项
		((XCheck *)pCtrl)->insertChar(ch,len);
		break;
	case PROPERTY_COMBO:	//选项
		((XCombo *)pCtrl)->insertChar(ch,len);
		break;
	}
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
INLINE void XPropertyBox::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_slider.setColor(r,g,b,a);
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].setColor(r,g,b,a);
	}
	m_titleTxt.setColor(0.0f,0.0f,0.0f,a);
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
		break;
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
INLINE void XPropertyLine::setColor(float r,float g,float b,float a)
{
	if(pNameFont != NULL) pNameFont->setColor(0.0f,0.0f,0.0f,a);
	switch(type)
	{
	case PROPERTY_BLANK:	//空白行，空行，什么都没有
		break;
	case PROPERTY_TITLE:	//只显示名字的行
		break;
	case PROPERTY_EDIT:	//输入框
		if(pCtrl != NULL) ((XEdit *)pCtrl)->setColor(r,g,b,a);
		break;
	case PROPERTY_CHECK:	//选择项
		if(pCtrl != NULL) ((XCheck *)pCtrl)->setColor(r,g,b,a);
		break;
	case PROPERTY_COMBO:	//选项
		if(pCtrl != NULL) ((XCombo *)pCtrl)->setColor(r,g,b,a);
		break;
	}
}