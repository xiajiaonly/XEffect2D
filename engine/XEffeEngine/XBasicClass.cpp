//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBasicClass.h"

#ifdef XEE_OS_WINDOWS
//void close(int temp)
//{
//    //注意这里需要使用shutdown(),但是具体如何使用还需要详细摸索（20100527）
//    //shutdown()
//    closesocket(temp);
//}

//#include "winsock2.h"

int initSocket()
{
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(1,1),&wsaData) != 0) return 0;
    if(LOBYTE(wsaData.wVersion) != 1 
           || HIBYTE(wsaData.wVersion) != 1) 
    {
        WSACleanup( );    
        return 0; 
    }
    return 1;
}
#endif