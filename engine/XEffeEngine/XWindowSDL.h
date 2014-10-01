#ifndef _JIA_XWINDOWSDL_
#define _JIA_XWINDOWSDL_
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XWindowCore.h"
#include "XSDL.h"
#include "windows.h"
#include "XStringFun.h"

class _XWindowSDL:public _XWindowCore
{
protected:
	_XWindowSDL()
		:m_screen(NULL)
	{}
	_XWindowSDL(const _XWindowSDL&);
	_XWindowSDL &operator= (const _XWindowSDL&);
	virtual ~_XWindowSDL() {}

	SDL_Surface *m_screen;
public:
	static _XWindowSDL& GetInstance()
	{
		static _XWindowSDL m_instance;
		return m_instance;
	}
public:
	virtual bool createWindow(int width,int height,const char *windowTitle,int isFullScreen,int withFrame);
	virtual void setWindowTitle(const std::string &title) {SDL_WM_SetCaption(ANSI2UTF8(title.c_str()).c_str(), NULL);}
	virtual void setCurcor(bool flag) 
	{
		//SDL_ShowCursor(flag);
		ShowCursor(flag);
	}
	virtual bool getCurcor() 
	{
		if(SDL_ShowCursor(-1)) return true;
		else return false;
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
};
#endif