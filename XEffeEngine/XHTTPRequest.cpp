//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XHTTPRequest.h"

_XHTTPRequest::_XHTTPRequest()
{
}

_XHTTPRequest::~_XHTTPRequest()
{
}

//初始化缓存并分配初始化的MEM_BUFFER_SIZE大小的内存空间
void _XHTTPRequest::MemBufferCreate(_XMemBuffer *b)
{
	b->size = MEM_BUFFER_SIZE;
	b->buffer =(unsigned char *) malloc( b->size );
	b->position = b->buffer;
}

//翻倍的扩大缓存的内存空间
void _XHTTPRequest::MemBufferGrow(_XMemBuffer *b)
{
	size_t sz;
	sz = b->position - b->buffer;
	b->size = b->size << 1;
	b->buffer =(unsigned char *) realloc(b->buffer,b->size);
	b->position = b->buffer + sz; // readjust current position
}

//向缓存空间在中写入一个字节的数据
void _XHTTPRequest::MemBufferAddByte(_XMemBuffer *b,unsigned char byt)
{
	if((size_t)(b->position-b->buffer) >= b->size)
	{
		MemBufferGrow(b);
	}

	*(b->position++) = byt;
}

//将一段数据写入到缓存空间中
void _XHTTPRequest::MemBufferAddBuffer(_XMemBuffer *b,unsigned char *buffer, size_t size)
{
	while(((size_t)(b->position-b->buffer)+size) >= b->size)
	{//不断的倍增空间知道空间足够为止
		MemBufferGrow(b);
	}
	memcpy(b->position,buffer,size);
	b->position+=size;
}

//使用DNS将域名装换成IP地址
unsigned long _XHTTPRequest::GetHostAddress(char *host)
{
	struct hostent *phe;
	char *p;
	phe = gethostbyname( host );

	if(phe == NULL)
	{
		return 0;
	}

	p = *phe->h_addr_list;
	return *((unsigned long*)p);
}

#ifdef OS_LINUX
void strupr(char *p)
{
	if(p == NULL) return;
	int tempLen = strlen(p);
	if(tempLen <= 0) return;
	for(int i = 0;i < tempLen;++ i)
	{
		if(p[i] >= 'a' && p[i] <= 'z')
		{
			p[i] += 'A' - 'a';
		}
	}
}
#endif

//解析URL，从中解析出协议，端口，主机地址，请求
void _XHTTPRequest::ParseURL(char *url,char *protocol,char *host,char *request,int *port)
{//使用多个指针对同一个字符串进行解析，这个思路很不错。
	char *work;	//分析字符串时用于临时暂存字符串，下同
	char *ptr;	
	char *ptr2;

	*protocol = NULL;
	*host = NULL;
	*request = NULL;
	*port = 80;

#ifdef OS_WINDOWS
	work = _strdup(url);		//复制URL字符串，作为备用
	_strupr(work);					//将字符串转换为大写
#endif
#ifdef OS_LINUX
	work = strdup(url);		//复制URL字符串，作为备用
	strupr(work);
#endif
	ptr = strchr(work,':');			//寻找到":"，以便于找到协议
	if(ptr != NULL)
	{//如果找到协议类型，则在这里复制
		*(ptr++) = 0;
		strcpy(protocol,work);
	}else
	{//如果没有找协议类型，则默认为HTTP协议
		strcpy(protocol,"HTTP");
		ptr = work;
	}
	if((*ptr == '/') && (*(ptr+1) == '/'))
	{//如果协议之后又反斜杠，则跳过这个反斜杠
		ptr += 2;
	}
	//下面开始寻找主机
	ptr2 = ptr;
	while(ValidHostChar(*ptr2) && *ptr2)
	{
		++ ptr2;
	}
	*ptr2 = 0;
	strcpy(host,ptr);
	//主机之后的都为请求字符串
	strcpy(request,url + (ptr2-work));
	//寻找端口号
	ptr = strchr(host,':');
	if(ptr!=NULL)
	{
		*ptr=0;
		*port = atoi(ptr+1);
	}//如果没有端口号则默认为80端口
	free(work);
}

