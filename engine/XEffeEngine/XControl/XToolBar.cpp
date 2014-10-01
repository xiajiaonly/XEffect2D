#include "XToolBar.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

void _XToolBar::setPosition(float x,float y)
{
	m_position.set(x,y);
	float offset = 10.0f;
	for(int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setPosition(m_position.x + offset * m_size.x,m_position.y);
		offset += m_objects[i].width;
	}
	m_nowMouseRect.set(m_position.x,m_position.y,
		m_position.x + 10.0f * m_size.x,m_position.y + m_height * m_size.y);
}
void _XToolBar::setSize(float x,float y)
{
	m_size.set(x,y);
	float offset = 10.0f;
	for(int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setSize(m_size * m_objects[i].size);
		m_objects[i].obj->setPosition(m_position.x + offset * m_size.x,m_position.y);
		offset += m_objects[i].width;
	}
	m_nowMouseRect.set(m_position.x,m_position.y,
		m_position.x + 10.0f * m_size.x,m_position.y + m_height * m_size.y);
}
_XBool _XToolBar::keyboardProc(int keyOrder,_XKeyState keyState)
{
	//if(!m_isInited ||	//如果没有初始化直接退出
	//	!m_isActive ||		//没有激活的控件不接收控制
	//	!m_isVisible ||	//如果不可见直接退出
	//	!m_isEnable) return XFalse;		//如果无效则直接退出
	//for(int i = 0;i < m_objects.size();++ i)
	//{
	//	m_objects[i].obj->keyboardProc(keyOrder,keyState);
	//}
	return XFalse;
}
_XBool _XToolBar::mouseProc(float x,float y,_XMouseState mouseState)		//对于鼠标动作的响应函数
{
	//for(int i = 0;i < m_objects.size();++ i)
	//{
	//	m_objects[i].obj->mouseProc(x,y,mouseState);
	//}
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible) return XFalse; 	//如果不可见直接退出
	switch(mouseState)
	{
	case MOUSE_LEFT_BUTTON_DOWN:
	case MOUSE_LEFT_BUTTON_DCLICK:
		if(m_nowMouseRect.isInRect(x,y))
		{
			m_isMouseDown = XTrue;
			m_oldMousePosition.set(x,y);
			return XTrue;
		}
		break;
	case MOUSE_LEFT_BUTTON_UP:
		if(m_isMouseDown) m_isMouseDown = XFalse;
		break;
	case MOUSE_MOVE:
		if(m_isMouseDown)
		{//发生拖动事件
			_XVector2 tmp = _XVector2(x,y) - m_oldMousePosition;
			setPosition(m_position.x + tmp.x,m_position.y + tmp.y);
			m_oldMousePosition.set(x,y);
			return XTrue;
		}
		break;
	}
	return XFalse;
}
void _XToolBar::addAObject(_XControlBasic *obj)
{
	if(obj == NULL) return;
	if(getIndexByP(obj) >= 0) return;//这里需要检查唯一性
	_XToolBarUnit tmp;
	tmp.enable = obj->getEnable();
	tmp.size = obj->getSize();
	tmp.obj = obj;
	tmp.width = obj->getBox(1).x - obj->getBox(0).x;
	tmp.obj->setSize(m_size * tmp.size);
	m_objects.push_back(tmp);
	obj->setPosition(m_position.x + m_insertPoint * m_size.x,m_position.y);
	m_insertPoint += tmp.width;
	//这里注销指定控件
//	_XCtrlManger.decreaseAObject(obj);	//注销这个物件
//#if WITH_OBJECT_MANAGER
//	_XObjManger.decreaseAObject(obj);
//#endif
}
int _XToolBar::getIndexByP(const _XControlBasic *obj)
{
	if(obj == NULL) return -1;
	for(int i = 0;i < m_objects.size();++ i)
	{
		if(m_objects[i].obj == obj) return i;
	}
	return -1;
}
void _XToolBar::releaseMem()
{
	for(int i = 0;i < m_objects.size();++ i)
	{
		XDELETE(m_objects[i].obj);
	}
	m_objects.clear();
}
void _XToolBar::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	drawFillBoxEx(m_position,_XVector2(10.0f * m_size.x,m_height * m_size.y),0.75f,0.75f,0.75f,true);
	drawLine(m_position.x + 3.0f * m_size.x,m_position.y,m_position.x + 3.0f * m_size.x,m_position.y + m_height * m_size.y,
		1.0f,0.0f,0.0f,0.0f,1.0f * m_size.y);
	drawLine(m_position.x + 7.0f * m_size.x,m_position.y,m_position.x + 7.0f * m_size.x,m_position.y + m_height * m_size.y,
		1.0f,0.0f,0.0f,0.0f,1.0f * m_size.y);
	//这里需要描绘所有子物件
	//for(int i = 0;i < m_objects.size();++ i)
	//{
	//	m_objects[i].obj->draw();
	//}
}
void _XToolBar::drawUp()
{
	//if(!m_isInited ||	//如果没有初始化直接退出
	//	!m_isVisible) return;	//如果不可见直接退出
	//for(int i = 0;i < m_objects.size();++ i)
	//{
	//	m_objects[i].obj->drawUp();
	//}
}
_XBool _XToolBar::initWithoutTex(int height)
{//工具栏的像素高度
	if(m_isInited) return XFalse;
	m_position.set(0.0f,0.0f);
	m_size.set(1.0f,1.0f);
	m_height = height;
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_nowMouseRect.set(m_position.x,m_position.y,
		m_position.x + 10.0f * m_size.x,m_position.y + m_height * m_size.y);

	_XCtrlManger.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XToolBar::release()
{
	if(!m_isInited) return;
	_XCtrlManger.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	m_isInited = XFalse;
}
void _XToolBar::setVisible()
{
	m_isVisible = XTrue;
	m_comment.setVisible();
	for(int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setVisible();
	}
	updateChildVisible();
}
void _XToolBar::disVisible()
{
	m_isVisible = XFalse;
	m_isBeChoose = XFalse;
	m_comment.disVisible();
	for(int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->disVisible();
	}
	updateChildVisible();
}