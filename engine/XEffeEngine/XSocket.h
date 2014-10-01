#ifndef _JIA_XSOCKET_
#define _JIA_XSOCKET_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:      2009.10.22
//--------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//说明：
//创建时间：    2009年10月22日
//程 序 员：    贾胜华
//修改时间：    2009年10月31日
//该文件为基于Linux下的 Socket而建立的一个网络交互的类
//缺陷说明：数据加密只对数据部分进行，不会对数据包头部分作加密，这将
//会导致部分数据信息泄露，考虑到加密的安全性，以后的版本中需要对这部
//分加密作进一步的处理
//------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//修改时间：    2009年12月30日
//修改人：      贾胜华
//修改内容：    1、调试输出使用宏定义的方式(完成)
//              2、指针参数的传递使用应用的方式(未完成)
//------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//修改时间：    2010年12月23日
//修改人：        贾胜华
//修改内容：    1、对于超大型数据包进行自动拆包
//                2、添加广播数据的方式
//------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//封包说明：
//定义最大的交互客户端数量
//#define MAX_CLIENT    20
//
//包定义：
//字节
//0        Data Package Type        数据类型    0：无效的数据包 1：握手数据包 2：用户自己定义的数据包 3:服务器状态消息
//1
//2
//3
//4        source                  源socket
//5
//6
//7
//8        target                  目标socket
//9
//10
//11
//12       length                  包裹有效数据位的长度
//13       key                     加密密钥字节，当key为0时，数据不进行加密操作，加密函数会直接跳过，这里考虑到用户自己的数据加密以及提升执行效率
//14       check data
//15
//…
//1023     data                    数据部分       
//
//注意到数据结构中限定数据最大长度实际上只有MAXDATASIZE - 15的长度
//------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//广播数据结构
//1、服务器使用的端口号
//2、服务器的网络标志字符串
//------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//扩展后的广播数据结构
//0
//1
//2
//3        Data Type        数据类型
//4       
//5
//6
//7        Data Length        数据长度
//8
//9
//…
//1023    data            数据部分
//2、服务器的网络标志字符串
//------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//服务器状态数据结构定义
//0
//1
//2
//3     有效客户端数量
//4
//5
//6
//7     服务器sock
//8
//9
//10   
//11    本客户端在服务器的ID
//12
//13
//14
//15    除本客户端以外的其他客户算Sock
//16
//....
//------------------------------------------------------

#include "XOSDefine.h"
class _XSocket;

//#define DEBUG_FOR_CN        //显示中文日志信息
//#define DEBUG_MODE_SHOW    //是否显示日志信息

//老版本的调试模式
#ifdef DEBUG_MODE
#define DSHOW printf

inline void addLogText(_XSocket *p,char *pCN,char *pEN)
{
#ifdef DEBUG_FOR_CN
//	p->m_logFile.addLogInfo(pCN);
#ifdef DEBUG_MODE_SHOW
        DSHOW("%s",pCN);
#endif
#else
//	p->m_logFile.addLogInfo(pEN);
#ifdef DEBUG_MODE_SHOW
	DSHOW("%s",pEN);
#endif
#endif
}
#else
#define DSHOW (void)
#define addLogText (void)
#endif

#pragma comment(lib, "ws2_32.lib")

#ifdef XEE_OS_WINDOWS
#include "winsock.h"
#endif

#include "stdio.h"
#include "stdlib.h"
#include "XBasicFun.h"

#ifdef XEE_OS_LINUX
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>        //inet_addr()
#include <unistd.h>           //usleep()
#include <fcntl.h>            //fcntl() 设置非阻塞函数
#include <signal.h>           //signal() 数据发送意外处理
#endif

//#ifdef DEBUG_MODE
//#include "XLogBook.h"       //日志系统类
//#endif

#include "XBasicClass.h"

