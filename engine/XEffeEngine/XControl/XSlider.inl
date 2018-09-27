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
		mouseProc(m_upMousePoint,MOUSE_MOVE);
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
INLINE XBool XSlider::initEx(const XVec2& position,	//对上面接口的简化
	const XSliderSkin &tex,float max,float min,XSliderType type)
{
	return init(position,tex.m_mouseRect,tex.m_mouseRectButton,tex,max,min,type);
}
INLINE XBool XSlider::initPlus(const char * path,float max,float min,XSliderType type,
	XResPos resPos)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXSLIDER_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVec2::zero,*(XSliderSkin *)m_resInfo->m_pointer,max,min,type);
}
INLINE void XSlider::setColor(const XFColor& c)
{
	if(!m_isInited) return;
	m_color = c;
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
INLINE XBool XSlider::canGetFocus(const XVec2& p)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(p);
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
INLINE XBool XSlider::isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XSlider::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return XVec2::zero;
	switch(order)
	{
	case 0: return m_curMouseRect.getLT();
	case 1: return m_curMouseRect.getRT();
	case 2: return m_curMouseRect.getRB();
	case 3: return m_curMouseRect.getLB();
	}
	return XVec2::zero;
}
INLINE void XSlider::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_withoutTex && !m_lightMD.getIsEnd())
	{
		XRender::drawRect(m_lightRect,1.0f * m_lightMD.getCurData() * 2.0f,XFColor(1.0f,(1.0f - m_lightMD.getCurTimer()) * 0.5f));
	}
	m_comment.draw();
}
INLINE void XSlider::setOprateState(void * data)
{
	setCurValue(*(float *)data,true);
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