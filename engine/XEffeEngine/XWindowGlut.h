#ifndef _JIA_XWINDOWGLUT_
#define _JIA_XWINDOWGLUT_
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XWindowCore.h"
#include <gl/glut.h>
namespace XE{
class XWindowGlut:public XWindowCore
{
public:
	XWindowGlut()
		:m_isShowCurcor(true)
	{}
	virtual ~XWindowGlut() {}
protected:
	XWindowGlut(const XWindowGlut&);
	XWindowGlut &operator= (const XWindowGlut&);
private:
	bool m_isShowCurcor;
	//bool m_isFullScreen;
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
	virtual unsigned char *getWindowBuff(){return NULL;}
	//virtual void toggleFullScreen()
	//{
	////	if(m_isFullScreen)
	////	else glutFullScreen();
	//	glutFullScreen();
	//	m_isFullScreen = !m_isFullScreen;
	//}
	//virtual bool getIsFullScreen(){return m_isFullScreen;}
};
}
#endif