#define MAX_CLIENT            2048         //服务器可以连接到客户端的最大客户端数目
#define MAX_DELAY_TIMES        10          //最大的握手失败限制
#define MAX_NET_FLAGS        20          //网络标记字符串的长度
#define MAX_DATABUFF_DEEP    500         //数据寄存堆栈深度
#define DATA_HEAD_SIZE        15          //数据包包头大小
#define BC_DATA_SIZE        1024        //广播数据包的大小
#define BC_BUFF_DEEP        128            //广播数据队列深度

#define SERVER_PORT       8019        //服务器广播端口

//TCP/IP网络数据传输的数据类型的定义
enum _XSocketDataType
{
    DATA_TYPE_NULL = 0,            //无效的数据
    DATA_TYPE_MUTUAL,                //网络心跳数据
    DATA_TYPE_USER_DATA,            //用户自己的数据
    DATA_TYPE_BOARDCAST_DATA,            //广播的数据
    DATA_TYPE_SERVER_STATE        //服务器的状态数据
};

#pragma pack(push)
#pragma pack(1)
struct _XSocketData
{
    char dataType;                               //数据类型   
    int sourceSocket;                            //源套接字
    int targetSocket;                            //目标套接字
    int dataLength;                              //数据长度
    char lockKey;                                //加密密钥
    char checkKey;                               //校验密码
    char *pData;                                 //数据部分的指针
    _XSocketData()
        :dataType(DATA_TYPE_NULL)
        ,sourceSocket(0)
        ,targetSocket(0)
        ,dataLength(0)
        ,lockKey(0)
        ,checkKey(0)
        ,pData(NULL)
    {
    }
/*	_XSocketData(const _XSocketData& temp)
	{
		dataType = temp.dataType;
		sourceSocket = temp.sourceSocket;
		targetSocket = temp.targetSocket;
		dataLength = temp.dataLength; 
		lockKey = temp.lockKey;	
		checkKey = temp.checkKey;	
		pData = createArrayMem<char>(dataLength);
		if(pData == NULL) 
		{
			dataLength = 0;			
			return;
		}
		memcpy(pData,temp.pData,dataLength);
	}*/
    ~_XSocketData()
    {//动态内存管理，那里分配，就在那里删除，不要乱删除
		//XDELETE_ARRAY(pData);
    }
/*	_XSocketData& operator = (const _XSocketData& temp)
	{
		if(&temp == this) return * this;
		dataType = temp.dataType;
		sourceSocket = temp.sourceSocket;
		targetSocket = temp.targetSocket;
		dataLength = temp.dataLength; 
		lockKey = temp.lockKey;	
		checkKey = temp.checkKey;	
		pData = createArrayMem<char>(dataLength);
		if(pData == NULL) 
		{
			dataLength = 0;			
			return * this;
		}
		memcpy(pData,temp.pData,dataLength);
		return * this;
	}*/
};
#pragma pack(pop)

enum
{
    ERROR_TYPE_NULL = 0,                        //0        没有错误
    ERROR_TYPE_SERVER_SOCKET_APPLY = 10000,        //10000    服务器套接字申请失败
    ERROR_TYPE_SERVER_SOCKET_BIND,                //10001    服务器套接字帮定失败
    ERROR_TYPE_SERVER_SOCKET_LISTEN,            //10002    服务器侦听失败
    ERROR_TYPE_ACCEPT_CLIENT,                    //10003    服务器接受客户端的连接请求失败
    ERROR_TYPE_THREAD_ACCEPT_CREATE,            //10004    服务器accept线程开启失败
    ERROR_TYPE_CLIENT_SOCKET_APPLY,                //10005    客户端套接字申请失败
    ERROR_TYPE_CLIENT_SOCKET_BIND,                //10006    客户端套接字帮定失败
    ERROR_TYPE_THREAD_BROADCAST_CREATE,            //10007    服务器建立广播线程出错
    ERROR_TYPE_CLIENT_S_SOCKET_APPLY,            //10008    客户端连接服务器失败
    ERROR_TYPE_THREAD_GETBROADCAST_CREATE_N,    //10009    客户端接受服务器广播线程开启失败
    ERROR_TYPE_THREAD_GETDATA_CREATE_N,            //10010    客户端握手线程开启失败
    ERROR_TYPE_THREAD_MUNUAL_CREATE,            //10011    服务器段握手线程开启失败
    ERROR_TYPE_THREAD_MUNUAL_CREATE_N,            //10012    客户端数据接受线程开启失败
    ERROR_TYPE_THREAD_CLIENT_GETDATA_CREATE,    //10013    服务器端数据接受线程开启失败
    ERROR_TYPE_CLIENT_CONNECT_SERVER            //10014    客户端连接服务器失败   
};

