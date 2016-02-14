//INLINE void XRadios::setCallbackFun(void (* funStateChange)(void *,int),void *pClass)
//{
//	m_funStateChange = funStateChange;
//	if(pClass == NULL) m_pClass = this;
//	else m_pClass = pClass;
//}
INLINE int XRadios::getCurChoose() const	//返回当前单选项中选择的项的编号
{
	return m_curChoose;
}
INLINE void XRadios::setRadioText(const char *temp,int order)	//设置单选项中某一项的文字
{
	if(!m_isInited ||	//如果没有初始化直接退出
		temp == NULL ||
		order < 0 || order >= m_radioSum) return;
	m_radio[order].setCaptionText(temp);
}
INLINE void XRadios::setRadioState(bool state,int order)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		order < 0 || order >= m_radioSum) return;
	if(state) m_radio[order].enable();
	else m_radio[order].disable();
}
INLINE void XRadios::setRadioPosition(const XVector2& position,int order)
{
	setRadioPosition(position.x,position.y,order);
}
INLINE XBool XRadios::initEx(int radioSum,			//对上面接口的简化
	const XVector2& distance,	
	const XVector2& position,	
	const XRadiosSkin* tex,const XFontUnicode &font,float captionSize)
{
	return init(radioSum,distance,position,tex->m_mouseRect,tex,font,captionSize,tex->m_fontPosition);
}
INLINE XBool XRadios::initPlus(int radioSum,			//选项的数量
	const XVector2& distance,	//每个单选项之间的距离
	const char *path,const XFontUnicode &font,float captionSize,
	XResourcePosition resoursePosition)
{
	if(m_isInited ||
		path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXCHECK_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(radioSum,distance,XVector2::zero,(XRadiosSkin *)m_resInfo->m_pointer,font,captionSize);
}
INLINE XBool XRadios::canGetFocus(float x,float y)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
INLINE void XRadios::disable()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isEnable) return;
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].disable();
	}
}
INLINE void XRadios::enable()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		m_isEnable) return;
	m_isEnable = XTrue;
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].enable();
	}
}
INLINE void XRadios::setRadioPosition(float x,float y,int order)			//设置单选项中某一项的位置(这里使用相对坐标)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		order < 0 || order >= m_radioSum) return;
	m_checkPosition[order].set(x,y);
	m_radio[order].setPosition(m_position.x + x * m_scale.x,m_position.y + y * m_scale.y);
}
INLINE void XRadios::setDistance(const XVector2& distance)
{
	if(!m_isInited) return;
	m_distance = distance;
	//更新数据
	for(int i = 0;i < m_radioSum;++ i)
	{
	//	m_checkPosition[i].set(m_distance.x * i,m_distance.y * i);
	//	m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_scale.x,m_position.y + m_checkPosition[i].y * m_scale.y);
		m_checkPosition[i].set(m_distance * i);
		m_radio[i].setPosition(m_position + m_checkPosition[i] * m_scale);
	}
}
INLINE void XRadios::setTextColor(const XFColor& color) 
{
	if(!m_isInited) return;
	m_textColor = color;
	XFColor tmpColor = m_textColor * m_color;
	m_caption.setColor(tmpColor);
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setTextColor(tmpColor);
	}
}	//设置字体的颜色
INLINE void XRadios::setColor(float r,float g,float b,float a) 
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setColor(m_color);
	}
}	//设置按钮的颜色
INLINE void XRadios::setAlpha(float a) 
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setColor(m_color);
	}
}	
INLINE XBool XRadios::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE void XRadios::draw()
{	
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].draw();
	}
}
INLINE void XRadios::drawUp()
{	
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].drawUp();
	}
}
INLINE void XRadios::update(float stepTime)
{
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].update(stepTime);
	}
}