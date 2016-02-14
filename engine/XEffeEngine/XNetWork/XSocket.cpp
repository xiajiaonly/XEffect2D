#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XSocket.h"
#include "XSocketBasic.h"

#ifdef XEE_OS_LINUX
#include <sys/time.h>
#endif
namespace XE{
char XSocketMutualData[4] = {0x5a,0x6a,0x5a,0x6a};    //注意这里程序硬编码只适用4位，如果需要优化的话，这里需要修改代码

XSocket::XSocket()
:dataProtectionFlag(XTrue)
,m_needMutual(XTrue)        //默认是需要握手的
,m_flagClientThreadID(-1)
,m_WrongFlag(ERROR_TYPE_NULL)
{
#ifdef XEE_OS_LINUX
    signal(SIGPIPE,SIG_IGN);    //linux下send函数向失效的socket发送数据时将会导致，进程退出，通过这个设置来忽略这个错误.
#endif
    m_clientConnectState = XMem::createArrayMem<char>(MAX_CLIENT);
    m_clientDelayTimes = XMem::createArrayMem<char>(MAX_CLIENT);
    m_clientSocket = XMem::createArrayMem<int>(MAX_CLIENT);
    m_getDataBuff = XMem::createArrayMem<XSocketData>(MAX_DATABUFF_DEEP);
    m_broadcastBuff = XMem::createArrayMem<XSocketData>(MAX_DATABUFF_DEEP);
    m_netFlags = XMem::createArrayMem<char>(MAX_NET_FLAGS);

    pthread_mutex_init(m_mutex,NULL);                        //还没有考虑好如何添加它呢
    setHostID("Xiajia's PC");
}

XSocket::~XSocket()
{
	if(m_isServer == NET_STATE_SERVER)
	{
		m_serverExitFlag = 1;
	}else
	if(m_isServer == NET_STATE_CLIENT)
	{
		m_clientExitFlag = 1;
	}
	XEE::sleep(100000);
    XMem::XDELETE_ARRAY(m_clientConnectState);
    XMem::XDELETE_ARRAY(m_clientDelayTimes);
    XMem::XDELETE_ARRAY(m_clientSocket);
    XMem::XDELETE_ARRAY(m_getDataBuff);
    XMem::XDELETE_ARRAY(m_broadcastBuff);
    XMem::XDELETE_ARRAY(m_netFlags);
    closesocket(m_mySocket);
}

int XSocket::getDataCheckKey(int length,char *data) const
{
    char temp = 0;
    int i;
    for(i = 0;i < length;++ i)
    {
        temp += data[i];
    }
    return temp;
}

void XSocket::dataLockProc(int length,char *data,char lockKey)
{
    if(lockKey == 0) return;       //密钥为特殊值时跳过加密过程
    if(length <= 0) return;        //数据合理性检查
    if(data == NULL) return;
    int i = 0;
    unsigned char temp = lockKey;
    for(i = 0;i < length;++ i)
    {
        data[i] = temp ^ data[i];
        if(temp < 255) temp += 1;
        else temp = 0;
    }
}

int XSocket::dataUnlockProc(int length,char *data,char lockKey)
{
    if(lockKey == 0) return 1;    //密钥为特殊值时跳过解密过程
    if(length <= 0) return 1;     //数据合理性检查
    if(data == NULL) return 1;
   
    int i = 0;
    unsigned char temp = lockKey;
    for(i = 0;i < length;++ i)
    {
        data[i] = temp ^ data[i];
        if(temp < 255) temp += 1;
        else temp = 0;
    }
    return 1;
}

void XSocket::dataBale(XSocketData *temp)
{//打包函数完成的主要任务是数据校验和数据加密
    if(dataProtectionFlag) (*temp).lockKey = XRand::random(256);            //生成随机密钥
    else (*temp).lockKey = 0;
   
    (*temp).checkKey = getDataCheckKey((*temp).dataLength,(*temp).pData);     //产生校验数据
    dataLockProc((*temp).dataLength,(*temp).pData,(*temp).lockKey);           //加密数据
}

int XSocket::dataRelieve(XSocketData *temp)
{//返回数据解密是否成功
    dataUnlockProc((*temp).dataLength,(*temp).pData,(*temp).lockKey);         //数据解密   
    (*temp).lockKey = 0;                                                     //数据解密完成之后清楚掉密钥，防止重复解密造成的密文损坏
   
    char tempC = getDataCheckKey((*temp).dataLength,(*temp).pData);
    if(tempC != (*temp).checkKey) return 0;                                  //数据校验失败
    return 1;
}

int XSocket::createServer(int port)
{
    struct sockaddr_in serverAddr;                                           //服务器地址
    m_serverPort = port;

#ifdef XEE_OS_WINDOWS
    if(!XSock::initSocket()) return 0;
#endif
    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);              //获取服务器套接字
    if(m_serverSocket < 0)
    {
        addLogText(this,"服务器套接字申请失败!\n","Server socket get fail.\n");
        m_WrongFlag = ERROR_TYPE_SERVER_SOCKET_APPLY;
        return 0;
    }
   
    resetSocketAddr(m_serverSocket);
   
    memset(&serverAddr,0,sizeof(serverAddr));                                 
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(m_serverPort);                                //设置服务器端口
   
    if(bind(m_serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)  //绑定服务器套接字
    {
        addLogText(this,"服务器套接字邦定失败!\n","Server socket bind fail.\n");
        m_WrongFlag = ERROR_TYPE_SERVER_SOCKET_BIND;
        return 0;
    }
    if(listen(m_serverSocket, SOMAXCONN) < 0)                                //侦听连接请求
    {
        addLogText(this,"服务器监听失败!\n","Server listen fail.\n");
        m_WrongFlag = ERROR_TYPE_SERVER_SOCKET_LISTEN;
        return 0;
    }
    //初始化服务器数据
    m_isServer = NET_STATE_SERVER;                                                          //是否为服务器 0：无效 1：服务器 2：客户端
    m_myConnectState = 1;                                                    //是否连接到网络
    memset(m_clientConnectState,0,sizeof(char) * MAX_CLIENT);
    memset(m_clientDelayTimes,0,sizeof(char) * MAX_CLIENT);
    memset(m_clientSocket,0,sizeof(int) * MAX_CLIENT);

    m_myDelayTimes = 0;
    m_mySocket = m_serverSocket;
    m_clientSum = 0;
    m_serverExitFlag = 0;    //标志服务器没有退出
   
    clearDataBuff();         //清除消息队列
   
    //建立线程
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_serverAcceptThreadP, NULL, &acceptThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,acceptThread,this,0,&m_serverAcceptThreadP) == 0)
#endif
    {//建立服务器accept线程
        addLogText(this,"开启Accept线程失败!\n","Open accept thread error.\n");
        m_WrongFlag = ERROR_TYPE_THREAD_ACCEPT_CREATE;
        return 0;
    }
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_serverBroadcastThreadP, NULL, &broadcastThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,broadcastThread,this,0,&m_serverBroadcastThreadP) == 0)
#endif
    {//建立服务器Broadcast线程
        addLogText(this,"开启Broadcast线程失败!\n","Open broadcast thread fail.\n");
        m_WrongFlag = ERROR_TYPE_THREAD_BROADCAST_CREATE;
        return 0;
    }
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_mutualThreadP, NULL, &mutualThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,mutualThread,this,0,&m_mutualThreadP) == 0)
#endif
    {
        addLogText(this,"开启Mutual线程失败!\n","Open mutual thread fail.\n");
        m_WrongFlag = ERROR_TYPE_THREAD_MUNUAL_CREATE;
        return 0;
    }
    return 1;
}

void XSocket::distroyServer()
{
    m_serverExitFlag = 1;    //标志服务器没有退出
	XEE::sleep(500000);

    m_isServer = 0;                                   
    m_myConnectState = 0;                                   
    for(int i =0;i < MAX_CLIENT;++ i)
    {
        m_clientConnectState[i] = 0;
        m_clientDelayTimes[i] = 0;
        m_clientSocket[i] = 0;
    }
    m_myDelayTimes = 0;
    m_mySocket = m_serverSocket;
    m_clientSum = 0;
   
    closesocket(m_mySocket);
}

void XSocket::addAClient(int clientSocket)
{
    //查找一个没有使用的空间
    pthread_mutex_lock(m_mutex);
    for(int i = 0;i < MAX_CLIENT;++ i)
    {
        if(m_clientConnectState[i] == 0)
        {//查找到空间之后作相应的处理
            //++++++++++++++++++++++++++++
            addLogText(this,"开始等待标记！\n","Waiting for flag.\n");
            while(m_flagClientThreadID >= 0)
            {
                //if(m_flagClientThreadID < 0) break;
				XEE::sleep(1);
            }
            m_flagClientThreadID = i;
            addLogText(this,"等待标记结束 over！\n","Waiting for flag over.\n");
            //----------------------------
            m_clientSocket[i] = clientSocket;
            m_clientDelayTimes[i] = 0;
            m_clientConnectState[i] = 1;
            ++m_clientSum;
            //开启一个线程
            #ifdef XEE_OS_LINUX
                if(pthread_create(&(m_getDataThreadPOnServer), NULL, &(getDataThreadOnServer), this) != 0)
            #endif
            #ifdef XEE_OS_WINDOWS
                if(CreateThread(0,0,getDataThreadOnServer,this,0,&(m_getDataThreadPOnServer)) == 0)
            #endif
                {
                    addLogText(this,"开启单独客户端数据获取线程线程失败!\n","Open data recv client thread fail.\n");
                    m_WrongFlag = ERROR_TYPE_THREAD_CLIENT_GETDATA_CREATE;
                    break;
                }
            break;
        }
    }

    sendServerState();            //向客户端发送变更的服务器状态信息
    pthread_mutex_unlock(m_mutex);
}