enum
{
    NET_STATE_NULL = 0,        //无效
    NET_STATE_SERVER,        //服务器
    NET_STATE_CLIENT        //客户端
};

enum
{
    BROADCAST_DATA_TYPE_NULL = 0,                //广播数据
    BROADCAST_DATA_TYPE_SERVER_INFO,            //服务器信息
    BROADCAST_DATA_TYPE_CLIENT_DATA                //客户端数据
};

enum
{
    BROADCAST_WAY_TCPIP,
    BROADCAST_WAY_UDP
};

class _XSocket
{
//#ifdef DEBUG_MODE
//public:   
//    _XLogbook m_logFile;
//#endif
private:
    //state flag
    int m_isServer;                             //是否为服务器 0：无效 1：服务器 2：客户端
    int m_myConnectState;                       //是否连接到网络
    //char m_clientConnectState[MAX_CLIENT];       //是否连接到网络
    char *m_clientConnectState;                    //是否连接到网络
    int m_myDelayTimes;                         //握手失败的次数
    //char m_clientDelayTimes[MAX_CLIENT];         //客户端的握手失败次数
    char *m_clientDelayTimes;                    //客户端的握手失败次数

    _XBool m_needMutual;                            //是否允许握手存在(2010年12月15日，服务器和客户端的这个设置必须一致)
   
    int m_serverExitFlag;                       //服务器退出标志
    int m_clientExitFlag;                       //客户端退出标志
       
    _XBool dataProtectionFlag;                    //是否使用数据保护功能
    //other member
    int m_mySocket;                             //本机使用的Socket
    int m_serverSocket;                         //服务器的Socket
    int m_serverPort;                           //服务器使用的端口号
    //int m_clientSocket[MAX_CLIENT];              //所有客户端的Socket
    int *m_clientSocket;              //所有客户端的Socket
    int m_clientSum;                            //有效客户端的数目   
   
    int m_serverID;                             //将所有网络成员在服务器上的Socket作为机器的ID，那么服务器ID则是服务器在自身的Socket
    int m_clientID;                             //自身作为客户端时，自身的ID
    //线程手柄
    pthread_t m_serverAcceptThreadP;            //服务器的接受线程
    pthread_t m_serverBroadcastThreadP;         //服务器的广播线程
    pthread_t m_mutualThreadP;                  //握手线程
    pthread_t m_getDataThreadPOnClient;                 //服务器的客户处理线程
    pthread_t m_clientGetBroadcastThreadP;      //客户端接受广播的线程
    //下面的线程是为了3.5版本的修改而完善的
    pthread_t m_getDataThreadPOnServer;    //服务器每检测大一个客户端加入,则开启一个该线程
    int m_flagClientThreadID;                //这个参数用于向上面的线程传递数据，告诉线程需要检测那个端口
   
    //char m_netFlags[MAX_NET_FLAGS];               //网络标记字符串，用于区分服务器的类型
    char *m_netFlags;               //网络标记字符串，用于区分服务器的类型
   
