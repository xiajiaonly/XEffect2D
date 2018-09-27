#ifndef _JIA_XNETCLIENT_
#define _JIA_XNETCLIENT_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.8
//--------------------------------
#include "XNetCommon.h"
#include "../XThread.h"
#include "../XCritical.h"
namespace XE{
//注意：尚未考虑多线程互斥问题
class XNetClient
{
private:
	//WSADATA m_wsaData;
	SOCKET m_netSocket;	//用于通讯的网络接口	

	XCritical m_mutexThread;
	XCritical m_mutexSend;	//发送线程锁
	XCritical m_mutexRecv;	//接收线程锁
	XCritical m_mutexBoardCast;	//线程锁

	XConnectState m_connectState; 
	XNetData * m_recvPacket;	//尚未接收完整的数据
	std::deque<XNetData *> m_recvDataBuff;	//接收到的数据
	std::deque<XNetData *> m_sendDataBuff;	//接收到的数据
	pthread_t m_recvThread;
	pthread_t m_sendThread;
	pthread_t m_boardcastThread;
	static DWORD WINAPI recvThread(void * pParam);	//接收线程
	static DWORD WINAPI sendThread(void * pParam);	//发送线程
	static DWORD WINAPI boardcastThread(void * pParam);	//发送线程
	//++++++++++++++++++++++++++++++++++++++++++++
	//20130903的修改
private:
	pthread_t m_connectThread;
	static DWORD WINAPI connectThread(void * pParam);	//连接线程
	XBool m_isClientCreated;
	int m_threadSum;			//线程的数量
	XBool m_useBroadcast;
	std::string m_serverIP;
public:
	XBool createClient(XBool useBroadcast = XTrue,const char *ip = NULL,int port = 0);			//通过广播的形式连接服务器
	void (*m_connectBrokenFun)();	//网络断开时候调用的回调函数
	void (*m_connectFun)();			//网络连接建立的时候调用的函数
	//--------------------------------------------
private:
	int m_recvPacketSize;	//接收到的数据包的大小
	unsigned char m_packetHeadData[PACKET_HEAD_LEN];
	XBool getDataPacket(unsigned char *buff,int len);
	XBool sendDataSocket(char * data,int len);

	//下面是为了广播才定义的接口
	sockaddr_in m_serverAddr;
	int m_serverPort;
	char m_projectStr[PROJECT_STRING_LEN];	//工程的特征字符串
	XBool m_boardcastThreadState;	//广播线程的状态
	//XBool recvBoardcast();	//接收回应
	XBool sendBoardcast();	//广播请求
	XBool m_isExit;
public:
	XBool connectToServer(const char *ip,int port = NET_SERVER_PORT);	//连接服务器,通过给定的IP和port连接服务器
	XBool connectToServer();			//自己建立广播连接服务器
	XBool getIsConnect() const{return m_connectState == CONNECT_STATE_CONNECT;}
	XBool setProjectStr(const char * str)
	{
		if(str == NULL ||
			strlen(str) >= PROJECT_STRING_LEN) return XFalse;
		strcpy_s(m_projectStr,PROJECT_STRING_LEN,str);
		return XTrue;
	}
	XBool sendData(XNetData *data)
	{
		if(data == NULL) return XFalse;
		m_mutexSend.Lock();
		m_sendDataBuff.push_back(data);
		if(m_sendDataBuff.size() > MAX_SEND_DATA_BUFF)	//为了防止数据拥堵，这里永远只保留最新的32帧数据
		{
			data = m_sendDataBuff[0];
			m_sendDataBuff.pop_front();
			m_mutexSend.Unlock();
			LogStr("XNetServer接收队列数据发生拥堵,丢弃较老的数据!");
			XMem::XDELETE_ARRAY(data->data);
			XMem::XDELETE(data);
		}else
			m_mutexSend.Unlock();

		return XTrue;
	}
	void clearSendData(XNetDataType type)	//从发送序列中取消所有的发送数据
	{
		m_mutexSend.Lock();
		std::deque<XNetData *>::iterator it = m_sendDataBuff.begin();
		for(;it != m_sendDataBuff.end();it ++)
		{
			if((* it)->type == type) (* it)->isEnable = XFalse;
		}
		m_mutexSend.Unlock();
	}
	XNetData *getData()
	{
		if(m_recvDataBuff.size() <= 0) return NULL;
		m_mutexRecv.Lock();
		if(m_recvDataBuff.front()->isEnable)
		{//数据包完整
			XNetData * ret = m_recvDataBuff.front();
			m_recvDataBuff.pop_front();
			m_mutexRecv.Unlock();
			return ret;
		}else
		{
			m_mutexRecv.Unlock();
			return NULL;
		}
	}
	XConnectState getConnectState() const {return m_connectState;}
	void release();
	XNetClient()
		:m_connectState(CONNECT_STATE_DISCONNECT)
		//,m_netSocket(-1)
		,m_netSocket(0)
		,m_boardcastThreadState(XFalse)
		,m_isExit(XFalse)
		,m_connectBrokenFun(NULL)
		,m_connectFun(NULL)
		,m_isClientCreated(XFalse)
		,m_threadSum(0)
	{
		m_recvDataBuff.clear();
		setProjectStr(DEFAULT_PROJECT_STR);
	}
	~XNetClient(){release();}
};
}
#endif