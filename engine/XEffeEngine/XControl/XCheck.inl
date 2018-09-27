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
INLINE XBool XCheck::initEx(const XVec2& position,	//对上面接口的简化
		const XCheckSkin &tex,			
		const char *caption,const XFontUnicode& font,float captionSize)
{
	return init(position,tex.m_mouseRect,tex,caption,font,captionSize,tex.m_fontPosition);
}
INLINE XBool XCheck::initPlus(const char * path,			//控件的贴图的文件夹路径
		const char *caption,const XFontUnicode& font,float captionSize,
		XResPos resPos)
{
	if(m_isInited ||
		path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXCHECK_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVec2::zero,*(XCheckSkin *)m_resInfo->m_pointer,caption,font,captionSize);
}
INLINE XBool XCheck::initWithoutSkin(const char *caption,			//这个是上一个接口的简化版本
		const XFontUnicode& font,const XRect& area)
{
	return initWithoutSkin(caption,font,1.0f,area,XVec2(area.right + 5.0f,area.getYCenter()));
}
INLINE void XCheck::setTextColor(const XFColor& color) 
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
INLINE void XCheck::setColor(const XFColor& c) 	//设置按钮的颜色
{
	m_color = c;
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
	m_mouseClickArea.set(m_caption.getBox(0),m_caption.getBox(2));
}
INLINE XBool XCheck::isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(p,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE XBool XCheck::canGetFocus(const XVec2& p)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_withCaption)
	{
		if(m_mouseClickArea.isInRect(p) || m_curMouseRect.isInRect(p)) return XTrue;
	}else
	{
		if(m_curMouseRect.isInRect(p)) return XTrue;
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