#include "XStdHead.h"
#include "XInputEventSDL.h"
#include "XWindowCommon.h"
#include "XTimer.h"
namespace XE{
bool isSDLDoubleClick(const SDL_Event &tmpEvent)
{
	static int upTime = XTime::getCurrentTicks();
	static int counter = 0;	//按下计数器
	if(tmpEvent.type == SDL_MOUSEBUTTONDOWN)
	{
		if(tmpEvent.button.button == SDL_BUTTON_LEFT)
		{
			if(counter == 0)
			{//第一次按下
				upTime = XTime::getCurrentTicks();
				++counter;
			}else
			{//第二次按下
				if(XTime::getCurrentTicks() - upTime < 500)	//连续判断时间为200ms
				{//双击时间成立
					counter = 0;
					//printf("鼠标双击事件\n");
					return true;
				}else
				{
					upTime = XTime::getCurrentTicks();
				}
			}
		}else
		{
			counter = 0;	//取消记录
			//printf("取消记录\n");
		}
	}else
	if(!(tmpEvent.button.button == SDL_BUTTON_LEFT && (tmpEvent.type == SDL_MOUSEBUTTONDOWN || tmpEvent.type == SDL_MOUSEBUTTONUP))
		&& counter != 0)
	{
		counter = 0;	//取消记录
		//printf("取消记录\n");
	}
	return false;
}

bool SDLEventToInputEvent(XInputEvent &event,const SDL_Event &tmpEvent)	//将SDL的输入事件转换成XEE的输入事件
{
	switch(tmpEvent.type)
	{
	case SDL_VIDEORESIZE:
		event.type = EVENT_RESIZE;
		event.mouseX = tmpEvent.resize.w;
		event.mouseY = tmpEvent.resize.h;
		break;
	case SDL_QUIT:event.type = EVENT_EXIT;break;
	case SDL_KEYDOWN:
		event.type = EVENT_KEYBOARD;
		event.keyState = KEY_STATE_DOWN;
		event.keyValue = (XKeyValue)XWindow.mapKey(tmpEvent.key.keysym.sym);
		event.unicode = tmpEvent.key.keysym.unicode;
		break;
	case SDL_KEYUP:
		event.type = EVENT_KEYBOARD;
		event.keyState = KEY_STATE_UP;
		event.keyValue = (XKeyValue)XWindow.mapKey(tmpEvent.key.keysym.sym);
		event.unicode = tmpEvent.key.keysym.unicode;
		break;
	case SDL_MOUSEMOTION:
		event.type = EVENT_MOUSE;
		event.mouseState = MOUSE_MOVE;
		event.mouseX = tmpEvent.motion.x;
		event.mouseY = tmpEvent.motion.y;
		isSDLDoubleClick(tmpEvent);
		break;
	case SDL_MOUSEBUTTONDOWN:
		event.type = EVENT_MOUSE;
		switch(tmpEvent.button.button)
		{
		case SDL_BUTTON_LEFT:
			if(isSDLDoubleClick(tmpEvent)) 
				event.mouseState = MOUSE_LEFT_BUTTON_DCLICK;
			else
				event.mouseState = MOUSE_LEFT_BUTTON_DOWN;
			break;
		case SDL_BUTTON_MIDDLE:event.mouseState = MOUSE_MIDDLE_BUTTON_DOWN;break;
		case SDL_BUTTON_RIGHT:event.mouseState = MOUSE_RIGHT_BUTTON_DOWN;break;
		case SDL_BUTTON_WHEELUP:event.mouseState = MOUSE_WHEEL_UP_DOWN;break;
		case SDL_BUTTON_WHEELDOWN:event.mouseState = MOUSE_WHEEL_DOWN_DOWN;break;
		}
		event.mouseX = tmpEvent.button.x;
		event.mouseY = tmpEvent.button.y;
		break;
	case SDL_MOUSEBUTTONUP:
		event.type = EVENT_MOUSE;
		isSDLDoubleClick(tmpEvent);
		switch(tmpEvent.button.button)
		{
		case SDL_BUTTON_LEFT:event.mouseState = MOUSE_LEFT_BUTTON_UP;break;
		case SDL_BUTTON_MIDDLE:event.mouseState = MOUSE_MIDDLE_BUTTON_UP;break;
		case SDL_BUTTON_RIGHT:event.mouseState = MOUSE_RIGHT_BUTTON_UP;break;
		case SDL_BUTTON_WHEELUP:event.mouseState = MOUSE_WHEEL_UP_UP;break;
		case SDL_BUTTON_WHEELDOWN:event.mouseState = MOUSE_WHEEL_DOWN_UP;break;
		}
		event.mouseX = tmpEvent.button.x;
		event.mouseY = tmpEvent.button.y;
		break;
	}
	return true;
}
}