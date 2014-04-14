#ifndef _JIA_XWINDOWCOMMON_
#define _JIA_XWINDOWCOMMON_
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XCommonDefine.h"
#include "XWindowSDL.h"
#include "XWindowGlut.h"

#ifdef CREATE_WINDOW_WITH_GLUT
#define _XWindow _XWindowGlut::GetInstance()
#endif
#ifdef CREATE_WINDOW_WITH_SDL
#define _XWindow _XWindowSDL::GetInstance()
#endif

#endif