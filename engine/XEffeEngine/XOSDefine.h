#ifndef _JIA_OSDEFINE_
#define _JIA_OSDEFINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#define XEE_OS_WINDOWS        //Windows系统下
//#define XEE_OS_LINUX        //Linux系统下

//只在这里添加关于剔除一些警告的代码，不允许在其他地方有任何类似的添加
#pragma warning(disable: 4244)    //conversion from 'int' to 'float', possible loss of data  
#pragma warning(disable: 4267)    //conversion from 'size_t' to 'int', possible loss of data 
#pragma warning(disable: 4100)    //unreferenced formal parameter
#pragma warning(disable: 4245)    //conversion from 'int' to 'unsigned int', signed/unsigned mismatch
#pragma warning(disable: 4127)    //conditional expression is constant
#pragma warning(disable: 4018)    //signed/unsigned mismatch
#pragma warning(disable: 4996)    //'sscanf' was declared deprecated

//由于有的编译环境下中没有定义_XBool类型，所以这里做一些统一化的处理
#include "windows.h"
#if 1
//typedef BOOL _XBool;
//#define XFalse (FALSE)
//#define XTrue (TRUE)
typedef int _XBool;
#define XFalse (0)
#define XTrue (1)
#else
typedef bool _XBool;
#define XFalse (false)
#define XTrue (true)
#endif

#if defined DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

#endif
