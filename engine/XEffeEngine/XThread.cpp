#include "XEffeEngine.h"
#include "XThread.h"

bool _XThread::createAThread(int *,void (* fun)(void *),void *p)			//建立一个线程
{
	HANDLE h = CreateThread(0,0,(LPTHREAD_START_ROUTINE)fun,p,0,NULL);
	if(h == 0) return false;	//连接线程建立失败
	_XThreadInfo *pInfo = createMem<_XThreadInfo>();
	if(pInfo == NULL) return false;
	pInfo->fun = fun;
	pInfo->h = h;
	pInfo->state = STATE_START;
	pInfo->p = p;
	m_threadBuff.push_back(pInfo);
	return true;
}
void _XThread::release()
{//释放资源
	closeAllThread();
	for(int i = 0;i < m_threadBuff.size();++ i)
	{
		XDELETE(m_threadBuff[i]);
	}
	m_threadBuff.clear();
}