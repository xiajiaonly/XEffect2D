INLINE void XPasswordPadSkin::release()
{
	if(!m_isInited) return;
	m_bottomTex.release();
	if(backGround != NULL)
	{
		backGround->release();			//无效状态下的按钮贴图
		XMem::XDELETE(backGround);
	}
	m_isInited = XFalse;
}
INLINE int XPasswordPad::getIsEnd()
{
	return m_stage;
}
INLINE void XPasswordPad::setDisShow()
{
	m_isVisible = XFalse;
}
INLINE void XPasswordPad::getPassword(char *p) const	//获取新的密码
{
	if(p == NULL) return;
	if(strlen(m_newPassword) < m_minPasswardLength) return;
	strcpy(p,m_newPassword);
}
INLINE void XPasswordPad::setPassword(const char *p)	//设置需要输入的正确密码
{
	if(p == NULL) return;	//输入参数非法
	if(strlen(p) <= m_minPasswardLength || strlen(p) > m_maxPasswardLength) return;	//密码长度不合法
	strcpy(m_oldPassword,p);
}
INLINE XBool XPasswordPad::initEx(const XVector2& position,XPasswordPadSkin *tex,const XFontUnicode& font)
{
	return init(position,tex,font,tex->m_data);
}
INLINE XBool XPasswordPad::initPlus(const char * path,const XFontUnicode& font,
	XResourcePosition resoursePosition)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXPASSWORDPAD_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVector2::zero,(XPasswordPadSkin *)m_resInfo->m_pointer,font);
}
INLINE void XPasswordPad::setScale(float x,float y)
{//尚未实现
	if(!m_isInited ||
		x <= 0 && y <= 0) return;
	m_scale.set(x,y);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	//设置各个物件的位置
	m_caption.setPosition(m_position + m_passwardPadData.captionPosition * m_scale);
	m_caption.setScale(m_scale);
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].setPosition(m_position + XVector2(m_passwardPadData.bottonOffsetPosition.x + (i % 3) * m_passwardPadData.bottonDistance.x,
		m_passwardPadData.bottonOffsetPosition.y + (int)(i / 3) * m_passwardPadData.bottonDistance.y) * m_scale);
		m_button[i].setScale(m_scale);
	}
	m_sprite.setPosition(m_position);
	m_sprite.setScale(m_scale);

	updateChildScale();
}
INLINE void XPasswordPad::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_caption.setColor(m_color);			//显示文字的字体
	m_sprite.setColor(m_color);		//显示背景贴图
	for(int i = 0;i < 12;++ i)
		m_button[i].setColor(m_color);	//12个小键盘的按键
	updateChildColor();
}
INLINE void XPasswordPad::setAlpha(float a)
{
	m_color.setA(a);
	m_caption.setAlpha(a);			//显示文字的字体
	m_sprite.setAlpha(a);		//显示背景贴图
	for(int i = 0;i < 12;++ i)
		m_button[i].setAlpha(a);	//12个小键盘的按键

	updateChildAlpha();
}
INLINE XBool XPasswordPad::canGetFocus(float x,float y)				//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
INLINE XBool XPasswordPad::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XPasswordPad::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return XVector2::zero;

	switch(order)
	{
	case 0: return XVector2(m_curMouseRect.left,m_curMouseRect.top);
	case 1: return XVector2(m_curMouseRect.right,m_curMouseRect.top);
	case 2: return XVector2(m_curMouseRect.right,m_curMouseRect.bottom);
	case 3: return XVector2(m_curMouseRect.left,m_curMouseRect.bottom); 
	}
	return XVector2::zero;
}
INLINE void XPasswordPad::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].drawUp();
	}
}
INLINE void XPasswordPad::update(float stepTime)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].update(stepTime);
	}
}