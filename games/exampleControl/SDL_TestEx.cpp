//********************************************************************************
//这是一个测试物件管理类的例子
//program by 贾胜华
//********************************************************************************
//Environment="PATH=%PATH%;..\..\engine\dll\libfreetype;..\..\engine\dll\SDL;..\..\engine\dll\gl;"	//需要设置dll路径
#include "GGame.h"

#ifdef OS_WINDOWS
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	//隐藏控制台的编译说明
#endif

#ifdef CREATE_WINDOW_WITH_SDL
int inputEvent();	//下面是事件处理函数，例如：键盘事件或者鼠标事件

int main(int argc, char **argv)
{
	if(!initWindowEx()) REPORT_ERROR("建立窗体失败");
	if(!_GGame::GetInstance().init()) REPORT_ERROR("初始化失败");

	while(!inputEvent()) 
	{//程序主循环
		_GGame::GetInstance().move(XEE::engineIdle());
		XEE::clearScreen();			//清除屏幕
		_GGame::GetInstance().draw();

		XEE::updateScreen();			//更新屏幕的内容
		//mySleep(1);
	}
	_GGame::GetInstance().release();
	XEE::release();
	return 0;	
}

int inputEvent()
{
	int ret =0;
	_XInputEvent tmpEvent;		//SDL事件句柄

	while(getInputEventSDL(tmpEvent)) 
	{
		switch(tmpEvent.type)
		{
		case EVENT_EXIT:ret = 1;break;
		case EVENT_KEYBOARD:
			if(tmpEvent.keyState == KEY_STATE_DOWN)
			{
				switch(tmpEvent.keyValue)
				{
				case XKEY_ESCAPE:ret = 1;break;
				}
			}
			break;
		}
		_GGame::GetInstance().input(tmpEvent);
		XEE::inputEvent(tmpEvent);
	}
	return ret;
}
#endif
#ifdef CREATE_WINDOW_WITH_GLUT
void draw(){}
void idle()
{
	_GGame::GetInstance().move(XEE::engineIdle());

	XEE::clearScreen();			//清除屏幕
	_GGame::GetInstance().draw();
	XEE::updateScreen();			//更新屏幕的内容
	//Sleep(1);
}
void inputProc(const _XInputEvent &input)
{
	_GGame::GetInstance().input(input);
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
	printf("%d\n",c);
}
void sInput(int k,int x,int y){}
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
	if(!_GGame::GetInstance().init()) REPORT_ERROR("初始化失败");

	glutDisplayFunc(draw);
	glutIdleFunc(idle);
	//glutReshapeFunc(changeSize);
	glutKeyboardFunc(input);
	glutSpecialFunc(sInput);
	glutMouseFunc(mouseProc);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);
	glutMainLoop();

	_GGame::GetInstance().release();
	XEE::release();
	return 0;	
}
#endif