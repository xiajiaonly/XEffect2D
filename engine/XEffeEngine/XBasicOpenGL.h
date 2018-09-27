#ifndef _JIA_XBASICOPENGL_
#define _JIA_XBASICOPENGL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "glew.h"
#include "gl.h"
#include "XBasicFun.h"
#include "XMath/XVector2.h"
#include "XMath/XRect.h"
#include "XMath/XFColor.h"
#include "XBasicClass.h"
#include "XMemory.h"
#ifdef _WIN64
#pragma comment(lib, "gl/glew64s.lib") 
#else
#pragma comment(lib, "gl/glew32s.lib") 
#endif
//#pragma comment(lib, "gl/opengl.lib") 
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
//	enum XBlendType
//	{
//		BLEND_TWO_DATA,		//两个因子的混合
//		BLEND_FOUR_DATA,	//四个因子的混合
//	};
	enum XBlendModel
	{
		BLEND_NULL,
		BLEND_ALPHA,	//45
		BLEND_ADD,		//41
		BLEND_MULTIPLY,
		BLEND_SCREEN,
		BLEND_SUBTRACT,
		BLEND_MODEL0,	//4515
		BLEND_MODEL1,	//15
		BLEND_MODEL2,	//11
		BLEND_MODEL3,	//25
	};
