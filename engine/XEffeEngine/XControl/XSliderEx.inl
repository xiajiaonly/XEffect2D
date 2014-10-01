inline void _XSliderEx::disable()//使控件无效
{
	m_mainSld.disable();
	m_secondarySld.disable();
	m_chooseBtn.disable();
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
inline void _XSliderEx::enable()//使控件有效
{
	m_mainSld.enable();
	m_secondarySld.enable();
	m_chooseBtn.enable();
	m_isEnable = XTrue;
}
inline float _XSliderEx::getNowValue() const	//获取滑块当前的值
{
	return m_mainSld.getNowValue();
}
inline float _XSliderEx::getMaxValue() const
{
	return m_mainSld.getMaxValue();
}
inline float _XSliderEx::getMinValue() const
{
	return m_mainSld.getMinValue();
}
inline void _XSliderEx::setCallbackFun(void (* funInit)(void *,int),
	void (* funRelease)(void *,int),
	void (* funMouseOn)(void *,int),
	void (* funMouseDown)(void *,int),
	void (* funMouseUp)(void *,int),
	void (* funValueChange)(void *,int),
	void (* funMouseMove)(void *,int),
	void *pClass)
{
	m_funInit = funInit;
	m_funRelease = funRelease;
	m_funMouseOn = funMouseOn;
	m_funMouseDown = funMouseDown;		//有效
	m_funMouseUp = funMouseUp;
	m_funValueChange = funValueChange;
	m_funMouseMove = funMouseMove;
	m_pClass = pClass;
}
inline void _XSliderEx::setNowValue(float temp)
{
	m_mainSld.setNowValue(temp);
}
inline void _XSliderEx::setRange(float max,float min)
{
	m_mainSld.setRange(max,min);
}
inline _XBool _XSliderEx::isInRect(float x,float y) //点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
inline _XVector2 _XSliderEx::getBox(int order)		//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return _XVector2::zero;
	return m_mainSld.getBox(order);
}
inline void _XSliderEx::setColor(float r,float g,float b,float a)
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);
	m_mainSld.setColor(m_color);
	m_secondarySld.setColor(m_color);
	m_chooseBtn.setColor(m_color);
	updateChildColor();
}//设置字体的颜色
inline void _XSliderEx::setAlpha(float a)
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_mainSld.setColor(m_color);
	m_secondarySld.setColor(m_color);
	m_chooseBtn.setColor(m_color);
	updateChildAlpha();
}
//下面是一些公共方法
inline _XBool _XSliderEx::setFont(const char *caption,const _XFontUnicode &font,float captionSize,const _XVector2 &fontPosition)
{
	return m_mainSld.setFont(caption,font,captionSize,fontPosition);
}
inline _XBool _XSliderEx::setFontEx(const char *caption,const _XFontUnicode &font,float captionSize)
{
	return m_mainSld.setFontEx(caption,font,captionSize);
}
inline _XBool _XSliderEx::keyboardProc(int keyOrder,_XKeyState keyState)
{
	return m_mainSld.keyboardProc(keyOrder,keyState);
}
//inline void _XSliderEx::setLostFocus() 
//{
//	if(!m_isInited ||	//如果没有初始化直接退出
//		!m_isActive ||		//没有激活的控件不接收控制
//		!m_isVisible ||	//如果不可见直接退出
//		!m_isEnable) return;		//如果无效则直接退出
//
//	//m_mainSld.setLostFocus();
//	//m_secondarySld.setLostFocus();
//	//m_chooseBtn.setLostFocus();
//	m_isBeChoose = XFalse;
//}
inline void _XSliderEx::setVisible()
{
	m_isVisible = XTrue;
	m_mainSld.setVisible();
	//m_secondarySld.setVisible();
	//m_chooseBtn.setVisible();
	updateChildVisible();
}
inline void _XSliderEx::disVisible()
{
	m_isVisible = XFalse;
	m_mainSld.disVisible();
	m_secondarySld.disVisible();
	m_chooseBtn.disVisible();
	updateChildVisible();
}
inline void _XSliderEx::draw()//描绘滑动条
{
	if(!m_isInited || //如果没有初始化直接退出
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return;		//如果无效则直接退出

	m_mainSld.draw();
	m_secondarySld.draw();
	m_chooseBtn.draw();
}
inline _XBool _XSliderEx::mouseProc(float x,float y,_XMouseState mouseState)	//对于鼠标动作的响应函数
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出

	m_mainSld.mouseProc(x,y,mouseState);
	m_secondarySld.mouseProc(x,y,mouseState);
	m_chooseBtn.mouseProc(x,y,mouseState);
	if(mouseState == MOUSE_LEFT_BUTTON_UP && m_mainSld.isInRect(x,y))
	{
		m_isBeChoose = XTrue;
	}
	return XTrue;
}
inline _XBool _XSliderEx::canGetFocus(float x,float y)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
inline void _XSliderEx::setOprateState(void * data)
{
	float index = *(float *)data;
	setNowValue(index);
}
inline void *_XSliderEx::getOprateState() const
{
	float *data = createMem<float>();
	*data = getNowValue();
	return data;
}
inline bool _XSliderEx::isSameState(void * data)
{
	if(data == NULL) return false;
	return(*(float*)data == getNowValue());
}