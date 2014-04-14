#ifndef _JIA_XPIXELCORE_
#define _JIA_XPIXELCORE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include <string>
#include "XBasicFun.h"

enum _XColorMode;

class _XPixelCore
{
protected:
	unsigned char * m_pPixels;	//像素数据
	int w;
	int h;
	_XColorMode m_colorMode;
	bool isInited;
public:
	_XPixelCore()
		:isInited(false)
		,m_pPixels(NULL)
	{}
	virtual ~_XPixelCore() {release();}
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

	virtual bool load(const std::string &filename,_XResourcePosition resPos = RESOURCE_SYSTEM_DEFINE) = 0;	//从文件中载入
	//virtual bool loaf(const unsigned char * data) = 0;	//从数据中载入
	virtual bool fitNPot() = 0;	//自适应为2的n次方的数据
	virtual bool create(int width,int height,_XColorMode mode = COLOR_RGB) = 0;	//建立一个空白的数据
};
template<typename T>
class _XPixels
{
private:
	_XPixelCore *m_pObj;
public:
	_XPixels()
	{
		m_pObj = createMem<T>();
	}
	~_XPixels()
	{
		XDELETE(m_pObj);
	}
	virtual void release() {m_pObj->release();}
	virtual unsigned char * getPixel() {return m_pObj->getPixel();}
	virtual int getWidth() const {return m_pObj->getWidth();}
	virtual int getHeight() const {return m_pObj->getHeight();}
	virtual _XColorMode getColorMode() const {return m_pObj->getColorMode();}
	virtual bool getIsInited() const {return m_pObj->getIsInited();}

	virtual bool load(const std::string &filename,_XResourcePosition resPos = RESOURCE_SYSTEM_DEFINE)
	{
		return m_pObj->load(filename,resPos);
	}
	//virtual bool loaf(const unsigned char * data)
	//{
	//	return m_pObj->load(data);
	//}
	virtual bool fitNPot() {return m_pObj->fitNPot();}	//自适应为2的n次方的数据
	virtual bool create(int width,int height,_XColorMode mode = COLOR_RGB) {return m_pObj->create(width,height,mode);}
};

#endif