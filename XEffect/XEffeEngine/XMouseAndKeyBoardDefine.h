#ifndef _JIA_XMOUSEANDKEYBOARDDEFINE_
#define _JIA_XMOUSEANDKEYBOARDDEFINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.10
//--------------------------------
//下面是关于鼠标和键盘的一些公共宏的定义
//enum _XMouseState
//{
//	_XMOUSE_STATE_MOVE,		//鼠标移动
//	_XMOUSE_STATE_DOWN,		//鼠标按下
//	_XMOUSE_STATE_UP		//鼠标弹起
//};
//enum _XMouseKeyOrder
//{
//	_XMOUSE_KEY_ORDER_LEFT,		//鼠标左键
//	_XMOUSE_KEY_ORDER_MIDDLE,	//鼠标中键
//	_XMOUSE_KEY_ORDER_RIGHT,	//鼠标右键
//	_XMOUSE_KEY_ORDER_NULL		//没有按键
//};
enum _XKeyState
{
	_XKEY_STATE_DOWN,		//按键按下
	_XKEY_STATE_UP			//按键弹起
};
enum _XMouseState
{
	_XMOUSE_MOVE,				//鼠标移动
	_XMOUSE_LEFT_BUTTON_DOWN,	//鼠标左键按下
	_XMOUSE_LEFT_BUTTON_UP,		//鼠标左键弹起
	_XMOUSE_RIGHT_BUTTON_DOWN,	//鼠标右键按下
	_XMOUSE_RIGHT_BUTTON_UP,	//鼠标右键弹起
	_XMOUSE_MIDDLE_BUTTON_DOWN,	//鼠标中键按下
	_XMOUSE_MIDDLE_BUTTON_UP,	//鼠标中键弹起
};
#endif