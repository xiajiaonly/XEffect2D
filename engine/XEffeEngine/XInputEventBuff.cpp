#include "XStdHead.h"
#include "XInputEventBuff.h"
namespace XE{
int XInputEventBuff::getEventProcIndex(const XEventBase *proc)
{
	for(std::vector<XEventBase *>::iterator it = m_eventProcs.begin();it != m_eventProcs.end();++ it)
	{
		if(*it == proc) return it - m_eventProcs.begin();
	}
	return -1;
}
bool XInputEventBuff::desEventProcByClass(void *pClass)
{
	//if(pClass == NULL) return false;
	//for(auto it = m_eventProcs.begin();it != m_eventProcs.end();++ it)
	//{
	//	if(it->isEvent(pClass))
	//	{
	//		m_eventProcs.erase(it);
	//		return true;
	//	}
	//}
	return false;
}
bool XInputEventBuff::isInBuff(const void *ep)
{
	if(ep == NULL) return false;
	for(std::vector<XEventBase *>::iterator it = m_eventProcs.begin();it != m_eventProcs.end();++ it)
	{
		if((*it)->isEvent(ep)) return true;
	}
	return false;
}
void XInputEventBuff::doProc(const XInputEvent &e)
{
	for(std::vector<XEventBase *>::iterator it = m_eventProcs.begin();it != m_eventProcs.end();++ it)
	{
		(*it)->procEvent(e);
	}
}
}