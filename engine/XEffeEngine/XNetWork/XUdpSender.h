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
	static const int m_recvBuffSize = 4096;
	static const int m_packHeadSize = 12;
	unsigned int m_id;	//客户端ID，用于网络封包时做识别
	bool m_isInited;	//是否进行了初始化
	SOCKET m_localSock;	//本机的sock
	sockaddr_in m_targetAddr;	//发送的目标地址

	std::deque<XNetPack> m_freePacks;	//没有使用到的数据包池
	std::deque<XNetPack> m_curPacks;	//当前接收到的数据包
	XCritical m_mutex;	//线程锁

	XThreadState m_threadState;
	static DWORD WINAPI sendThread(void * pParam);		//发送线程
	void sendAPack();
	void _sendPack(unsigned char* data, int len);
	//包内容为：clientID,packID,packSize,data
	std::vector<unsigned char> m_sendBuff;
	unsigned int m_packID;	//数据包的编号
	bool m_autoPack;	//是否进行自动分包操作，如果是与第三方的通讯的话，是不能自动分包的，否则会造成数据不兼容
public:
	bool getIsAutoPack()const { return m_autoPack; }
	//与第三方通讯时autoPack必须为false;
	//autoPack为true时会对大数据进行自动分解为小尺寸的数据多次发送，以便于接收端能接收到完整的数据包
	bool setup(const std::string&ip,int port,int myPort = 0,bool autoPack = false,int id = -1);		//建立连接
	void close();	//关闭连接
	bool sendData(const void *data,int len);	//发送数据
	bool sendData(const std::string& str){return sendData((unsigned char *)str.c_str(),str.size());}
	XUdpSender()
		:m_isInited(false)
		, m_id(0)
		, m_packID(0)
		, m_autoPack(false)
		, m_localSock(INVALID_SOCKET)
	{}
	~XUdpSender(){close();}
	bool getIsInited()const { return m_isInited; }
};
}
#endif