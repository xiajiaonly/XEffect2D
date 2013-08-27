//********************************************************************************
//下面是一个程序的框架，框架包括窗口的建立，鼠标键盘事件的处理部分以及图像渲染部分
//program by 贾胜华
//********************************************************************************

#include "XEffeEngine.h"

#ifdef OS_WINDOWS
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	//隐藏控制台的编译说明
#endif

//下面是事件处理函数，例如：键盘事件或者鼠标事件
int SDL_event(SDL_Surface *screen);

int main(int argc, char **argv)
{
	Uint32 interval;	//两次调用之间的时间差			
	SDL_Surface *screen = NULL;	//窗口绘图面
	int quit = 0;
	//建立窗体
	screen = initWindow(1024,768,"Xiajia");	
	if(screen == NULL)
	{
		printf("Window init error!\n");
		return 0;
	}
	while (!quit) 
	{
		quit = SDL_event(screen);
		interval = getFrameTiming();
		
		engineMove(interval);	//引擎的更新内容
		clearScreen();	//清除屏幕

		updateScreen();	//更新屏幕的内容
		
		SDL_Delay(1);
	}

	releaseSDL();
	return 0;	
}

int SDL_event(SDL_Surface *screen)
{
	int flag =0;
	SDL_Event event;		//SDL事件句柄

	while(SDL_PollEvent(&event)) 
	{
		switch(event.type)
		{
		case SDL_QUIT:
			flag = 1;
			break;
		case SDL_KEYUP:
			break;
		case SDL_KEYDOWN:

			switch(event.key.keysym.sym )
			{
			case SDLK_s:
				break;
			case SDLK_LALT:
				break;
			case SDLK_RALT:
				break;
			case SDLK_F1:
				SDL_WM_ToggleFullScreen(screen);
				break;
			case SDLK_RIGHT:
				break;
			case SDLK_LEFT:
				break;
			case SDLK_UP:
				break;
			case SDLK_DOWN:
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == 1)
			{//左键
			}else
			if(event.button.button == 3)
			{//右键
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == 1)
			{//左键
			}else
			if(event.button.button == 3)
			{//右键
			}
			break;
		}
		inputEvent(event);
	}
	return flag;
}