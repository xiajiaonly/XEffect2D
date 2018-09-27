#include "XStdHead.h"
#include "XSimpleTcpClient.h"
namespace XE{
bool XSimpleTcpClient::connectServer(const std::string& ip,int port)
{
	if(m_isConnected) return XFalse;		//防止重复连接
	
	if(WSAStartup(MAKEWORD(2,2),&m_wsaData) != 0) LogStr("Socket init error");
	m_cmdsocket = socket(AF_INET,SOCK_STREAM,0); //分配套接字
	sockaddr_in serveraddr;
	serveraddr.sin_addr.s_addr = inet_addr(ip.c_str());
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	if(connect(m_cmdsocket,(const sockaddr *)&serveraddr,sizeof(sockaddr))==SOCKET_ERROR)
	{
		LogStr("connect error!");
		closesocket(m_cmdsocket);
		return XFalse;
	}
	m_isConnected = true;
	return true;
}
void XSimpleTcpClient::disConnect()
{
	if(!m_isConnected) return;
	closesocket(m_cmdsocket);
	m_isConnected = XFalse;
}
bool XSimpleTcpClient::sendData(const void *data,int len)
{
	if(!m_isConnected) return false;
	if(send(m_cmdsocket,(const char *)data,len,0) == SOCKET_ERROR) return false;
	return true;
}
int XSimpleTcpClient::recvData(void *data,int len)
{
	if(!m_isConnected) return false;
	return recv(m_cmdsocket,(char *)data,len,0);
}
}