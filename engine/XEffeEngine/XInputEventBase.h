#ifndef _JIA_XINPUTEVENTBASE_
#define _JIA_XINPUTEVENTBASE_
#include "XCommonDefine.h"
namespace XE {
struct XInputEvent;
class XEventBase
{
public:
	virtual bool procEvent(const XInputEvent &e) = 0;
	virtual bool isEvent(const void *ep) = 0;		//判断当前事件是否为pClass的事件
};
template <class T>
class XEvent :public XEventBase
{
public:
	XEvent(T* obj, void (T::*fun)(const XInputEvent &e))
		:m_pClass(obj)
		, m_fun(fun)
	{}
	~XEvent() {}
	virtual bool procEvent(const XInputEvent &e)
	{
		if (m_pClass)
		{
			(m_pClass->*m_fun)(e);
			return true;
		}
		return false;
	}
	virtual bool isEvent(const void *ep)
	{
		return m_pClass == ((XEvent *)ep)->m_pClass &&
			m_fun == ((XEvent *)ep)->m_fun;
	}
protected:
	T *m_pClass;
	void (T::*m_fun)(const XInputEvent &e);
private:
	XEvent();
};
}
#endif