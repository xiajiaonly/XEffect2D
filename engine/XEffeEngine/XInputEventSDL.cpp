#include "XInputEventSDL.h"
#include "XWindowCommon.h"

bool getInputEventSDL(_XInputEvent &event)
{
	SDL_Event tmpEvent;		//SDL事件句柄
	if(SDL_PollEvent(&tmpEvent) == 0) return false;
	//下面转换事件
	switch(tmpEvent.type)
	{
	case SDL_QUIT:event.type = EVENT_EXIT;break;
	case SDL_KEYDOWN:
		event.type = EVENT_KEYBOARD;
		event.keyState = KEY_STATE_DOWN;
		event.keyValue = (_XKeyValue)_XWindow.mapKey(tmpEvent.key.keysym.sym);
		event.unicode = tmpEvent.key.keysym.unicode;
		break;
	case SDL_KEYUP:
		event.type = EVENT_KEYBOARD;
		event.keyState = KEY_STATE_UP;
		event.keyValue = (_XKeyValue)_XWindow.mapKey(tmpEvent.key.keysym.sym);
		event.unicode = tmpEvent.key.keysym.unicode;
		break;
	case SDL_MOUSEMOTION:
		event.type = EVENT_MOUSE;
		event.mouseState = MOUSE_MOVE;
		event.mouseX = tmpEvent.motion.x;
		event.mouseY = tmpEvent.motion.y;
		break;
	case SDL_MOUSEBUTTONDOWN:
		event.type = EVENT_MOUSE;
		switch(tmpEvent.button.button)
		{
		case 1:event.mouseState = MOUSE_LEFT_BUTTON_DOWN;break;
		case 2:event.mouseState = MOUSE_MIDDLE_BUTTON_DOWN;break;
		case 3:event.mouseState = MOUSE_RIGHT_BUTTON_DOWN;break;
		}
		event.mouseX = tmpEvent.button.x;
		event.mouseY = tmpEvent.button.y;
		break;
	case SDL_MOUSEBUTTONUP:
		event.type = EVENT_MOUSE;
		switch(tmpEvent.button.button)
		{
		case 1:event.mouseState = MOUSE_LEFT_BUTTON_UP;break;
		case 2:event.mouseState = MOUSE_MIDDLE_BUTTON_UP;break;
		case 3:event.mouseState = MOUSE_RIGHT_BUTTON_UP;break;
		}
		event.mouseX = tmpEvent.button.x;
		event.mouseY = tmpEvent.button.y;
		break;
	}
	return true;
}