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
class _XWindowTitle;
class _XFontUnicode;
namespace XEE
{ 
	extern int windowWidth;		//当前窗口的宽度
	extern int windowHeight;	//当前窗口的高度
	extern int sceneX;			//场景的位置	//目前这个定义尚不完整，在很多模式下并没有效果
	extern int sceneY;			//场景的位置
	extern int sceneWidth;		//场景的宽度
	extern int sceneHeight;		//场景的高度
	extern float sceneSizeX;		//窗口的缩放比例
	extern float sceneSizeY;		//窗口的缩放比例
	extern float viewAngle3D;		//3D观察的角度
	extern _XWindowData windowData;	//窗口的相关数据
	extern _XResourcePosition defaultResourcePosition;
	extern bool isWindowExit;		//是否准备退出程序
	extern bool isAlwaysTop;		//窗口是否总是在最前面

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
	extern char isMultiSampleSupport;	//是否支持多重采样

	extern _XBool stopFlag;	//是否暂停
	extern int manualFPS;	//-1是不设置帧率
	extern float manualFrameTime;	//设置的每帧的时间
	extern _XBool needReset3DParam;	//是否需要回复3D参数的设置

	extern _XBool haveSystemFont;
	extern _XFontUnicode systemFont;
	extern _XBool isSuspend;	//程序是否进入中断，如果进入中断的话要退出loading的多线程
	//下面的变量用于自动显示版本号
	extern int showVersionTimer;			//显示版本号的计时器
	extern _XFontUnicode showVersionStr;	//显示版本号的字符串
	extern int autoShutDownTimer;			//自动关机的计时
	extern char autoShutDownState;			//自动关机的状态，只有跨越设定时间才会触发自动关机
	//下面是关于自定义标题栏的支持
	extern _XWindowTitle customWinTitle;	//自定义的窗口标题
	inline void setStop(int temp = -1)	//停止 0,play,1,stop,-1,change state
	{
		if(temp > 0) XEE::stopFlag = XTrue; 
		else if(temp == 0) XEE::stopFlag = XFalse; 
		else
		if(temp < 0)
		{
			XEE::stopFlag = !XEE::stopFlag;
		}
	}
	inline _XBool getStopState()
	{
		return XEE::stopFlag;
	}
	inline void setFPS(int temp)		//设置游戏帧率，小于0为不限制帧率(目前这个函数好像没有起到应有的作用)
	{
		if(temp <= 0) return;	//不合理的数据
		XEE::manualFPS = temp;
		XEE::manualFrameTime = 2000.0f / temp;
	}

}

extern bool createWindow(int width,int height,const char *windowTitle = NULL,int isFullScreen = 0,int withFrame = 1);	//初始化SDL窗口
inline void setWindowPos(int x,int y,bool always = false)
{//设置窗口的位置
	RECT rect;
	GetWindowRect(XEE::wHandle,&rect);
	XEE::isAlwaysTop = always;
	if(XEE::isAlwaysTop) SetWindowPos(XEE::wHandle,HWND_TOPMOST,x,y,rect.right - rect.left,rect.bottom - rect.top,SWP_SHOWWINDOW);
	else SetWindowPos(XEE::wHandle,HWND_TOP,x,y,rect.right - rect.left,rect.bottom - rect.top,SWP_SHOWWINDOW);
}
inline void getWindowPos(int &x,int &y)
{
	RECT rect;
	GetWindowRect(XEE::wHandle,&rect);
	x = rect.left;
	y = rect.top;
}
inline void getWindowSize(int &w,int &h)
{
	RECT rect;
	GetWindowRect(XEE::wHandle,&rect);
	w = rect.right - rect.left;
	h = rect.bottom - rect.top;
}
inline void minSizeWindow(){ShowWindow(XEE::wHandle,SW_MINIMIZE);}
inline void exitWindow(){XEE::isWindowExit = true;}	//退出窗口
inline void setWindowSize(float x,float y)
{
	RECT rect;
	GetWindowRect(XEE::wHandle,&rect);
	if(XEE::isAlwaysTop) SetWindowPos(XEE::wHandle,HWND_TOPMOST,rect.left,rect.top,x,y,SWP_SHOWWINDOW);
	else SetWindowPos(XEE::wHandle,HWND_TOP,rect.left,rect.top,x,y,SWP_SHOWWINDOW);
}
//设置窗口的 透明度
inline void setWindowAlpha(float a)
{
	//bool isFirst = true;
	//if(isFirst)
	//{
	//	SetWindowLong(XEE::wHandle, GWL_EXSTYLE, GetWindowLong(XEE::wHandle, GWL_EXSTYLE) | WS_EX_LAYERED);
	//	isFirst = false;
	//}
	////SetLayeredWindowAttributes(XEE::wHandle,RGB(64,64,64),0,LWA_COLORKEY);
	//SetLayeredWindowAttributes(XEE::wHandle,0,a * 255,LWA_ALPHA);
}

#endif