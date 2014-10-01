#ifndef _JIA_XBASICOPENGL_
#define _JIA_XBASICOPENGL_
//#include "gl.h"
//#include "glew.h"
//#include "stdlib.h"
#include "XBasicFun.h"
#include "XMath/XVector2.h"
#include "XMath/XRect.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//基本内部格式			红色成分	绿色成分	蓝色成分	alpha成分
//GL_ALPHA				不改变		不改变		不改变		At
//GL_LUMINANCE			Lt			Lt			Lt			不改变
//GL_LUMINANCE_ALPHA	Lt			Lt			Lt			At
//GL_INTENSITY			It			It			It			It
//GL_RGB				Rt			Gt			Bt			不改变
//GL_RGBA				Rt			Gt			Bt			At
enum _XBlendType
{
	BLEND_TWO_DATA,		//两个因子的混合
	BLEND_FOUR_DATA,		//四个因子的混合
};
extern unsigned int srcBlendMode[9];
extern unsigned int dstBlendMode[8];

extern void printShaderInfoLog(GLuint obj);						//输出shader的信息的函数
extern int printOglError(const char *file, int line);					//显示OpenGL的错误信息
#define printOpenGLError() printOglError(__FILE__, __LINE__)	//打印程序执行错误的函数
extern void printProgramInfoLog(GLuint obj);					//打印程序信息的函数

