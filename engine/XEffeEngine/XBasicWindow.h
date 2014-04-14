#ifndef _JIA_XBASICWINDOW_
#define _JIA_XBASICWINDOW_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XBasicFun.h"
#include "XCommonDefine.h"
#include "XInputEventCommon.h"

class _XFColor;
namespace XEE
{ 
extern int windowWidth;		//当前窗口的宽度
extern int windowHeight;	//当前窗口的高度
extern int sceneX;			//场景的位置
extern int sceneY;			//场景的位置
extern int sceneWidth;		//场景的宽度
extern int sceneHeight;		//场景的高度
extern float sceneSizeX;		//窗口的缩放比例
extern float sceneSizeY;		//窗口的缩放比例
extern float viewAngle3D;		//3D观察的角度
extern _XWindowData windowData;	//窗口的相关数据
extern _XResourcePosition defaultResourcePosition;

extern int frameTime;	//上一帧的时间
extern float nowFPS;	//当前的帧率

extern int audioFrequency;	//音频初始化的频率
extern int audioFormat;		//音频的格式
extern int audioChannel;	//音频的声道
extern _XVector2 mousePosition;	//当前的鼠标位置

extern void release();
extern void inputEvent(const _XInputEvent& inputEvent);
extern void keyProc(int key,_XKeyState keyState);
extern void mouseProc(int x,int y,_XMouseState mouseState);
extern LRESULT (*customWinProc)(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam,bool &end);	//自定义的系统消息处理函数
extern HWND wHandle;
extern HDC wHDC;	//窗口容器的指针
extern HGLRC wCurrentHGLRC;	//当前的上下文
extern HGLRC wCopyHGLRC;	//当前上下文的副本
extern unsigned int mainThreadHandle;	//主线程的ID

extern char isOpenGL2Support;	//OpenGL是否支持2.0是否支持GLSL
extern int maxTetureSize;		//显卡支持的最大贴图尺寸
extern char isLineSmooth;		//是否使用划线的平滑处理
extern _XFColor defaultBGColor;	//默认的背景颜色

}

extern bool createWindow(int width,int height,const char *windowTitle = NULL,int isFullScreen = 0,int withFrame = 1);	//初始化SDL窗口

#endif