//发送HTTP信息
//URL：顾名思义拉URL字符串
//headerReceive：发给服务器的包头
//post：发送的数据的指针
//postLength：发送字符串的长度
//req：回应的数据
//返回值：0、失败 1、成功(好怪异的返回值设置)
int _XHTTPRequest::SendHTTP(char *url,char * headerReceive,unsigned char *post,int postLength,_XHTTPRequestStruct *req)
{
	//SOCKADDR_IN sin;			//用于连接web服务器
	sockaddr_in sin;			//用于连接web服务器
	int sock;					//连接服务器的套接字
	char buffer[512];		
	char protocol[20];			//用于暂存协议
	char host[256];				//用于暂存主机名
	char request[1024];			//用于暂存请求
	int l,port,chars;//,err;
	_XMemBuffer headersBuffer;	//包头缓存
	_XMemBuffer messageBuffer;	//包体缓存
	char headerSend[1024];		//发送包头
	int done;
	
	//解析URL
	ParseURL(url,protocol,host,request,&port);
	if(strcmp(protocol,"HTTP"))
	{//目前这个模块只支持HTTP协议
		return 0;
	}

#ifdef OS_WINDOWS
	if(initSocket() == 0) return 0;
#endif

	//申请套结字
	sock = socket(AF_INET,SOCK_STREAM,0);
	//if(sock == INVALID_SOCKET)
	if(sock < 0)
	{//申请套结字失败
		return 0;
	}

	//连接到web服务器
	sin.sin_family = AF_INET;
	sin.sin_port = htons((unsigned short)port);
	sin.sin_addr.s_addr = GetHostAddress(host);
	if(connect(sock,(sockaddr*)&sin, sizeof(sockaddr_in)))
	{//如果连接失败则返回
		return 0;
	}

	if(!*request)
	{//如果没有请求，则请求为反斜杠
		strcpy(request,"/");
	}

	//发送HTTP包头信息
	if(post == NULL)
	{//如果不是发送，则作为接收处理
		SendString(sock,"GET ");
		strcpy(headerSend, "GET ");
	}else
	{
		SendString(sock,"POST ");
		strcpy(headerSend, "POST ");
	}
	SendString(sock,request);
	strcat(headerSend, request);

	SendString(sock," HTTP/1.0\r\n");
	strcat(headerSend, " HTTP/1.0\r\n");

	SendString(sock,"Accept: image/gif, image/x-xbitmap,"
		" image/jpeg, image/pjpeg, application/vnd.ms-excel,"
		" application/msword, application/vnd.ms-powerpoint,"
		" */*\r\n");
	strcat(headerSend, "Accept: image/gif, image/x-xbitmap,"
		" image/jpeg, image/pjpeg, application/vnd.ms-excel,"
		" application/msword, application/vnd.ms-powerpoint,"
		" */*\r\n");
	SendString(sock,"Accept-Language: en-us\r\n");
	strcat(headerSend, "Accept-Language: en-us\r\n");

	SendString(sock,"Accept-Encoding: gzip, default\r\n");
	strcat(headerSend, "Accept-Encoding: gzip, default\r\n");
	SendString(sock,"User-Agent: Neeao/4.0\r\n");
	strcat(headerSend, "User-Agent: Neeao/4.0\r\n");

	if(postLength)
	{
		sprintf(buffer,"Content-Length: %ld\r\n",postLength);
		SendString(sock,buffer);
		strcat(headerSend, buffer);
	}
	//SendString(sock,"Cookie: mycookie=blablabla\r\n");
	// printf("Cookie: mycookie=blablabla\r\n");
	SendString(sock,"Host: ");
	strcat(headerSend, "Host: ");

	SendString(sock,host);
	strcat(headerSend, host);

	SendString(sock,"\r\n");
	strcat(headerSend, "\r\n");

	if((headerReceive != NULL) && *headerReceive)
	{
		SendString(sock,headerReceive);
		strcat(headerSend, headerReceive);
	}

	SendString(sock,"\r\n"); // Send a blank line to signal end of HTTP headerReceive
	strcat(headerSend, "\r\n");

	if((post != NULL) && postLength)
	{//发送需要发送的内容
		send(sock,(const char*)post,postLength,0);
		post[postLength] = '\0';

		strcat(headerSend, (const char*)post);
	}

	//strcpy(req->headerSend, headerSend);
	req->headerSend = (char*) malloc( sizeof(char*) * strlen(headerSend));
	strcpy(req->headerSend, (char*) headerSend );

	//下面开始解收包头
	MemBufferCreate(&headersBuffer );
	chars = 0;
	done = 0;

	while(!done)
	{
		l = recv(sock,buffer,1,0);
		if(l < 0)
		{//接受完成
			done = 1;
		}

		switch(*buffer)
		{
		case '\r':
			break;
		case '\n':
			if(chars==0)
			{//接受完成
				done = 1;
			}
			chars=0;
			break;
		default:
			chars++;
			break;
		}

		MemBufferAddByte(&headersBuffer,*buffer);
	}
	//将接受到的包头保存起来
	req->headerReceive = (char*) headersBuffer.buffer;
	*(headersBuffer.position) = 0;

	//下面接受包体
	MemBufferCreate(&messageBuffer); // Now read the HTTP body
	do
	{
		l = recv(sock,buffer,sizeof(buffer)-1,0);
		if(l<0)
		{//包体接受完成
			break;
		}
		*(buffer + l)=0;
		MemBufferAddBuffer(&messageBuffer, (unsigned char*)&buffer, l);
	}while(l > 0);

	*messageBuffer.position = 0;
	req->message = (char*) messageBuffer.buffer;
	req->messageLength = (messageBuffer.position - messageBuffer.buffer);

	//交互完毕之后关闭socket
#ifdef OS_WINDOWS
	closesocket(sock);
#endif
#ifdef OS_LINUX
	close(sock);
#endif

	return 1;
}

//发送HTTP请求
//IsPost：			是否为发送
//url：				URL字符串
//req：				HTTP请求的结构体
//返回值：			1、成功 0、失
int _XHTTPRequest::SendRequest(bool IsPost, char *url,_XHTTPRequestStruct &req)
{
	int i,rtn;
	char* buffer;

	if(req.headerReceive != NULL) free(req.headerSend);
	if(req.message != NULL) free(req.message);
	req.messageLength = 0;

	if(IsPost)
	{//发送 
		i = strlen(req.headerSend);
		buffer = (char*) malloc(i+1);
		strcpy(buffer, req.headerSend);
		//释放内存空间
		free(req.headerSend);

		rtn = SendHTTP(url,"Content-Type: application/x-www-form-urlencoded\r\n",
			(unsigned char*)buffer,i,&req);

		free(buffer);
	}else
	{//接收
		rtn = SendHTTP(url,NULL,NULL,0,&req);
	}

	if(rtn)
	{//将数据输出
		return 1;
	}else
	{
		return 0;
	}
}
