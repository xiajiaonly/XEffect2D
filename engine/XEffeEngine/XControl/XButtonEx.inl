inline _XBool _XButtonEx::initEx(const _XVector2& position,	//这是对上面接口的简化
		const _XButtonTexture &tex,				
		const char *caption,float captionSize,	
		const _XFontUnicode &font)
{
	if(tex.m_pArea == NULL || tex.m_areaPointSum <= 2) return XFalse;	//控件必须要有一个合适的响应区间，不然会造成错误
	return init(position,tex.m_pArea,tex.m_areaPointSum,tex,caption,captionSize,tex.m_fontPosition,font);
}
inline _XBool _XButtonEx::initPlus(const char *path,				//按钮的个中贴图信息
		const char *caption,float captionSize,	//按钮上现实的文字的相关信息
		const _XFontUnicode &font,_XResourcePosition resoursePosition)				//按钮上使用的字体
{
	if(m_isInited) return XFalse;
	if(path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XBUTTON_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(_XVector2::zero,*(_XButtonTexture *)m_resInfo->m_pointer,caption,captionSize,font);
}
inline void _XButtonEx::setTextColor(const _XFColor& color) 
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
inline void _XButtonEx::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
}	//设置按钮的颜色
inline void _XButtonEx::setAlpha(float a) 
{
	m_color.setA(a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
}
inline _XBool _XButtonEx::canGetFocus(float x,float y)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return getIsInRect(x,y,m_pNowArea,m_areaPointSum);
}
inline void _XButtonEx::setLostFocus() 
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	if(m_nowButtonState != BUTTON_STATE_DISABLE) 
	{
		m_nowButtonState = BUTTON_STATE_NORMAL;
//		m_isBeChoose = XFalse;
	}
}
inline _XBool _XButtonEx::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
	//return getIsInRect(m_upMousePoint,m_pNowArea,m_areaPointSum);
}
inline void _XButtonEx::setCaptionPosition(const _XVector2& textPosition)			//设置按钮的标题的位置，相对于按键左上角
{
	setCaptionPosition(textPosition.x,textPosition.y);
}
inline void _XButtonEx::setCaptionPosition(float x,float y)			//设置按钮的标题的位置，相对于按键左上角
{
	m_textPosition.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
}
inline void _XButtonEx::setCaptionSize(const _XVector2& size)						//设置按钮的标题的尺寸
{
	setCaptionSize(size.x,size.y);
}
inline void _XButtonEx::setCaptionSize(float x,float y)						//设置按钮的标题的尺寸
{
	if(x < 0 || y < 0) return;
	m_textSize.set(x,y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
}
inline void _XButtonEx::setHotKey(int hotKey)	//设置按键的热键
{
	m_hotKey = hotKey;
}
inline void _XButtonEx::setCaptionText(const char *caption)						//设置按钮的标题的文字
{
	if(caption != NULL) m_caption.setString(caption);
}
inline int _XButtonEx::getHotKey() const			//获取当前按键的热键信息
{
	return m_hotKey;
}
inline void _XButtonEx::setState(_XButtonState temp)		//设置按钮的状态
{
	m_nowButtonState = temp;
} 
inline _XButtonState _XButtonEx::getState() const
{
	if(!m_isEnable) return BUTTON_STATE_DISABLE;
	return m_nowButtonState;
}
inline void _XButtonEx::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
	m_nowButtonState = BUTTON_STATE_DISABLE;
}
inline void _XButtonEx::enable()
{
	if(m_nowButtonState != BUTTON_STATE_DISABLE) return;
	m_isEnable = XTrue;
	m_nowButtonState = BUTTON_STATE_NORMAL;
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
}
inline void _XButtonEx::setMouseDownCB(void (* funMouseDown)(void *,int),void *pClass)
{
	m_funMouseDown = funMouseDown;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline void _XButtonEx::setMouseUpCB(void (* funMouseUp)(void *,int),void *pClass)
{
	m_funMouseUp = funMouseUp;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}