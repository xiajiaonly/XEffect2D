#include "XStdHead.h"
#include "XButtonBar.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE {
	XBool XButtonBar::initWithoutSkin(int height)
	{
		if (m_isInited) return XFalse;
		m_position.reset();
		m_scale.set(1.0f);
		m_insertPoint = 0.0f;
		m_barHeight = height;

		m_isInited = m_isVisible = m_isEnable = m_isActive = XTrue;
		m_curMouseRect.set(m_position,
			m_position + XVec2(m_insertPoint, m_barHeight) * m_scale);

		XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
		return XTrue;
	}
	void XButtonBar::setPosition(const XVec2& p)
	{
		m_position = p;
		int offset = 0;
		for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		{
			it->button->setPosition(m_position.x + offset, m_position.y);
			offset += it->width * m_scale.x;
		}
		m_curMouseRect.set(m_position,
			m_position + XVec2(m_insertPoint, m_barHeight) * m_scale);
	}
	void XButtonBar::setScale(const XVec2& s)
	{
		m_scale = s;
		int offset = 0;
		for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		{
			it->button->setScale(m_scale);
			it->button->setPosition(m_position.x + offset, m_position.y);
			offset += it->width * m_scale.x;
		}
		m_curMouseRect.set(m_position,
			m_position + XVec2(m_insertPoint, m_barHeight) * m_scale);
	}
	void XButtonBar::setTextColor(const XFColor& color)	//设置字体的颜色
	{
		for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		{
			it->button->setTextColor(color);
		}
	}
	void XButtonBar::setColor(const XFColor& c)
	{
		for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		{
			it->button->setColor(c);
		}
	}
	void XButtonBar::setAlpha(float a)
	{
		for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		{
			it->button->setAlpha(a);
		}
	}
	int XButtonBar::getIndexByName(const std::string& name)
	{
		for (unsigned int i = 0; i < m_buttons.size(); ++i)
		{
			if (m_buttons[i].name == name) return i;
		}
		return -1;
	}
	XBool XButtonBar::addAButton(const std::string& name, const std::string& caption)
	{
		//下面检查名字的唯一性
		if (getIndexByName(name) >= 0) return XFalse;
		XButtonBarCore tmp;
		tmp.button = XMem::createMem<XButton>();
		if (tmp.button == NULL) return XFalse;
		tmp.name = name;
		tmp.button->initWithoutSkin(caption.c_str(), XVec2(m_barHeight));
		XCtrlManager.decreaseAObject(tmp.button);	//注销这个物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(tmp.button);
#endif
		tmp.button->setPosition(m_position.x + m_insertPoint * m_scale.x, m_position.y);
		tmp.button->setScale(m_scale);
		m_insertPoint += m_barHeight;
		tmp.width = m_barHeight;
		m_buttons.push_back(tmp);
		m_curMouseRect.set(m_position,
			m_position + XVec2(m_insertPoint, m_barHeight) * m_scale);
		return XTrue;
	}
	XBool XButtonBar::addAButton(const std::string& name, int width, const std::string& caption)
	{
		//下面检查名字的唯一性
		if (getIndexByName(name) >= 0) return XFalse;
		XButtonBarCore tmp;
		tmp.button = XMem::createMem<XButton>();
		if (tmp.button == NULL) return XFalse;
		tmp.name = name;
		tmp.button->initWithoutSkin(caption.c_str(), XVec2(width, m_barHeight));
		XCtrlManager.decreaseAObject(tmp.button);	//注销这个物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(tmp.button);
#endif
		tmp.button->setPosition(m_position.x + m_insertPoint * m_scale.x, m_position.y);
		tmp.button->setScale(m_scale);
		m_insertPoint += width;
		tmp.width = width;
		m_buttons.push_back(tmp);
		m_curMouseRect.set(m_position,
			m_position + XVec2(m_insertPoint, m_barHeight) * m_scale);
		return XTrue;
	}
	void XButtonBar::draw()
	{
		if (!m_isInited ||	//如果没有初始化直接退出
			!m_isVisible ||	//如果不可见直接退出
			m_buttons.size() <= 0) return;
		for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		{
			it->button->draw();
		}
	}
	void XButtonBar::drawUp()
	{
		if (!m_isInited ||	//如果没有初始化直接退出
			!m_isVisible ||	//如果不可见直接退出
			m_buttons.size() <= 0) return;
		for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		{
			it->button->drawUp();
		}
	}
	void XButtonBar::update(float stepTime)
	{
		for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		{
			it->button->update(stepTime);
		}
	}
	void XButtonBar::release()
	{
		if (!m_isInited) return;
		for (unsigned int i = 0; i < m_buttons.size(); ++i)
		{
			XMem::XDELETE(m_buttons[i].button);
		}
		m_buttons.clear();
		XCtrlManager.decreaseAObject(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(this);
#endif

		m_isInited = XFalse;
	}
	void XButtonBar::disable()
	{
		for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		{
			it->button->disable();
		}
	}
	void XButtonBar::enable()
	{
		for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		{
			if (it->isEnable) it->button->enable();
		}
	}
	XBool XButtonBar::mouseProc(const XVec2& p, XMouseState mouseState)		//对于鼠标动作的响应函数
	{
		if (!m_isInited ||	//如果没有初始化直接退出
			!m_isActive ||		//没有激活的控件不接收控制
			!m_isVisible || m_isSilent) return XFalse;
		XBool ret = XFalse;
		for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		{
			if(it->button->mouseProc(p,mouseState)) ret = XTrue;
		}
		return ret;
	}
XBool XButtonBar::keyboardProc(int keyOrder,XKeyState keyState)			//返回是否触发按键动作
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if(m_isSilent) return XFalse;
	XBool ret = XFalse;
	for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
	{
		if(it->button->keyboardProc(keyOrder,keyState)) ret = XTrue;
	}
	return ret;
}
#if !WITH_INLINE_FILE
#include "XButtonBar.inl"
#endif
}