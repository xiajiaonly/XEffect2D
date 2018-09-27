#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XNetClient.h"
namespace XE{
XBool XNetClient::connectToServer(const char *ip,int port)
{
	if(m_connectState == CONNECT_STATE_CONNECT) return XFalse;	//防止重复连接
	if(ip == NULL) return XFalse;
	//连接服务器
	if(!getANewSocket(m_netSocket)) return XFalse;
	sockaddr_in serveraddr;
	serveraddr.sin_addr.s_addr = inet_addr(ip);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	if(connect(m_netSocket,(const sockaddr *)&serveraddr,sizeof(sockaddr)) == SOCKET_ERROR)
	{
		LogStr("connect error!");
		closesocket(m_netSocket);
		return XFalse;
	}
	m_connectState = CONNECT_STATE_CONNECT;
	//开启通讯线程
	if(CreateThread(0,0,recvThread,this,0,&m_recvThread) == 0)
	{//线程建立失败
		LogStr("服务器接受线程建立失败!");
		return XFalse;
	}
	if(CreateThread(0,0,sendThread,this,0,&m_sendThread) == 0)
	{//线程建立失败
		LogStr("服务器发送线程建立失败!");
		return XFalse;
	}

	return XTrue;
}
XBool XNetClient::sendBoardcast()
{
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
	if(sendSocket < 0) return XFalse;
	resetSocketAddr(sendSocket); 
	if(setsockopt(sendSocket,SOL_SOCKET,SO_BROADCAST,(char *)&bBroadcast,sizeof(bBroadcast)) == SOCKET_ERROR) return XFalse;
	if(bind(sendSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return XFalse;	//接收端口

    memset(&addrUDP,0,sizeof(addrUDP)); 
	addrUDP.sin_family = AF_INET;
	addrUDP.sin_port = htons(BOARDCAST_PORT);
#ifdef WITH_LOCAL_BOARDCAST_IP
	addrUDP.sin_addr.s_addr = inet_addr(BOARDCASR_IP); 
#else
	addrUDP.sin_addr.s_addr = htonl(INADDR_BROADCAST);       
#endif

	//下面准备需要发送的数据
	unsigned char needSendData[BOARDCAST_DATA_LEN];
	needSendData[0] = 0xcc;needSendData[1] = 0x01;offset = 2;
	len = strlen(m_projectStr);
	memcpy(needSendData + offset,&len,sizeof(int));offset += sizeof(int);
	memcpy(needSendData + offset,m_projectStr,len);offset += len;
	tempCheck = 0;
	for(int i = 1;i < offset;++ i)
	{
		tempCheck += needSendData[i];
	}
	needSendData[offset] = tempCheck;offset += 1;
	needSendData[offset] = 0xdd;offset += 1;

	if(sendto(sendSocket,(char *)needSendData,offset,0,(sockaddr *)&addrUDP,sizeof(addrUDP)) < offset) 
	{
		closesocket(sendSocket);
		return XFalse;	//向网络广播数据
	}

	closesocket(sendSocket);
	return XTrue;
}
//XBool XNetClient::recvBoardcast()
//{
//	unsigned char tempCheck;
//	int offset = 0;
//	int len;
//	sockaddr_in addrUDP;
//    memset(&addrUDP,0,sizeof(addrUDP)); 
//	addrUDP.sin_family = AF_INET;
//    addrUDP.sin_port = htons(BOARDCAST_PORT);
//    addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
//    int recvSocket = socket(AF_INET,SOCK_DGRAM,0);
//	if(recvSocket < 0) return XFalse;
//	resetSocketAddr(recvSocket);
//	if(bind(recvSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return XFalse;	//接收端口
//
//	sockaddr_in addrPeer;
//	int tempServerPort;
//#ifdef XEE_OS_WINDOWS
//	int nSize = sizeof(addrPeer);
//#endif
//#ifdef XEE_OS_LINUX 
//	socklen_t nSize = sizeof(addrPeer);
//#endif
//	int nRecvLength = 0;
//	char szRecvBuffer[BOARDCAST_DATA_LEN] = {0};	//此处初始化有无
//	char recvProjectStr[PROJECT_STRING_LEN];
//	
//	//从服务器接收反馈
//	memset(szRecvBuffer,0,BOARDCAST_DATA_LEN);
//	nRecvLength = recvfrom(recvSocket,szRecvBuffer,BOARDCAST_DATA_LEN,0,(sockaddr*)&addrPeer,&nSize);		//从网络接收数据
//	if(nRecvLength > 2 + sizeof(int))
//	{//如果接收到数据
//		//|0xcc   |0x00(server),0x01(client)|									|0xdd|
//		//|Head(1)|Type(1)|Port(4)[for server]|StrLen(4)|String(x)|Check(1)|End(1)|
//		if(szRecvBuffer[0] != 0xcc) return XFalse;	//数据不合法直接丢弃
//		if(szRecvBuffer[1] != 0x00) return XFalse;	//只处理从服务器发送的数据
//		memcpy(&tempServerPort,szRecvBuffer + 2,sizeof(int));offset = 2 + sizeof(int);
//		memcpy(&len,szRecvBuffer + offset,sizeof(int));offset += sizeof(int);
//		if(len <= 0 || nRecvLength < offset + 2 + len) return XFalse;
//		//检查数据校验和
//		tempCheck = 0;
//		for(int i = 1;i < offset + len;++ i)
//		{
//			tempCheck += (unsigned char)szRecvBuffer[i];
//		}
//		if(tempCheck != (unsigned char)szRecvBuffer[offset + len]) return XFalse;
//		memcpy(recvProjectStr,szRecvBuffer + offset,len);offset += len;
//		recvProjectStr[len] = '\0';
//		LogNull("recv:%s",recvProjectStr);
//		if(strcpy(recvProjectStr,m_projectStr) != 0) return XFalse;
//		//校验正确之后连接数据
//		m_serverPort = tempServerPort;
//		m_serverAddr = addrPeer;
//		closesocket(recvSocket);
//		return XTrue;
//	}
//	closesocket(recvSocket);
//	return XFalse;
//}
XBool XNetClient::connectToServer()
{
	if(m_connectState == CONNECT_STATE_CONNECT) return XFalse;	//防止重复连接
	//连接服务器
	if(!getANewSocket(m_netSocket)) return XFalse;
	if(!m_boardcastThreadState)
	{//下面建立广播线程
		if(CreateThread(0,0,boardcastThread,this,0,&m_boardcastThread) == 0)
		{//线程建立失败
			LogStr("广播线程建立失败!");
			return XFalse;
		}
	}
	if(!sendBoardcast()) return XFalse;
	//if(!recvBoardcast()) return XFalse;
	Sleep(1000);	//等待1s
	m_mutexBoardCast.Lock();
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(m_serverPort);
	if(connect(m_netSocket,(const sockaddr *)&m_serverAddr,sizeof(sockaddr)) == SOCKET_ERROR)
	{
		LogStr("网络链接失败!");
		closesocket(m_netSocket);
		m_mutexBoardCast.Unlock();
		return XFalse;
	}
	m_mutexBoardCast.Unlock();
	m_connectState = CONNECT_STATE_CONNECT;
	//开启通讯线程
	if(CreateThread(0,0,recvThread,this,0,&m_recvThread) == 0)
	{//线程建立失败
		LogStr("客户端链接线程建立失败!");
		closesocket(m_netSocket);
		return XFalse;
	}
	if(CreateThread(0,0,sendThread,this,0,&m_sendThread) == 0)
	{//线程建立失败
		LogStr("客户端发送线程建立失败!");
		closesocket(m_netSocket);
		return XFalse;
	}

	return XTrue;
}
XBool XNetClient::createClient(XBool useBroadcast,const char *ip,int port)
{
	if(m_connectState == CONNECT_STATE_CONNECT) return XFalse;	//防止重复连接
	if(m_isClientCreated) return XFalse;
	m_useBroadcast = useBroadcast;
	if(ip != NULL) m_serverIP = ip;
	else m_serverIP = "192.168.0.1";
	m_serverPort = port;
	if(CreateThread(0,0,connectThread,this,0,&m_connectThread) == 0)
	{//线程建立失败
		LogStr("客户端连接线程建立失败!");
		closesocket(m_netSocket);
		return XFalse;
	}
	m_isClientCreated = XTrue;
	return XTrue;
}
XBool XNetClient::sendDataSocket(char * data,int len)
{//每次发送4096个数据
	if(data == NULL || len <= 0) return XFalse;
	int ret = 0;
	//方案1：来多少数据都一次发送
	ret = send(m_netSocket,data,len,0);
	if(ret < len)
	{
		LogStr("发送错误!");
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
	//			LogStr("发送错误!");
	//			return XFalse;
	//		}
	//		return XTrue;
	//	}else
	//	{
	//		ret = send(m_netSocket,data + offset,4096,0);
	//		if(ret < 4096)
	//		{
	//			LogStr("发送错误!");
	//			return XFalse;
	//		}
	//		offset += 4096;
	//	}
	//	Sleep(1);
	//}
	//return XTrue;
}
XBool XNetClient::getDataPacket(unsigned char *buff,int len)
{
	if(len <= 0) return XTrue;
	int offset = 0;
	if(m_recvPacketSize == 0)
	{
		if(len >= PACKET_HEAD_LEN)
		{//完整的包头
			m_recvPacket = XMem::createMem<XNetData>();
			if(m_recvPacket == NULL) return XFalse;
			m_recvPacket->type = (XNetDataType)buff[0];
			//if(m_recvPacket->type != DATA_TYPE_VIDEO)
			//{
			//	LogStr("haha");
			//}
			memcpy(&(m_recvPacket->dataLen),buff + 1,sizeof(int));
			//if(m_recvPacket->dataLen > 1000000)
			//{
			//	LogStr("haha");
			//}
			m_recvPacket->data = XMem::createArrayMem<unsigned char>(m_recvPacket->dataLen);
			if(m_recvPacket->data == NULL) return XFalse;
			if(len - PACKET_HEAD_LEN >= m_recvPacket->dataLen)
			{//数据完整
				m_recvPacket->isEnable = XTrue;
				memcpy(m_recvPacket->data,buff + PACKET_HEAD_LEN,m_recvPacket->dataLen);
				offset = PACKET_HEAD_LEN + m_recvPacket->dataLen;
				m_mutexRecv.Lock();
				m_recvDataBuff.push_back(m_recvPacket);
				if(m_recvDataBuff.size() > MAX_RECV_DATA_BUFF)
				{
					XNetData *tmp = m_recvDataBuff[0];
					m_recvDataBuff.pop_front();
					LogStr("XNetServer接收队列数据发生拥堵,丢弃较老的数据!");
					XMem::XDELETE_ARRAY(tmp->data);
					XMem::XDELETE(tmp);
				}
				m_mutexRecv.Unlock();
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
				m_mutexRecv.Lock();
				m_recvDataBuff.push_back(m_recvPacket);
				m_recvPacketSize = 0;
				m_mutexRecv.Unlock();
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
				m_recvPacket = XMem::createMem<XNetData>();
				if(m_recvPacket == NULL) return XFalse;
				m_recvPacket->type = (XNetDataType)m_packetHeadData[0];
				if(m_recvPacket->type != DATA_TYPE_VIDEO)
				{
					LogStr("haha");
				}
				memcpy(&(m_recvPacket->dataLen),m_packetHeadData + 1,sizeof(int));
				if(m_recvPacket->dataLen > 1000000)
				{
					LogStr("haha");
				}
				m_recvPacket->data = XMem::createArrayMem<unsigned char>(m_recvPacket->dataLen);
				if(m_recvPacket->data == NULL) return XFalse;
				//解析余下的数据
				if(len - (PACKET_HEAD_LEN - m_recvPacketSize) >= m_recvPacket->dataLen)
				{//数据完整
					m_recvPacket->isEnable = XTrue;
					memcpy(m_recvPacket->data,buff + (PACKET_HEAD_LEN - m_recvPacketSize),m_recvPacket->dataLen);
					offset = PACKET_HEAD_LEN - m_recvPacketSize + m_recvPacket->dataLen;
					m_mutexRecv.Lock();
					m_recvDataBuff.push_back(m_recvPacket);
					m_recvPacketSize = 0;
					m_mutexRecv.Unlock();
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
DWORD WINAPI XNetClient::recvThread(void * pParam)
{
	XNetClient &pPar = *(XNetClient *)pParam;
	pPar.m_mutexThread.Lock();
	++ pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	int buffLen = 4096;
	char recvBuff[4096];
	int ret = 0;
	pPar.m_recvPacketSize = 0;
	fd_set readfds;
	timeval timeout;
	while(true)
	{
		//if(pPar.m_isExit) break;
		if(pPar.m_connectState == CONNECT_STATE_DISCONNECT) 
		{
			pPar.m_mutexRecv.Lock();
			if(pPar.m_recvPacketSize != 0)
			{//如果网络断开，但是最后一个包没有接收完整，则丢弃最后一个包
				XMem::XDELETE(pPar.m_recvPacket);	//丢弃不完整的包
				pPar.m_recvPacketSize = 0;
			}
			pPar.m_mutexRecv.Unlock();
			break;
		}
		//下面接收数据
		//非阻塞访问
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		FD_ZERO(&readfds);
		FD_SET(pPar.m_netSocket,&readfds);
		ret = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
		if(ret > 0 && FD_ISSET(pPar.m_netSocket,&readfds))
		{//可以接受数据
			FD_CLR(pPar.m_netSocket, &readfds);
			ret = recv(pPar.m_netSocket,recvBuff,buffLen,0);
			if(ret == SOCKET_ERROR) pPar.m_connectState = CONNECT_STATE_DISCONNECT;	//连接断开
			if(ret > 0) pPar.getDataPacket((unsigned char *)recvBuff,ret);//接收到数据
		}
		Sleep(1);
	}
	pPar.m_mutexThread.Lock();
	-- pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	return 1;
}
DWORD WINAPI XNetClient::sendThread(void * pParam)
{
	XNetClient &pPar = *(XNetClient *)pParam;
	pPar.m_mutexThread.Lock();
	++ pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	int buffSize = 4096;
	char * sendBuff = XMem::createArrayMem<char>(buffSize);
	if(sendBuff == NULL) return 0;
	while(pPar.m_connectState != CONNECT_STATE_DISCONNECT)
	{
		//if(pPar.m_isExit) break;
		//if(pPar.m_connectState == CONNECT_STATE_DISCONNECT) break;
		pPar.m_mutexSend.Lock();
		if(pPar.m_sendDataBuff.size() > 0)
		{//有数据需要发送
			XNetData *tempData = (* pPar.m_sendDataBuff.begin());
			pPar.m_sendDataBuff.pop_front();
			pPar.m_mutexSend.Unlock();
			if(tempData->isEnable)
			{
				if(tempData->dataLen + PACKET_HEAD_LEN > buffSize)
				{//数据长度不够需要重新分配数据尺寸
					XMem::XDELETE_ARRAY(sendBuff);
					buffSize = tempData->dataLen + PACKET_HEAD_LEN;
					sendBuff = XMem::createArrayMem<char>(buffSize);
					if(sendBuff == NULL) return 0;
				}
				sendBuff[0] = tempData->type;
				memcpy(sendBuff + 1,&tempData->dataLen,sizeof(int));
				memcpy(sendBuff + PACKET_HEAD_LEN,tempData->data,tempData->dataLen);
				//发送数据
				if(!pPar.sendDataSocket(sendBuff,tempData->dataLen + PACKET_HEAD_LEN))
				{//如果发送失败，则这里将数据推回
					pPar.m_mutexSend.Lock();
					pPar.m_sendDataBuff.push_front(tempData);
					pPar.m_mutexSend.Unlock();
				}else
				{
					showNetData(tempData);
					XMem::XDELETE(tempData);	//释放资源
				}
			}else
			{//无效的数据直接丢弃
				showNetData(tempData);
				XMem::XDELETE(tempData);	//释放资源
			}
		}else
		{
			pPar.m_mutexSend.Unlock();
		}
		Sleep(1);
	}
	XMem::XDELETE_ARRAY(sendBuff);
	pPar.m_mutexThread.Lock();
	-- pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	return 1;
}
DWORD WINAPI XNetClient::boardcastThread(void * pParam)
{
	XNetClient &pPar = *(XNetClient *)pParam;
	pPar.m_mutexThread.Lock();
	++ pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	pPar.m_boardcastThreadState = XTrue;
	unsigned char tempCheck;
	int offset = 0;
	int len;
	sockaddr_in addrUDP;
    memset(&addrUDP,0,sizeof(addrUDP)); 
	addrUDP.sin_family = AF_INET;
    addrUDP.sin_port = htons(BOARDCAST_PORT);
    addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
    int recvSocket = socket(AF_INET,SOCK_DGRAM,0);
	if(recvSocket < 0) return 0;
	resetSocketAddr(recvSocket);
	if(bind(recvSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return 0;	//接收端口

	sockaddr_in addrPeer;
	int tempServerPort;
#ifdef XEE_OS_WINDOWS
	int nSize = sizeof(addrPeer);
#endif
#ifdef XEE_OS_LINUX 
	socklen_t nSize = sizeof(addrPeer);
#endif
	int nRecvLength = 0;
	unsigned char szRecvBuffer[BOARDCAST_DATA_LEN];
	//memset(szRecvBuffer,0,BOARDCAST_DATA_LEN);
	char recvProjectStr[PROJECT_STRING_LEN];
	while(!pPar.m_isExit)
	{
		//if(pPar.m_isExit) break;
		//从服务器接收反馈
		//memset(szRecvBuffer,0,BOARDCAST_DATA_LEN);
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
			memset(szRecvBuffer,0,BOARDCAST_DATA_LEN);
			nRecvLength = recvfrom(recvSocket,(char *)szRecvBuffer,BOARDCAST_DATA_LEN,0,(sockaddr*)&addrPeer,&nSize);		//从网络接收数据
			if(nRecvLength > 2 + sizeof(int))
			{//如果接收到数据
				//|0xcc   |0x00(server),0x01(client)|									|0xdd|
				//|Head(1)|Type(1)|Port(4)[for server]|StrLen(4)|String(x)|Check(1)|End(1)|
				if(szRecvBuffer[0] != 0xcc) continue;	//数据不合法直接丢弃
				if(szRecvBuffer[1] != 0x00) continue;	//只处理从服务器发送的数据
				memcpy(&tempServerPort,szRecvBuffer + 2,sizeof(int));offset = 2 + sizeof(int);
				memcpy(&len,szRecvBuffer + offset,sizeof(int));offset += sizeof(int);
				if(len <= 0 || nRecvLength < offset + 2 + len) continue;
				//检查数据校验和
				tempCheck = 0;
				for(int i = 1;i < offset + len;++ i)
				{
					tempCheck += szRecvBuffer[i];
				}
				if(tempCheck != szRecvBuffer[offset + len]) continue;
				memcpy(recvProjectStr,szRecvBuffer + offset,len);offset += len;
				recvProjectStr[len] = '\0';
				LogNull("recv:%s",recvProjectStr);
				if(strcmp(recvProjectStr,pPar.m_projectStr) != 0) continue;
				//校验正确之后连接数据
				pPar.m_mutexBoardCast.Lock();
				pPar.m_serverPort = tempServerPort;
				pPar.m_serverAddr = addrPeer;
				pPar.m_mutexBoardCast.Unlock();
			}
		}
		Sleep(1);
	}
	closesocket(recvSocket);
	pPar.m_boardcastThreadState = XFalse;
	pPar.m_mutexThread.Lock();
	-- pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	return 0;
}
void XNetClient::release()
{
	m_isExit = XTrue;
	if(m_connectState == CONNECT_STATE_CONNECT) m_connectState = CONNECT_STATE_DISCONNECT;
	while(m_threadSum != 0)
	{//等待所有线程结束
		//if(m_threadSum == 0) break;
		Sleep(1);
	}
	std::deque<XNetData *>::iterator i;
	m_mutexRecv.Lock();
	for(i = m_recvDataBuff.begin();i != m_recvDataBuff.end();++ i)
	{
		XMem::XDELETE(*i);
	}
	m_recvDataBuff.clear();
	m_mutexRecv.Unlock();

	m_mutexSend.Lock();
	for(i = m_sendDataBuff.begin();i != m_sendDataBuff.end();++ i)
	{
		XMem::XDELETE(*i);
	}
	m_sendDataBuff.clear();
	m_mutexSend.Unlock();
	closesocket(m_netSocket);
	m_netSocket = INVALID_SOCKET;
	m_isClientCreated = XFalse;
}
DWORD WINAPI XNetClient::connectThread(void * pParam)
{
	XNetClient &pPar = *(XNetClient *)pParam;
	pPar.m_mutexThread.Lock();
	++ pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	XBool upState = pPar.getIsConnect();
	while(!pPar.m_isExit)
	{
		//if(pPar.m_isExit) break;
		if(!pPar.getIsConnect())
		{//如果连接失败，这里开始连接服务器
			Sleep(1000);	//每秒连接一次，防止连接频率过快
			if(upState)
			{
				if(pPar.m_connectBrokenFun != NULL) (*pPar.m_connectBrokenFun)();
				upState = XFalse;
			}
			if(pPar.m_useBroadcast)
			{
				if(pPar.connectToServer())
				{
					if(pPar.m_connectFun != NULL) (*pPar.m_connectFun)();
					upState = XTrue;
				}
			}else
			{
				if(pPar.connectToServer(pPar.m_serverIP.c_str(),pPar.m_serverPort))
				{
					if(pPar.m_connectFun != NULL) (*pPar.m_connectFun)();
					upState = XTrue;
				}
			}
		}
		Sleep(1);
	}
	pPar.m_mutexThread.Lock();
	-- pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	return 1;
}
}