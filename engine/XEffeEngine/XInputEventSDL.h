#ifndef _JIA_XINPUTEVENTSDL_
#define _JIA_XINPUTEVENTSDL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XInputEventCore.h"
#include "SDL.h"
namespace XE{
//返回是否有未处理的事件
extern bool SDLEventToInputEvent(XInputEvent &event,const SDL_Event &tmpEvent);	//将SDL的输入事件转换成XEE的输入事件
inline bool getInputEventSDL(XInputEvent &event)
{
#ifdef _WIN64
	return false;
#endif
	SDL_Event tmpEvent;		//SDL事件句柄
	if(SDL_PollEvent(&tmpEvent) == 0) return false;	//在x64下此处也会crash问题不明
	//下面转换事件
	return SDLEventToInputEvent(event,tmpEvent);
}
}

#endif