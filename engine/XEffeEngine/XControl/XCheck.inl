inline void _XCheckTexture::releaseTex()
{
	XDELETE(checkChoosed);
	XDELETE(checkDischoose);
	XDELETE(checkDisableChoosed);
	XDELETE(checkDisableDischoose);
}
inline void _XCheckTexture::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
inline _XBool _XCheck::initEx(const _XVector2 & position,	//对上面接口的简化
		const _XCheckTexture &tex,			
		const char *caption,const _XFontUnicode &font,float captionSize)
{
	return init(position,tex.m_mouseRect,tex,caption,font,captionSize,tex.m_fontPosition);
}
inline _XBool _XCheck::initPlus(const char * path,			//控件的贴图的文件夹路径
		const char *caption,const _XFontUnicode &font,float captionSize,
		_XResourcePosition resoursePosition)
{
	if(m_isInited ||
		path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XCHECK_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(_XVector2::zero,*(_XCheckTexture *)m_resInfo->m_pointer,caption,font,captionSize);
}
inline _XBool _XCheck::initWithoutTex(const char *caption,			//这个是上一个接口的简化版本
		const _XFontUnicode &font,const _XRect& area)
{
	return initWithoutTex(caption,font,1.0f,area,_XVector2(area.right + 5.0f,area.getYCenter()));
}
inline void _XCheck::setTextColor(const _XFColor& color) 
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
inline void _XCheck::setColor(float r,float g,float b,float a) 	//设置按钮的颜色
{
	m_color.setColor(r,g,b,a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	updateChildColor();
}	//设置按钮的颜色
inline void _XCheck::setAlpha(float a) 
{
	m_color.setA(a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	updateChildAlpha();
}	//设置按钮的颜色
inline void _XCheck::setCaptionText(const char *temp)			//设置复选框的文字
{
	if(temp == NULL) return;
	m_caption.setString(temp);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
}
inline _XBool _XCheck::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
inline _XBool _XCheck::canGetFocus(float x,float y)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_withCaption)
	{
		if(m_mouseClickArea.isInRect(x,y) || m_nowMouseRect.isInRect(x,y)) return XTrue;
	}else
	{
		if(m_nowMouseRect.isInRect(x,y)) return XTrue;
	}
	return XFalse;
}
inline void _XCheck::disable()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(m_isEnable) m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
inline void _XCheck::enable()
{
	if(!m_isInited) return ;	//如果没有初始化直接退出
	if(!m_isEnable) m_isEnable = XTrue;
}
inline _XBool _XCheck::getState() const
{
	return m_state;
}
inline void _XCheck::setState(_XBool temp)
{
	//理论上状态不论何时都是可以改变的
//	if(!m_isVisible) return;	//如果不可见直接退出
//	if(!m_isEnable) return;
	if((m_state && temp) || (!m_state && !temp)) return;
	m_state = temp;
//	if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
}
inline void _XCheck::setStateChangeCB(void (*funStateChange)(void *,int),void *pClass)
{
	m_funStateChange = funStateChange;			//控件状态改变时调用
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline void _XCheck::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_withoutTex && !m_lightMD.getIsEnd())
	{
		drawRect(m_lightRect,1.0f * m_lightMD.getNowData() * 2.0f,1.0f,1.0f,1.0f,(1.0f - m_lightMD.getNowTimer()) * 0.5f);
	}
	m_comment.draw();
}