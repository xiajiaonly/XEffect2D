#include "XTab.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

void tabBtnProc(void *pClass,int id)
{
	_XTab &pPar = *(_XTab *)pClass;
	if(id == pPar.m_leftBtn.getControlID())
	{
		if(pPar.m_nowStartTabIndex > 0)
		{
			-- pPar.m_nowStartTabIndex;
			pPar.updateTitle();
			pPar.updateBtnState();
		}
	}else
	if(id == pPar.m_rightBtn.getControlID())
	{
		if(pPar.m_nowStartTabIndex < pPar.m_tabObjects.size() - 1)
		{
			++ pPar.m_nowStartTabIndex;
			pPar.updateTitle();
			pPar.updateBtnState();
		}
	}
}
_XBool _XTab::initWithoutTex(const _XRect &rect,const _XFontUnicode &font)
{
	if(m_isInited) return XFalse;
	if(rect.getWidth() <= 0 || rect.getHeight() <= 0) return XFalse;
	//检查数据的合法性
	m_mouseRect = rect;
	m_position.set(0.0f,0.0f);
	m_size.set(1.0f,1.0f);
	m_nowMouseRect.set(m_mouseRect.left * m_size.x + m_position.x,
		m_mouseRect.top * m_size.y + m_position.y,
		m_mouseRect.right * m_size.x + m_position.x,
		m_mouseRect.bottom * m_size.y + m_position.y);
	m_titleRect.set(m_nowMouseRect.left,m_nowMouseRect.top,m_nowMouseRect.right,m_nowMouseRect.top + TAB_TITLE_HEIGHT * m_size.y);
	m_texFont.setACopy(font);
	m_texFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_texFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_texFont.setColor(0.0f,0.0f,0.0f,1.0f);
	m_texFont.setSize(m_size);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_texFont);
#endif
	m_nowTextWidth = m_texFont.getTextSize().x * m_texFont.getSize().x * 0.5f;
	m_maxTitleLen = (m_mouseRect.getWidth() - (TAB_BTN_WIDTH << 1) - TAB_TITLE_DISTANCE) / m_nowTextWidth;	//计算标题的最大长度
	if(m_maxTitleLen < 3) return false;	//窗口太小

	m_leftBtn.initWithoutTex("<",font,_XRect(0.0f,0.0f,TAB_BTN_WIDTH,32.0f));
	m_leftBtn.setPosition(m_nowMouseRect.right - ((TAB_BTN_WIDTH << 1) * m_size.x),m_nowMouseRect.top);
	m_leftBtn.disable();
	m_leftBtn.setMouseDownCB(tabBtnProc,this);
	m_leftBtn.setSize(m_size);
	_XCtrlManger.decreaseAObject(&m_leftBtn);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_leftBtn);
#endif
	m_rightBtn.initWithoutTex(">",font,_XRect(0.0f,0.0f,TAB_BTN_WIDTH,32.0f));
	m_rightBtn.setPosition(m_nowMouseRect.right - TAB_BTN_WIDTH * m_size.x,m_nowMouseRect.top);
	m_rightBtn.disable();
	m_rightBtn.setMouseDownCB(tabBtnProc,this);
	m_rightBtn.setSize(m_size);
	_XCtrlManger.decreaseAObject(&m_rightBtn);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_rightBtn);
