#ifndef _JIA_XFCOLOR_
#define _JIA_XFCOLOR_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//#include "XBasicFun.h"
#include "XRandomFun.h"
namespace XE{
class XCColor;
//浮点数的颜色 0.0 - 1.0
class XFColor
{
//private:
public:
    float fR;    //0 - 1
    float fG;
    float fB;
    float fA;
	float fW;	//亮度
public:
	XFColor(const XCColor &temp);
	XFColor& operator = (const XCColor& temp);

    XFColor()
        :fR(0.0f),fG(0.0f),fB(0.0f),fA(0.0f)
    {}
	void calculateW(){fW = XEE_Max(fR,XEE_Max(fG,fB));}	//计算亮度，在必要的时候计算亮度
	//XFColor(float r,float g,float b,float a)
	//{
	//	if(r < 0.0f) r = 0.0f;
	//	if(r > 1.0f) r = 1.0f;
	//	if(g < 0.0f) g = 0.0f;
	//	if(g > 1.0f) g = 1.0f;
	//	if(b < 0.0f) b = 0.0f;
	//	if(b > 1.0f) b = 1.0f;
	//	if(a < 0.0f) a = 0.0f;
	//	if(a > 1.0f) a = 1.0f;
	//	fR = r;fG = g;fB = b;fA = a;
	//}
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}
	XFColor(float r,float g,float b,float a = 1.0f)
		:fR(r),fG(g),fB(b),fA(a)
	{}
	XFColor(unsigned int color)
	{
		fR = (color >> 24) / 255.0f;
		fG = ((color >> 16) % 256) / 255.0f;
		fB = ((color >> 8) % 256) / 255.0f;
		fA = (color % 256) / 255.0f;
	}
    void setColor(float r,float g,float b,float a);
	XFColor operator * (const XFColor& temp) const;
	void setR(float r);
	void setG(float g);
	void setB(float b);
	void setA(float a);
	float getR() const;
	float getG() const;
	float getB() const;
	float getA() const;
	void randColor();
	float limit() const {return 1.0f;}
	XFColor anti(){return XFColor(1.0f - fR,1.0f - fG,1.0f - fB,fA);}
	static const XFColor white, gray, black, red, green, blue, cyan, magenta,
        yellow,aliceBlue,antiqueWhite,aqua,aquamarine,azure,beige,bisque,blanchedAlmond,
        blueViolet,brown,burlyWood,cadetBlue,chartreuse,chocolate,coral,cornflowerBlue,cornsilk,
        crimson,darkBlue,darkCyan,darkGoldenRod,darkGray,darkGrey,darkGreen,darkKhaki,
        darkMagenta,darkOliveGreen,darkorange,darkOrchid,darkRed,darkSalmon,darkSeaGreen,
        darkSlateBlue,darkSlateGray,darkSlateGrey,darkTurquoise,darkViolet,deepPink,
        deepSkyBlue,dimGray,dimGrey,dodgerBlue,fireBrick,floralWhite,forestGreen,fuchsia,
        gainsboro,ghostWhite,gold,goldenRod,grey,greenYellow,honeyDew,hotPink,indianRed,indigo,
        ivory,khaki,lavender,lavenderBlush,lawnGreen,lemonChiffon,lightBlue,lightCoral,
        lightCyan,lightGoldenRodYellow,lightGray,lightGrey,lightGreen,lightPink,lightSalmon,
        lightSeaGreen,lightSkyBlue,lightSlateGray,lightSlateGrey,lightSteelBlue,lightYellow,
        lime,limeGreen,linen,maroon,mediumAquaMarine,mediumBlue,mediumOrchid,mediumPurple,
        mediumSeaGreen,mediumSlateBlue,mediumSpringGreen,mediumTurquoise,mediumVioletRed,
        midnightBlue,mintCream,mistyRose,moccasin,navajoWhite,navy,oldLace,olive,oliveDrab,
        orange,orangeRed,orchid,paleGoldenRod,paleGreen,paleTurquoise,paleVioletRed,papayaWhip,
        peachPuff,peru,pink,plum,powderBlue,purple,rosyBrown,royalBlue,saddleBrown,salmon,
        sandyBrown,seaGreen,seaShell,sienna,silver,skyBlue,slateBlue,slateGray,slateGrey,snow,
        springGreen,steelBlue,tan,teal,thistle,tomato,turquoise,violet,wheat,whiteSmoke,
        yellowGreen;
};
#if WITH_INLINE_FILE
#include "XFColor.inl"
#endif
namespace XColor{
//两种颜色之间进行差值，rate为差值的位置(0.0 - 1.0)之间
inline XFColor getInterpolationColor(const XFColor &color1,const XFColor &color2,float rate)	//rate为0.0f - 1.0f
{
	return XFColor(color1.fR + (color2.fR - color1.fR) * rate,
		color1.fG + (color2.fG - color1.fG) * rate,
		color1.fB + (color2.fB - color1.fB) * rate,
		color1.fA + (color2.fA - color1.fA) * rate);
}
//两种颜色按比例混合的计算公式：rate1 + rate = 1
extern XFColor colorMix(const XFColor &color1,const XFColor &color2,float rate1,float rate2);
//四种颜色按比例混合的计算公式
extern XFColor colorMix(const XFColor &color1,const XFColor &color2,const XFColor &color3,const XFColor &color4,
						 float rate1,float rate2,float rate3,float rate4);
//四种颜色混合，不单独计算明度
extern XFColor colorMixEx(const XFColor &color1,const XFColor &color2,const XFColor &color3,const XFColor &color4,
						 float rate1,float rate2,float rate3,float rate4);
//多种颜色混合，目前尚未实现
extern XFColor colorMix(const XFColor *color,const float *rate,int mixSum);	//需要注意参数的匹配问题
extern XFColor colorMixEx(const XFColor *color,const float *rate,int mixSum);	//需要注意参数的匹配问题
}
}
#endif