#ifdef XEE_OS_WINDOWS
DWORD WINAPI XSocket::acceptThread(void * pParam)
#endif
#ifdef XEE_OS_LINUX
void *XSocket::acceptThread(void * pParam)
#endif
{
    XSocket &pPar = *(XSocket *) pParam;
    int clientSocket;

    addLogText(pPar,"Accept线程开启\n","Start Accept thread.\n");

    while(pPar.m_serverExitFlag == 0)
    {
        if(pPar.m_clientSum < MAX_CLIENT)                                    //连接成功之后做相应的处理
        {
            clientSocket = accept(pPar.m_serverSocket, NULL, NULL);
            addLogText(pPar,"有新的客户端加入\n","A new client join!\n");
            if(clientSocket < 0)
            {//接收客户端失败
                addLogText(pPar,"客户端加入失败\n","Client join fail.\n");
                pPar.m_WrongFlag = ERROR_TYPE_ACCEPT_CLIENT;
            }else
            {//接收客户端成功之后开启一个接收线程
                pPar.addAClient(clientSocket);
            }
        }
        XEE::sleep(100);    //线程中使用睡眠
    }
    addLogText(pPar,"accept线程退出!\n","Accept thread exit.\n");
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}

//这里可以优化为服务器被动的接受广播，估计会好很多
#ifdef XEE_OS_WINDOWS
DWORD WINAPI XSocket::broadcastThread(void * pParam)
#endif
#ifdef XEE_OS_LINUX
void *XSocket::broadcastThread(void * pParam)
#endif
{
    XSocket &pPar = *(XSocket *) pParam;
    int test_error;
    addLogText(pPar,"Broadcast线程开始\n","Start Broadcast thread.\n");
   
    int bBroadcast = 1;
    int hSocket = socket(PF_INET, SOCK_DGRAM, 0);
    if(hSocket >= 0) pPar.resetSocketAddr(hSocket);
    XSocketData BCServerInfoData;
    BCServerInfoData.dataType = BROADCAST_DATA_TYPE_SERVER_INFO;
    BCServerInfoData.sourceSocket = 0;
    BCServerInfoData.targetSocket = 0;
    BCServerInfoData.lockKey = 0;
    BCServerInfoData.checkKey = 0;
    BCServerInfoData.dataLength = MAX_NET_FLAGS + sizeof(int);

    int BCServerInfoDataLength = DATA_HEAD_SIZE + BCServerInfoData.dataLength;
    char *broadcastServerInfo = XMem::createArrayMem<char>(BCServerInfoDataLength);
    if(broadcastServerInfo == NULL) return 0;

    memcpy(broadcastServerInfo,&BCServerInfoData,DATA_HEAD_SIZE);                                    //拷贝数据头
    memcpy(broadcastServerInfo + DATA_HEAD_SIZE,&pPar.m_serverPort,sizeof(int));                    //拷贝数据体
    memcpy(broadcastServerInfo + DATA_HEAD_SIZE + sizeof(int),pPar.m_netFlags,MAX_NET_FLAGS);    //拷贝数据体
    char *broadcastUserData = NULL;	//这里用于广播玩家的数据

    if(hSocket >= 0 && setsockopt(hSocket, SOL_SOCKET, SO_BROADCAST, (char *) &bBroadcast, sizeof(bBroadcast)) <=0)
    {
        sockaddr_in addrUDP;
        memset(&addrUDP,0,sizeof(addrUDP));
        addrUDP.sin_family  = AF_INET;
        addrUDP.sin_port  = htons(0);
        addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(hSocket, (sockaddr*) &addrUDP, sizeof(addrUDP)) == 0)
        {
            addrUDP.sin_family = AF_INET;
            addrUDP.sin_port = htons(SERVER_PORT);
            addrUDP.sin_addr.s_addr = htonl(INADDR_BROADCAST);       
        //    addrUDP.sin_addr.s_addr = inet_addr("192.168.1.255");        //在OMAP3下,这里需要如此设置

            int time_temp = 100;
            while(true)
            {
                if(pPar.m_broadCastWay == BROADCAST_WAY_UDP)
                {//使用UDP广播的形式
                    pthread_mutex_lock(pPar.m_mutex);
                    if(pPar.m_broadcastOldPoint != pPar.m_broadcastNewPoint)	//需要等到广播队列为空才广播客户端连接的信息，这样实际上是不安全的
                    {//队列为空，无需发送
                        ++ pPar.m_broadcastOldPoint;
                        if(pPar.m_broadcastOldPoint >= BC_BUFF_DEEP) pPar.m_broadcastOldPoint = 0;
						broadcastUserData = XMem::createArrayMem<char>(DATA_HEAD_SIZE + pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].dataLength);
						if(broadcastUserData == NULL) return 0;
			
                        memcpy(broadcastUserData,&(pPar.m_broadcastBuff[pPar.m_broadcastOldPoint]),DATA_HEAD_SIZE);                                    //拷贝数据头
                        memcpy(broadcastUserData + DATA_HEAD_SIZE,pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].pData,
                            pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].dataLength);    //拷贝数据体
                        //然后开始发送
                        test_error = sendto(hSocket,broadcastUserData,
                            pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].dataLength + DATA_HEAD_SIZE,0,(sockaddr *) &addrUDP,sizeof(addrUDP));
                        if(test_error < pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].dataLength + DATA_HEAD_SIZE)
                        {
                            addLogText(pPar,"服务器数据广播失败!\n","Server send data turn error.\n");
                            //    if(test_error == -1)
                            //    {//对方网络主动断开
                            addLogText(pPar,"服务器数据转发错误-!\n","server data guangbo error!");
                            //    }
                        }
                        //发送完成之后释放数据
                        XMem::XDELETE_ARRAY(pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].pData);
                        pthread_mutex_unlock(pPar.m_mutex);

						XMem::XDELETE_ARRAY(broadcastUserData);
                        XEE::sleep(1);
                    }else
                    {   
                        pthread_mutex_unlock(pPar.m_mutex);
                        if(time_temp > 25)        //0.25s 广播一次消息
                        {
                            time_temp = 0;
                            test_error = sendto(hSocket,broadcastServerInfo,BCServerInfoDataLength, 0, (sockaddr *) &addrUDP, sizeof(addrUDP));
                            if(test_error < MAX_NET_FLAGS + sizeof(int))
                            {
                                addLogText(pPar,"服务器广播线程广播数据失败\n","Server broadcast thread send data fail.\n");
                            }
                            if(test_error == -1)
                            {
                                addLogText(pPar,"服务器广播错误!\n","Server Braodcast error!");
                            }
                        }
                        XEE::sleep(10000);
                        ++ time_temp;
                    }
                }else
                {
                    if(time_temp > 25)        //0.25s 广播一次消息
                    {
                        time_temp = 0;
                        test_error = sendto(hSocket,broadcastServerInfo,BCServerInfoDataLength, 0, (sockaddr *) &addrUDP, sizeof(addrUDP));
                        if(test_error < MAX_NET_FLAGS + sizeof(int))
                        {
                            addLogText(pPar,"服务器广播线程广播数据失败\n","Server broadcast thread send data fail.\n");
                        }
                        if(test_error == -1)
                        {
                            addLogText(pPar,"服务器广播错误!\n","Server Braodcast error!");
                        }
                    }
                    XEE::sleep(10000);
                    ++ time_temp;
                }
                if(pPar.m_serverExitFlag !=0)
                {
                    closesocket(hSocket);
                    break;
                }
            }
        }
    }
    XMem::XDELETE_ARRAY(broadcastServerInfo);
    addLogText(pPar,"Broadcast线程结束\n","Broadcast thread exit.\n");
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}

int XSocket::createClient()
{   
    struct sockaddr_in clientAddr;

#ifdef XEE_OS_WINDOWS
	if (!XSock::initSocket()) return 0;
#endif

    m_mySocket = socket(AF_INET, SOCK_STREAM, 0);         //取得服务器套接字
    if (m_mySocket < 0)
    {
        m_WrongFlag = ERROR_TYPE_CLIENT_SOCKET_APPLY;
        return 0;
    }
    resetSocketAddr(m_mySocket);

    memset(&clientAddr,0,sizeof(clientAddr));             
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddr.sin_port = htons(0);                       //设置客户端端口
    if(bind(m_mySocket, (sockaddr*) &clientAddr, sizeof (clientAddr)) < 0)        //绑定客户端套接字
    {
        m_WrongFlag = ERROR_TYPE_CLIENT_SOCKET_BIND;
        return 0;
    }
    //初始化相关的数据
    m_isServer = NET_STATE_CLIENT;                           
    m_myConnectState = 0;                   
    m_myDelayTimes = 0;                       
   
    m_clientExitFlag = 0;                   
    m_serverSocket = 0;                       
    m_serverPort = 0;                       
    memset(m_clientSocket,0,sizeof(m_clientSocket));
    m_clientSum = 0;
    m_serverID = 0;                           
    m_clientID = 0;
   
    clearDataBuff();    //清除消息队列
   
    return 1;
}
void XSocket::distroyClient()
{
    m_clientExitFlag = 1;
	XEE::sleep(500000);                   //等待半秒钟，等待所有线程资源释放       
    m_isServer = NET_STATE_NULL;
    closesocket(m_mySocket);
   
    m_myConnectState = 0;             //是否连接到网络
    m_myDelayTimes = 0;               //握手失败的次数
   
    m_clientExitFlag = 0;             //客户端退出标志
    m_serverSocket = 0;                       
    m_serverPort = 0;                       
    memset(m_clientSocket,0,sizeof(m_clientSocket));
    m_clientSum = 0;
    m_serverID = 0;                           
    m_clientID = 0;
    clearDataBuff();                 //清除消息队列
}
int XSocket::connectToServer(sockaddr_in serverAddr)
{//连接到服务器
#ifdef XEE_OS_WINDOWS
	if (!XSock::initSocket()) return 0;
#endif
    m_serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
 
    if(m_serverSocket < 0)
    {
        addLogText(this,"服务器套接字分配失败!\n","Server socket get error.\n");
        m_WrongFlag = ERROR_TYPE_CLIENT_S_SOCKET_APPLY;
        return 0;
    }
    resetSocketAddr(m_serverSocket);
       
    sockaddr_in addrRemote;

    memset(&addrRemote,0,sizeof(addrRemote));
    addrRemote.sin_family = AF_INET;
    addrRemote.sin_port = htons(m_serverPort);
    addrRemote.sin_addr.s_addr = serverAddr.sin_addr.s_addr;

    if(connect(m_serverSocket,(sockaddr*)&addrRemote,sizeof(addrRemote)) != 0)
    {
        addLogText(this,"连接服务器失败\n","Connect server fail.\n");
        m_WrongFlag = ERROR_TYPE_CLIENT_CONNECT_SERVER;
        return 0;
    }
    //开启数据接受线程
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_getDataThreadPOnClient, NULL, &getDataThreadOnClient, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,getDataThreadOnClient,this,0,&m_getDataThreadPOnClient) == 0)
#endif
    {
        addLogText(this,"开启获得数据线程失败!\n","Open data recv thread fail.\n");
        m_WrongFlag = ERROR_TYPE_THREAD_GETDATA_CREATE_N;
        return 0;
    }
    //连接服务器成功之后开启握手线程
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_mutualThreadP, NULL, &mutualThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,mutualThread,this,0,&m_mutualThreadP) == 0)
#endif
    {
        addLogText(this,"开启mutual线程失败!\n","Open mutual thread fail.\n");
        m_WrongFlag = ERROR_TYPE_THREAD_MUNUAL_CREATE_N;
        return 0;
    }
    return 1;
}

