#ifndef _JIA_XSOCKETEX_
#define _JIA_XSOCKETEX_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.2.25
//--------------------------------
//这是一个增强型的基于socket的网络连接的类，与其他网络连接类的最大区别就是支持多客户端连接
//设计使用连接和通讯两层设计，连接层负责保持网络的连接以及反馈连接状态
//通讯层负责通讯以及反馈通讯状态，通讯层还包括封包和加密模块。
#include "XOSDefine.h"
#include <string>
#include <vector>
#include <deque>
#include "XCritical.h"
#include "XThread.h"
#include "XMedia/XNetCommon.h"
//1、尚未完成对数据进行加密
//2、名称不考虑唯一性，也就是可以重名，是否允许重名根据具体应用来确定
//3、当服务器遇到客户端掉线是没有对原有客户端的数据进行处理，这样会累积资源消耗，需要有相应的掉线处理机制。

//网络连接状态
enum _XSocketConnectState
{
	CON_STATE_NULL,		//尚未进行连接
	CON_STATE_NORMAL,	//连接正常
	CON_STATE_UNSTABLE,	//连接不稳定，短暂断开
	CON_STATE_ERROR,	//网络连接断开
};
enum _XSocketNetRole
{
	NET_NULL,		//尚未连接
	NET_SERVER,		//网络服务器
	NET_CLIENT,		//网络客户端
};
enum _XSocketMode
{
	SOCKET_LAN,		//局域网
	SOCKET_WAN,		//广域网
};
enum _XSocketWorkMode
{
	SOCKET_WORK_MODE_IP,	//通过IP的方式建立连接
	SOCKET_WORK_MODE_NAME,	//通过服务器名称的方式建立连接
};
enum _XSocketSysDataType
{//系统数据的类型
	SYS_DATA_TYPE_SETNAME,		//设置名称
	SYS_DATA_TYPE_SETID,		//设置ID
	SYS_DATA_TYPE_UPDATEINIFO,	//更新信息
	SYS_DATA_TYPE_HEARTBEAT,	//心跳信号
};
enum _XSocketExDataType
{
	SOCKET_DATA_TYPE_SYSTEM,	//系统数据
	SOCKET_DATA_TYPE_CUSTOM,	//定制数据
};
#define SOCKET_PACKET_HEAD_SIZE (16)	//数据包的包头大小
#define DEFAULT_CLIENT_NAME ("xiajia_1981@163.com")
#pragma pack(push)
#pragma pack(1)
struct _XSocketDataPack
{//数据包的结构
	int fromID;	//从哪里来
	int toID;	//到哪里去
	_XSocketExDataType dataType;	//数据类型
	int dataLen;	//数据长度
	unsigned char * data;	//数据体
	_XSocketDataPack()
		:data(NULL)
		,dataType(SOCKET_DATA_TYPE_CUSTOM)
	{}
};
#pragma pack(pop)
enum _XSocketClientState
{
	CLIENT_STATE_ONLINE,	//在线
	CLIENT_STATE_OFFLINE,	//离线
};
#define SOCKET_INFO_HEAD_SIZE (16)	//用户信息头大小
struct _XSocketUserInfo
{
	int userID;				//用户ID（由服务器分配）
	int userSocket;			//用户套接字
	_XSocketClientState userState;			//用户状态
	int userNameLen;
	char *userName;			//用户名称
	_XSocketUserInfo()
		:userName(NULL)
		,userNameLen(0)
	{}
};
enum _XSocketClinetState
{//客户端的状态
	CLINET_STATE_UNCONNECT,	//尚未连接
	CLINET_STATE_CONNECT,	//刚连接
	CLINET_STATE_DISCONNECT,//连接断开
};
struct _XSocketRecvPacket
{
	int recvDataLen;				//接收到的数据包的长度
	unsigned char headData[SOCKET_PACKET_HEAD_SIZE];		//数据包的包头数据
	_XSocketDataPack *dataPacket;	//数据包的数据
	_XSocketRecvPacket()
		:recvDataLen(0)
		,dataPacket(NULL)
	{}
};
struct _XSocketUserData
{
	bool isEnable;	//是否有效
	int userID;		//用户ID
	int userSocket;	//用户的连接套接字
	_XSocketClinetState state;
	int heartbeatTimer;	//心跳计时器

	_XSocketRecvPacket recvPacket; //接收的数据包
	_XThreadState recvThreadState;

