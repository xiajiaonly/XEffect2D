#ifndef _JIA_XBASICSDL_
#define _JIA_XBASICSDL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "SDL_image.h"

#define WITH_OBJECT_MANAGER (0)	//是否使用物件管理

extern int XEE_windowWidth;		//当前窗口的宽度
extern int XEE_windowHeight;	//当前窗口的高度
extern int XEE_frameTime;	//上一帧的时间
extern float XEE_nowFPS;	//当前的帧率

extern int XEE_audioFrequency;	//音频初始化的频率
extern int XEE_audioFormat;		//音频的格式
extern int XEE_audioChannel;	//音频的声道

extern SDL_Surface * loadImage(const char *pFileName,int alphaFlag = 0,int resourcesPosition = 0);	//载入图片资源
extern SDL_Surface * loadImageEx(const char *pFileName,int resourcesPosition = 0);	//载入图片资源
extern int isNPOT(int width, int height);									//判断是否符合2的n次方

extern int getFrameTiming();											//计算显示5秒的帧率，并且返回两次调用之间的时间差
extern SDL_Surface * initSDL(int width,int height,const char *windowTitle = NULL,int isFullScreen = 0);	//初始化SDL窗口
extern void releaseSDL();
extern void inputEvent(const SDL_Event& inputEvent);
extern void engineMove(int delay);			//引擎内部的动作处理

#endif