void XSocket::setHostID(const char *p)
{
    memset(m_netFlags,0,sizeof(m_netFlags));
    if(p == NULL) return;
    if(strlen(p) < MAX_NET_FLAGS)
    {//长度在范围内直接复制
        strcpy(m_netFlags,p);
    }else
    {//长度超过范围需要截取
        memcpy(m_netFlags,p,(MAX_NET_FLAGS - 1) * sizeof(char));
        m_netFlags[MAX_NET_FLAGS - 1] = '\0';
    }
    addLogText(this,"设置服务器ID!\n","Set host ID.\n");
}

int XSocket::searchNet(const char *p)
{
    setHostID(p);
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,searchNetThread,this,0,&m_clientGetBroadcastThreadP) == 0)
#endif
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_clientGetBroadcastThreadP, NULL, &searchNetThread, (void*) this) != 0)
#endif
    {//客户端接受服务器的广播线程
        m_WrongFlag = ERROR_TYPE_THREAD_GETBROADCAST_CREATE_N;
        return 0;
    }
	XEE::sleep(500000);    //等待1s
   
//    int tempRandTime = random(3);    //加入这个随机因子是为了防止同时启动机器，不过其实机器的启动已经具有随机性，所以这里可能会适得其反
//    sleep(tempRandTime * 500000)
   
    return m_myConnectState;
}

int XSocket::searchNet()
{
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,searchNetThread,this,0,&m_clientGetBroadcastThreadP) == 0)
#endif
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_clientGetBroadcastThreadP, NULL, &searchNetThread, (void*) this) != 0)
#endif
    {//客户端接受服务器的广播线程
        m_WrongFlag = ERROR_TYPE_THREAD_GETBROADCAST_CREATE_N;
        return 0;
    }
	XEE::sleep(500000);    //等待1s
    return m_myConnectState;
}

#ifdef XEE_OS_WINDOWS
DWORD WINAPI XSocket::searchNetThread(void * pParam)
#endif
#ifdef XEE_OS_LINUX
void *XSocket::searchNetThread(void * pParam)
#endif
{
    XSocket &pPar = *(XSocket *) pParam;
    addLogText(pPar,"开启搜索网络线程\n","Start netsearch thread.\n");
   
    sockaddr_in addrUDP;
    memset(&addrUDP,0,sizeof(addrUDP));       
    XSocketData tempBroadCastData;
    tempBroadCastData.pData = XMem::createArrayMem<char>(BC_DATA_SIZE - DATA_HEAD_SIZE);
    if(tempBroadCastData.pData == NULL) return 0;//内存分配失败

    addrUDP.sin_family = AF_INET;
    addrUDP.sin_port = htons(SERVER_PORT);
    addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
   
    int hSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if(hSocket >= 0) pPar.resetSocketAddr(hSocket);
    if(hSocket >= 0 && bind(hSocket, (sockaddr*) &addrUDP, sizeof(addrUDP)) >= 0)
    {
    /*  int nTimeout = 100; // 设置超时一秒
        if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *) &nTimeout, sizeof(int)) == 0)
        {
            addLogText(pPar,"接收超时设置成功\n","Connect timeout set success\n");
        }*/

        sockaddr_in addrPeer;        //接受方的地址
#ifdef XEE_OS_WINDOWS
        int nSize = sizeof(addrPeer);
#endif
#ifdef XEE_OS_LINUX 
        socklen_t nSize = sizeof(addrPeer);
#endif
        int nRecvLength = 0;
        char szRecvBuffer[BC_DATA_SIZE];
		memset(szRecvBuffer,0,BC_DATA_SIZE);

        //int time_temp = 0;
        while(true)
        {
            if(pPar.m_myConnectState == 0)
            {//如果没有连接到网络,则这里接受网络广播
                addLogText(pPar,"flag X\n","flag X\n");
                //sleep(1);
            }else
            {//如果这里已经连接到网络,则这里不需要再接受网络广播
                addLogText(pPar,"flag O\n","flag O\n");
                //sleep(500000);
                //continue;
            }
        //  addLogText(pPar,"客户端接受广播数据\n","cleint get broadcast data!\n");
			memset(szRecvBuffer,0,BC_DATA_SIZE);
            nRecvLength = recvfrom(hSocket, szRecvBuffer, BC_DATA_SIZE, 0, (sockaddr*)&addrPeer, &nSize);
            if(nRecvLength > 0)
            {
                //+++++++++++++++++++++++++++++++++++++++++++
                //分析提取数据
                memcpy(&tempBroadCastData,szRecvBuffer,DATA_HEAD_SIZE);
                memcpy(tempBroadCastData.pData,szRecvBuffer + DATA_HEAD_SIZE,tempBroadCastData.dataLength);
                //-------------------------------------------
                if(pPar.m_myConnectState == 0)    //如果客户端没有连接到服务器，或者第一次获得服务器信息，这里都要进行相应的连接
                {//尚未连接网络
                    if(tempBroadCastData .dataType == BROADCAST_DATA_TYPE_SERVER_INFO)
                    {
                        memcpy(&pPar.m_serverPort,tempBroadCastData.pData,sizeof(pPar.m_serverPort));    //获得服务器连接端口号
                       
                        if(strcmp(tempBroadCastData.pData + sizeof(int),pPar.m_netFlags) == 0)
                        {//如果标志位检测正确，这里就连接网络

                            if(pPar.connectToServer(addrPeer) == 1)
                            {
                                pPar.m_myConnectState = 1;
                               
                                //注意网络连接成功之后，并没有结束这个线程，目的在于网络服务器掉线之后再次连接时能重新连接服务器
                                //这个功能还有代测试
                            }else
                            {
                                pPar.m_myConnectState = 0;
                            }
                        }
                    }//否则直接抛弃数据
                }else
                {//已经连接网络
                    if(tempBroadCastData .dataType == DATA_TYPE_BOARDCAST_DATA && pPar.m_broadCastWay == BROADCAST_WAY_UDP)
                    {//收到服务器端的广播信号
                        pthread_mutex_lock(pPar.m_mutex);
                        //pPar.test_data ++;
                        addLogText(pPar,"客户端收到服务器转发数据!\n","Client get server turn data.\n");

                        ++ pPar.m_getDataNewP;
                        if(pPar.m_getDataNewP >= MAX_DATABUFF_DEEP) pPar.m_getDataNewP = 0;
                        if(pPar.m_getDataNewP == pPar.m_getDataOldP)
                        {//如果发生数据拥堵,这里删除最后一个没有处理的数据
                            ++ pPar.m_getDataOldP;
                            if(pPar.m_getDataOldP >= MAX_DATABUFF_DEEP) pPar.m_getDataOldP = 0;
                        }
                        XMem::XDELETE_ARRAY(pPar.m_getDataBuff[pPar.m_getDataNewP].pData);
                        //拷贝数据头
                        memcpy(&(pPar.m_getDataBuff[pPar.m_getDataNewP]),&tempBroadCastData,DATA_HEAD_SIZE);
                        //拷贝数据体
                        pPar.m_getDataBuff[pPar.m_getDataNewP].pData = XMem::createArrayMem<char>(tempBroadCastData.dataLength *sizeof(char));
                        if(pPar.m_getDataBuff[pPar.m_getDataNewP].pData == 0) return 0;

                        memcpy(pPar.m_getDataBuff[pPar.m_getDataNewP].pData,tempBroadCastData.pData,tempBroadCastData.dataLength); 

                        pthread_mutex_unlock(pPar.m_mutex);
                    }//否则直接抛弃数据
                }
#if WITH_LOG
                {
                    char tempLogTextData[256];
                    sprintf(tempLogTextData,"我的连接状态:%d,dataType:%d\n",pPar.m_myConnectState,tempBroadCastData .dataType);
                    addLogText(pPar,tempLogTextData,tempLogTextData);
                }
#endif
                addLogText(pPar,"接收到服务器的广播数据\n","Get server broadcast data.\n");
            }else
            {
                addLogText(pPar,"没有接收到服务器的广播数据\n","No server broadcast data.\n");
            }
            if(pPar.m_clientExitFlag != 0 || pPar.m_isServer == NET_STATE_SERVER)
            {//如果是服务器,则直接退出这个网络搜索线程
                closesocket(hSocket);
                break;
            }
			XEE::sleep(1);
        }
    }
    XMem::XDELETE_ARRAY(tempBroadCastData.pData);
    addLogText(pPar,"退出搜索网络线程\n","Exit from search net thread.\n");
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}

