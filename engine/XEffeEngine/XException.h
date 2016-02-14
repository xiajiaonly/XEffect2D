#ifndef _JIA_XEXCEPTION_
#define _JIA_XEXCEPTION_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
//这里封装了一些异常处理的接口
//目前关于异常处理并没有太多有效而且深入的见解，很可惜
#include "XOSDefine.h"
namespace XE{
namespace XException
{
	extern LONG WINAPI unhandledExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionPointers);  
	extern BOOL WINAPI consoleHandler(DWORD consoleEvent);
	extern bool initException();	//初始化异常处理机制
	//为了兼容以前的例子而对外开放的接口
	extern void dumpMemory(const char *filename, void *p);
};
}
#endif