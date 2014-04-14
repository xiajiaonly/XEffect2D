#ifndef _JIA_XFTPCOMMON_
#define _JIA_XFTPCOMMON_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.12
//--------------------------------
#include <iostream>
#include <String.h>
#include <fstream>
#include <string>
#include "XBasicFun.h"
#include "XBasicClass.h"
//#include <winsock2.h>

#define RECV_BUFF_SIZE (4096)
#define FTP_SOCKET_PORT (21)
#define SERVER_COMMAND_LEN (64)	//服务器端最多能够接受的命令长度
//用户类型
enum _XFtpUserType
{
	FTP_USER_TYPE_NULL,		//无定义
	FTP_USER_TYPE_DEFAULT,	//默认用户
	FTP_USER_TYPE_ANONY,	//匿名用户
};

//下面是返回客户端的一些信息
extern char returnStr125[];
extern char returnStr150[];
extern char returnStr200[];
extern char returnStr202[];
extern char returnStr211[];
extern char returnStr213[];
extern char returnStr215[];
extern char returnStr220[];
extern char returnStr221[];
extern char returnStr226[];
extern char returnStr230[];
extern char returnStr250[];
extern char returnStr257[];
extern char returnStr331[];
extern char returnStr350[];
extern char returnStr530[];
extern char returnStr531[];

extern char defaultUserName[];		//默认的用户名
extern char defaultUserPassword[];	//默认的密码
extern char anonyUserName[];		//匿名用户名
extern char anonyUserPassword[];	//匿名的密码

extern _XBool analyseFTPString(const char * str,char * IP,char * path);	//从ftp字符串中解析出相关数据 例子：FTP://192.168.1.110/folder

#endif