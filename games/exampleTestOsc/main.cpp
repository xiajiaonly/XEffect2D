//********************************************************************************
//这是一个基于OfxOSC的测试工具
//program by 贾胜华
//********************************************************************************
#include "GGame.h"
//Environment="PATH=%PATH%;..\..\engine\dll\gl;..\..\engine\dll\MemoryPool;..\..\engine\dll\fmodex;"	//需要设置dll路径
#ifdef XEE_OS_WINDOWS
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	//隐藏控制台的编译说明
#endif

int main(int argc, char **argv)
{
	XE::runGame<GGame>();
	return 0;	
}