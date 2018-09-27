#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMaskTex.h"
#include "XPixelCommon.h"
#include "XMath/XByteFun.h"
namespace XE{
XBool XMaskTex::init(const char *filename,XResPos resPos)
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	XPixels<XCurPixel> tmpPixels;
	if(!tmpPixels.load(filename,resPos)) return XFalse;

	if(tmpPixels.getColorMode() != COLOR_RGBA) 
	{
		tmpPixels.release();
		return XFalse;	//必须带有alpha通道
	}

	m_sourcePixelSize.x = tmpPixels.getWidth();
	m_sourcePixelSize.y = tmpPixels.getHeight();
	if((tmpPixels.getWidth() % 8) == 0) m_curPixelSize.x = tmpPixels.getWidth();
	else m_curPixelSize.x = (((tmpPixels.getWidth() >> 3) + 1) << 3);
	if((tmpPixels.getHeight() % 8) == 0) m_curPixelSize.y = tmpPixels.getHeight();
	else m_curPixelSize.y = (((tmpPixels.getHeight() >> 3) + 1) << 3);

	m_pixelData = XMem::createArrayMem<unsigned char>((int)(m_curPixelSize.x * m_curPixelSize.y) >> 3);
	if(m_pixelData == NULL) return XFalse;
	memset(m_pixelData,0,(int)(m_curPixelSize.x * m_curPixelSize.y) >> 3);
	//根据数值赋值
	unsigned char * pData = tmpPixels.getPixel();
	int index,bitIndex;
	for(int h = 0;h < tmpPixels.getHeight();++ h)
	{
		for(int w = 0;w < tmpPixels.getWidth();++ w)
		{
			index = h * ((int)(m_curPixelSize.x) >> 3) + (w >> 3);
			bitIndex = w % 8;
			if(*(pData + (h * tmpPixels.getWidth() + w) * 4 + 3) > 127) XByte::setBit(m_pixelData[index],bitIndex,1);
			else XByte::setBit(m_pixelData[index],bitIndex,0);
		}
	}
	tmpPixels.release();
	m_position.reset();
	m_scale.set(1.0f);
	m_rect.set(XVec2::zero,m_sourcePixelSize);

	m_isInited = XTrue;
	return XTrue;
}
XBool XMaskTex::getIsMask(const XVec2& p) const
{
	if(!m_isInited) return XTrue;
	if(!m_rect.isInRect(p)) return XFalse;
	int index = (int)((p.x - m_rect.left) / m_scale.x + (p.y - m_rect.top) / m_scale.y * m_curPixelSize.x) >> 3;
	int bitIndex = (int)((p.x - m_rect.left) / m_scale.x) % 8;
	return !(XByte::getBit(m_pixelData[index],bitIndex) == 0);
}
//void XMaskTex::draw()	//用于测试
//{
//	if(m_isInited == 0) return 0;
//	int index,bitIndex;
//	for(int h = 0;h < m_curPixelSize.y;++ h)
//	{
//		for(int w = 0;w < m_curPixelSize.x;++ w)
//		{
//			if(getIsMask(w,h) == 0) drawLine(w,h,w + 1,h + 1);
//			else drawLine(w,h,w + 1,h + 1,1,0,0,0);
//		}
//	}
//}
}