#ifdef XEE_OS_WINDOWS
DWORD WINAPI XSocket::mutualThread(void * pParam)
#endif                       
#ifdef XEE_OS_LINUX
void *XSocket::mutualThread(void * pParam)
#endif                       
{
   XSocket &pPar = *(XSocket *) pParam;
    int test_error;
    int i;
    addLogText(pPar,"Mutual线程开启!\n","Start Mutual thread.\n");

    XSocketData temp_data;
    temp_data.pData = XMem::createArrayMem<char>(4*sizeof(char));
    if(temp_data.pData == NULL) return 0;

    if(pPar.m_isServer == NET_STATE_SERVER)
    {//服务器，向所有有效的客户端发送握手信号
    //  char tempMutualData[4];
        while(pPar.m_needMutual)
        {
            //if(!pPar.m_needMutual) break;    //如果设置为不需要握手信息则直接退出这个线程
			XEE::sleep(1000000);
            if(pPar.m_clientSum > 0)
            {
                //for(int i = 0;i < pPar.m_clientSum;i ++)
                for(i = 0;i < MAX_CLIENT;++ i)
                {
                    if(i < 0) i =0;
                    if(pPar.m_clientConnectState[i] != 0)
                    {
                    //    if(pPar.m_clientDelayTimes[i] < (int)(MAX_DELAY_TIMES / 2.0))
                        {//如果延迟时间延允许范围内,则不发送延迟数据,只做延迟计算
                            ++ pPar.m_clientDelayTimes[i];
                            //下面为超时处理
                            if(pPar.m_clientDelayTimes[i] >= MAX_DELAY_TIMES)
                            {//有客户端掉线
                                pthread_mutex_lock(pPar.m_mutex);
                                pPar.dropNetProc(i);
                                pthread_mutex_unlock(pPar.m_mutex);
                          
                                -- i;
                            }
                    //        continue;
                        }
                        temp_data.dataType = DATA_TYPE_MUTUAL;
                        temp_data.sourceSocket = pPar.m_mySocket;
                        temp_data.targetSocket = pPar.m_clientSocket[i];
                        temp_data.dataLength = sizeof(XSocketMutualData); 
                        memcpy(temp_data.pData,XSocketMutualData,sizeof(XSocketMutualData));

                        pPar.dataBale(&temp_data);
                   
                    //  addLogText(pPar,"服务器发送握手数据!\n","Server send mutual data.\n");
                        test_error = pPar.sendDataFunction(pPar.m_clientSocket[i],temp_data,temp_data.dataLength + DATA_HEAD_SIZE);
                        if(test_error < temp_data.dataLength + DATA_HEAD_SIZE)
                        {
#if WITH_LOG
                            {
                                char tempLogTextData[256];
                                sprintf(tempLogTextData,"握手信号发送失败!%d：%d\n",pPar.m_clientSocket[i],i);
                                addLogText(pPar,tempLogTextData,tempLogTextData);
                            }
#endif                         
                            addLogText(pPar,"握手信号发送失败!\n","Mutual data send error.\n");
                            //对方网络主动断开
                        //    if(test_error == -1)
                        //    {
                                addLogText(pPar,"服务器发送握手数据错误!\n","Server send mutual error!\n");

                                pthread_mutex_lock(pPar.m_mutex);
                                pPar.dropNetProc(i);
                                pthread_mutex_unlock(pPar.m_mutex);
                           
                                -- i;
                        //    }
                        }else
                        {
#if WITH_LOG
                            {
                                char tempLogTextData[256];
                                sprintf(tempLogTextData,"Server send mutual succes!%d,%d\n",i,pPar.m_clientDelayTimes[i]);
                                addLogText(pPar,tempLogTextData,tempLogTextData);
                            }
#endif                       
                            pthread_mutex_lock(pPar.m_mutex);
                            ++ pPar.m_clientDelayTimes[i];
                            //下面为超时处理
                            if(pPar.m_clientDelayTimes[i] >= MAX_DELAY_TIMES)
                            {//有客户端掉线
                                pPar.dropNetProc(i);
                                -- i;
                            }
                            pthread_mutex_unlock(pPar.m_mutex);
                            addLogText(pPar,"操作完成!\n","Opareator over!\n");
                        }
                    }
                }
            }
            if(pPar.m_serverExitFlag != 0)
            {
                break;
            }
        }
    }else
    if(pPar.m_isServer == NET_STATE_CLIENT)
    {//客户端
        while(pPar.m_needMutual)
        {
            //if(!pPar.m_needMutual) break;    //如果设置为不需要握手信息则直接退出这个线程
			XEE::sleep(1000000);
            temp_data.dataType = DATA_TYPE_MUTUAL;
            temp_data.sourceSocket = pPar.m_mySocket;
            temp_data.targetSocket = pPar.m_serverSocket;
            temp_data.dataLength = sizeof(XSocketMutualData); 
            memcpy(temp_data.pData,XSocketMutualData,sizeof(XSocketMutualData));
            pPar.dataBale(&temp_data);

            addLogText(pPar,"客户端发送握手数据!\n","Client send mutual data.\n");
       
            test_error = pPar.sendDataFunction(pPar.m_serverSocket,temp_data,temp_data.dataLength + DATA_HEAD_SIZE);

            if(test_error < temp_data.dataLength + DATA_HEAD_SIZE)
            {
                addLogText(pPar,"客户端握手信号发送失败!\n","Client send mutual data error.\n");

            //  if(test_error == -1)
            //  {//对方网络主动断开
                    addLogText(pPar,"客户端发送握手信号错误!\n","Client send mutual error!\n");

                    pPar.dropNetProc();
                    break;
            //  }
            }else
            {
                addLogText(pPar,"客户端发送握手信号成功!\n","Client send mutual succes!\n");

                ++ pPar.m_myDelayTimes;
       
                //下面为超时处理
                if(pPar.m_myDelayTimes >= MAX_DELAY_TIMES)
                {//如果是客户端超时，则客户端全部卸载，等待重新连接
                    pthread_mutex_lock(pPar.m_mutex);
                    pPar.dropNetProc();
                    pthread_mutex_unlock(pPar.m_mutex);
                    break;
                }
            }           
            if(pPar.m_clientExitFlag != 0)
            {
                break;
            }
        }
    }
    XMem::XDELETE_ARRAY(temp_data.pData);
    addLogText(pPar,"Mutual线程退出!\n","Mutual thread exit.\n");
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}

int XSocket::getAMessageClient(XSocketData tempData)
{
    if(tempData.dataType == DATA_TYPE_USER_DATA || tempData.dataType == DATA_TYPE_BOARDCAST_DATA)
    {//客户消息 ，将数据推入堆栈
        addLogText(this,"客户端收到服务器转发数据!\n","Client get server turn data.\n");
       
        pthread_mutex_lock(m_mutex);
        if(pushAMessage(tempData) == 0)
        {
            pthread_mutex_unlock(m_mutex);
            return 0;
        }
        pthread_mutex_unlock(m_mutex);
    }else
    if(tempData.dataType == DATA_TYPE_MUTUAL)
    {
        if(dataRelieve(&tempData) != 0)
        {
            addLogText(this,"客户端收到服务器延迟数据!\n","Client get delay data.\n");
            if(tempData.pData[0] == XSocketMutualData[0] &&
                tempData.pData[1] == XSocketMutualData[1] &&
                tempData.pData[2] == XSocketMutualData[2] &&
                tempData.pData[3] == XSocketMutualData[3])
            {
                m_myDelayTimes = 0;    //受到握手数据之后清除延迟记录
                addLogText(this,"客户端收到握手数据\n","Client get mutual data.\n");
            }
        }else
        {
            addLogText(this,"客户端收数据解包失败!\n","Client get data.Open fail.\n");
        }
    }else
    if(tempData.dataType == DATA_TYPE_SERVER_STATE)
    {//服务器消息
        if(dataRelieve(&tempData) != 0)
        {
            addLogText(this,"客户端收到服务器状态信息!\n","Client get server state data.\n");
            pthread_mutex_lock(m_mutex);
           
            memcpy(&m_clientSum,&tempData.pData[0],sizeof(int));                    //获取客户端数量
            memcpy(&m_serverID,&tempData.pData[sizeof(int)],sizeof(int));            //获取服务器Socket
            memcpy(&m_clientID,&tempData.pData[2*sizeof(int)],sizeof(int));        //获取服务器Socket
           
            for(int i = 0;i < m_clientSum -1;++ i)
            {//对于客户端来说，一个客户端时，这里只有服务器
                memcpy(&m_clientSocket[i],&tempData.pData[3*sizeof(int) + i*sizeof(int)],sizeof(int));   
            }
            pthread_mutex_unlock(m_mutex);
            addLogText(this,"客户端的服务器状态信息处理完成!\n","Client proc over for server state.\n");
        }else
        {
            addLogText(this,"客户端收数据解包失败!\n","Client get data.Open fail.\n");
        }
    }else
    {
        addLogText(this,"客户端收到未定义结构数据!\n","Client get undefine data.\n");
    }
    return 1;
}

