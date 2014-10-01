//********************************************************************************
//下面是一个程序的框架，框架包括窗口的建立，鼠标键盘事件的处理部分以及图像渲染部分
//program by 贾胜华
//********************************************************************************
#include "GGame.h"
//Environment="PATH=%PATH%;..\..\engine\dll\libfreetype;..\..\engine\dll\SDL;..\..\engine\dll\gl;..\..\engine\dll\MemoryPool;"	//需要设置dll路径
#ifdef XEE_OS_WINDOWS
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	//隐藏控制台的编译说明
#endif

#ifdef CREATE_WINDOW_WITH_SDL
int inputEvent();	//下面是事件处理函数，例如：键盘事件或者鼠标事件

inline void XEE_Error(const char *p){REPORT_ERROR(p);}
LONG WINAPI unhandledExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionPointers)  
{  
    //SetErrorMode( SEM_NOGPFAULTERRORBOX );  
	//printf("Error address %x/n",pExceptionPointers->ExceptionRecord->ExceptionAddress);   
	//printf("CPU register:/n");   
	//printf("eax %x ebx %x ecx %x edx %x/n",pExceptionPointers->ContextRecord->Eax,   
	//	pExceptionPointers->ContextRecord->Ebx,pExceptionPointers->ContextRecord->Ecx,   
	//	pExceptionPointers->ContextRecord->Edx); 
	XEE_Error("运行异常");
	return EXCEPTION_EXECUTE_HANDLER;//EXCEPTION_CONTINUE_SEARCH; //或者 EXCEPTION_EXECUTE_HANDLER 关闭程序  
} 
BOOL WINAPI consoleHandler(DWORD consoleEvent)
{
	switch(consoleEvent)
	{
	case CTRL_C_EVENT:		//关闭
	case CTRL_BREAK_EVENT:	//暂停
	case CTRL_CLOSE_EVENT:	//关闭
	case CTRL_LOGOFF_EVENT:	//用户退出
	case CTRL_SHUTDOWN_EVENT:	//关闭
		_XLogbook::GetInstance().releaseLog();	//窗口退出时，保证日志的完整性
		//XEE::release();	//捕获到退出事件，但是未解决问题，需要进一步的处理
		break;
	}
	return TRUE;
}
int main(int argc, char **argv)
{
	SetUnhandledExceptionFilter(unhandledExceptionFilter);  
	__try{ 
		if(SetConsoleCtrlHandler((PHANDLER_ROUTINE)consoleHandler,TRUE) == FALSE)
		{
			printf("Unable to install handler!\n");
			return 0;
		}
		if(!initWindowEx()) XEE_Error("建立窗体失败");
		if(!_GGameMain.init()) XEE_Error("初始化失败");

		while(!inputEvent()) 
		{//程序主循环
			_GGameMain.move(XEE::engineIdle());
			XEE::clearScreen();			//清除屏幕
			_GGameMain.draw();

			XEE::updateScreen();			//更新屏幕的内容
			//mySleep(1);
		}
		_GGameMain.release();
		XEE::release();
	}__except(EXCEPTION_EXECUTE_HANDLER) 
	{
		XEE_Error("运行异常!");
	}
	return 0;	
}

int inputEvent()
{
	_XInputEvent tmpEvent;		//SDL事件句柄
	while(getInputEventSDL(tmpEvent)) 
	{
		switch(tmpEvent.type)
		{
		case EVENT_EXIT:return 1;
		case EVENT_RESIZE:	//窗口大小发生变化
			XEE::windowData.w = tmpEvent.mouseX;
			XEE::windowData.h = tmpEvent.mouseY;
			initWindowMatrix();
			break;
		case EVENT_KEYBOARD:
			if(tmpEvent.keyState == KEY_STATE_DOWN)
			{
				switch(tmpEvent.keyValue)
				{
				case XKEY_ESCAPE:return 1;
				}
			}
			break;
		}
		_GGameMain.input(tmpEvent);
		XEE::inputEvent(tmpEvent);
	}
	return 0;
}
#endif
#ifdef CREATE_WINDOW_WITH_GLUT
void draw(){}
void idle()
{
	_GGameMain.move(XEE::engineIdle());

	XEE::clearScreen();			//清除屏幕
	_GGameMain.draw();
	XEE::updateScreen();			//更新屏幕的内容
	//Sleep(1);
}
void inputProc(const _XInputEvent &input)
{
	_GGameMain.input(input);
	XEE::inputEvent(input);
}
void input(unsigned char c,int x,int y)
{
	_XInputEvent tmpEvent;
	tmpEvent.type = EVENT_KEYBOARD;
	tmpEvent.keyState = KEY_STATE_DOWN;
	tmpEvent.keyValue = (_XKeyValue)_XWindow.mapKey(c);
	tmpEvent.mouseX = x;
	tmpEvent.mouseY = y;
	inputProc(tmpEvent);
	if(tmpEvent.keyValue == XKEY_ESCAPE) exit(1);
}
void sInput(int k,int x,int y)
{
	_XInputEvent tmpEvent;
	tmpEvent.type = EVENT_KEYBOARD;
	tmpEvent.keyState = KEY_STATE_DOWN;
	tmpEvent.keyValue = (_XKeyValue)_XWindow.mapKey(k + 512);
	tmpEvent.mouseX = x;
	tmpEvent.mouseY = y;
	inputProc(tmpEvent);
	if(tmpEvent.keyValue == XKEY_ESCAPE) exit(1);
}
void mouseMove(int x,int y)
{
	_XInputEvent tmpEvent;
	tmpEvent.type = EVENT_MOUSE;
	tmpEvent.mouseState = MOUSE_MOVE;
	tmpEvent.mouseX = x;
	tmpEvent.mouseY = y;
	inputProc(tmpEvent);
}
void mouseProc(int b,int s,int x,int y)
{
	_XInputEvent tmpEvent;
	tmpEvent.type = EVENT_MOUSE;
	tmpEvent.mouseX = x;
	tmpEvent.mouseY = y;
	if(s == GLUT_DOWN)
	{
		switch(b)
		{
		case GLUT_LEFT_BUTTON: tmpEvent.mouseState = MOUSE_LEFT_BUTTON_DOWN;break;
		case GLUT_RIGHT_BUTTON: tmpEvent.mouseState = MOUSE_RIGHT_BUTTON_DOWN;break;
		case GLUT_MIDDLE_BUTTON: tmpEvent.mouseState = MOUSE_MIDDLE_BUTTON_DOWN;break;
		}
	}else
	{
		switch(b)
		{
		case GLUT_LEFT_BUTTON: tmpEvent.mouseState = MOUSE_LEFT_BUTTON_UP;break;
		case GLUT_RIGHT_BUTTON: tmpEvent.mouseState = MOUSE_RIGHT_BUTTON_UP;break;
		case GLUT_MIDDLE_BUTTON: tmpEvent.mouseState = MOUSE_MIDDLE_BUTTON_UP;break;
		}
	}
	inputProc(tmpEvent);
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	if(!initWindowEx()) REPORT_ERROR("建立窗体失败");
	if(!_GGameMain.init()) REPORT_ERROR("初始化失败");

	glutDisplayFunc(draw);
	glutIdleFunc(idle);
	//glutReshapeFunc(changeSize);
	glutKeyboardFunc(input);
	glutSpecialFunc(sInput);
	glutMouseFunc(mouseProc);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);
	glutMainLoop();

	_GGameMain.release();
	XEE::release();
	return 0;	
}
#endif