	_XSocketUserData()
		:isEnable(false)
		,state(CLINET_STATE_UNCONNECT)
		,heartbeatTimer(0)
		,recvThreadState(STATE_BEFORE_START)
	{}
};

class _XSocketEx
{
private:
	bool m_isInited;	//是否初始化
	SOCKET m_netSocket;	//用于网络连接的套接字
	_XSocketNetRole m_socketRole;		//服务器或者客户端
	int m_timer;	//计时器

	//客户端的情况
	_XSocketConnectState m_conState;	//网络连接状态
	_XSocketRecvPacket m_recvPacket;	//接收的数据包
	//服务器的情况
	int m_nowAddClientID;	//当前接入的客户端的ID
	void addAClient(int clientSocket);	//新的客户端加入
	int m_myID;		//自身的编号
	int m_IDIndex;	//用于建立ID

	_XCritical m_recvMutex;				//接收的线程锁
	std::deque<_XSocketDataPack *> m_recvDeque;		//接收队列
	_XCritical m_sendMutex;				//发送的线程锁
	std::deque<_XSocketDataPack *> m_sendDeque;		//发送队列
	_XCritical m_userInfoMutex;			//用户信息的线程锁
	std::vector<_XSocketUserInfo *> m_usersInfo;	//用户信息队列
	//_XCritical m_userDataMutex;			//用户数据的线程锁
	std::vector<_XSocketUserData *> m_usersData;	//用户数据队列

	_XSocketWorkMode m_workMode;

	sockaddr_in m_serverAddr;	//服务器地址
	std::string m_serverName;	//服务器的名称
	std::string m_serverIP;		//服务器的IP
	int m_serverPort;		//服务器的端口号
	std::string m_myName;	//自己的名字

	bool getDataPacket(_XSocketRecvPacket &packet,unsigned char *buff,int len);	//将数据解析成完整的数据包

	_XThreadState m_connectThreadState;	//连接线程的状态
	_XThreadState m_recvThreadState;	//连接线程的状态
	_XThreadState m_sendThreadState;	//连接线程的状态
	_XThreadState m_acceptThreadState;	//连接线程的状态
	HANDLE m_acceptThreadHandle;
	_XThreadState m_boardcastThreadState;	//广播线程的状态
#ifdef XEE_OS_WINDOWS
    static DWORD WINAPI connectThread(void * pParam);	//连接线程 
	static DWORD WINAPI recvThread(void * pParam);		//接收线程 
	static DWORD WINAPI sendThread(void * pParam);		//发送线程 
	static DWORD WINAPI acceptThread(void * pParam);	//监听线程 
	static DWORD WINAPI recvThreadX(void * pParam);		//服务器段为客户端开启的接收线程 
	static DWORD WINAPI boardcastThread(void * pParam);		//广播线程 
#endif
	bool connectToNet();	//连接到网络
	//下面是一些私有的方法
	bool connectToServer();	//连接到服务器
	bool createServer();	//简历服务器
	void sendClientInfo();	//向所有客户端发送目前的网络状态
	void systemDataproc(const _XSocketDataPack *data);
	void lockData(unsigned char *data,int len);		//加密数据
	void unlockData(unsigned char *data,int len);	//解密数据
	bool sendAData(_XSocketDataPack * data)
	{//注意多线程互斥,这里默认为数据已经经过加密
		if(data == NULL) return false;
		if(data->toID == m_myID)	//自己发送给自己的消息直接丢弃
		{
			XDELETE_ARRAY(data->data);
			XDELETE(data);
			return false;
		}
		m_sendMutex.Lock();
		m_sendDeque.push_back(data);
		m_sendMutex.Unlock();
		return true;
	}
	void requestRename();	//向服务器发送更名请求
	void sendHeartbeat();	//发送心跳信号
	bool m_clientStateChange;	//客户端的状态是否发生变化
	void setOffline(int id);//设置某个客户端离线
	void heartbeatProc(int stepTime);//计算客户端的心跳并做相应的处理
	bool sendBoardcast();	//广播请求
public:
	bool createNetWorkByName(const char *serverName,int port)		//通过服务器名称建立网络
	{
		if(m_isInited) return false;	//重复连接
		if(serverName == NULL) return false;

		m_workMode = SOCKET_WORK_MODE_NAME;
		m_serverName = serverName;
		m_serverPort = port;
		m_conState = CON_STATE_NULL;
		//m_socketRole = NET_NULL;
		//下面建立连接线程
		m_connectThreadState = STATE_BEFORE_START;
		if(CreateThread(0,0,connectThread,this,0,NULL) == 0) return false;	//连接线程建立失败
		//下面建立数据处理线程(尚未完成)

		m_isInited = true;
		return true;
	}
	bool createNetWorkByIP(const char *serverIP,int port)		//通过服务器IP建立网络
	{//尚未完成
		if(m_isInited) return false;	//重复连接
		if(serverIP == NULL) return false;

		m_workMode = SOCKET_WORK_MODE_IP;
		m_serverIP = serverIP;
		m_serverPort = port;
		m_conState = CON_STATE_NULL;
		//m_socketRole = NET_NULL;
		//下面建立连接线程
		m_connectThreadState = STATE_BEFORE_START;
		if(CreateThread(0,0,connectThread,this,0,NULL) == 0) return false;	//连接线程建立失败
		//下面建立数据处理线程(尚未完成)

		m_isInited = true;
		return true;
	}
	//发送
	bool sendAData(int toID,unsigned char * data,int len);	//向指定用户发送信息
	bool sendToAll(unsigned char * data,int len)
	{
		if(m_socketRole == NET_CLIENT) return sendAData(-1,data,len); else
		if(m_socketRole == NET_SERVER)
		{//如果是服务器则向所有客户端发送数据
		//	m_userDataMutex.Lock();
			for(int i = 0;i < m_usersData.size();++ i)
			{
				if(m_usersData[i]->state == CLINET_STATE_CONNECT) 
					sendAData(m_usersData[i]->userID,data,len);
			}
		//	m_userDataMutex.Unlock();
		}
		return true;
	}			//向当前在线的所有用户发送信息(尚未实现)
	//接收
	_XSocketDataPack *popAData()
	{
		m_recvMutex.Lock();
		if(m_recvDeque.size() <= 0)
		{
			m_recvMutex.Unlock();
			return NULL;
		}
		_XSocketDataPack *ret = m_recvDeque[0];
		m_recvDeque.pop_front();
		m_recvMutex.Unlock();
		return ret;
	}
	void deleteAData(_XSocketDataPack * p)
	{
		XDELETE_ARRAY(p->data);
		XDELETE_ARRAY(p);
	}

