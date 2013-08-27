#ifndef _JIA_OSDEFINE_
#define _JIA_OSDEFINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

//#define DEBUG_MODE	//所有需要调试与正式版本不同的模块都需要添加这个头文件
//#define GET_ALL_PIXEL		//统计整体的贴图面积，用于标称游戏的贴图效率
#define OS_WINDOWS        //Windows系统下
//#define OS_LINUX        //Linux系统下

#if defined DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

#endif
