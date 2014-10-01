inline void _XMultiText::enable()		//使其有效
{
	m_isEnable = XTrue;
	m_verticalSlider.enable();
	m_horizontalSlider.enable();
}
inline void _XMultiText::disable()		//使其无效
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
	m_verticalSlider.disable();
	m_horizontalSlider.disable();
}
inline int _XMultiText::getSelectLine() const			//返回当前鼠标选择的是哪一行，-1位没有选择
{
	if(!m_isInited ||
		!m_haveSelect) return -1;
	else return m_selectLineOrder;
}
inline int _XMultiText::getLineSum() const
{
	return m_nowMaxLineSum;
}
inline _XBool _XMultiText::initEx(const _XVector2& position,	//对上面接口的简化
	const _XMultiTextTexture& tex,	
	const char *str,const _XFontUnicode& font, float strSize,
	const _XSlider &vSlider,const _XSlider &hSlider)
{
	return init(position,tex.m_mouseRect,tex,str,font,strSize,vSlider,hSlider);
}
inline void _XMultiText::setSelectCallBackFun(void (* funSelect)(void *,int),void *pClass)
{
	m_funSelect = funSelect;
	m_pClass = pClass;
}
inline _XBool _XMultiText::canGetFocus(float x,float y)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
inline void _XMultiText::setTextColor(const _XFColor& color) 
{
	if(!m_isInited) return;
	m_textColor = color;
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].nowText.setColor(m_textColor);
	}
}	//设置字体的颜色
inline void _XMultiText::setColor(float r,float g,float b,float a) 
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);
	m_spriteBackGround.setColor(m_color);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].nowText.setColor(m_textColor * m_color);
	}
	m_verticalSlider.setColor(m_color);
	m_horizontalSlider.setColor(m_color);
}	//设置按钮的颜色
inline void _XMultiText::setAlpha(float a) 
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_spriteBackGround.setColor(m_color);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].nowText.setColor(m_textColor * m_color);
	}
	m_verticalSlider.setColor(m_color);
	m_horizontalSlider.setColor(m_color);
}
inline void _XMultiText::addString(const char *str)			//向字符串中添加字符串
{
	if(!m_isInited ||		//如果没有初始化直接退出
		str == NULL ||
		strlen(str) == 0 ||
		strlen(str) + strlen(m_nowString) >= MAX_STRING_LENGTH) return;
	strcat(m_nowString,str);
	checkStr();
	updateLineStr();
}
inline _XBool _XMultiText::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	//return m_nowMouseRect.isInRect(x,y);
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
inline _XVector2 _XMultiText::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0: return _XVector2(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y);
	case 1: return _XVector2(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.top * m_size.y);
	case 2: return _XVector2(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	case 3: return _XVector2(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y); 
	}
	return _XVector2::zero;
}
inline _XBool _XMultiText::canLostFocus(float x,float y) 
{
	if(m_needShowVSlider && !m_verticalSlider.canLostFocus(x,y)) return XFalse;
	if(m_needShowHSlider && !m_horizontalSlider.canLostFocus(x,y)) return XFalse;
	return XTrue;
}
inline void _XMultiText::setLostFocus() 
{
	m_verticalSlider.setLostFocus();
	m_horizontalSlider.setLostFocus();
	m_isBeChoose = XFalse;
}
inline void _XMultiText::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	m_verticalSlider.drawUp();
	m_horizontalSlider.drawUp();
}
inline void _XMultiText::update(int stepTime)
{
	m_verticalSlider.update(stepTime);
	m_horizontalSlider.update(stepTime);
	if(!m_actionMD.getIsEnd())
	{
		m_actionMD.move(stepTime);
	}
}
inline void _XMultiText::setAction(_XMultiTextActionType type,int index)
{
	switch(type)
	{
	case MLTTXT_ACTION_TYPE_IN:		//选项出现
		m_actionMD.set(0.0f,1.0f,0.005f,MOVE_DATA_MODE_SHAKE);
		break;
	case MLTTXT_ACTION_TYPE_MOVE:	//选项移动
		m_actionMD.set(0.0f,1.0f,0.005f);
		break;
	case MLTTXT_ACTION_TYPE_DCLICK:	//双击
		m_actionMD.set(1.0f,1.2f,0.01f,MOVE_DATA_MODE_SIN_MULT,1);
		break;
	case MLTTXT_ACTION_TYPE_OUT:		//取消选择
		m_actionMD.set(1.0f,0.0f,0.005f);
		break;
	}
	m_actionType = type;
	m_actionPosition = index;
}