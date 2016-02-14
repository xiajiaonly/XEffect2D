#include "XStdHead.h"
#include "XThread.h"
namespace XE{
void waitThreadEnd(XThreadState &state)
{
	if(state != STATE_START) return;
	state = STATE_SET_TO_END;
	while(state != STATE_END)
	{
		Sleep(1);
	}
}
bool XThreadManager::createAThread(int *,void (* fun)(void *),void *p)			//建立一个线程
{
	HANDLE h = CreateThread(0,0,(LPTHREAD_START_ROUTINE)fun,p,0,NULL);
	if(h == 0) return false;	//连接线程建立失败
	XThreadInfo *pInfo = XMem::createMem<XThreadInfo>();
	if(pInfo == NULL) return false;
	pInfo->fun = fun;
	pInfo->h = h;
	pInfo->state = STATE_START;
	pInfo->p = p;
	m_threadBuff.push_back(pInfo);
	return true;
}
void XThreadManager::release()
{//释放资源
	closeAllThread();
	for(unsigned int i = 0;i < m_threadBuff.size();++ i)
	{
		XMem::XDELETE(m_threadBuff[i]);
	}
	m_threadBuff.clear();
}
DWORD WINAPI XThread::threadFun(LPVOID hParameter)
{
	XThread &p = *(XThread *)hParameter;
	p.m_threadState = STATE_START;
	int curTime = XTime::getCurrentTicks();
	int oldTime = curTime;
	int timer = 0;
	int frameSum = 0;
	while(p.m_threadState != STATE_SET_TO_END)
	{
		++ frameSum;
		curTime = XTime::getCurrentTicks();
		timer += curTime - oldTime;
		oldTime = curTime;
		if(timer >= 10000)
		{
			p.m_fps = frameSum / (timer * 0.001f);
			frameSum = 0;
			timer = 0;
		}
		//if(p.m_threadState == STATE_SET_TO_END) break;
		if(p.m_cbFun != NULL) p.m_cbFun(p.m_pClass);
		Sleep(1);
	}
	p.m_threadState = STATE_END;
	return 1;
}
bool XThread::setup(void (* cbFun)(void *),void *pClass)
{
	if(m_isSetup) return false;
	m_cbFun = cbFun;
	m_pClass = pClass;
	//下面开启线程
	m_threadState = STATE_BEFORE_START;
	if(CreateThread(NULL,NULL,threadFun,this,0,NULL) <= 0) return false;

	m_isSetup = true;
	return true;
}
}