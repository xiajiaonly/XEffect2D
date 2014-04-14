#ifndef _JIA_XTHREAD_
#define _JIA_XTHREAD_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:        2011.4.9
//--------------------------------
//这是一个统一对线程进行管理的类，目的在于引擎中的线程能被统一管理
//这个设计尚未完成

#include "windows.h"
#include <vector>

//多线程的状态
enum _XThreadState
{
	STATE_BEFORE_START,		//线程尚未开始
	STATE_START,			//线程已经运行
	STATE_SET_TO_END,		//线程被设置为结束
	STATE_END,				//线程结束
};
inline void waitThreadEnd(_XThreadState &state)
{
	if(state != STATE_START) return;
	state = STATE_SET_TO_END;
	while(state != STATE_END)
	{
		Sleep(1);
	}
}

typedef DWORD pthread_t;    

struct _XThreadInfo
{
	_XThreadState state;	//线程的状态
	HANDLE h;				//线程的句柄
	void (* fun)(void *);	//线程中调用的函数
	void *p;				//线程函数的参数
	_XThreadInfo()
		:state(STATE_BEFORE_START)
		,h(0)
		,fun(NULL)
		,p(NULL)
	{}
};
//对于短线程的调用机制，这个类会存在问题，需要修正
//短线程机制会不停的建立线程，长时间运行，将会导致这个类的buff越来越大
class _XThread
{
protected:
    _XThread()
	{}
    _XThread(const _XThread&);
	_XThread &operator= (const _XThread&);
    virtual ~_XThread(){release();}
public:
    static _XThread& GetInstance()
	{
		static _XThread m_instance;
		return m_instance;
	}
private:
	std::vector<_XThreadInfo *> m_threadBuff;	//线程的队列
	void release();//释放资源
public:
	//id,这个线程的编号，fun线程调用的函数，p线程传递的参数
	bool createAThread(int *id,void (* fun)(void *),void *p);			//建立一个线程
	bool closeAThread(int id)			//关闭一个线程
	{
		if(id < 0 || id >= m_threadBuff.size() || m_threadBuff[id]->state != STATE_START) return false;	
		if(TerminateThread(m_threadBuff[id]->h,0))
		{//线程结束
			m_threadBuff[id]->state = STATE_END;
			return true;
		}else
		{//线程
			return false;
		}
	}
	bool closeAllThread()			//关闭所有线程
	{
		return true;
	}
	_XThreadState getThreadState();	//获取一个线程的状态
	bool suspendAThread();			//挂起一个线程
	bool resumeAThread();			//恢复一个线程
	bool suspendAllThread();		//挂起所有线程
	bool resumeAllThread();			//恢复所有线程

	void update();	//更新线程的状态
};

#endif