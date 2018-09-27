#ifndef _JIA_XINPUTEVENTGLFW_
#define _JIA_XINPUTEVENTGLFW_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XInputEventCore.h"
#include "windows.h"
#if CREATE_WINDOW_METHOD == 2
namespace XE {
//返回是否有未处理的事件
extern bool getInputEventGlfw(XInputEvent &event);
}
#endif
#endif