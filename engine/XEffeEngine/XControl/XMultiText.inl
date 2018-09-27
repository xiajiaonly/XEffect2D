INLINE void XMultiText::enable()		//使其有效
{
	m_isEnable = XTrue;
	m_verticalSlider.enable();
	m_horizontalSlider.enable();
}
INLINE void XMultiText::disable()		//使其无效
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
	m_verticalSlider.disable();
	m_horizontalSlider.disable();
}
INLINE int XMultiText::getSelectLine() const			//返回当前鼠标选择的是哪一行，-1位没有选择
{
	if(!m_isInited ||
		!m_haveSelect) return -1;
	else return m_selectLineOrder;
}
INLINE int XMultiText::getLineSum() const
{
	return m_curMaxLineSum;
}
INLINE XBool XMultiText::initEx(const XVec2& position,	//对上面接口的简化
	const XMultiTextSkin& tex,	
	const char *str,const XFontUnicode& font, float strSize,
	const XSlider &vSlider,const XSlider &hSlider)
{
	return init(position,tex.m_mouseRect,tex,str,font,strSize,vSlider,hSlider);
}
//INLINE void XMultiText::setSelectCallBackFun(void (* funSelect)(void *,int),void *pClass)
//{
//	m_funSelect = funSelect;
//	m_pClass = pClass;
//}
INLINE XBool XMultiText::canGetFocus(const XVec2& p)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(p);
}
INLINE void XMultiText::setTextColor(const XFColor& color) 
{
	if(!m_isInited) return;
	m_textColor = color;
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].curText.setColor(m_textColor);
	}
}	//设置字体的颜色
INLINE void XMultiText::setColor(const XFColor& c)
{
	if(!m_isInited) return;
	m_color = c;
	m_spriteBackGround.setColor(m_color);
	XFColor tmpColor = m_textColor * m_color;
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].curText.setColor(tmpColor);
	}
	m_verticalSlider.setColor(m_color);
	m_horizontalSlider.setColor(m_color);
}	//设置按钮的颜色
INLINE void XMultiText::setAlpha(float a) 
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_spriteBackGround.setColor(m_color);
	XFColor tmpColor = m_textColor * m_color;
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].curText.setColor(tmpColor);
	}
	m_verticalSlider.setColor(m_color);
	m_horizontalSlider.setColor(m_color);
}
INLINE XBool XMultiText::isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	//return m_curMouseRect.isInRect(x,y);
	return XMath::getIsInRect(p,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE XVec2 XMultiText::getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
{
	if(!m_isInited) return XVec2::zero;
	switch(order)
	{
	case 0: return m_position + m_mouseRect.getLT() * m_scale;
	case 1: return m_position + m_mouseRect.getRT() * m_scale;
	case 2: return m_position + m_mouseRect.getRB() * m_scale;
	case 3: return m_position + m_mouseRect.getLB() * m_scale; 
	}
	return XVec2::zero;
}
INLINE XBool XMultiText::canLostFocus(const XVec2& p)
{
	if(m_needShowVSlider && !m_verticalSlider.canLostFocus(p)) return XFalse;
	if(m_needShowHSlider && !m_horizontalSlider.canLostFocus(p)) return XFalse;
	return XTrue;
}
INLINE void XMultiText::setLostFocus() 
{
	m_verticalSlider.setLostFocus();
	m_horizontalSlider.setLostFocus();
	m_isBeChoose = XFalse;
}
INLINE void XMultiText::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	m_verticalSlider.drawUp();
	m_horizontalSlider.drawUp();
}
INLINE void XMultiText::update(float stepTime)
{
	m_verticalSlider.update(stepTime);
	m_horizontalSlider.update(stepTime);
	if(!m_actionMD.getIsEnd())
	{
		m_actionMD.move(stepTime);
	}
}
INLINE void XMultiText::setAction(XMultiTextActionType type,int index)
{
	switch(type)
	{
	case MLTTXT_ACTION_TYPE_IN:		//选项出现
		m_actionMD.set(0.0f,1.0f,0.005f,MD_MODE_SHAKE);
		break;
	case MLTTXT_ACTION_TYPE_MOVE:	//选项移动
		m_actionMD.set(0.0f,1.0f,0.005f);
		break;
	case MLTTXT_ACTION_TYPE_DCLICK:	//双击
		m_actionMD.set(1.0f,1.2f,0.01f,MD_MODE_SIN_MULT,1);
		break;
	case MLTTXT_ACTION_TYPE_OUT:		//取消选择
		m_actionMD.set(1.0f,0.0f,0.005f);
		break;
	}
	m_actionType = type;
	m_actionPosition = index;
}
INLINE XBool XMultiText::moveUpLine(int order)	//将某一行上移
{
	if (!m_isInited ||		//如果没有初始化直接退出
		!m_haveSelect) return XFalse;
	if (order <= 0 || order >= m_curMaxLineSum) return XFalse;	//非法的值则退出
	auto ot = m_curStr.begin() + order;
	std::swap(*ot, *(ot - 1));
	updateLineStr();
	return XTrue;
}
INLINE XBool XMultiText::moveDownLine(int order)	//将某一行下移
{
	if (!m_isInited ||		//如果没有初始化直接退出
		!m_haveSelect) return XFalse;
	if (order < 0 || order >= m_curMaxLineSum - 1) return XFalse;	//非法的值则退出
	auto ot = m_curStr.begin() + order;
	std::swap(*ot, *(ot + 1));
	updateLineStr();
	return XTrue;
}
