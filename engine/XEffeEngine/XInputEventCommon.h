#ifndef _JIA_XINPUTEVENTCOMMON_
#define _JIA_XINPUTEVENTCOMMON_
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XCommonDefine.h"
#include "XInputEventSDL.h"
#include "XInputEventGlut.h"
#include "XInputEventGlfw.h"
namespace XE{
#if CREATE_WINDOW_METHOD == 0
#define getInputEvent(p) getInputEventSDL(p) 
#endif
#if CREATE_WINDOW_METHOD == 1
#define getInputEvent(p) getInputEventGlut(p) 
#endif
#if CREATE_WINDOW_METHOD == 2
#define getInputEvent(p) getInputEventGlfw(p) 
#endif
}
#endif