INLINE void XCheckSkin::releaseTex()
{
	XMem::XDELETE(checkChoosed);
	XMem::XDELETE(checkDischoose);
	XMem::XDELETE(checkDisableChoosed);
	XMem::XDELETE(checkDisableDischoose);
}
INLINE void XCheckSkin::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
INLINE XBool XCheck::initEx(const XVector2 & position,	//对上面接口的简化
		const XCheckSkin &tex,			
		const char *caption,const XFontUnicode &font,float captionSize)
{
	return init(position,tex.m_mouseRect,tex,caption,font,captionSize,tex.m_fontPosition);
}
INLINE XBool XCheck::initPlus(const char * path,			//控件的贴图的文件夹路径
		const char *caption,const XFontUnicode &font,float captionSize,
		XResourcePosition resoursePosition)
{
	if(m_isInited ||
		path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXCHECK_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVector2::zero,*(XCheckSkin *)m_resInfo->m_pointer,caption,font,captionSize);
}
INLINE XBool XCheck::initWithoutSkin(const char *caption,			//这个是上一个接口的简化版本
		const XFontUnicode &font,const XRect& area)
{
	return initWithoutSkin(caption,font,1.0f,area,XVector2(area.right + 5.0f,area.getYCenter()));
}
INLINE void XCheck::setTextColor(const XFColor& color) 
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
INLINE void XCheck::setColor(float r,float g,float b,float a) 	//设置按钮的颜色
{
	m_color.setColor(r,g,b,a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	updateChildColor();
}	//设置按钮的颜色
INLINE void XCheck::setAlpha(float a) 
{
	m_color.setA(a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	updateChildAlpha();
}	//设置按钮的颜色
INLINE void XCheck::setCaptionText(const char *temp)			//设置复选框的文字
{
	if(temp == NULL) return;
	m_caption.setString(temp);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
}
INLINE XBool XCheck::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE XBool XCheck::canGetFocus(float x,float y)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_withCaption)
	{
		if(m_mouseClickArea.isInRect(x,y) || m_curMouseRect.isInRect(x,y)) return XTrue;
	}else
	{
		if(m_curMouseRect.isInRect(x,y)) return XTrue;
	}
	return XFalse;
}
INLINE void XCheck::disable()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(m_isEnable) m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
INLINE void XCheck::enable()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(!m_isEnable) m_isEnable = XTrue;
}
INLINE XBool XCheck::getState() const
{
	return m_state;
}
INLINE void XCheck::setState(XBool temp)
{
	//理论上状态不论何时都是可以改变的
//	if(!m_isVisible) return;	//如果不可见直接退出
//	if(!m_isEnable) return;
	if((m_state && temp) || (!m_state && !temp)) return;
	m_state = temp;
//	if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
}