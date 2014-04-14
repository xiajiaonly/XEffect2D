//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFtpServer.h"
#include "XDirectory.h"

DWORD WINAPI _XFtpServer::serverCMDThread(void * pParam)		//服务器命令线程
{
	_XFtpServer * pPar = (_XFtpServer *) pParam;
	char srv_cmd[SERVER_COMMAND_LEN];

	while(1)
	{
		printf("ftp_server> ");	
		fgets(srv_cmd,SERVER_COMMAND_LEN,stdin);
		if(strncmp("exit",srv_cmd,4) == 0 || strncmp("quit",srv_cmd,4) == 0 || strncmp("bye",srv_cmd,3) == 0)
		{//接收服务器的命令字并作出相应的反应
			printf("Thank you for use!\nBye!\n");
			exit(1);	//服务器退出
		}
		if(strncmp("clientsum",srv_cmd,9) == 0) printf("There are %d Current active users.\n",pPar->m_clientSum); //显示连接的客户端的数量
		if(strncmp("countall",srv_cmd,8) == 0) printf("There are %d visitors\n",pPar->m_allClientSum);
		if(strncmp("listname",srv_cmd,8) == 0) 
		{//罗列所有客户端的名字
			printf("It is not finished!\n");
		}
		if(strncmp("killuser",srv_cmd,8) == 0)
		{//强制关闭某个客户端的连接
			printf("It is not finished!\n");
		}
		if(strncmp("help",srv_cmd,4) == 0)
		{//显示帮助信息
			printf("It is not finished!\n");
		}
	}
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}
DWORD WINAPI _XFtpServer::serverAcceptThread(void * pParam)	//服务器连接线程
{
	int sinSize = sizeof(sockaddr_in);
	sockaddr_in clientAddr;
	_XFtpServer * pPar = (_XFtpServer *) pParam;
	int clientSock;	//客户端的套接字
	while(1)
	{
		printf("ftp_server> ");	
		clientSock = accept(pPar->m_serverSock,(sockaddr *)&clientAddr,&sinSize);
		if(clientSock == -1)
		{
			printf("Accept error!\n");
			exit(1);
		}
		_XFtpClientSockInfo * tempClient = createMem<_XFtpClientSockInfo>();
		if(tempClient == NULL) exit(1);
		
		tempClient->clientSock = clientSock;
		memcpy(&tempClient->clientAddr,&clientAddr,sizeof(clientAddr));
		tempClient->pClass = pParam;
		//建立客户端响应线程
		pthread_t thread;
#ifdef XEE_OS_LINUX
		if(pthread_create(&thread,NULL,&serverRequestThread,(void*)tempClient) != 0)
#endif
#ifdef XEE_OS_WINDOWS
		if(CreateThread(0,0,serverRequestThread,tempClient,0,&thread) == 0)
#endif
		{//线程建立失败
			printf("Server command thread create error!\n");
			exit(1);
		}
	}
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}
DWORD WINAPI _XFtpServer::serverRequestThread(void * pParam)	//服务器请求回应线程(尚未完成)
{//对客户端的请求作出相应,这是整个的核心部分
	_XFtpClientSockInfo  *clientInfo = (_XFtpClientSockInfo *)pParam;
	_XFtpServer * pPar = (_XFtpServer *) clientInfo->pClass;
	printf("You got a connection from %s\n",inet_ntoa(clientInfo->clientAddr.sin_addr));
	//login操作(存在问题)
	_XFtpUserType clientType = pPar->clientLoginProc(clientInfo->clientSock);
	char recvBuf[RECV_BUFF_SIZE];
	if(clientType == FTP_USER_TYPE_DEFAULT)
	{//普通用户的命令回应
		while(1)
		{//接受用户命令并对用户的命令作出回应(尚未完成)
			if(!pPar->recvData(clientInfo->clientSock,recvBuf)) break;
			//下面分析并处理命令
			if(strncmp("quit",recvBuf,4) == 0 || strncmp("QUIT",recvBuf,4) == 0)
			{//quit
				pPar->sendData(clientInfo->clientSock,returnStr221,strlen(returnStr221));
				-- pPar->m_clientSum;	//注意多线程互斥
				break;	//结束接收循环
			}else
			if(strncmp("close",recvBuf,5) == 0 || strncmp("CLOSE",recvBuf,5) == 0)
			{//close(尚未完成)
				break;
			}else
			if(strncmp("pwd",recvBuf,3) == 0 || strncmp("PWD",recvBuf,3) == 0)
			{//设置当前路径
				char retStr[RECV_BUFF_SIZE];
				sprintf(retStr,returnStr257,clientInfo->nowPath);
				pPar->sendData(clientInfo->clientSock,retStr,strlen(retStr));
			}else
			if(strncmp("cwd",recvBuf,3) == 0 || strncmp("CWD",recvBuf,3) == 0)
			{//改变当前路径
				char path[RECV_BUFF_SIZE];
				char retStr[RECV_BUFF_SIZE];
				int len = strlen(recvBuf);
				memcpy(path,recvBuf + 4,len - 6);
				path[len - 6] = '\0';
				//判断路径是否存在
				if(isExistFileEx(path) && isFolderEx(path))	//是文件夹并存在
				{//存在则返回路径设置成功
					strcpy(clientInfo->nowPath,path);
					sprintf(retStr,returnStr250,clientInfo->nowPath);
					pPar->sendData(clientInfo->clientSock,retStr,strlen(retStr));
				}else
				{//不存在则返回路径设置失败
					sprintf(retStr,"550 %s is not exist!\r\n",path);
					pPar->sendData(clientInfo->clientSock,retStr,strlen(retStr));
				}
			}else
			if(strncmp("dele",recvBuf,4) == 0 || strncmp("DELE",recvBuf,4) == 0)
			{//删除指定文件
				char filename[RECV_BUFF_SIZE];
				char retStr[RECV_BUFF_SIZE];
				int len = strlen(recvBuf);
				memcpy(filename,recvBuf + 5,len - 7);
				filename[len - 7] = '\0';
				//判断是否删除成功
				sprintf(retStr,"%s/%s",clientInfo->nowPath + 1,filename);
				if(isExistFileEx(retStr) && !isFolderEx(retStr) && deleteFile(retStr))
				{//文件存在
					sprintf(retStr,"250 \"%s\" is deleted!\r\n",filename);
					pPar->sendData(clientInfo->clientSock,retStr,strlen(retStr));
				}else
				{//如果删除失败
					sprintf(retStr,"550 %s is not exist!\r\n",filename);
					pPar->sendData(clientInfo->clientSock,retStr,strlen(retStr));
				}
			}else
			if(strncmp("list",recvBuf,4) == 0 || strncmp("LIST",recvBuf,4) == 0)
			{//返回当前目录的文件列表
				pPar->sendData(clientInfo->clientSock,returnStr125,strlen(returnStr125));	//提示就绪
				//开始传输list数据
				char pathStr[RECV_BUFF_SIZE];
				char nowPath[MAX_FILE_NAME_LENGTH];
				GetCurrentDirectory(MAX_FILE_NAME_LENGTH,nowPath);
				sprintf(pathStr,"%s/%s/*.*",nowPath,clientInfo->nowPath);
				string needStr = "";
				HANDLE hFind;
				WIN32_FIND_DATA fileInfo;
				SYSTEMTIME systemTime;
				hFind = FindFirstFile(pathStr, &fileInfo);
				if(hFind != INVALID_HANDLE_VALUE)
				{
					while(1)
					{
						if(strcmp(fileInfo.cFileName,".") != 0 && strcmp(fileInfo.cFileName,"..") != 0)
						{
							FileTimeToSystemTime(&fileInfo.ftCreationTime,&systemTime);
							if(fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
							{//文件夹
								sprintf(nowPath,"%02d-%02d-%02d %02d:%02d  <DIR>  %s\n",systemTime.wMonth,systemTime.wDay,systemTime.wYear,systemTime.wHour,systemTime.wMinute,
									fileInfo.cFileName);
							}else
							{
								sprintf(nowPath,"%02d-%02d-%02d %02d:%02d  %8d  %s\n",systemTime.wMonth,systemTime.wDay,systemTime.wYear,systemTime.wHour,systemTime.wMinute,
									(fileInfo.nFileSizeHigh << 16) + fileInfo.nFileSizeLow,fileInfo.cFileName);
							}
							needStr += nowPath;
						}
						if(!FindNextFile(hFind,&fileInfo)) break;
					}
				}
				int len = needStr.length();
				for(int i = 0;i < len;i += RECV_BUFF_SIZE)
				{
					if(i + RECV_BUFF_SIZE >= len)
					{
						send(clientInfo->clientDataSock,needStr.c_str() + i,len - i,0);
					}else
					{
						send(clientInfo->clientDataSock,needStr.c_str() + i,RECV_BUFF_SIZE,0);
					}
				}

				//数据传输完成返回
				closesocket(clientInfo->clientDataSock);
				pPar->sendData(clientInfo->clientSock,returnStr226,strlen(returnStr226));	//提示数据传输完成
			}else
			if(strncmp("port",recvBuf,4) == 0 || strncmp("PORT",recvBuf,4) == 0)
			{//设置传输端口:PORT 192,168,1,110,20,30
				int portData[6];
				sscanf(recvBuf + 5,"%d,%d,%d,%d,%d,%d",&portData[0],&portData[1],&portData[2],&portData[3],&portData[4],&portData[5]);
				clientInfo->clientDataSock = socket(AF_INET,SOCK_STREAM,0);
				sockaddr_in clientDataSock;
				clientDataSock.sin_addr.S_un.S_un_b.s_b1 = portData[0];
				clientDataSock.sin_addr.S_un.S_un_b.s_b2 = portData[1];
				clientDataSock.sin_addr.S_un.S_un_b.s_b3 = portData[2];
				clientDataSock.sin_addr.S_un.S_un_b.s_b4 = portData[3];
				clientDataSock.sin_family = AF_INET;
				clientDataSock.sin_port = htons((portData[4] << 8) + portData[5]);
				if(connect(clientInfo->clientDataSock,(const sockaddr *)&clientDataSock,sizeof(sockaddr))==SOCKET_ERROR)
				{//端口连接失败
					printf("connect error!\n");
					return 1;	//这里需要注意
				}
				char retStr[RECV_BUFF_SIZE];
				sprintf(retStr,returnStr200,"PORT");
				pPar->sendData(clientInfo->clientSock,retStr,strlen(retStr));
			}else
			if(strncmp("type",recvBuf,4) == 0 || strncmp("TYPE",recvBuf,4) == 0)
			{//改变数据的传输方式
				char mode = recvBuf[5];
				if(mode == 'I' || mode == 'i') clientInfo->transnatType = FTP_TRANSNAION_TYPE_BIN;else
				if(mode == 'A' || mode == 'a') clientInfo->transnatType = FTP_TRANSNAION_TYPE_ASCII;
				char retStr[RECV_BUFF_SIZE];
				sprintf(retStr,returnStr200,"MODE");
				pPar->sendData(clientInfo->clientSock,retStr,strlen(retStr));
			}else
			if(strncmp("retr",recvBuf,4) == 0 || strncmp("RETR",recvBuf,4) == 0)
			{//下载文件
				pPar->sendData(clientInfo->clientSock,returnStr150,strlen(returnStr150));	//提示就绪
				char filename[RECV_BUFF_SIZE];
				int len = strlen(recvBuf);
				memcpy(filename,recvBuf + 5,len - 7);	//获取文件名
				filename[len - 7] = '\0';
				//打开指定文件并传输文件数据
				FILE *fp = NULL;
				char filePath[RECV_BUFF_SIZE];
				sprintf(filePath,"%s/%s",clientInfo->nowPath + 1,filename);
				if((fp = fopen(filePath,"rb")) == NULL)
				{//打开文件失败
					printf("Open file error!\n");
					return 1;	//这里需要注意
				}
				len = getFileLen(fp);
				for(int index = 0;index < len;index += RECV_BUFF_SIZE)
				{
					if(index + RECV_BUFF_SIZE >= len)
					{
						fread(filename,len - index,1,fp);
						send(clientInfo->clientDataSock,filename,len - index,0);
					}else
					{
						fread(filename,RECV_BUFF_SIZE,1,fp);
						send(clientInfo->clientDataSock,filename,RECV_BUFF_SIZE,0);
					}
				}
				fclose(fp);
				//数据传输完成返回
				closesocket(clientInfo->clientDataSock);
				pPar->sendData(clientInfo->clientSock,returnStr226,strlen(returnStr226));	//提示数据传输完成
			}else
			if(strncmp("stor",recvBuf,4) == 0 || strncmp("STOR",recvBuf,4) == 0)
			{//上传文件
				pPar->sendData(clientInfo->clientSock,returnStr150,strlen(returnStr150));	//提示就绪
				unsigned char filename[RECV_BUFF_SIZE];
				int len = strlen(recvBuf);
				memcpy(filename,recvBuf + 5,len - 7);	//获取文件名
				filename[len - 7] = '\0';
				//建立文件并开始接收数据
				FILE *fp = NULL;
				char filePath[RECV_BUFF_SIZE];
				sprintf(filePath,"%s/%s",clientInfo->nowPath + 1,filename);

				if((fp = fopen(filePath,"wb")) == NULL)
				{//打开文件失败
					printf("Open file error!\n");
					return 1;	//这里需要注意
				}
				int ret = 0;
				while(1)
				{
					ret = recv(clientInfo->clientDataSock,(char *)filename,RECV_BUFF_SIZE,0);
					if(ret <= 0) break;
					fwrite(filename,ret,1,fp);
					Sleep(1);
				}
				fclose(fp);
				//数据传输完成返回
				closesocket(clientInfo->clientDataSock);
				pPar->sendData(clientInfo->clientSock,returnStr226,strlen(returnStr226));	//提示数据传输完成
			}else
			//if(strncmp("size",recvBuf,4) == 0 || strncmp("SIZE",recvBuf,4) == 0)
			//{//获取文件大小
			//}else
			//if(strncmp("syst",recvBuf,4) == 0 || strncmp("SYST",recvBuf,4) == 0)
			//{//返回系统信息
			//}else
			//if(strncmp("feat",recvBuf,4) == 0 || strncmp("FEAT",recvBuf,4) == 0)
			//{//功能还不是很清楚
			//}else
			//if(strncmp("rest",recvBuf,4) == 0 || strncmp("REST",recvBuf,4) == 0)
			//{//功能还不是很清楚
			//}else
			//if(strncmp("pasv",recvBuf,4) == 0 || strncmp("PASV",recvBuf,4) == 0)
			//{//功能还不是很清楚
			//}else
			//if(strncmp("mkd",recvBuf,3) == 0 || strncmp("MKD",recvBuf,3) == 0)
			//{//功能还不是很清楚
			//}else
			//if(strncmp("rmd",recvBuf,3) == 0 || strncmp("RMD",recvBuf,3) == 0)
			//{//功能还不是很清楚
			//}else
			{//不支持的命令格式
				pPar->sendData(clientInfo->clientSock,returnStr202,strlen(returnStr202));
			}
			Sleep(1);
		}
	}else
	if(clientType == FTP_USER_TYPE_ANONY)
	{//匿名登录的用户的动作响应
		while(1)
		{//接受用户命令并对用户的命令作出回应(尚未完成)
			Sleep(1);
		}
	}
	closesocket(clientInfo->clientSock);
	XDELETE(clientInfo);
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}
_XBool _XFtpServer::createServer()	//建立服务器，开解accept线程，没接收一个连接，则开启一个客户端处理线程用于处理客户端请求
{
	if(m_isCreated) return XFalse;	//防止重复建立
	if(WSAStartup(MAKEWORD(2,2),&m_wsaData) != 0) printf("Socket init error\n");
	m_serverSock = socket(AF_INET,SOCK_STREAM,0);
	if(m_serverSock == -1)
	{//建立服务器套接字失败
		printf("Socket create error!\n");
		return XFalse;
	}
	int opt = SO_REUSEADDR;
	setsockopt(m_serverSock,SOL_SOCKET,SO_REUSEADDR,(const char *)&opt,sizeof(opt));
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(FTP_SOCKET_PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(m_serverSock,(sockaddr *)&server,sizeof(sockaddr)) == -1)
	{//绑定套接字失败
		printf("Bind fail!\n");
		return XFalse;
	}
	if(listen(m_serverSock,SOMAXCONN) == -1)
	{
		printf("Listen error!\n");
		return XFalse;
	}
	//下面开启服务器线程
#ifdef XEE_OS_LINUX
	if(pthread_create(&m_serverCMDThread, NULL, &serverCMDThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
	if(CreateThread(0,0,serverCMDThread,this,0,&m_serverCMDThread) == 0)
#endif
	{//线程建立失败
		printf("Server command thread create error!\n");
		return XFalse;
	}
	//下面开始accept线程
#ifdef XEE_OS_LINUX
	if(pthread_create(&m_serverAcceptThread, NULL, &serverAcceptThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
	if(CreateThread(0,0,serverAcceptThread,this,0,&m_serverAcceptThread) == 0)
#endif
	{//线程建立失败
		printf("Server accept thread create error!\n");
		return XFalse;
	}

	//初始化相关数据
	m_clientSum = 0;	//目前连接的客户端的数量为0
	m_allClientSum = 0;

	m_isCreated = XTrue;
	return XTrue;
}
_XFtpUserType _XFtpServer::clientLoginProc(int clientSock)//处理客户端登陆操作，返回用户的类型，0:无效,1:普通用户,2:特殊用户
{
//	if(!m_isCreated) return XFalse;	//尚未建立服务器则返回失败
	if(!sendData(clientSock,returnStr220,strlen(returnStr220))) return FTP_USER_TYPE_NULL;	//返回数据
	//后面是用户名的数据等操作
	char recvBuf[RECV_BUFF_SIZE];
	while(1)
	{//等待输入用户名
		if(recvData(clientSock,recvBuf))
		{//如果接收到数据
			if(strncmp("USER",recvBuf,4) == 0 || strncmp("user",recvBuf,4) == 0)
			{//输入的是用户名
				break;
			}else
			{//输入的数据非法
				sendData(clientSock,returnStr202,strlen(returnStr202));
			}
		}
		Sleep(1);
	}
	//检查用户类型
	int len = strlen(recvBuf);
	if(len < 7) return FTP_USER_TYPE_NULL;	//数据非法
	int index = 0;
	for(index = 5;index < len;++ index)
	{
		recvBuf[index - 5] = recvBuf[index];
	}
	recvBuf[index - 7] = '\0';
	_XFtpUserType userType = FTP_USER_TYPE_NULL;
	if(strncmp(recvBuf,defaultUserName,strlen(defaultUserName)) == 0) userType = FTP_USER_TYPE_DEFAULT;else
	if(strncmp(recvBuf,anonyUserName,strlen(anonyUserName)) == 0) userType = FTP_USER_TYPE_ANONY;
	else return FTP_USER_TYPE_NULL;	//用户名不存在
	//提示用户输入密码
	sendData(clientSock,returnStr331,strlen(returnStr331));
	//接收并校验密码
	recvData(clientSock,recvBuf);
	len = strlen(recvBuf);
	if(len < 7) return FTP_USER_TYPE_NULL;	//数据非法
	for(index = 5;index < len;++ index)
	{
		recvBuf[index - 5] = recvBuf[index];
	}
	recvBuf[index- 7] = '\0';
	if(userType == FTP_USER_TYPE_DEFAULT && strncmp(recvBuf,defaultUserPassword,strlen(defaultUserPassword)) == 0)
	{//密码正确
		sendData(clientSock,returnStr230,strlen(returnStr230));	//返回登录成功
		++ m_clientSum;
		++ m_allClientSum;
		return userType;
	}else
	if(userType == FTP_USER_TYPE_ANONY && strncmp(recvBuf,anonyUserPassword,strlen(anonyUserPassword)) == 0)
	{//密码正确
		sendData(clientSock,returnStr531,strlen(returnStr531));	//返回匿名用户登录
		++ m_clientSum;
		++ m_allClientSum;
		return userType;
	}else
	{//密码错误
		sendData(clientSock,returnStr530,strlen(returnStr530));	//返回登录失败
		return FTP_USER_TYPE_NULL;
	}

//	return FTP_USER_TYPE_NULL;
}