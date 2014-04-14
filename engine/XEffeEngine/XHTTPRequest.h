#ifndef _JIA_XHTTPREQUEST_
#define _JIA_XHTTPREQUEST_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//#include <iostream>
//#include "AtlBase.h"

#include "XOSDefine.h"
#ifdef XEE_OS_WINDOWS
#include "winsock.h"
#pragma comment(lib, "ws2_32.lib")
#endif

#ifdef XEE_OS_LINUX
#include <sys/socket.h>
#include <sys/types.h>
#include <malloc.h>
#include <arpa/inet.h>        //inet_addr()
#include <unistd.h>           //usleep()
#include <netdb.h>
#endif

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "XBasicClass.h"

#define MEM_BUFFER_SIZE 10

//HTTP请求的结构体
struct _XHTTPRequestStruct
{
	char *headerSend;			//发送的包头的指针
	char *headerReceive;		//接受的包头的指针
	char *message;				//接受的HTTP包体的指针
	long messageLength;			//包体的长度
	_XHTTPRequestStruct()
		:headerSend(NULL)
		,headerReceive(NULL)
		,message(NULL)
		,messageLength(0)
	{
	}
};

//缓存结构体
struct _XMemBuffer
{
	unsigned char *buffer;		//缓存的总指针
	unsigned char *position;	//缓存的当前位置指针
	size_t size;				//缓存的大小
	_XMemBuffer()
		:buffer(NULL)
		,position(NULL)
		,size(0)
	{
	}
};

class _XHTTPRequest
{
public:
	_XHTTPRequest();			//构造函数
	virtual ~_XHTTPRequest();	//析构函数
private:
	void memBufferCreate(_XMemBuffer *b);											//建立缓存，并分配MEM_BUFFER_SIZE的存储空间
	void memBufferGrow(_XMemBuffer *b);												//倍增缓存的存储空间
	void memBufferAddByte(_XMemBuffer *b, unsigned char byt);						//将一个字节的数据放入缓存
	void memBufferAddBuffer(_XMemBuffer *b, unsigned char *buffer, size_t size);	//将一定字节的数据放入缓存
	unsigned long getHostAddress(const char *host);												//根据域名获得IP
	int sendString(int sock,char *str);											//向指定的sock发送字符串
	int validHostChar(char ch);													//检查字符串是否符合主机名规则
	void parseURL(char *url,char * protocol, char * host,char * request,int *port);								//解析URL字符串
	int sendHTTP(char *url,char * headerReceive,unsigned char *post, int postLength,_XHTTPRequestStruct *req);			//发送HTTP请求
public:
	int sendRequest(_XBool IsPost,char *url,_XHTTPRequestStruct &req);				//对外调用的发送HTTP请求的函数
};
//向指定的网络发送一个字符串，注意这里是字符串，必须要以及字符结束符为结束标记
inline int _XHTTPRequest::sendString(int sock,char *str)
{
	return send(sock,str,strlen(str),0);
}

//检查字符串是否符合主机名的特点：i.e. A-Z or 0-9 or -.:
inline int _XHTTPRequest::validHostChar(char ch)
{
	//return (isalpha(ch) || isdigit(ch) || ch == '-' || ch == '.' || ch == ':');
	return (((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')) || ch == '-' || ch == '.' || ch == ':');
}

#endif
