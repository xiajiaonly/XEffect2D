#include "XStdHead.h"
#include "XSocketEx.h"
#include "XTimer.h"
namespace XE{
DWORD WINAPI XSocketEx::connectThread(void * pParam)
{
	XSocketEx &par = *(XSocketEx *)pParam;
	par.m_connectThreadState = STATE_START;
	XTimer tempTimer;
	int oldTime = tempTimer.getCurrentTime();
	int curTime;
	float stepTime = 0;
	while(par.m_connectThreadState != STATE_SET_TO_END)
	{
		//if(par.m_connectThreadState == STATE_SET_TO_END) break;
		//下面是保持连接状态的代码
		switch(par.m_conState)
		{//尚未完成
		case CON_STATE_NULL:
			par.connectToNet();//尚未进行连接，则直接进行连接
			break;
		case CON_STATE_NORMAL:	//连接正常则什么也不做
			curTime = tempTimer.getCurrentTime();
			stepTime = curTime - oldTime;
			oldTime = curTime;
			par.m_timer += stepTime;
			if(par.m_timer > 2000)
			{//这里进行心跳处理
				par.m_timer -= 2000;
				if(par.m_socketRole == NET_CLIENT)
				{//向服务器发送心跳
					par.sendHeartbeat();
				}
			}
			if(par.m_socketRole == NET_SERVER)
			{//对客户端进行心跳计时
				par.heartbeatProc(stepTime);
			}
			break;
		case CON_STATE_UNSTABLE://短暂的失去连接则进行连接
		case CON_STATE_ERROR:
			par.connectToNet();//如果长时间失去连接则进行处理
			break;
		}
		//下面是心跳计时
		Sleep(1);
	}
	par.m_connectThreadState = STATE_END;
	return 1;
}
DWORD WINAPI XSocketEx::recvThreadX(void * pParam)
{
	XSocketEx &par = *(XSocketEx *)pParam;
	int clinetID = par.m_curAddClientID;
	par.m_curAddClientID = -1;
	par.m_usersData[clinetID]->recvThreadState = STATE_START;
	int buffLen = 4096;
	char recvBuff[4096];
	int ret = 0;
	fd_set readfds;
	timeval timeout;
	int testError;
	while(par.m_usersData[clinetID]->recvThreadState != STATE_SET_TO_END)
	{
		//if(par.m_usersData[clinetID]->recvThreadState == STATE_SET_TO_END) break;
		if(par.m_usersData[clinetID]->state == CLINET_STATE_CONNECT
			&& par.m_usersData[clinetID]->isEnable)
		{//网络连接正常才接收数据
			timeout.tv_sec = 0;
			timeout.tv_usec = 0;
			FD_ZERO(&readfds);
			FD_SET(par.m_usersData[clinetID]->userSocket,&readfds);
			testError = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
			if(testError > 0 && FD_ISSET(par.m_usersData[clinetID]->userSocket,&readfds))
			{//可以接受数据
				FD_CLR(par.m_usersData[clinetID]->userSocket,&readfds);
				ret = recv(par.m_usersData[clinetID]->userSocket,recvBuff,buffLen,0);
				if(ret == SOCKET_ERROR) par.setOffline(par.m_usersData[clinetID]->userID);	//数据接收失败,则表示网络连接失败
				if(ret > 0)
				{
					par.getDataPacket(par.m_usersData[clinetID]->recvPacket,(unsigned char *)recvBuff,ret);	//接收到数据,下面对数据进行处理
					par.m_usersData[clinetID]->heartbeatTimer = 0;	//收到数据也清除心跳记录
				}
			}
		}
		Sleep(1);
	}
	par.m_usersData[clinetID]->recvThreadState = STATE_END;
	return 1;
}
DWORD WINAPI XSocketEx::boardcastThread(void * pParam)
{//广播线程，服务端负责接收广播并返回数据，客户端负责接收并记录服务器的地址
	XSocketEx &par = *(XSocketEx *)pParam;
	par.m_boardcastThreadState = STATE_START;

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

	int sendSocket;
	unsigned char needSendData[BOARDCAST_DATA_LEN];	//如果是服务器的话，这是需要发送的数据
	if(par.m_socketRole == NET_SERVER)
	{
		int bBroadcast = 1;
		memset(&addrUDP,0,sizeof(addrUDP));
		addrUDP.sin_family  = AF_INET;
		addrUDP.sin_port  = htons(0);
		addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
		sendSocket = socket(AF_INET,SOCK_DGRAM,0);
		if(sendSocket < 0) return 0;
		resetSocketAddr(sendSocket); 
		if(setsockopt(sendSocket,SOL_SOCKET,SO_BROADCAST,(char *)&bBroadcast,sizeof(bBroadcast)) == SOCKET_ERROR) return 0;
		if(bind(sendSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return 0;	//发送端口

		memset(&addrUDP,0,sizeof(addrUDP)); 
		addrUDP.sin_family = AF_INET;
		addrUDP.sin_port = htons(BOARDCAST_PORT);
#ifdef WITH_LOCAL_BOARDCAST_IP
		addrUDP.sin_addr.s_addr = inet_addr(BOARDCASR_IP); 
#else
		addrUDP.sin_addr.s_addr = htonl(INADDR_BROADCAST);      
#endif
		//下面准备需要发送的数据
		needSendData[0] = 0xcc;needSendData[1] = 0x00;offset = 2;
		memcpy(needSendData + offset,&par.m_serverPort,sizeof(int));offset += sizeof(int);
		len = (int)(par.m_serverName.size()) + 1;
		memcpy(needSendData + offset,&len,sizeof(int));offset += sizeof(int);
		memcpy(needSendData + offset,par.m_serverName.c_str(),len);offset += len;
		tempCheck = 0;
		for(int i = 1;i < offset;++ i)
		{
			tempCheck += needSendData[i];
		}
		needSendData[offset] = tempCheck;offset += 1;
		needSendData[offset] = 0xdd;offset += 1;
	}
	sockaddr_in addrPeer;
	int tempServerPort;
	int nRecvLength = 0;
#ifdef XEE_OS_WINDOWS
	int nSize = sizeof(addrPeer);
#endif
	unsigned char szRecvBuffer[BOARDCAST_DATA_LEN];
	std::string serverName;
	while(par.m_boardcastThreadState != STATE_SET_TO_END)
	{
		//if(par.m_boardcastThreadState == STATE_SET_TO_END) break;
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
				if(par.m_socketRole == NET_SERVER)
				{//服务器，接收客户端的数据并返回响应
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
					serverName = (char *)(szRecvBuffer + 2 + sizeof(int));
				//	printf("recv:%s\n",serverName.c_str());
					if(serverName != par.m_serverName) continue;
					//下面回应客户端的请求
					sendto(sendSocket,(char *)needSendData,offset,0,(sockaddr *)&addrUDP,sizeof(addrUDP));
				}else
				if(par.m_socketRole == NET_CLIENT
					|| par.m_socketRole == NET_NULL)
				{//客户端，接收服务器端的数据并建立连接
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
					serverName = (char *)(szRecvBuffer + offset);
				//	printf("recv:%s\n",serverName.c_str());
					if(serverName != par.m_serverName) continue;
					//校验正确之后连接数据
					par.m_serverPort = tempServerPort;
					par.m_serverAddr = addrPeer;
				}
			}
		}
		Sleep(1);
	}
	par.m_boardcastThreadState = STATE_END;
	return 1;
}
DWORD WINAPI XSocketEx::recvThread(void * pParam)
{
	XSocketEx &par = *(XSocketEx *)pParam;
	par.m_recvThreadState = STATE_START;
	int buffLen = 4096;
	char recvBuff[4096];
	int ret = 0;
	fd_set readfds;
	timeval timeout;
	int testError;
	while(par.m_recvThreadState != STATE_SET_TO_END)
	{
		//if(par.m_recvThreadState == STATE_SET_TO_END) break;
		//从网络接收数据，并对数据进行处理，如果作为服务器收到需要转发的数据，则直接转发
		//非阻塞访问
		if(par.m_conState == CON_STATE_NORMAL)
		{//网络连接正常才接收数据
			if(par.m_socketRole == NET_CLIENT)
			{//服务器是为每个客户端开启一个接收线程
				timeout.tv_sec = 0;
				timeout.tv_usec = 0;
				FD_ZERO(&readfds);
				FD_SET(par.m_netSocket,&readfds);
				testError = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
				if(testError > 0 && FD_ISSET(par.m_netSocket,&readfds))
				{//可以接受数据
					FD_CLR(par.m_netSocket,&readfds);
					ret = recv(par.m_netSocket,recvBuff,buffLen,0);
					if(ret == SOCKET_ERROR) par.m_conState = CON_STATE_UNSTABLE;	//数据接收失败,则表示网络连接失败
					if(ret > 0)
					{
						par.getDataPacket(par.m_recvPacket,(unsigned char *)recvBuff,ret);	//接收到数据,下面对数据进行处理
					//	par.m_timer = 0;	//客户端收到数据则可以不用发送心跳
					}
				}
			}
		}
		Sleep(1);
	}
	par.m_recvThreadState = STATE_END;
	return 1;
}
DWORD WINAPI XSocketEx::sendThread(void * pParam)
{
	XSocketEx &par = *(XSocketEx *)pParam;
	par.m_sendThreadState = STATE_START;
	int ret,len;
	int tempDataSize = 2048;
	unsigned char * tempData = XMem::createArrayMem<unsigned char>(tempDataSize);
	while(par.m_sendThreadState != STATE_SET_TO_END)
	{
		//if(par.m_sendThreadState == STATE_SET_TO_END) break;
		if(par.m_sendDeque.size() <= 0) 
		{
			Sleep(1);
			continue;
		}//有数据需要发送，则这里发送数据
		par.m_sendMutex.Lock();
		XSocketDataPack *data = par.m_sendDeque[0];
		par.m_sendMutex.Unlock();
		if(par.m_socketRole == NET_CLIENT)
		{//直接将数据发送到服务器
			len = SOCKET_PACKET_HEAD_SIZE + data->dataLen;
			if(len > tempDataSize)
			{
				tempDataSize = (tempDataSize << 1);
				XMem::XDELETE_ARRAY(tempData);
				tempData = XMem::createArrayMem<unsigned char>(tempDataSize);
			}
			memcpy(tempData,(unsigned char *)data,SOCKET_PACKET_HEAD_SIZE);
			memcpy(tempData + SOCKET_PACKET_HEAD_SIZE,data->data,data->dataLen);
			assert(len <= 2048);
			ret = send(par.m_netSocket,(char *)tempData,len,0);
			if(ret == SOCKET_ERROR) par.m_conState = CON_STATE_UNSTABLE;
			if(ret == len)
			{//发送成功,清除临时数据
				par.m_sendMutex.Lock();
				par.m_sendDeque.pop_front();
				par.m_sendMutex.Unlock();
				XMem::XDELETE_ARRAY(data->data);
				XMem::XDELETE(data);
				par.m_timer = 0;	//客户端收到数据则可以不用发送心跳
			}
		}else
		if(par.m_socketRole == NET_SERVER)
		{//需要解析数据的目标再将数据发送出去
			len = 0;
			par.m_userInfoMutex.Lock();
			for(unsigned int i = 0;i < par.m_usersInfo.size();++ i)
			{
				if(par.m_usersInfo[i]->userID == data->toID)
				{//向目标发送数据
					if(par.m_usersInfo[i]->userState == CLIENT_STATE_OFFLINE)
					{//如果已经离线，则直接将数据包向后推
						par.m_sendMutex.Lock();
						par.m_sendDeque.pop_front();
						par.m_sendDeque.push_back(data);
						par.m_sendMutex.Unlock();
						len = 1;
						break;
					}

					len = SOCKET_PACKET_HEAD_SIZE + data->dataLen;
					if(len > tempDataSize)
					{
						tempDataSize = (tempDataSize << 1);
						XMem::XDELETE_ARRAY(tempData);
						tempData = XMem::createArrayMem<unsigned char>(tempDataSize);
					}
					memcpy(tempData,(unsigned char *)data,SOCKET_PACKET_HEAD_SIZE);
					memcpy(tempData + SOCKET_PACKET_HEAD_SIZE,data->data,data->dataLen);
					ret = send(par.m_usersInfo[i]->userSocket,(char *)tempData,len,0);
					if(ret == SOCKET_ERROR)
					{//如果发送失败则将数据退后
						par.m_sendMutex.Lock();
						par.m_sendDeque.pop_front();
						par.m_sendDeque.push_back(data);
						par.m_sendMutex.Unlock();
						par.setOffline(par.m_usersInfo[i]->userID);
					}
					if(ret == len)
					{//发送成功,清除临时数据
						par.m_sendMutex.Lock();
						par.m_sendDeque.pop_front();
						par.m_sendMutex.Unlock();
						XMem::XDELETE_ARRAY(data->data);
						XMem::XDELETE(data);
					}
					break;
				}
			}
			par.m_userInfoMutex.Unlock();
			if(len == 0)
			{//目标不存在则将数据丢弃(这里可能存在问题)
				par.m_sendMutex.Lock();
				par.m_sendDeque.pop_front();
				par.m_sendMutex.Unlock();
				XMem::XDELETE_ARRAY(data->data);
				XMem::XDELETE(data);
			}
		}
		Sleep(1);
	}
	XMem::XDELETE_ARRAY(tempData);
	par.m_sendThreadState = STATE_END;
	return 1;
}
DWORD WINAPI XSocketEx::acceptThread(void * pParam)
{
	XSocketEx &par = *(XSocketEx *)pParam;
	par.m_acceptThreadState = STATE_START;
	SOCKET clientSocket = INVALID_SOCKET;
	while(par.m_acceptThreadState != STATE_SET_TO_END)
	{
        clientSocket = accept(par.m_netSocket,NULL,NULL);
		if(clientSocket == INVALID_SOCKET)
		{//接收客户端失败
			//LogStr("客户端加入失败!");
		}else
		{//接收客户端成功之后开启一个接收线程
			LogStr("客户端加入成功!");
			par.addAClient(clientSocket);
		}
		Sleep(1);
	}
	par.m_acceptThreadState = STATE_END;
	return 1;
}
void XSocketEx::addAClient(int clientSocket)
{
	if(m_curAddClientID != -1)
	{//如果正在由客户端接入，则这里等待客户端加入完成
		while(m_curAddClientID != -1)
		{
			Sleep(1);
		}
	}
	++ m_IDIndex;
	//增加一个客户端信息
	XSocketUserData *data = XMem::createMem<XSocketUserData>();
	if(data == NULL) return;	
	data->userID = m_IDIndex;
	data->userSocket = clientSocket;
	data->state = CLINET_STATE_CONNECT;
	data->isEnable = true;
	data->recvThreadState = STATE_BEFORE_START;
	m_usersData.push_back(data);
	m_curAddClientID = (int)(m_usersData.size()) - 1;
	//为这个客户端开启接收线程
	if(CreateThread(0,0,recvThreadX,this,0,NULL) == 0) return;
	//向其他客户端发送新客户端接入的信息
	//更新用户信息
	char tempCStr[128];
	sprintf_s(tempCStr,128,"%d",m_IDIndex);
	std::string tempStr = DEFAULT_CLIENT_NAME;
	tempStr += tempCStr;
	XSocketUserInfo *newInfo = XMem::createMem<XSocketUserInfo>();
	if(newInfo == NULL) return;
	newInfo->userID = m_IDIndex;
	newInfo->userSocket = clientSocket;
	newInfo->userNameLen = (int)(tempStr.size()) + 1;
	newInfo->userName = XMem::createArrayMem<char>(newInfo->userNameLen);
	strcpy_s(newInfo->userName,newInfo->userNameLen,tempStr.c_str());
	newInfo->userState = CLIENT_STATE_ONLINE;
	m_userInfoMutex.Lock();
	m_usersInfo.push_back(newInfo);
	m_userInfoMutex.Unlock();
	//向服务器发送设置ID的命令
	XSocketDataPack *tempPacket = XMem::createMem<XSocketDataPack>();
	if(tempPacket == NULL) return;
	tempPacket->fromID = m_myID;
	tempPacket->toID = m_IDIndex;
	tempPacket->dataType = SOCKET_DATA_TYPE_SYSTEM;
	tempPacket->dataLen = sizeof(XSocketSysDataType);
	tempPacket->data = XMem::createArrayMem<unsigned char>(tempPacket->dataLen);
	if(tempPacket->data == NULL) return;
	XSocketSysDataType tempType = SYS_DATA_TYPE_SETID;
	memcpy(tempPacket->data,&tempType,sizeof(XSocketSysDataType));
	lockData(tempPacket->data,tempPacket->dataLen);
	sendAData(tempPacket);
	sendClientInfo();
}
void XSocketEx::sendClientInfo()
{
	int buffSize = sizeof(int) * 2;
	m_userInfoMutex.Lock();
	for(unsigned int i = 0;i < m_usersInfo.size();++ i)
	{
		buffSize += SOCKET_INFO_HEAD_SIZE;
		buffSize += m_usersInfo[i]->userNameLen;
	}
	unsigned char * tempData = XMem::createArrayMem<unsigned char>(buffSize);
	if(tempData == NULL)
	{
		m_userInfoMutex.Unlock();
		return;
	}
	int offset = 0;
	//建立数据
	int size = SYS_DATA_TYPE_UPDATEINIFO;
	memcpy(tempData + offset,&size,sizeof(int));
	offset += sizeof(int);
	size = m_usersInfo.size();
	memcpy(tempData + offset,&size,sizeof(int));
	offset += sizeof(int);
	for(unsigned int  i = 0;i < m_usersInfo.size();++ i)
	{
		memcpy(tempData + offset,&m_usersInfo[i]->userID,sizeof(int));
		offset += sizeof(int);
		memcpy(tempData + offset,&m_usersInfo[i]->userSocket,sizeof(int));
		offset += sizeof(int);
		memcpy(tempData + offset,&m_usersInfo[i]->userState,sizeof(int));
		offset += sizeof(int);
		memcpy(tempData + offset,&m_usersInfo[i]->userNameLen,sizeof(int));
		offset += sizeof(int);
		memcpy(tempData + offset,m_usersInfo[i]->userName,m_usersInfo[i]->userNameLen);
		offset += m_usersInfo[i]->userNameLen;
	}
	m_userInfoMutex.Unlock();
	lockData(tempData,buffSize);
	XSocketDataPack *tempPacket = NULL;
	m_userInfoMutex.Lock();
	for(unsigned int i = 0;i < m_usersInfo.size();++ i)
	{//向每个客户端发送数据
		if(m_usersInfo[i]->userID == m_myID) continue;	//不用给自己发送信息
		if(m_usersInfo[i]->userState == CLIENT_STATE_OFFLINE) continue;
		tempPacket = XMem::createMem<XSocketDataPack>();
		if(tempPacket == NULL) return;
		tempPacket->fromID = m_myID;
		tempPacket->toID = m_usersInfo[i]->userID;
		tempPacket->dataType = SOCKET_DATA_TYPE_SYSTEM;
		tempPacket->dataLen = buffSize;
		tempPacket->data = XMem::createArrayMem<unsigned char>(buffSize);
		if(tempPacket->data == NULL) return;
		memcpy(tempPacket->data,tempData,buffSize);
		sendAData(tempPacket);
	}
	m_userInfoMutex.Unlock();
	//释放临时资源
	XMem::XDELETE_ARRAY(tempData);
}
void XSocketEx::systemDataproc(const XSocketDataPack *data)
{
	if(data->dataType != SOCKET_DATA_TYPE_SYSTEM) return;
	XSocketSysDataType type;
	switch(m_socketRole)
	{
	case NET_CLIENT://服务器发送信息//尚未完成,需要从服务器知道自己的ID
		if(data->dataLen < sizeof(XSocketSysDataType)) return;	//Error
		memcpy(&type,data->data,sizeof(XSocketSysDataType));
		switch(type)
		{
		case SYS_DATA_TYPE_SETID:
			m_myID = data->toID;
			break;
		case SYS_DATA_TYPE_UPDATEINIFO:
			int sum = 0;
			memcpy(&sum,data->data + sizeof(int),sizeof(int));
			if(sum <= 0) return;	//Error
			//删除原有的信息
			m_userInfoMutex.Lock();
			for(unsigned int i = 0;i < m_usersInfo.size();++ i)
			{
				XMem::XDELETE_ARRAY(m_usersInfo[i]->userName);
				XMem::XDELETE(m_usersInfo[i]);
			}
			m_usersInfo.clear();
			//建立新的信息
			XSocketUserInfo *tempInfo = NULL;
			int offset = sizeof(int) * 2;
			for(int i = 0;i < sum;++ i)
			{
				tempInfo = XMem::createMem<XSocketUserInfo>();
				if(tempInfo == NULL)
				{
					m_userInfoMutex.Unlock();
					return;	//Error
				}
				memcpy(&tempInfo->userID,data->data + offset,sizeof(int));
				offset += sizeof(int);
				memcpy(&tempInfo->userSocket,data->data + offset,sizeof(int));
				offset += sizeof(int);
				memcpy(&tempInfo->userState,data->data + offset,sizeof(int));
				offset += sizeof(int);
				memcpy(&tempInfo->userNameLen,data->data + offset,sizeof(int));
				offset += sizeof(int);
				tempInfo->userName = XMem::createArrayMem<char>(tempInfo->userNameLen);
				if(tempInfo->userName == NULL)
				{
					m_userInfoMutex.Unlock();
					return;	//Error
				}
				memcpy(tempInfo->userName,data->data + offset,tempInfo->userNameLen);
				offset += tempInfo->userNameLen;

				m_usersInfo.push_back(tempInfo);
			}
			m_userInfoMutex.Unlock();
			break;
		}
		break;
	case NET_SERVER://客户端改名,找到指定的客户端并经修改名字，然后广播
		if(data->dataLen < sizeof(XSocketSysDataType)) return;	//Error
		memcpy(&type,data->data,sizeof(XSocketSysDataType));
		switch(type)
		{
		case SYS_DATA_TYPE_SETNAME:
			m_userInfoMutex.Lock();
			for(unsigned int i = 0;i < m_usersInfo.size();++ i)
			{
				if(m_usersInfo[i]->userID == data->fromID)
				{//修改名字并广播
					if(m_usersInfo[i]->userNameLen != data->dataLen - (int)(sizeof(XSocketSysDataType)))
					{
						m_usersInfo[i]->userNameLen = data->dataLen - (int)(sizeof(XSocketSysDataType));
						XMem::XDELETE_ARRAY(m_usersInfo[i]->userName);
						m_usersInfo[i]->userName = XMem::createArrayMem<char>(m_usersInfo[i]->userNameLen);
						if(m_usersInfo[i]->userName == NULL) return;
						memcpy(m_usersInfo[i]->userName,data->data + (int)(sizeof(XSocketSysDataType)),m_usersInfo[i]->userNameLen);
					}
					sendClientInfo();	//这里可以优化
					break;
				}
			}
			m_userInfoMutex.Unlock();
			break;
		case SYS_DATA_TYPE_HEARTBEAT:
			for(unsigned int i = 0;i < m_usersData.size();++ i)
			{
				if(m_usersData[i]->userID == data->fromID)
				{//收到心跳信号则归零心跳时间
					m_usersData[i]->heartbeatTimer = 0;
				}
			}
			break;
		}
		break;
	}
}
void XSocketEx::lockData(unsigned char *,int)
{//尚未完成,不忙于实现
}
void XSocketEx::unlockData(unsigned char *,int)
{//尚未完成,不忙于实现
}
bool XSocketEx::getDataPacket(XSocketRecvPacket &packet,unsigned char *buff,int len)
{//尚未完成
	if(len <= 0 || buff == NULL) return false;
	if(packet.recvDataLen == 0)
	{//第一次接收
		if(len >= SOCKET_PACKET_HEAD_SIZE)
		{//包头完整
			memcpy(packet.headData,buff,SOCKET_PACKET_HEAD_SIZE);
			packet.recvDataLen = SOCKET_PACKET_HEAD_SIZE;
			packet.dataPacket = XMem::createMem<XSocketDataPack>();
			if(packet.dataPacket == NULL) return false;	//严重错误
			//下面将包头数据解析出来
			memcpy(&(packet.dataPacket->fromID),packet.headData,sizeof(int));
			memcpy(&(packet.dataPacket->toID),packet.headData + sizeof(int),sizeof(int));
			memcpy(&(packet.dataPacket->dataType),packet.headData + sizeof(int) * 2,sizeof(int));
			memcpy(&(packet.dataPacket->dataLen),packet.headData + sizeof(int) * 3,sizeof(int));
			packet.dataPacket->data = XMem::createArrayMem<unsigned char>(packet.dataPacket->dataLen);
			if(packet.dataPacket->data == NULL) return false;	//严重错误
			return getDataPacket(packet,buff + SOCKET_PACKET_HEAD_SIZE,len - SOCKET_PACKET_HEAD_SIZE);
		}else
		{//包头不完整
			memcpy(packet.headData,buff,len);
			packet.recvDataLen = len;
			return true;
		}
	}else
	{//已经接收了部分
		if(packet.recvDataLen >= SOCKET_PACKET_HEAD_SIZE)
		{//包头已经接收完整
			if(len + packet.recvDataLen >= packet.dataPacket->dataLen + SOCKET_PACKET_HEAD_SIZE)
			{//数据包完整
				int needLen = packet.dataPacket->dataLen + SOCKET_PACKET_HEAD_SIZE - packet.recvDataLen;
				memcpy(packet.dataPacket->data + (packet.recvDataLen - SOCKET_PACKET_HEAD_SIZE),buff,needLen);
				//数据包数据完整，对数据包进行处理
				if(m_socketRole == NET_CLIENT)
				{//如果是客户端则直接将数据推入接受队列
					unlockData(packet.dataPacket->data,packet.dataPacket->dataLen);
					if(packet.dataPacket->dataType == SOCKET_DATA_TYPE_SYSTEM)
					{//如果接收到的是系统数据的话，这里处理系统数据
						systemDataproc(packet.dataPacket);
						XMem::XDELETE_ARRAY(packet.dataPacket->data);
						XMem::XDELETE(packet.dataPacket);
					}else
					{
						m_recvMutex.Lock();
						m_recvDeque.push_back(packet.dataPacket);
						m_recvMutex.Unlock();
					}
				}else
				if(m_socketRole == NET_SERVER)
				{//如果是服务器需要判断数据是否是发给自己的
					if(packet.dataPacket->toID != m_myID)
					{
						if(packet.dataPacket->toID > 0) sendAData(packet.dataPacket);	//数据需要转发
						else
						{//需要广播的数据
							//这里有问题，发送的源改变了
							XSocketDataPack *tempPacket = NULL;
							for(unsigned int i = 0;i < m_usersData.size();++ i)
							{
								if(m_usersData[i]->state == CLINET_STATE_CONNECT
									&& m_usersData[i]->userID != packet.dataPacket->fromID)
								{
									tempPacket = XMem::createMem<XSocketDataPack>();
									tempPacket->fromID = packet.dataPacket->fromID;
									tempPacket->toID = m_usersData[i]->userID;
									tempPacket->dataType = packet.dataPacket->dataType;
									tempPacket->dataLen = packet.dataPacket->dataLen;
									tempPacket->data = XMem::createArrayMem<unsigned char>(tempPacket->dataLen);
									memcpy(tempPacket->data,packet.dataPacket->data,packet.dataPacket->dataLen);
									sendAData(tempPacket);
								}
									//sendAData(m_usersData[i]->userID,packet.dataPacket->data,packet.dataPacket->dataLen);
							}
							unlockData(packet.dataPacket->data,packet.dataPacket->dataLen);	//解密
							m_recvMutex.Lock();
							m_recvDeque.push_back(packet.dataPacket);
							m_recvMutex.Unlock();
						}
					}else
					{
						unlockData(packet.dataPacket->data,packet.dataPacket->dataLen);
						if(packet.dataPacket->dataType == SOCKET_DATA_TYPE_SYSTEM)
						{//如果接收到的是系统数据的话，这里处理系统数据
							systemDataproc(packet.dataPacket);
							XMem::XDELETE_ARRAY(packet.dataPacket->data);
							XMem::XDELETE(packet.dataPacket);
						}else
						{
							m_recvMutex.Lock();
							m_recvDeque.push_back(packet.dataPacket);
							m_recvMutex.Unlock();
						}
					}
				}
				//将余下的数据继续处理
				packet.recvDataLen = 0;
				return getDataPacket(packet,buff + needLen,len - needLen);
			}else
			{//数据包不完整
				memcpy(packet.dataPacket->data + (packet.recvDataLen - SOCKET_PACKET_HEAD_SIZE),buff,len);
				packet.recvDataLen += len;
				return true;
			}
		}else
		{//包头尚未接收完整
			if(packet.recvDataLen + len >= SOCKET_PACKET_HEAD_SIZE)
			{//完整的包头
				int needLen = SOCKET_PACKET_HEAD_SIZE - packet.recvDataLen;
				memcpy(packet.headData + packet.recvDataLen,buff,needLen);
				packet.recvDataLen = SOCKET_PACKET_HEAD_SIZE;
				packet.dataPacket = XMem::createMem<XSocketDataPack>();
				if(packet.dataPacket == NULL) return false;	//严重错误
				//下面将包头数据解析出来
				memcpy(&(packet.dataPacket->fromID),packet.headData,sizeof(int));
				memcpy(&(packet.dataPacket->toID),packet.headData + sizeof(int),sizeof(int));
				memcpy(&(packet.dataPacket->dataType),packet.headData + sizeof(int) * 2,sizeof(int));
				memcpy(&(packet.dataPacket->dataLen),packet.headData + sizeof(int) * 3,sizeof(int));
				packet.dataPacket->data = XMem::createArrayMem<unsigned char>(packet.dataPacket->dataLen);
				if(packet.dataPacket->data == NULL) return false;	//严重错误
				return getDataPacket(packet,buff + needLen,len - needLen);
			}else
			{//包头任然不完整
				memcpy(packet.headData + packet.recvDataLen,buff,len);
				packet.recvDataLen += len;
				return true;
			}
		}
	}
//	return true;
}
bool XSocketEx::connectToServer()
{//通过IP连接到服务器
	if(m_netSocket == INVALID_SOCKET && !getANewSocket(m_netSocket)) return false;
	resetSocketAddr(m_netSocket);
	switch(m_workMode)
	{
	case SOCKET_WORK_MODE_IP://通过服务器IP连接到服务器
		sockaddr_in addrRemote;
		memset(&addrRemote,0,sizeof(addrRemote));
		addrRemote.sin_addr.s_addr = inet_addr(m_serverIP.c_str());
		addrRemote.sin_family = AF_INET;
		addrRemote.sin_port = htons(m_serverPort);
		if(connect(m_netSocket,(const sockaddr*)&addrRemote,sizeof(sockaddr)) == SOCKET_ERROR) return false;	//连接网络失败
		break;
	case SOCKET_WORK_MODE_NAME://通过服务器名称连接到服务器
		if(m_conState == CON_STATE_NULL)
		{
			m_boardcastThreadState = STATE_BEFORE_START;
			if(CreateThread(0,0,boardcastThread,this,0,NULL) == 0) return false;	//连接线程建立失败
		}
		sendBoardcast();	//向网络广播
		Sleep(1000);		//等待服务器回应
		m_serverAddr.sin_family = AF_INET;
		m_serverAddr.sin_port = htons(m_serverPort);
		if(connect(m_netSocket,(const sockaddr*)&m_serverAddr,sizeof(sockaddr)) == SOCKET_ERROR)
		{
			waitThreadEnd(m_boardcastThreadState);
			return false;	//连接网络失败
		}
		break;
	}

	//下面需要建立接收线程，发送线程(尚未完成)
	if(m_conState == CON_STATE_NULL)
	{//第一次建立连接，这里需要开启接收线程和发送线程
		m_recvThreadState = STATE_BEFORE_START;
		if(CreateThread(0,0,recvThread,this,0,NULL) == 0) return false;	//连接线程建立失败
		m_sendThreadState = STATE_BEFORE_START;
		if(CreateThread(0,0,sendThread,this,0,NULL) == 0)
		{
			waitThreadEnd(m_recvThreadState);
			return false;	//连接线程建立失败
		}
	}
	requestRename();//客户端连接到服务器之后需要向服务器发送更名请求

	return true;
}
bool XSocketEx::createServer()
{
	if(m_netSocket != INVALID_SOCKET) resetSocketAddr(m_netSocket);
	else
	{
		if (!getANewSocket(m_netSocket)) return false;
	}
	int opt = SO_REUSEADDR;
	setsockopt(m_netSocket,SOL_SOCKET,SO_REUSEADDR,(const char *)&opt,sizeof(opt));
	sockaddr_in addrRemote;
    memset(&addrRemote,0,sizeof(addrRemote));
    addrRemote.sin_family = AF_INET;
    addrRemote.sin_port = htons(m_serverPort);
	switch(m_workMode)
	{
	case SOCKET_WORK_MODE_NAME:addrRemote.sin_addr.s_addr = htonl(INADDR_ANY);break;
	case SOCKET_WORK_MODE_IP:addrRemote.sin_addr.s_addr = inet_addr(m_serverIP.c_str());break;
	}

	if(bind(m_netSocket,(sockaddr*)&addrRemote, sizeof(addrRemote)) < 0) return false;	//绑定
	if(listen(m_netSocket,SOMAXCONN) < 0) return false;	//监听
	//建立服务器时将自己添加到成员列表中
	if(m_usersInfo.size() <= 0)
	{
		XSocketUserInfo *newInfo = XMem::createMem<XSocketUserInfo>();
		if(newInfo == NULL) return false;
		newInfo->userID = m_myID;
		newInfo->userSocket = m_netSocket;
		//newInfo->userNameLen = sizeof(DEFAULT_CLIENT_NAME);
		newInfo->userNameLen = (int)(m_myName.size()) + 1;
		newInfo->userName = XMem::createArrayMem<char>(newInfo->userNameLen);
		strcpy_s(newInfo->userName,newInfo->userNameLen,m_myName.c_str());
		newInfo->userState = CLIENT_STATE_ONLINE;
		m_userInfoMutex.Lock();
		m_usersInfo.push_back(newInfo);
		m_userInfoMutex.Unlock();
	}
	//下面需要建立监听线程，接受线程，发送线程
	m_sendThreadState = STATE_BEFORE_START;
	if(CreateThread(0,0,sendThread,this,0,NULL) == 0) return false;	//连接线程建立失败
	m_acceptThreadState = STATE_BEFORE_START;
	if(CreateThread(0,0,acceptThread,this,0,NULL) == 0)
	{//连接线程建立失败
		waitThreadEnd(m_sendThreadState);
		return false;
	}
	//开启广播线程
	if(m_workMode == SOCKET_WORK_MODE_NAME)
	{
		m_boardcastThreadState = STATE_BEFORE_START;
		if(CreateThread(0,0,boardcastThread,this,0,NULL) == 0)
		{
			waitThreadEnd(m_recvThreadState);
			waitThreadEnd(m_sendThreadState);
			return false;	//连接线程建立失败
		}
	}
	return true;
}
bool XSocketEx::connectToNet()	//连接到网络
{
	switch(m_conState)
	{
	case CON_STATE_NULL:	//第一次建立连接
		if(!getANewSocket(m_netSocket)) return false;
		switch(m_socketRole)
		{
		case NET_SERVER://如果是服务器断开连接之后什么也不做等待连接
			if(!createServer()) return false;
			m_conState = CON_STATE_NORMAL;
			break;
		case NET_NULL:
			//连接网络
			if(!connectToServer())
			{
				if(m_workMode == SOCKET_WORK_MODE_NAME || getMyIP() == m_serverIP || m_serverIP == std::string("127.0.0.1"))
				{//如果本机IP与设置IP一直则尝试建立服务器
					closesocket(m_netSocket);
					m_netSocket = INVALID_SOCKET;
					if(createServer())
					{
						m_conState = CON_STATE_NORMAL;
						m_socketRole = NET_SERVER;
					}
				}
			}else
			{
				m_conState = CON_STATE_NORMAL;
				m_socketRole = NET_CLIENT;
			}
			break;
		case NET_CLIENT:
			if(!connectToServer()) return false;
			m_conState = CON_STATE_NORMAL;
			break;
		}
		break;
	case CON_STATE_NORMAL:
		return true;
		break;
	case CON_STATE_UNSTABLE:
	case CON_STATE_ERROR:
		switch(m_socketRole)
		{
		case NET_SERVER://如果是服务器断开连接之后什么也不做等待连接
			return true;
			break;
		case NET_NULL:
		case NET_CLIENT:
			//如果是客户端断开连接之后主动连接服务器
			if(!getANewSocket(m_netSocket)) return false;
			//连接网络
			if(connectToServer()) m_conState = CON_STATE_NORMAL;
			break;
		}
		break;
	}
	return false;
}
void XSocketEx::release()
{
	if(!m_isInited) return;
	//这里需要结束所有的线程(尚未完成)
	waitThreadEnd(m_connectThreadState);
	waitThreadEnd(m_recvThreadState);
	waitThreadEnd(m_sendThreadState);
	waitThreadEnd(m_boardcastThreadState);
	if(m_socketRole == NET_SERVER)
	{//如果是服务器端的话，需要等待所有客户端接收线程结束
		for(unsigned int i = 0;i < m_usersData.size();++ i)
		{
			waitThreadEnd(m_usersData[i]->recvThreadState);
		}
	}
	closesocket(m_netSocket);
	m_netSocket = INVALID_SOCKET;
	WSACleanup(); //++

	waitThreadEnd(m_acceptThreadState);	//这里有问题
	//下面释放掉所有的资源
	for(auto it = m_recvDeque.begin();it != m_recvDeque.end();++ it)
	{
		XMem::XDELETE_ARRAY((*it)->data);
		XMem::XDELETE((*it));
	}
	m_recvDeque.clear();
	for (auto it = m_sendDeque.begin(); it != m_sendDeque.end(); ++it)
	{
		XMem::XDELETE_ARRAY((*it)->data);
		XMem::XDELETE((*it));
	}
	m_sendDeque.clear();
	for(unsigned int i = 0;i < m_usersInfo.size();++ i)
	{
		XMem::XDELETE_ARRAY(m_usersInfo[i]->userName);
		XMem::XDELETE(m_usersInfo[i]);
	}
	m_usersInfo.clear();
	for(unsigned int i = 0;i < m_usersData.size();++ i)
	{
		XMem::XDELETE(m_usersData[i]);
	}
	m_usersData.clear();
	
	m_isInited = false;
}
bool XSocketEx::setMyName(const std::string& myName)
{//注意这里不考虑重名问题
	if(m_myName == myName) return false;
	if(m_socketRole == NET_NULL)
	{//尚未连接网络，则直接进行修改
		m_myName = myName;
		return true;
	}
	if(m_socketRole == NET_SERVER)
	{
		m_myName = myName;
		m_userInfoMutex.Lock();
		m_usersInfo[0]->userNameLen = (int)(m_myName.size()) + 1;
		XMem::XDELETE_ARRAY(m_usersInfo[0]->userName);
		m_usersInfo[0]->userName = XMem::createArrayMem<char>(m_usersInfo[0]->userNameLen);
		strcpy_s(m_usersInfo[0]->userName,m_usersInfo[0]->userNameLen,m_myName.c_str());
		m_userInfoMutex.Unlock();
		sendClientInfo();
	}
	if(m_socketRole == NET_CLIENT)
	{
		m_myName = myName;
		requestRename();
	}
	return true;
}
bool XSocketEx::sendAData(int toID,const void * data,int len)
{
	if(len <= 0 || data == NULL ||
		toID == m_myID) return false;
	XSocketDataPack *tempPacket = XMem::createMem<XSocketDataPack>();
	if(tempPacket == NULL) return false;
	tempPacket->fromID = m_myID;
	tempPacket->toID = toID;
	tempPacket->dataType = SOCKET_DATA_TYPE_CUSTOM;
	tempPacket->dataLen = len;
	tempPacket->data = XMem::createArrayMem<unsigned char>(len);
	if(tempPacket->data == NULL) return false;
	memcpy(tempPacket->data,data,len);
	lockData(tempPacket->data,len);
	sendAData(tempPacket);
	return true;
}
void XSocketEx::requestRename()	//向服务器发送更名请求
{
	XSocketDataPack *tempPacket = XMem::createMem<XSocketDataPack>();
	if(tempPacket == NULL) return;
	tempPacket->fromID = m_myID;
	tempPacket->toID = 0;
	tempPacket->dataType = SOCKET_DATA_TYPE_SYSTEM;
	tempPacket->dataLen = sizeof(XSocketSysDataType) + (int)(m_myName.size()) + 1;
	tempPacket->data = XMem::createArrayMem<unsigned char>(tempPacket->dataLen);
	if(tempPacket->data == NULL) return;
	XSocketSysDataType temp = SYS_DATA_TYPE_SETNAME;
	memcpy(tempPacket->data,&temp,sizeof(XSocketSysDataType));
	memcpy(tempPacket->data + sizeof(XSocketSysDataType),m_myName.c_str(),(int)(m_myName.size()) + 1);
	lockData(tempPacket->data,tempPacket->dataLen);
	sendAData(tempPacket);
}
void XSocketEx::sendHeartbeat()
{//发送心跳信号
	XSocketDataPack *tempPacket = XMem::createMem<XSocketDataPack>();
	if(tempPacket == NULL) return;
	tempPacket->fromID = m_myID;
	tempPacket->toID = 0;
	tempPacket->dataType = SOCKET_DATA_TYPE_SYSTEM;
	tempPacket->dataLen = sizeof(XSocketSysDataType);
	tempPacket->data = XMem::createArrayMem<unsigned char>(tempPacket->dataLen);
	if(tempPacket->data == NULL) return;
	XSocketSysDataType temp = SYS_DATA_TYPE_HEARTBEAT;
	memcpy(tempPacket->data,&temp,tempPacket->dataLen);
	lockData(tempPacket->data,tempPacket->dataLen);
	sendAData(tempPacket);
}
void XSocketEx::setOffline(int id)
{//设置某个客户端离线
//	m_userDataMutex.Lock();
	for(unsigned int i = 0;i < m_usersData.size();++ i)
	{
		if(m_usersData[i]->userID == id)
		{
			m_usersData[i]->state = CLINET_STATE_DISCONNECT;
			break;
		}
	}
//	m_userDataMutex.Unlock();
	m_userInfoMutex.Lock();
	for(unsigned int i = 0;i < m_usersInfo.size();++ i)
	{
		if(m_usersInfo[i]->userID == id)
		{
			m_usersInfo[i]->userState = CLIENT_STATE_OFFLINE;
			break;
		}
	}
	m_userInfoMutex.Unlock();
	m_clientStateChange = true;
}
void XSocketEx::heartbeatProc(float stepTime)
{//计算客户端的心跳并做相应的处理
//	m_userDataMutex.Lock();
	for(unsigned int i = 0;i < m_usersData.size();++ i)
	{
		if(m_usersData[i]->state == CLINET_STATE_CONNECT)
		{
			m_usersData[i]->heartbeatTimer += stepTime;
			if(m_usersData[i]->heartbeatTimer >= 10000)
			{//连接异常
				setOffline(m_usersData[i]->userID);
			}
		}
	}
//	m_userDataMutex.Unlock();
	if(m_clientStateChange)
	{
		m_clientStateChange = false;
		sendClientInfo();	//向客户端发送状态变更
	}
}
bool XSocketEx::sendBoardcast()
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
	if(sendSocket < 0) return false;
	resetSocketAddr(sendSocket); 
	if(setsockopt(sendSocket,SOL_SOCKET,SO_BROADCAST,(char *)&bBroadcast,sizeof(bBroadcast)) == SOCKET_ERROR) return false;
	if(bind(sendSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return false;	//接收端口

    memset(&addrUDP,0,sizeof(addrUDP)); 
	addrUDP.sin_family = AF_INET;
	addrUDP.sin_port = htons(BOARDCAST_PORT);
#ifdef WITH_LOCAL_BOARDCAST_IP
	addrUDP.sin_addr.s_addr = inet_addr(BOARDCASR_IP); 
#else
	addrUDP.sin_addr.s_addr = htonl(INADDR_BROADCAST);      
#endif

	//下面准备需要发送的数据(尚未完成)
	unsigned char needSendData[BOARDCAST_DATA_LEN];
	needSendData[0] = 0xcc;needSendData[1] = 0x01;offset = 2;
	len = (int)(m_serverName.size()) + 1;
	memcpy(needSendData + offset,&len,sizeof(int));offset += sizeof(int);
	memcpy(needSendData + offset,m_serverName.c_str(),len);offset += len;
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
		return false;	//向网络广播数据
	}

	closesocket(sendSocket);
	return true;
}
#if !WITH_INLINE_FILE
#include "XSocketEx.inl"
#endif
}