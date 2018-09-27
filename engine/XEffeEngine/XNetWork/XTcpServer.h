#ifndef _JIA_XTCPSERVER_
#define _JIA_XTCPSERVER_
#include <vector>
#include "XNetCommon.h"
#include "../XThread.h"
#include "../XCritical.h"
//尚未完成
//最简单的TCP服务器，可以建立连接，接收和发送数据
//支持自动分包，如果需要更复杂的功能，请使用XSocketEx或者XSocket
namespace XE {
class XTcpServer
{
private:
	static const int m_recvBuffSize = 2048;
	static const int m_packHeadSize = 8;
	bool m_isInited;
	SOCKET m_localSock;	//本机的sock
	XThreadState m_recvThreadState;		//连接线程的状态
	XThreadState m_sendThreadState;		//连接线程的状态
	XThreadState m_acceptThreadState;	//连接线程的状态

	static DWORD WINAPI recvThread(void * pParam);		//接收线程 
	static DWORD WINAPI sendThread(void * pParam);		//发送线程 
	static DWORD WINAPI acceptThread(void * pParam);	//监听线程 

	struct XClientInfo
	{
		bool isConnectOK;	//网络连接是否正常
		SOCKET sock;
		int id;
		int rPackID;
		std::vector<unsigned char> recvBuff;
	};
	int m_curClientID;
	std::vector<XClientInfo> m_clients;
	void addAClient(SOCKET sock);
	int getIndexBySock(SOCKET sock)
	{
		for (int i = 0; i < m_clients.size(); ++i)
		{
			if (m_clients[i].sock == sock) return i;
		}
		return -1;
	}
	int getIndexByID(int id)
	{
		for (int i = 0; i < m_clients.size(); ++i)
		{
			if (m_clients[i].id == id) return i;
		}
		return -1;
	}
	//发送部分的定义
	std::deque<XNetPack> m_sFreePacks;	//没有使用到的数据包池
	std::deque<XNetPack> m_sCurPacks;	//当前接收到的数据包
	XCritical m_sMutex;	//线程锁
	void sendAPack();
	void _sendData(unsigned char* data, int len, const SOCKET& sock);
	//包内容为：clientID,packID,packSize,data
	std::vector<unsigned char> m_sendBuff;
	int m_sPackID;	//数据包的编号

	//接收部分的定义
	std::deque<XNetPack> m_rFreePacks;	//没有使用到的数据包池
	std::deque<XNetPack> m_rCurPacks;	//当前接收到的数据包
	XCritical m_rMutex;	//线程锁
	void _recvAData(unsigned char* data, int len, int index);
	void recvAData(unsigned char* data, int len, int index);

	bool m_isAutoPack;	//是否支持自动分包
public:
	bool setup(const std::string& ip, int port, bool isAutoPack = false);
	bool setup(int port, bool isAutoPack = false);
	void close();
	XTcpServer()
		:m_isInited(false)
		, m_localSock(INVALID_SOCKET)
		, m_curClientID(0)
		, m_isAutoPack(false)
		, m_sPackID(-1)
	{}
	~XTcpServer() { close(); }
	bool sendData(const void* data, int len, int cID = 0);
	bool sendData(const std::string& data, int cID = 0)
	{
		return sendData(&data[0], data.size(), cID);
	}
	bool haveRecvData()const { return m_rCurPacks.size() > 0; }
	bool recoverAData(XNetPack &pack);	//回收一个数据包
	void popARecvData(XNetPack &data);	//获取一个接收数据包
};
}
#endif