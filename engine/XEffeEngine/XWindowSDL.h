#ifndef _JIA_XWINDOWSDL_
#define _JIA_XWINDOWSDL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XWindowCore.h"
#include "XSDL.h"
//#include "windows.h"
#include "XStringFun.h"
namespace XE{
class XWindowSDL:public XWindowCore
{
public:
	XWindowSDL()
		:m_screen(NULL)
	{}
	virtual ~XWindowSDL() {}
protected:
	XWindowSDL(const XWindowSDL&);
	XWindowSDL &operator= (const XWindowSDL&);

	SDL_Surface *m_screen;
	//bool m_isFullScreen;
public:
	virtual bool createWindow(int width,int height,const char *windowTitle,int isFullScreen,int withFrame);
	virtual void setWindowTitle(const std::string &title) {SDL_WM_SetCaption(XString::ANSI2UTF8(title.c_str()).c_str(), NULL);}
	virtual void setCurcor(bool flag) 
	{
		//SDL_ShowCursor(flag);
		ShowCursor(flag);
	}
	virtual unsigned char *getWindowBuff()
	{
		if(m_screen == NULL) return NULL;
		return (unsigned char *)m_screen->pixels;
	}
	virtual bool getCurcor() 
	{
		return SDL_ShowCursor(-1) == 1;
	}
	virtual void release() 
	{
		SDL_FreeSurface(m_screen);
		SDL_Quit();
	}
	virtual void update()
	{
		//glFlush();
		SDL_GL_SwapBuffers();
	}
	virtual int mapKey(int key);
	//virtual void toggleFullScreen()
	//{
	////	if(m_isFullScreen)
	////	else SDL_WM_ToggleFullScreen(m_screen);
	//	//if(SDL_WM_ToggleFullScreen(m_screen))
	//		m_isFullScreen = !m_isFullScreen;
	//	//else
	//	//{
	//	//	int flags = 0;
	//	//	int bpp = 0;
	//
	//	//	const SDL_VideoInfo* info = SDL_GetVideoInfo();		//获得窗口设备信息
	//	//	bpp = info->vfmt->BitsPerPixel;	//获得显示色深

	//	//	//SDL_NOFRAME没有边框的 |SDL_RESIZABLE
	//	//	if(m_isFullScreen != 0) flags = SDL_RESIZABLE | SDL_OPENGL | SDL_FULLSCREEN;			//设置为全屏
	//	//	else flags = SDL_RESIZABLE | SDL_OPENGL | SDL_DOUBLEBUF;
	//	//	//if(withFrame == 0) flags |= SDL_NOFRAME;
	//	//	//窗口标志符
	//	//	if((m_screen = SDL_SetVideoMode(640,480, bpp, flags)) == NULL) 
	//	//	{//初始化窗口信息
	//	//		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
	//	//	//	LogNull("Video mode set failed: %s\n", SDL_GetError());
	//	//	//	return false;
	//	//	}
	//	//}
	//}
	//virtual bool getIsFullScreen(){return m_isFullScreen;}
};
}
#endif