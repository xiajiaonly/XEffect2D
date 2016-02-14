#ifndef _JIA_XWINSOCKCOMMON_
#define _JIA_XWINSOCKCOMMON_
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		2012.4.3
//--------------------------------
#include "../XOSDefine.h"

#ifdef XEE_OS_WINDOWS
#include "winsock.h"
#endif
#pragma comment(lib, "ws2_32.lib")

#ifdef XEE_OS_LINUX
#include <sys/socket.h>
#include <sys/types.h>
#include <malloc.h>
#include <arpa/inet.h>        //inet_addr()
#include <unistd.h>           //usleep()
#include <netdb.h>
#endif
#endif