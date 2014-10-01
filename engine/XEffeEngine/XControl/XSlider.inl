inline void _XSliderTexture::releaseTex()
{
	XDELETE(sliderLineNormal);
	XDELETE(sliderLineDown);
	XDELETE(sliderLineOn);
	XDELETE(sliderLineDisable);
	XDELETE(sliderButtonNormal);
	XDELETE(sliderButtonDown);
	XDELETE(sliderButtonOn);
	XDELETE(sliderButtonDisable);
}
inline void _XSlider::setDataChangeCB(void (*funValueChange)(void *,int),
		void (*funMouseMove)(void *,int),
		void *pClass)
{
	m_funValueChange = funValueChange;
	m_funMouseMove = funMouseMove;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline void _XSliderTexture::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
inline void _XSlider::disable()//使控件无效
{
	m_nowSliderState = SLIDER_STATE_DISABLE;
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
inline void _XSlider::enable()//使控件有效
{
	if(m_nowSliderState == SLIDER_STATE_DISABLE)
	{
		m_nowSliderState = SLIDER_STATE_NORMAL;
		//还需要调用鼠标函数
		mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	}
	m_isEnable = XTrue;
}
inline float _XSlider::getNowValue() const	//获取滑块当前的值
{
	return m_nowValue;
}
inline float _XSlider::getMaxValue() const
{
	return m_maxValue;
}
inline float _XSlider::getMinValue() const
{
	return m_minValue;
}
inline _XBool _XSlider::initEx(const _XVector2& position,	//对上面接口的简化
	const _XSliderTexture &tex,float max,float min,_XSliderType type)
{
	return init(position,tex.m_mouseRect,tex.m_mouseRectButton,tex,max,min,type);
}
inline _XBool _XSlider::initPlus(const char * path,float max,float min,_XSliderType type,
	_XResourcePosition resoursePosition)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XSLIDER_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(_XVector2::zero,*(_XSliderTexture *)m_resInfo->m_pointer,max,min,type);
}
inline void _XSlider::setColor(float r,float g,float b,float a)
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);
	m_buttonSprite.setColor(m_color);
	m_lineSprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	updateChildColor();
}
inline void _XSlider::setTextColor(const _XFColor& color)
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
inline void _XSlider::setAlpha(float a)
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_buttonSprite.setColor(m_color);
	m_lineSprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	updateChildAlpha();
}
inline _XBool _XSlider::canGetFocus(float x,float y)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
inline void _XSlider::setLostFocus() 
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出
	if(m_nowSliderState != SLIDER_STATE_DISABLE) m_nowSliderState = SLIDER_STATE_NORMAL;
	m_isBeChoose = XFalse;
}
inline _XBool _XSlider::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XSlider::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0: return _XVector2(m_nowMouseRect.left,m_nowMouseRect.top);
	case 1: return _XVector2(m_nowMouseRect.right,m_nowMouseRect.top);
	case 2: return _XVector2(m_nowMouseRect.right,m_nowMouseRect.bottom);
	case 3: return _XVector2(m_nowMouseRect.left,m_nowMouseRect.bottom);
	}
	return _XVector2::zero;
}
inline void _XSlider::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_withoutTex && !m_lightMD.getIsEnd())
	{
		drawRect(m_lightRect,1.0f * m_lightMD.getNowData() * 2.0f,1.0f,1.0f,1.0f,(1.0f - m_lightMD.getNowTimer()) * 0.5f);
	}
}
inline void _XSlider::setOprateState(void * data)
{
	float index = *(float *)data;
	setNowValue(index);
}
inline void *_XSlider::getOprateState() const
{
	float *data = createMem<float>();
	*data = getNowValue();
	return data;
}
inline bool _XSlider::isSameState(void * data)
{
	if(data == NULL) return false;
	return(*(float*)data == getNowValue());
}