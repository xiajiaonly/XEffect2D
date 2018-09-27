#include "XStdHead.h"
#include "XUdpReceiver.h"
namespace XE{
void XUdpReceiver::_pushAPack(unsigned char *p, int len)	//压入一个数据包
{
	if (len <= m_packHeadSize) return;	//非法的数据
	int id = *(int *)p;
	int packID = *(int *)(p + 4);
	int size = *(int *)(p + 8);
	if (size == len - m_packHeadSize)
	{//本身数据包就是完整的，不需要进行下面的一系列操作
		pushAPack(p + m_packHeadSize, len - m_packHeadSize);
		return;
	}
	int index = getIndexByID(id);
	if (index < 0)
	{//需要添加
		m_clientsInfo.push_back(XClientInfo());
		XClientInfo &ref = m_clientsInfo[m_clientsInfo.size() - 1];
		ref.id = id;
		ref.packID = packID;
		ref.buff.resize(len - m_packHeadSize);
		memcpy(&ref.buff[0], p + m_packHeadSize, len - m_packHeadSize);
		if (ref.buff.size() == size)
		{//数据已经接受完整
			pushAPack(&ref.buff[0],size);
			ref.buff.clear();
		}
	}
	else
	{
		XClientInfo &ref = m_clientsInfo[index];
		if (ref.buff.size() > 0 && ref.packID != packID)
		{//发生了丢包事件，抛弃不完整的数据
			LogStr("发生丢包");
			ref.packID = packID;
			ref.buff.resize(len - m_packHeadSize);
			memcpy(&ref.buff[0], p + m_packHeadSize, len - m_packHeadSize);
		}
		else
		{//追加数据
			ref.packID = packID;
			int tmpSize = ref.buff.size();
			ref.buff.resize(tmpSize + len - m_packHeadSize);
			memcpy(&ref.buff[tmpSize], p + m_packHeadSize, len - m_packHeadSize);
		}
		if (ref.buff.size() == size)
		{//数据已经接受完整
			pushAPack(&ref.buff[0],size);
			ref.buff.clear();
		}
	}
}
void XUdpReceiver::pushAPack(unsigned char *p,int len)	//压入一个数据包
{
	if(p == NULL || len <= 0) return;
	XNetPack tmp;
	m_mutex.Lock();
	if(m_freePacks.size() > 0)
	{
		tmp = m_freePacks[0];
		m_freePacks.pop_front();
	}
	m_mutex.Unlock();
	if(tmp.buffSize < len)
	{
		XMem::XDELETE_ARRAY(tmp.data);
		tmp.data = XMem::createArrayMem<unsigned char>(len);
		if(tmp.data == NULL)
		{
			LogNull("内存分配失败!");
			return;
		}
		tmp.buffSize = len;
	}
	memcpy(tmp.data,p,len);
	tmp.curDataLen = len;
	m_mutex.Lock();
	m_curPacks.push_back(tmp);
	m_mutex.Unlock();
}
bool XUdpReceiver::setup(int port, bool autoPack)	//建立
{
	if(m_isInited) return false;
	m_autoPack = autoPack;
	//建立本机sock
	if (!getANewSocket(m_localSock, SOCK_DGRAM)) return false;
	sockaddr_in addrUDP;
	memset(&addrUDP,0,sizeof(addrUDP)); 
	addrUDP.sin_family = AF_INET;
	addrUDP.sin_port = htons(port);
	addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(m_localSock,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return false;	//接收端口
	//开启接收线程
	m_threadState = STATE_BEFORE_START;
	if(CreateThread(0,0,recvThread,this,0,NULL) == 0)
	{//线程建立失败
		LogNull("建立接收线程失败!");
		return false;
	}

	m_isInited = true;
	return true;
}
void XUdpReceiver::close()			//关闭
{
	if(!m_isInited) return;
	waitThreadEnd(m_threadState);
	closesocket(m_localSock);
	//这里需要释放所有的内存空间
	m_mutex.Lock();
	for (auto it = m_freePacks.begin(); it != m_freePacks.end(); ++it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_freePacks.clear();
	for (auto it = m_curPacks.begin(); it != m_curPacks.end(); ++it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_curPacks.clear();
	m_mutex.Unlock();
	m_isInited = false;
}
DWORD WINAPI XUdpReceiver::recvThread(void * pParam)		//接收线程
{
	XUdpReceiver &pPar = *(XUdpReceiver *)pParam;
	int nRecvLength = 0;
	XBuffer recvBuff;
	recvBuff.fitSize(m_recvBuffSize);
	memset(recvBuff.getBuffer(), 0, m_recvBuffSize);
	//获取缓存大小
	//int nRevBuf;
	//int nRevOpt = sizeof(nRevBuf);
	//getsockopt(pPar.m_localSock, SOL_SOCKET, SO_RCVBUF, (CHAR *)&nRevBuf, &nRevOpt);
	sockaddr_in addrPeer;
	int nSize = sizeof(addrPeer);
	//非阻塞访问
	fd_set readfds;
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	pPar.m_threadState = STATE_START;
	while(pPar.m_threadState != STATE_SET_TO_END)
	{
		FD_ZERO(&readfds);
		FD_SET(pPar.m_localSock,&readfds);
		if (select(FD_SETSIZE, &readfds, NULL, NULL, &timeout) > 0 &&
			FD_ISSET(pPar.m_localSock, &readfds))
		{//可以接受数据
			FD_CLR(pPar.m_localSock, &readfds);
			if (pPar.m_autoPack)
			{
				nRecvLength = recvfrom(pPar.m_localSock, (char *)recvBuff.getBuffer(),
					recvBuff.getBuffSize(), 0, (sockaddr*)&addrPeer, &nSize);
				if (nRecvLength > 0)//接收到数据
					pPar._pushAPack(recvBuff.getBuffer(), nRecvLength);
			}
			else
			{
				while (true)
				{
					nRecvLength = recvfrom(pPar.m_localSock, (char *)recvBuff.getBuffer(),
						recvBuff.getBuffSize(), 0, (sockaddr*)&addrPeer, &nSize);
					if (nRecvLength > 0)//接收到数据
					{//成功接受
						pPar.pushAPack(recvBuff.getBuffer(), nRecvLength);
						break;
					}
					if (nRecvLength < 0)
					{//缓存大小不够
						recvBuff.fitSize(recvBuff.getBuffSize() * 2);
						continue;
					}
					LogStr("连接中止");
					break;
				}
			}
		}
		//Sleep(1);
	}
	pPar.m_threadState = STATE_END;
	return 1;
}
bool XUdpReceiver::recoverAData(XNetPack &pack)	//回收一个数据包
{
	m_mutex.Lock();
	//这里需要检查数据是否已经存在
	for (auto it = m_freePacks.begin(); it != m_freePacks.end(); ++it)
	{
		if (it->data != pack.data) continue;
		m_mutex.Unlock();
		return false;
	}
	m_freePacks.push_back(pack);
	m_mutex.Unlock();
	return true;
}
}