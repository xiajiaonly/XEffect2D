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
#include "XBasicFun.h"
#include "XBasicClass.h"

#define NET_SERVER_IP ("192.168.1.110")
#define NET_SERVER_PORT (9988)
#define BOARDCAST_PORT (7898)	//广播端口
#define BOARDCAST_DATA_LEN (1024)
#define PROJECT_STRING_LEN (256)	//工程字符串的长度
#define DEFAULT_PROJECT_STR ("xiajia_1981@163.com")	//默认的工程字符串
//网络连接状态
enum _XConnectState
{
	CONNECT_STATE_DISCONNECT,	//连接断开
	CONNECT_STATE_CONNECT,		//连接正常
};
//网络数据的类型
enum _XNetDataType
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
};
//说明网络传输的数据协议为
//CMD[1byte]Len[4byte][data]
#define PACKET_HEAD_LEN (5)		//包头大小
struct _XNetData	//网络数据
{
	_XBool isEnable;
	_XNetDataType type;
	int dataLen;
	unsigned char * data;
	_XNetData()
		:isEnable(XFalse)
		,dataLen(0)
		,data(NULL)
	{}
	~_XNetData()
	{
		if(!isEnable) return;
		XDELETE_ARRAY(data);
		isEnable = XFalse;
	}
};
inline void showNetData(_XNetData *)//data)
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
inline void resetSocketAddr(int socket)
{
	int opt = 1;
    int len = sizeof(opt);
    setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,len);
}
//分配一个socket号
inline bool getANewSocket(SOCKET &mySock)
{
	if(mySock != INVALID_SOCKET)
	{//如果没有分配套接字，这里进入，防止重复分配
		closesocket(mySock);
		mySock = INVALID_SOCKET;
	}
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0) return false;
	mySock = socket(AF_INET,SOCK_STREAM,0); //分配套接字
	if(mySock == INVALID_SOCKET) return false;
	resetSocketAddr(mySock);
	return true;
}
//获取当前计算机的IP地址
inline string getMyIP()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	char hname[128] = "";
	gethostname(hname,sizeof(hname));
	hostent *hent = gethostbyname(hname);
	if(hent != NULL && hent->h_addr_list[0])
	{
		sprintf(hname,"%d.%d.%d.%d",(unsigned char)(hent->h_addr_list[0][0]),
			(unsigned char)(hent->h_addr_list[0][1]),
			(unsigned char)(hent->h_addr_list[0][2]),
			(unsigned char)(hent->h_addr_list[0][3]));
	}
	WSACleanup();
	return hname;
}
//#include <IPHlpApi.h>
////参数pMacAddr应该是8个字节的数组
//BOOL GetMacAddress(unsigned char* pMacAddr)
//{
//    DWORD nRet;
//    //只查询物理地址
//    DWORD nFlags = GAA_FLAG_SKIP_UNICAST 
//        | GAA_FLAG_SKIP_ANYCAST
//        | GAA_FLAG_SKIP_FRIENDLY_NAME
//        | GAA_FLAG_SKIP_MULTICAST
//        | GAA_FLAG_SKIP_DNS_SERVER;
//
//    ULONG bufLen = 1024;
//    PIP_ADAPTER_ADDRESSES pAdapterAddr = (PIP_ADAPTER_ADDRESSES)malloc(bufLen);
//    if(pAdapterAddr == NULL)
//        return FALSE;
//
//    //AF_INET: return only IPv4 addresses.
//    nRet = GetAdaptersAddresses(AF_INET, nFlags,  NULL, pAdapterAddr, &bufLen);
//    if(nRet == ERROR_BUFFER_OVERFLOW)
//    {
//        pAdapterAddr = (PIP_ADAPTER_ADDRESSES)realloc(pAdapterAddr, bufLen);
//        if(pAdapterAddr == NULL)
//            return FALSE;
//
//        nRet = GetAdaptersAddresses(AF_INET, nFlags,  NULL, pAdapterAddr, &bufLen);
//    }
//
//    if(nRet == ERROR_SUCCESS)
//    {
//        memcpy(pMacAddr, &pAdapterAddr->PhysicalAddress, pAdapterAddr->PhysicalAddressLength);
//        free(pAdapterAddr);
//        return TRUE;
//    }else
//    {
//        //ff-ff-ff-ff-ff-ff: 表示获取失败（未知）
//        memset(pMacAddr, 0xff, 6);
//        free(pAdapterAddr);
//        return FALSE;
//    }
//}
#endif