    //数据缓存    现在我们假设最多可以缓存5个数据
    //_XSocketData m_getDataBuff[MAX_DATABUFF_DEEP];
    _XSocketData *m_getDataBuff;
    int m_getDataNewP;
    int m_getDataOldP;
    int pushAMessage(_XSocketData tempData);    //向队列中推入一条消息
    int popAMessage(_XSocketData *tempData);    //从队列中取出一条消息
   
    void clearDataBuff();
    pthread_mutex_t m_mutex;                        //数据互斥

    int m_WrongFlag;                                //错误编号   

    //++++++++++++++++++++++++++++++++++++++
    //下面是为了实现广播而定义的成员和方法
    int m_broadCastWay;        //广播使用的方法
    //_XSocketData m_broadcastBuff[BC_BUFF_DEEP];
    _XSocketData *m_broadcastBuff;				//广播的队列，所有需要广播的数据都通过这个队列来管理，这个队列在服务器上才有，需要广播的数据会推入这个队列中，在广播进程中处理掉这些数据
    int m_broadcastNewPoint;
    int m_broadcastOldPoint;
    int pushABroadcastMessage(_XSocketData tempData);    //返回推入是否成功，如果失败返回0
public:
    void setBroadCastWay(int temp);
    //int broadcastData(char *pData,int length,int source = -1);        //发送广播数据
    //--------------------------------------
  
public:        //对类属性的间接访问方法
    void setDataProtection(_XBool temp);
    void disNeedMutual();
    int getClientSum() const;
    int getMyDelay() const;
    int getWrongFlag() const;
    int getIsServer() const;
    //+++++++++++++++++++++++++++++++++++++++
    //2009年12月29日
    //添加新的外部访问接口
    int getClientID(int order) const;
    int getIsOnline() const;
    //---------------------------------------
private:
    //下面是为了简化代码而定义的函数
    void addAClient(int clientSocket);
    int getAMessageServer(_XSocketData tempData,int clientID);
    int getAMessageClient(_XSocketData tempData);
    void broadcastDataByTcpIP(_XSocketData tempData);

private:   
    int getDataCheckKey(int length,char *data) const;                  //获得数据校验位
    void dataLockProc(int length,char *data,char lockKey);       //数据加密函数
    int dataUnlockProc(int length,char *data,char lockKey);      //数据解密函数
    void dataBale(_XSocketData *temp);                           //数据打包函数
    int dataRelieve(_XSocketData *temp);                         //数据解包函数，返回解包是否成功

#ifdef XEE_OS_WINDOWS
    static DWORD WINAPI acceptThread(void * pParam);             //服务器的accept线程
    static DWORD WINAPI broadcastThread(void * pParam);          //服务器的广播线程
    static DWORD WINAPI searchNetThread(void * pParam);          //搜索网络线程
    static DWORD WINAPI mutualThread(void * pParam);             //握手线程
    static DWORD WINAPI getDataThreadOnClient(void * pParam);    //客户端网络数据接受线程
    //下面的线程为3.5版本新增
    static DWORD WINAPI getDataThreadOnServer(void * pParam);      //服务器每检测到一个客户端则开启一个该线程(目前服务器中使用的是这种方式)
#endif

#ifdef XEE_OS_LINUX
    static void *acceptThread(void * pParam);                    //服务器的accept线程
    static void *broadcastThread(void * pParam);                 //服务器的广播线程
    static void *searchNetThread(void * pParam);                 //搜索网络线程
    static void *mutualThread(void * pParam);                    //握手线程
    //static void *getDataThread(void * pParam);                   //网络数据接受线程
    //下面的线程为3.5版本新增
    static void *getDataThreadOnClient(void * pParam);      //服务器每检测到一个客户端则开启一个该线程
    static void *getDataThreadOnServer(void * pParam);
#endif

