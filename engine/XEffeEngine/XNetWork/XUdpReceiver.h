#ifndef _JIA_XUDPRECEIVER_
#define _JIA_XUDPRECEIVER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
//这是一个基于UDP的接收器
#include <vector>
#include "XNetCommon.h"
#include "../XThread.h"
#include "../XCritical.h"
namespace XE{
struct XUdpPack
{
	int buffSize;		//内存空间的大小	
	int curDataLen;		//有效数据的大小
	unsigned char *data;
	XUdpPack()
		:data(NULL)
		,buffSize(0)
		,curDataLen(0)
	{}
};
class XUdpReceiver
{
private:
	static const int m_recvBuffSize = 2048;
	std::deque<XUdpPack> m_freePacks;	//没有使用到的数据包池
	std::deque<XUdpPack> m_curPacks;	//当前接收到的数据包
	bool m_isInited;
	WSADATA m_wsaData;
	SOCKET m_localSock;	//本机的sock
	XThreadState m_threadState;
	static DWORD WINAPI recvThread(void * pParam);		//接收线程
	void pushAPack(unsigned char *p,int len);	//压入一个数据包
	XCritical m_mutex;	//线程锁
public:
	//建立之后开启一个线程不停的接收数据
	bool setup(int port);	//建立
	void close();			//关闭
	bool haveData()const{return m_curPacks.size() > 0;}		//是否有接收到数据
	bool popAData(XUdpPack &pack)	//从队列中推出一个数据包,这个内存空间
	{
		if(m_curPacks.size() <= 0) return false;
		m_mutex.Lock();
		pack = m_curPacks[0];
		m_curPacks.pop_front();
		m_mutex.Unlock();
		return true;
	}
	bool recoverAData(XUdpPack &pack);	//回收一个数据包
	//处理数据包的过程如下：
	//调用haveData，检查是否存在数据包
	//popAData从队列中取出一个数据包，然后进行处理
	//处理完成之后调用recoverAData，将数据包回收，以便于重复使用

	XUdpReceiver()
		:m_isInited(false)
	{}
	~XUdpReceiver(){close();}
};
}
#endif