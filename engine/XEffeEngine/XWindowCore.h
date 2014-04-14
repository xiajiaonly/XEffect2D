#ifndef _JIA_XWINDOWCORE_
#define _JIA_XWINDOWCORE_
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include <string>

class _XWindowCore
{
public:
	virtual ~_XWindowCore() {;}
	virtual bool createWindow(int width,int height,const char *windowTitle,int isFullScreen,int withFrame) = 0;
	virtual void setWindowTitle(const std::string &title) = 0;
	virtual void setCurcor(bool flag) = 0;
	virtual bool getCurcor() = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual int mapKey(int key) = 0;
};
#endif