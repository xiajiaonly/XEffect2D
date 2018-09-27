#ifndef _JIA_XSIMPLETCPCLIENT_
#define _JIA_XSIMPLETCPCLIENT_
#include <string>
namespace XE{
class XSimpleTcpClient
{
private:
	bool m_isConnected;	//是否已经链接
	WSADATA m_wsaData;	
	SOCKET m_cmdsocket;		//连接服务器的套接字
public:
	XSimpleTcpClient()
		:m_isConnected(false)
	{}
	~XSimpleTcpClient(){disConnect();}
	bool connectServer(const std::string& ip,int port);
	void disConnect();
	bool sendData(const void *data,int len);
	bool sendData(const std::string& str){return sendData(str.data(),str.size());}
	int recvData(void *data,int len);
	bool getIsConnected()const{return m_isConnected;}
};
}
#endif