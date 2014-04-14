//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMaskTex.h"
#include "XEffeEngine.h"
#include "XPixelCommon.h"

_XBool _XMaskTex::init(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(filename == NULL) return XFalse;
	_XPixels<_XCurPixel> tmpPixels;
	if(!tmpPixels.load(filename,resoursePosition)) return XFalse;

	if(tmpPixels.getColorMode() != COLOR_RGBA) 
	{
		tmpPixels.release();
		return XFalse;	//必须带有alpha通道
	}

	m_sourcePixelSize.x = tmpPixels.getWidth();
	m_sourcePixelSize.y = tmpPixels.getHeight();
	if((tmpPixels.getWidth() % 8) == 0) m_nowPixelSize.x = tmpPixels.getWidth();
	else m_nowPixelSize.x = (((tmpPixels.getWidth() >> 3) + 1) << 3);
	if((tmpPixels.getHeight() % 8) == 0) m_nowPixelSize.y = tmpPixels.getHeight();
	else m_nowPixelSize.y = (((tmpPixels.getHeight() >> 3) + 1) << 3);

	m_pixelData = createArrayMem<unsigned char>((int)(m_nowPixelSize.x * m_nowPixelSize.y) >> 3);
	memset(m_pixelData,0,(int)(m_nowPixelSize.x * m_nowPixelSize.y) >> 3);
	//根据数值赋值
	unsigned char * pData = tmpPixels.getPixel();
	int index,bitIndex;
	for(int h = 0;h < tmpPixels.getHeight();++ h)
	{
		for(int w = 0;w < tmpPixels.getWidth();++ w)
		{
			index = h * ((int)(m_nowPixelSize.x) >> 3) + (w >> 3);
			bitIndex = w % 8;
			if(*(pData + (h * tmpPixels.getWidth() + w) * 4 + 3) > 127) setBit(m_pixelData[index],bitIndex,1);
			else setBit(m_pixelData[index],bitIndex,0);
		}
	}
	tmpPixels.release();
	m_position.set(0.0f,0.0f);
	m_size.set(1.0f,1.0f);
	m_rect.set(0.0f,0.0f,m_sourcePixelSize.x,m_sourcePixelSize.y);

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XMaskTex::getIsMask(float x,float y) const
{
	if(!m_isInited) return XTrue;
	if(m_rect.isInRect(x,y))
	{
		int index = (int)((x - m_rect.left) / m_size.x + (y - m_rect.top) / m_size.y * m_nowPixelSize.x) >> 3;
		int bitIndex = (int)((x - m_rect.left) / m_size.x) % 8;
		if(getBit(m_pixelData[index],bitIndex) == 0) return XFalse;
		else return XTrue;
	}else
	{
		return XFalse;
	}
}
//void _XMaskTex::draw()	//用于测试
//{
//	if(m_isInited == 0) return 0;
//	int index,bitIndex;
//	for(int h = 0;h < m_nowPixelSize.y;++ h)
//	{
//		for(int w = 0;w < m_nowPixelSize.x;++ w)
//		{
//			if(getIsMask(w,h) == 0) drawLine(w,h,w + 1,h + 1);
//			else drawLine(w,h,w + 1,h + 1,1,0,0,0);
//		}
//	}
//}