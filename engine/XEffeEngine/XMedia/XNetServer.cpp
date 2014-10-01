//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XNetServer.h"

_XBool _XNetServer::createServer(int port,_XBool autoIP)
{
	if(m_connectState == CONNECT_STATE_CONNECT) return XFalse;	//防止重复连接
	m_autoIPMode = autoIP;
	m_serverPort = port;
	//连接服务器
	if(WSAStartup(MAKEWORD(2,2),&m_wsaData) != 0) printf("Socket init error\n");
	m_serverSocket = socket(AF_INET,SOCK_STREAM,0); //分配套接字
	if(m_serverSocket == -1)
	{//建立服务器套接字失败
		printf("Socket create error!\n");
		return XFalse;
	}
	resetSocketAddr(m_serverSocket);
	int opt = SO_REUSEADDR;
	setsockopt(m_serverSocket,SOL_SOCKET,SO_REUSEADDR,(const char *)&opt,sizeof(opt));
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(m_serverPort);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(m_serverSocket,(sockaddr *)&server,sizeof(sockaddr)) == -1)
	{//绑定套接字失败
		printf("Bind fail!\n");
		return XFalse;
	}
	if(listen(m_serverSocket,SOMAXCONN) == -1)
	{
		printf("Listen error!\n");
		return XFalse;
	}
	m_isExit = XFalse;
	m_connectState = CONNECT_STATE_DISCONNECT;
	//m_clientConnect = XFalse;
	//开启通讯线程
	if(CreateThread(0,0,recvThread,this,0,&m_recvThread) == 0)
	{//线程建立失败
		printf("Server command thread create error!\n");
		return XFalse;
	}
	if(CreateThread(0,0,sendThread,this,0,&m_sendThread) == 0)
	{//线程建立失败
		printf("Server command thread create error!\n");
		return XFalse;
	}
	if(CreateThread(0,0,acceptThread,this,0,&m_acceptThread) == 0)
	{//线程建立失败
		printf("Server command thread create error!\n");
		return XFalse;
	}
	if(m_autoIPMode)
	{//开启广播线程
		if(CreateThread(0,0,boardcastThread,this,0,&m_boardcastThread) == 0)
		{//线程建立失败
			printf("Server boardcast thread create error!\n");
			return XFalse;
		}
	}

	return XTrue;
}
_XBool _XNetServer::sendDataSocket(char * data,int len)
{//每次发送4096个数据
	if(data == NULL || len <= 0) return XFalse;
	int ret = 0;
	//方案1：来多少数据都一次发送
	ret = send(m_netSocket,data,len,0);
	if(ret < len)
	{
		printf("发送错误！\n");
		return XFalse;
	}
	return XTrue;
	//方案2：限制发送的最小数据单位
	//int offset = 0;
	//while(true)
	//{
	//	if(len - offset < 4096)
	//	{
	//		ret = send(m_netSocket,data + offset,len - offset,0);
	//		if(ret < len - offset)
	//		{
	//			printf("发送错误！\n");
	//			return XFalse;
	//		}
	//		return XTrue;
	//	}else
	//	{
	//		ret = send(m_netSocket,data + offset,4096,0);
	//		if(ret < 4096)
	//		{
	//			printf("发送错误！\n");
	//			return XFalse;
	//		}
	//		offset += 4096;
	//	}
	//	Sleep(1);
	//}
	//return XTrue;
}
_XBool _XNetServer::getDataPacket(unsigned char *buff,int len)
{
	if(len <= 0) return XTrue;
	int offset = 0;
	if(m_recvPacketSize == 0)
	{
		if(len >= PACKET_HEAD_LEN)
		{//完整的包头
			m_recvPacket = createMem<_XNetData>();
			if(m_recvPacket == NULL) return XFalse;
			m_recvPacket->type = (_XNetDataType)buff[0];
			memcpy(&(m_recvPacket->dataLen),buff + 1,sizeof(int));
			m_recvPacket->data = createArrayMem<unsigned char>(m_recvPacket->dataLen);
			if(m_recvPacket->data == NULL) return XFalse;
			if(len - PACKET_HEAD_LEN >= m_recvPacket->dataLen)
			{//数据完整
				m_recvPacket->isEnable = XTrue;
				memcpy(m_recvPacket->data,buff + PACKET_HEAD_LEN,m_recvPacket->dataLen);
				offset = PACKET_HEAD_LEN + m_recvPacket->dataLen;
				m_mutex.Lock();
				m_recvDataBuff.push_back(m_recvPacket);
				m_mutex.Unlock();
				m_recvPacketSize = 0;
				//继续迭代
				return getDataPacket(buff + offset,len - offset);
			}else
			{//数据不完整
				m_recvPacket->isEnable = XFalse;	//数据包尚不完整
				memcpy(m_recvPacket->data,buff + PACKET_HEAD_LEN,len - PACKET_HEAD_LEN);
				m_recvPacketSize = len;
				return XTrue;
			}
		}else
		{//不完整的包头
			m_recvPacketSize = len;
			memcpy(m_packetHeadData,buff,len);
			return XTrue;
		}
	}else
	{
		if(m_recvPacketSize >= PACKET_HEAD_LEN)
		{//包头已经接收完整
			if(len >= m_recvPacket->dataLen - m_recvPacketSize + PACKET_HEAD_LEN)
			{//完整的
				memcpy(m_recvPacket->data + m_recvPacketSize - PACKET_HEAD_LEN,buff,m_recvPacket->dataLen - m_recvPacketSize + PACKET_HEAD_LEN);
				m_recvPacket->isEnable = XTrue;
				offset = m_recvPacket->dataLen - m_recvPacketSize + PACKET_HEAD_LEN;
				m_mutex.Lock();
				m_recvDataBuff.push_back(m_recvPacket);
				m_mutex.Unlock();
				m_recvPacketSize = 0;
				//迭代
				return getDataPacket(buff + offset,len - offset);
			}else
			{//不完整的
				memcpy(m_recvPacket->data + m_recvPacketSize - PACKET_HEAD_LEN,buff,len);
				m_recvPacketSize += len;
				return XTrue;
			}
		}else
		{//包头尚未接收完整
			if(m_recvPacketSize + len >= PACKET_HEAD_LEN)
			{//包头将会完整
				memcpy(m_packetHeadData + m_recvPacketSize,buff,PACKET_HEAD_LEN - m_recvPacketSize);
				//解析包头
				m_recvPacket = createMem<_XNetData>();
				if(m_recvPacket == NULL) return XFalse;
				m_recvPacket->type = (_XNetDataType)m_packetHeadData[0];
				memcpy(&(m_recvPacket->dataLen),m_packetHeadData + 1,sizeof(int));
				m_recvPacket->data = createArrayMem<unsigned char>(m_recvPacket->dataLen);
				if(m_recvPacket->data == NULL) return XFalse;
				//解析余下的数据
				if(len - (PACKET_HEAD_LEN - m_recvPacketSize) >= m_recvPacket->dataLen)
				{//数据完整
					m_recvPacket->isEnable = XTrue;
					memcpy(m_recvPacket->data,buff + (PACKET_HEAD_LEN - m_recvPacketSize),m_recvPacket->dataLen);
					offset = PACKET_HEAD_LEN - m_recvPacketSize + m_recvPacket->dataLen;
					m_mutex.Lock();
					m_recvDataBuff.push_back(m_recvPacket);
					m_mutex.Unlock();
					m_recvPacketSize = 0;
					//继续迭代
					return getDataPacket(buff + offset,len - offset);
				}else
				{//数据不完整
					m_recvPacket->isEnable = XFalse;	//数据包尚不完整
					memcpy(m_recvPacket->data,buff + (PACKET_HEAD_LEN - m_recvPacketSize),len - (PACKET_HEAD_LEN - m_recvPacketSize));
					m_recvPacketSize += len;
					return XTrue;
				}
			}else
			{//包头仍然不完整
				memcpy(m_packetHeadData + m_recvPacketSize,buff,len);
				m_recvPacketSize += len;
				return XTrue;
			}
		}
	}
}
DWORD WINAPI _XNetServer::recvThread(void * pParam)
{
	_XNetServer &pPar = *(_XNetServer *)pParam;
	int buffLen = 4096;
	char recvBuff[4096];
	int ret = 0;
	pPar.m_recvPacketSize = 0;
	while(true)
	{
		if(pPar.m_isExit) break;
		if(pPar.m_connectState == CONNECT_STATE_DISCONNECT)
		{
			if(pPar.m_recvPacketSize != 0)
			{//如果网络断开，但是最后一个包没有接收完整，则丢弃最后一个包
				XDELETE(pPar.m_recvPacket);	//丢弃不完整的包
				pPar.m_recvPacketSize = 0;
			}
			Sleep(1);
			continue;
		}
		//下面接收数据
		//if(pPar.m_clientConnect)
		{
			//非阻塞访问
			fd_set readfds;
			timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = 0;
			FD_ZERO(&readfds);
			FD_SET(pPar.m_netSocket,&readfds);
			int test_error = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
			if(test_error > 0 && FD_ISSET(pPar.m_netSocket,&readfds))
			{//可以接受数据
				FD_CLR(pPar.m_netSocket, &readfds);
				ret = recv(pPar.m_netSocket,recvBuff,buffLen,0);
				if(ret == SOCKET_ERROR) pPar.m_connectState = CONNECT_STATE_DISCONNECT;	//连接断开
				if(ret > 0) pPar.getDataPacket((unsigned char *)recvBuff,ret);//接收到数据
			}
		}
		Sleep(1);
	}
	return 1;
}
DWORD WINAPI _XNetServer::sendThread(void * pParam)
{
	_XNetServer &pPar = *(_XNetServer *)pParam;
	int buffSize = 4096;
	char * sendBuff = createArrayMem<char>(buffSize);
	if(sendBuff == NULL) return 0;
	while(true)
	{
		if(pPar.m_isExit) break;
		if(pPar.m_connectState == CONNECT_STATE_DISCONNECT)
		{
			Sleep(1);
			continue;
		}
		pPar.m_mutex.Lock();
		if(pPar.m_sendDataBuff.size() > 0)// && pPar.m_clientConnect)
		{//有数据需要发送
			_XNetData *tempData = (* pPar.m_sendDataBuff.begin());
			pPar.m_sendDataBuff.pop_front();
			pPar.m_mutex.Unlock();
			if(tempData->isEnable)
			{
				if(tempData->dataLen + PACKET_HEAD_LEN > buffSize)
				{//数据长度不够需要重新分配数据尺寸
					XDELETE_ARRAY(sendBuff);
					buffSize = tempData->dataLen + PACKET_HEAD_LEN;
					sendBuff = createArrayMem<char>(buffSize);
					if(sendBuff == NULL) return 0;
				}
				sendBuff[0] = tempData->type;
				memcpy(sendBuff + 1,&tempData->dataLen,sizeof(int));
				memcpy(sendBuff + PACKET_HEAD_LEN,tempData->data,tempData->dataLen);
				//发送数据
				if(!pPar.sendDataSocket(sendBuff,tempData->dataLen + PACKET_HEAD_LEN))
				{//发送失败则将数据推回
					pPar.m_mutex.Lock();
					pPar.m_sendDataBuff.push_front(tempData);
					pPar.m_mutex.Unlock();
				}else
				{
					showNetData(tempData);
					XDELETE(tempData);	//释放资源
				}
			}else
			{//无效的数据直接丢弃
				showNetData(tempData);
				XDELETE(tempData);	//释放资源
			}
		}else
		{
			pPar.m_mutex.Unlock();
		}
		Sleep(1);
	}
	XDELETE_ARRAY(sendBuff);
	return 1;
}
DWORD WINAPI _XNetServer::acceptThread(void * pParam)
{//默认每次只连接一个客户端
	_XNetServer &pPar = *(_XNetServer *)pParam;
	int sinSize = sizeof(sockaddr_in);
	sockaddr_in clientAddr;
	int clientSock;	//客户端的套接字
	while(true)
	{
		if(pPar.m_isExit) break;
		//if(pPar.m_connectState == CONNECT_STATE_DISCONNECT) break;
		clientSock = accept(pPar.m_serverSocket,(sockaddr *)&clientAddr,&sinSize);
		if(clientSock == -1)
		{//连接失败
			printf("Accept error!\n");
			return 0;
		}else
		{//连接成功
			pPar.m_netSocket = clientSock;
			printf("client connect!\n");
			//pPar.m_clientConnect = XTrue;
			if(pPar.m_connectState == CONNECT_STATE_DISCONNECT)
				pPar.m_connectState = CONNECT_STATE_CONNECT;
		}
		Sleep(1);
	}
    return 1;
}
DWORD WINAPI _XNetServer::boardcastThread(void * pParam)
{
	_XNetServer &pPar = *(_XNetServer *)pParam;
	unsigned char tempCheck;
	int offset = 0;
	int len;
	int bBroadcast = 1;
	sockaddr_in addrUDP;

	memset(&addrUDP,0,sizeof(addrUDP));
	addrUDP.sin_family  = AF_INET;
	addrUDP.sin_port  = htons(0);
	addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
    int sendSocket = socket(AF_INET,SOCK_DGRAM,0);
	if(sendSocket < 0) return 0;
	resetSocketAddr(sendSocket); 
	if(setsockopt(sendSocket,SOL_SOCKET,SO_BROADCAST,(char *)&bBroadcast,sizeof(bBroadcast)) == SOCKET_ERROR) return 0;
	if(bind(sendSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return 0;	//发送端口

    memset(&addrUDP,0,sizeof(addrUDP)); 
	addrUDP.sin_family = AF_INET;
    addrUDP.sin_port = htons(BOARDCAST_PORT);
    addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
    int recvSocket = socket(AF_INET,SOCK_DGRAM,0);
	if(recvSocket < 0) return 0;
	resetSocketAddr(recvSocket);
	if(bind(recvSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return 0;	//接收端口

    memset(&addrUDP,0,sizeof(addrUDP)); 
	addrUDP.sin_family = AF_INET;
	addrUDP.sin_port = htons(BOARDCAST_PORT);
	addrUDP.sin_addr.s_addr = htonl(INADDR_BROADCAST);       
	//addrUDP.sin_addr.s_addr = inet_addr("192.168.1.255"); 
	//下面准备需要发送的数据
	unsigned char needSendData[BOARDCAST_DATA_LEN];
	needSendData[0] = 0xcc;needSendData[1] = 0x00;offset = 2;
	memcpy(needSendData + offset,&pPar.m_serverPort,sizeof(int));offset += sizeof(int);
	len = strlen(pPar.m_projectStr);
	memcpy(needSendData + offset,&len,sizeof(int));offset += sizeof(int);
	memcpy(needSendData + offset,pPar.m_projectStr,len);offset += len;
	tempCheck = 0;
	for(int i = 1;i < offset;++ i)
	{
		tempCheck += needSendData[i];
	}
	needSendData[offset] = tempCheck;offset += 1;
	needSendData[offset] = 0xdd;offset += 1;

	sockaddr_in addrPeer;
#ifdef XEE_OS_WINDOWS
	int nSize = sizeof(addrPeer);
#endif
#ifdef XEE_OS_LINUX 
	socklen_t nSize = sizeof(addrPeer);
#endif
	int nRecvLength = 0;
	unsigned char szRecvBuffer[BOARDCAST_DATA_LEN];
	memset(szRecvBuffer,0,BOARDCAST_DATA_LEN);
	char recvProjectStr[PROJECT_STRING_LEN];
	while(true)
	{//下面开始接收数据
		if(pPar.m_isExit) break;
		memset(szRecvBuffer,0,BOARDCAST_DATA_LEN);
		//非阻塞访问
		fd_set readfds;
		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		FD_ZERO(&readfds);
		FD_SET(recvSocket,&readfds);
		int test_error = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
		if(test_error > 0 && FD_ISSET(recvSocket,&readfds))
		{//可以接受数据
			FD_CLR(recvSocket, &readfds);
			nRecvLength = recvfrom(recvSocket,(char *)szRecvBuffer,BOARDCAST_DATA_LEN,0,(sockaddr*)&addrPeer,&nSize);
			if(nRecvLength > 2 + sizeof(int))
			{//如果接收到数据
				//|0xcc   |0x00(server),0x01(client)|									|0xdd|
				//|Head(1)|Type(1)|Port(4)[for server]|StrLen(4)|String(x)|Check(1)|End(1)|
				if(szRecvBuffer[0] != 0xcc) continue;	//数据不合法直接丢弃
				if(szRecvBuffer[1] != 0x01) continue;	//只处理从客户端接收的数据
				memcpy(&len,szRecvBuffer + 2,sizeof(int));
				if(len <= 0 || nRecvLength < 4 + sizeof(int) + len) continue;
				//检查数据校验和
				tempCheck = 0;
				for(int i = 0;i < len + 1 + sizeof(int);++ i)
				{
					tempCheck += szRecvBuffer[i + 1];
				}
				if(tempCheck != szRecvBuffer[2 + sizeof(int) + len]) continue;
				memcpy(recvProjectStr,szRecvBuffer + 2 + sizeof(int),len);
				recvProjectStr[len] = '\0';
				printf("recv:%s\n",recvProjectStr);
				if(strcmp(recvProjectStr,pPar.m_projectStr) != 0) continue;
				//下面回应客户端的请求
				sendto(sendSocket,(char *)needSendData,offset,0,(sockaddr *)&addrUDP,sizeof(addrUDP));
			}
		}
		Sleep(1);
	}
	closesocket(recvSocket);
	closesocket(sendSocket);
	return 0;
}
void _XNetServer::release()
{
	m_isExit = XTrue;	//标记主动退出
	if(m_connectState == CONNECT_STATE_CONNECT) m_connectState = CONNECT_STATE_DISCONNECT;
	m_mutex.Lock();
	std::deque<_XNetData *>::iterator i;
	for(i = m_recvDataBuff.begin();i != m_recvDataBuff.end();++ i)
	{
		XDELETE(*i);
	}
	m_recvDataBuff.clear();
	for(i = m_sendDataBuff.begin();i != m_sendDataBuff.end();++ i)
	{
		XDELETE(*i);
	}
	m_sendDataBuff.clear();
	m_mutex.Unlock();
	closesocket(m_netSocket);
	closesocket(m_serverSocket);
}