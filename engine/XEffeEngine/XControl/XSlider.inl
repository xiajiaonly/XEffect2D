INLINE void XSliderSkin::releaseTex()
{
	XMem::XDELETE(sliderLineNormal);
	XMem::XDELETE(sliderLineDown);
	XMem::XDELETE(sliderLineOn);
	XMem::XDELETE(sliderLineDisable);
	XMem::XDELETE(sliderButtonNormal);
	XMem::XDELETE(sliderButtonDown);
	XMem::XDELETE(sliderButtonOn);
	XMem::XDELETE(sliderButtonDisable);
}
INLINE void XSliderSkin::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
INLINE void XSlider::disable()//使控件无效
{
	m_curSliderState = SLIDER_STATE_DISABLE;
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
INLINE void XSlider::enable()//使控件有效
{
	if(m_curSliderState == SLIDER_STATE_DISABLE)
	{
		m_curSliderState = SLIDER_STATE_NORMAL;
		//还需要调用鼠标函数
		mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	}
	m_isEnable = XTrue;
}
INLINE float XSlider::getCurValue() const	//获取滑块当前的值
{
	return m_curValue;
}
INLINE float XSlider::getMaxValue() const
{
	return m_maxValue;
}
INLINE float XSlider::getMinValue() const
{
	return m_minValue;
}
INLINE XBool XSlider::initEx(const XVector2& position,	//对上面接口的简化
	const XSliderSkin &tex,float max,float min,XSliderType type)
{
	return init(position,tex.m_mouseRect,tex.m_mouseRectButton,tex,max,min,type);
}
INLINE XBool XSlider::initPlus(const char * path,float max,float min,XSliderType type,
	XResourcePosition resoursePosition)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXSLIDER_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVector2::zero,*(XSliderSkin *)m_resInfo->m_pointer,max,min,type);
}
INLINE void XSlider::setColor(float r,float g,float b,float a)
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);
	m_buttonSprite.setColor(m_color);
	m_lineSprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	updateChildColor();
}
INLINE void XSlider::setTextColor(const XFColor& color)
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
INLINE void XSlider::setAlpha(float a)
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_buttonSprite.setColor(m_color);
	m_lineSprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	updateChildAlpha();
}
INLINE XBool XSlider::canGetFocus(float x,float y)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
INLINE void XSlider::setLostFocus() 
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出
	if(m_curSliderState != SLIDER_STATE_DISABLE) m_curSliderState = SLIDER_STATE_NORMAL;
	m_isBeChoose = XFalse;
}
INLINE XBool XSlider::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XSlider::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
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
INLINE void XSlider::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_withoutTex && !m_lightMD.getIsEnd())
	{
		XRender::drawRect(m_lightRect,1.0f * m_lightMD.getCurData() * 2.0f,1.0f,1.0f,1.0f,(1.0f - m_lightMD.getCurTimer()) * 0.5f);
	}
}
INLINE void XSlider::setOprateState(void * data)
{
	float index = *(float *)data;
	setCurValue(index);
}
INLINE void *XSlider::getOprateState() const
{
	float *data = XMem::createMem<float>();
	*data = getCurValue();
	return data;
}
INLINE void XSlider::releaseOprateStateData(void *p)
{
	float *data = (float *)p;
	XMem::XDELETE(data);
}
INLINE bool XSlider::isSameState(void * data)
{
	if(data == NULL) return false;
	return(*(float*)data == getCurValue());
}