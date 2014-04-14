#ifndef _JIA_XFTPCLIENT_
#define _JIA_XFTPCLIENT_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.12
//--------------------------------
//这是一个封装了用于FTP通讯的客户端的类

#include "XFtpCommon.h"

class _XFtpClient
{
private:
	_XBool m_isConnect;		//是否已经连接到服务器
	WSADATA m_wsaData;	
	SOCKET m_cmdsocket;		//连接服务器的套接字
	int m_retCode;			//最近一次命令返回的数据
	char m_commandBuff[RECV_BUFF_SIZE + 1];	//命令交互的缓存

	_XBool recvRetCode()
	{
		int ret = recv(m_cmdsocket,m_commandBuff,RECV_BUFF_SIZE,0);
		if(ret == SOCKET_ERROR || ret == 0) 
		{
			printf("Recv error!\n");
			return XFalse;
		}
		m_commandBuff[ret] = '\0';
		printf("%s",m_commandBuff);
		if(!getRetCode(m_commandBuff,ret)) return XFalse;
		return XTrue;
	}

	_XBool getRetCode(const char *buf,int len)
	{
		if(len > 4 && buf[0] >= '0' && buf[0] <= '9'
			&& buf[1] >= '0' && buf[1] <= '9'
			&& buf[2] >= '0' && buf[2] <= '9'
			&& buf[3] == ' ')
		{
			m_retCode = (buf[0] - '0') * 100 + (buf[1] - '0') * 10 + (buf[2] - '0');
			return XTrue;
		}
		return XFalse;
	}
	_XBool sendCommand(const char * cmd)		//向服务器发送命令
	{
		send(m_cmdsocket,cmd,strlen(cmd),0);
		return recvRetCode();
	}
public:
	_XBool connectServer(const char * serverIP,int port = FTP_SOCKET_PORT);	//与服务器建立连接
	_XBool sendClose()
	{
		if(!m_isConnect) return XFalse;
		if(!sendCommand("QUIT\r\n")) return XFalse;
		closesocket(m_cmdsocket);
		m_isConnect = XFalse;
		if(m_retCode != 221) return XFalse;
		return XTrue;
	}
	//下面是对ftp封装的一些方法
	_XBool sendUserName(const char * userName)
	{
		if(userName == NULL) return XFalse;
		if(!m_isConnect) return XFalse;
		strcpy(m_commandBuff,"USER ");
		strcat(m_commandBuff,userName);
		strcat(m_commandBuff,"\r\n");
		if(!sendCommand(m_commandBuff)) return XFalse;
		if(m_retCode != 331) return XFalse;	//需要密码
		return XTrue;
	}
	_XBool sendPassword(const char * password)
	{
		if(password == NULL) return XFalse;
		if(!m_isConnect) return XFalse;
		strcpy(m_commandBuff,"PASS ");
		strcat(m_commandBuff,password);
		strcat(m_commandBuff,"\r\n");
		if(!sendCommand(m_commandBuff)) return XFalse;
		if(m_retCode != 230) return XFalse;	//登录成功
		return XTrue;
	}
	_XBool setPort(SOCKET *listenSock);	//设置通讯端口
	_XBool sendFileList();	//获取文件列表
	_XBool sendDeleteFile(const char *filename)	//发送删除文件的命令
	{
		if(!m_isConnect) return XFalse;
		if(filename == NULL) return XFalse;
		strcpy(m_commandBuff,"DELE ");
		strcat(m_commandBuff,filename);
		strcat(m_commandBuff,"\r\n");	
		if(!sendCommand(m_commandBuff)) return XFalse;
		if(m_retCode != 250) return XFalse;	//delete成功
		return XTrue;
	}
	_XBool sendDownloadFile(const char *filename);	//下载指定文件
	_XBool sendUpdateFile(const char *filename);	//上传文件
	_XBool sendUpdateFile(const char *inFilename,const char *outFilename);	//上传文件
	_XBool sendChangeDirectory(const char *dir)	//换一个目录
	{
		if(!m_isConnect) return XFalse;
		if(dir == NULL) return XFalse;
		strcpy(m_commandBuff,"CWD ");
		strcat(m_commandBuff,dir);
		strcat(m_commandBuff,"\r\n");	
		if(!sendCommand(m_commandBuff)) return XFalse;
		if(m_retCode != 250) return XFalse;	//CWD成功
		if(!sendCommand("PWD\r\n")) return XFalse;
		if(m_retCode != 257) return XFalse;	//CWD成功
		return XTrue;
	}
	int sendGetFileSize(const char *filename)
	{
		if(!m_isConnect) return -1;
		if(filename == NULL) return -1;
		strcpy(m_commandBuff,"SIZE ");
		strcat(m_commandBuff,filename);
		strcat(m_commandBuff,"\r\n");	
		if(!sendCommand(m_commandBuff)) return -1;
		if(m_retCode != 213) return -1;
		int size = -1;
		if(sscanf(m_commandBuff,"213 %d",&size) != 1) return -1;
		return size;
	}

	_XFtpClient()
		:m_isConnect(XFalse)
	{
	}
};

#endif