//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFtpCommon.h"

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

_XBool analyseFTPString(const char * str,char * IP,char * path)	//从ftp字符串中解析出相关数据 例子：FTP://192.168.1.110/folder
{
	if(str == NULL) return XFalse;
	int len = strlen(str);
	if(len < 13) return XFalse;	//最小长度要求FTP://0.0.0.0
	if(str[0] != 'F' && str[0] != 'f' ) return XFalse;
	if(str[1] != 'T' && str[1] != 't' ) return XFalse;
	if(str[2] != 'P' && str[2] != 'p' ) return XFalse;
	int pos = getCharPosition(str + 6,'/');
	if(pos > 0)
	{
		memcpy(IP,str + 6,pos);
		IP[pos] = '\0';
		//解析路径
		if(len == pos + 6) path[0] = '\0';
		else strcpy(path,str + 6 + pos + 1);
	}else
	{
		strcpy(IP,str + 6);
		path[0] = '\0';
	}
	return XTrue;
}