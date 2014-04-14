//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMouseRightButtonMenu.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourceManager.h"

_XMouseRightButtonMenu::_XMouseRightButtonMenu()
:m_isInited(XFalse)
,m_menu(NULL)				//按钮的指针
,m_menuSum(0)				//按钮的数量
,m_nowChoose(-1)			//当前选择的值（-1什么也没有选择）//当使用键盘选择菜单时这个值才有效
,m_lastChoose(-1)			//最终选择的值
,m_funChooseChange(NULL)	//改变选值时调用
,m_funChooseOver(NULL)		//最终确定选值时调用
,m_resInfo(NULL)
{
}
_XMouseRightButtonMenu::~_XMouseRightButtonMenu()
{
	release();
}
inline void _XMouseRightButtonMenu::release()	//释放资源
{
	if(!m_isInited) return;
	XDELETE_ARRAY(m_menu);
	_XControlManager::GetInstance().decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	m_isInited = XFalse;
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
_XBool _XMouseRightButtonMenu::init(int menuSum,	//菜单中的物件数量
		const _XVector2& position,	//菜单的位置
		const _XRect& Area,	//菜单按键的响应范围
		const _XMouseRightButtonMenuTexture &tex,	//菜单的贴图
		const _XFontUnicode &font,float captionSize, const _XVector2& textPosition)		//菜单的字体
{
	if(m_isInited) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//空间必须要有一个响应区间，不然会出现除零错误
	if(tex.buttonNormal == NULL || tex.buttonOn == NULL) return XFalse;//这几个贴图是必须要有的
	if(menuSum <= 0) return XFalse;	//没有菜单项的初始化是失败的
	m_position = position;
	m_mouseRect = Area;

	m_size.set(1.0f,1.0f);
	m_menuSum = menuSum;
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//默认初始位置为第一个按钮的正中间

	//设置各个菜单项
	m_menu = createArrayMem<_XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//内存分配失败则直接返回失败

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(m_menu[i].init(_XVector2(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_size.y),m_mouseRect,tex," ",font,captionSize,textPosition) == 0)
		{
			XDELETE_ARRAY(m_menu);
			return XFalse;
		}
		m_menu[i].setSize(m_size);
	//将这些物件从物件管理器中注销掉
		_XControlManager::GetInstance().decreaseAObject(&(m_menu[i]));	//注销这个物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_menu[i]);
#endif
	}
	m_nowChoose = -1;
	m_lastChoose = -1;		//最终选择的值

	m_isVisiable = XFalse;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MOUSERIGHTBUTTONMENU);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XMouseRightButtonMenu::initEx(int menuSum,	//菜单中的物件数量
	const _XVector2& position,	//菜单的位置
	const _XMouseRightButtonMenuTexture &tex,	//菜单的贴图
	const _XFontUnicode &font,float captionSize)		//菜单的字体
{
	if(m_isInited) return XFalse;
	if(tex.m_mouseRect.getHeight() <= 0 || tex.m_mouseRect.getWidth() <= 0) return XFalse;	//空间必须要有一个响应区间，不然会出现除零错误
	if(tex.buttonNormal == NULL || tex.buttonOn == NULL) return XFalse;//这几个贴图是必须要有的
	if(menuSum <= 0) return XFalse;	//没有菜单项的初始化是失败的
	m_position = position;
	m_mouseRect = tex.m_mouseRect;

	m_size.set(1.0f,1.0f);
	m_menuSum = menuSum;
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//默认初始位置为第一个按钮的正中间

	//设置各个菜单项
	m_menu = createArrayMem<_XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//内存分配失败则直接返回失败

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(m_menu[i].initEx(_XVector2(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_size.y),tex," ",font,captionSize) == 0)
		{
			XDELETE_ARRAY(m_menu);
			return XFalse;
		}
		m_menu[i].setSize(m_size);
	//将这些物件从物件管理器中注销掉
		_XControlManager::GetInstance().decreaseAObject(&(m_menu[i]));	//注销这个物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_menu[i]);
#endif
	}
	m_nowChoose = -1;
	m_lastChoose = -1;		//最终选择的值

	m_isVisiable = XFalse;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MOUSERIGHTBUTTONMENU);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XMouseRightButtonMenu::initPlus(const char * path,int menuSum,	//菜单中的物件数量
	const _XFontUnicode &font,float captionSize,_XResourcePosition resoursePosition)		//菜单的字体
{
	if(m_isInited) return XFalse;
	if(path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XBUTTON_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	_XButtonTexture * tex = (_XButtonTexture *)m_resInfo->m_pointer;

	if(tex->m_mouseRect.getHeight() <= 0 || tex->m_mouseRect.getWidth() <= 0) return XFalse;	//空间必须要有一个响应区间，不然会出现除零错误
	if(tex->buttonNormal == NULL || tex->buttonOn == NULL) return XFalse;//这几个贴图是必须要有的
	if(menuSum <= 0) return XFalse;	//没有菜单项的初始化是失败的
	m_position.set(0.0f,0.0f);
	m_mouseRect = tex->m_mouseRect;

	m_size.set(1.0f,1.0f);
	m_menuSum = menuSum;
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//默认初始位置为第一个按钮的正中间

	//设置各个菜单项
	m_menu = createArrayMem<_XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//内存分配失败则直接返回失败

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(!m_menu[i].initEx(_XVector2(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_size.y),(*tex)," ",font,captionSize))
		{
			XDELETE_ARRAY(m_menu);
			return XFalse;
		}
		m_menu[i].setSize(m_size);
	//将这些物件从物件管理器中注销掉
		_XControlManager::GetInstance().decreaseAObject(&(m_menu[i]));	//注销这个物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_menu[i]);
#endif
	}
	m_nowChoose = -1;
	m_lastChoose = -1;		//最终选择的值

	m_isVisiable = XFalse;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MOUSERIGHTBUTTONMENU);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XMouseRightButtonMenu::initWithoutTex(int menuSum,const _XRect& area,
	const _XFontUnicode &font,float captionSize, const _XVector2& textPosition)
{
	if(m_isInited) return XFalse;
	if(menuSum <= 0) return XFalse;	//没有菜单项的初始化是失败的
	m_position.set(0.0f,0.0f);
	m_mouseRect = area;

	m_size.set(1.0f,1.0f);
	m_menuSum = menuSum;
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//默认初始位置为第一个按钮的正中间

	//设置各个菜单项
	m_menu = createArrayMem<_XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//内存分配失败则直接返回失败

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(m_menu[i].initWithoutTex(" ",font,captionSize,m_mouseRect,textPosition) == 0)
		{
			XDELETE_ARRAY(m_menu);
			return XFalse;
		}
		m_menu[i].setSize(m_size);
	//将这些物件从物件管理器中注销掉
		_XControlManager::GetInstance().decreaseAObject(&(m_menu[i]));	//注销这个物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_menu[i]);
#endif
	}
	m_nowChoose = -1;
	m_lastChoose = -1;		//最终选择的值

	m_isVisiable = XFalse;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MOUSERIGHTBUTTONMENU);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XMouseRightButtonMenu::draw()
{
	if(!m_isInited) return;		//没有初始化时不显示
	if(!m_isVisiable) return;	//不可见时不显示
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].draw();
	}
}
void _XMouseRightButtonMenu::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setPosition(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_size.y);
	}
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//默认初始位置为第一个按钮的正中间
	m_nowChoose = -1;
	updateChildPos();
}
void _XMouseRightButtonMenu::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setPosition(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_size.y);
		m_menu[i].setSize(m_size);
	}
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//默认初始位置为第一个按钮的正中间
	m_nowChoose = -1;
	updateChildSize();
}
_XBool _XMouseRightButtonMenu::canGetFocus(float x,float y)
{
	if(!m_isInited) return XFalse;	//如果没有初始化直接退出
	if(!m_isActive) return XFalse;		//没有激活的控件不接收控制
	if(!m_isVisiable) return XFalse;	//如果不可见直接退出
	if(!m_isEnable) return XFalse;		//如果无效则直接退出
	return isInRect(x,y);
}
_XBool _XMouseRightButtonMenu::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited) return XFalse;	//如果没有初始化直接退出
	if(!m_isEnable) return XFalse;		//如果无效则直接退出

	if(!m_isVisiable)
	{//没有显示的时候需要通过检查鼠标右键来检查是否弹出菜单
		if(mouseState == MOUSE_RIGHT_BUTTON_UP)
		{//右键弹起，则显示菜单
			m_isVisiable = XTrue;
			setPosition(x,y);
		}
	}else
	{//显示时对按键动作的响应
		for(int i = 0;i < m_menuSum;++ i)
		{
			m_menu[i].mouseProc(x,y,mouseState);
		}
		if(m_allArea.isInRect(x,y))
		{
			if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_RIGHT_BUTTON_DOWN)
			{
				m_lastChoose = (y - m_position.y - m_mouseRect.top * m_size.y) / (m_mouseRect.getHeight() * m_size.y);
				if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
				//恢复所有按键的状态为普通状态
				for(int i = 0;i < m_menuSum;++ i)
				{
					m_menu[i].setState(BUTTON_STATE_NORMAL);
				}
				m_isVisiable = XFalse;
			}
		//	m_upMousePoint.set(x,y);
			//m_upMousePoint.y = (int)((y - m_objRect.top) / m_objRect.getHeight() * m_size.y) * m_objRect.getHeight() * m_size.y + m_objRect.getHeight() * m_size.y * 0.5;
		}else
		{//区域外的操作
			if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_RIGHT_BUTTON_DOWN)
			{
				m_lastChoose = -1;
				if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
				m_isVisiable = XFalse;
			}
		}
	}
	return XTrue;
}
_XBool _XMouseRightButtonMenu::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited) return XFalse;		//如果没有初始化直接退出
	if(!m_isEnable) return XFalse;		//如果无效则直接退出
	if(!m_isVisiable) return XFalse;	//键盘操作只有在显示的情况下才能显示
	if(keyState == KEY_STATE_UP)
	{
		if(keyOrder == XKEY_UP)
		{//向上键
			-- m_nowChoose;
			if(m_nowChoose < 0) m_nowChoose = m_menuSum -1;
			m_upMousePoint.y = m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * (m_nowChoose + 0.5f)) * m_size.y;

		//	m_upMousePoint.y -= m_objRect.getHeight() * m_size.y;
		//	if(m_upMousePoint.y < m_allArea.top)
		//	{
		//		m_upMousePoint.y += m_allArea.getHeight();
		//	}
			mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
		}else
		if(keyOrder == XKEY_DOWN)
		{//向下键
			++ m_nowChoose;
			if(m_nowChoose >= m_menuSum) m_nowChoose = 0;
			m_upMousePoint.y = m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * (m_nowChoose + 0.5f)) * m_size.y;

		//	m_upMousePoint.y += m_objRect.getHeight() * m_size.y;
		//	if(m_upMousePoint.y > m_allArea.bottom)
		//	{
		//		m_upMousePoint.y -= m_allArea.getHeight();
		//	}
			mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
		}else
		if(keyOrder ==  XKEY_RETURN)
		{//回车键
			for(int i = 0;i < m_menuSum;++ i)
			{
				if(m_menu[i].getState() == BUTTON_STATE_ON)
				{
					m_lastChoose = i;
					if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
					m_isVisiable = XFalse;
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
					if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
					m_isVisiable = XFalse;
					return XTrue;
				}
			}
		}
	}
	return XTrue;
}
_XBool _XMouseRightButtonMenu::setACopy(const _XMouseRightButtonMenu &temp)
{
	if(& temp == this) return XTrue;	//防止自身赋值
	if(!temp.m_isInited) return XFalse;	//如果目标没有初始化则直接返回
	if(m_isInited)	release();//如果自身已经初始化,释放掉相应的资源
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MOUSERIGHTBUTTONMENU);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}
	m_isInited = temp.m_isInited;

	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	//设置各个菜单项
	m_menuSum = temp.m_menuSum;		//按钮的数量
	m_menu = createArrayMem<_XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//内存分配失败则直接返回失败

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(!m_menu[i].setACopy(temp.m_menu[i])) return XFalse;
		//将这些控件从控件管理器中注销掉
		_XControlManager::GetInstance().decreaseAObject(&(m_menu[i]));	//注销这个物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_menu[i]);
#endif
	}

	m_nowChoose = temp.m_nowChoose;		//当前选择的值（-1什么也没有选择）//当使用键盘选择菜单时这个值才有效
	m_lastChoose = temp.m_lastChoose;		//最终选择的值

	m_funChooseChange = temp.m_funChooseChange;	//改变选值时调用
	m_funChooseOver = temp.m_funChooseOver;		//最终确定选值时调用

	m_allArea = temp.m_allArea;		//鼠标动作的相应区域
	m_upMousePoint = temp.m_upMousePoint;	//上次记录的鼠标在范围内的位置

	return XTrue;
}
_XBool _XMouseRightButtonMenu::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
_XVector2 _XMouseRightButtonMenu::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	if(order == 0) ret.set(m_allArea.left,m_allArea.top);else
	if(order == 1) ret.set(m_allArea.right,m_allArea.top);else
	if(order == 2) ret.set(m_allArea.right,m_allArea.bottom);else
	if(order == 3) ret.set(m_allArea.left,m_allArea.bottom);
	return ret;
}