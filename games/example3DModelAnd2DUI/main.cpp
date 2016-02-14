//********************************************************************************
//这是一个展示3D与2D结合的例子
//program by 贾胜华
//********************************************************************************
#include "GGameE.h"
//Environment="PATH=%PATH%;..\..\engine\dll\gl;..\..\engine\dll\MemoryPool;"	//需要设置dll路径
#ifdef XEE_OS_WINDOWS
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	//隐藏控制台的编译说明
#endif

int main(int argc, char **argv)
{
	XE::runGame<GGame>();
	return 0;	
}