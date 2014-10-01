#ifndef _JIA_XCRITICAL_
#define _JIA_XCRITICAL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "XOSDefine.h"

#ifdef XEE_OS_WINDOWS
#include "windows.h"
//下面的类用于线程互斥
class _XCritical 
{ 
public: 
	_XCritical() {InitializeCriticalSection(&m_sec);}
	~_XCritical() {DeleteCriticalSection(&m_sec);}
    void Lock() {EnterCriticalSection(&m_sec);}
    void Unlock() {LeaveCriticalSection(&m_sec);}
private: 
    CRITICAL_SECTION m_sec; 
};
typedef _XCritical pthread_mutex_t;
inline void pthread_mutex_init(_XCritical &,int){;}	//do nothing
inline void pthread_mutex_lock(_XCritical &temp) {temp.Lock();}
inline void pthread_mutex_unlock(_XCritical &temp) {temp.Unlock();}

#endif
#endif