#ifdef XEE_OS_WINDOWS
DWORD WINAPI XSocket::getDataThreadOnClient(void * pParam)
#endif
#ifdef XEE_OS_LINUX
void *XSocket::getDataThreadOnClient(void * pParam)
#endif
{
    XSocketData temp_data;
    //XSocketData temp_BC_data;
    XSocket &pPar = *(XSocket *) pParam;
    int test_error;
    addLogText(pPar,"开启数据接收线程!\n","Start data recv thread.\n");

    if(pPar.m_isServer != NET_STATE_CLIENT) return 0;    //只有客户端能使用这个线程

    while(pPar.m_clientExitFlag == 0)
    {
        XEE::sleep(10);

        temp_data.pData = NULL;
        test_error = pPar.recvDataProc(pPar.m_serverSocket,&temp_data);

        if(test_error > 0)    //接受数据
        {//接受到数据，这里处理数据
            addLogText(pPar,"客户端接受到数据\n","Client get data.\n");
            if(pPar.getAMessageClient(temp_data) == 0) return 0;
        }else
        if(test_error == -1)
        {
            addLogText(pPar,"客户端接收错误!\n","client recv error!\n");
        }
        XMem::XDELETE_ARRAY(temp_data.pData);
        
    //    if(pPar.m_clientExitFlag != 0)
    //    {
    //        break;
     //   }
    }
    addLogText(pPar,"数据接受线程结束\n","Data recv thread exit.\n");
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}

int XSocket::pushABroadcastMessage(XSocketData tempData)
{
    //将数据放入广播队列中，以便于之后进行全员广播
    ++ m_broadcastNewPoint;
    if(m_broadcastNewPoint >= BC_BUFF_DEEP) m_broadcastNewPoint = 0;
    if(m_broadcastNewPoint == m_broadcastOldPoint)
    {//发生数据拥堵，这里抛弃拥堵到数据
        //释放数据
        ++ m_broadcastOldPoint;
        if(m_broadcastOldPoint >= BC_BUFF_DEEP) m_broadcastOldPoint = 0;
    }
    //将数据推入广播队列
    m_broadcastBuff[m_broadcastNewPoint].dataType = DATA_TYPE_BOARDCAST_DATA;
    m_broadcastBuff[m_broadcastNewPoint].sourceSocket = tempData.sourceSocket;

    m_broadcastBuff[m_broadcastNewPoint].targetSocket = 0;                    //广播的时候默认目标Socket为0
    m_broadcastBuff[m_broadcastNewPoint].lockKey = tempData.lockKey;        //目前广播数据不需要加密和交验
    m_broadcastBuff[m_broadcastNewPoint].checkKey = tempData.checkKey;
    m_broadcastBuff[m_broadcastNewPoint].dataLength = tempData.dataLength;

    XMem::XDELETE_ARRAY(m_broadcastBuff[m_broadcastNewPoint].pData);

    m_broadcastBuff[m_broadcastNewPoint].pData = XMem::createArrayMem<char>(tempData.dataLength);        //分配内存空间
    if(m_broadcastBuff[m_broadcastNewPoint].pData == NULL) return 0;    //注意这里是一个非常严重的错误，将会造成非常严重的影响

    memcpy(m_broadcastBuff[m_broadcastNewPoint].pData,tempData.pData,tempData.dataLength);
    return 1;
}

int XSocket::pushAMessage(XSocketData tempData)
{
    //将数据推入接收队列
    ++ m_getDataNewP;
    if(m_getDataNewP >= MAX_DATABUFF_DEEP) m_getDataNewP = 0;
    if(m_getDataNewP == m_getDataOldP)
    {//如果发生数据拥堵,这里删除最后一个没有处理的数据
        ++ m_getDataOldP;
        if(m_getDataOldP >= MAX_DATABUFF_DEEP) m_getDataOldP = 0;
    }
    XMem::XDELETE_ARRAY(m_getDataBuff[m_getDataNewP].pData);
    //拷贝数据头
    memcpy(&(m_getDataBuff[m_getDataNewP]),&tempData,DATA_HEAD_SIZE);
    //拷贝数据体
    m_getDataBuff[m_getDataNewP].pData = XMem::createArrayMem<char>(tempData.dataLength * sizeof(char));
    if(m_getDataBuff[m_getDataNewP].pData == NULL) return 0;

    memcpy(m_getDataBuff[m_getDataNewP].pData,tempData.pData,tempData.dataLength);
    return 1;
}

int XSocket::getAMessageServer(XSocketData tempData,int clientID)
{
    if(tempData.dataType == DATA_TYPE_USER_DATA)
    {//客户消息，将数据推入堆栈
        if(tempData.pData == NULL)
        {//如果没有数据需要处理的话，直接返回
            addLogText(this,"内存分配错误!\n","server data memery error!");
            return 1;
        }       
        if(tempData.targetSocket == -1)
		{
            pthread_mutex_lock(m_mutex);
			if(pushABroadcastMessage(tempData) == 0) 
			{
				pthread_mutex_unlock(m_mutex);
				return 0;
			}
            pthread_mutex_unlock(m_mutex);
		}else
        if(tempData.targetSocket != m_serverSocket)
        {//如果目标不是自己则转发
            if(sendDataFunction(tempData.targetSocket,tempData,tempData.dataLength + DATA_HEAD_SIZE)
                < tempData.dataLength + DATA_HEAD_SIZE)
            {
                addLogText(this,"数据转发失败!\n","Server send data turn error.\n");
            }
        }else
        {//如果目标为自己,则将数据推入队列
            pthread_mutex_lock(m_mutex);
            if(pushAMessage(tempData) == 0)
            {
                pthread_mutex_unlock(m_mutex);
                return 0;
            }
            pthread_mutex_unlock(m_mutex);
        }
    }else
    if(tempData.dataType == DATA_TYPE_MUTUAL)
    {//握手数据
        if(dataRelieve(&tempData) != 0)
        {
            if(tempData.pData[0] == XSocketMutualData[0] &&
                tempData.pData[1] == XSocketMutualData[1] &&
                tempData.pData[2] == XSocketMutualData[2] &&
                tempData.pData[3] == XSocketMutualData[3])
            {
                m_clientDelayTimes[clientID] = 0;    //收到握手数据之后清除延迟记录
                addLogText(this,"收到握手数据,ID:","Server get mutual data,ID:");
#if WITH_LOG
                {
                    char tempLogTextData[256];
                    sprintf(tempLogTextData,"%d.\n",clientID);
                    addLogText(this,tempLogTextData,tempLogTextData);
                }
#endif
            }
        }else
        {
            addLogText(this,"收到握手数据,解包失败!\n","Server get mutual data.Open it fail.\n");
        }
    }else
    if(tempData.dataType == DATA_TYPE_BOARDCAST_DATA)
    {//收到的是广播数据
        pthread_mutex_lock(m_mutex);
        //将数据放入广播队列中，以便于之后进行全员广播
        if(pushABroadcastMessage(tempData) == 0) 
        {
            pthread_mutex_unlock(m_mutex);
            return 0;
        }
        //将数据推入接收队列
        if(pushAMessage(tempData) == 0)
        {
            pthread_mutex_unlock(m_mutex);
            return 0;
        }
        
        pthread_mutex_unlock(m_mutex);
    }else
    {//非法数据
        addLogText(this,"服务器接收到未知结构的数据!\n","Server get undefime data.\n");
    }
    return 1;
}

void XSocket::broadcastDataByTcpIP(XSocketData tempData)
{
    if(tempData.pData == NULL) return;    //如果没有数据需要发送的话直接返回
    for(int i = 0;i < MAX_CLIENT;++ i)
    {
        if(m_clientSocket[i] != 0 && m_clientSocket[i] != tempData.sourceSocket)
        {
            if(sendDataFunction(m_clientSocket[i],tempData,tempData.dataLength + DATA_HEAD_SIZE) < tempData.dataLength + DATA_HEAD_SIZE)
            {
                addLogText(this,"服务器数据广播失败!\n","Server send data turn error.\n");
            }
			XEE::sleep(1);
        }
    }
}

