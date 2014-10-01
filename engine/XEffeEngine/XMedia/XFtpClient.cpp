//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFtpClient.h"
#include "XNetCommon.h"

_XBool _XFtpClient::connectServer(const char * serverIP,int port)	//与服务器建立连接
{
	if(serverIP == NULL ||
		m_isConnect) return XFalse;		//防止重复连接
	
	if(WSAStartup(MAKEWORD(2,2),&m_wsaData) != 0) LogStr("Socket init error");
	m_cmdsocket = socket(AF_INET,SOCK_STREAM,0); //分配套接字
	sockaddr_in serveraddr;
	serveraddr.sin_addr.s_addr = inet_addr(serverIP);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	if(connect(m_cmdsocket,(const sockaddr *)&serveraddr,sizeof(sockaddr))==SOCKET_ERROR)
	{
		LogStr("connect error!");
		closesocket(m_cmdsocket);
		return XFalse;
	}
	//接收返回数据
	if(!recvRetCode()) return XFalse;
	if(m_retCode != 220) return XFalse;	//返回数据不对
	m_isConnect = XTrue;
	return XTrue;
}
//下面是对ftp封装的一些方法
_XBool _XFtpClient::setPort(SOCKET &listenSock)	//设置通讯端口
{
	if(!m_isConnect) return XFalse;
	//获取自身的ip
	//char hname[128];
	//gethostname(hname,sizeof(hname));
	//hostent *hent = gethostbyname(hname);
	//sprintf(hname,"%d.%d.%d.%d",(unsigned char)(hent->h_addr_list[0][0]),
	//	(unsigned char)(hent->h_addr_list[0][1]),
	//	(unsigned char)(hent->h_addr_list[0][2]),
	//	(unsigned char)(hent->h_addr_list[0][3]));
	//std::string dip = hname;
	std::string dip = getMyIP();
	std::string::size_type pos = 0;
	sockaddr_in localAddr;
	int sizeAddr = sizeof(sockaddr);
	UINT dataport;
	std::string str;
	listenSock = socket(AF_INET,SOCK_STREAM,0);
	//localAddr.sin_addr.s_addr = inet_addr(hname);
	localAddr.sin_addr.s_addr = inet_addr(dip.c_str());
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(0);
	if(bind(listenSock,(const sockaddr*)& localAddr,sizeAddr) == SOCKET_ERROR)
	{
		LogNull("bind error:%d",GetLastError());
		closesocket(listenSock);
		return XFalse;
	}
	listen(listenSock,5);
	getsockname(listenSock,(struct sockaddr*)&localAddr,&sizeAddr);
	dataport = ntohs(localAddr.sin_port);	
	while((pos = dip.find(".", pos)) != std::string::npos)
		dip.replace(pos,1, ",");

	sprintf(m_commandBuff,"PORT %s,%d,%d\r\n",dip.c_str(),dataport >> 8,dataport%256);
	if(!sendCommand(m_commandBuff))
	{
		closesocket(listenSock);
		return XFalse;
	}
	if(m_retCode != 200)
	{
		closesocket(listenSock);
		return XFalse;	//返回数据不对
	}
	//这里修改为不立刻链接，在一些情况下会造成卡死
	////下面等待服务器的连接
	//sockaddr_in clientaddr;
	//int sizeaddr = sizeof(sockaddr);
	//SOCKET clientsocket = accept(listenSock,(struct sockaddr*)&clientaddr,&sizeaddr);
	//closesocket(listenSock);
	//listenSock = clientsocket;
	return XTrue;
}
_XBool _XFtpClient::setPasv(SOCKET &listenSock) 
{ 
	sprintf(m_commandBuff,"PASV\r\n");
	if(!sendCommand(m_commandBuff)) return XFalse;
	if(m_retCode != 227) return XFalse;	//返回数据不对
    listenSock = socket(AF_INET,SOCK_STREAM,0); 
    unsigned int v[6]; 
    union { 
        struct sockaddr sa; 
        struct sockaddr_in in; 
    } sin; 
    sscanf(m_commandBuff,"%*[^(](%u,%u,%u,%u,%u,%u",&v[2],&v[3],&v[4],&v[5],&v[0],&v[1]); 
    sin.sa.sa_family = AF_INET; 
    sin.sa.sa_data[2] = v[2]; 
    sin.sa.sa_data[3] = v[3]; 
    sin.sa.sa_data[4] = v[4]; 
    sin.sa.sa_data[5] = v[5]; 
    sin.sa.sa_data[0] = v[0]; 
    sin.sa.sa_data[1] = v[1]; 
 
    int on =1; 
    if(setsockopt(listenSock,SOL_SOCKET,SO_REUSEADDR, 
        (const char*) &on,sizeof(on)) == -1) 
    { 
        perror("setsockopt"); 
        closesocket(listenSock); 
        return XFalse; 
    } 
    struct linger lng = { 0, 0 }; 
 
    if(setsockopt(listenSock,SOL_SOCKET,SO_LINGER, 
        (const char*) &lng,sizeof(lng)) == -1) 
    { 
        perror("setsockopt"); 
        closesocket(listenSock); 
        return XFalse; 
    } 
 
	if(connect(listenSock,(sockaddr*)&sin,sizeof(sin)) == SOCKET_ERROR)
	{
		closesocket(listenSock);
		return XFalse;
	}
	return XTrue;
}
_XBool _XFtpClient::sendFileList()	//获取文件列表
{//这个文件列表的数据并没有传递出来
	if(!m_isConnect) return XFalse;
	SOCKET listensocket;
	bool isPortMode = true;	//是否使用port模式
	if(!setPort(listensocket))
	{
		if(!setPasv(listensocket)) return XFalse;
		isPortMode = false;
	}
	if(!sendCommand("LIST\r\n") ||
		m_retCode != 125)
	{
		closesocket(listensocket);
		return XFalse;	//返回数据不对
	}
	//下面开始接收数据
	SOCKET clientsocket = listensocket;
	if(isPortMode)
	{
		sockaddr_in clientaddr;
		int sizeaddr = sizeof(sockaddr);
		clientsocket = accept(listensocket,(struct sockaddr*)&clientaddr,&sizeaddr);
		closesocket(listensocket);
	}
	char buf[RECV_BUFF_SIZE + 1];
	int ret = 0;
	while(true)
	{
		ret = recv(clientsocket,buf,RECV_BUFF_SIZE,0);
		if(ret > 0)
		{
			buf[ret]='\0';
			LogNull("%s",buf);
		}else
		{//数据发送完成
			break;
		}
	}
	//查看数据是否已经发送完成
	if(!recvRetCode()) return XFalse;
	if(m_retCode != 226) return XFalse;
	closesocket(clientsocket);
	return XTrue;
}
_XBool _XFtpClient::sendDownloadFile(const char *filename)	//下载指定文件
{
	if(!m_isConnect ||
		filename == NULL) return XFalse;
	SOCKET listensocket;
	bool isPortMode = true;	//是否使用port模式
	if(!setPort(listensocket))
	{
		if(!setPasv(listensocket)) return XFalse;
		isPortMode = false;
	}
	strcpy(m_commandBuff,"RETR ");
	strcat(m_commandBuff,filename);
	strcat(m_commandBuff,"\r\n");
	if(!sendCommand(m_commandBuff) ||
		m_retCode != 150)
	{
		closesocket(listensocket);
		return XFalse;
	}
	//下面开始接收文件数据
	SOCKET clientsocket = listensocket;
	if(isPortMode)
	{
		sockaddr_in clientaddr;
		int sizeaddr = sizeof(sockaddr);
		clientsocket = accept(listensocket,(struct sockaddr*)&clientaddr,&sizeaddr);
		closesocket(listensocket);
	}
	std::string str = std::string(".") + "//" + filename;
	ofstream file;
	file.open(str.c_str(),ios::binary);
	int ret = 0;
	char buf[RECV_BUFF_SIZE];
	while(true)
	{
		ret = recv(clientsocket,buf,RECV_BUFF_SIZE,0);
		if(ret > 0) file.write(buf,ret);
		else break;	//数据接收完毕
	}
	file.close();
	closesocket(clientsocket);
	if(!recvRetCode()) return XFalse;
	if(m_retCode != 226) return XFalse;
	return XTrue;
}
_XBool _XFtpClient::sendUpdateFile(const char *filename)	//上传文件
{
	if(!m_isConnect ||
		filename == NULL) return XFalse;
	//检查文件是否存在
	std::string str = std::string(".") + "//" + filename;
	ifstream file;
	file.open(str.c_str(),ios::binary);
	if(!file.is_open())
	{
		LogStr("file not exist!");
		return XFalse;	//如果需要上传的文件不存在则直接返回
	}
	//filebuf *fb = file.rdbuf();
	file.seekg(0, std::ios::end);//这里获得文件的长度
	int length = file.tellg(); 
	file.seekg(0, std::ios::beg);
	if(length < 0)
	{
		file.close();
		return XFalse;
	}
	//下面建立于服务器的连接
	SOCKET listensocket;
	bool isPortMode = true;	//是否使用port模式
	if(!setPort(listensocket))
	{
		if(!setPasv(listensocket))
		{
			file.close();
			return XFalse;
		}
		isPortMode = false;
	}
	//发送文件存储命令
	strcpy(m_commandBuff,"STOR ");
	strcat(m_commandBuff,filename);
	strcat(m_commandBuff,"\r\n");
	if(!sendCommand(m_commandBuff) ||
		(m_retCode != 150 && m_retCode != 125))
	{
		closesocket(listensocket);
		file.close();
		return XFalse;
	}
	SOCKET clientSocket = listensocket;
	if(isPortMode)
	{//如果是port模式的话，这里等待服务器的链接
		sockaddr_in clientaddr;
		int sizeaddr = sizeof(sockaddr);
		clientSocket = accept(listensocket,(struct sockaddr*)&clientaddr,&sizeaddr);
		closesocket(listensocket);
	}
	//下面开始上传数据
	int left = length;
	char buf[RECV_BUFF_SIZE];
	while(left > 0)
	{//发送文件数据直到发送完毕
		file.read(buf,RECV_BUFF_SIZE<left?RECV_BUFF_SIZE:left);
		send(clientSocket,buf,RECV_BUFF_SIZE<left?RECV_BUFF_SIZE:left,0);
		left -= RECV_BUFF_SIZE;
		//Sleep(1);
	}
	//printf("\n");
	file.close();
	closesocket(clientSocket);
	if(!recvRetCode() ||
		m_retCode != 226) return XFalse;
	return XTrue;
}
_XBool _XFtpClient::sendUpdateFile(const char *inFilename,const char *outFilename)
{
	if(!m_isConnect ||
		inFilename == NULL ||
		outFilename == NULL) return XFalse;
	//检查文件是否存在
	std::string str;
	if(isAbsolutePath(inFilename)) str = inFilename;	//绝对路径
	else str = std::string(".") + "//" + inFilename;	//相对路径
	ifstream file;
	file.open(str.c_str(),ios::binary);
	if(!file.is_open())
	{
		LogStr("file not exist!");
		return XFalse;	//如果文件不存在则直接返回
	}
	//filebuf *fb = file.rdbuf();
	file.seekg(0, std::ios::end);//这里获得文件的长度
	int length = file.tellg(); 
	file.seekg(0, std::ios::beg);
	if(length < 0)
	{//文件打开失败
		file.close();
		return XFalse;
	}
	//连接服务器
	SOCKET listensocket;
	bool isPortMode = true;	//是否使用port模式
	if(!setPort(listensocket))
	{
		if(!setPasv(listensocket))
		{
			file.close();
			return XFalse;
		}
		isPortMode = false;
	}
	//发送文件上传命令
	strcpy(m_commandBuff,"STOR ");
	strcat(m_commandBuff,outFilename);
	strcat(m_commandBuff,"\r\n");
	if(!sendCommand(m_commandBuff) ||
		(m_retCode != 150 && m_retCode != 125))
	{
		closesocket(listensocket);
		file.close();
		return XFalse;
	}
	SOCKET clientSocket = listensocket;
	if(isPortMode)
	{//如果是port模式的话，这里等待服务器的链接
		sockaddr_in clientaddr;
		int sizeaddr = sizeof(sockaddr);
		clientSocket = accept(listensocket,(struct sockaddr*)&clientaddr,&sizeaddr);
		closesocket(listensocket);
	}
	//下面开始传输数据
	int left = length;
	char buf[RECV_BUFF_SIZE];
	while(left > 0)
	{//发送文件数据直到发送完毕
		file.read(buf,RECV_BUFF_SIZE<left?RECV_BUFF_SIZE:left);
		send(clientSocket,buf,RECV_BUFF_SIZE<left?RECV_BUFF_SIZE:left,0);
		left -= RECV_BUFF_SIZE;
		//Sleep(1);
	}
	//printf("\n");
	file.close();
	closesocket(clientSocket);
	if(!recvRetCode() ||
		m_retCode != 226) return XFalse;
	return XTrue;
}