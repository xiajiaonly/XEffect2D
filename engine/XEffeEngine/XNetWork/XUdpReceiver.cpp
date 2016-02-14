#include "XStdHead.h"
#include "XUdpReceiver.h"
namespace XE{
void XUdpReceiver::pushAPack(unsigned char *p,int len)	//压入一个数据包
{
	if(p == NULL || len <= 0) return;
	XUdpPack tmp;
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
bool XUdpReceiver::setup(int port)	//建立
{
	if(m_isInited) return false;
	//建立本机sock
	if(WSAStartup(MAKEWORD(2,2),&m_wsaData) != 0) printf("Socket init error\n");
	sockaddr_in addrUDP;
	memset(&addrUDP,0,sizeof(addrUDP)); 
	addrUDP.sin_family = AF_INET;
	addrUDP.sin_port = htons(port);
	addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
	m_localSock = socket(AF_INET,SOCK_DGRAM,0);
	if(m_localSock < 0) return false;
	resetSocketAddr(m_localSock);
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
	for(int i = 0;i < m_freePacks.size();++ i)
	{
		XMem::XDELETE_ARRAY(m_freePacks[i].data);
	}
	m_freePacks.clear();
	for(int i = 0;i < m_curPacks.size();++ i)
	{
		XMem::XDELETE_ARRAY(m_curPacks[i].data);
	}
	m_curPacks.clear();
	m_mutex.Unlock();
	m_isInited = false;
}
DWORD WINAPI XUdpReceiver::recvThread(void * pParam)		//接收线程
{
	XUdpReceiver &pPar = *(XUdpReceiver *)pParam;
	int nRecvLength = 0;
	unsigned char szRecvBuffer[m_recvBuffSize];
	memset(szRecvBuffer,0,m_recvBuffSize);
	sockaddr_in addrPeer;
	int nSize = sizeof(addrPeer);
	pPar.m_threadState = STATE_START;
	while(pPar.m_threadState != STATE_SET_TO_END)
	{
		//非阻塞访问
		fd_set readfds;
		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		FD_ZERO(&readfds);
		FD_SET(pPar.m_localSock,&readfds);
		int test_error = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
		if(test_error > 0 && FD_ISSET(pPar.m_localSock,&readfds))
		{//可以接受数据
			FD_CLR(pPar.m_localSock, &readfds);
			nRecvLength = recvfrom(pPar.m_localSock,(char *)szRecvBuffer,m_recvBuffSize,0,(sockaddr*)&addrPeer,&nSize);
			if(nRecvLength > 0)//接收到数据
				pPar.pushAPack(szRecvBuffer,nRecvLength);
		}
		Sleep(1);
	}
	pPar.m_threadState = STATE_END;
	return 1;
}
bool XUdpReceiver::recoverAData(XUdpPack &pack)	//回收一个数据包
{
	m_mutex.Lock();
	//这里需要检查数据是否已经存在
	for(int i = 0;i < m_freePacks.size();++ i)
	{
		if(m_freePacks[i].data == pack.data)
		{
			m_mutex.Unlock();
			return false;
		}
	}
	m_freePacks.push_back(pack);
	m_mutex.Unlock();
	return true;
}
}