#ifdef XEE_OS_WINDOWS
DWORD WINAPI XSocket::getDataThreadOnServer(void * pParam)
#endif
#ifdef XEE_OS_LINUX
void *XSocket::getDataThreadOnServer(void * pParam)
#endif
{
    XSocketData tempData;
    XSocketData tempBCData;
    XSocket &pPar = *(XSocket *) pParam;
    int test_error = 0;
    int myClientID = 0;
    addLogText(pPar,"开启单数据接收线程!\n","Start single data recv thread.\n");
   
    pthread_mutex_lock(pPar.m_mutex);
    if(pPar.m_flagClientThreadID < 0 || pPar.m_flagClientThreadID >= MAX_CLIENT)
    {
        addLogText(pPar,"单客户接受线程参数数据错误!\n","Client data recv thread param error.\n");
    }
    myClientID = pPar.m_flagClientThreadID;
    pPar.m_flagClientThreadID = -1;    //去除标记
    pthread_mutex_unlock(pPar.m_mutex);

    //DSHOW("开启接受数据线程:%d\n",myClientID);

    if(pPar.m_isServer != NET_STATE_SERVER) return 0;//只有服务器才能使用这个线程
    while(pPar.m_serverExitFlag == 0)
    {
		XEE::sleep(10);
        tempData.pData = NULL;
        if(myClientID >= 0 && myClientID < MAX_CLIENT)
        {
            test_error = pPar.recvDataProc(pPar.m_clientSocket[myClientID],&tempData);
            if(test_error > 0)    //接受数据
            {//接受到数据，这里处理数据
                if(pPar.getAMessageServer(tempData,myClientID) == 0) return 0;
            }else
            if(test_error == 0)
            {//如果没有接受到数据
                if(pPar.m_broadCastWay == BROADCAST_WAY_TCPIP)
                {//使用TCPIP方式进行广播
                    pthread_mutex_lock(pPar.m_mutex);
                    if(pPar.m_broadcastOldPoint != pPar.m_broadcastNewPoint)
                    {//队列为空，无需发送
                        ++ pPar.m_broadcastOldPoint;
                        if(pPar.m_broadcastOldPoint >= BC_BUFF_DEEP) pPar.m_broadcastOldPoint = 0;
                    /*    temp_BC_data.dataType = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].dataType;
                        temp_BC_data.checkKey = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].checkKey;
                        temp_BC_data.lockKey = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].lockKey;
                        temp_BC_data.sourceSocket = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].sourceSocket;
                        temp_BC_data.targetSocket = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].targetSocket;
                        temp_BC_data.dataLength = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].dataLength;
                        temp_BC_data.pData = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].pData;
                        */
                        tempBCData = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint]; //这里使用默认赋值操作符，由于有指针所以这里要特别小心
                        pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].pData = NULL;
                        pthread_mutex_unlock(pPar.m_mutex);

                        //向所有广播玩家发送数据
                        pPar.broadcastDataByTcpIP(tempBCData);

                        //发送完成之后释放数据
                        XMem::XDELETE_ARRAY(tempBCData.pData);
                    }else
                    {
                        pthread_mutex_unlock(pPar.m_mutex);
                    }
                }
            }else
            if(test_error == -1)
            {
                addLogText(pPar,"服务器接收错误","server recv error!\n");
            }
            XMem::XDELETE_ARRAY(tempData.pData);
        }

        //+++++++++++++++++++++++++++++++++
        //如果这个客户端退出,则它的线程也需要退出
        //---------------------------------
        if(pPar.m_clientConnectState[myClientID] == 0)
        {
            break;
        }

        //if(pPar.m_serverExitFlag != 0)
        //{
        //    break;
        //}
    }

    addLogText(pPar,"单数据接受线程结束:","Single data recv thread exit.\n");
#if WITH_LOG
    {
        char tempLogTextData[256];
        sprintf(tempLogTextData,"%d.\n",myClientID);
        addLogText(pPar,tempLogTextData,tempLogTextData);
    }
#endif   
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}

int XSocket::sendDataToNet(int clientOrder,char *p,int length)
{
//  m_logFile.addLogInfo("发送数据");
    //addLogText(this,"进入数据发送函数!\n","Enter function of data send!\n");
    int testError = 0;
    XSocketData tempData;

	if(clientOrder < 0)
	{//广播
		if(length >= BC_DATA_SIZE - DATA_HEAD_SIZE) return 0;    //超过广播的最大字节限制
	}
    if(m_isServer == NET_STATE_SERVER)
    {//服务器
		if(clientOrder >= MAX_CLIENT) return 0;
        if(clientOrder >= 0)
		{
			if(m_clientConnectState[clientOrder] == 0) return 0;
			tempData.dataType = DATA_TYPE_USER_DATA;
			tempData.sourceSocket = m_mySocket;
			tempData.targetSocket = m_clientSocket[clientOrder];
			tempData.dataLength = length;   
	       
			char * sendData = XMem::createArrayMem<char>(length * sizeof(char));    //分配内存空间来暂存数据
			if(sendData == NULL) return 0;

			memcpy(sendData,p,length);
			tempData.pData = sendData;

			dataBale(&tempData);
			//addLogText(this,"--> 服务器发送数据!\n","--> Server send data!\n");
			testError = sendDataFunction(m_clientSocket[clientOrder],tempData,length + DATA_HEAD_SIZE);
	       
			XMem::XDELETE_ARRAY(sendData);//发送完成之后将暂存的数据空间释放

			if(testError < length + DATA_HEAD_SIZE)
			{//数据发送失败
				addLogText(this,"服务器发送数据错误!\n","server send data error!");
				dropNetProc(clientOrder);
#if WITH_LOG
				{
					char tempLogTextData[256];
					sprintf(tempLogTextData,"Server data send error!%d：%d\n",m_clientSocket[clientOrder],clientOrder);
					addLogText(this,tempLogTextData,tempLogTextData);
				}
#endif
				return 0;
			}
		}else
		{//广播
			tempData.dataType = DATA_TYPE_BOARDCAST_DATA;
			tempData.sourceSocket = m_mySocket;
			tempData.targetSocket = -1;
			tempData.dataLength = length;   
	       
			char * sendData = XMem::createArrayMem<char>(length * sizeof(char));    //分配内存空间来暂存数据
			if(sendData == NULL) return 0;

			memcpy(sendData,p,length);
			tempData.pData = sendData;

			dataBale(&tempData);
			pthread_mutex_lock(m_mutex);
			if(pushABroadcastMessage(tempData) == 0) 
			{
				pthread_mutex_unlock(m_mutex);
				return 0;
			}
			pthread_mutex_unlock(m_mutex);
			XMem::XDELETE_ARRAY(sendData);//发送完成之后将暂存的数据空间释放
		}
    }else
    if(m_isServer == NET_STATE_CLIENT)
    {//客户端
        if(clientOrder >= m_clientSum) return 0;
        if(clientOrder >= 0)
        {//TCP/IP 点对点的数据发送
            tempData.dataType = DATA_TYPE_USER_DATA;
            tempData.sourceSocket = m_clientID;
            if(clientOrder == 0) tempData.targetSocket = m_serverID;
            else tempData.targetSocket = m_clientSocket[clientOrder -1];
        }else
        {//广播数据
            tempData.dataType = DATA_TYPE_BOARDCAST_DATA;
            tempData.sourceSocket = m_clientID;
            tempData.targetSocket = -1;
        }
        tempData.dataLength = length;  

        char * sendData = XMem::createArrayMem<char>(length * sizeof(char));    //分配内存空间来暂存数据
        if(sendData == NULL) return 0;

        memcpy(sendData,p,length);
        tempData.pData = sendData;                 

        dataBale(&tempData);
       
        //addLogText(this,"客户端发送数据!\n","Client send data.\n");
        testError = sendDataFunction(m_serverSocket,tempData,length + DATA_HEAD_SIZE);

        XMem::XDELETE_ARRAY(sendData);//发送完成之后将暂存的数据空间释放

        if(testError < length + DATA_HEAD_SIZE)
        {
            addLogText(this,"客户端发送数据失败!\n","Client send data error!\n");
            dropNetProc();
#if WITH_LOG
            {
                char tempLogTextData[256];
                sprintf(tempLogTextData,"客户端用户数据发送失败!%d：%d\n",tempData.targetSocket,clientOrder);
                addLogText(this,tempLogTextData,tempLogTextData);
            }
#endif
            return 0;
        }
    }
    //addLogText(this,"结束数据发送函数!\n","Exit function of data send!\n");
    return 1;
}

int XSocket::popAMessage(XSocketData *tempData)
{
    pthread_mutex_lock(m_mutex);
    if(m_getDataOldP != m_getDataNewP)
    {
        ++ m_getDataOldP;
        if(m_getDataOldP >= MAX_DATABUFF_DEEP) m_getDataOldP = 0;
       
        //XSocketData temp_data;
        tempData->pData = NULL;
        memcpy(tempData,&m_getDataBuff[m_getDataOldP],DATA_HEAD_SIZE);                //拷贝数据头
        tempData->pData = XMem::createArrayMem<char>(tempData->dataLength * sizeof(char));
        if(tempData->pData == NULL) return 0;

        if(m_getDataBuff[m_getDataOldP].pData == NULL) return 0;    //如果没有数据直接返回

        memcpy(tempData->pData,m_getDataBuff[m_getDataOldP].pData,tempData->dataLength);
        XMem::XDELETE_ARRAY(m_getDataBuff[m_getDataOldP].pData);//删除暂存的内存空间
        pthread_mutex_unlock(m_mutex);
        return 1;
    }else
    {
        pthread_mutex_unlock(m_mutex);
        return 0;
    }
}

