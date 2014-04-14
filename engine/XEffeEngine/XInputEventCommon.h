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

#ifdef CREATE_WINDOW_WITH_GLUT
#define getInputEvent(p) getInputEventGlut(p) 
#endif
#ifdef CREATE_WINDOW_WITH_SDL
#define getInputEvent(p) getInputEventSDL(p) 
#endif

#endif