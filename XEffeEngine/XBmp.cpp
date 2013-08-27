//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBmp.h"
#include "string.h"
#include "XBasicSDL.h"
#include "XBasicFun.h"

int _XBMP::init(const char *filename,int needAlpha)
{
    if(m_isInited != 0) return 0;        //防止重复初始化
    //SDL_Surface *tempPic0;
    //SDL_Surface *tempPic;
    //tempPic0 = loadImage(filename,needAlpha);
    //tempPic = zoomSurface(tempPic0,0.5,0.5,1);
    SDL_Surface *tempPic;
    tempPic = loadImage(filename,needAlpha);
    if(tempPic == NULL) return 0;
    m_width = tempPic->w;
    m_height = tempPic->h;
    //为像素数据分配内存空间
    try
    {
        m_data = new unsigned char[m_width * m_height * _XBMP_COLOR_BYTES];
        if(m_data == NULL) return 0;
    }catch(...)
    {
        return 0;
    }
    //提取像素数据
    unsigned char * tempPixes = (unsigned char *)(tempPic->pixels);
    float tempAlpha = 0.0f;
    float tempFillColor = 0.0f;
    int tempOffsetDst = 0;
    int tempOffsetSrc = 0;
    if(tempPic->format->BitsPerPixel == 32)
    {//32 bits color deep(对于存在透明通道的图像使用白色衬底来填用透明或者半透明部分)
        for(int i = 0;i < m_height;++ i)
        {
            for(int j = 0 ;j < m_width;++ j)
            {
                tempAlpha = (*(tempPixes + (m_height - i - 1) * m_width * 4 + j * 4 + 3) / 256.0f);
                tempFillColor = 255.0f * (1.0f - tempAlpha);
                tempOffsetDst = i * m_width * _XBMP_COLOR_BYTES + j * _XBMP_COLOR_BYTES;
                tempOffsetSrc = (m_height - i - 1) * m_width * 4.0f + j * 4.0f;
                *(m_data + tempOffsetDst) = *(tempPixes + tempOffsetSrc) * tempAlpha + tempFillColor; 
                *(m_data + tempOffsetDst + 1) = *(tempPixes + tempOffsetSrc + 1) * tempAlpha + tempFillColor; 
                *(m_data + tempOffsetDst + 2) = *(tempPixes + tempOffsetSrc + 2) * tempAlpha + tempFillColor; 
            }
        }
    }else
    if(tempPic->format->BitsPerPixel == 24)
    {//24 bits color deep
        for(int i = 0;i < m_height;++ i)
        {
            for(int j = 0 ;j < m_width;++ j)
            {
                tempOffsetDst = i * m_width * _XBMP_COLOR_BYTES + j * _XBMP_COLOR_BYTES;
                tempOffsetSrc = (m_height - i - 1) * m_width * 4 + j * 4;
                *(m_data + tempOffsetDst) = *(tempPixes + tempOffsetSrc); 
                *(m_data + tempOffsetDst + 1) = *(tempPixes + tempOffsetSrc + 1); 
                *(m_data + tempOffsetDst + 2) = *(tempPixes + tempOffsetSrc + 2); 
            }
        }
    }
    //释放临时资源
    SDL_FreeSurface(tempPic);
	tempPic = NULL;
    //SDL_FreeSurface(tempPic0);
	//tempPic0 = NULL;

    m_isInited = 1;
    return 1;
}


int _XBMP::init(int width,int height)
{
    if(m_isInited != 0) return 0;        //防止重复初始化
    if(width <= 0 || height <= 0) return 0;
    m_width = width;
    m_height = height;
    try
    {
        m_data = new unsigned char[m_width * m_height * _XBMP_COLOR_BYTES];
        if(m_data == NULL) return 0;
    }catch(...)
    {
        return 0;
    }
    m_isInited = 1;
    return 1;
}

int _XBMP::writeToFile(const char *filename)
{//here write with 24 bits color deep
	if(m_isInited == 0) return 0;
    unsigned char tempData[4] ={0};
    int oneLine = m_width * _XBMP_COLOR_BYTES;
    int needAdd = (4 - ((m_width * _XBMP_COLOR_BYTES) % 4)) % 4;

    _XBMPFileHeader fileHeader;
    memcpy(&(fileHeader.bfType),"BM",2);
    fileHeader.bfSize = sizeof(_XBMPFileHeader) + sizeof(_XBMPInfoHeader) + (oneLine + needAdd) * m_height;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfOffBits = sizeof(_XBMPFileHeader) + sizeof(_XBMPInfoHeader);

    _XBMPInfoHeader infoHeader;
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
    if((fp = fopen(filename,"wb")) == NULL)
    {
        return 0;
    }
    fwrite(&fileHeader,sizeof(fileHeader),1,fp);
    fwrite(&infoHeader,sizeof(infoHeader),1,fp);
    int offset = 0;
    for(int i = 0;i < m_height;++ i)
    {
        fwrite(m_data + offset,oneLine,1,fp);
        offset += oneLine;
        fwrite(tempData,needAdd,1,fp);
    }
    fclose(fp);

    return 1;
}

int _XBMP::release()
{
    if(m_isInited == 0) return 0;    //防止重复释放

    m_width = 0;
    m_height = 0;

	XDELETE_ARRAY(m_data);
    m_isInited = 0;
    return 1;
}