	void release();
	_XSocketEx()
		:m_isInited(false)
		,m_socketRole(NET_NULL)
		,m_conState(CON_STATE_NULL)
		,m_workMode(SOCKET_WORK_MODE_IP)
		,m_netSocket(INVALID_SOCKET)
		,m_nowAddClientID(-1)
		,m_IDIndex(0)
		,m_myID(0)
		,m_myName(DEFAULT_CLIENT_NAME)
		,m_timer(0)
		,m_clientStateChange(false)
		,m_connectThreadState(STATE_BEFORE_START)	//连接线程的状态
		,m_recvThreadState(STATE_BEFORE_START)	//连接线程的状态
		,m_sendThreadState(STATE_BEFORE_START)	//连接线程的状态
		,m_acceptThreadState(STATE_BEFORE_START)	//连接线程的状态
		,m_boardcastThreadState(STATE_BEFORE_START)	//广播线程的状态
		,m_acceptThreadHandle(0)
	{}
	~_XSocketEx() {release();}

	//下面是一些零散的方法定义
	int getClientSum() {return m_usersInfo.size();}
	const _XSocketUserInfo * getUserInfo(int index)
	{
		_XSocketUserInfo * temp = NULL;
		m_userInfoMutex.Lock();
		if(index < 0 || index >= m_usersInfo.size())
		{
			m_userInfoMutex.Unlock();
			return NULL;
		}
		temp = m_usersInfo[index];
		m_userInfoMutex.Unlock();
		return temp;
	}
	void setAsClient()
	{ 
		if(m_isInited) return;
		m_socketRole = NET_CLIENT;
	}
	void setAsServer()
	{ 
		if(m_isInited) return;
		m_socketRole = NET_SERVER;
	}
	_XSocketNetRole getRole() const {return m_socketRole;}
	int getMyID() const {return m_myID;}
	int getMySocket() const {return m_netSocket;}
	bool setMyName(const std::string &myName);
	std::string getMyName(){return m_myName;}
	_XSocketConnectState getConState() const {return m_conState;}
};
#endif