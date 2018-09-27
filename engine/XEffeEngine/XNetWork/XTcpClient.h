#ifndef _JIA_XTCPCLIENT_
#define _JIA_XTCPCLIENT_
#include <vector>
#include "XNetCommon.h"
#include "../XThread.h"
#include "../XCritical.h"

namespace XE {
class XTcpClient
{
private:
	static const int m_recvBuffSize = 2048;
	static const int m_packHeadSize = 8;
	bool m_isInited;	//是否进行了初始化
	WSADATA m_wsaData;
	SOCKET m_localSock;	//本机的sock
	sockaddr_in m_targetAddr;	//发送的目标地址
	//发送部分的定义
	std::deque<XNetPack> m_freePacks;	//没有使用到的数据包池
	std::deque<XNetPack> m_curPacks;	//当前接收到的数据包
	XCritical m_mutex;	//线程锁
	XThreadState m_sendThreadState;
	static DWORD WINAPI sendThread(void* pParam);		//发送线程
	void sendAPack();
	void _sendData(unsigned char* data, int len);
	//包内容为：clientID,packID,packSize,data
	std::vector<unsigned char> m_sendBuff;
	int m_sPackID;	//数据包的编号
	//接收部分的定义
	std::deque<XNetPack> m_rfreePacks;	//没有使用到的数据包池
	std::deque<XNetPack> m_rcurPacks;	//当前接收到的数据包
	XCritical m_rmutex;	//线程锁
	XThreadState m_recvThreadState;
	static DWORD WINAPI recvThread(void * pParam);		//发送线程
	void _recvAData(unsigned char* data, int len);
	void recvAData(unsigned char* data, int len);
	int m_rPackID;
	std::vector<unsigned char> m_recvBuff;

	bool m_isAutoPack;
public:
	bool setup(const std::string& ip, int port, bool isAutoPack = false);
	void close();
	bool sendData(const void* data, int len);
	bool sendData(const std::string& data)
	{
		return sendData(&data[0], data.size());
	}
	bool haveRecvData()const { return m_rcurPacks.size() > 0; }
	bool recoverAData(XNetPack &pack);	//回收一个数据包
	void popARecvData(XNetPack &data);	//获取一个接收数据包
	XTcpClient()
		:m_isInited(false)
		, m_isAutoPack(false)
		, m_sPackID(-1)
		, m_rPackID(-1)
	{}
	~XTcpClient() { close(); }
};
}
#endif