#ifndef _JIA_XINPUTEVENTBUFF_
#define _JIA_XINPUTEVENTBUFF_
#include "XInputEventBase.h"
#include <vector>
namespace XE{
class XInputEventBuff
{
private:
	std::vector<XEventBase *> m_eventProcs;
	int getEventProcIndex(const XEventBase *proc);
public:
	template <class T>
	bool addEventProc(T *pClass, void(T::*fun)(const XInputEvent &))
	{
		if (pClass == NULL || fun == NULL) return false;
		XEvent<T> *p = new XEvent<T>(pClass, fun);
		if (p == NULL) return false;
		if (isInBuff(p))
		{
			delete p;
			return true;
		}
		m_eventProcs.push_back(p);
		return true;
	}
	template <class T>
	bool desEventProc(T *pClass, void(T::*fun)(const XInputEvent &))
	{
		if (pClass == NULL || fun == NULL) return false;
		XEvent<T> e(pClass, fun);
		for (std::vector<XEventBase*>::iterator it = m_eventProcs.begin(); it != m_eventProcs.end(); ++it)
		{
			if ((*it)->isEvent(&e))
			{
				delete (*it);
				m_eventProcs.erase(it);
				return true;
			}
		}
		return false;
	}
	bool desEventProcByClass(void *pClass);	//ипн╢й╣ож

	bool isInBuff(const void *ep);
	void doProc(const XInputEvent &e);
};
}
#endif