int XSocket::getDataFromNet(int *clientOrder,int *clientID,char **p,int *length)
{
//  m_logFile.addLogInfo("接收数据");
    if(m_isServer == NET_STATE_SERVER)
    {//服务器
        XSocketData tempData;
        if(popAMessage(&tempData) == 0)
        {//没有消息
            return 0;
        }else
        {//提取消息成功
            if(dataRelieve(&tempData) != 0)
            {//数据符合要求之后，将数据传出去
                (*length) = tempData.dataLength;

                XMem::XDELETE_ARRAY((*p));//释放p已经有的资源

                (*p) = XMem::createArrayMem<char>(tempData.dataLength * sizeof(char));    //重新给p分配资源
                if((*p) == NULL) 
                {
                    XMem::XDELETE_ARRAY(tempData.pData);//删除暂存资源
                    return 0;
                }

                memcpy((*p),tempData.pData,tempData.dataLength);    //将数据拷贝出去
                XMem::XDELETE_ARRAY(tempData.pData);//删除暂存资源

                for(int i = 0;i < MAX_CLIENT;++ i)
                {
                    if(m_clientSocket[i] == tempData.sourceSocket && m_clientConnectState[i] != 0)
                    {
                        (* clientOrder) = i;
                        (* clientID) = tempData.sourceSocket;
                        return 1;
                    }
                }
#if WITH_LOG
                {//如果在客户端掉线之前，对应的接受数据没有处理完，这里就会出现这个错误。这里的处理方法是直接将数据丢弃
                    char tempLogTextData[256];
                    sprintf(tempLogTextData,"-->从缓存中读取数据失败!%d,%d,%d\n",m_clientSocket[0],tempData.sourceSocket,m_getDataOldP);
                    addLogText(this,tempLogTextData,tempLogTextData);
                }
#endif
                return 0;
            }else
            {//数据解包失败
                XMem::XDELETE_ARRAY(tempData.pData);//删除暂存资源

                addLogText(this,"服务器端获得数据包，解包错误!\n","Server get data,Open data error!\n");
                return 0;
            }
        }
    }else
    if(m_isServer == NET_STATE_CLIENT)
    {//客户端
        XSocketData tempData;
        if(popAMessage(&tempData) == 0)
        {//没有消息
            return 0;
        }else
        {
            if(dataRelieve(&tempData) != 0)// || temp_data.dataType == DATA_TYPE_BOARDCAST_DATA)
            {
                (*length) = tempData.dataLength;
                XMem::XDELETE_ARRAY((*p));//释放p已经有的资源
               
                (*p) = XMem::createArrayMem<char>(tempData.dataLength * sizeof(char));    //重新给p分配资源
                if((*p) == NULL) 
                {
                    XMem::XDELETE_ARRAY(tempData.pData);//删除暂存资源
                    return 0;
                }
                memcpy((*p),tempData.pData,tempData.dataLength);    //将数据拷贝出去
                XMem::XDELETE_ARRAY(tempData.pData);//删除暂存资源

                if(tempData.sourceSocket == m_serverID)
                {
                    (* clientOrder) = 0;
                    (* clientID) = tempData.sourceSocket;
                    return 1;
                }
                //客户端是按照顺序排列的，所以可以不用修改
                for(int i = 0;i < m_clientSum;++ i)
                {
                    if(m_clientSocket[i] == tempData.sourceSocket)
                    {
                        (* clientOrder) = i +1;
                        (* clientID) = tempData.sourceSocket;
                        return 1;
                    }
                }

                addLogText(this,"客户端缓存区数据读取失败!\n","Client data buff get data error.\n");
                return 0;
            }else
            {//数据解包失败
                XMem::XDELETE_ARRAY(tempData.pData);//删除暂存资源
                addLogText(this,"客户端获得数据包，解包错误!\n","Client get data，Open data error.\n");
                return 0;
            }
        }
    }else
    {//既不是服务器也不是客户端
        //addLogText(this,"严重的意外错误!\n","A error!\n");
        return 0;
    }
//    return 1;
}

//下面为对外接口
int XSocket::connectToNet(int port,const char *serverID)
{
    setBroadCastWay(BROADCAST_WAY_UDP);
    setHostID(serverID);                        //设置网络ID
    if(createClient() == 0)
    {
        addLogText(this,"建立客户端失败!\n","Create client error.\n");
        return 0;            //建立客户端
    }
    if(searchNet() == 0)                        //搜索网络并连接服务器
    {
        addLogText(this,"网络中没有服务器!\n","There is no server online\n");
        distroyClient();
       
        if(createServer(port) == 0)
        {
            addLogText(this,"建立服务器失败!\n","Create server error.\n");
            return 0;    //建立服务器
        }
    }
    return 1;
}

int XSocket::xCreateServer(const char *serverIP,int port)
{
    setBroadCastWay(BROADCAST_WAY_TCPIP);
    struct sockaddr_in serverAddr;                                        //服务器地址
    m_serverPort = port;

#ifdef XEE_OS_WINDOWS
	if (!XSock::initSocket()) return 0;
#endif
    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);        //获取服务器套接字

    if(m_serverSocket < 0)
    {
        m_WrongFlag = ERROR_TYPE_SERVER_SOCKET_APPLY;
        return 0;
    }
   
    resetSocketAddr(m_serverSocket);

    memset(&serverAddr,0,sizeof(serverAddr));                            
    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(m_serverPort);                            //设置服务器端口
   
    if(bind(m_serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)        //绑定服务器套接字
    {
        m_WrongFlag = ERROR_TYPE_SERVER_SOCKET_BIND;
        return 0;
    }
    addLogText(this,"开启监听\n","Open listen thread.\n");
    if(listen(m_serverSocket, SOMAXCONN) < 0)                //侦听连接请求
    {
        m_WrongFlag = ERROR_TYPE_SERVER_SOCKET_LISTEN;
        return 0;
    }
    //初始化服务器数据
    m_isServer = NET_STATE_SERVER;                                        //是否为服务器 0：无效 1：服务器 2：客户端
    m_myConnectState = 1;                                    //是否连接到网络
    for(int i =0;i < MAX_CLIENT;++ i)
    {
        m_clientConnectState[i] = 0;
        m_clientDelayTimes[i] = 0;
        m_clientSocket[i] = 0;
    }
    m_myDelayTimes = 0;
    m_mySocket = m_serverSocket;
    m_clientSum = 0;
    m_serverExitFlag = 0;    //标志服务器没有退出
   
    clearDataBuff();    //清除消息队列
   
    addLogText(this,"开始建立线程\n","Start open thread.\n");
    //建立线程
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_serverAcceptThreadP, NULL, &acceptThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,acceptThread,this,0,&m_serverAcceptThreadP) == 0)
#endif
    {//建立服务器accept线程
        m_WrongFlag = ERROR_TYPE_THREAD_ACCEPT_CREATE;
        return 0;
    }
    addLogText(this,"建立accept线程\n","Open accept thread.\n");
/*    if(pthread_create(&m_serverBroadcastThreadP, NULL, &broadcastThread, (void*) this) != 0)
    {//建立服务器Broadcast线程
        m_WrongFlag = ERROR_TYPE_THREAD_BROADCAST_CREATE;
        return 0;
    }
    addLogText(this,"建立Broadcast线程\n","Open broadcast thread.\n");*/
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_mutualThreadP, NULL, &mutualThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,mutualThread,this,0,&m_mutualThreadP) == 0)
#endif
    {
        m_WrongFlag = ERROR_TYPE_THREAD_MUNUAL_CREATE;
        return 0;
    }
    addLogText(this,"建立Mutual线程\n","Open mutual thread.\n");

    return 1;
}

int XSocket::xCreateClient(const char *serverIP,int serverPort)
{
    setBroadCastWay(BROADCAST_WAY_TCPIP);
   if(createClient() == 0) return 0;
    m_serverPort = serverPort;

    sockaddr_in addrRemote;
    memset(&addrRemote,0,sizeof(addrRemote));
    addrRemote.sin_family = AF_INET;
    addrRemote.sin_port = htons(serverPort);
    addrRemote.sin_addr.s_addr = inet_addr(serverIP);
   
    if(connectToServer(addrRemote) == 0) return 0;
    return 1;
}

void XSocket::sendServerState()
{
	if(m_clientSum <= 0) return;
    XSocketData temp_data;
    char tempSeverStateData[(MAX_CLIENT + 3) * sizeof(int)];
    int test_error;
    int i,j,k;
    //for(int i = 0;i < m_clientSum;i ++)
    for(i = 0;i < MAX_CLIENT;++ i)
    {
        if(m_clientConnectState[i] != 0)
        {
        /*
#if WITH_LOG
            {//如果在客户端掉线之前，对应的接受数据没有处理完，这里就会出现这个错误。这里的处理方法是直接将数据丢弃
                char tempLogTextData[256];
                sprintf(tempLogTextData,"服务器向客户端 %d 发送服务器状态信息!\n",i);
                addLogText(pPar,tempLogTextData,tempLogTextData);
            }
#endif    */
            temp_data.dataType = DATA_TYPE_SERVER_STATE;
            temp_data.sourceSocket = m_mySocket;
            temp_data.targetSocket = m_clientSocket[i];
            temp_data.dataLength = m_clientSum * sizeof(int) + 2 * sizeof(int);      
            memcpy(&tempSeverStateData[0],&m_clientSum,sizeof(int));
            memcpy(&tempSeverStateData[sizeof(int)],&m_mySocket,sizeof(int));   
               
            memcpy(&tempSeverStateData[2 * sizeof(int)],&m_clientSocket[i],sizeof(int));
            //3.5版本的数据发送需要修改
            //for(int j = 0,k = 0;j < m_clientSum ;j ++)
            for(j = 0,k = 0;j < MAX_CLIENT;++ j)
            {
                if(m_clientSocket[j] != m_clientSocket[i] && m_clientConnectState[j] != 0)
                {//不是自身,并且在线
                    memcpy(&tempSeverStateData[3 * sizeof(int) + k * sizeof(int)],&m_clientSocket[j],sizeof(int));   
                    ++ k;   
                }
            }                   
            temp_data.pData = tempSeverStateData;
            dataBale(&temp_data);
           
            addLogText(this,"服务器发送状态信息\n","Server send state data.\n");
            test_error = sendDataFunction(m_clientSocket[i],temp_data,temp_data.dataLength + DATA_HEAD_SIZE);
            if(test_error < temp_data.dataLength + DATA_HEAD_SIZE)
            {
                addLogText(this,"服务器数据发送错误\n","Server data send error.\n");
            //    if(test_error == -1)
            //    {//对方网络断开
                    addLogText(this,"发送服务器状态数据错误!\n","Send server state error!\n");
                    dropNetProc(i);
                    -- i;
            //    }
            }
        }
    }
}

