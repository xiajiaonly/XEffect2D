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

class _XNetServer
{
private:
	WSADATA m_wsaData;
	//_XBool m_clientConnect;
	SOCKET m_netSocket;	//用于通讯的网络接口	
	SOCKET m_serverSocket;

	_XCritical m_mutex;	//线程锁

	_XConnectState m_connectState; 
	_XBool m_isExit;				//是否主动退出
	_XNetData * m_recvPacket;		//尚未接收完整的数据
	std::deque<_XNetData *> m_recvDataBuff;	//接收到的数据
	std::deque<_XNetData *> m_sendDataBuff;	//接收到的数据
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
	_XBool getDataPacket(unsigned char *buff,int len);	//从不完整的数据中提取出完整的数据包
	_XBool sendDataSocket(char * data,int len);

	_XBool m_autoIPMode;	//是否支持自动获取ip模式
	int m_serverPort;
	char m_projectStr[PROJECT_STRING_LEN];	//工程的特征字符串
public:
	_XBool createServer(int port = NET_SERVER_PORT,_XBool autoIP = XFalse);	//连接服务器
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
		m_mutex.Lock();
		m_sendDataBuff.push_back(data);
		m_mutex.Unlock();
		return XTrue;
	}
	void clearSendData(_XNetDataType type)	//从发送序列中取消所有的发送数据
	{
		m_mutex.Lock();
		std::deque<_XNetData *>::iterator it = m_sendDataBuff.begin();
		for(;it != m_sendDataBuff.end();it ++)
		{
			if((* it)->type == type) (* it)->isEnable = XFalse;
		}
		m_mutex.Unlock();
	}
	_XNetData *getData()
	{
		if(m_recvDataBuff.size() <= 0) return NULL;
		m_mutex.Lock();
		if(m_recvDataBuff.front()->isEnable)
		{//数据包完整
			_XNetData * ret = m_recvDataBuff.front();
			m_recvDataBuff.pop_front();
			m_mutex.Unlock();
			return ret;
		}else
		{
			m_mutex.Unlock();
			return NULL;
		}
	}
	_XConnectState getConnectState() const {return m_connectState;}
	void release();
	_XNetServer()
		:m_connectState(CONNECT_STATE_DISCONNECT)
	{
		m_recvDataBuff.clear();
		setProjectStr(DEFAULT_PROJECT_STR);
	}
	~_XNetServer()
	{
		release();
	}
};

#endif