#endif

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	m_withoutTex = XTrue;
	m_isInited = XTrue;

	_XCtrlManger.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif

	return XTrue;
}
void _XTab::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_withoutTex)
	{//不使用贴图时
		//描绘标签的背景
		drawFillBox(m_nowMouseRect.left,m_nowMouseRect.top,
			m_nowMouseRect.getWidth(),TAB_TITLE_HEIGHT * m_size.y,
			0.2f * m_color.fR,0.2f * m_color.fG,0.2f * m_color.fB,m_color.fA);
		//下面显示标签
		if(m_withAction && m_isInAction)
		{
			for(int i = m_nowStartTabIndex;i < m_tabObjects.size();++ i)
			{//显示标签背景
				if(m_tabObjects[i].needDraw)
				{
					drawFillBoxExAS(_XVector2(m_nowMouseRect.left + m_tabObjects[i].offset,m_nowMouseRect.top),
						_XVector2(m_tabObjects[i].len,TAB_TITLE_HEIGHT * m_size.y),
						0.35f * m_color.fR,0.35f * m_color.fG,0.35f * m_color.fB,m_color.fA);
				}
			}
		}else
		{
			for(int i = m_nowStartTabIndex;i < m_tabObjects.size();++ i)
			{//显示标签背景
				if(m_tabObjects[i].needDraw && m_nowChooseTabIndex != i)
				{
					if(m_tabObjects[i].isEnable)
					{//没有被选中的标签
						drawFillBoxExAS(_XVector2(m_nowMouseRect.left + m_tabObjects[i].offset,m_nowMouseRect.top),
							_XVector2(m_tabObjects[i].len,TAB_TITLE_HEIGHT * m_size.y),
							0.35f * m_color.fR,0.35f * m_color.fG,0.35f * m_color.fB,m_color.fA);
						m_tabObjects[i].pFont->setColor(0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
						//m_texFont.setColor(0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
					}else
					{
						drawFillBoxExAS(_XVector2(m_nowMouseRect.left + m_tabObjects[i].offset,m_nowMouseRect.top),
							_XVector2(m_tabObjects[i].len,TAB_TITLE_HEIGHT * m_size.y),
							0.25f * m_color.fR,0.25f * m_color.fG,0.25f * m_color.fB,m_color.fA);
						m_tabObjects[i].pFont->setColor(0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
						//m_texFont.setColor(0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
					}
				}
			}
			for(int i = m_nowStartTabIndex;i < m_tabObjects.size();++ i)
			{//显示标签文字
				if(m_tabObjects[i].needDraw && m_nowChooseTabIndex != i)
				{
				//	m_tabObjects[i].pFont->setPosition(m_nowMouseRect.left + m_tabObjects[i].offset,m_nowMouseRect.top);
					m_tabObjects[i].pFont->draw();
				//	m_texFont.setString(m_tabObjects[i].title.c_str());
				//	m_texFont.setPosition(m_nowMouseRect.left + m_tabObjects[i].offset,m_nowMouseRect.top);
				//	m_texFont.draw();
				}
			}
		}
		//描绘下面的面板
		drawFillBoxExAS(_XVector2(m_nowMouseRect.left,m_nowMouseRect.top + TAB_TITLE_HEIGHT * m_size.y),
			_XVector2(m_nowMouseRect.getWidth(),m_nowMouseRect.getHeight() - TAB_TITLE_HEIGHT * m_size.y),
			0.35f * m_color.fR,0.35f * m_color.fG,0.35f * m_color.fB,m_color.fA,true);
		//描绘被选中的项
		if(m_withAction && m_isInAction)
		{
			//描绘动态，注意这里没有考虑源出于不显示状态的情况
			if(m_tabObjects[m_oldChooseTabIndex].needDraw)
			{
				drawFillBoxExExA(_XVector2(m_nowMouseRect.left + 
					lineSlerp<float>(m_tabObjects[m_oldChooseTabIndex].offset,m_tabObjects[m_nowChooseTabIndex].offset,m_actionMoveData.getNowData()),m_nowMouseRect.top),
					_XVector2(lineSlerp<float>(m_tabObjects[m_oldChooseTabIndex].len,m_tabObjects[m_nowChooseTabIndex].len,m_actionMoveData.getNowData()),TAB_TITLE_HEIGHT * m_size.y),
					0.35f * m_color.fR,0.35f * m_color.fG,0.35f * m_color.fB,m_color.fA,0x0b,0x00,true);
			}else
			{
				if(m_oldChooseTabIndex < m_nowChooseTabIndex)
				{
					float pos = m_nowMouseRect.left - m_tabObjects[m_oldChooseTabIndex].len;
					pos = lineSlerp<float>(pos,m_tabObjects[m_nowChooseTabIndex].offset,m_actionMoveData.getNowData());
					float len = lineSlerp<float>(m_tabObjects[m_oldChooseTabIndex].len,m_tabObjects[m_nowChooseTabIndex].len,m_actionMoveData.getNowData());
					if(pos <= 0)
					{
						len += pos;
						pos = 0;
					}
					drawFillBoxExExA(_XVector2(m_nowMouseRect.left + pos,m_nowMouseRect.top),
						_XVector2(len,TAB_TITLE_HEIGHT * m_size.y),
						0.35f * m_color.fR,0.35f * m_color.fG,0.35f * m_color.fB,m_color.fA,0x0b,0x00,true);
				}else
				{
					float pos = m_nowMouseRect.right;
					pos = lineSlerp<float>(pos,m_tabObjects[m_nowChooseTabIndex].offset,m_actionMoveData.getNowData());
					float len = lineSlerp<float>(m_tabObjects[m_oldChooseTabIndex].len,m_tabObjects[m_nowChooseTabIndex].len,m_actionMoveData.getNowData());
					if(pos + len > m_nowMouseRect.right)
					{
						len -= m_nowMouseRect.right - pos;
					}
					drawFillBoxExExA(_XVector2(m_nowMouseRect.left + pos,m_nowMouseRect.top),
						_XVector2(len,TAB_TITLE_HEIGHT * m_size.y),
						0.35f * m_color.fR,0.35f * m_color.fG,0.35f * m_color.fB,m_color.fA,0x0b,0x00,true);
				}
			}
			for(int i = m_nowStartTabIndex;i < m_tabObjects.size();++ i)
			{//显示标签文字
				if(m_tabObjects[i].needDraw)
				{
					if(m_nowChooseTabIndex != i)
					{
						if(m_oldChooseTabIndex == i)
						{
							float temp = m_actionMoveData.getNowData();
							m_tabObjects[i].pFont->setColor(0.5f * m_color.fR * temp,0.5f * m_color.fG * temp,0.5f * m_color.fB * temp,m_color.fA);
							//m_texFont.setColor(0.5f * m_color.fR * temp,0.5f * m_color.fG * temp,0.5f * m_color.fB * temp,m_color.fA);
						}else
						{
							m_tabObjects[i].pFont->setColor(0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
							//m_texFont.setColor(0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
						}
					}else
					{
						float temp = 1.0f - m_actionMoveData.getNowData();
						m_tabObjects[i].pFont->setColor(0.5f * m_color.fR * temp,0.5f * m_color.fG * temp,0.5f * m_color.fB * temp,m_color.fA);
						//m_texFont.setColor(0.5f * m_color.fR * temp,0.5f * m_color.fG * temp,0.5f * m_color.fB * temp,m_color.fA);
					}
				//	m_tabObjects[i].pFont->setPosition(m_nowMouseRect.left + m_tabObjects[i].offset,m_nowMouseRect.top);
					m_tabObjects[i].pFont->draw();
				//	m_texFont.setString(m_tabObjects[i].title.c_str());
				//	m_texFont.setPosition(m_nowMouseRect.left + m_tabObjects[i].offset,m_nowMouseRect.top);
				//	m_texFont.draw();
				}
			}
		}else
		{
			if(m_nowChooseTabIndex >= m_nowStartTabIndex && m_nowChooseTabIndex < m_tabObjects.size() &&
				m_tabObjects[m_nowChooseTabIndex].needDraw)
			{//被选中的标签
			//	drawFillBoxEx(_XVector2(m_nowMouseRect.left + m_tabObjects[m_nowChooseTabIndex].offset,m_nowMouseRect.top),
			//		_XVector2(m_tabObjects[m_nowChooseTabIndex].len,TAB_TITLE_HEIGHT * m_size.y),0.35f,0.35f,0.35f,false,false,true);
				drawFillBoxExExA(_XVector2(m_nowMouseRect.left + m_tabObjects[m_nowChooseTabIndex].offset,m_nowMouseRect.top),
					_XVector2(m_tabObjects[m_nowChooseTabIndex].len,TAB_TITLE_HEIGHT * m_size.y),
					0.35f * m_color.fR,0.35f * m_color.fG,0.35f * m_color.fB,m_color.fA,0x0b,0x00,true);
				m_tabObjects[m_nowChooseTabIndex].pFont->setColor(0.0f,0.0f,0.0f,m_color.fA);
			//	m_tabObjects[m_nowChooseTabIndex].pFont->setPosition(m_nowMouseRect.left + m_tabObjects[m_nowChooseTabIndex].offset,m_nowMouseRect.top);
				m_tabObjects[m_nowChooseTabIndex].pFont->draw();
			//	m_texFont.setColor(0.0f,0.0f,0.0f,m_color.fA);
			//	m_texFont.setString(m_tabObjects[m_nowChooseTabIndex].title.c_str());
			//	m_texFont.setPosition(m_nowMouseRect.left + m_tabObjects[m_nowChooseTabIndex].offset,m_nowMouseRect.top);
			//	m_texFont.draw();
			}
		}
	}
	m_leftBtn.draw();
	m_rightBtn.draw();
#if TAB_MODE == 1
	//显示其子物体
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->draw();
		}
	}
#endif
}
void _XTab::updateBtnState()	//更新按钮的状态
{//不可见的物件不在显示范围内
	int startIndex = -1;
	int endIndex = -1;
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		if(m_tabObjects[i].isVisible)
		{
			startIndex = i;
			break;
		}
	}
	for(int i = m_tabObjects.size() - 1;i >= 0;-- i)
	{
		if(m_tabObjects[i].isVisible)
		{
			endIndex = i;
			break;
		}
	}
	if(m_tabObjects.size() <= 0 || startIndex < 0 || 
		(m_tabObjects[startIndex].needDraw && m_tabObjects[endIndex].needDraw))
	{//不需要显示左右按钮
		m_leftBtn.disVisible();
		m_rightBtn.disVisible();
	}else
	{
		m_leftBtn.setVisible();
		m_rightBtn.setVisible();
		if(m_nowStartTabIndex <= startIndex) m_leftBtn.disable();
		else m_leftBtn.enable();
		if(m_tabObjects[endIndex].needDraw) m_rightBtn.disable();
		else m_rightBtn.enable();
	}
}
void _XTab::updateTitle()	//更新标题的显示
{
	int pixelOffset = 0;
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		if(i < m_nowStartTabIndex)
		{
			m_tabObjects[i].needDraw = XFalse;
			continue;
		}
		if(!m_tabObjects[i].isVisible)
		{//不可见的物件
			m_tabObjects[i].needDraw = XFalse;
			continue;
		}
		//m_texFont.setString(m_tabObjects[i].title.c_str());
		//m_tabObjects[i].len = m_texFont.getMaxPixelWidth();
		m_tabObjects[i].len = m_tabObjects[i].pFont->getMaxPixelWidth();
		if(pixelOffset + m_tabObjects[i].len + TAB_TITLE_DISTANCE * m_size.x <= m_nowMouseRect.getWidth() - ((TAB_BTN_WIDTH << 1) * m_size.x))
		{//需要描绘
			m_tabObjects[i].needDraw = XTrue;
			m_tabObjects[i].offset = pixelOffset;
			m_tabObjects[i].pFont->setPosition(m_nowMouseRect.left + m_tabObjects[i].offset,m_nowMouseRect.top);
			pixelOffset += m_tabObjects[i].len + TAB_TITLE_DISTANCE * m_size.x;
		}else
		{
			m_tabObjects[i].needDraw = XFalse;
			pixelOffset += m_tabObjects[i].len + TAB_TITLE_DISTANCE * m_size.x;
		}
	}
}
_XBool _XTab::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
#if TAB_MODE == 1
	//for(int i = 0;i < m_tabObjects.size();++ i)
	//{
	//	for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
	//	{
	//		m_tabObjects[i].objs[j]->mouseProc(x,y,mouseState);
	//	}
	//}