extern char *textFileRead(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//从filename的文件中读取shader代码	//资源位置，0为外部资源，默认为外部资源
extern int textFileWrite(const char *filename, const char *s);					//将shader代码s写入到filename的文件中
extern _XBool setShader(const char* vertFile,const char* fragFile,int &pHandle,
						_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//资源位置，0为外部资源，默认为外部资源
extern _XBool setShader(const char* vertFile,const char* fragFile,const char* geomFile,_XShaderHandle &pHandle,
						_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//资源位置，0为外部资源，默认为外部资源

extern void changeSize(int h, int w);							//SDL中窗口改变事件的响应函数

//extern _XBool TextureLoad(GLuint &texture,const char *filename,int withAlpha,int * w = NULL,int * h = NULL,
//					   _XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//使用SDL载入贴图,默认的资源位置为外部
extern _XBool TextureLoadEx(GLuint &texture,const char *filename,int * w = NULL,int * h = NULL,
						 _XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//使用SDL载入贴图,默认的资源位置为外部
//这是一个没有进行更多检查的函数接口，调用之前确认资源符合各种约束
extern _XBool TextureLoadQuarter(GLuint &texture,const char *filename);	//以四分之一尺寸读取资源

extern void initWindowMatrix();	//初始化窗口的矩阵
extern _XBool initOpenGL3D();	//3D窗口的初始化一些openGL属性
extern _XBool initOpenGL2D();	//可以拉伸和旋转的窗口的初始化一些openGL属性
//最简单的划线函数
extern void drawLine(float ax,float ay,float bx,float by,float width = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f);
//在指定位置与指定位置之间划一条直线 type 0:实线，1:虚线
extern void drawLine(float ax,float ay,float bx,float by,float width,float r,float g,float b,float a,int type = 0);
inline void drawLine(const _XVector2& p0,const _XVector2& p1,float width = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0)
{
	drawLine(p0.x,p0.y,p1.x,p1.y,width,r,g,b,a,type);
}
inline void drawLine(const _XVector2& p,float angle,float length)	//angle[0 - 360]
{
	drawLine(p,_XVector2(p.x + length * cos(angle * DEGREE2RADIAN),p.y + length * sin(angle * DEGREE2RADIAN)));
}
//通过VBO画多条线
extern void drawLinesVbo(unsigned int v,int pointSum,float w,float r,float g,float b);	
extern void drawLinesVbo(unsigned int v,int pointSum,float w,float r,float g,float b,float a);	
inline void drawRectS(const _XRect &rect)
{
	glBegin(GL_LINE_STRIP);
	glVertex2f(rect.left,rect.top);
	glVertex2f(rect.right,rect.top);
	glVertex2f(rect.right,rect.bottom);
	glVertex2f(rect.left,rect.bottom);
	glVertex2f(rect.left,rect.top);
	glEnd();
}
//画线框，实线或者虚线
extern void drawRect(const _XRect& rect,float width = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0);
//画线框，与背景反色
extern void drawRectAntiColor(const _XRect& rect,float width = 1.0f,int type = 0);
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
inline void drawBoxS(const _XVector2 &p1,const _XVector2 &p2,const _XVector2 &p3,const _XVector2 &p4)
{
	glBegin(GL_LINE_STRIP);
	glVertex2fv(p1);
	glVertex2fv(p2);
	glVertex2fv(p4);
	glVertex2fv(p3);
	glVertex2fv(p1);
	glEnd();
}
inline void drawBox(const _XVector2 &pos,const _XVector2 &size)
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
extern void drawBox(float x,float y,float sizeX,float sizeY,int w,float r,float g,float b,float a);
inline void drawBox(float x,float y,float sizeX,float sizeY,int w,float r = 1.0f,float g = 1.0f,float b = 1.0f)
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(r,g,b);
	glLineWidth(w);
	drawBoxS(x,y,sizeX,sizeY);
	glEnable(GL_TEXTURE_2D);
}
inline void drawBox(float x,float y,float sizeX,float sizeY)
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glLineWidth(1);
	drawBoxS(x,y,sizeX,sizeY);
	glEnable(GL_TEXTURE_2D);
}
extern void drawBox(float x,float y,float sizeX,float sizeY,float angle);
extern void drawBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,float width = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f);
extern void drawBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,float width,float r,float g,float b,float a);
extern void drawTriangle(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,float width = 0.5f,float r = 1.0f,float g = 1.0f,float b = 1.0f);	//描绘三角形
//画点
extern void drawPoint(float x,float y,float r,float g,float b,float a);
inline void drawPoint(const _XVector2& p,float r,float g,float b,float a)
{
	drawPoint(p.x,p.y,r,g,b,a);
}
extern void drawPoint(float x,float y,float size,float r,float g,float b,float a);
inline void drawPoint(const _XVector2& p,float size,float r,float g,float b,float a)
{
	drawPoint(p.x,p.y,size,r,g,b,a);
}
//Vbo画点
extern void drawPointsVbo(unsigned int v,int pointSum,float s,float r,float g,float b);	
extern void drawPointsVbo(unsigned int v,int pointSum,float s,float r,float g,float b,float a);	
//画十字
extern void drawCross(const _XVector2& p,float size = 10.0f,float w = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f);
//画圆形
//px,px:位置
//sr:半径
//an:边数量
extern void drawCircle(float px, float py, float sr, int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
//画线圆
extern void drawCircleLine(float px, float py, float sr, int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
//描绘圆环
//x,y圆心的位置，
//r0,r1内外半径，
//angleS,angleE起止角度[0 - 360]
//an:边数量
extern void drawFillRing(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
//描绘圆环带半透明渐变
extern void drawFillRingEx(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
//画实心矩形
class _XFColor;
extern void drawFillBox(const _XVector2 &p0,const _XFColor &c0,		//第一个点以及颜色
						const _XVector2 &p1,const _XFColor &c1,		//第二个点以及颜色
						const _XVector2 &p2,const _XFColor &c2,		//第三个点以及颜色
						const _XVector2 &p3,const _XFColor &c3);	//第四个点以及颜色
extern void drawFillBox(float x,float y,float w,float h,
						float r,float g,float b,float a);	//描绘实心矩形
extern void drawFillBox(const _XVector2& pos,const _XVector2& size,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,bool withLine = false);	//描绘实心矩形
extern void drawFillBoxA(const _XVector2& pos,const _XVector2& size,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,bool withLine = false);	//描绘实心矩形
extern void drawFillBoxEx(const _XVector2& pos,const _XVector2& size,	
						float r = 1.0f,float g = 1.0f,float b = 1.0f,
						bool withLine = false,bool withRoundCorner = false,bool down = false);	//描绘实心矩形
//立体感没有上面那个强
extern void drawFillBoxExAS(const _XVector2& pos,const _XVector2& size,	
						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
						bool withLine = false,bool withRoundCorner = false,bool down = false);	//描绘实心矩形
//有更强的立体感
//extern void drawFillBoxExA(const _XVector2& pos,const _XVector2& size,	
//						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
//						bool withLine = false,bool withRoundCorner = false,bool down = false);	//描绘实心矩形
inline void drawFillBoxExA(const _XVector2& pos,const _XVector2& size,	
						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
						bool withLine = false,bool withRoundCorner = false,bool down = false)	//描绘实心矩形
{
	drawFillBoxExAS(pos,size,r,g,b,a,withLine,withRoundCorner,down);
}

//cornerStyle:描述圆角的样式每2bits控制一个角，依次为右下角、左下角、左上角、右上角
//0为直角、1为内圆角，2、为外圆角，3、为另一个方向的外圆角
//lineStyle:线的样式，4个bits分别描述4条边是否需要需要描绘
extern void drawFillBoxExEx(const _XVector2& pos,const _XVector2& size,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,
						unsigned char lineStyle = 0,unsigned char cornerStyle = 0,bool down = false);	//描绘实心矩形
extern void drawFillBoxExExA(const _XVector2& pos,const _XVector2& size,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
						unsigned char lineStyle = 0,unsigned char cornerStyle = 0,bool down = false);	//描绘实心矩形
extern void drawFillBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,
						float r = 1.0f,float g = 1.0f,float b = 1.0f);	//描绘实心四边形
extern void drawFillBoxEx(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,
						bool withLine = false);	//描绘实心四边形
extern void drawFillBoxBW(const _XVector2& pos,const _XVector2& size,const _XVector2& cSize);	//描绘黑白色的间隔
extern void drawFillTriangle(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,
						float r = 1.0f,float g = 1.0f,float b = 1.0f);	//描绘实心三角形
extern void drawFillTriangleEx(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,
						float r = 1.0f,float g = 1.0f,float b = 1.0f);	//描绘实心三角形
//画实心多边形
extern void drawFillPolygon(const _XVector2 *p,int sum,float r,float g,float b);
extern void drawFillPolygon(const _XVector2 *p,int sum,const _XVector2 &pos,float angle,float r,float g,float b);
extern void drawFillPolygonEx(const _XVector2 *p,const float *c,int sum,float r,float g,float b,bool withLine = false);	//增加渐变的质感
extern void drawFillPolygonExA(const _XVector2 *p,const float *c,int sum,float r,float g,float b,float a,bool withLine = false);	//增加渐变的质感
//下面的函数用于描绘一张贴图
class _XTexture;
extern void drawTexture(_XTexture &tex,const _XVector2 &pos,int cW,int cH,_XVector2 *vArray,_XVector2 *uArray,int arrayW,int arrayH);	//使用WITH_XSPRITE_EX的时候需要进行贴图归一化
extern void drawTexture(unsigned int tex,const _XVector2 &pos,int texW,int texH,bool blend = false);
extern void drawTextureEx(unsigned int tex,const _XVector2 &pos,const _XVector2 &size,_XVector2 *vArray,
						  _XVector2 *uArray,int arrayW,int arrayH,bool blend = false);
//建立一张空白贴图0:RGBA,1:RGB,2:灰度图
extern GLuint emptyTexture(int x,int y,_XColorMode mode = COLOR_RGBA);	//使用RGBA的格式建立一张指定大小的空白贴图
//withThread：是否使用多线程
//将屏幕从点x，y开始的w和h矩形内的图像保存到png文件
extern _XBool gameShot(const char *fileName,int w,int h,bool withThread = true,float x = 0.0f,float y = 0.0f);
inline void setVSync(bool sync)	//开启和关闭垂直同步，关闭的话，可以放开帧率，开启的话会限制帧率
{	
	// Function pointer for the wgl extention function we need to enable/disable
	// vsync
	typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALPROC)( int );
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

	const char *extensions = (char*)glGetString( GL_EXTENSIONS );

	if(strstr(extensions,"WGL_EXT_swap_control") == 0) return;
	else
	{
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
		if(wglSwapIntervalEXT) wglSwapIntervalEXT(sync);
	}
	//说明
	//In Linux / GLX
	//Use the GLX_EXT_swap_control extension to control swap interval. Check the GLX-specific extensions string via glXQueryExtensionsString() to verify that the extension is actually present.
	//The extension provides glXSwapIntervalEXT(), which also directly specifies the swap interval. glXSwapIntervalEXT(1) is used to enable vsync; glXSwapIntervalEXT(0) to disable vsync.
}
#include "XBasicFun.h"
#include "string.h"
class _XTexture;
class _XTextureInfo;
class _XFrame;			//简单测试
class _XFrameEx;
class _XNodeLine;		//简单测试
class _XFontUnicode;	//简单测试
class _XSCounter	//智能计数器的类
{
private:
	friend _XTexture;
	friend _XTextureInfo;
	friend _XFrame;
	friend _XFrameEx;
	friend _XNodeLine;
	friend _XFontUnicode;

	int m_counter;
	_XSCounter()
		:m_counter(1)
	{}
};
#define TEX_DEBUG (0)
#define TEX_DEBUG1 (0)
#if TEX_DEBUG
extern int XTSum;
#endif
#if TEX_DEBUG1
extern int CPSum;
#endif

class _XResourceInfo;
class _XPBO;
class _XTexture	//这个接口建议只对内，或者是用于没有裁剪的外部接口
{
private:
	_XSCounter *m_cp;	//引用计数器
	void release();	//释放图片资源
	_XBool m_isLoaded;	//是否载入
	//char * m_fileName;	//贴图资源的文件名
	_XResourceInfo * m_resInfo;	//资源的指针
public:
	GLuint m_texture;	//贴图的编号:0无效，大约0有效
	int m_w;			//贴图的像素宽度
	int m_h;			//贴图的像素高度
	_XBool getIsEnable()	//检查当前贴图是否有效
	{
		return m_isLoaded;
	}
	_XColorMode m_mode;	//0:RGBA else RGB
	void bind()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,m_texture);
	}
	_XBool reset();
public:
	//resoursePosition表示资源的位置：0表示外部资源；1：表示内部资源，将从资源文件包中读取资源	
	_XBool load(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);//从指定文件中载入图片资源
	//下面两个接口尚未实现
	_XBool createTexture(int w,int h,_XColorMode mode);			//建立一张指定大小的空白贴图
	_XBool createWithTexture(int w,int h,_XColorMode mode,unsigned int tex);	//直接使用已经存在的贴图
	void updateTexture(const _XPBO &pbo);		//使用指定数据更新贴图
	void updateTextureR2B(const _XPBO &pbo);		//使用指定数据更新贴图
	void updateTexture(const void *data,int w = 0,int h = 0,int x = 0,int y = 0);		//使用指定数据更新贴图
	void updateTextureR2B(const void *data,int w = 0,int h = 0,int x = 0,int y = 0);	//功能与上相同，不同在于红蓝色分量互换
	//void updateTextureGray(const unsigned char *data);

	_XTexture()
	:m_texture(0)
	,m_isLoaded(XFalse)
	,m_cp(NULL)
	//,m_fileName(NULL)
	,m_mode(COLOR_RGBA)
	,m_resInfo(NULL)
	{}

	~_XTexture()
	{
		if(m_cp != NULL && -- m_cp->m_counter <= 0)
		{
			release();
#if TEX_DEBUG1
			CPSum --;
			printf("%d-\n",CPSum);
#endif
			XDELETE(m_cp);
		//	XDELETE_ARRAY(m_fileName);
		}
	}
	_XTexture(const _XTexture& temp);	//拷贝构造函数
	_XTexture& operator = (const _XTexture &temp);
};
#endif
