#include "XStdHead.h"
#include "XSocketBasic.h"
namespace XE{
#ifdef XEE_OS_WINDOWS
//void close(int temp)
//{
//    //注意这里需要使用shutdown(),但是具体如何使用还需要详细摸索（20100527）
//    //shutdown()
//    closesocket(temp);
//}

//#include "winsock2.h"
namespace XSock
{
	bool initSocket()
	{
		WSADATA wsaData;

		if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) return false;
		if (LOBYTE(wsaData.wVersion) != 1
			|| HIBYTE(wsaData.wVersion) != 1)
		{
			WSACleanup();
			return false;
		}
		return true;
	}
}
#endif
}