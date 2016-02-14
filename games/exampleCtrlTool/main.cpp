//********************************************************************************
//下面是一个程序的框架，框架包括窗口的建立，鼠标键盘事件的处理部分以及图像渲染部分
//program by 贾胜华
//********************************************************************************
#include "GGame.h"
//Environment="PATH=%PATH%;..\..\engine\dll\libfreetype;..\..\engine\dll\SDL;..\..\engine\dll\gl;..\..\engine\dll\MemoryPool;"	//需要设置dll路径
#ifdef XEE_OS_WINDOWS
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	//隐藏控制台的编译说明
#endif

int main(int argc, char **argv)
{	
	XE::runGame<GGame>(&XE::XWindowInfo(1366,768));
	return 0;	
}