inline void _XButtonTexture::releaseTex()
{
	XDELETE(buttonNormal);
	XDELETE(buttonDown);
	XDELETE(buttonOn);
	XDELETE(buttonDisable);
}
inline void _XButtonTexture::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
inline _XBool _XButton::initEx(const _XVector2& position,	//上面一个接口的简化版本
		const _XButtonTexture &tex,		
		const char *caption,const _XFontUnicode &font,float captionSize)
{
	return init(position,tex.m_mouseRect,tex,caption,font,captionSize,tex.m_fontPosition);
}
inline _XBool _XButton::initPlus(const char * path,const char *caption,const _XFontUnicode &font,float captionSize,
		_XResourcePosition resoursePosition)//这是经过最终优化的版本，估计以后尽量只是用这个版本
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XBUTTON_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(_XVector2::zero,*(_XButtonTexture *)m_resInfo->m_pointer,caption,font,captionSize);
}
inline _XBool _XButton::initWithoutTex(const char *caption,const _XFontUnicode &font,const _XRect& area)	//这个接口是上个接口的简化版本
{
	return initWithoutTex(caption,font,1.0f,area,area.getCenter());
}
inline void _XButton::setTextColor(const _XFColor& color) 
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
inline void _XButton::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	if(m_withNormalIcon) m_normalIcon.setColor(m_color);
	if(m_withDisableIcon) m_disableIcon.setColor(m_color);
	updateChildColor();
}
inline void _XButton::setAlpha(float a)
{
	m_color.setA(a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	if(m_withNormalIcon) m_normalIcon.setColor(m_color);
	if(m_withDisableIcon) m_disableIcon.setColor(m_color);
	updateChildAlpha();
}
inline _XBool _XButton::canGetFocus(float x,float y)				//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return m_nowMouseRect.isInRect(x,y);
}
inline void _XButton::setLostFocus() 
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||		//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	if(m_nowButtonState != BUTTON_STATE_DISABLE) m_nowButtonState = BUTTON_STATE_NORMAL;
	m_isBeChoose = XFalse;	//控件处于焦点状态
}
inline _XBool _XButton::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XButton::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
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
inline void _XButton::setCaptionPosition(const _XVector2& textPosition)			//设置按钮的标题的位置，相对于按键左上角
{
	setCaptionPosition(textPosition.x,textPosition.y);
}
inline void _XButton::setCaptionPosition(float x,float y)			//设置按钮的标题的位置，相对于按键左上角
{
	m_textPosition.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
}
inline void _XButton::setCaptionSize(const _XVector2& size)						//设置按钮的标题的尺寸
{
	setCaptionSize(size.x,size.y);
}
inline void _XButton::setCaptionSize(float x,float y)						//设置按钮的标题的尺寸
{
	if(x < 0 || y < 0) return;
	m_textSize.set(x,y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
}
inline void _XButton::setHotKey(int hotKey)	//设置按键的热键
{
	m_hotKey = hotKey;
}
inline void _XButton::setCaptionText(const char *caption)						//设置按钮的标题的文字
{
	if(caption != NULL) m_caption.setString(caption);
}
inline int _XButton::getHotKey() const			//获取当前按键的热键信息
{
	return m_hotKey;
}
inline void _XButton::setState(_XButtonState temp)		//设置按钮的状态
{
	m_nowButtonState = temp;
}
 
inline _XButtonState _XButton::getState() const
{
	if(!m_isEnable) return BUTTON_STATE_DISABLE;
	return m_nowButtonState;
}
inline void _XButton::disable()
{
	m_isEnable = XFalse;
	m_nowButtonState = BUTTON_STATE_DISABLE;
	m_isBeChoose = XFalse;
	m_caption.setColor((m_textColor * m_color).anti());
}
inline void _XButton::enable()
{
	if(m_nowButtonState == BUTTON_STATE_DISABLE)
	{
		m_isEnable = XTrue;
		m_nowButtonState = BUTTON_STATE_NORMAL;
		mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
		m_caption.setColor(m_textColor * m_color);
	}
}
inline void _XButton::setMouseDownCB(void (* funMouseDown)(void *,int),void *pClass)
{
	m_funMouseDown = funMouseDown;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline void _XButton::setMouseUpCB(void (* funMouseUp)(void *,int),void *pClass)
{
	m_funMouseUp = funMouseUp;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline void _XButton::setIconPosition(float x,float y)	//设置icon的位置
{
	m_iconPosition.set(x,y);
	if(m_withNormalIcon) m_normalIcon.setPosition(m_position + m_iconPosition * m_size);
	if(m_withDisableIcon) m_disableIcon.setPosition(m_position + m_iconPosition * m_size);
}
inline void _XButton::setIconSize(float x,float y)		//设置icon的缩放大小
{
	if(x <= 0.0f || y <= 0.0f) return;
	m_iconSize.set(x,y);
	if(m_withNormalIcon) m_normalIcon.setSize(m_iconSize * m_size);
	if(m_withDisableIcon) m_disableIcon.setSize(m_iconSize * m_size);
}
inline void _XButton::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_withoutTex && !m_lightMD.getIsEnd())
	{
		drawRect(m_lightRect,1.0f * m_lightMD.getNowData() * 2.0f,1.0f,1.0f,1.0f,(1.0f - m_lightMD.getNowTimer()) * 0.5f);
	}
	m_comment.draw();
}
