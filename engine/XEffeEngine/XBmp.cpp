#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
//#include "XBasicOpenGL.h"
#include "XBmp.h"
//#include "string.h"
#include "XPixelCommon.h"
//#include "XBasicFun.h"
namespace XE {
XBool XBMP::init(const char *filename, int)
{
	if (m_isInited) return XFalse;        //防止重复初始化
	XPixels<XCurPixel> tmp;
	if (!tmp.load(filename)) return XFalse;
	m_width = tmp.getWidth();
	m_height = tmp.getHeight();
	//为像素数据分配内存空间
	m_data = XMem::createArrayMem<unsigned char>(m_width * m_height * m_bmpColorBytes);
	if (m_data == NULL) return XFalse;
	//提取像素数据
	unsigned char * tempPixes = tmp.getPixel();
	float tempAlpha = 0.0f;
	float tempFillColor = 0.0f;
	int tempOffsetDst = 0;
	int tempOffsetSrc = 0;
	switch (tmp.getColorMode())
	{
	case COLOR_RGBA:
	case COLOR_BGRA:
		for (int i = 0; i < m_height; ++i)
		{
			for (int j = 0; j < m_width; ++j)
			{
				tempAlpha = (*(tempPixes + (m_height - i - 1) * m_width * 4 + j * 4 + 3) / 256.0f);
				tempFillColor = 255.0f * (1.0f - tempAlpha);
				tempOffsetDst = i * m_width * m_bmpColorBytes + j * m_bmpColorBytes;
				tempOffsetSrc = (m_height - i - 1) * m_width * 4.0f + j * 4.0f;
				*(m_data + tempOffsetDst) = *(tempPixes + tempOffsetSrc) * tempAlpha + tempFillColor;
				*(m_data + tempOffsetDst + 1) = *(tempPixes + tempOffsetSrc + 1) * tempAlpha + tempFillColor;
				*(m_data + tempOffsetDst + 2) = *(tempPixes + tempOffsetSrc + 2) * tempAlpha + tempFillColor;
			}
		}
		break;
	case COLOR_RGB:
	case COLOR_BGR:
		for (int i = 0; i < m_height; ++i)
		{
			for (int j = 0; j < m_width; ++j)
			{
				tempOffsetDst = i * m_width * m_bmpColorBytes + j * m_bmpColorBytes;
				tempOffsetSrc = (m_height - i - 1) * m_width * 4 + j * 4;
				*(m_data + tempOffsetDst) = *(tempPixes + tempOffsetSrc);
				*(m_data + tempOffsetDst + 1) = *(tempPixes + tempOffsetSrc + 1);
				*(m_data + tempOffsetDst + 2) = *(tempPixes + tempOffsetSrc + 2);
			}
		}
		break;
	default:	//其他格式不支持该操作
		tmp.release();
		XMem::XDELETE_ARRAY(m_data);
		return XFalse;
	}
	tmp.release();

	m_isInited = XTrue;
	return XTrue;
}
XBool XBMP::init(int width, int height)
{
	if (m_isInited ||        //防止重复初始化
		width <= 0 || height <= 0) return XFalse;
	m_width = width;
	m_height = height;
	m_data = XMem::createArrayMem<unsigned char>(m_width * m_height * m_bmpColorBytes);
	if (m_data == NULL) return XFalse;
	m_isInited = XTrue;
	return XTrue;
}
XBool XBMP::writeToFile(const char *filename)
{//here write with 24 bits color deep
	if (!m_isInited) return XFalse;
	unsigned char tempData[4] = { 0,0,0,0 };
	int oneLine = m_width * m_bmpColorBytes;
	int needAdd = (4 - ((m_width * m_bmpColorBytes) % 4)) % 4;

	XBMPFileHeader fileHeader;
	memcpy(&(fileHeader.bfType), "BM", 2);
	fileHeader.bfSize = sizeof(XBMPFileHeader) + sizeof(XBMPInfoHeader) + (oneLine + needAdd) * m_height;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(XBMPFileHeader) + sizeof(XBMPInfoHeader);

	XBMPInfoHeader infoHeader;
	infoHeader.biBitCount = 24;
	infoHeader.biClrImportant = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biCompression = 0;
	infoHeader.biWidth = m_width;
	infoHeader.biHeight = m_height;
	infoHeader.biPlanes = 1;
	infoHeader.biSize = sizeof(infoHeader);
	infoHeader.biSizeImage = 0;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	FILE *fp;
	if ((fp = fopen(filename, "wb")) == NULL) return XFalse;
	fwrite(&fileHeader, sizeof(fileHeader), 1, fp);
	fwrite(&infoHeader, sizeof(infoHeader), 1, fp);
	int offset = 0;
	for (int i = 0; i < m_height; ++i)
	{
		fwrite(m_data + offset, oneLine, 1, fp);
		offset += oneLine;
		fwrite(tempData, needAdd, 1, fp);
	}
	fclose(fp);

	return XTrue;
}
XBool XBMP::release()
{
	if (!m_isInited) return XFalse;    //防止重复释放

	m_width = 0;
	m_height = 0;

	XMem::XDELETE_ARRAY(m_data);
	m_isInited = XFalse;
	return XTrue;
}
}