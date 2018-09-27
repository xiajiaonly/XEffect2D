INLINE bool XSocketEx::sendAData(XSocketDataPack * data)
{//注意多线程互斥,这里默认为数据已经经过加密
	if(data == NULL) return false;
	if(data->toID == m_myID)	//自己发送给自己的消息直接丢弃
	{
		XMem::XDELETE_ARRAY(data->data);
		XMem::XDELETE(data);
		return false;
	}
	m_sendMutex.Lock();
	m_sendDeque.push_back(data);
	m_sendMutex.Unlock();
	return true;
}
INLINE bool XSocketEx::createNetWorkByName(const char *serverName,int port)		//通过服务器名称建立网络
{
	if(m_isInited ||	//重复连接
		serverName == NULL) return false;

	m_workMode = SOCKET_WORK_MODE_NAME;
	m_serverName = serverName;
	m_serverPort = port;
	m_conState = CON_STATE_NULL;
	//m_socketRole = NET_NULL;
	//下面建立连接线程
	m_connectThreadState = STATE_BEFORE_START;
	if(CreateThread(0,0,connectThread,this,0,NULL) == 0) return false;	//连接线程建立失败
	//下面建立数据处理线程(尚未完成)

	m_isInited = true;
	return true;
}
INLINE bool XSocketEx::createNetWorkByIP(const char *serverIP,int port)		//通过服务器IP建立网络
{//尚未完成
	if(m_isInited ||	//重复连接
		serverIP == NULL) return false;

	m_workMode = SOCKET_WORK_MODE_IP;
	m_serverIP = serverIP;
	m_serverPort = port;
	m_conState = CON_STATE_NULL;
	//m_socketRole = NET_NULL;
	//下面建立连接线程
	m_connectThreadState = STATE_BEFORE_START;
	if(CreateThread(0,0,connectThread,this,0,NULL) == 0) return false;	//连接线程建立失败
	//下面建立数据处理线程(尚未完成)

	m_isInited = true;
	return true;
}
INLINE bool XSocketEx::sendToAll(unsigned char * data,int len)
{
	switch(m_socketRole)
	{
	case NET_CLIENT: return sendAData(-1,data,len);
	case NET_SERVER://如果是服务器则向所有客户端发送数据
	//	m_userDataMutex.Lock();
		for(unsigned int i = 0;i < m_usersData.size();++ i)
		{
			if(m_usersData[i]->state == CLINET_STATE_CONNECT) 
				sendAData(m_usersData[i]->userID,data,len);
		}
	//	m_userDataMutex.Unlock();
		break;
	}
	return true;
}	
INLINE XSocketDataPack *XSocketEx::popAData()
{
	m_recvMutex.Lock();
	if(m_recvDeque.size() <= 0)
	{
		m_recvMutex.Unlock();
		return NULL;
	}
	XSocketDataPack *ret = m_recvDeque[0];
	m_recvDeque.pop_front();
	m_recvMutex.Unlock();
	return ret;
}
INLINE void XSocketEx::deleteAData(XSocketDataPack * p)
{
	XMem::XDELETE_ARRAY(p->data);
	XMem::XDELETE_ARRAY(p);
}
INLINE const XSocketUserInfo * XSocketEx::getUserInfo(unsigned int index)
{
	XSocketUserInfo * temp = NULL;
	m_userInfoMutex.Lock();
	if(index >= m_usersInfo.size())
	{
		m_userInfoMutex.Unlock();
		return NULL;
	}
	temp = m_usersInfo[index];
	m_userInfoMutex.Unlock();
	return temp;
}
INLINE void XSocketEx::setAsClient()
{ 
	if(m_isInited) return;
	m_socketRole = NET_CLIENT;
}
INLINE void XSocketEx::setAsServer()
{ 
	if(m_isInited) return;
	m_socketRole = NET_SERVER;
}