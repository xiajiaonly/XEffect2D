INLINE void XProgressSkin::release()
{
	if(!m_isInited) return;
	XMem::XDELETE(progressBackGround);
	XMem::XDELETE(progressMove);
	XMem::XDELETE(progressUpon);
	m_isInited = XFalse;
}
INLINE float XProgress::getValue() const
{
	return m_curValue;
}
INLINE void XProgress::enable()
{
	m_isEnable = XTrue;
}
INLINE void XProgress::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
INLINE XBool XProgress::initEx(const XVector2& position,//对上面接口的简化
	const XProgressSkin &tex,	
	XNumber* font,float captionSize,
	int mode)
{
	return init(position,tex.m_mouseRect,tex,font,captionSize,tex.m_fontPosition,mode);
}
INLINE XBool XProgress::initPlus(const char * path,
	XNumber* font,float captionSize,	//控件的文字
	int mode,XResourcePosition resoursePosition)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXPROGRESS_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVector2::zero,*(XProgressSkin *)m_resInfo->m_pointer,font,captionSize,mode);
}
INLINE void XProgress::setTextColor(const XFColor& color) 
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
INLINE void XProgress::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	m_spriteBackground.setColor(m_color);
	m_spriteMove.setColor(m_color);
	m_spriteUpon.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
}	//设置按钮的颜色
INLINE void XProgress::setAlpha(float a) 
{
	m_color.setA(a);
	m_spriteBackground.setColor(m_color);
	m_spriteMove.setColor(m_color);
	m_spriteUpon.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
}
INLINE XBool XProgress::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XProgress::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
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