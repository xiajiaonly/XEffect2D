#ifndef _XMOVIE_DLL_
#define _XMOVIE_DLL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XEffeEngine.h"

#if defined DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

#include "windows.h"

extern "C"
{
	DECLDIR bool startHook();	//初始化钩子
	DECLDIR void stopHook();	//结束钩子
	DECLDIR bool popAInputEvent(_XInputEvent & e);	//从列表中弹出一个输入时间
	DECLDIR std::string getCurWindowTitle();
}

#endif