#ifndef _JIA_XNETSERVER_
#define _JIA_XNETSERVER_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.8
//--------------------------------
//注意：尚未考虑多线程互斥问题
//只支持1c1s的模式
#include "XNetCommon.h"
#include "../XThread.h"
#include "../XCritical.h"
namespace XE{
class XNetServer
{
private:
	WSADATA m_wsaData;
	//XBool m_clientConnect;
	SOCKET m_netSocket;	//用于通讯的网络接口	
	SOCKET m_serverSocket;

	XCritical m_mutex;	//线程锁

	XConnectState m_connectState; 
	XBool m_isExit;				//是否主动退出
	XNetData * m_recvPacket;		//尚未接收完整的数据
	std::deque<XNetData *> m_recvDataBuff;	//接收到的数据
	std::deque<XNetData *> m_sendDataBuff;	//接收到的数据
	pthread_t m_recvThread;
	pthread_t m_sendThread;
	pthread_t m_acceptThread;
	pthread_t m_boardcastThread;		//广播线程
	static DWORD WINAPI recvThread(void * pParam);		//接收线程
	static DWORD WINAPI sendThread(void * pParam);		//发送线程
	static DWORD WINAPI acceptThread(void * pParam);	//连接线程
	static DWORD WINAPI boardcastThread(void * pParam);	//广播线程

	int m_recvPacketSize;	//接收到的数据包的大小
	unsigned char m_packetHeadData[PACKET_HEAD_LEN];
	XBool getDataPacket(unsigned char *buff,int len);	//从不完整的数据中提取出完整的数据包
	XBool sendDataSocket(char * data,int len);

	XBool m_autoIPMode;	//是否支持自动获取ip模式
	int m_serverPort;
	char m_projectStr[PROJECT_STRING_LEN];	//工程的特征字符串
public:
	XBool createServer(int port = NET_SERVER_PORT,XBool autoIP = XFalse);	//连接服务器
	XBool setProjectStr(const char * str)
	{
		if(str == NULL ||
			strlen(str) >= PROJECT_STRING_LEN) return XFalse;
		strcpy(m_projectStr,str);
		return XTrue;
	}
	XBool sendData(XNetData *data)
	{
		if(data == NULL) return XFalse;
		m_mutex.Lock();
		m_sendDataBuff.push_back(data);
		if(m_sendDataBuff.size() > 32)	//为了防止数据拥堵，这里永远只保留最新的32帧数据
		{
			data = m_sendDataBuff[0];
			m_sendDataBuff.pop_front();
			m_mutex.Unlock();
			LogStr("XNetServer接收队列数据发生拥堵,丢弃较老的数据!");
			XMem::XDELETE_ARRAY(data->data);
			XMem::XDELETE(data);
		}else
			m_mutex.Unlock();
		return XTrue;
	}
	void clearSendData(XNetDataType type)	//从发送序列中取消所有的发送数据
	{
		m_mutex.Lock();
		std::deque<XNetData *>::iterator it = m_sendDataBuff.begin();
		for(;it != m_sendDataBuff.end();it ++)
		{
			if((* it)->type == type) (* it)->isEnable = XFalse;
		}
		m_mutex.Unlock();
	}
	XNetData *getData()
	{
		if(m_recvDataBuff.size() <= 0) return NULL;
		m_mutex.Lock();
		if(m_recvDataBuff.front()->isEnable)
		{//数据包完整
			XNetData * ret = m_recvDataBuff.front();
			m_recvDataBuff.pop_front();
			m_mutex.Unlock();
			return ret;
		}else
		{
			m_mutex.Unlock();
			return NULL;
		}
	}
	XConnectState getConnectState() const {return m_connectState;}
	void release();
	XNetServer()
		:m_connectState(CONNECT_STATE_DISCONNECT)
	{
		m_recvDataBuff.clear();
		setProjectStr(DEFAULT_PROJECT_STR);
	}
	~XNetServer()
	{
		release();
	}
};
}
#endif