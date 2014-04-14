#ifndef _JIA_XCOLORSPACE_
#define _JIA_XCOLORSPACE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.4.15
//--------------------------------
//这里封装了各种颜色空间的转换函数
#include "stdlib.h"

//HSV	H(0 - 6) S(0 - 1) V(0 - 1)
extern void HSV2RGB(unsigned char *color,float H,float S,float V);
extern void RGB2HSV(unsigned char *color,float& H,float& S,float&V);
extern float HueToRGB (float n1,float n2,float hue);
extern void YUV2RGB(unsigned char *color,unsigned char Y,unsigned char U,unsigned char V);
extern void YUVToRGB(unsigned char *pYUV, unsigned char *pRGB,int width,int height);
extern void YUV420P2RGB(unsigned char *Y,unsigned char *U,unsigned char *V, unsigned char *pRGB,int width,int height);	//尚未完成
extern void RGB2YUV420(unsigned char *Y,unsigned char *U,unsigned char *V, unsigned char *pRGB,int width,int height);	
#endif