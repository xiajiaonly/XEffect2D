#ifndef _JIA_XINPUTEVENTGLUT_
#define _JIA_XINPUTEVENTGLUT_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XInputEventCore.h"
#if CREATE_WINDOW_METHOD == 1
namespace XE{
//返回是否有未处理的事件
inline bool getInputEventGlut(XInputEvent &) {return true;}
}
#endif
#endif