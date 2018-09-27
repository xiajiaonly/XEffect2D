#ifndef _JIA_XNETCOMMON_
#define _JIA_XNETCOMMON_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.8
//--------------------------------
#include <deque>
#include <iostream>
#include <String.h>
#include <fstream>
#include <string>
#include "..\XMemory.h"
namespace XE{
#define MAX_SEND_DATA_BUFF (3200)	//发送队列的大小，防止队列拥堵造成内存溢出
#define MAX_RECV_DATA_BUFF (6400)	//接收队列的大小，防止队列拥堵造成内存溢出

//#define NET_SERVER_IP "192.168.1.110"
#define NET_SERVER_PORT (9988)
#define BOARDCAST_PORT (7898)	//广播端口
#define BOARDCAST_DATA_LEN (1024)
#define PROJECT_STRING_LEN (256)	//工程字符串的长度
#define DEFAULT_PROJECT_STR "xiajia_1981@163.com"	//默认的工程字符串
//#define WITH_LOCAL_BOARDCAST_IP
#define BOARDCASR_IP "192.168.1.255"
//网络连接状态
enum XConnectState
{
	CONNECT_STATE_DISCONNECT,	//连接断开
	CONNECT_STATE_CONNECT,		//连接正常
};
//网络数据的类型
enum XNetDataType
{
	DATA_TYPE_EPC,			//用户的epc信息
	DATA_TYPE_EXCESS_SUM,	//剩余的位置数量
	DATA_TYPE_VIDEO,		//视频数据
	DATA_TYPE_VIDEO_UPDATE,	//有新的视频更新
	DATA_TYPE_GAME_OVER,	//游戏结束
	DATA_TYPE_GAME_START,	//游戏开始
	DATA_TYPE_PHOTO,		//图像数据:[是否有epc][epc][图像数据]
	DATA_TYPE_KEY,			//按键状态

	DATA_TYPE_CONFIG_INFO,		//配置项的总体信息
	DATA_TYPE_CONFIG_ITEM,		//单个配置项的值
	DATA_TYPE_CONFIG_ITEMS,		//所有配置项的值
	DATA_TYPE_CONFIG_INJECT,	//数据注入
	DATA_TYPE_CONFIG_OPERATE,	//对配置界面的操作

	DATA_TYPE_CUSTOM,			//自定义的数据类型 
	DATA_TYPE_CUSTOM_01,			//自定义的数据类型 
	DATA_TYPE_CUSTOM_02,			//自定义的数据类型 
	DATA_TYPE_CUSTOM_03,			//自定义的数据类型 
	DATA_TYPE_CUSTOM_04,			//自定义的数据类型 
	DATA_TYPE_CUSTOM_05,			//自定义的数据类型 
};
//说明网络传输的数据协议为
//CMD[1byte]Len[4byte][data]
#define PACKET_HEAD_LEN (5)		//包头大小
struct XNetData	//网络数据
{
	XBool isEnable;
	XNetDataType type;
	int dataLen;
	unsigned char * data;
	XNetData()
		:isEnable(XFalse)
		,dataLen(0)
		,data(NULL)
	{}
	~XNetData()
	{
		if(!isEnable) return;
		XMem::XDELETE_ARRAY(data);
		isEnable = XFalse;
	}
};
//为简单通讯而封装的数据包形结构
struct XNetPack
{
	int clientID;		//客户端的编号
	int buffSize;		//内存空间的大小	
	int curDataLen;		//有效数据的大小
	unsigned char *data;
	XNetPack()
		:data(NULL)
		, buffSize(0)
		, curDataLen(0)
		, clientID(0)
	{}
};
inline void showNetData(XNetData *)//data)
{
	//if(data == NULL) return;
	//printf("%d|%d:",data->type,data->dataLen);
	//for(int i = 0;i < data->dataLen;++ i)
	//{
	//	printf("%02x",data->data[i]);
	//}
	//printf("\n");
}
//重置指定socket
inline void resetSocketAddr(SOCKET socket)
{
	int opt = 1;
    int len = sizeof(opt);
    setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,len);
}
//分配一个socket号
inline bool getANewSocket(SOCKET &mySock,int protocol = SOCK_STREAM)
{
	if(mySock != INVALID_SOCKET)
	{//如果没有分配套接字，这里进入，防止重复分配
		closesocket(mySock);
		mySock = INVALID_SOCKET;
	}
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return false;
	mySock = socket(AF_INET, protocol, 0); //分配套接字
	if (mySock == INVALID_SOCKET) return false;
	resetSocketAddr(mySock);
	return true;
}
//获取当前计算机的IP地址
inline std::string getMyIP()
{
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0) return XString::gNullStr;
	char hname[128] = "";
	gethostname(hname,sizeof(hname));
	hostent *hent = gethostbyname(hname);
	if(hent != NULL && hent->h_addr_list[0])
	{
		sprintf_s(hname,128,"%d.%d.%d.%d",(unsigned char)(hent->h_addr_list[0][0]),
			(unsigned char)(hent->h_addr_list[0][1]),
			(unsigned char)(hent->h_addr_list[0][2]),
			(unsigned char)(hent->h_addr_list[0][3]));
	}
	WSACleanup();
	return hname;
}
}
#endif