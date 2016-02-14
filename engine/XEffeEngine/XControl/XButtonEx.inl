INLINE XBool XButtonEx::initEx(const XVector2& position,	//这是对上面接口的简化
		const XButtonSkin &tex,				
		const char *caption,float captionSize,	
		const XFontUnicode &font)
{
	if(tex.m_pArea == NULL || tex.m_areaPointSum <= 2) return XFalse;	//控件必须要有一个合适的响应区间，不然会造成错误
	return init(position,tex.m_pArea,tex.m_areaPointSum,tex,caption,captionSize,tex.m_fontPosition,font);
}
INLINE XBool XButtonEx::initPlus(const char *path,				//按钮的个中贴图信息
		const char *caption,float captionSize,	//按钮上现实的文字的相关信息
		const XFontUnicode &font,XResourcePosition resoursePosition)				//按钮上使用的字体
{
	if(m_isInited) return XFalse;
	if(path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXBUTTON_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVector2::zero,*(XButtonSkin *)m_resInfo->m_pointer,caption,captionSize,font);
}
INLINE void XButtonEx::setTextColor(const XFColor& color) 
{
	m_textColor = color;
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);
}
INLINE void XButtonEx::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	m_sprite.setColor(m_color);
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);
}	//设置按钮的颜色
INLINE void XButtonEx::setAlpha(float a) 
{
	m_color.setA(a);
	m_sprite.setColor(m_color);
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);
}
INLINE XBool XButtonEx::canGetFocus(float x,float y)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return XMath::getIsInRect(x,y,m_pCurArea,m_areaPointSum);
}
INLINE void XButtonEx::setLostFocus() 
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	if(m_curButtonState != BUTTON_STATE_DISABLE) 
	{
		m_curButtonState = BUTTON_STATE_NORMAL;
//		m_isBeChoose = XFalse;
	}
}
INLINE XBool XButtonEx::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
	//return getIsInRect(m_upMousePoint,m_pCurArea,m_areaPointSum);
}
INLINE void XButtonEx::setCaptionPosition(const XVector2& textPosition)			//设置按钮的标题的位置，相对于按键左上角
{
	setCaptionPosition(textPosition.x,textPosition.y);
}
INLINE void XButtonEx::setCaptionPosition(float x,float y)			//设置按钮的标题的位置，相对于按键左上角
{
	m_textPosition.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
}
INLINE void XButtonEx::setCaptionSize(const XVector2& size)						//设置按钮的标题的尺寸
{
	setCaptionSize(size.x,size.y);
}
INLINE void XButtonEx::setCaptionSize(float x,float y)						//设置按钮的标题的尺寸
{
	if(x < 0 || y < 0) return;
	m_textSize.set(x,y);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
}
INLINE void XButtonEx::setHotKey(int hotKey)	//设置按键的热键
{
	m_hotKey = hotKey;
}
INLINE void XButtonEx::setCaptionText(const char *caption)						//设置按钮的标题的文字
{
	if(caption != NULL) m_caption.setString(caption);
}
INLINE int XButtonEx::getHotKey() const			//获取当前按键的热键信息
{
	return m_hotKey;
}
INLINE void XButtonEx::setState(XButtonState temp)		//设置按钮的状态
{
	m_curButtonState = temp;
} 
INLINE XButtonState XButtonEx::getState() const
{
	if(!m_isEnable) return BUTTON_STATE_DISABLE;
	return m_curButtonState;
}
INLINE void XButtonEx::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
	m_curButtonState = BUTTON_STATE_DISABLE;
}
INLINE void XButtonEx::enable()
{
	if(m_curButtonState != BUTTON_STATE_DISABLE) return;
	m_isEnable = XTrue;
	m_curButtonState = BUTTON_STATE_NORMAL;
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
}