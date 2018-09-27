#include "XStdHead.h"
#include "XTab.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "../XXml.h"
namespace XE{
void XTab::ctrlProc(void*pClass,int id,int eventID)
{
	if(eventID != XButton::BTN_MOUSE_DOWN) return;
	XTab &ref = *(XTab *)pClass;
	if(id == ref.m_leftBtn.getControlID())
	{
		if(ref.m_curStartTabIndex > 0)
		{
			-- ref.m_curStartTabIndex;
			ref.updateTitle();
			ref.updateBtnState();
		}
		return;
	}
	if(id == ref.m_rightBtn.getControlID())
	{
		if(ref.m_curStartTabIndex < (int)(ref.m_tabObjects.size()) - 1)
		{
			++ ref.m_curStartTabIndex;
			ref.updateTitle();
			ref.updateBtnState();
		}
		return;
	}
}
XBool XTab::initWithoutSkin(const XRect& rect, const XFontUnicode& font)
{
	if (m_isInited) return XFalse;
	if (rect.getWidth() <= 0 || rect.getHeight() <= 0) return XFalse;
	//检查数据的合法性
	m_mouseRect = rect;
	m_position.reset();
	m_scale.set(1.0f);
	m_curMouseRect.set(m_mouseRect.getLT() * m_scale + m_position,
		m_mouseRect.getRB() * m_scale + m_position);
	m_titleRect.set(m_curMouseRect.left, m_curMouseRect.top,
		m_curMouseRect.right, m_curMouseRect.top + TAB_TITLE_HEIGHT * m_scale.y);
	if (!m_texFont.setACopy(font)) return XFalse;
	m_texFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_texFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_texFont.setColor(XFColor::black);
	m_texFont.setScale(m_scale);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_texFont);
#endif
	m_curTextWidth = m_texFont.getTextSize().x * m_texFont.getScale().x * 0.5f;
	m_maxTitleLen = (m_mouseRect.getWidth() - (m_tabBtnWidth << 1) - m_tabTitleDistance) / m_curTextWidth;	//计算标题的最大长度
	if (m_maxTitleLen < 3) return false;	//窗口太小

	m_leftBtn.initWithoutSkin("", font, XRect(0.0f, 0.0f, m_tabBtnWidth, TAB_TITLE_HEIGHT));
	m_leftBtn.setPosition(m_curMouseRect.right - ((m_tabBtnWidth << 1) * m_scale.x), m_curMouseRect.top);
	m_leftBtn.disable();
	m_leftBtn.setEventProc(ctrlProc, this);
	m_leftBtn.setScale(m_scale);
	m_leftBtn.setSymbol(BTN_SYMBOL_LEFT);
	XCtrlManager.decreaseAObject(&m_leftBtn);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_leftBtn);
#endif
	m_rightBtn.initWithoutSkin("", font, XRect(0.0f, 0.0f, m_tabBtnWidth, TAB_TITLE_HEIGHT));
	m_rightBtn.setPosition(m_curMouseRect.right - m_tabBtnWidth * m_scale.x, m_curMouseRect.top);
	m_rightBtn.disable();
	m_rightBtn.setEventProc(ctrlProc, this);
	m_rightBtn.setScale(m_scale);
	m_rightBtn.setSymbol(BTN_SYMBOL_RIGHT);
	XCtrlManager.decreaseAObject(&m_rightBtn);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_rightBtn);
