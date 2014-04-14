#ifndef _JIA_XWINDOWGLUT_
#define _JIA_XWINDOWGLUT_
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XWindowCore.h"
#include <gl/glut.h>

class _XWindowGlut:public _XWindowCore
{
protected:
	_XWindowGlut()
		:m_isShowCurcor(true)
	{}
	_XWindowGlut(const _XWindowGlut&);
	_XWindowGlut &operator= (const _XWindowGlut&);
	virtual ~_XWindowGlut() {}
public:
	static _XWindowGlut& GetInstance()
	{
		static _XWindowGlut m_instance;
		return m_instance;
	}
private:
	bool m_isShowCurcor;
public:
	virtual bool createWindow(int width,int height,const char *windowTitle,int isFullScreen,int withFrame);
	virtual void setWindowTitle(const std::string &title) {glutSetWindowTitle(title.c_str());}
	virtual void setCurcor(bool flag) 
	{
		m_isShowCurcor = flag;
		glutSetCursor(flag);
	}
	virtual bool getCurcor(){return m_isShowCurcor;}
	virtual void release() {}
	virtual void update() {glutSwapBuffers();}
	virtual int mapKey(int key);
};
#endif