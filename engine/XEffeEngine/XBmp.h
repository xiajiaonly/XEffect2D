#ifndef _JIA_XBMP_
#define _JIA_XBMP_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#include "XCommonDefine.h"
namespace XE{
#pragma pack(push)
#pragma pack(1)
struct XBMPFileHeader
{
    unsigned short bfType;			//位图文件的类型，必须为BM
    int bfSize;						//位图文件的大小，以字节为单位
    unsigned short bfReserved1;		//位图文件保留字，必须为0
    unsigned short bfReserved2;		//位图文件保留字，必须为0
    int bfOffBits;					//位图数据的起始位置，以相对于位图
    // 文件头的偏移量表示，以字节为单位
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct XBMPInfoHeader
{
    int biSize;                    // 本结构所占用字节数
    int biWidth;                // 位图的宽度，以像素为单位
    int biHeight;                // 位图的高度，以像素为单位
    unsigned short biPlanes;    // 目标设备的级别，必须为1
    unsigned short biBitCount;    // 每个像素所需的位数，必须是1(双色),4(16色)，8(256色)或24(真彩色)之一
    int biCompression;            // 位图压缩类型，必须是 0(不压缩),
    // 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
    int biSizeImage;            // 位图的大小，以字节为单位
    int biXPelsPerMeter;        // 位图水平分辨率，每米像素数
    int biYPelsPerMeter;        // 位图垂直分辨率，每米像素数
    int biClrUsed;                // 位图实际使用的颜色表中的颜色数
    int biClrImportant;            // 位图显示过程中重要的颜色数
};
#pragma pack(pop)

//这里保存的BMP图像使用24bits RGB_888的格式，暂时不考虑其扩展性
class XBMP
{
private:
	static const int m_bmpColorBytes = 3;
	XBool m_isInited;
public:
	int m_width;
	int m_height;
	unsigned char *m_data;
public:
	XBool init(const char *filename, int needAlpha = 0);    //从一个文件中读取图片数据
	XBool init(int width, int height);                        //建立一个空的图片
	XBool writeToFile(const char *filename);                //将图片保存到BMP文件中
	XBool release();                                        //释放所有的资源
	XBMP()
		:m_isInited(XFalse)
		, m_width(0)
		, m_height(0)
		, m_data(NULL)
	{}
};
}
#endif