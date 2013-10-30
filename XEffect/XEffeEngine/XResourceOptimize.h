#ifndef _JIA_XRESOURCEOPTIMIZE_
#define _JIA_XRESOURCEOPTIMIZE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//#include "XTextureInformation.h"

#define PNG_FILE_LIST_NAME "pngFileList.txt"
#define PNG_INFORMATION_FILE_NAME "textureInformation.txt"

//资源优化和打包统一处理之后造成优化的最后一张图不够优化，这个需要后面处理
#define MAX_PIXELS_OFFSET (0)	//图片优化时允许的图片像素的容差
extern int ResourceOptimize();
extern void releaseTempFile();	//处理掉临时文件
extern void clearTempFile();	//处理掉临时文件
extern int pixelCompare(const unsigned int *p0,const unsigned int *p1,int maxPixelsOffset);

#endif