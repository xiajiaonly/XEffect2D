#ifndef _JIA_XFTPSERVER_
#define _JIA_XFTPSERVER_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.12
//--------------------------------
#include "XFtpCommon.h"
//尚未完成
//建议客户端的连接如果在x时间内没有通讯，则自动断开
enum _XFtpTransnationType	//FTP数据传输的模式
{
	FTP_TRANSNAION_TYPE_ASCII,	//字符串的传输方式
	FTP_TRANSNAION_TYPE_BIN,	//二进制的传输方式
};
struct _XFtpClientSockInfo
{
	void * pClass;
	int clientSock;
	int clientDataSock;	//数据传输的端口
	sockaddr_in clientAddr;	
	_XFtpTransnationType transnatType;					//当前设置的传输模式
	char nowPath[MAX_FILE_NAME_LENGTH];	//当前设置的相对路径

	_XFtpClientSockInfo()
		:pClass(NULL)
		,transnatType(FTP_TRANSNAION_TYPE_ASCII)
	{
		nowPath[0] = '/';
		nowPath[1] = '\0';
	}
};
class _XFtpServer
{
private:
	_XBool m_isCreated;	//服务器是否已经建立起来
	int m_serverSock;	//服务器的套接字
	int m_clientSum;	//当前客户端的数量
	int m_allClientSum;	//所有访问的客户端的数量
	WSADATA m_wsaData;

	_XBool sendData(int clientSock,const char * data,int len)	//向客户端发送数据
	{
	//	if(!m_isCreated) return XFalse;	//尚未建立服务器则返回失败
		if(data == NULL || len <= 0) return XFalse;
		int ret = send(clientSock,data,len,0);
		if(ret < 0) return XFalse;	//数据发送失败
		return XTrue;
	}
	_XBool recvData(int clientSock,char * p)	//从客户端获取数据
	{
	//	if(!m_isCreated) return XFalse;	//尚未建立服务器则返回失败
		if(p == NULL) return XFalse;
		int ret = recv(clientSock,p,RECV_BUFF_SIZE,0);
		if(ret < 0) return XFalse;	//接收数据失败
		p[ret] = '\0';
		printf("%s",p);	//后台显示接收到的数据
		return XTrue;
	}
	_XFtpUserType clientLoginProc(int clientSock);//处理客户端登陆操作，返回用户的类型，0:无效,1:普通用户,2:特殊用户
	pthread_t m_serverCMDThread;
	pthread_t m_serverAcceptThread;
#ifdef XEE_OS_WINDOWS
    static DWORD WINAPI serverCMDThread(void * pParam);		//服务器命令线程
	static DWORD WINAPI serverAcceptThread(void * pParam);	//服务器连接线程
	static DWORD WINAPI serverRequestThread(void * pParam);	//服务器请求回应线程(尚未完成)
#endif
public:
	_XBool createServer();	//建立服务器，开解accept线程，没接收一个连接，则开启一个客户端处理线程用于处理客户端请求
	void close()
	{//释放所有的资源，并断开所有的连接(尚未完成)
		if(!m_isCreated) return;
		//todo:
		m_isCreated = XFalse;
	}
	_XFtpServer()
		:m_isCreated(XFalse)
		,m_clientSum(0)
		,m_allClientSum(0)
	{}
	~_XFtpServer(){close();}
};

#endif 