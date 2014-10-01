#ifndef _JIA_XINPUTEVENTCORE_
#define _JIA_XINPUTEVENTCORE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XMouseAndKeyBoardDefine.h"

//输入事件的类型
enum _XInputType
{
	EVENT_NULL,		//无效的事件
	EVENT_MOUSE,	//鼠标事件
	EVENT_KEYBOARD,	//键盘事件
	EVENT_JOYSTICK,	//手柄事件
	EVENT_SYSTEM,	//系统事件
	EVENT_RESIZE,	//改变窗口事件
	EVENT_EXIT,		//退出事件
};
//输入事件(尚未完成，可以更具需求增加)
struct _XInputEvent
{
	_XInputType type;	//输入事件的类型

	_XKeyState keyState;	//键盘的状态
	_XKeyValue keyValue;	//键盘的键值

	_XMouseState mouseState;	//鼠标状态
	int mouseX;					//鼠标的位置
	int mouseY;
	unsigned short unicode;
	_XInputEvent()
		:type(EVENT_NULL)
	{}
};
//返回是否有未处理的事件
//inline bool getInputEvent(_XInputEvent &event);

#endif