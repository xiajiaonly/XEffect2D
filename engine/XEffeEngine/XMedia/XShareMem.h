#ifndef _JIA_XSHAREMEM_
#define _JIA_XSHAREMEM_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "XOSDefine.h"

#ifdef XEE_OS_WINDOWS
#include "windows.h"
#endif

#ifdef XEE_OS_LINUX	//由于linux下的共享内存机制较为复杂，这里尚未经过测试，linux下不能使用
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif

class _XShareMem
{
private:
	bool m_isInited;
	unsigned char *m_pAddress;
#ifdef XEE_OS_WINDOWS
	HANDLE m_hMapFile;
#endif
#ifdef XEE_OS_LINUX
	sem_t *m_prsem;
	int m_shmid;
#endif
	int m_size;
public:
	bool init(const char *name,int size);
	unsigned char *getBuff() {return m_pAddress;}
	int getSize() {return m_size;}

	void release();
	_XShareMem()
		:m_isInited(false)
		,m_pAddress(NULL)
#ifdef XEE_OS_LINUX
		,m_prsem(NULL)
#endif
	{}
	~_XShareMem(){release();}
};

#endif