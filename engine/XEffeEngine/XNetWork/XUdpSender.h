#ifndef _JIA_XUDPSENDER_
#define _JIA_XUDPSENDER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include <string>
#include "XUdpReceiver.h"
namespace XE{
class XUdpSender
{
private:
	bool m_isInited;	//是否进行了初始化
	WSADATA m_wsaData;
	SOCKET m_localSock;	//本机的sock
	sockaddr_in m_targetAddr;	//发送的目标地址

	std::deque<XUdpPack> m_freePacks;	//没有使用到的数据包池
	std::deque<XUdpPack> m_curPacks;	//当前接收到的数据包
	XCritical m_mutex;	//线程锁

	XThreadState m_threadState;
	static DWORD WINAPI sendThread(void * pParam);		//发送线程
	void sendAPack();
public:
	bool setup(const std::string&ip,int port);		//建立连接
	void close();	//关闭连接
	bool sendData(const unsigned char *data,int len);	//发送数据
	bool sendData(const std::string &str){return sendData((unsigned char *)str.c_str(),str.size());}
	XUdpSender()
		:m_isInited(false)
	{}
	~XUdpSender(){close();}
};
}
#endif