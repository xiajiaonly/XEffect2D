#ifndef _JIA_XBASICOPENGL_
#define _JIA_XBASICOPENGL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "glew.h"
#include "gl.h"
//#include "stdlib.h"
#include "XBasicFun.h"
#include "XMath/XVector2.h"
#include "XMath/XRect.h"
#include "XMath/XFColor.h"
//#include "XBasicFun.h"
#include "XBasicClass.h"
//#include "string.h"
#include "XMemory.h"
#ifdef _WIN64
#pragma comment(lib, "glew64s.lib") 
#else
#pragma comment(lib, "glew32s.lib") 
#endif
#pragma comment(lib, "opengl32.lib") 
namespace XE{
namespace XGL
{
	//基本内部格式			红色成分	绿色成分	蓝色成分	alpha成分
	//GL_ALPHA				不改变		不改变		不改变		At
	//GL_LUMINANCE			Lt			Lt			Lt			不改变
	//GL_LUMINANCE_ALPHA	Lt			Lt			Lt			At
	//GL_INTENSITY			It			It			It			It
	//GL_RGB				Rt			Gt			Bt			不改变
	//GL_RGBA				Rt			Gt			Bt			At
	enum XBlendType
	{
		BLEND_TWO_DATA,		//两个因子的混合
		BLEND_FOUR_DATA,	//四个因子的混合
	};
	extern const int srcBlendMode[9];
	extern const int dstBlendMode[8];

	extern void printShaderInfoLog(GLuint obj);						//输出shader的信息的函数
	extern int printOglError(const char *file, int line);					//显示OpenGL的错误信息
	#define printOpenGLError() printOglError(__FILE__, __LINE__)	//打印程序执行错误的函数
	extern void printProgramInfoLog(GLuint obj);					//打印程序信息的函数

