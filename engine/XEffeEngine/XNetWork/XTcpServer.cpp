#include "XStdHead.h"
#include "XTcpServer.h"
namespace XE {
bool XTcpServer::setup(const std::string& ip, int port, bool isAutoPack)
{
	if (m_isInited) return false;
	m_isAutoPack = isAutoPack;
	if (!getANewSocket(m_localSock)) return false;
	if (m_isAutoPack)
	{
		m_sendBuff.resize(m_recvBuffSize);
	}

	int opt = SO_REUSEADDR;
	setsockopt(m_localSock, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
	sockaddr_in addrRemote;
	memset(&addrRemote, 0, sizeof(addrRemote));
	addrRemote.sin_family = AF_INET;
	addrRemote.sin_port = htons(port);
	//addrRemote.sin_addr.s_addr = htonl(INADDR_ANY);
	addrRemote.sin_addr.s_addr = inet_addr(ip.c_str());

	if (bind(m_localSock, (sockaddr*)&addrRemote, sizeof(addrRemote)) < 0) return false;	//绑定
	if (listen(m_localSock, SOMAXCONN) < 0) return false;	//监听
	//发送线程													//下面需要建立监听线程，接受线程，发送线程
	m_sendThreadState = STATE_BEFORE_START;
	if (CreateThread(0, 0, sendThread, this, 0, NULL) == 0) return false;	//连接线程建立失败
	//开启连接线程
	m_acceptThreadState = STATE_BEFORE_START;
	if (CreateThread(0, 0, acceptThread, this, 0, NULL) == 0)
	{
		waitThreadEnd(m_sendThreadState);
		return false;
	}
	//开启接收线程
	m_recvThreadState = STATE_BEFORE_START;
	if (CreateThread(0, 0, recvThread, this, 0, NULL) == 0)
	{//开启广播线程
		waitThreadEnd(m_acceptThreadState);
		waitThreadEnd(m_sendThreadState);
		return false;	//连接线程建立失败
	}

	m_isInited = true;
	return true;
}
bool XTcpServer::setup(int port, bool isAutoPack)
{
	if (m_isInited) return false;
	m_isAutoPack = isAutoPack;
	if (!getANewSocket(m_localSock)) return false;
	if (m_isAutoPack)
	{
		m_sendBuff.resize(m_recvBuffSize);
	}

	int opt = SO_REUSEADDR;
	setsockopt(m_localSock, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
	sockaddr_in addrRemote;
	memset(&addrRemote, 0, sizeof(addrRemote));
	addrRemote.sin_family = AF_INET;
	addrRemote.sin_port = htons(port);
	addrRemote.sin_addr.s_addr = htonl(INADDR_ANY);
	//addrRemote.sin_addr.s_addr = inet_addr(ip.c_str());

	if (bind(m_localSock, (sockaddr*)&addrRemote, sizeof(addrRemote)) < 0) return false;	//绑定
	if (listen(m_localSock, SOMAXCONN) < 0) return false;	//监听
															//发送线程													//下面需要建立监听线程，接受线程，发送线程
	m_sendThreadState = STATE_BEFORE_START;
	if (CreateThread(0, 0, sendThread, this, 0, NULL) == 0) return false;	//连接线程建立失败
	m_recvThreadState = STATE_BEFORE_START;
	if (CreateThread(0, 0, recvThread, this, 0, NULL) == 0)
	{
		waitThreadEnd(m_sendThreadState);
		return false;	//连接线程建立失败
	}																			//开启连接线程
	m_acceptThreadState = STATE_BEFORE_START;
	if (CreateThread(0, 0, acceptThread, this, 0, NULL) == 0)
	{
		waitThreadEnd(m_sendThreadState);
		waitThreadEnd(m_recvThreadState);
		return false;
	}

	m_isInited = true;
	return true;
}
void XTcpServer::close()
{
	if (!m_isInited) return;
	waitThreadEnd(m_recvThreadState);
	waitThreadEnd(m_sendThreadState);
	closesocket(m_localSock);
	m_localSock = INVALID_SOCKET;
	WSACleanup(); //++
	waitThreadEnd(m_acceptThreadState);
	//这里需要释放所有的内存空间
	m_sMutex.Lock();
	for (auto it = m_sFreePacks.begin(); it != m_sFreePacks.end(); ++it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_sFreePacks.clear();
	for (auto it = m_sCurPacks.begin(); it != m_sCurPacks.end(); ++it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_sCurPacks.clear();
	m_sMutex.Unlock();
	//这里需要释放所有的内存空间
	m_rMutex.Lock();
	for (auto it = m_rFreePacks.begin(); it != m_rFreePacks.end(); ++it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_rFreePacks.clear();
	for (auto it = m_rCurPacks.begin(); it != m_rCurPacks.end(); ++it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_rCurPacks.clear();
	m_rMutex.Unlock();

	m_isInited = false;
}
DWORD WINAPI XTcpServer::recvThread(void * pParam)		//接收线程 
{
	XTcpServer &ref = *(XTcpServer*)pParam;

	XBuffer recvBuff;
	recvBuff.fitSize(m_recvBuffSize);
	memset(recvBuff.getBuffer(), 0, m_recvBuffSize);
	int ret = 0;
	fd_set readfds;
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	ref.m_recvThreadState = STATE_START;
	while (ref.m_recvThreadState != STATE_SET_TO_END)
	{
		for (int i = 0; i < ref.m_clients.size(); ++i)
		{
			if (!ref.m_clients[i].isConnectOK) continue;
			FD_ZERO(&readfds);
			FD_SET(ref.m_clients[i].sock, &readfds);
			if (select(FD_SETSIZE, &readfds, NULL, NULL, &timeout) > 0 &&
				FD_ISSET(ref.m_clients[i].sock, &readfds))
			{//可以接受数据
				FD_CLR(ref.m_clients[i].sock, &readfds);
				ret = recv(ref.m_clients[i].sock, (char*)recvBuff.getBuffer(), recvBuff.getBuffSize(), 0);
				if (ret == SOCKET_ERROR)
				{//接收缓存不够大
					if (!ref.m_isAutoPack)
						recvBuff.fitSize(recvBuff.getBuffSize() * 2);
					continue;
				}
				if (ret == 0)
				{
					ref.m_clients[i].isConnectOK = false;
					LogStr("网络连接断开!");
					continue;
				}
				if (ret > 0)
				{//下面是接收到数据的处理
					if(ref.m_isAutoPack)
						ref._recvAData(recvBuff.getBuffer(), ret, i);
					else
						ref.recvAData(recvBuff.getBuffer(), ret, i);
				}
			}
		}
		Sleep(1);
	}
	ref.m_recvThreadState = STATE_END;
	LogStr("m_recvThreadState over");
	return 1;
}
DWORD WINAPI XTcpServer::sendThread(void * pParam)		//发送线程 
{
	XTcpServer &ref = *(XTcpServer*)pParam;
	ref.m_sendThreadState = STATE_START;
	while (ref.m_sendThreadState != STATE_SET_TO_END)
	{
		//检查队列中是否有数据，如果有则发送
		ref.sendAPack();	//发送一个数据
		Sleep(1);
	}
	ref.m_sendThreadState = STATE_END;
	LogStr("m_sendThreadState over");
	return 1;
}
DWORD WINAPI XTcpServer::acceptThread(void * pParam)	//监听线程 
{
	XTcpServer &ref = *(XTcpServer*)pParam;
	ref.m_acceptThreadState = STATE_START;
	SOCKET clientSocket = INVALID_SOCKET;
	while (ref.m_acceptThreadState != STATE_SET_TO_END)
	{
		//if(par.m_acceptThreadState == STATE_SET_TO_END) break;
		clientSocket = accept(ref.m_localSock, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{//接收客户端失败
			LogStr("客户端加入失败!");
		}
		else
		{//接收客户端成功之后开启一个接收线程
			ref.addAClient(clientSocket);
			LogStr("客户端加入成功!");
		}
		Sleep(1);
	}
	ref.m_acceptThreadState = STATE_END;
	LogStr("m_acceptThreadState over");
	return 1;
}
bool XTcpServer::sendData(const void* data, int len, int cID)
{
	if (data == NULL || len <= 0) return false;
	XNetPack tmp;
	m_sMutex.Lock();
	if (m_sFreePacks.size() > 0)
	{
		tmp = m_sFreePacks[0];
		m_sFreePacks.pop_front();
	}
	m_sMutex.Unlock();
	if (tmp.buffSize < len)
	{
		XMem::XDELETE_ARRAY(tmp.data);
		tmp.data = XMem::createArrayMem<unsigned char>(len);
		if (tmp.data == NULL)
		{
			LogNull("内存分配失败!");
			return false;
		}
		tmp.buffSize = len;
	}
	memcpy(tmp.data, data, len);
	tmp.curDataLen = len;
	tmp.clientID = cID;
	m_sMutex.Lock();
	m_sCurPacks.push_back(tmp);
	m_sMutex.Unlock();
	return true;
}
bool XTcpServer::recoverAData(XNetPack &pack)	//回收一个数据包
{
	m_rMutex.Lock();
	//这里需要检查数据是否已经存在
	for (auto it = m_rFreePacks.begin(); it != m_rFreePacks.end(); ++it)
	{
		if (it->data != pack.data) continue;
		m_rMutex.Unlock();
		return false;
	}
	m_rFreePacks.push_back(pack);
	m_rMutex.Unlock();
	return true;
}
void XTcpServer::popARecvData(XNetPack &data)	//获取一个接收数据包
{
	if (m_rCurPacks.size() <= 0) return;
	m_rMutex.Lock();
	data = m_rCurPacks[0];
	m_rCurPacks.pop_front();
	m_rMutex.Unlock();
}
void XTcpServer::sendAPack()
{
	if (m_sCurPacks.size() <= 0) return;
	//发送数据
	m_sMutex.Lock();
	XNetPack tmp = m_sCurPacks[0];
	m_sCurPacks.pop_front();
	m_sMutex.Unlock();
	//下面发送数据
	int index = getIndexByID(tmp.clientID);
	if (index >= 0)
	{
		if (m_isAutoPack)
		{
			//使用2048的大小进行封包
			memcpy(&m_sendBuff[0], &m_sPackID, sizeof(unsigned int));
			memcpy(&m_sendBuff[4], &tmp.curDataLen, sizeof(int));
			_sendData(tmp.data, tmp.curDataLen, m_clients[index].sock);
			++m_sPackID;
		}
		else
		{
			if (index >= 0)
				send(m_clients[index].sock, (char *)tmp.data, tmp.curDataLen, 0);
		}
	}
	//回收数据包
	m_sMutex.Lock();
	m_sFreePacks.push_back(tmp);
	m_sMutex.Unlock();
}
void XTcpServer::_sendData(unsigned char* data, int len, const SOCKET& sock)
{
	if (len <= m_recvBuffSize - m_packHeadSize)
	{
		memcpy(&m_sendBuff[m_packHeadSize], data, len);
		send(sock, (char *)(&m_sendBuff[0]), len + m_packHeadSize, 0);
		Sleep(1);//防止过快的发送数据而造成卡死
	}
	else
	{
		int size = m_recvBuffSize - m_packHeadSize;
		memcpy(&m_sendBuff[m_packHeadSize], data, size);
		send(sock, (char *)(&m_sendBuff[0]), m_recvBuffSize, 0);
		_sendData(data + size, len - size, sock);
	}
}
void XTcpServer::recvAData(unsigned char* data, int len, int index)
{
	if (data == NULL || len <= 0) return;
	XNetPack tmp;
	m_rMutex.Lock();
	if (m_rFreePacks.size() > 0)
	{
		tmp = m_rFreePacks[0];
		m_rFreePacks.pop_front();
	}
	m_rMutex.Unlock();
	if (tmp.buffSize < len)
	{
		XMem::XDELETE_ARRAY(tmp.data);
		tmp.data = XMem::createArrayMem<unsigned char>(len);
		if (tmp.data == NULL)
		{
			LogNull("内存分配失败!");
			return;
		}
		tmp.buffSize = len;
	}
	memcpy(tmp.data, data, len);
	tmp.curDataLen = len;
	tmp.clientID = m_clients[index].id;
	m_rMutex.Lock();
	m_rCurPacks.push_back(tmp);
	m_rMutex.Unlock();
}
void XTcpServer::_recvAData(unsigned char* data, int len, int index)
{
	if (len <= m_packHeadSize) return;	//非法的数据
	int packID = *(int *)(data);
	int size = *(int *)(data + 4);
	if (size == len - m_packHeadSize)
	{//本身数据包就是完整的，不需要进行下面的一系列操作
		recvAData(data + m_packHeadSize, len - m_packHeadSize, index);
		return;
	}
	XClientInfo& ref = m_clients[index];
	if (ref.recvBuff.size() > 0 && ref.rPackID != packID)
	{//发生了丢包事件，抛弃不完整的数据
		LogStr("发生丢包");
		ref.rPackID = packID;
		ref.recvBuff.resize(len - m_packHeadSize);
		memcpy(&ref.recvBuff[0], data + m_packHeadSize, len - m_packHeadSize);
	}
	else
	{//追加数据
		ref.rPackID = packID;
		int tmpSize = ref.recvBuff.size();
		ref.recvBuff.resize(tmpSize + len - m_packHeadSize);
		memcpy(&ref.recvBuff[tmpSize], data + m_packHeadSize, len - m_packHeadSize);
	}
	if (ref.recvBuff.size() == size)
	{//数据已经接受完整
		recvAData(&ref.recvBuff[0], size, index);
		ref.recvBuff.clear();
	}
}
void XTcpServer::addAClient(SOCKET sock)
{
	int index = getIndexBySock(sock);
	if (index >= 0)
	{
		if (!m_clients[index].isConnectOK)
			m_clients[index].isConnectOK = true;
		return;
	}
	XClientInfo tmp;
	tmp.sock = sock;
	tmp.id = m_curClientID;
	tmp.isConnectOK = true;
	++m_curClientID;
	m_clients.push_back(tmp);
	if (m_isAutoPack)
	{
		m_clients[m_clients.size() - 1].rPackID = -1;
		m_clients[m_clients.size() - 1].recvBuff.resize(m_recvBuffSize);
	}
}
}