//	extern const int srcBlendMode[9];
//	extern const int dstBlendMode[8];
	enum XShaderResult
	{
		CMP_SUCCESS,
		CMP_WARNING,
		CMP_ERROR,
	};
	//返回是否成功：0成功，1有警告，2有错误
	extern XShaderResult printShaderInfoLog(GLuint obj);						//输出shader的信息的函数
	extern int printOglError(const char *file, int line);					//显示OpenGL的错误信息
	#define printOpenGLError() printOglError(__FILE__, __LINE__)	//打印程序执行错误的函数
	//返回是否成功：0成功，1有警告，2有错误
	extern XShaderResult printProgramInfoLog(GLuint obj);					//打印程序信息的函数

	extern XBool setShader(const char* vertFile,const char* fragFile,int &pHandle,
							XResPos resPos = RES_SYS_DEF);	//资源位置，0为外部资源，默认为外部资源
	extern XBool setShaderFromStr(const char* vStr,const char *fStr,const char *gStr,XShaderHandle &pHandle);	//直接从字符串中加载shader
	extern XBool setShader(const char* vertFile,const char* fragFile,const char* geomFile,XShaderHandle &pHandle,
							XResPos resPos = RES_SYS_DEF);	//资源位置，0为外部资源，默认为外部资源
	//extern void changeSize(int h, int w);							//SDL中窗口改变事件的响应函数
	//extern XBool TextureLoad(GLuint &texture,const char *filename,int withAlpha,int * w = NULL,int * h = NULL,
	//					   XResPos resPos = RES_SYS_DEF);	//使用SDL载入贴图,默认的资源位置为外部
	extern XBool TextureLoadEx(GLuint &texture, const char *filename, int * w = NULL, int * h = NULL,
		XResPos resPos = RES_SYS_DEF, bool isInThread = false);	//使用SDL载入贴图,默认的资源位置为外部
	//这是一个没有进行更多检查的函数接口，调用之前确认资源符合各种约束
	extern XBool TextureLoadQuarter(GLuint &texture,const char *filename,XResPos resPos = RES_SYS_DEF);	//以四分之一尺寸读取资源
	//建立一张空白贴图0:RGBA,1:RGB,2:灰度图,不输入内部数据格式时为默认格式
	extern GLuint emptyTexture(int x, int y, XColorMode mode,int &internalFormat,int filter = GL_LINEAR,int MSSum = 0);	//使用RGBA的格式建立一张指定大小的空白贴图
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
	inline void SetBlendFunc(int src, int dst)
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
		glBlendFunc(src, dst);
	}
	inline void setBlendAlpha()
	{
		EnableBlend();
		SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	inline void setBlendAdd()	//41
	{
		EnableBlend();
		SetBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	inline void setBlendMultiply()
	{
		EnableBlend();
		SetBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA /* GL_ZERO or GL_ONE_MINUS_SRC_ALPHA */);
	}
	inline void setBlendScreen()
	{
		EnableBlend();
		SetBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
	}
	inline void setBlendSubtract()
	{
		EnableBlend();
		SetBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}	
	inline void setBlendInverse()
	{//与背景反色
		EnableBlend();
		XGL::SetBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
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
	inline void setBlendModel(XBlendModel m)
	{
		switch (m)
		{
		case BLEND_NULL:DisableBlend(); break;
		case BLEND_ALPHA:setBlendAlpha(); break;
		case BLEND_ADD:setBlendAdd(); break;
		case BLEND_MULTIPLY:setBlendMultiply(); break;
		case BLEND_SCREEN:setBlendScreen();	break;
		case BLEND_SUBTRACT:setBlendSubtract();break;
		case BLEND_MODEL0:
			EnableBlend();
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BLEND_MODEL1:
			EnableBlend();
			SetBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BLEND_MODEL2:
			EnableBlend();
			SetBlendFunc(GL_ONE, GL_ONE);
			break;
		case BLEND_MODEL3:
			EnableBlend();
			SetBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
			break;
		default:
			printf("Blend error!");
			//LogStr("Blend error!");
			//aseert()
			break;
		}
	}
//	inline int GetBlendSrc(){return blendSrc;}
//	inline int GetBlendDst(){return blendDst;}
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
	inline void BindTexture2DMS(const unsigned int &t,const int &index = 0)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,t);
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
	//下面的方式是一些实验性的方法
	// read back pixel values
	extern void readFrame(void* backbuffer, int width, int height);
	//下面的宏定义用于检查GL内部的错误，建议在所有调用到GL接口的地方调用这个以便于排除GL内部错误
#if defined(NDEBUG)
#define glCheck(x) x
#else
#define glCheck(x) {x; glMsg(#x, __LINE__, __FILE__);}
	extern void glMsg(const char* msg, long line, const char* file);
#endif
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
	extern void drawLine(const XVec2&a, const XVec2&b, float width = 1.0f,
		const XFColor&c = XFColor::white);
	inline void drawLine(float ax, float ay, float bx, float by, float width,
		float r, float g, float b)
	{
		drawLine(XVec2(ax, ay), XVec2(bx, by), width, XFColor(r, g, b));
	}
	inline void drawLine(float ax, float ay, float bx, float by, float width,
		const XFColor&c)
	{
		drawLine(XVec2(ax, ay), XVec2(bx, by), width, c);
	}
	//在指定位置与指定位置之间划一条直线 type 0:实线，1:虚线
	extern void drawLine(const XVec2& p0, const XVec2& p1, float width,
		const XFColor& c, XLineStyle type);
	inline void drawLine(float ax, float ay, float bx, float by, float width,
		const XFColor& c, XLineStyle type)
	{
		drawLine(XVec2(ax, ay), XVec2(bx, by), width, c, type);
	}
	extern void drawLines(const std::vector<XVec2>&points, float width = 1.0f, const XFColor& c = XFColor::white);
	//	inline void drawLine(const XVec2& p0, const XVec2& p1, float width = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, XLineStyle type = LS_SOLID)
	//	{
	//		drawLine(p0.x,p0.y,p1.x,p1.y,width,r,g,b,a,type);
	//	}
	inline void drawLine(const XVec2& p, float angle, float length, float w = 1.0f, const XFColor& c = XFColor::white)	//angle[0 - 360]
	{
		drawLine(p, p + XMath::getRotateRate(angle * DEGREE2RADIAN) * length, w, c);
	}
	//描绘与背景反色的线
	extern void drawLineAntiColor(const XVec2& a, const XVec2& b, float width = 1.0f, XLineStyle type = LS_SOLID);
	//通过VBO画多条线
	extern void drawLinesVbo(unsigned int v, int pointSum, float w, float r, float g, float b);
	extern void drawLinesVbo(unsigned int v, int pointSum, float w, const XFColor& c);
	//画线框，与背景反色
	extern void drawRectAntiColor(const XRect& rect, float width = 1.0f, XLineStyle type = LS_SOLID);
	//画线框
	//点顺序：左上、右上、右下、左下
	inline void drawQuadBase(const XVec2& p1, const XVec2& p2, const XVec2& p3, const XVec2& p4)
	{
		glBegin(GL_LINE_LOOP);
		glVertex2fv(p1);
		glVertex2fv(p2);
		glVertex2fv(p3);
		glVertex2fv(p4);
		glEnd();
	}
	//pos:左上角坐标，size:全尺寸
	inline void drawRect(const XVec2& pos, const XVec2& size)
	{
		drawQuadBase(pos, XVec2(pos.x + size.x, pos.y),
			pos + size, XVec2(pos.x, pos.y + size.y));
	}
	inline void drawQuad(const XVec2& p0, const XVec2& p1, const XVec2& p2, const XVec2& p3,
		float width = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f)
	{
		glColor3f(r, g, b);
		glLineWidth(width);
		XGL::DisableTexture2D();
		drawQuadBase(p0, p1, p2, p3);
	}
	extern void drawQuad(const XVec2& p0, const XVec2& p1, const XVec2& p2, const XVec2& p3,
		float width, const XFColor& c);
	inline void drawRectBase(const XRect& rect)
	{
		drawQuadBase(rect.getLT(), rect.getRT(), rect.getRB(), rect.getLB());
	}
	//画线框，实线或者虚线
	//pos:中心坐标，size，半尺寸
	extern void drawRect(const XVec2& pos, const XVec2& size, float width, const XFColor& c);
	extern void drawRect(const XRect& rect, float width = 1.0f, const XFColor& c = XFColor::white, XLineStyle type = LS_SOLID);
	//p:左上角坐标，s：全尺寸
	inline void drawFillRectBase(const XVec2&p, const XVec2&s)
	{
		glBegin(GL_QUADS);
		glVertex2fv(p);
		glVertex2f(p.x + s.x, p.y);
		glVertex2fv(p + s);
		glVertex2f(p.x, p.y + s.y);
		glEnd();
	}
	//inline void drawFillBoxS(float x,float y,float w,float h)
	//{
	//	drawFillBoxS(XVec2(x,y),XVec2(w,h));
	//}
	//x,y：中心坐标，
	//sizeX,sizeY:半尺寸
	extern void drawRectEx(float x, float y, float sizeX, float sizeY, float angle, const XFColor& c = XFColor::white);
	//描绘三角形
	extern void drawTriangle(const XVec2& p0, const XVec2& p1, const XVec2& p2, float width = 0.5f, float r = 1.0f, float g = 1.0f, float b = 1.0f);
	//描绘正三角形
	extern void drawTriangle(const XVec2& p, float size, float width, const XFColor& color);
	//画点
//	extern void drawPoint(float x,float y,float r,float g,float b,float a);
//	inline void drawPoint(const XVec2& p,float r,float g,float b,float a)
//	{
//		drawPoint(p.x,p.y,r,g,b,a);
//	}
	extern void drawPoint(const XVec2& p, const XFColor& color);
	extern void drawPoints(const std::vector<XVec2>& ps, float size, const XFColor& color);
	//	extern void drawPoint(float x,float y,float size,float r,float g,float b,float a);
	//	inline void drawPoint(const XVec2& p,float size,float r,float g,float b,float a)
	//	{
	//		drawPoint(p.x,p.y,size,r,g,b,a);
	//	}
	extern void drawPoint(const XVec2& p, float size, const XFColor& color);
	//Vbo画点
	extern void drawPointsVbo(unsigned int v, int pointSum, float s, float r, float g, float b);
	extern void drawPointsVbo(unsigned int v, int pointSum, float s, const XFColor& c);
	//画十字
	extern void drawCross(const XVec2& p, float size = 10.0f, float w = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f);
	extern void drawCross(const XVec2& p, float size, float w, const XFColor& color);
	//画对号
	extern void drawMaskRight(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	//画错号
	extern void drawMaskWrong(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	//下面是描述方向的标记的描绘
	extern void drawLeft(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawRight(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawUp(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawDown(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawSave(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawLoad(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawDefault(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawDownload(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawUpdate(const XVec2& p, const XVec2& size, float width, const XFColor& color);

	//画圆形
	//px,px:位置
	//sr:半径
	//an:边数量
//	extern void drawCircle(float px, float py, float sr, int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	extern void drawCircle(const XVec2&pos, float sr, int an, const XFColor& color);
	//	extern void drawCircleEx(float px, float py, float sr,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	extern void drawCircleEx(const XVec2&pos, float sr, const XFColor& color);
	//画线圆
	extern void drawEllipseLine(const XVec2&pos, float ar, float br, int an, const XFColor&c = XFColor::white);
	extern void drawEllipseLineEx(const XVec2&pos, float ar, float br, int an, float angle, const XFColor&color);
	extern void drawFillEllipseEx(const XVec2&pos, float ar, float br, int an, float angle, const XFColor&color);
	extern void drawCircleLine(const XVec2& pos, float sr, int an, float w, const XFColor& color);
	inline void drawCircleLine(float px, float py, float sr, int an, const XFColor&c = XFColor::white)
	{
		drawCircleLine(XVec2(px, py), sr, an, 1.0f, c);
	}
	extern void drawCircleLineEx(const XVec2& pos, float sr, float w, const XFColor& color);
	inline void drawCircleLineEx(float px, float py, float sr, const XFColor&c = XFColor::white)
	{
		drawCircleLineEx(XVec2(px, py), sr, 1.0f, c);
	}
	//描绘一个蜂窝状的六边形
	extern void drawHexagonLine(const XVec2& pos, float sr, float w, const XFColor& color);
	//填充的六边形
	extern void drawFillHexagon(const XVec2& pos, float sr, const XFColor& color);
	//描绘圆环
	//x,y圆心的位置，
	//r0,r1内外半径，
	//angleS,angleE起止角度[0 - 360]
	//an:边数量
	//extern void drawFillRing(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	extern void drawFillRing(const XVec2& pos, float r0, float r1, float angleS, float angleE, int an, const XFColor& c = XFColor::white);
	//描绘圆环带半透明渐变
	extern void drawFillRingEx(const XVec2& pos, float r0, float r1, float angleS, float angleE,
		int an, const XFColor& c = XFColor::white);
	//描绘扇形
	extern void drawFillFan(const XVec2& pos, float r, float angleS, float angleE, int an, const XFColor& c = XFColor::white);
	//画实心四边形
	extern void drawFillQuad(const XVec2& p0, const XFColor& c0,		//第一个点以及颜色
		const XVec2& p1, const XFColor& c1,		//第二个点以及颜色
		const XVec2& p2, const XFColor& c2,		//第三个点以及颜色
		const XVec2& p3, const XFColor& c3);	//第四个点以及颜色
	extern void drawFillQuad(const XVec2& p0, const XVec2& p1, const XVec2& p2, const XVec2& p3,
		const XFColor& c);	//第四个点以及颜色
	//左上角坐标和全尺寸
	inline void drawFillRect(const XVec2& pos, const XVec2& size,
		const XFColor& c)	//描绘实心矩形
	{
		XGL::setBlendAlpha();
		XGL::DisableTexture2D();
		glColor4fv(c);
		drawFillRectBase(pos, size);
		XGL::DisableBlend();
	}
	extern void drawFillRect(const XVec2& pos, const XVec2& size,
		float r = 1.0f, float g = 1.0f, float b = 1.0f, bool withLine = false);	//描绘实心矩形
	extern void drawFillRectA(const XVec2& pos, const XVec2& size,
		const XFColor& c, bool withLine = false);	//描绘实心矩形
//	inline void drawFillRectA(const XVec2& pos,const XVec2& size,
//							float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,bool withLine = false)	//描绘实心矩形
//	{
//		drawFillRectA(pos,size,XFColor(r,g,b,a),withLine);
//	}
	extern void drawFillRectEx(const XVec2& pos, const XVec2& size,
		float r = 1.0f, float g = 1.0f, float b = 1.0f,
		bool withLine = false, bool withRoundCorner = false, bool down = false);	//描绘实心矩形
//立体感没有上面那个强
	extern void drawFillRectExAS(const XVec2& pos, const XVec2& size,
		float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f,
		bool withLine = false, bool withRoundCorner = false, bool down = false);	//描绘实心矩形
	inline void drawFillRectExAS(const XVec2& pos, const XVec2& size,
		const XFColor& c, bool withLine = false, bool withRoundCorner = false, bool down = false)	//描绘实心矩形
	{
		drawFillRectExAS(pos, size, c.r, c.g, c.b, c.a, withLine, withRoundCorner, down);
	}
	//有更强的立体感
	extern void drawFillRectExA(const XVec2& pos, const XVec2& size,
		float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f,
		bool withLine = false, bool withRoundCorner = false, bool down = false);	//描绘实心矩形
//更高效
//inline void drawFillRectExA(const XVec2& pos,const XVec2& size,	
//						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
//						bool withLine = false,bool withRoundCorner = false,bool down = false)	//描绘实心矩形
//{
//	drawFillRectExAS(pos,size,r,g,b,a,withLine,withRoundCorner,down);
//}
	inline void drawFillRectExA(const XVec2& pos, const XVec2& size,
		const XFColor& c,
		bool withLine = false, bool withRoundCorner = false, bool down = false)
	{
		drawFillRectExA(pos, size, c.r, c.g, c.b, c.a, withLine, withRoundCorner, down);
	}
	//指向某一点的箭头,angle单位度
	inline void drawArrow(const XVec2& arm, float angle, float length)
	{
		drawLine(arm, angle, length);	//画主线
		drawLine(arm, angle + 30, 10);
		drawLine(arm, angle - 30, 10);
	}
	//从某一点出发的箭头,angle单位度
	inline void drawArrowOut(const XVec2& arm, float angle, float length)
	{
		drawArrow(arm + XMath::getRotateRate(angle * DEGREE2RADIAN) * length,
			angle + 180.0f, length);
	}
	//描绘箭头,从start到end的箭头
	inline void drawArrow(const XVec2& start, const XVec2& end)
	{
		//float len = start.getLength(end);
		//float angle = end.getAngle(start) * RADIAN2DEGREE;
		////描绘分叉
		//drawArrow(end,angle,len);
		drawArrow(end, end.getAngle(start) * RADIAN2DEGREE, start.getLength(end));
	}
	//描绘圆箭头
	//pos：描绘的位置
	//r：描绘的半径
	//start：描绘的起始角度，单位角度
	//end：描绘的终止角度，单位角度
	extern void drawArrow(const XVec2&pos, float r, float start, float end, const XFColor& color);
	//cornerStyle:描述圆角的样式每2bits控制一个角，依次为右下角、左下角、左上角、右上角
	//0为直角、1为内圆角，2、为外圆角，3、为另一个方向的外圆角
	//lineStyle:线的样式，4个bits分别描述4条边是否需要需要描绘
	extern void drawFillRectExEx(const XVec2& pos, const XVec2& size,
		float r = 1.0f, float g = 1.0f, float b = 1.0f,
		unsigned char lineStyle = 0, unsigned char cornerStyle = 0, bool down = false);	//描绘实心矩形
	extern void drawFillRectExExA(const XVec2& pos, const XVec2& size,
		float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f,
		unsigned char lineStyle = 0, unsigned char cornerStyle = 0, bool down = false);	//描绘实心矩形
	inline void drawFillRectExExA(const XVec2& pos, const XVec2& size,
		const XFColor& c, unsigned char lineStyle = 0, unsigned char cornerStyle = 0, bool down = false)	//描绘实心矩形
	{
		drawFillRectExExA(pos, size, c.r, c.g, c.b, c.a, lineStyle, cornerStyle, down);
	}
	inline void drawFillQuad(const XVec2& p0, const XVec2& p1, const XVec2& p2, const XVec2& p3,
		float r = 1.0f, float g = 1.0f, float b = 1.0f)	//描绘实心四边形
	{
		glColor3f(r, g, b);
		XGL::DisableTexture2D();
		glBegin(GL_QUADS);
		glVertex2fv(p0);
		glVertex2fv(p1);
		glVertex2fv(p2);
		glVertex2fv(p3);
		glEnd();
	}
	extern void drawFillQuadEx(const XVec2& p0, const XVec2& p1, const XVec2& p2, const XVec2& p3,
		float r = 1.0f, float g = 1.0f, float b = 1.0f,
		bool withLine = false);	//描绘实心四边形
//描绘黑白色的间隔,anti是否反色
//size:总大小
//cSize:方块的数量
	extern void drawFillRectBW(const XVec2& pos, const XVec2& size, const XVec2& cSize, bool anti = false);
	//cSize:单个方块的大小
	//sum:方块的数量
	extern void drawFillRectBWEx(const XVec2& pos, const XVec2& cSize, const XVec2& sum, bool anti = false);
	extern void drawFillTriangle(const XVec2& p0, const XVec2& p1, const XVec2& p2,
		float r = 1.0f, float g = 1.0f, float b = 1.0f);	//描绘实心三角形
	extern void drawFillTriangle(const XVec2& p0, const XVec2& p1, const XVec2& p2,
		const XFColor&c);	//描绘实心三角形
	extern void drawFillTriangleEx(const XVec2& p0, const XVec2& p1, const XVec2& p2,
		float r = 1.0f, float g = 1.0f, float b = 1.0f);	//描绘实心三角形
//画实心多边形(下面的函数只适用于凸多边形)
	extern void drawFillPolygon(const XVec2 *p, int sum, float r, float g, float b);
	extern void drawFillPolygon(const XVec2 *p, int sum, const XVec2& pos, float angle, float r, float g, float b);
	extern void drawFillPolygonEx(const XVec2 *p, const float *c, int sum, float r, float g, float b, bool withLine = false);	//增加渐变的质感
	extern void drawFillPolygonExA(const XVec2 *p, const float *c, int sum, float r, float g, float b, float a, bool withLine = false);	//增加渐变的质感
	inline void drawFillPolygonExA(const XVec2 *p, const float *c, int sum, const XFColor& cl, bool withLine = false)	//增加渐变的质感
	{
		drawFillPolygonExA(p, c, sum, cl.r, cl.g, cl.b, cl.a, withLine);
	}
	//下面的函数用于描绘一张贴图
	extern void drawTexture(XTexture &tex, const XVec2& pos, int cW, int cH,
		const XVec2 *vArray, const XVec2 *uArray, int arrayW, int arrayH);	//使用WITHXSPRITE_EX的时候需要进行贴图归一化
	extern void drawTexture(unsigned int tex, const XVec2& pos, int texW, int texH, bool blend = false);
	//作为一条描绘图片
	extern void drawTexture(unsigned int tex, const XVec2* v, const XVec2* u, int pSum,
		bool blend = false, const XFColor& c = XFColor::white);
	//作为一条描绘图片，可以进行细分，（可以优化为描绘一张网格的图，而不是一条）
	extern void drawTextureEx(unsigned int tex, const XVec2* v, const XVec2* u, int pSum, 
		int subSum = 1, bool blend = false);
	//通过UV网格绘制一张贴图，（可以优化加入细分）
	extern void drawTextureEx(unsigned int tex, const XVec2& pos, const XVec2& size, XVec2 *vArray,
		XVec2 *uArray, int arrayW, int arrayH, bool blend = false);
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
	bool m_isThreadEvn;	//是否为多线程的加载环境
public:
	void setIsThreadEvn(bool flag) { m_isThreadEvn = flag; }
	bool getIsThreadEvn() const { return m_isThreadEvn; }
	GLuint m_texture;	//贴图的编号:0无效，大约0有效
	int m_w;			//贴图的像素宽度
	int m_h;			//贴图的像素高度
	int m_internalFormat;	//内部数据格式
	XBool getIsEnable() { return m_isLoaded; }//检查当前贴图是否有效
	XColorMode m_mode;	//0:RGBA else RGB
	void bind()
	{
		XGL::EnableTexture2D();
		XGL::BindTexture2D(m_texture);
	}
	XBool reset();
	int m_MSSum;
public:
	//resPos表示资源的位置：0表示外部资源；1：表示内部资源，将从资源文件包中读取资源	
	XBool load(const char *filename, XResPos resPos = RES_SYS_DEF);//从指定文件中载入图片资源
	//下面两个接口尚未实现，不输入内部数据格式时为默认格式
	XBool createTexture(int w, int h, XColorMode mode, int internalFormat = -1, int filter = GL_LINEAR, int MSSum = 0);			//建立一张指定大小的空白贴图
	XBool createWithTexture(int w, int h, XColorMode mode, unsigned int tex);	//直接使用已经存在的贴图
	void updateTexture(const XPBO &pbo);		//使用指定数据更新贴图
	void updateTextureR2B(const XPBO &pbo);		//使用指定数据更新贴图
	void updateTexture(const void *data, int w = 0, int h = 0, int x = 0, int y = 0)		//使用指定数据更新贴图
	{
		if (!m_isLoaded || data == NULL) return;	//PBO的时候可以为 NULL
		updateTextureX(data, w, h, x, y);
	}
	void updateTextureR2B(const void *data, int w = 0, int h = 0, int x = 0, int y = 0)	//功能与上相同，不同在于红蓝色分量互换
	{
		if (!m_isLoaded || data == NULL) return;	//PBO的时候可以为 NULL
		updateTextureR2BX(data, w, h, x, y);
	}
	void updateTextureEx(const void *data);		//不建议使用这个函数，但是初步测试这个性能会比下面的性能好
	void updateTextureR2BEx(const void *data);	//不建议使用这个函数，但是初步测试这个性能会比下面的性能好
private:
	void updateTextureX(const void *data, int w = 0, int h = 0, int x = 0, int y = 0);		//使用指定数据更新贴图
	void updateTextureR2BX(const void *data, int w = 0, int h = 0, int x = 0, int y = 0);	//功能与上相同，不同在于红蓝色分量互换
public:
	XTexture()
		:m_texture(0)
		, m_isLoaded(XFalse)
		, m_cp(NULL)
		, m_mode(COLOR_RGBA)
		, m_resInfo(NULL)
		, m_w(0)
		, m_h(0)
		, m_MSSum(0)
		, m_isThreadEvn(false)
	{}

	~XTexture()
	{
		if (m_cp != NULL && --m_cp->m_counter <= 0)
		{
			release();
#if TEX_DEBUG1
			-- CPSum;
			printf("%d-\n", CPSum);
#endif
			XMem::XDELETE(m_cp);
		}
	}
	XTexture(const XTexture& temp);	//拷贝构造函数
	XTexture& operator = (const XTexture &temp);
};
inline int getBytePerPixelByColorMode(XColorMode mode)
{
	switch (mode)
	{
	case COLOR_BGRA:
	case COLOR_RGBA: return 4;
	case COLOR_BGR:
	case COLOR_RGB: return 3;
	case COLOR_RED:
	case COLOR_GRAY: return 1;
	case COLOR_RGBA_F: return 32;
	case COLOR_RGB_F: return 24;
	case COLOR_GRAY_F: return 4;
	case COLOR_DEPTH: return 4;
	default:return 0;
	}
}
inline unsigned int colorMode2GLMode(XColorMode mode)
{
	switch (mode)
	{
	case COLOR_BGRA: return GL_BGRA;
	case COLOR_RGBA: return GL_RGBA;
	case COLOR_BGR: return GL_BGR;
	case COLOR_RGB: return GL_RGB;
	case COLOR_RED: return GL_RED;
	case COLOR_GRAY: return GL_LUMINANCE;
	case COLOR_RGBA_F: return GL_RGBA;
	case COLOR_RGB_F: return GL_RGB;
	case COLOR_GRAY_F: return GL_RED;
	default:return 0;
	}
}
}
#endif
