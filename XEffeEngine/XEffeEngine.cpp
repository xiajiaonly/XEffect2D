//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XEffeEngine.h"
#include "XEngineLogo.h"
#include "XResourcePack.h"

SDL_Surface * initWindow(int width,int height,const char *windowTitle,int isFullScreen,int isShowCursor)
{
	if(width <= 0 || height <= 0 || windowTitle == NULL)
	{//检查输入参数是否正确
		return NULL;
	}
	srand((unsigned)time(0)); //引入随机因子
	SDL_Surface *screen = initSDL(width,height,windowTitle,isFullScreen);
	if(screen == NULL) return NULL;

	if(isShowCursor == 0)
	{//隐藏鼠标
		SDL_ShowCursor(0);	//隐藏光标
	}
	if(initOpenGL(width,height) == 0) return NULL;

	if(screen != NULL)
	{
		_XEngineLogo EngineLogo;
		_XResourcePack::GetInstance().setOutFileName("XEngine.log");
		EngineLogo.init(_XVector2(width * 0.5 - 256 + 160,height * 0.5 - 256 + 128 + 96),1);
		_XResourcePack::GetInstance().setOutFileName(NULL);
		EngineLogo.reset();
		EngineLogo.setBackSize(width/8.0f,height/8.0f);
		int interval = 0;
		while(1) 
		{
			interval = getFrameTiming();
			EngineLogo.move(interval);
			EngineLogo.draw();
			if(EngineLogo.getIsEnd() != 0) break;
		}
		EngineLogo.release();
	}
	return screen;
}

char XEE_stopFlag = 0;	//是否暂停
int XEE_setFPS = -1;	//-1是不设置帧率

void XSetStop(int temp)
{
	if(temp > 0) XEE_stopFlag = 1; else
	if(temp == 0) XEE_stopFlag = 0; else
	if(temp < 0)
	{
		if(XEE_stopFlag == 0) XEE_stopFlag = 1;
		else XEE_stopFlag = 0;
	}
}

int XGetStopState()
{
	return XEE_stopFlag;
}

void XSetFPS(int temp)
{
	XEE_setFPS = temp;
}

int XEngineIdle()
{
	static Uint32 current, last = 0, five = 0, nframes = 0;

	current = SDL_GetTicks();
	++nframes;

	if (current - five > 5*1000) 
	{//将帧率显示出来
		XEE_nowFPS = nframes/5.0f;
#ifdef DEBUG_MODE
		printf("%u frames in 5 seconds = %.1f FPS\n",nframes,XEE_nowFPS);
#endif
		nframes = 0;
		five = current;
	}
	
	XEE_frameTime = current - last;
	last = current;

	if(XEE_setFPS > 0 && XEE_frameTime < 1000.0f/(float)(XEE_setFPS))
	{
		SDL_Delay(1000.0f/(float)(XEE_setFPS) - XEE_frameTime);
		//mySleep(1000.0f/(float)(XEE_setFPS) - XEE_frameTime);
	}
	
	return XEE_frameTime;	//返回两次调用之间的时间差
}
