inline void _XComboTexture::releaseTex()
{
	XDELETE(comboInputNormal);
	XDELETE(comboInputDisable);
	
	downButtonTex.release();
	downMenuUpButtonTex.release();
	downMenuButtonTex.release();
	downMenuDownButtonTex.release();
}
inline void _XComboTexture::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
inline _XBool _XCombo::initEx(const _XComboTexture &tex,	//上面接口的简化版本
		const _XVector2& position,		
		int menuSum,					
		int drawMenuSum,				
		const _XFontUnicode &font,		
		float fontSize)
{
	return init(tex,position,tex.m_mouseRect,tex.downButtonTex.m_mouseRect,
		tex.downMenuUpButtonTex.m_mouseRect,tex.downMenuButtonTex.m_mouseRect,
		tex.downMenuDownButtonTex.m_mouseRect,menuSum,drawMenuSum,font,fontSize);
}
inline _XBool _XCombo::initPlus(const char * path,
		int menuSum,					//下拉菜单中的选项的数量
		int drawMenuSum,				//下拉菜单中显示的菜单项的数量
		const _XFontUnicode &font,		//显示文字使用的字体
		float fontSize,
		_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;				//防止重复初始化
	if(path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XCOMBO_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(*(_XComboTexture *)m_resInfo->m_pointer,_XVector2::zero,menuSum,drawMenuSum,font,fontSize);
}
inline _XBool _XCombo::canGetFocus(float x,float y)	//用于判断当前物件是否可以获得焦点
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
inline _XBool _XCombo::canLostFocus(float,float)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XTrue;		//如果无效则直接退出

	if(m_isDrawDownMenu) return XFalse;
	return XTrue;
}
inline void _XCombo::setColor(float r,float g,float b,float a) 	//设置按钮的颜色
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setColor(m_color);
	}
}	//设置按钮的颜色
inline void _XCombo::setAlpha(float a) 
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setColor(m_color);
	}
}
inline _XBool _XCombo::isInRect(float x,float y)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	if(!m_isInited) return XFalse;
	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
inline void _XCombo::setTextColor(const _XFColor& color) 
{
	if(!m_isInited) return;
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setTextColor(m_textColor);
	}
}	//设置字体的颜色
inline _XBool _XCombo::setNowChooseOrder(int index)	//设置当前选项编号
{
	if(index < 0 || index >= m_menuSum) return XFalse;
	if(m_nowChooseOrder == index) return XTrue;
	m_nowChooseOrder = index;
	updateString();
	return XTrue;
}
inline void _XCombo::setCallbackFun(void (* funDataChange)(void *,int),void *pClass)
{
	if(funDataChange != NULL) m_funDataChange = funDataChange;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline void _XCombo::disable()					//使无效
{
	m_isEnable = XFalse;
//	m_isBeChoose = XFalse;
}
inline void _XCombo::enable()					//使能
{
	m_isEnable = XTrue;
}
inline void _XCombo::update(int stepTime)
{
	m_comment.update(stepTime);
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].update(stepTime);
	}
	if(m_isInAction)
	{
		m_actionMoveData.move(stepTime);
		float action;
		if(m_actionMoveData.getIsEnd()) 
		{
			m_isInAction = false;
			for(int i = 0;i < m_menuDrawSum + 2;++ i)
			{
				m_buttom[1 + i].setWithAction(XTrue);
			}
			action = 1.0f;	//这里需要恢复
			if(!m_isDraw)
			{
				for(int i = 0;i < m_menuDrawSum + 2;++ i)
				{
					m_buttom[1 + i].disVisible();
				}
			}
		}else
		{
			action = m_actionMoveData.getNowData();
		}
		m_buttom[1].setAlpha(action * m_color.fA);
		//m_buttom[1].setPosition(m_position.x,m_position.y + (m_inputArea.bottom - m_downMenuUpArea.top) * m_size.y * action);
		m_buttom[2].setAlpha(action * m_color.fA);
		m_buttom[2].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
			+ m_downMenuArea.getHeight() * m_menuDrawSum * action - m_downMenuDownArea.top) * m_size.y);

		for(int i = 0;i < m_menuDrawSum;++ i)
		{
			m_buttom[3 + i].setAlpha(action * m_color.fA);
			m_buttom[3 + i].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
				+ m_downMenuArea.getHeight() * i * action - m_downMenuArea.top) * m_size.y);
		}
	}
}