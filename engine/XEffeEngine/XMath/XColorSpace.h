#ifndef _JIA_XCOLORSPACE_
#define _JIA_XCOLORSPACE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.4.15
//--------------------------------
//这里封装了各种颜色空间的转换函数
namespace XE{
namespace XColorSpace
{//颜色空间的转换
	//HSV	H(0 - 6) S(0 - 1) V(0 - 1)
	extern void HSV2RGB(unsigned char *color,float H,float S,float V);
	extern void RGB2HSV(unsigned char *color,float& H,float& S,float&V);
	extern float HueToRGB (float n1,float n2,float hue);
	extern void YUV2RGB(unsigned char *color,unsigned char Y,unsigned char U,unsigned char V);
	extern void YUVToRGB(unsigned char *pYUV, unsigned char *pRGB,int width,int height);
	extern void YUV420P2RGB(unsigned char *Y,unsigned char *U,unsigned char *V, unsigned char *pRGB,int width,int height);	//尚未完成
	extern void RGB2YUV420(unsigned char *Y,unsigned char *U,unsigned char *V, unsigned char *pRGB,int width,int height);	
	//根据角度计算色轮上的颜色[0-360]
	extern XFColor getWheelColor(float angle);	//角度的单位为度，色轮的0度方向为红色，颜色为红绿蓝的顺序 
}
}
#endif