#ifndef _JIA_XSOCKETBASIC_
#define _JIA_XSOCKETBASIC_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:        2011.4.9
//--------------------------------
//这里定义的是一些关于socket操作的相关接口
//#include "XOSDefine.h"
namespace XE{
#ifdef XEE_OS_WINDOWS
namespace XSock
{
	extern bool initSocket();
};
#endif
}
#endif