#ifndef _JIA_XPIXELSDL_
#define _JIA_XPIXELSDL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XPixelCore.h"
//
namespace XE{
class XPixelSDL :public XPixelCore
{
public:
	XPixelSDL() {}
	virtual ~XPixelSDL() { release(); }
	virtual void release()
	{
		if (!m_isInited) return;
		XMem::XDELETE_ARRAY(m_pPixels);
		m_isInited = false;
	}
	virtual unsigned char * getPixel() { return m_pPixels; }
	virtual int getWidth() const { return w; }
	virtual int getHeight() const { return h; }
	virtual XVec2 getPixelSize() const { return XVec2(w, h); }
	virtual XColorMode getColorMode() const { return m_colorMode; }
	virtual bool getIsInited() const { return m_isInited; }
	virtual int getBytePerPixel()const
	{
		return getBytePerPixelByColorMode(m_colorMode);
	}
	virtual int getPixelIndex(int x, int y)const { return (x + y * w) * getBytePerPixel(); }
	virtual int getAllPixelsSum()const { return w * h; }
	virtual int getAllBuffSize()const { return w * h * getBytePerPixel(); }
private:
	bool load_(void *);	//这个是内部调用的接口
public:
	virtual bool load(const std::string& filename, XResPos resPos = RES_SYS_DEF);
	virtual bool load(const XBuffer &buff);
	virtual bool fitNPot();
	virtual bool create(int width, int height, XColorMode mode = COLOR_RGB)
	{
		if (m_isInited ||
			width <= 0 || height <= 0) return false;
		w = width;
		h = height;
		m_colorMode = mode;

		switch (mode)
		{
		case COLOR_RGBA:
		case COLOR_BGRA:
			m_pPixels = XMem::createArrayMem<unsigned char>((w * h) << 2);	//像素数据
			break;
		case COLOR_RGB:
		case COLOR_BGR:
			m_pPixels = XMem::createArrayMem<unsigned char>(w * h * 3);	//像素数据
			break;
		case COLOR_GRAY:
			m_pPixels = XMem::createArrayMem<unsigned char>(w * h);	//像素数据
			break;
		default:	//其他格式不支持
			return false;
			break;
		}
		if (m_pPixels == NULL) return false;

		m_isInited = true;
		return true;
	}
};
}
#endif