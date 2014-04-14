#ifndef _JIA_XPIXELSDL_
#define _JIA_XPIXELSDL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XPixelCore.h"
#include "XBasicWindow.h"

class _XPixelSDL:public _XPixelCore
{
public:
	_XPixelSDL(){}
	virtual ~_XPixelSDL() {release();}
	virtual void release()
	{
		if(!isInited) return;
		XDELETE_ARRAY(m_pPixels);
		isInited = false;
	}
	virtual unsigned char * getPixel() {return m_pPixels;}
	virtual int getWidth() const {return w;}
	virtual int getHeight() const {return h;}
	virtual _XColorMode getColorMode() const {return m_colorMode;}
	virtual bool getIsInited() const {return isInited;}

	virtual bool load(const std::string &filename,_XResourcePosition resPos = RESOURCE_SYSTEM_DEFINE);
	//virtual bool loaf(const unsigned char * data)
	//{
	//	return true;
	//}
	virtual bool fitNPot();
	virtual bool create(int width,int height,_XColorMode mode = COLOR_RGB)
	{
		if(isInited) return false;
		if(width <= 0 || height <= 0) return false;
		w = width;
		h = height;
		m_colorMode = mode;

		switch(mode)
		{
		case COLOR_RGBA:
		case COLOR_BGRA:
			m_pPixels = createArrayMem<unsigned char>(w * h * 4);	//像素数据
			break;
		case COLOR_RGB:
		case COLOR_BGR:
			m_pPixels = createArrayMem<unsigned char>(w * h * 3);	//像素数据
			break;
		case COLOR_GRAY:
			m_pPixels = createArrayMem<unsigned char>(w * h);	//像素数据
			break;
		}
		if(m_pPixels == NULL) return false;

		isInited = true;
		return true;
	}
};

#endif