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
class XUdpReceiver
{
private:
	static const int m_recvBuffSize = 4096;
	static const int m_packHeadSize = 12;
	std::deque<XNetPack> m_freePacks;	//没有使用到的数据包池
	std::deque<XNetPack> m_curPacks;	//当前接收到的数据包
	bool m_isInited;
	SOCKET m_localSock;	//本机的sock
	XThreadState m_threadState;
	static DWORD WINAPI recvThread(void * pParam);		//接收线程
	void pushAPack(unsigned char *p,int len);	//压入一个数据包
	void _pushAPack(unsigned char *p,int len);	//压入一个数据包
	XCritical m_mutex;	//线程锁
	struct XClientInfo
	{
		int id;
		int packID;
		std::vector<unsigned char> buff;
	};
	std::vector<XClientInfo> m_clientsInfo;
	int getIndexByID(int id)
	{
		for (int i = 0;i < m_clientsInfo.size();++i)
		{
			if (m_clientsInfo[i].id == id) return i;
		}
		return -1;
	}
	bool m_autoPack;	//是否进行自动分包操作，如果是与第三方的通讯的话，是不能自动分包的，否则会造成数据不兼容
public:
	//建立之后开启一个线程不停的接收数据
	bool getIsAutoPack()const { return m_autoPack; }
	//与第三方通讯时autoPack必须为false;
	//此时数据包大小不能大于m_recvBuffSize，否则会造成接收失败
	//autoPack为true时会对大数据进行自动分解为小尺寸的数据多次发送，以便于接收端能接收到完整的数据包
	bool setup(int port, bool autoPack = false);	//建立
	void close();			//关闭
	bool haveData()const{return m_curPacks.size() > 0;}		//是否有接收到数据
	bool popAData(XNetPack &pack)	//从队列中推出一个数据包,这个内存空间需要回收
	{
		if(m_curPacks.size() <= 0) return false;
		m_mutex.Lock();
		pack = m_curPacks[0];
		m_curPacks.pop_front();
		m_mutex.Unlock();
		return true;
	}
	bool recoverAData(XNetPack &pack);	//回收一个数据包
	//处理数据包的过程如下：
	//调用haveData，检查是否存在数据包
	//popAData从队列中取出一个数据包，然后进行处理
	//处理完成之后调用recoverAData，将数据包回收，以便于重复使用

	XUdpReceiver()
		:m_isInited(false)
		, m_autoPack(false)
		, m_localSock(INVALID_SOCKET)
	{}
	~XUdpReceiver(){close();}
};
}
#endif