	extern XBool setShader(const char* vertFile,const char* fragFile,int &pHandle,
							XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//资源位置，0为外部资源，默认为外部资源
	extern XBool setShaderFromStr(const char* vStr,const char *fStr,const char *gStr,XShaderHandle &pHandle);	//直接从字符串中加载shader
	extern XBool setShader(const char* vertFile,const char* fragFile,const char* geomFile,XShaderHandle &pHandle,
							XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//资源位置，0为外部资源，默认为外部资源
	//extern void changeSize(int h, int w);							//SDL中窗口改变事件的响应函数
	//extern XBool TextureLoad(GLuint &texture,const char *filename,int withAlpha,int * w = NULL,int * h = NULL,
	//					   XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//使用SDL载入贴图,默认的资源位置为外部
	extern XBool TextureLoadEx(GLuint &texture,const char *filename,int * w = NULL,int * h = NULL,
							 XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//使用SDL载入贴图,默认的资源位置为外部
	//这是一个没有进行更多检查的函数接口，调用之前确认资源符合各种约束
	extern XBool TextureLoadQuarter(GLuint &texture,const char *filename);	//以四分之一尺寸读取资源
	//建立一张空白贴图0:RGBA,1:RGB,2:灰度图
	extern GLuint emptyTexture(int x, int y, XColorMode mode = COLOR_RGBA);	//使用RGBA的格式建立一张指定大小的空白贴图
	extern void showGLInfoToLog();
	//下面需要对GL进行优化(尚未完成，判断状态是否需要改变再对状态进行改变,目前尚不明确这一步的工作能带来多大的性能优化)
	//cocos2d进行了该项优化,可以借鉴.
	//说明：gl自带有函数来获取gl的状态
	//glGetIntegerv(GL_TEXTURE_BINDING_2D,&t);
	//glIsEnabled(GL_TEXTURE_2D);等，但是这些函数的消耗都比较大
	//+++++++++++++++++++++++++++++++++
	//需要优化的函数有
	//glEnable(GL_TEXTURE_2D);系列
	//glBindTexture(GL_TEXTURE_2D,t);系列
	//glEnable(Blend);系列
	//矩阵系列
	//---------------------------------
	//说明：这一部分暂时尚未生效，因为没有处理glPopAttrib系列函数，造成逻辑错误
//	extern bool blendFlag;					//blend的状态
//	extern int blendSrc;					//blend的参数src
//	extern int blendDst;					//blend的参数sst
//	extern bool texture2DFlag;				//当前Texture2D的状态
//	extern unsigned int curTextureID[32];	//当前绑定的贴图的ID
//	inline bool GetIsTexture2D(){return texture2DFlag;}
	inline void EnableTexture2D()
	{
//		if(texture2DFlag)
//		{//有问题
//			bool flag = glIsEnabled(GL_TEXTURE_2D);
//			if(texture2DFlag != flag) printf("EnableTexture2D error!\n");
//			return;
//		}
//		texture2DFlag = true;
		glEnable(GL_TEXTURE_2D);
	}
	inline void DisableTexture2D()
	{
//		if(!texture2DFlag)
//		{
//			bool flag = glIsEnabled(GL_TEXTURE_2D);
//			if(texture2DFlag != flag) printf("DisableTexture2D error!\n");
//			return;
//		}
//		texture2DFlag = false;
		glDisable(GL_TEXTURE_2D);
	}
//	inline bool GetIsBlend(){return blendFlag;}
	inline void EnableBlend()
	{
//		if(blendFlag)
//		{//没问题
//			bool flag = glIsEnabled(GL_BLEND);
//			if(blendFlag != flag) printf("EnableBlend error!\n");
//			return;
//		}
//		blendFlag = true;
		glEnable(GL_BLEND);
	}
	inline void DisableBlend()
	{
//		if(!blendFlag)
//		{//没问题
//			bool flag = glIsEnabled(GL_BLEND);
//			if(blendFlag != flag) printf("DisableBlend error!\n");
//			return;
//		}
//		blendFlag = false;
		glDisable(GL_BLEND);
	}
//	inline int GetBlendSrc(){return blendSrc;}
//	inline int GetBlendDst(){return blendDst;}
	inline void SetBlendFunc(int src,int dst)
	{//这里存在问题在流水项目中可以检测出来，这里开或者不开会有不同
//		if(blendSrc == src && blendDst == dst)
//		{//有问题
//			int tmpS,tmpD;
//			glGetIntegerv(GL_BLEND_SRC,&tmpS);
//			glGetIntegerv(GL_BLEND_DST,&tmpD);
//			return;
//		}
//		blendSrc = src;
//		blendDst = dst;
		glBlendFunc(src,dst);
	}
	inline void BindTexture2D(const unsigned int &t,const int &index = 0)
	{
	//	if(curTextureID[index] == t)
	//	{
	//		if(texture2DFlag != flag) printf("BindTexture2D error!\n");
	//		return;
	//	}
	//	curTextureID[index] = t;
		glBindTexture(GL_TEXTURE_2D,t);
	}
//	inline unsigned int getCurBindTexture2D(const int &index = 0){return curTextureID[index];}
	inline void initOpenGLState()
	{
	//	texture2DFlag = glIsEnabled(GL_TEXTURE_2D);
	//	blendFlag = glIsEnabled(GL_BLEND);
	//	memset(curTextureID,0,sizeof(int) * 32);
	//	glGetIntegerv(GL_BLEND_SRC,&blendSrc);
	//	glGetIntegerv(GL_BLEND_DST,&blendDst);
		//glActiveTexture(GL_TEXTURE0);
	}
}
class XTexture;
namespace XRender
{
	//Solid;Dashes;Short Dashes;Dots;Sparse Dots;Dash Dot;Dash Dot Dot
	//实线;虚线;短虚线;点线;稀点线;点划线;点点划线 
	enum XLineStyle
	{
		LS_SOLID = 0xFFFF,
		LS_DASHES = 0x00FF,
		LS_SHORT_DASHES = 0x0F0F,
		LS_DOTS = 0x5555,
		LS_SPARSE_DOTS = 0x2222,
		LS_DASH_DOT = 0xFE10,
		LS_DASH_DOT_DOT = 0x24FF,
	};
	//最简单的划线函数
	extern void drawLine(float ax,float ay,float bx,float by,float width = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f);
	//在指定位置与指定位置之间划一条直线 type 0:实线，1:虚线
	extern void drawLine(float ax, float ay, float bx, float by, float width, const XFColor &c, XLineStyle type = LS_SOLID);
	inline void drawLine(float ax, float ay, float bx, float by, float width, float r, float g, float b, float a, XLineStyle type = LS_SOLID)
	{
		drawLine(ax,ay,bx,by,width,XFColor(r,g,b,a),type);
	}
	inline void drawLine(const XVector2& p0, const XVector2& p1, float width, const XFColor & c, XLineStyle type = LS_SOLID)
	{
		drawLine(p0.x,p0.y,p1.x,p1.y,width,c.fR,c.fG,c.fB,c.fA,type);
	}
	inline void drawLine(const XVector2& p0, const XVector2& p1, float width = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, XLineStyle type = LS_SOLID)
	{
		drawLine(p0.x,p0.y,p1.x,p1.y,width,r,g,b,a,type);
	}
	inline void drawLine(const XVector2& p,float angle,float length)	//angle[0 - 360]
	{
		drawLine(p,XVector2(p.x + length * cos(angle * DEGREE2RADIAN),p.y + length * sin(angle * DEGREE2RADIAN)));
	}
	//描绘与背景反色的线
	extern void drawLineAntiColor(float ax,float ay,float bx,float by,float width = 1.0f,XLineStyle type = LS_SOLID);
	//通过VBO画多条线
	extern void drawLinesVbo(unsigned int v,int pointSum,float w,float r,float g,float b);	
	extern void drawLinesVbo(unsigned int v,int pointSum,float w,float r,float g,float b,float a);	
	inline void drawRectS(const XRect &rect)
	{
		//glBegin(GL_LINE_STRIP);
		glBegin(GL_LINE_LOOP);
		glVertex2f(rect.left,rect.top);
		glVertex2f(rect.right,rect.top);
		glVertex2f(rect.right,rect.bottom);
		glVertex2f(rect.left,rect.bottom);
		//glVertex2f(rect.left,rect.top);
		glEnd();
	}
	//画线框，实线或者虚线
	extern void drawRect(const XVector2& pos,const XVector2& size,float width, const XFColor &c);
	extern void drawRect(const XRect& rect, float width, const XFColor &c, XLineStyle type = LS_SOLID);
	inline void drawRect(const XRect& rect, float width = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, XLineStyle type = LS_SOLID)
	{
		drawRect(rect,width,XFColor(r,g,b,a),type);
	}
	//画线框，与背景反色
	extern void drawRectAntiColor(const XRect& rect, float width = 1.0f, XLineStyle type = LS_SOLID);
	//画线框
	inline void drawBoxS(float x,float y,float sizeX,float sizeY)
	{
		glBegin(GL_LINE_STRIP);
		glVertex2f(x - sizeX,y + sizeY);
		glVertex2f(x - sizeX,y - sizeY);
		glVertex2f(x + sizeX,y - sizeY);
		glVertex2f(x + sizeX,y + sizeY);
		glVertex2f(x - sizeX,y + sizeY);
		glEnd();
	}
	inline void drawBoxS(const XVector2 &p1,const XVector2 &p2,const XVector2 &p3,const XVector2 &p4)
	{
		glBegin(GL_LINE_STRIP);
		glVertex2fv(p1);
		glVertex2fv(p2);
		glVertex2fv(p4);
		glVertex2fv(p3);
		glVertex2fv(p1);
		glEnd();
	}
	inline void drawBox(const XVector2 &pos,const XVector2 &size)
	{
		glBegin(GL_LINE_STRIP);
			glVertex2fv(pos);
			glVertex2f(pos.x + size.x,pos.y);
			glVertex2f(pos.x + size.x,pos.y + size.y);
			glVertex2f(pos.x,pos.y + size.y);
			glVertex2fv(pos);
		glEnd();
	}
	inline void drawFillBoxS(float x,float y,float w,float h)
	{
		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x + w,y);
		glVertex2f(x + w,y + h);
		glVertex2f(x,y + h);
		glEnd();
	}
	extern void drawBox(float x,float y,float sizeX,float sizeY,int w,const XFColor &c);
	inline void drawBox(float x,float y,float sizeX,float sizeY,int w,float r,float g,float b,float a)
	{
		drawBox(x,y,sizeX,sizeY,w,XFColor(r,g,b,a));
	}
	inline void drawBox(float x,float y,float sizeX,float sizeY,int w,float r = 1.0f,float g = 1.0f,float b = 1.0f)
	{
		XGL::DisableTexture2D();
		glColor3f(r,g,b);
		glLineWidth(w);
		drawBoxS(x,y,sizeX,sizeY);
	}
	inline void drawBox(float x,float y,float sizeX,float sizeY)
	{
		XGL::DisableTexture2D();
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glLineWidth(1);
		drawBoxS(x,y,sizeX,sizeY);
	}
	extern void drawBox(float x,float y,float sizeX,float sizeY,float angle);
	extern void drawBox(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,float width = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f);
	extern void drawBox(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,float width,const XFColor & c);
	inline void drawBox(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,float width,float r,float g,float b,float a)
	{
		drawBox(p0,p1,p2,p3,width,XFColor(r,g,b,a));
	}
	//描绘三角形
	extern void drawTriangle(const XVector2& p0,const XVector2& p1,const XVector2& p2,float width = 0.5f,float r = 1.0f,float g = 1.0f,float b = 1.0f);	
	//描绘正三角形
	extern void drawTriangle(const XVector2& p,float size,float width,const XFColor &color);
	//画点
	extern void drawPoint(float x,float y,float r,float g,float b,float a);
	inline void drawPoint(const XVector2& p,float r,float g,float b,float a)
	{
		drawPoint(p.x,p.y,r,g,b,a);
	}
	extern void drawPoint(float x,float y,float size,float r,float g,float b,float a);
	inline void drawPoint(const XVector2& p,float size,float r,float g,float b,float a)
	{
		drawPoint(p.x,p.y,size,r,g,b,a);
	}
	//Vbo画点
	extern void drawPointsVbo(unsigned int v,int pointSum,float s,float r,float g,float b);	
	extern void drawPointsVbo(unsigned int v,int pointSum,float s,float r,float g,float b,float a);	
	//画十字
	extern void drawCross(const XVector2& p,float size = 10.0f,float w = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f);
	extern void drawCross(const XVector2& p,float size,float w,const XFColor& color);
	//画对号
	extern void drawMaskRight(const XVector2& p,const XVector2& size,float width,const XFColor& color);
	//画错号
	extern void drawMaskWrong(const XVector2& p,const XVector2& size,float width,const XFColor& color);
	//下面是描述方向的标记的描绘
	extern void drawLeft(const XVector2& p,const XVector2& size,float width,const XFColor& color);
	extern void drawRight(const XVector2& p,const XVector2& size,float width,const XFColor& color);
	extern void drawUp(const XVector2& p,const XVector2& size,float width,const XFColor& color);
	extern void drawDown(const XVector2& p,const XVector2& size,float width,const XFColor& color);
	//画圆形
	//px,px:位置
	//sr:半径
	//an:边数量
	extern void drawCircle(float px, float py, float sr, int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	inline void drawCircle(const XVector2&pos, float sr, int an,const XFColor &color)
	{
		drawCircle(pos.x,pos.y,sr,an,color.fR,color.fG,color.fB,color.fA);
	}
	extern void drawCircleEx(float px, float py, float sr,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	inline void drawCircleEx(const XVector2&pos, float sr,const XFColor &color)
	{
		drawCircleEx(pos.x,pos.y,sr,color.fR,color.fG,color.fB,color.fA);
	}
	//画线圆
	extern void drawCircleLine(float px, float py, float sr, int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	extern void drawCircleLine(const XVector2 &pos, float sr, int an,float w,const XFColor &color);
	extern void drawCircleLineEx(float px, float py, float sr,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	extern void drawCircleLineEx(const XVector2 &pos, float sr,float w,const XFColor &color);
	//描绘圆环
	//x,y圆心的位置，
	//r0,r1内外半径，
	//angleS,angleE起止角度[0 - 360]
	//an:边数量
	extern void drawFillRing(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	inline void drawFillRing(float x,float y,float r0,float r1,float angleS,float angleE,int an,const XFColor &c)
	{
		drawFillRing(x,y,r0,r1,angleS,angleE,an,c.fR,c.fG,c.fB,c.fA);
	}
	//描绘圆环带半透明渐变
	extern void drawFillRingEx(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	//画实心矩形
	extern void drawFillBox(const XVector2 &p0,const XFColor &c0,		//第一个点以及颜色
							const XVector2 &p1,const XFColor &c1,		//第二个点以及颜色
							const XVector2 &p2,const XFColor &c2,		//第三个点以及颜色
							const XVector2 &p3,const XFColor &c3);	//第四个点以及颜色
	extern void drawFillBox(float x,float y,float w,float h,
		const XFColor & c);	//描绘实心矩形
	inline void drawFillBox(float x,float y,float w,float h,
							float r,float g,float b,float a)	//描绘实心矩形
	{
		drawFillBox(x,y,w,h,XFColor(r,g,b,a));
	}
	extern void drawFillBox(const XVector2& pos,const XVector2& size,
							float r = 1.0f,float g = 1.0f,float b = 1.0f,bool withLine = false);	//描绘实心矩形
	extern void drawFillBoxA(const XVector2& pos,const XVector2& size,
		const XFColor &c,bool withLine = false);	//描绘实心矩形
	inline void drawFillBoxA(const XVector2& pos,const XVector2& size,
							float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,bool withLine = false)	//描绘实心矩形
	{
		drawFillBoxA(pos,size,XFColor(r,g,b,a),withLine);
	}
	extern void drawFillBoxEx(const XVector2& pos,const XVector2& size,	
							float r = 1.0f,float g = 1.0f,float b = 1.0f,
							bool withLine = false,bool withRoundCorner = false,bool down = false);	//描绘实心矩形
	//立体感没有上面那个强
	extern void drawFillBoxExAS(const XVector2& pos,const XVector2& size,	
							float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
							bool withLine = false,bool withRoundCorner = false,bool down = false);	//描绘实心矩形
	inline void drawFillBoxExAS(const XVector2& pos,const XVector2& size,	
		const XFColor &c,bool withLine = false,bool withRoundCorner = false,bool down = false)	//描绘实心矩形
	{
		drawFillBoxExAS(pos,size,c.fR,c.fG,c.fB,c.fA,withLine,withRoundCorner,down);
	}
	//有更强的立体感
	extern void drawFillBoxExA(const XVector2& pos,const XVector2& size,	
							float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
							bool withLine = false,bool withRoundCorner = false,bool down = false);	//描绘实心矩形
	//更高效
	//inline void drawFillBoxExA(const XVector2& pos,const XVector2& size,	
	//						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
	//						bool withLine = false,bool withRoundCorner = false,bool down = false)	//描绘实心矩形
	//{
	//	drawFillBoxExAS(pos,size,r,g,b,a,withLine,withRoundCorner,down);
	//}
	inline void drawFillBoxExA(const XVector2& pos,const XVector2& size,	
							const XFColor &c,
							bool withLine = false,bool withRoundCorner = false,bool down = false)
	{
		drawFillBoxExA(pos,size,c.fR,c.fG,c.fB,c.fA,withLine,withRoundCorner,down);
	}
	//指向某一点的箭头,angle单位度
	inline void drawArrow(const XVector2& arm,float angle,float length)
	{
		drawLine(arm,angle,length);	//画主线
		drawLine(arm,angle + 30,10);
		drawLine(arm,angle - 30,10);
	}
	//描绘箭头,从start到end的箭头
	inline void drawArrow(const XVector2& start,const XVector2& end)
	{
		float len = start.getLength(end);
		float angle = end.getAngle(start) * RADIAN2DEGREE;
		//描绘分叉
		drawArrow(end,angle,len);
	}
	//cornerStyle:描述圆角的样式每2bits控制一个角，依次为右下角、左下角、左上角、右上角
	//0为直角、1为内圆角，2、为外圆角，3、为另一个方向的外圆角
	//lineStyle:线的样式，4个bits分别描述4条边是否需要需要描绘
	extern void drawFillBoxExEx(const XVector2& pos,const XVector2& size,
							float r = 1.0f,float g = 1.0f,float b = 1.0f,
							unsigned char lineStyle = 0,unsigned char cornerStyle = 0,bool down = false);	//描绘实心矩形
	extern void drawFillBoxExExA(const XVector2& pos,const XVector2& size,
							float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
							unsigned char lineStyle = 0,unsigned char cornerStyle = 0,bool down = false);	//描绘实心矩形
	inline void drawFillBoxExExA(const XVector2& pos,const XVector2& size,
		const XFColor &c,unsigned char lineStyle = 0,unsigned char cornerStyle = 0,bool down = false)	//描绘实心矩形
	{
		drawFillBoxExExA(pos,size,c.fR,c.fG,c.fB,c.fA,lineStyle,cornerStyle,down);
	}
	extern void drawFillBox(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,
							float r = 1.0f,float g = 1.0f,float b = 1.0f);	//描绘实心四边形
	extern void drawFillBoxEx(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,
							float r = 1.0f,float g = 1.0f,float b = 1.0f,
							bool withLine = false);	//描绘实心四边形
	extern void drawFillBoxBW(const XVector2& pos,const XVector2& size,const XVector2& cSize);	//描绘黑白色的间隔
	extern void drawFillTriangle(const XVector2& p0,const XVector2& p1,const XVector2& p2,
							float r = 1.0f,float g = 1.0f,float b = 1.0f);	//描绘实心三角形
	extern void drawFillTriangleEx(const XVector2& p0,const XVector2& p1,const XVector2& p2,
							float r = 1.0f,float g = 1.0f,float b = 1.0f);	//描绘实心三角形
	//画实心多边形
	extern void drawFillPolygon(const XVector2 *p,int sum,float r,float g,float b);
	extern void drawFillPolygon(const XVector2 *p,int sum,const XVector2 &pos,float angle,float r,float g,float b);
	extern void drawFillPolygonEx(const XVector2 *p,const float *c,int sum,float r,float g,float b,bool withLine = false);	//增加渐变的质感
	extern void drawFillPolygonExA(const XVector2 *p,const float *c,int sum,float r,float g,float b,float a,bool withLine = false);	//增加渐变的质感
	inline void drawFillPolygonExA(const XVector2 *p,const float *c,int sum,const XFColor &cl,bool withLine = false)	//增加渐变的质感
	{
		drawFillPolygonExA(p,c,sum,cl.fR,cl.fG,cl.fB,cl.fA,withLine);
	}
	//下面的函数用于描绘一张贴图
	extern void drawTexture(XTexture &tex,const XVector2 &pos,int cW,int cH,XVector2 *vArray,XVector2 *uArray,int arrayW,int arrayH);	//使用WITHXSPRITE_EX的时候需要进行贴图归一化
	extern void drawTexture(unsigned int tex,const XVector2 &pos,int texW,int texH,bool blend = false);
	extern void drawTextureEx(unsigned int tex,const XVector2 &pos,const XVector2 &size,XVector2 *vArray,
							  XVector2 *uArray,int arrayW,int arrayH,bool blend = false);
}

#define TEX_DEBUG (0)
#define TEX_DEBUG1 (0)
#if TEX_DEBUG
extern int XTSum;
#endif
#if TEX_DEBUG1
extern int CPSum;
#endif

class XResourceInfo;
class XPBO;
class XTexture	//这个接口建议只对内，或者是用于没有裁剪的外部接口
{
private:
	XSCounter *m_cp;	//引用计数器
	void release();	//释放图片资源
	XBool m_isLoaded;	//是否载入
	XResourceInfo * m_resInfo;	//资源的指针
public:
	GLuint m_texture;	//贴图的编号:0无效，大约0有效
	int m_w;			//贴图的像素宽度
	int m_h;			//贴图的像素高度
	XBool getIsEnable() {return m_isLoaded;}//检查当前贴图是否有效
	XColorMode m_mode;	//0:RGBA else RGB
	void bind()
	{
		XGL::EnableTexture2D();
		XGL::BindTexture2D(m_texture);
	}
	XBool reset();
public:
	//resoursePosition表示资源的位置：0表示外部资源；1：表示内部资源，将从资源文件包中读取资源	
	XBool load(const char *filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);//从指定文件中载入图片资源
	//下面两个接口尚未实现
	XBool createTexture(int w,int h,XColorMode mode);			//建立一张指定大小的空白贴图
	XBool createWithTexture(int w,int h,XColorMode mode,unsigned int tex);	//直接使用已经存在的贴图
	void updateTexture(const XPBO &pbo);		//使用指定数据更新贴图
	void updateTextureR2B(const XPBO &pbo);		//使用指定数据更新贴图
	void updateTexture(const void *data,int w = 0,int h = 0,int x = 0,int y = 0)		//使用指定数据更新贴图
	{
		if(!m_isLoaded || data == NULL) return;	//PBO的时候可以为 NULL
		updateTextureX(data,w,h,x,y);
	}
	void updateTextureR2B(const void *data,int w = 0,int h = 0,int x = 0,int y = 0)	//功能与上相同，不同在于红蓝色分量互换
	{
		if(!m_isLoaded || data == NULL) return;	//PBO的时候可以为 NULL
		updateTextureR2BX(data,w,h,x,y);
	}
	void updateTextureEx(const void *data);		//不建议使用这个函数，但是初步测试这个性能会比下面的性能好
	void updateTextureR2BEx(const void *data);	//不建议使用这个函数，但是初步测试这个性能会比下面的性能好
private:
	void updateTextureX(const void *data,int w = 0,int h = 0,int x = 0,int y = 0);		//使用指定数据更新贴图
	void updateTextureR2BX(const void *data,int w = 0,int h = 0,int x = 0,int y = 0);	//功能与上相同，不同在于红蓝色分量互换
public:
	XTexture()
		:m_texture(0)
		,m_isLoaded(XFalse)
		,m_cp(NULL)
		,m_mode(COLOR_RGBA)
		,m_resInfo(NULL)
		,m_w(0)
		,m_h(0)
	{}

	~XTexture()
	{
		if(m_cp != NULL && -- m_cp->m_counter <= 0)
		{
			release();
#if TEX_DEBUG1
			-- CPSum;
			printf("%d-\n",CPSum);
#endif
			XMem::XDELETE(m_cp);
		}
	}
	XTexture(const XTexture& temp);	//拷贝构造函数
	XTexture& operator = (const XTexture &temp);
};
}
#endif
