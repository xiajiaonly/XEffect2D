#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFtpCommon.h"
namespace XE{
//下面是返回客户端的一些信息
char returnStr125[]="125 Data connection already open;Transfer starting.\r\n";			//*
char returnStr150[]="150 File status okay; about to open data connection.\r\n";
char returnStr200[]="200 %s command successful.\r\n";									//*
char returnStr202[]="202 Command not implemented, superfluous at this site.\r\n";
char returnStr211[]="211 System status, or system help reply.\r\n";
char returnStr213[]="213 File status.\r\n";
char returnStr215[]="215 Windows_NT.\r\n";												//*
char returnStr220[]="220 xiajia's server \r\n";											//*
char returnStr221[]="221 Goodbye.\r\n";													//*
char returnStr226[]="226 Transfer complete.\r\n";										//*
char returnStr230[]="230 User logged in.\r\n";											//*
char returnStr250[]="250 \"%s\" path is set.\r\n";										//*
char returnStr257[]="257 \"%s\" is current location.\r\n";								//*
char returnStr331[]="331 Password required for this user.\r\n";							//*
char returnStr350[]="350 Restarting at x.\r\n";											//*
char returnStr530[]="530 Not logged in.\r\n";
char returnStr531[]="531 Not root client. Anonymous client.\r\n";

char defaultUserName[] = "booss";		
char defaultUserPassword[] = "xiajia90";
char anonyUserName[] = "anonymous";		
char anonyUserPassword[] = "anonymous";	
//从ftp字符串中解析出相关数据 例子：FTP://192.168.1.110:21/folder
XBool analyseFTPString(const char * str,char * IP,char * path,int &port)	
{
	//方案1：字符串分析
	if(str == NULL) return XFalse;
	int len = strlen(str);
	if(len < 13) return XFalse;	//最小长度要求FTP://0.0.0.0
	if(str[0] != 'F' && str[0] != 'f' ) return XFalse;
	if(str[1] != 'T' && str[1] != 't' ) return XFalse;
	if(str[2] != 'P' && str[2] != 'p' ) return XFalse;
	int pos = XString::getCharPosition(str + 6,'/');
	int posx = XString::getCharPosition(str + 6,':');
	if(pos > 0)
	{
		if(posx < pos && posx > 0)
		{//包含端口号
			memcpy(IP,str + 6,posx);
			IP[posx] = '\0';
			if(pos - posx - 1 > 0)
			{
				char tmp[1024];
				memcpy(tmp,str + 6 + posx + 1,pos - posx - 1);
				tmp[pos - posx - 1] = '\0';
				port = atoi(tmp);
			}else
			{
				port = 21;
			}
		}else
		{
			memcpy(IP,str + 6,pos);
			IP[pos] = '\0';
			port = 21;	//使用默认端口号
		}
		//解析路径
		if(len == pos + 6) path[0] = '\0';
		else strcpy(path,str + 6 + pos + 1);
	}else
	{
		strcpy(IP,str + 6);
		path[0] = '\0';
		port = 21;
	}
	return XTrue;
}
//ftp完整路径如下：ftp://username:password@192.168.1.1:21/folder
XBool analyseFTPStrEx(const char * str,std::string &protocol,	//协议：http、ftp等，输出为大写
							  std::string &name,		//用户名
							  std::string &password,	//密码
							  std::string &IP,			//IP地址
							  int &port,				//端口号
							  std::string &path)		//路径
{
	//经过简单的初步测试
//	std::string proto;
//	std::string userName;
//	std::string password;
//	std::string IP;
//	int port;
//	std::string path;
////	if(analyseFTPStrEx("ftp://username:password@192.168.1.1:21/folder",proto,userName,password,IP,port,path))
////	if(analyseFTPStrEx("ftp://192.168.1.1:21/folder",proto,userName,password,IP,port,path))
////	if(analyseFTPStrEx("ftp://192.168.1.1/folder",proto,userName,password,IP,port,path))
////	if(analyseFTPStrEx("ftp://192.168.1.1/",proto,userName,password,IP,port,path))
//	if(analyseFTPStrEx("ftp://192.168.1.1",proto,userName,password,IP,port,path))
//		printf("OK");
//	else printf("Fail");

	if(str == NULL) return XFalse;
	char tProto[128];
	char tUserName[128];
	char tPassword[128];
	char tIp[128];
	int tPort = 0;
	char tPath[128];
	int offset = XString::getCharPosition(str,'@');
	if(offset >= 0)
	{//路径中包含用户名和密码
		if(XString::getCharPosition(str + offset,':') >= 0)
		{//路径中包含端口号
			if(sscanf(str,"%99[^:]://%99[^:]:%99[^@]@%99[^:]:%99d/%99[^\n]",tProto,tUserName,tPassword,tIp,&tPort,tPath) == 6)
			{
				_strupr(tProto);
				protocol = tProto;
				name = tUserName;
				password = tPassword;
				IP = tIp;
				port = tPort;
				path = tPath;
				return XTrue;
			}else
			{
				return XFalse;
			}
		}else
		{//路径中不包含端口号
			if(sscanf(str,"%99[^:]://%99[^:]:%99[^@]@%99[^/]/%99[^\n]",tProto,tUserName,tPassword,tIp,tPath) == 5)
			{
				_strupr(tProto);
				protocol = tProto;
				name = tUserName;
				password = tPassword;
				IP = tIp;
				port = 0;
				path = tPath;
				return XTrue;
			}else
			{
				return XFalse;
			}
		}
	}else
	{//路径中不包含用户名和密码
		offset = XString::getCharPosition(str,':');
		if(offset < 0) return XFalse;
		if(XString::getCharPosition(str + offset + 1,':') >= 0)
		{//路径中包含端口号
			if(sscanf(str,"%99[^:]://%99[^:]:%99d/%99[^\n]",tProto,tIp,&tPort,tPath) == 4)
			{
				_strupr(tProto);
				protocol = tProto;
				name = "";
				password = "";
				IP = tIp;
				port = tPort;
				path = tPath;
				return XTrue;
			}else
			{
				return XFalse;
			}
		}else
		{//路径中不包含端口号
			if(sscanf(str, "%99[^:]://%99[^/]/%99[^\n]",tProto,tIp,tPath) == 3)
			{
				_strupr(tProto);
				protocol = tProto;
				name = "";
				password = "";
				IP = tIp;
				port = 0;
				path = tPath;
				return XTrue;
			}else
			if(sscanf(str, "%99[^:]://%99[^/]",tProto,tIp) == 2)
			{
				_strupr(tProto);
				protocol = tProto;
				name = "";
				password = "";
				IP = tIp;
				port = 0;
				path = "";
				return XTrue;
			}else
			{
				return XFalse;
			}
		}
	}
//	return XFalse;
}
}