//********************************************************************************
//这是一种对老的拖尾效果的总结和新的拖尾效果的研究
//program by 贾胜华
//********************************************************************************
#include "GGameTail.h"
//Environment="PATH=%PATH%;..\..\engine\dll\SDL;..\..\engine\dll\fmodex;"	//需要设置dll路径
#ifdef XEE_OS_WINDOWS
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	//隐藏控制台的编译说明
#endif

int main(int argc, char **argv)
{  
	XE::runGame<GGame>(&XE::XWindowInfo(1280,720));
	return 0;	
}