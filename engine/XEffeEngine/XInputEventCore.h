#ifndef _JIA_XINPUTEVENTCORE_
#define _JIA_XINPUTEVENTCORE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XMouseAndKeyBoardDefine.h"
namespace XE{
//输入事件的类型
enum XInputType
{
	EVENT_NULL,		//无效的事件
	EVENT_MOUSE,	//鼠标事件
	EVENT_KEYBOARD,	//键盘事件
	EVENT_TOUCH,	//触摸屏事件
	EVENT_JOYSTICK,	//手柄事件
	EVENT_SYSTEM,	//系统事件
	EVENT_RESIZE,	//改变窗口事件
	EVENT_EXIT,		//退出事件
};
enum XTouchState
{
	TOUCH_DOWN,
	TOUCH_MOVE,
	TOUCH_UP,
};
//输入事件(尚未完成，可以更具需求增加)
struct XInputEvent
{
	XInputType type;	//输入事件的类型
	int touchID;				//触摸的ID号
	XTouchState touchState;	//触摸的状态

	XKeyState keyState;	//键盘的状态
	XKeyValue keyValue;	//键盘的键值
	XMouseState mouseState;	//鼠标状态
	//鼠标的位置,这个数据是鼠标在窗口中的真实位置，没有经过场景缩放和平移，所以最好不要直接使用这个数据
	//建议使用经过转换的getMousePos;函数
	int mouseX;
	int mouseY;
	unsigned short unicode;
	XInputEvent()
		:type(EVENT_NULL)
	{}
	bool isKeyBoardDown()const { return type == EVENT_KEYBOARD && keyState == KEY_STATE_DOWN; }
	bool isKeyBoardUp()const { return type == EVENT_KEYBOARD && keyState == KEY_STATE_UP; }
	bool isMouseLeftDClick()const { return type == EVENT_MOUSE && mouseState == MOUSE_LEFT_BUTTON_DCLICK; }
	bool isMouseLeftDown()const { return type == EVENT_MOUSE && mouseState == MOUSE_LEFT_BUTTON_DOWN; }
	bool isMouseLeftUp()const { return type == EVENT_MOUSE && mouseState == MOUSE_LEFT_BUTTON_UP; }
	bool isMouseRightDown()const { return type == EVENT_MOUSE && mouseState == MOUSE_RIGHT_BUTTON_DOWN; }
	bool isMouseRightUp()const { return type == EVENT_MOUSE && mouseState == MOUSE_RIGHT_BUTTON_UP; }
	bool isMouseMove()const { return type == EVENT_MOUSE && mouseState == MOUSE_MOVE; }
	bool isMouseMiddleDown()const { return type == EVENT_MOUSE && mouseState == MOUSE_MIDDLE_BUTTON_DOWN; }
	bool isMouseMiddleUp()const { return type == EVENT_MOUSE && mouseState == MOUSE_MIDDLE_BUTTON_UP; }
	bool isMouseWheelDown()const { return type == EVENT_MOUSE && mouseState == MOUSE_WHEEL_DOWN_DOWN; }
	bool isMouseWheelUp()const { return type == EVENT_MOUSE && mouseState == MOUSE_WHEEL_UP_DOWN; }
};
//返回是否有未处理的事件
//inline bool getInputEvent(XInputEvent &event);
}
#endif