void XSocket::dropNetProc(int clientID)
{
    addLogText(this,"网络成员退出,ID:","Connect member exit.ID:");
#if WITH_LOG
    {
        char tempLogTextData[256];
        sprintf(tempLogTextData,"%d\n",clientID);
        addLogText(this,tempLogTextData,tempLogTextData);
    }
#endif
    if(m_isServer == NET_STATE_SERVER)
    {//服务器
        if(clientID < 0 || clientID >= MAX_CLIENT) return;
        //3.5版本掉线处理重新设计
        m_clientSocket[clientID] = 0;
        m_clientDelayTimes[clientID] = 0;
        m_clientConnectState[clientID] = 0;
        -- m_clientSum;
		if(m_clientSum < 0) m_clientSum = 0;
      /*for(int j = clientID;j < m_clientSum -1;j++)
        {
            m_clientConnectState[j] = m_clientConnectState[j +1];
            m_clientDelayTimes[j] = m_clientDelayTimes[j +1];
            m_clientSocket[j] = m_clientSocket[j +1];
        }
        m_clientConnectState[m_clientSum -1] = 0;
        -- m_clientSum;*/
        sendServerState();
    }else
    if(m_isServer == NET_STATE_CLIENT)
    {//客户端
        distroyClient();
    }
}
int XSocket::recvDataFunction(int sock,char *temp_data,int length)
{
//    addLogText(this,"进入数据接收函数!\n","Enter data recv proc.\n");
    if(sock < 0) return 0;
    int test_error;
    {//非阻塞访问
        fd_set readfds;
        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        FD_ZERO(&readfds);
        FD_SET(sock,&readfds);
        test_error = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
        if(test_error >0 && FD_ISSET(sock,&readfds))
        {//可以接受数据
            FD_CLR(sock, &readfds);
            test_error = recv(sock,temp_data,length,0);
        }
    }
//    addLogText(this,"退出数据接收函数!\n","Exit data recv proc.\n");
    return test_error;
}

//说明3.5版本修改之后，数据发送需要先发送数据头部分，然后再发送数据部分。
//int XSocket::sendDataFunction(int sock,char *temp_data,int length)
int XSocket::sendDataFunction(int sock,XSocketData temp_data,int length)
{
    addLogText(this,"进入数据发送函数!\n","Enter data send proc.\n");
   
    if(sock < 0) return 0;
    int test_error = 0;
    char *sendData = XMem::createArrayMem<char>((DATA_HEAD_SIZE + temp_data.dataLength) * sizeof(char));    //为数据发送创建存储空间
    if(sendData == NULL) return 0;

    memcpy(sendData,&temp_data,DATA_HEAD_SIZE);                                    //拷贝数据头
    memcpy(sendData + DATA_HEAD_SIZE,temp_data.pData,temp_data.dataLength);        //拷贝数据体
   
    {//非阻塞访问
        fd_set writefds;
        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 1;
        FD_ZERO(&writefds);
        FD_SET(sock,&writefds);
        test_error = select(FD_SETSIZE,NULL,&writefds,NULL,&timeout);
        if(test_error > 0 && FD_ISSET(sock,&writefds))
        {//可以发送数据
            FD_CLR(sock, &writefds);
            test_error = send(sock,sendData,length,0);
        }
    }
    //test_error = send(sock,sendData,length,0);
    XMem::XDELETE_ARRAY(sendData);
    //addLogText(this,"退出数据发送函数!\n","Exit data send proc.\n");
    return test_error;
}

int XSocket::recvDataProc(int sock,XSocketData *temp_data)
{
    if(sock < 0) return 0;
    int test_error = 0;
    test_error = recvDataFunction(sock,(char *)temp_data,DATA_HEAD_SIZE);
    if(test_error == 0) return 0;
    if(test_error > 0 && test_error < DATA_HEAD_SIZE)
    {//读取不完整包头,循环读取完整
    //  addLogText(this,"收到不完整包头!\n","Get a part of head data.\n");
        int test_getSum;
        char test_data[DATA_HEAD_SIZE];
        while(true)
        {
            test_getSum = recvDataFunction(sock,test_data,DATA_HEAD_SIZE - test_error);
            if(test_getSum == -1)
            {//通讯错误
                test_error = -1;
                break;
            }
            if(test_getSum > 0)
            {
                memcpy((unsigned char *)(temp_data) + test_error,test_data,test_getSum);
                test_error += test_getSum;
                if(test_error >= DATA_HEAD_SIZE) break;
            }
        }
    }
    if(test_error == -1) return -1;            //数据错误
    if(test_error >= DATA_HEAD_SIZE)
    {//读取完整包头之后,读取完整的数据部分
        /*
#if WITH_LOG
        {
            char tempLogTextData[256];
            sprintf(tempLogTextData,"接受到完整包头,%d\n",temp_data->dataLength);
            addLogText(this,tempLogTextData,tempLogTextData);
        }
#endif    */
        //分配足够的内存空间来存数数据体
        temp_data->pData = XMem::createArrayMem<char>(temp_data->dataLength * sizeof(char));    //分配内存空间
        if(temp_data->pData == NULL) return 0;

        test_error = recvDataFunction(sock,temp_data->pData,temp_data->dataLength);
        if(test_error > 0 && test_error < temp_data->dataLength)
        {
            char *test_data = XMem::createArrayMem<char>(temp_data->dataLength * sizeof(char));
            if(test_data == NULL) return 0;

            int test_getSum;
            while(true)
            {
                test_getSum = recvDataFunction(sock,test_data, temp_data->dataLength - test_error);
                if(test_getSum == -1)
                {//通讯错误
                    test_error = -1;
                    break;
                }
                if(test_getSum > 0)
                {
                    memcpy(temp_data->pData + test_error,test_data,test_getSum);
                    test_error += test_getSum;
                    if(test_error >= temp_data->dataLength) break;
                }
            }
            XMem::XDELETE_ARRAY(test_data);
        }
        if(test_error == -1)
        {
            XMem::XDELETE_ARRAY(temp_data->pData);
            return -1;
        }
        if(test_error >= temp_data->dataLength)
        {//数据读取完成lockKey
            /*
#if WITH_LOG
            {
                char tempLogTextData[256];
                sprintf(tempLogTextData,"一个数据包收取完成,数据类型为:%d,数据长度为:%d,%d,%d\n",temp_data->dataType,temp_data->dataLength,temp_data->lockKey,temp_data->checkKey);
                addLogText(this,tempLogTextData,tempLogTextData);
            }
#endif        */
            return  temp_data->dataLength + DATA_HEAD_SIZE;
        }
    }
    return 0;
}

void XSocket::clearDataBuff()
{
    //memset(m_getDataBuff,0,sizeof(m_getDataBuff));    //清除消息堆栈
    int i = 0;
    for(i = 0;i < MAX_DATABUFF_DEEP;++ i)
    {
        m_getDataBuff[i].checkKey = 0;
        m_getDataBuff[i].dataLength = 0;
        m_getDataBuff[i].dataType = 0;
        m_getDataBuff[i].lockKey = 0;
        m_getDataBuff[i].sourceSocket = 0;
        m_getDataBuff[i].targetSocket = 0;
        XMem::XDELETE_ARRAY(m_getDataBuff[i].pData);
    }
    m_getDataNewP = 0;
    m_getDataOldP = 0;   

    //memset(m_broadcastBuff,0,sizeof(m_broadcastBuff));    //清除消息堆栈
    for(i = 0;i < BC_BUFF_DEEP;++ i)
    {
        m_broadcastBuff[i].checkKey = 0;
        m_broadcastBuff[i].dataLength = 0;
        m_broadcastBuff[i].dataType = 0;
        m_broadcastBuff[i].lockKey = 0;
        m_broadcastBuff[i].sourceSocket = 0;
        m_broadcastBuff[i].targetSocket = 0;
        XMem::XDELETE_ARRAY(m_broadcastBuff[i].pData);
    }
    m_broadcastNewPoint = 0;
    m_broadcastOldPoint = 0;   
}
/*
int XSocket::broadcastData(char *pData,int length,int source)        //发送广播数据
{
    //数据和理性检查
    if(length <= 0) return 0;
    if(pData == NULL) return 0;
    if(length >= BC_DATA_SIZE - DATA_HEAD_SIZE) return 0;    //超过广播的最大字节限制

    if(m_isServer == NET_STATE_SERVER)
    {//服务器直接将数据推入发送队列
        pthread_mutex_lock(&m_mutex);
        ++ m_broadcastNewPoint;
        if(m_broadcastNewPoint >= BC_BUFF_DEEP) m_broadcastNewPoint = 0;
        if(m_broadcastNewPoint == m_broadcastOldPoint)
        {//发生数据拥堵，这里抛弃拥堵到数据
            ++ m_broadcastOldPoint;
            if(m_broadcastOldPoint >= BC_BUFF_DEEP) m_broadcastOldPoint = 0;
        }
        //将数据推入广播队列
        m_broadcastBuff[m_broadcastNewPoint].dataType = DATA_TYPE_BOARDCAST_DATA;
        if(source == -1)
        {//自己是源
            m_broadcastBuff[m_broadcastNewPoint].sourceSocket = m_mySocket;
        }else
        {//服务器转发
            m_broadcastBuff[m_broadcastNewPoint].sourceSocket = source;
        }
        m_broadcastBuff[m_broadcastNewPoint].targetSocket = 0;    //广播的时候默认目标Socket为0
        m_broadcastBuff[m_broadcastNewPoint].lockKey = 0;        //目前广播数据不需要加密和交验
        m_broadcastBuff[m_broadcastNewPoint].checkKey = 0;
        m_broadcastBuff[m_broadcastNewPoint].dataLength = length;
        XMem::XDELETE_ARRAY(m_broadcastBuff[m_broadcastNewPoint].pData);
        m_broadcastBuff[m_broadcastNewPoint].pData = XMem::createArrayMem<char>(length);        //分配内存空间
        if(m_broadcastBuff[m_broadcastNewPoint].pData == NULL) return 0;    //注意这里是一个非常严重的错误，将会造成非常严重的影响
        memcpy(m_broadcastBuff[m_broadcastNewPoint].pData,pData,length);
        dataBale(&m_broadcastBuff[m_broadcastNewPoint]);
        pthread_mutex_unlock(&m_mutex);
    }else
    if(m_isServer == NET_STATE_CLIENT)
    {//客户端通过tcp/ip给服务器发送广播数据
        if(sendDataToNet(-1,pData,length) == 0) return 0;
    }
    return 1;
}
*/
}