    void resetSocketAddr(int socket);                            //设置socket可以被重用
   
//public:    //为了测试的方便这里定以为公共方法   
    int createServer(int port);                                  //建立服务器，建立服务器之后需要开启多个线程，握手线程 和 接受连接线程 和 应答线程
    void distroyServer();                                        //取消服务器端
    int createClient();                                          //建立客户端
    void distroyClient();                                        //取消客户端
    int connectToServer(sockaddr_in serverAddr);                 //客户端连接到服务器
   
    void setHostID(const char *p);                               //设置服务器网络标志符
   
    int searchNet(const char *p);                                //搜索指定网络
    int searchNet();
   
    void sendServerState();                                      //服务器向客户端发送服务器状态信息
   
    void dropNetProc(int clientID = 0);                          //网络通讯过程中自身掉线或者客户端掉线
   
    int recvDataProc(int sock,_XSocketData *temp_data);          //从某socket获取数据
    int recvDataFunction(int sock,char *temp_data,int length);    //重新封装数据接收函数，使得windows下socket非阻塞
    int sendDataFunction(int sock,_XSocketData temp_data,int length);    //重新封装数据发送函数，使得windows下socket非阻塞
   
    //下面为两套对外的公共接口，一套用于局域网，一套用于广域网
public:
    int connectToNet(int port,const char *serverID);             //自动功能的连接到局域网的服务器
   
    int xCreateServer(const char *serverIP,int port);            //广域网建立服务器
    int xCreateClient(const char *serverIP,int serverPort);      //广域网建立客户端并连接服务器
   
	//函数说明 <0:广播，0 - n发给指定可以联系用户
    int sendDataToNet(int clientOrder,char *p,int length);        //向网络发送数据
    int getDataFromNet(int *clientOrder,int *clientID,char **p,int *length);     //从网络读取数据

    void disconnect();        //断开网络
   
    int isRecvBuffEmpty() const;    //是否接受队列为空
    int isSendBuffEmptyBC() const;    //是否广播发送队列为空
                   
    _XSocket();
    ~_XSocket();
};

inline void _XSocket::setBroadCastWay(int temp)
{
    if(temp != BROADCAST_WAY_TCPIP)
    {
        m_broadCastWay = BROADCAST_WAY_UDP;
    }else
    {
        m_broadCastWay = BROADCAST_WAY_TCPIP;
    }
}
inline int _XSocket::getClientSum() const
{
    return m_clientSum;
}
inline int _XSocket::getMyDelay() const
{
    return m_myDelayTimes;
}
inline int _XSocket::getWrongFlag() const
{
    return m_WrongFlag;
}
inline int _XSocket::getIsServer() const
{
    return m_isServer;
}
inline int _XSocket::getClientID(int order) const
{
    if(m_clientSum <= 0) return 0;
    if(order < 0 || order >= MAX_CLIENT) return 0;
    return m_clientSocket[order];
}
inline int _XSocket::getIsOnline() const
{
    if(m_isServer == NET_STATE_SERVER && m_serverExitFlag == 1) return 0;
    if(m_isServer == NET_STATE_CLIENT && m_clientExitFlag == 1) return 0;
    return 1;
}
inline void _XSocket::resetSocketAddr(int socket)                             //设置socket可以被重用
{
    int opt = 1;
    int len = sizeof(opt);
    setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,len);
}
inline void _XSocket::disconnect()
{
    m_serverExitFlag =1;
    m_clientExitFlag =1;
    mySleep(500);
    closesocket(m_mySocket);
}
inline int _XSocket::isRecvBuffEmpty() const
{
    if(m_getDataBuff[m_getDataOldP].pData == NULL) return 1;
    else return 0;
}
inline int _XSocket::isSendBuffEmptyBC() const
{
    if(m_broadcastBuff[m_broadcastOldPoint].pData == NULL) return 1;
    else return 0;
}
inline void _XSocket::setDataProtection(_XBool temp)
{
    dataProtectionFlag = temp;
}
inline void _XSocket::disNeedMutual()
{
    m_needMutual = XFalse;
}

#endif