#endif
	if(m_titleRect.isInRect(x,y))
	{
		m_leftBtn.mouseProc(x,y,mouseState);
		m_rightBtn.mouseProc(x,y,mouseState);
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
		{
			_XRect tmpRect;
			tmpRect.top = m_nowMouseRect.top;
			tmpRect.bottom = m_nowMouseRect.top + TAB_TITLE_HEIGHT * m_size.y;
			for(int i = m_nowStartTabIndex;i < m_tabObjects.size();++ i)
			{
				if(m_tabObjects[i].needDraw && m_tabObjects[i].isEnable && m_tabObjects[i].isVisible)
				{
					tmpRect.left = m_tabObjects[i].offset + m_nowMouseRect.left;
					tmpRect.right = m_tabObjects[i].offset + m_nowMouseRect.left + m_tabObjects[i].len;
					if(tmpRect.isInRect(x,y) && i != m_nowChooseTabIndex)	//已经被选中不会再次被选中
					{
						startAction();
						m_nowChooseTabIndex = i;
						if(m_withAction)
						{
							for(int i = 0;i < m_tabObjects[m_nowChooseTabIndex].objs.size();++ i)
							{
								m_tabObjects[m_nowChooseTabIndex].objs[i]->setAlpha(m_actionMoveData.getNowData());
								m_tabObjects[m_nowChooseTabIndex].objs[i]->setVisible();
							}
						}else
						{
							updateObjState();
						}
						if(m_funChooseTab != NULL) m_funChooseTab(m_pClass,m_objectID,m_tabObjects[i].srcTitle);
						break;
					}
				}
			}
		}
		return XTrue;
	}
	return XFalse;
}
_XBool _XTab::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
	!m_isActive ||		//没有激活的控件不接收控制
	!m_isVisible ||	//如果不可见直接退出
	!m_isEnable) return XFalse;		//如果无效则直接退出
