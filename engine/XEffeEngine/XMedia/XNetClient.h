#ifndef _JIA_XNETCLIENT_
#define _JIA_XNETCLIENT_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.8
//--------------------------------
#include "XNetCommon.h"
//注意：尚未考虑多线程互斥问题
class _XNetClient
{
private:
	//WSADATA m_wsaData;
	SOCKET m_netSocket;	//用于通讯的网络接口	

	_XCritical m_mutexThread;
	_XCritical m_mutexSend;	//发送线程锁
	_XCritical m_mutexRecv;	//接收线程锁
	_XCritical m_mutexBoardCast;	//线程锁

	_XConnectState m_connectState; 
	_XNetData * m_recvPacket;	//尚未接收完整的数据
	std::deque<_XNetData *> m_recvDataBuff;	//接收到的数据
	std::deque<_XNetData *> m_sendDataBuff;	//接收到的数据
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
	_XBool m_isClientCreated;
	int m_threadSum;			//线程的数量
	_XBool m_useBroadcast;
	string m_serverIP;
public:
	_XBool createClient(_XBool useBroadcast = XTrue,const char *ip = NULL,int port = 0);			//通过广播的形式连接服务器
	void (*m_connectBrokenFun)();	//网络断开时候调用的回调函数
	void (*m_connectFun)();			//网络连接建立的时候调用的函数
	//--------------------------------------------
private:
	int m_recvPacketSize;	//接收到的数据包的大小
	unsigned char m_packetHeadData[PACKET_HEAD_LEN];
	_XBool getDataPacket(unsigned char *buff,int len);
	_XBool sendDataSocket(char * data,int len);

	//下面是为了广播才定义的接口
	sockaddr_in m_serverAddr;
	int m_serverPort;
	char m_projectStr[PROJECT_STRING_LEN];	//工程的特征字符串
	_XBool m_boardcastThreadState;	//广播线程的状态
	//_XBool recvBoardcast();	//接收回应
	_XBool sendBoardcast();	//广播请求
	_XBool m_isExit;
public:
	_XBool connectToServer(const char *ip,int port = NET_SERVER_PORT);	//连接服务器,通过给定的IP和port连接服务器
	_XBool connectToServer();			//自己建立广播连接服务器
	_XBool getIsConnect() const
	{
		return m_connectState == CONNECT_STATE_CONNECT;
	}
	_XBool setProjectStr(const char * str)
	{
		if(str == NULL) return XFalse;
		if(strlen(str) >= PROJECT_STRING_LEN) return XFalse;
		strcpy(m_projectStr,str);
		return XTrue;
	}
	_XBool sendData(_XNetData *data)
	{
		if(data == NULL) return XFalse;
		m_mutexSend.Lock();
		m_sendDataBuff.push_back(data);
		m_mutexSend.Unlock();
		return XTrue;
	}
	void clearSendData(_XNetDataType type)	//从发送序列中取消所有的发送数据
	{
		m_mutexSend.Lock();
		std::deque<_XNetData *>::iterator it = m_sendDataBuff.begin();
		for(;it != m_sendDataBuff.end();it ++)
		{
			if((* it)->type == type) (* it)->isEnable = XFalse;
		}
		m_mutexSend.Unlock();
	}
	_XNetData *getData()
	{
		if(m_recvDataBuff.size() <= 0) return NULL;
		m_mutexRecv.Lock();
		if(m_recvDataBuff.front()->isEnable)
		{//数据包完整
			_XNetData * ret = m_recvDataBuff.front();
			m_recvDataBuff.pop_front();
			m_mutexRecv.Unlock();
			return ret;
		}else
		{
			m_mutexRecv.Unlock();
			return NULL;
		}
	}
	_XConnectState getConnectState() const {return m_connectState;}
	void release();
	_XNetClient()
		:m_connectState(CONNECT_STATE_DISCONNECT)
		,m_netSocket(-1)
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
	~_XNetClient(){release();}
};
#endif