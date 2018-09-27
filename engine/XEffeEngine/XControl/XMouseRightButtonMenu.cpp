#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMouseRightButtonMenu.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourceManager.h"
namespace XE{
XMouseRightButtonMenu::XMouseRightButtonMenu()
	:m_isInited(XFalse)
	, m_menu(NULL)				//按钮的指针
	, m_menuSum(0)				//按钮的数量
	, m_curChoose(-1)			//当前选择的值（-1什么也没有选择）//当使用键盘选择菜单时这个值才有效
	, m_lastChoose(-1)			//最终选择的值
	//,m_funChooseChange(NULL)	//改变选值时调用
	//,m_funChooseOver(NULL)		//最终确定选值时调用
	, m_resInfo(NULL)
{
	m_ctrlType = CTRL_OBJ_MOUSERIGHTBUTTONMENU;
}
inline void XMouseRightButtonMenu::release()	//释放资源
{
	if(!m_isInited) return;
	XMem::XDELETE_ARRAY(m_menu);
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = XFalse;
	if(m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
XBool XMouseRightButtonMenu::init(int menuSum,	//菜单中的物件数量
		const XVec2& position,	//菜单的位置
		const XRect& Area,	//菜单按键的响应范围
		const XMouseRightButtonMenuSkin &tex,	//菜单的贴图
		const XFontUnicode& font,float captionSize, const XVec2& textPosition)		//菜单的字体
{
	if(m_isInited) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//空间必须要有一个响应区间，不然会出现除零错误
	if(tex.buttonNormal == NULL || tex.buttonOn == NULL) return XFalse;//这几个贴图是必须要有的
	if(menuSum <= 0) return XFalse;	//没有菜单项的初始化是失败的
	m_position = position;
	m_mouseRect = Area;

	m_scale.set(1.0f);
	m_menuSum = menuSum;
	m_allArea.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + XVec2(m_mouseRect.right, m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_scale);
	m_upMousePoint.set(m_position + m_mouseRect.getCenter() * m_scale);	//默认初始位置为第一个按钮的正中间

	//设置各个菜单项
	m_menu = XMem::createArrayMem<XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//内存分配失败则直接返回失败

	for(int i = 0;i < m_menuSum;++ i)
	{
		if (!m_menu[i].init(XVec2(m_position.x, m_position.y + m_mouseRect.getHeight() * i * m_scale.y),
			m_mouseRect, tex, " ", font, captionSize, textPosition))
		{
			XMem::XDELETE_ARRAY(m_menu);
			return XFalse;
		}
		m_menu[i].setScale(m_scale);
	//将这些物件从物件管理器中注销掉
		XCtrlManager.decreaseAObject(&(m_menu[i]));	//注销这个物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_menu[i]);
#endif
	}
	m_curChoose = -1;
	m_lastChoose = -1;		//最终选择的值

	m_isVisible = XFalse;
	m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XMouseRightButtonMenu::initWithoutSkin(int menuSum,const XRect& area,
	const XFontUnicode& font,float captionSize, const XVec2& textPosition)
{
	if(m_isInited ||
		menuSum <= 0) return XFalse;	//没有菜单项的初始化是失败的
	m_position.reset();
	m_mouseRect = area;

	m_scale.set(1.0f);
	m_menuSum = menuSum;
	m_allArea.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + XVec2(m_mouseRect.right,m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_scale);
	m_upMousePoint.set(m_position + m_mouseRect.getCenter() * m_scale);	//默认初始位置为第一个按钮的正中间

	//设置各个菜单项
	m_menu = XMem::createArrayMem<XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//内存分配失败则直接返回失败

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(!m_menu[i].initWithoutSkin(" ",font,captionSize,m_mouseRect,textPosition))
		{
			XMem::XDELETE_ARRAY(m_menu);
			return XFalse;
		}
		m_menu[i].setScale(m_scale);
	//将这些物件从物件管理器中注销掉
		XCtrlManager.decreaseAObject(&(m_menu[i]));	//注销这个物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_menu[i]);
#endif
	}
	m_curChoose = -1;
	m_lastChoose = -1;		//最终选择的值

	m_isVisible = XFalse;
	m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void XMouseRightButtonMenu::setPosition(const XVec2& p)
{
	m_position = p;
	m_allArea.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + XVec2(m_mouseRect.right,m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_scale);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setPosition(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_scale.y);
	}
	m_upMousePoint.set(m_position + m_mouseRect.getCenter() * m_scale);	//默认初始位置为第一个按钮的正中间
	m_curChoose = -1;
	updateChildPos();
}
void XMouseRightButtonMenu::setScale(const XVec2& s)
{
	if(s.x <= 0 || s.y <= 0) return;
	m_scale = s;
	m_allArea.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + XVec2(m_mouseRect.right,m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_scale);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setPosition(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_scale.y);
		m_menu[i].setScale(m_scale);
	}
	m_upMousePoint.set(m_position + m_mouseRect.getCenter() * m_scale);	//默认初始位置为第一个按钮的正中间
	m_curChoose = -1;
	updateChildScale();
}
XBool XMouseRightButtonMenu::mouseProc(const XVec2& p,XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	//if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if(m_isSilent) return XFalse;

	if(!m_isVisible)
	{//没有显示的时候需要通过检查鼠标右键来检查是否弹出菜单
		if(mouseState == MOUSE_RIGHT_BUTTON_UP)
		{//右键弹起，则显示菜单
			m_isVisible = XTrue;
			setPosition(p);
			//菜单出现
			if(m_withAction)
			{//下面定义进入的动作
				m_isInAction = XTrue;
				m_isDraw = true;
				m_actionMoveData.set(0.0f,1.0f,0.005f);
				//初始化各个按钮的状态
				for(int i = 0;i < m_menuSum;++ i)
				{
					m_menu[i].setWithAction(XFalse);
					m_menu[i].setAlpha(0.0f);
				}
			}
		}
	}else
	{//显示时对按键动作的响应
		for(int i = 0;i < m_menuSum;++ i)
		{
			m_menu[i].mouseProc(p,mouseState);
		}
		if(m_allArea.isInRect(p))
		{
			if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK || mouseState == MOUSE_RIGHT_BUTTON_DOWN)
			{
				m_lastChoose = (p.y - m_position.y - m_mouseRect.top * m_scale.y) / (m_mouseRect.getHeight() * m_scale.y);
				//if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MRBM_CHOOSE_OVER);
				else XCtrlManager.eventProc(m_objectID,MRBM_CHOOSE_OVER);
				//恢复所有按键的状态为普通状态
				for(int i = 0;i < m_menuSum;++ i)
				{
					if(m_menu[i].getEnable())
						m_menu[i].setState(BUTTON_STATE_NORMAL);
				}
				//m_isVisible = XFalse;
				if(m_withAction)//下面定义进入的动作
					playOutAction();
				return XTrue;
			}
		//	m_upMousePoint.set(x,y);
			//m_upMousePoint.y = (int)((y - m_objRect.top) / m_objRect.getHeight() * m_scale.y) * m_objRect.getHeight() * m_scale.y + m_objRect.getHeight() * m_scale.y * 0.5;
		}else
		{//区域外的操作
			if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK || mouseState == MOUSE_RIGHT_BUTTON_DOWN)
			{
				m_lastChoose = -1;
				//if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MRBM_CHOOSE_OVER);
				else XCtrlManager.eventProc(m_objectID,MRBM_CHOOSE_OVER);
				//m_isVisible = XFalse;
				//菜单消失
				if(m_withAction)//下面定义进入的动作
					playOutAction();
				return XTrue;
			}
		}
	}
	return XFalse;
}
void XMouseRightButtonMenu::playOutAction()
{//这里需要互斥，如果已经进入则不需要重复进入
	if(!m_isDraw) return;	//如果已经开始退出，则不重复退出
	m_isInAction = XTrue;
	m_isDraw = false;
	m_actionMoveData.set(0.0f,1.0f,0.005f);
	//初始化各个按钮的状态
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setWithAction(XFalse);
		m_menu[i].setAlpha(1.0f);
	}
}
XBool XMouseRightButtonMenu::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||		//如果没有初始化直接退出
		!m_isEnable ||		//如果无效则直接退出
		!m_isVisible) return XFalse;	//键盘操作只有在显示的情况下才能显示
	if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if(m_isSilent) return XFalse;
	if(keyState == KEY_STATE_UP)
	{
		if(keyOrder == XKEY_UP)
		{//向上键
			-- m_curChoose;
			if(m_curChoose < 0) m_curChoose = m_menuSum -1;
			m_upMousePoint.y = m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * (m_curChoose + 0.5f)) * m_scale.y;

		//	m_upMousePoint.y -= m_objRect.getHeight() * m_scale.y;
		//	if(m_upMousePoint.y < m_allArea.top)
		//	{
		//		m_upMousePoint.y += m_allArea.getHeight();
		//	}
			mouseProc(m_upMousePoint,MOUSE_MOVE);
		}else
		if(keyOrder == XKEY_DOWN)
		{//向下键
			++ m_curChoose;
			if(m_curChoose >= m_menuSum) m_curChoose = 0;
			m_upMousePoint.y = m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * (m_curChoose + 0.5f)) * m_scale.y;

		//	m_upMousePoint.y += m_objRect.getHeight() * m_scale.y;
		//	if(m_upMousePoint.y > m_allArea.bottom)
		//	{
		//		m_upMousePoint.y -= m_allArea.getHeight();
		//	}
			mouseProc(m_upMousePoint,MOUSE_MOVE);
		}else
		if(keyOrder ==  XKEY_RETURN)
		{//回车键
			for(int i = 0;i < m_menuSum;++ i)
			{
				if(m_menu[i].getState() == BUTTON_STATE_ON)
				{
					m_lastChoose = i;
					//if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
					if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MRBM_CHOOSE_OVER);
					else XCtrlManager.eventProc(m_objectID,MRBM_CHOOSE_OVER);
					//m_isVisible = XFalse;
					if(m_withAction)//下面定义进入的动作
						playOutAction();

					return XTrue;
				}
			}
		}else
		{
			for(int i = 0;i < m_menuSum;++ i)
			{
				if(keyOrder == m_menu[i].getHotKey() && m_menu[i].getState() != BUTTON_STATE_DISABLE)
				{//如果按键与菜单的快捷键相同，而且菜单的状态不是无效
					m_lastChoose = i;
					//if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
					if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MRBM_CHOOSE_OVER);
					else XCtrlManager.eventProc(m_objectID,MRBM_CHOOSE_OVER);
					//m_isVisible = XFalse;
					if(m_withAction)//下面定义进入的动作
						playOutAction();
					
					return XTrue;
				}
			}
		}
	}
	return XFalse;
}
XBool XMouseRightButtonMenu::setACopy(const XMouseRightButtonMenu &temp)
{
	if(& temp == this) return XTrue;	//防止自身赋值
	if(!temp.m_isInited) return XFalse;	//如果目标没有初始化则直接返回
	if(m_isInited)	release();//如果自身已经初始化,释放掉相应的资源
	if(!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}
	m_isInited = temp.m_isInited;

	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	//设置各个菜单项
	m_menuSum = temp.m_menuSum;		//按钮的数量
	m_menu = XMem::createArrayMem<XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//内存分配失败则直接返回失败

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(!m_menu[i].setACopy(temp.m_menu[i])) return XFalse;
		//将这些控件从控件管理器中注销掉
		XCtrlManager.decreaseAObject(&(m_menu[i]));	//注销这个物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_menu[i]);
#endif
	}

	m_curChoose = temp.m_curChoose;		//当前选择的值（-1什么也没有选择）//当使用键盘选择菜单时这个值才有效
	m_lastChoose = temp.m_lastChoose;		//最终选择的值
	m_isDraw = temp.m_isDraw;	//是否是菜单出现
	m_actionMoveData = temp.m_actionMoveData;
//	m_funChooseChange = temp.m_funChooseChange;	//改变选值时调用
//	m_funChooseOver = temp.m_funChooseOver;		//最终确定选值时调用

	m_allArea = temp.m_allArea;		//鼠标动作的相应区域
	m_upMousePoint = temp.m_upMousePoint;	//上次记录的鼠标在范围内的位置

	return XTrue;
}
#if !WITH_INLINE_FILE
#include "XMouseRightButtonMenu.inl"
#endif
}