#endif

	m_isVisible = m_isEnable = m_isActive = XTrue;
	m_withoutTex = m_isInited = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	return XTrue;
}
void XTab::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	if(m_withoutTex)
	{//不使用贴图时
		//描绘标签的背景
		XRender::drawFillRect(m_curMouseRect.getLT(),
			XVec2(m_curMouseRect.getWidth(),TAB_TITLE_HEIGHT * m_scale.y),
			XCCS::lightBlackColor * m_color);
		//下面显示标签
		if(m_withAction && m_isInAction)
		{
			for(unsigned int i = m_curStartTabIndex;i < m_tabObjects.size();++ i)
			{//显示标签背景
				if (!m_tabObjects[i].needDraw) continue;
				XRender::drawFillRectExAS(XVec2(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top),
					XVec2(m_tabObjects[i].len,TAB_TITLE_HEIGHT * m_scale.y),
					XCCS::disableColor * m_color);
			}
		}else
		{
			for(unsigned int i = m_curStartTabIndex;i < m_tabObjects.size();++ i)
			{//显示标签背景
				if (!m_tabObjects[i].needDraw || m_curChooseTabIndex == i) continue;
				if(m_tabObjects[i].isEnable)
				{//没有被选中的标签
					XRender::drawFillRectExAS(XVec2(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top),
						XVec2(m_tabObjects[i].len,TAB_TITLE_HEIGHT * m_scale.y),
						XCCS::disableColor * m_color);
					m_tabObjects[i].pFont->setColor(XCCS::downColor * m_color);
					//m_texFont.setColor(0.5f * m_color.r,0.5f * m_color.g,0.5f * m_color.b,m_color.a);
				}else
				{
					XRender::drawFillRectExAS(XVec2(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top),
						XVec2(m_tabObjects[i].len,TAB_TITLE_HEIGHT * m_scale.y),
						XCCS::blackDownColor * m_color);
					m_tabObjects[i].pFont->setColor(XCCS::downColor * m_color);
					//m_texFont.setColor(0.5f * m_color.r,0.5f * m_color.g,0.5f * m_color.b,m_color.a);
				}
			}
			for(unsigned int i = m_curStartTabIndex;i < m_tabObjects.size();++ i)
			{//显示标签文字
				if (!m_tabObjects[i].needDraw || m_curChooseTabIndex == i) continue;
			//	m_tabObjects[i].pFont->setPosition(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top);
				m_tabObjects[i].pFont->draw();
			//	m_texFont.setString(m_tabObjects[i].title.c_str());
			//	m_texFont.setPosition(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top);
			//	m_texFont.draw();
			}
		}
		//描绘下面的面板
		XRender::drawFillRectExAS(XVec2(m_curMouseRect.left,m_curMouseRect.top + TAB_TITLE_HEIGHT * m_scale.y),
			XVec2(m_curMouseRect.getWidth(),m_curMouseRect.getHeight() - TAB_TITLE_HEIGHT * m_scale.y),
			XCCS::disableColor * m_color,true);
		//描绘被选中的项
		if(m_withAction && m_isInAction)
		{
			//描绘动态，注意这里没有考虑源出于不显示状态的情况
			if(m_tabObjects[m_oldChooseTabIndex].needDraw)
			{
				XRender::drawFillRectExExA(XVec2(m_curMouseRect.left +
					XMath::lineSlerp<float>(m_tabObjects[m_oldChooseTabIndex].offset, m_tabObjects[m_curChooseTabIndex].offset, m_actionMoveData.getCurData()), m_curMouseRect.top),
					XVec2(XMath::lineSlerp<float>(m_tabObjects[m_oldChooseTabIndex].len, m_tabObjects[m_curChooseTabIndex].len, m_actionMoveData.getCurData()), TAB_TITLE_HEIGHT * m_scale.y),
					XCCS::disableColor * m_color, 0x0b, 0x00, true);
			}
			else
			{
				if (m_oldChooseTabIndex < m_curChooseTabIndex)
				{
					float pos = m_curMouseRect.left - m_tabObjects[m_oldChooseTabIndex].len;
					pos = XMath::lineSlerp<float>(pos, m_tabObjects[m_curChooseTabIndex].offset, m_actionMoveData.getCurData());
					float len = XMath::lineSlerp<float>(m_tabObjects[m_oldChooseTabIndex].len, m_tabObjects[m_curChooseTabIndex].len, m_actionMoveData.getCurData());
					if (pos <= 0)
					{
						len += pos;
						pos = 0;
					}
					XRender::drawFillRectExExA(XVec2(m_curMouseRect.left + pos, m_curMouseRect.top),
						XVec2(len, TAB_TITLE_HEIGHT * m_scale.y),
						XCCS::disableColor * m_color, 0x0b, 0x00, true);
				}
				else
				{
					float pos = m_curMouseRect.right;
					pos = XMath::lineSlerp<float>(pos, m_tabObjects[m_curChooseTabIndex].offset, m_actionMoveData.getCurData());
					float len = XMath::lineSlerp<float>(m_tabObjects[m_oldChooseTabIndex].len, m_tabObjects[m_curChooseTabIndex].len, m_actionMoveData.getCurData());
					if (pos + len > m_curMouseRect.right)
					{
						len -= m_curMouseRect.right - pos;
					}
					XRender::drawFillRectExExA(XVec2(m_curMouseRect.left + pos, m_curMouseRect.top),
						XVec2(len, TAB_TITLE_HEIGHT * m_scale.y),
						XCCS::disableColor * m_color, 0x0b, 0x00, true);
				}
			}
			for(unsigned int i = m_curStartTabIndex;i < m_tabObjects.size();++ i)
			{//显示标签文字
				if (!m_tabObjects[i].needDraw) continue;
				if(m_curChooseTabIndex != i)
				{
					if(m_oldChooseTabIndex == i)
					{
						m_tabObjects[i].pFont->setColor(XCCS::downColor * m_color * XFColor(m_actionMoveData.getCurData(),1.0f));
						//m_texFont.setColor(0.5f * m_color.r * temp,0.5f * m_color.g * temp,0.5f * m_color.b * temp,m_color.a);
					}else
					{
						m_tabObjects[i].pFont->setColor(XCCS::downColor * m_color);
						//m_texFont.setColor(0.5f * m_color.r,0.5f * m_color.g,0.5f * m_color.b,m_color.a);
					}
				}else
				{
					m_tabObjects[i].pFont->setColor(XCCS::downColor * m_color * XFColor(1.0f - m_actionMoveData.getCurData(),1.0f));
					//m_texFont.setColor(0.5f * m_color.r * temp,0.5f * m_color.g * temp,0.5f * m_color.b * temp,m_color.a);
				}
			//	m_tabObjects[i].pFont->setPosition(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top);
				m_tabObjects[i].pFont->draw();
			//	m_texFont.setString(m_tabObjects[i].title.c_str());
			//	m_texFont.setPosition(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top);
			//	m_texFont.draw();
			}
		}else
		{
			if(m_curChooseTabIndex >= m_curStartTabIndex && 
				(m_curChooseTabIndex < m_tabObjects.size() || m_curChooseTabIndex < 0) &&
				m_tabObjects[m_curChooseTabIndex].needDraw)
			{//被选中的标签
			//	drawFillRectEx(XVec2(m_curMouseRect.left + m_tabObjects[m_curChooseTabIndex].offset,m_curMouseRect.top),
			//		XVec2(m_tabObjects[m_curChooseTabIndex].len,TAB_TITLE_HEIGHT * m_scale.y),0.35f,0.35f,0.35f,false,false,true);
				XRender::drawFillRectExExA(XVec2(m_curMouseRect.left + m_tabObjects[m_curChooseTabIndex].offset, m_curMouseRect.top),
					XVec2(m_tabObjects[m_curChooseTabIndex].len, TAB_TITLE_HEIGHT * m_scale.y),
					XCCS::disableColor * m_color, 0x0b, 0x00, true);
				m_tabObjects[m_curChooseTabIndex].pFont->setColor(XCCS::darkColor * m_color);
			//	m_tabObjects[m_curChooseTabIndex].pFont->setPosition(m_curMouseRect.left + m_tabObjects[m_curChooseTabIndex].offset,m_curMouseRect.top);
				m_tabObjects[m_curChooseTabIndex].pFont->draw();
			//	m_texFont.setColor(0.0f,0.0f,0.0f,m_color.a);
			//	m_texFont.setString(m_tabObjects[m_curChooseTabIndex].title.c_str());
			//	m_texFont.setPosition(m_curMouseRect.left + m_tabObjects[m_curChooseTabIndex].offset,m_curMouseRect.top);
			//	m_texFont.draw();
			}
		}
	}
	m_leftBtn.draw();
	m_rightBtn.draw();

	m_ctrlManager.draw();
	m_ctrlManager.drawUp();

	if (m_funUpDraw != nullptr) m_funUpDraw(m_pClass);
}
void XTab::updateBtnState()	//更新按钮的状态
{//不可见的物件不在显示范围内
	int startIndex = -1;
	int endIndex = -1;
	for (unsigned int i = 0; i < m_tabObjects.size(); ++i)
	{
		if (!m_tabObjects[i].isVisible) continue;
		startIndex = i;
		break;
	}
	for (int i = (int)(m_tabObjects.size()) - 1; i >= 0; --i)
	{
		if (!m_tabObjects[i].isVisible) continue;
		endIndex = i;
		break;
	}
	if (m_tabObjects.size() <= 0 || startIndex < 0 ||
		(m_tabObjects[startIndex].needDraw && m_tabObjects[endIndex].needDraw))
	{//不需要显示左右按钮
		m_leftBtn.disVisible();
		m_rightBtn.disVisible();
	}
	else
	{
		m_leftBtn.setVisible();
		m_rightBtn.setVisible();
		if (m_curStartTabIndex <= startIndex) m_leftBtn.disable();
		else m_leftBtn.enable();
		if (m_tabObjects[endIndex].needDraw) m_rightBtn.disable();
		else m_rightBtn.enable();
	}
}
void XTab::updateTitle()	//更新标题的显示
{
	int pixelOffset = 0;
	for (unsigned int i = 0; i < m_tabObjects.size(); ++i)
	{
		if (i < m_curStartTabIndex)
		{
			m_tabObjects[i].needDraw = XFalse;
			continue;
		}
		if (!m_tabObjects[i].isVisible)
		{//不可见的物件
			m_tabObjects[i].needDraw = XFalse;
			continue;
		}
		//m_texFont.setString(m_tabObjects[i].title.c_str());
		//m_tabObjects[i].len = m_texFont.getMaxPixelWidth();
		m_tabObjects[i].len = m_tabObjects[i].pFont->getMaxPixelWidth();
		if (pixelOffset + m_tabObjects[i].len + m_tabTitleDistance * m_scale.x <= m_curMouseRect.getWidth() - ((m_tabBtnWidth << 1) * m_scale.x))
		{//需要描绘
			m_tabObjects[i].needDraw = XTrue;
			m_tabObjects[i].offset = pixelOffset;
			m_tabObjects[i].pFont->setPosition(m_curMouseRect.left + m_tabObjects[i].offset, m_curMouseRect.top);
			pixelOffset += m_tabObjects[i].len + m_tabTitleDistance * m_scale.x;
		}
		else
		{
			m_tabObjects[i].needDraw = XFalse;
			pixelOffset += m_tabObjects[i].len + m_tabTitleDistance * m_scale.x;
		}
	}
}
XBool XTab::mouseProc(const XVec2& p, XMouseState mouseState)
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if (m_isSilent) return XFalse;

	m_ctrlManager.mouseProc(p, mouseState);
	if (m_titleRect.isInRect(p))
	{
		m_leftBtn.mouseProc(p, mouseState);
		m_rightBtn.mouseProc(p, mouseState);
		if (mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
		{
			XRect tmpRect;
			tmpRect.top = m_curMouseRect.top;
			tmpRect.bottom = m_curMouseRect.top + TAB_TITLE_HEIGHT * m_scale.y;
			for (int i = m_curStartTabIndex; i < m_tabObjects.size(); ++i)
			{
				if (m_tabObjects[i].needDraw && m_tabObjects[i].isEnable && m_tabObjects[i].isVisible)
				{
					tmpRect.left = m_tabObjects[i].offset + m_curMouseRect.left;
					tmpRect.right = m_tabObjects[i].offset + m_curMouseRect.left + m_tabObjects[i].len;
					if (tmpRect.isInRect(p) && i != m_curChooseTabIndex)	//已经被选中不会再次被选中
					{
						startAction();
						m_curChooseTabIndex = i;
						if (m_withAction)
						{
							for (unsigned int j = 0; j < m_tabObjects[m_curChooseTabIndex].objs.size(); ++j)
							{
								m_tabObjects[m_curChooseTabIndex].objs[j]->setAlpha(m_actionMoveData.getCurData());
								m_ctrlManager.setNeedDrawAndInput(true, true, m_tabObjects[m_curChooseTabIndex].objs[j]);
							}
						}
						else
						{
							updateObjState();
						}
						//if(m_funChooseTab != NULL) m_funChooseTab(m_pClass,m_objectID,m_tabObjects[i].srcTitle);
						//m_curChooseTabStr = m_tabObjects[i].srcTitle;
						if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, TAB_CHOOSE);
						else XCtrlManager.eventProc(m_objectID, TAB_CHOOSE);
						break;
					}
				}
			}
		}
		return XTrue;
	}
	return XFalse;
}
XBool XTab::keyboardProc(int keyOrder, XKeyState keyState)
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if (m_isSilent) return XFalse;
	m_ctrlManager.keyProc(keyOrder, keyState);
	if (keyState == KEY_STATE_UP && m_isBeChoose)
	{//按键弹起时才作相应
		switch (keyOrder)
		{
		case XKEY_LEFT:
			if (m_curChooseTabIndex > 0)
			{
				startAction();
				--m_curChooseTabIndex;
				if (m_curStartTabIndex > m_curChooseTabIndex) m_curStartTabIndex = m_curChooseTabIndex;
				updateTitle();	//更新标题的显示
				updateBtnState();	//更新按钮的状态
				if (m_withAction)
				{
					for (unsigned int i = 0; i < m_tabObjects[m_curChooseTabIndex].objs.size(); ++i)
					{
						m_tabObjects[m_curChooseTabIndex].objs[i]->setAlpha(m_actionMoveData.getCurData());
						m_ctrlManager.setNeedDrawAndInput(true, true, m_tabObjects[m_curChooseTabIndex].objs[i]);
					}
				}
				else
				{
					updateObjState();
				}
				//if(m_funChooseTab != NULL) m_funChooseTab(m_pClass,m_objectID,m_tabObjects[m_curChooseTabIndex].srcTitle);
				//m_curChooseTabStr = m_tabObjects[m_curChooseTabIndex].srcTitle;
				if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, TAB_CHOOSE);
				else XCtrlManager.eventProc(m_objectID, TAB_CHOOSE);
			}
			return XTrue;
		case XKEY_RIGHT:
			if (m_curChooseTabIndex < (int)(m_tabObjects.size()) - 1)
			{
				startAction();
				++m_curChooseTabIndex;
				//if(m_curStartTabIndex < m_curChooseTabIndex) m_curStartTabIndex = m_curChooseTabIndex;
				updateTitle();	//更新标题的显示
				updateBtnState();	//更新按钮的状态
				while (true)
				{//移动直到显示出来(超长标签时这里会出现错误)
					if (!m_tabObjects[m_curChooseTabIndex].needDraw && m_curStartTabIndex < m_curChooseTabIndex)
					{
						++m_curStartTabIndex;
						updateTitle();	//更新标题的显示
						updateBtnState();	//更新按钮的状态
					}
					else
					{
						break;
					}
				}
				if (m_withAction)
				{
					for (unsigned int i = 0; i < m_tabObjects[m_curChooseTabIndex].objs.size(); ++i)
					{
						m_tabObjects[m_curChooseTabIndex].objs[i]->setAlpha(m_actionMoveData.getCurData());
						m_ctrlManager.setNeedDrawAndInput(true, true, m_tabObjects[m_curChooseTabIndex].objs[i]);
					}
				}
				else
				{
					updateObjState();
				}
				//if(m_funChooseTab != NULL) m_funChooseTab(m_pClass,m_objectID,m_tabObjects[m_curChooseTabIndex].srcTitle);
				//m_curChooseTabStr = m_tabObjects[m_curChooseTabIndex].srcTitle;
				if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, TAB_CHOOSE);
				else XCtrlManager.eventProc(m_objectID, TAB_CHOOSE);
			}
			return XTrue;
			break;
		}
	}
	return XFalse;
}
void XTab::setPosition(const XVec2& p)
{
	m_position = p;
	m_curMouseRect.set(m_mouseRect.getLT() * m_scale + m_position,
		m_mouseRect.getRB() * m_scale + m_position);
	for (unsigned int i = 0; i < m_tabObjects.size(); ++i)
	{
		for (unsigned int j = 0; j < m_tabObjects[i].objs.size(); ++j)
		{
		//	m_tabObjects[i].objs[j]->setPosition(XVec2(m_tabObjects[i].pos[j].x * m_scale.x,m_tabObjects[i].pos[j].y * m_scale.y) + m_position);
			m_tabObjects[i].objs[j]->setPosition(m_tabObjects[i].pos[j] * m_scale + m_position);
		}
		if (m_tabObjects[i].needDraw)
			m_tabObjects[i].pFont->setPosition(m_curMouseRect.left + m_tabObjects[i].offset, m_curMouseRect.top);
	}
	m_titleRect.set(m_curMouseRect.left, m_curMouseRect.top, m_curMouseRect.right, m_curMouseRect.top + TAB_TITLE_HEIGHT * m_scale.y);
	m_leftBtn.setPosition(m_curMouseRect.right - ((m_tabBtnWidth << 1) * m_scale.x), m_curMouseRect.top);
	m_rightBtn.setPosition(m_curMouseRect.right - m_tabBtnWidth * m_scale.x, m_curMouseRect.top);
	//检查是否需要更新相关属性
}
void XTab::setScale(const XVec2& s)
{
	if (s.x <= 0.0f || s.y <= 0.0f) return;
	m_scale = s;
	//下面更新相关属性
	for (unsigned int i = 0; i < m_tabObjects.size(); ++i)
	{
		for (unsigned int j = 0; j < m_tabObjects[i].objs.size(); ++j)
		{
		//	m_tabObjects[i].objs[j]->setPosition(XVec2(m_tabObjects[i].pos[j].x * m_scale.x,m_tabObjects[i].pos[j].y * m_scale.y) + m_position);
			m_tabObjects[i].objs[j]->setPosition(m_tabObjects[i].pos[j] * m_scale + m_position);
			m_tabObjects[i].objs[j]->setScale(m_tabObjects[i].scale[j] * m_scale);
			m_tabObjects[i].pFont->setScale(m_scale);
		}
	}
	m_texFont.setScale(m_scale);
	m_curMouseRect.set(m_mouseRect.getLT() * m_scale + m_position,
		m_mouseRect.getRB() * m_scale + m_position);
	m_titleRect.set(m_curMouseRect.left, m_curMouseRect.top, m_curMouseRect.right, m_curMouseRect.top + TAB_TITLE_HEIGHT * m_scale.y);
	m_leftBtn.setPosition(m_curMouseRect.right - ((m_tabBtnWidth << 1) * m_scale.x), m_curMouseRect.top);
	m_leftBtn.setScale(m_scale);
	m_rightBtn.setPosition(m_curMouseRect.right - m_tabBtnWidth * m_scale.x, m_curMouseRect.top);
	m_rightBtn.setScale(m_scale);
	updateTitle();
	updateBtnState();
}
int XTab::getTabIndexByTitle(const std::string& title) const
{
	for (unsigned int i = 0; i < m_tabObjects.size(); ++i)
	{
		if (m_tabObjects[i].srcTitle == title) return i;
	}
	return -1;
}
void XTab::updateObjState(bool flag)
{
	if(!m_isVisible) return;
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		if(i == m_curChooseTabIndex)
		{
			if(!flag) continue;
			for(unsigned int j = 0;j < m_tabObjects[i].objs.size();++ j)
			{
				m_ctrlManager.setNeedDrawAndInput(true,true,m_tabObjects[i].objs[j]);
				//m_tabObjects[i].objs[j]->setVisible();
			}
		}else
		{
			//这里换一种控制方式不改变控件的内部状态，而是通过管理器来管理其状态
			for(unsigned int j = 0;j < m_tabObjects[i].objs.size();++ j)
			{
				m_ctrlManager.setNeedDrawAndInput(false,false,m_tabObjects[i].objs[j]);
				//m_tabObjects[i].objs[j]->disVisible();
			}
		}
	}
}
void XTab::release()
{
	if (!m_isInited) return;
	for (unsigned int i = 0; i < m_tabObjects.size(); ++i)
	{
		XMem::XDELETE(m_tabObjects[i].pFont);
	}
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_ctrlManager.clearAllObject();	//这里需要清空所有的物件
	m_isInited = XFalse;
}
void XTab::setTabVisible(bool flag, int index)	//设置标签可见
{
	if (index < 0 || index >= m_tabObjects.size()) return;
	if ((m_tabObjects[index].isVisible && flag) ||
		(!m_tabObjects[index].isVisible && !flag)) return;	//重复的设置
	m_tabObjects[index].isVisible = flag;

	if (!flag)		//需要判断这个值
	{//如果后面没有可显示的值则回退到第一个可以显示的值
		int endIndex = 0;
		for (int i = (int)(m_tabObjects.size()) - 1; i >= 0; --i)
		{
			if (m_tabObjects[i].isVisible)
			{
				endIndex = i;
				break;
			}
		}
		if (m_curStartTabIndex > endIndex)
		{
			m_curStartTabIndex = endIndex;
		}
	}
	if ((m_curChooseTabIndex == index && !flag)
		|| (m_curChooseTabIndex < 0 && flag))
	{//这里应该选择第一个可以选的
		m_curChooseTabIndex = -1;	//需要判断这个值
		for (unsigned int i = 0; i < m_tabObjects.size(); ++i)
		{
			if (m_tabObjects[i].isEnable && m_tabObjects[i].isVisible)
			{
				m_curChooseTabIndex = i;
				break;
			}
		}
	}
	//更新界面
	updateTitle();	//更新标题的显示
	updateBtnState();	//更新按钮的状态
	updateObjState();
}
void XTab::setTabEnable(bool flag,unsigned int index)
{
	if(index >= m_tabObjects.size()) return;
	if((m_tabObjects[index].isEnable && flag) ||
		(!m_tabObjects[index].isEnable && !flag)) return;	//重复的设置
	m_tabObjects[index].isEnable = flag;

	if((m_curChooseTabIndex == index && !flag)
		|| (m_curChooseTabIndex < 0 && flag)) 
	{//这里应该选择第一个可以选的
		m_curChooseTabIndex = -1;	//需要判断这个值
		for(unsigned int i = 0;i < m_tabObjects.size();++ i)
		{
			if(m_tabObjects[i].isEnable && m_tabObjects[i].isVisible)
			{
				m_curChooseTabIndex = i;
				break;
			}
		}
	}
	//更新界面
	updateTitle();	//更新标题的显示
	updateBtnState();	//更新按钮的状态
	updateObjState();
}
void XTab::update(float stepTime)
{//下面是动作的演示
	m_leftBtn.update(stepTime);
	m_rightBtn.update(stepTime);
	if(m_isInAction)
	{
		m_actionMoveData.move(stepTime);
		if(m_actionMoveData.getIsEnd())
		{
			m_isInAction = XFalse;
			for(unsigned int i = 0;i < m_tabObjects[m_oldChooseTabIndex].objs.size();++ i)
			{
				m_tabObjects[m_oldChooseTabIndex].objs[i]->setAlpha(1.0f);
			}
			for(unsigned int i = 0;i < m_tabObjects[m_curChooseTabIndex].objs.size();++ i)
			{
				m_tabObjects[m_curChooseTabIndex].objs[i]->setAlpha(1.0f);
			}
			updateObjState(false);
		}else
		{
			for(unsigned int i = 0;i < m_tabObjects[m_oldChooseTabIndex].objs.size();++ i)
			{
				m_tabObjects[m_oldChooseTabIndex].objs[i]->setAlpha(1.0f - m_actionMoveData.getCurData());
			}
			for(unsigned int i = 0;i < m_tabObjects[m_curChooseTabIndex].objs.size();++ i)
			{
				m_tabObjects[m_curChooseTabIndex].objs[i]->setAlpha(m_actionMoveData.getCurData());
			}
		}
	}
	m_ctrlManager.update(stepTime);
}
void XTab::addATab(const std::string& title)
{
	if (!m_isInited) return;
	if (getTabIndexByTitle(title) >= 0) return;	//title重名
	//这里需要处理超长标签
	XTabObject tmp;
	tmp.pFont = XMem::createMem<XFontUnicode>();
	if (tmp.pFont == NULL) return;
	tmp.pFont->setACopy(m_texFont);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(tmp.pFont);
#endif	
	tmp.srcTitle = title;
	if (title.length() > m_maxTitleLen || m_maxTitleLen < 0)
	{//这里需要截断超长字符串
		char *tmpStr = XMem::createArrayMem<char>(m_maxTitleLen + 1);
		if (tmpStr == NULL)
		{
			XMem::XDELETE(tmp.pFont);
			return;
		}
		memcpy(tmpStr, title.c_str(), m_maxTitleLen);
		tmpStr[m_maxTitleLen] = '\0';
		if (XString::isAtUnicodeEnd(tmpStr, m_maxTitleLen - 3))
		{
			memcpy(tmpStr + m_maxTitleLen - 2, &"..", 2);
			//tmpStr[m_maxTitleLen - 2] = '.';
			//tmpStr[m_maxTitleLen - 1] = '.';
		}
		else
		{
			memcpy(tmpStr + m_maxTitleLen - 3, &"..", 3);
			//tmpStr[m_maxTitleLen - 3] = '.';
			//tmpStr[m_maxTitleLen - 2] = '.';
			//tmpStr[m_maxTitleLen - 1] = '\0';
		}
		tmp.title = tmpStr;
		XMem::XDELETE_ARRAY(tmpStr);
	}
	else
	{
		tmp.title = title;
	}
	tmp.pFont->setString(tmp.title.c_str());
	tmp.isEnable = true;
	m_tabObjects.push_back(tmp);
	updateTitle();
	updateBtnState();
}
void XTab::addATabs(const std::string& title)
{
	if (!m_isInited) return;
	int sum = XString::getCharSum(title.c_str(), ';');
	if (sum < 0) sum = 0;
	int len = title.size();
	if (title[len - 1] != ';') sum += 1; //不是以';'结束
	int offset = 0;
	for (int i = 0; i < sum; ++i)
	{
		len = XString::getCharPosition(title.c_str() + offset, ';');
		std::string tmpStr = title.c_str() + offset;
		if (len >= 0) tmpStr.at(len) = '\0';
		addATab(tmpStr.c_str());
		offset += len + 1;
	}
	return;
}
XBool XTab::setTabStr(const char *str, unsigned int index)
{
	if (str == NULL || index >= m_tabObjects.size()) return XFalse;
	if (getTabIndexByTitle(str) >= 0) return XFalse;	//title重名
	m_tabObjects[index].srcTitle = str;
	if (strlen(str) > m_maxTitleLen || m_maxTitleLen < 0)
	{//这里需要截断超长字符串
		char *tmpStr = XMem::createArrayMem<char>(m_maxTitleLen + 1);
		if (tmpStr == NULL) return XFalse;
		memcpy(tmpStr, str, m_maxTitleLen);
		tmpStr[m_maxTitleLen] = '\0';
		if (XString::isAtUnicodeEnd(tmpStr, m_maxTitleLen - 3))
		{
			memcpy(tmpStr + m_maxTitleLen - 2, &"..", 2);
			//tmpStr[m_maxTitleLen - 2] = '.';
			//tmpStr[m_maxTitleLen - 1] = '.';
		}
		else
		{
			memcpy(tmpStr + m_maxTitleLen - 3, &"..", 3);
			//tmpStr[m_maxTitleLen - 3] = '.';
			//tmpStr[m_maxTitleLen - 2] = '.';
			//tmpStr[m_maxTitleLen - 1] = '\0';
		}
		m_tabObjects[index].title = tmpStr;
		XMem::XDELETE_ARRAY(tmpStr);
	}
	else
	{
		m_tabObjects[index].title = str;
	}
	m_tabObjects[index].pFont->setString(m_tabObjects[index].title.c_str());
	updateTitle();
	updateBtnState();
	return XTrue;
}
XBool XTab::setTabsStr(const char *str)		//;作为分隔符
{
	if (str == NULL) return XFalse;
	//这里需要检查数据的合法性，防止重复的标题名称(尚未完成)
	int index = 0;
	int offset = strlen(str) + 1;
	char *tmp = XMem::createArrayMem<char>(offset);
	strcpy_s(tmp, offset, str);
	for (unsigned int i = 0; i < m_tabObjects.size(); ++i)
	{
		if (index == 0) offset = XString::getCharPosition(tmp + index, ';');
		else offset = XString::getCharPosition(tmp + index + 1, ';');
		if (offset > 0)
		{
			if (index == 0)
			{
				tmp[index + offset] = '\0';
				setTabStr(tmp + index, i);
				index += offset;
			}
			else
			{
				tmp[index + offset + 1] = '\0';
				setTabStr(tmp + index + 1, i);
				index += offset + 1;
			}
		}
		else
		{
			if (tmp[index + 1] == '\0') break;
			else
			{
				setTabStr(tmp + index + 1, i);
				break;
			}
		}
	}
	XMem::XDELETE_ARRAY(tmp);
	return XTrue;
}
void XTab::clearAllObj()
{
	//将所有控件的控制权交给控件管理器
	for (unsigned int i = 0; i < m_tabObjects.size(); ++i)
	{
		for (unsigned int j = 0; j < m_tabObjects[i].objs.size(); ++j)
		{
			XCtrlManager.addACtrl(m_tabObjects[i].objs[j]);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
			XObjManager.addAObject(m_tabObjects[i].objs[j]);
#endif
		}
		m_ctrlManager.clearAllObject();
		m_tabObjects[i].objs.clear();
		m_tabObjects[i].pos.clear();
		m_tabObjects[i].scale.clear();
	}
}
void XTab::delObjFromTab(XControlBasic *obj, int index)
{
	if (obj == NULL || index >= m_tabObjects.size()) return;

	bool find = false;
	for (int i = 0; i < m_tabObjects[index].objs.size(); ++i)
	{
		if (m_tabObjects[index].objs[i] == obj)
		{
			m_tabObjects[index].objs.erase(m_tabObjects[index].objs.begin() + i);
			m_tabObjects[index].pos.erase(m_tabObjects[index].pos.begin() + i);
			m_tabObjects[index].scale.erase(m_tabObjects[index].scale.begin() + i);
			find = true;
			break;
		}
	}
	if (!find) return;	//如果没有找到指定的，则直接返回
	updateObjState();
	XCtrlManager.addACtrl(obj);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(obj);
#endif
	m_ctrlManager.decreaseAObject(obj);
}
void XTab::addObjToTab(XControlBasic *obj, unsigned int index)	//向一个标签中添加物件
{
	if (obj == NULL || index >= m_tabObjects.size()) return;
	for (int i = 0; i < m_tabObjects[index].objs.size(); ++i)
	{
		if (m_tabObjects[index].objs[i] == obj)
			return;	//防止重复添加
	}

	m_tabObjects[index].objs.push_back(obj);
	m_tabObjects[index].pos.push_back((obj->getPosition() - m_position) / m_scale);
	m_tabObjects[index].scale.push_back(obj->getScale() / m_scale);
	//updateObjState();
	XCtrlManager.decreaseAObject(obj);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(obj);
#endif
	m_ctrlManager.addACtrl(obj);
	//对下面的优化
	if (index == m_curChooseTabIndex)
		m_ctrlManager.setNeedDrawAndInput(true, true, m_ctrlManager.getObjSum() - 1);
	else
		m_ctrlManager.setNeedDrawAndInput(false, false, m_ctrlManager.getObjSum() - 1);
	//if(index == m_curChooseTabIndex)
	//	m_ctrlManager.setNeedDrawAndInput(true,true,obj);
	//else 
	//	m_ctrlManager.setNeedDrawAndInput(false,false,obj);
}
XBool XTab::saveState(TiXmlNode &e)
{
	TiXmlElement elmNode(m_ctrlName.c_str());
	XBool flag = XTrue;
	for (unsigned int i = 0; i < m_tabObjects.size(); ++i)
	{
		if (!m_tabObjects[i].saveState(elmNode)) flag = XFalse;
	}
	if (e.InsertEndChild(elmNode) == NULL) flag = XFalse;
	return flag;
}
XBool XTab::loadState(TiXmlNode *e)
{
	//遍历所有的空间，读取其状态
	TiXmlNode * keyNode = e->FirstChildElement(m_ctrlName.c_str());
	if (keyNode == NULL) return XFalse;
	XBool flag = XTrue;
	for (unsigned int i = 0; i < m_tabObjects.size(); ++i)
	{
		if (!m_tabObjects[i].loadState(keyNode)) flag = XFalse;
	}
	return flag;
}
XBool XTabObject::saveState(TiXmlNode &e)
{
	TiXmlElement elmNode(srcTitle.c_str());
	bool ret = true;
	for (unsigned int i = 0; i < objs.size(); ++i)
	{
		if (objs[i] != NULL &&
			!objs[i]->saveState(elmNode)) ret = false;
	}
	if (e.InsertEndChild(elmNode) == NULL) ret = false;
	return ret;
}
XBool XTabObject::loadState(TiXmlNode *e)
{
	//遍历所有的空间，读取其状态
	TiXmlNode * keyNode = e->FirstChildElement(srcTitle.c_str());
	if (keyNode == NULL) return XFalse;
	bool ret = true;
	for (unsigned int i = 0; i < objs.size(); ++i)
	{
		if (objs[i] != NULL &&
			!objs[i]->loadState(keyNode)) ret = false;
	}
	return ret;
}
#if !WITH_INLINE_FILE
#include "XTab.inl"
#endif
}