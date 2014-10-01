#include "XButtonBar.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
_XBool _XButtonBar::initWithoutTex(int height)
{
	if(m_isInited) return XFalse;
	m_position.set(0.0f,0.0f);
	m_size.set(1.0f,1.f);
	m_insertPoint = 0.0f;
	m_barHeight = height;

	m_isInited = XTrue;
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_nowMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_size.x,
		m_position.y + m_barHeight * m_size.y);

	_XCtrlManger.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	return XTrue;
}
void _XButtonBar::setPosition(float x,float y)
{
	m_position.set(x,y);
	int offset = 0;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setPosition(m_position.x + offset,m_position.y);
		offset += m_buttons[i].width * m_size.x;
	}
	m_nowMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_size.x,
		m_position.y + m_barHeight * m_size.y);
}
void _XButtonBar::setSize(float x,float y)
{
	m_size.set(x,y);
	int offset = 0;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setSize(m_size);
		m_buttons[i].button->setPosition(m_position.x + offset,m_position.y);
		offset += m_buttons[i].width * m_size.x;
	}
	m_nowMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_size.x,
		m_position.y + m_barHeight * m_size.y);
}
void _XButtonBar::setTextColor(const _XFColor& color)	//设置字体的颜色
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setTextColor(color);
	}
}
void _XButtonBar::setColor(float r,float g,float b,float a)
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setColor(r,g,b,a);
	}
}
void _XButtonBar::setAlpha(float a)
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setAlpha(a);
	}
}
int _XButtonBar::getIndexByName(const std::string &name)
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		if(m_buttons[i].name == name) return i;
	}
	return -1;
}
_XBool _XButtonBar::addAButton(const std::string &name,const std::string &caption)
{
	//下面检查名字的唯一性
	if(getIndexByName(name) >= 0) return XFalse;
	_XButtonBarCore tmp;
	tmp.button = createMem<_XButton>();
	if(tmp.button == NULL) return XFalse;
	tmp.name = name;
	tmp.button->initWithoutTex(caption.c_str(),_XVector2(m_barHeight,m_barHeight));
	_XCtrlManger.decreaseAObject(tmp.button);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(tmp.button);
#endif
	tmp.button->setPosition(m_position.x + m_insertPoint * m_size.x,m_position.y);
	tmp.button->setSize(m_size);
	m_insertPoint += m_barHeight;
	tmp.width = m_barHeight;
	m_buttons.push_back(tmp);
	m_nowMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_size.x,
		m_position.y + m_barHeight * m_size.y);
	return XTrue;
}
_XBool _XButtonBar::addAButton(const std::string& name,int width,const std::string &caption)
{
	//下面检查名字的唯一性
	if(getIndexByName(name) >= 0) return XFalse;
	_XButtonBarCore tmp;
	tmp.button = createMem<_XButton>();
	if(tmp.button == NULL) return XFalse;
	tmp.name = name;
	tmp.button->initWithoutTex(caption.c_str(),_XVector2(width,m_barHeight));
	_XCtrlManger.decreaseAObject(tmp.button);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(tmp.button);
#endif
	tmp.button->setPosition(m_position.x + m_insertPoint * m_size.x,m_position.y);
	tmp.button->setSize(m_size);
	m_insertPoint += width;
	tmp.width = width;
	m_buttons.push_back(tmp);
	m_nowMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_size.x,
		m_position.y + m_barHeight * m_size.y);
	return XTrue;
}
void _XButtonBar::draw()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible ||	//如果不可见直接退出
		m_buttons.size() <= 0) return;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->draw();
	}
}
void _XButtonBar::drawUp()
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible ||	//如果不可见直接退出
		m_buttons.size() <= 0) return;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->drawUp();
	}
}
void _XButtonBar::update(int stepTime)
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->update(stepTime);
	}
}
void _XButtonBar::release()
{
	if(!m_isInited) return;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		XDELETE(m_buttons[i].button);
	}
	m_buttons.clear();
	_XCtrlManger.decreaseAObject(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif

	m_isInited = XFalse;
}
void _XButtonBar::disable()
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->disable();
	}
}
void _XButtonBar::enable()
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		if(m_buttons[i].isEnable) m_buttons[i].button->enable();
	}
}
_XBool _XButtonBar::mouseProc(float x,float y,_XMouseState mouseState)		//对于鼠标动作的响应函数
{
	if(!m_isInited ||	//如果没有初始化直接退出
	!m_isActive ||		//没有激活的控件不接收控制
	!m_isVisible) return XFalse; 	//如果不可见直接退出
	_XBool ret = XFalse;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		if(m_buttons[i].button->mouseProc(x,y,mouseState)) ret = XTrue;
	}
	return ret;
}
_XBool _XButtonBar::keyboardProc(int keyOrder,_XKeyState keyState)			//返回是否触发按键动作
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	_XBool ret = XFalse;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		if(m_buttons[i].button->keyboardProc(keyOrder,keyState)) ret = XTrue;
	}
	return ret;
}