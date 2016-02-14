#ifndef _JIA_XCRITICAL_
#define _JIA_XCRITICAL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "XOSDefine.h"
namespace XE{
#ifdef XEE_OS_WINDOWS
//下面的类用于线程互斥
class XCritical 
{ 
public: 
	XCritical() {InitializeCriticalSection(&m_sec);}
	~XCritical() {DeleteCriticalSection(&m_sec);}
    void Lock() {EnterCriticalSection(&m_sec);}
    void Unlock() {LeaveCriticalSection(&m_sec);}
private: 
    CRITICAL_SECTION m_sec; 
};
//下面是为了和linux下面的接口吻合而定义的
typedef XCritical pthread_mutex_t;
inline void pthread_mutex_init(XCritical &,int){;}	//do nothing
inline void pthread_mutex_lock(XCritical &temp) {temp.Lock();}
inline void pthread_mutex_unlock(XCritical &temp) {temp.Unlock();}

#endif
}
#endif