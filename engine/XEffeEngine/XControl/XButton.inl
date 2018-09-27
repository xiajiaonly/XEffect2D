INLINE void XButtonSkin::releaseTex()
{
	XMem::XDELETE(buttonNormal);
	XMem::XDELETE(buttonDown);
	XMem::XDELETE(buttonOn);
	XMem::XDELETE(buttonDisable);
}
INLINE void XButtonSkin::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
INLINE XBool XButton::initEx(const XVec2& position,	//上面一个接口的简化版本
		const XButtonSkin &tex,		
		const char *caption,const XFontUnicode& font,float captionSize)
{
	return init(position,tex.m_mouseRect,tex,caption,font,captionSize,tex.m_fontPosition);
}
INLINE XBool XButton::initPlus(const char * path,const char *caption,const XFontUnicode& font,float captionSize,
		XResPos resPos)//这是经过最终优化的版本，估计以后尽量只是用这个版本
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXBUTTON_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVec2::zero,*(XButtonSkin *)m_resInfo->m_pointer,caption,font,captionSize);
}
INLINE XBool XButton::initWithoutSkin(const char *caption,const XFontUnicode& font,const XRect& area)	//这个接口是上个接口的简化版本
{
	return initWithoutSkin(caption,font,1.0f,area,area.getCenter());
}
INLINE void XButton::setTextColor(const XFColor& color) 
{
	m_textColor = color;
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);
}
INLINE void XButton::setColor(const XFColor& c)
{
	m_color = c;
	m_sprite.setColor(m_color);
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);
	if(m_withNormalIcon) m_normalIcon.setColor(m_color);
	if(m_withDisableIcon) m_disableIcon.setColor(m_color);
	updateChildColor();
}
INLINE void XButton::setAlpha(float a)
{
	m_color.setA(a);
	m_sprite.setColor(m_color);
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);
	if(m_withNormalIcon) m_normalIcon.setColor(m_color);
	if(m_withDisableIcon) m_disableIcon.setColor(m_color);
	updateChildAlpha();
}
INLINE XBool XButton::canGetFocus(const XVec2& p)				//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return m_curMouseRect.isInRect(p);
}
INLINE void XButton::setLostFocus() 
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	if(m_curButtonState != BUTTON_STATE_DISABLE) m_curButtonState = BUTTON_STATE_NORMAL;
	m_isBeChoose = XFalse;	//控件处于焦点状态
}
INLINE XBool XButton::isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XButton::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
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
INLINE void XButton::setCaptionPosition(const XVec2& textPosition)			//设置按钮的标题的位置，相对于按键左上角
{
	m_textPosition = textPosition;
	m_caption.setPosition(m_position + m_textPosition * m_scale);
}
//INLINE void XButton::setCaptionPosition(float x,float y)			//设置按钮的标题的位置，相对于按键左上角
//{
//	setCaptionPosition(XVec2(x,y));
//}
INLINE void XButton::setCaptionSize(const XVec2& size)						//设置按钮的标题的尺寸
{
	if(size.x < 0 || size.y < 0) return;
	m_textSize = size;
	m_caption.setScale(m_textSize * m_scale);
}
//INLINE void XButton::setCaptionSize(float x,float y)						//设置按钮的标题的尺寸
//{
//	setCaptionSize(XVec2(x,y));
//}
INLINE void XButton::setHotKey(int hotKey)	//设置按键的热键
{
	m_hotKey = hotKey;
}
INLINE void XButton::setCaptionText(const char *caption)						//设置按钮的标题的文字
{
	if(caption != NULL) m_caption.setString(caption);
}
INLINE int XButton::getHotKey() const			//获取当前按键的热键信息
{
	return m_hotKey;
}
INLINE void XButton::setState(XButtonState temp)		//设置按钮的状态
{
	m_curButtonState = temp;
} 
INLINE XButtonState XButton::getState() const
{
	if(!m_isEnable) return BUTTON_STATE_DISABLE;
	return m_curButtonState;
}
INLINE void XButton::disable()
{
	m_isEnable = XFalse;
	m_curButtonState = BUTTON_STATE_DISABLE;
	m_isBeChoose = XFalse;
	m_caption.setColor((m_textColor * m_color).anti());
}
INLINE void XButton::enable()
{
	if(m_curButtonState != BUTTON_STATE_DISABLE) return;
	m_isEnable = XTrue;
	m_curButtonState = BUTTON_STATE_NORMAL;
	//2016.6.5下面的函数会造成一些显示上的问题，但是有不明确为什么要这么做，所以先屏蔽掉
//	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	m_caption.setColor(m_textColor * m_color);
}
INLINE void XButton::setIconPosition(const XVec2& p)	//设置icon的位置
{
	m_iconPosition = p;
	if(m_withNormalIcon) m_normalIcon.setPosition(m_position + m_iconPosition * m_scale);
	if(m_withDisableIcon) m_disableIcon.setPosition(m_position + m_iconPosition * m_scale);
}
INLINE void XButton::setIconSize(const XVec2& p)		//设置icon的缩放大小
{
	if(p.x <= 0.0f || p.y <= 0.0f) return;
	m_iconSize = p;
	if(m_withNormalIcon) m_normalIcon.setScale(m_iconSize * m_scale);
	if(m_withDisableIcon) m_disableIcon.setScale(m_iconSize * m_scale);
}
INLINE void XButton::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_withoutTex && !m_lightMD.getIsEnd())
	{
		XRender::drawRect(m_lightRect,1.0f * m_lightMD.getCurData() * 2.0f,XFColor(1.0f,(1.0f - m_lightMD.getCurTimer()) * 0.5f));
	}
	m_comment.draw();
}