#if TAB_MODE == 1
	//for(int i = 0;i < m_tabObjects.size();++ i)
	//{
	//	for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
	//	{
	//		m_tabObjects[i].objs[j]->keyboardProc(keyOrder,keyState);
	//	}
	//}
#endif
	if(keyState == KEY_STATE_UP && m_isBeChoose)
	{//按键弹起时才作相应
		switch(keyOrder)
		{
		case XKEY_LEFT:
			if(m_nowChooseTabIndex > 0) 
			{
				startAction();
				-- m_nowChooseTabIndex;
				if(m_nowStartTabIndex > m_nowChooseTabIndex) m_nowStartTabIndex = m_nowChooseTabIndex;
				updateTitle();	//更新标题的显示
				updateBtnState();	//更新按钮的状态
				if(m_withAction)
				{
					for(int i = 0;i < m_tabObjects[m_nowChooseTabIndex].objs.size();++ i)
					{
						m_tabObjects[m_nowChooseTabIndex].objs[i]->setAlpha(m_actionMoveData.getNowData());
						m_tabObjects[m_nowChooseTabIndex].objs[i]->setVisible();
					}
				}else
				{
					updateObjState();
				}
				if(m_funChooseTab != NULL) m_funChooseTab(m_pClass,m_objectID,m_tabObjects[m_nowChooseTabIndex].srcTitle);
			}
			return XTrue;
		case XKEY_RIGHT:
			if(m_nowChooseTabIndex < m_tabObjects.size() - 1)
			{
				startAction();
				++ m_nowChooseTabIndex;
				//if(m_nowStartTabIndex < m_nowChooseTabIndex) m_nowStartTabIndex = m_nowChooseTabIndex;
				updateTitle();	//更新标题的显示
				updateBtnState();	//更新按钮的状态
				while(true)
				{//移动直到显示出来(超长标签时这里会出现错误)
					if(!m_tabObjects[m_nowChooseTabIndex].needDraw && m_nowStartTabIndex < m_nowChooseTabIndex)
					{
						++ m_nowStartTabIndex;
						updateTitle();	//更新标题的显示
						updateBtnState();	//更新按钮的状态
					}else
					{
						break;
					}
				}
				if(m_withAction)
				{
					for(int i = 0;i < m_tabObjects[m_nowChooseTabIndex].objs.size();++ i)
					{
						m_tabObjects[m_nowChooseTabIndex].objs[i]->setAlpha(m_actionMoveData.getNowData());
						m_tabObjects[m_nowChooseTabIndex].objs[i]->setVisible();
					}
				}else
				{
					updateObjState();
				}
				if(m_funChooseTab != NULL) m_funChooseTab(m_pClass,m_objectID,m_tabObjects[m_nowChooseTabIndex].srcTitle);
			}
			return XTrue;
			break;
		}
	}
	return XFalse;

}
void _XTab::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_nowMouseRect.set(m_mouseRect.left * m_size.x + m_position.x,
		m_mouseRect.top * m_size.y + m_position.y,
		m_mouseRect.right * m_size.x + m_position.x,
		m_mouseRect.bottom * m_size.y + m_position.y);
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->setPosition(_XVector2(m_tabObjects[i].pos[j].x * m_size.x,m_tabObjects[i].pos[j].y * m_size.y) + m_position);
		}
		if(m_tabObjects[i].needDraw)
			m_tabObjects[i].pFont->setPosition(m_nowMouseRect.left + m_tabObjects[i].offset,m_nowMouseRect.top);
	}
	m_titleRect.set(m_nowMouseRect.left,m_nowMouseRect.top,m_nowMouseRect.right,m_nowMouseRect.top + TAB_TITLE_HEIGHT * m_size.y);
	m_leftBtn.setPosition(m_nowMouseRect.right - ((TAB_BTN_WIDTH << 1) * m_size.x),m_nowMouseRect.top);
	m_rightBtn.setPosition(m_nowMouseRect.right - TAB_BTN_WIDTH * m_size.x,m_nowMouseRect.top);
	//检查是否需要更新相关属性
}
void _XTab::setSize(float x,float y)
{
	if(x <= 0.0f || y <= 0.0f) return;
	m_size.set(x,y);
	//下面更新相关属性
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->setPosition(_XVector2(m_tabObjects[i].pos[j].x * m_size.x,m_tabObjects[i].pos[j].y * m_size.y) + m_position);
			m_tabObjects[i].objs[j]->setSize(m_tabObjects[i].size[j] * m_size);
			m_tabObjects[i].pFont->setSize(m_size);
		}
	}
	m_texFont.setSize(m_size);
	m_nowMouseRect.set(m_mouseRect.left * m_size.x + m_position.x,
		m_mouseRect.top * m_size.y + m_position.y,
		m_mouseRect.right * m_size.x + m_position.x,
		m_mouseRect.bottom * m_size.y + m_position.y);
	m_titleRect.set(m_nowMouseRect.left,m_nowMouseRect.top,m_nowMouseRect.right,m_nowMouseRect.top + TAB_TITLE_HEIGHT * m_size.y);
	m_leftBtn.setPosition(m_nowMouseRect.right - ((TAB_BTN_WIDTH << 1) * m_size.x),m_nowMouseRect.top);
	m_leftBtn.setSize(m_size);
	m_rightBtn.setPosition(m_nowMouseRect.right - TAB_BTN_WIDTH * m_size.x,m_nowMouseRect.top);
	m_rightBtn.setSize(m_size);
	updateTitle();
	updateBtnState();
}
int _XTab::getTabIndexByTitle(const std::string &title) const
{
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		if(m_tabObjects[i].srcTitle == title) return i;
	}
	return -1;
}
void _XTab::updateObjState(bool flag)
{
	if(!m_isVisible) return;
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		if(i == m_nowChooseTabIndex)
		{
			if(flag)
			{
				for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
				{
					m_tabObjects[i].objs[j]->setVisible();
				}
			}
		}else
		{
			for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
			{
				m_tabObjects[i].objs[j]->disVisible();
			}
		}
	}
}
void _XTab::release()
{
	if(!m_isInited) return;
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		XDELETE(m_tabObjects[i].pFont);
	}
	_XCtrlManger.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	m_isInited = XFalse;
}
void _XTab::setTabVisible(bool flag,int index)	//设置标签可见
{
	if(index < 0 || index >= m_tabObjects.size()) return;
	if((m_tabObjects[index].isVisible && flag) ||
		(!m_tabObjects[index].isVisible && !flag)) return;	//重复的设置
	m_tabObjects[index].isVisible = flag;

	if(!flag)		//需要判断这个值
	{//如果后面没有可显示的值则回退到第一个可以显示的值
		int endIndex = 0;
		for(int i = m_tabObjects.size() - 1;i >= 0;-- i)
		{
			if(m_tabObjects[i].isVisible)
			{
				endIndex = i;
				break;
			}
		}
		if(m_nowStartTabIndex > endIndex) 
		{
			m_nowStartTabIndex = endIndex;
		}
	}
	if((m_nowChooseTabIndex == index && !flag)
		|| (m_nowChooseTabIndex < 0 && flag)) 
	{//这里应该选择第一个可以选的
		m_nowChooseTabIndex = -1;	//需要判断这个值
		for(int i = 0;i < m_tabObjects.size();++ i)
		{
			if(m_tabObjects[i].isEnable && m_tabObjects[i].isVisible)
			{
				m_nowChooseTabIndex = i;
				break;
			}
		}
	}
	//更新界面
	updateTitle();	//更新标题的显示
	updateBtnState();	//更新按钮的状态
	updateObjState();
}
void _XTab::setTabEnable(bool flag,int index)
{
	if(index < 0 || index >= m_tabObjects.size()) return;
	if((m_tabObjects[index].isEnable && flag) ||
		(!m_tabObjects[index].isEnable && !flag)) return;	//重复的设置
	m_tabObjects[index].isEnable = flag;

	if((m_nowChooseTabIndex == index && !flag)
		|| (m_nowChooseTabIndex < 0 && flag)) 
	{//这里应该选择第一个可以选的
		m_nowChooseTabIndex = -1;	//需要判断这个值
		for(int i = 0;i < m_tabObjects.size();++ i)
		{
			if(m_tabObjects[i].isEnable && m_tabObjects[i].isVisible)
			{
				m_nowChooseTabIndex = i;
				break;
			}
		}
	}
	//更新界面
	updateTitle();	//更新标题的显示
	updateBtnState();	//更新按钮的状态
	updateObjState();
}
void _XTab::update(int stepTime)
{//下面是动作的演示
#if TAB_MODE == 1
	//for(int i = 0;i < m_tabObjects.size();++ i)
	//{
	//	for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
	//	{
	//		m_tabObjects[i].objs[j]->update(stepTime);
	//	}
	//}
#endif
	m_leftBtn.update(stepTime);
	m_rightBtn.update(stepTime);
	if(m_isInAction)
	{
		m_actionMoveData.move(stepTime);
		if(m_actionMoveData.getIsEnd())
		{
			m_isInAction = XFalse;
			for(int i = 0;i < m_tabObjects[m_oldChooseTabIndex].objs.size();++ i)
			{
				m_tabObjects[m_oldChooseTabIndex].objs[i]->setAlpha(1.0f);
			}
			for(int i = 0;i < m_tabObjects[m_nowChooseTabIndex].objs.size();++ i)
			{
				m_tabObjects[m_nowChooseTabIndex].objs[i]->setAlpha(1.0f);
			}
			updateObjState(false);
		}else
		{
			for(int i = 0;i < m_tabObjects[m_oldChooseTabIndex].objs.size();++ i)
			{
				m_tabObjects[m_oldChooseTabIndex].objs[i]->setAlpha(1.0f - m_actionMoveData.getNowData());
			}
			for(int i = 0;i < m_tabObjects[m_nowChooseTabIndex].objs.size();++ i)
			{
				m_tabObjects[m_nowChooseTabIndex].objs[i]->setAlpha(m_actionMoveData.getNowData());
			}
		}
	}
}
void _XTab::addATab(const std::string &title)
{
	if(!m_isInited) return;
	if(getTabIndexByTitle(title) >= 0) return;	//title重名
	//这里需要处理超长标签
	_XTabObject tmp;
	tmp.pFont = createMem<_XFontUnicode>();
	if(tmp.pFont == NULL) return;
	tmp.pFont->setACopy(m_texFont);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(tmp.pFont);
#endif	
	tmp.srcTitle = title;
	if(strlen(title.c_str()) > m_maxTitleLen)
	{//这里需要截断超长字符串
		char *tmpStr = createArrayMem<char>(m_maxTitleLen + 1);
		if(tmpStr == NULL) 
		{
			XDELETE(tmp.pFont);
			return;
		}
		memcpy(tmpStr,title.c_str(),m_maxTitleLen);
		tmpStr[m_maxTitleLen] = '\0';
		if(isAtUnicodeEnd(tmpStr,m_maxTitleLen - 3))
		{
			tmpStr[m_maxTitleLen - 2] = '.';
			tmpStr[m_maxTitleLen - 1] = '.';
		}else
		{
			tmpStr[m_maxTitleLen - 3] = '.';
			tmpStr[m_maxTitleLen - 2] = '.';
			tmpStr[m_maxTitleLen - 1] = '\0';
		}
		tmp.title = tmpStr;
		XDELETE_ARRAY(tmpStr);
	}else
	{
		tmp.title = title;
	}
	tmp.pFont->setString(tmp.title.c_str());
	tmp.isEnable = true;
	m_tabObjects.push_back(tmp);
	updateTitle();
	updateBtnState();
}
_XBool _XTab::setTabStr(const char *str,int index)
{
	if(str == NULL || index < 0 || index >= m_tabObjects.size()) return XFalse;
	if(getTabIndexByTitle(str) >= 0) return XFalse;	//title重名
	m_tabObjects[index].srcTitle = str;
	if(strlen(str) > m_maxTitleLen)
	{//这里需要截断超长字符串
		char *tmpStr = createArrayMem<char>(m_maxTitleLen + 1);
		if(tmpStr == NULL) return XFalse;
		memcpy(tmpStr,str,m_maxTitleLen);
		tmpStr[m_maxTitleLen] = '\0';
		if(isAtUnicodeEnd(tmpStr,m_maxTitleLen - 3))
		{
			tmpStr[m_maxTitleLen - 2] = '.';
			tmpStr[m_maxTitleLen - 1] = '.';
		}else
		{
			tmpStr[m_maxTitleLen - 3] = '.';
			tmpStr[m_maxTitleLen - 2] = '.';
			tmpStr[m_maxTitleLen - 1] = '\0';
		}
		m_tabObjects[index].title = tmpStr;
		XDELETE_ARRAY(tmpStr);
	}else
	{
		m_tabObjects[index].title = str;
	}
	m_tabObjects[index].pFont->setString(m_tabObjects[index].title.c_str());
	updateTitle();
	updateBtnState();
	return XTrue;
}
_XBool _XTab::setTabsStr(const char *str)		//;作为分隔符
{
	if(str == NULL) return XFalse;
	//这里需要检查数据的合法性，防止重复的标题名称(尚未完成)
	int index = 0;
	int offset;
	char *tmp = createArrayMem<char>(strlen(str) + 1);
	strcpy(tmp,str);
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		if(index == 0) offset = getCharPosition(tmp + index,';');
		else offset = getCharPosition(tmp + index + 1,';');
		if(offset > 0)
		{
			if(index == 0)
			{
				tmp[index + offset] = '\0';
				setTabStr(tmp + index,i);
				index += offset;
			}else
			{
				tmp[index + offset + 1] = '\0';
				setTabStr(tmp + index + 1,i);
				index += offset + 1;
			}
		}else
		{
			if(tmp[index + 1] == '\0') break;
			else
			{
				setTabStr(tmp + index + 1,i);
				break;
			}
		}
	}
	XDELETE_ARRAY(tmp);
	return XTrue;
}
void _XTab::clearAllObj()
{
	//将所有控件的控制权交给控件管理器
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
#if TAB_MODE == 1
//		for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
//		{
//			_XCtrlManger.addACtrl(m_tabObjects[i].objs[j]);	//在物件管理器中注册当前物件
//#if WITH_OBJECT_MANAGER
//			_XObjManger.addAObject(m_tabObjects[i].objs[j]);
//#endif
//		}
		for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			_XCtrlManger.setNeedDraw(XTrue,m_tabObjects[i].objs[j]);	//归还显示权
		}
#endif
		m_tabObjects[i].objs.clear();
		m_tabObjects[i].pos.clear();
		m_tabObjects[i].size.clear();
	}
}
void _XTab::addObjToTab(_XControlBasic *obj,int index)	//向一个标签中添加物件
{
	if(obj == NULL || index < 0 || index >= m_tabObjects.size()) return;
	m_tabObjects[index].objs.push_back(obj);
	m_tabObjects[index].pos.push_back((obj->getPosition() - m_position) / m_size);
	m_tabObjects[index].size.push_back(obj->getSize() / m_size);
	updateObjState();
#if TAB_MODE == 1
	//从设备管理器中获取设备的管理权限
//	_XCtrlManger.decreaseAObject(obj);	//在物件管理器中注册当前物件
//#if WITH_OBJECT_MANAGER
//	_XObjManger.decreaseAObject(obj);
//#endif
	_XCtrlManger.setNeedDraw(XFalse,obj);	//剥夺显示权
#endif
}