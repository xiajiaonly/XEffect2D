#ifndef _JIA_XBASICOPENGL_
#define _JIA_XBASICOPENGL_
//#include "gl.h"
//#include "glew.h"
//#include "stdlib.h"
#include "XBasicFun.h"
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
extern _XBool setShader(const char* vertFile,const char* fragFile,int &pHandle,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//资源位置，0为外部资源，默认为外部资源

extern void changeSize(int h, int w);							//SDL中窗口改变事件的响应函数

//extern _XBool TextureLoad(GLuint &texture,const char *filename,int withAlpha,int * w = NULL,int * h = NULL,
//					   _XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//使用SDL载入贴图,默认的资源位置为外部
extern _XBool TextureLoadEx(GLuint &texture,const char *filename,int * w = NULL,int * h = NULL,
						 _XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//使用SDL载入贴图,默认的资源位置为外部
//这是一个没有进行更多检查的函数接口，调用之前确认资源符合各种约束
extern _XBool TextureLoadQuarter(GLuint &texture,const char *filename);	//以四分之一尺寸读取资源

extern _XBool initOpenGL(int width,int height);
extern _XBool initOpenGL3D(int width,int height);
extern _XBool initOpenGLEx(const _XWindowData &windowData);
//最简单的划线函数
extern void drawLine(int ax,int ay,int bx,int by,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f);
//在制定位置与指定位置之间划一条直线 type 0:实线，1:虚线
extern void drawLine(int ax,int ay,int bx,int by,int width,float r,float g,float b,float a,int type = 0);
class _XVector2;
extern void drawLine(const _XVector2& p0,const _XVector2& p1,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0);
extern void drawLine(const _XVector2& p,float angle,float length);	//angle[0 - 360]
class _XRect;
//画线框，实线或者虚线
extern void drawRect(const _XRect& rect,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0);
//画线框
extern void drawBox(float x,float y,float sizeX,float sizeY,int w,float r,float g,float b,float a);
extern void drawBox(float x,float y,float sizeX,float sizeY,int w,float r = 1.0f,float g = 1.0f,float b = 1.0f);
extern void drawBox(float x,float y,float sizeX,float sizeY);
extern void drawBox(float x,float y,float sizeX,float sizeY,float angle);
extern void drawBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f);
extern void drawBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,int width,float r,float g,float b,float a);
//画点
extern void drawPoint(int x,int y,float r,float g,float b,float a);
extern void drawPoint(int x,int y,float size,float r,float g,float b,float a);
//画圆形
//px,px:位置
//sr:半径
//an:边数量
extern void drawCircle(float px, float py, float sr, int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
//画实心矩形
extern void drawFillBox(const _XVector2& pos,const _XVector2& size,
						float r = 1.0f,float g = 1.0f,float b = 1.0f);	//描绘实心矩形
extern void drawFillBoxEx(const _XVector2& pos,const _XVector2& size,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,
						bool withLine = false,bool withRoundCorner = false);	//描绘实心矩形
extern void drawFillBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,
						float r = 1.0f,float g = 1.0f,float b = 1.0f);	//描绘实心四边形
extern void drawFillBoxEx(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,
						bool withLine = false);	//描绘实心四边形
extern void drawFillBoxBW(const _XVector2& pos,const _XVector2& size,const _XVector2& cSize);	//描绘黑白色的间隔
//下面的函数用于描绘一张贴图
class _XTexture;
extern void drawTexture(_XTexture &tex,_XVector2 &pos,int cW,int cH,_XVector2 *vArray,_XVector2 *uArray,int arrayW,int arrayH);	//使用WITH_XSPRITE_EX的时候需要进行贴图归一化
extern void drawTexture(unsigned int tex,const _XVector2 &pos,int texW,int texH,bool blend = false);
extern void drawTextureEx(unsigned int tex,const _XVector2 &pos,_XVector2 *vArray,_XVector2 *uArray,int arrayW,int arrayH,bool blend = false);
//建立一张空白贴图0:RGBA,1:RGB,2:灰度图
extern GLuint emptyTexture(int x,int y,_XColorMode mode = COLOR_RGBA);	//使用RGBA的格式建立一张指定大小的空白贴图
//withThread：是否使用多线程
//将屏幕从点x，y开始的w和h矩形内的图像保存到png文件
extern _XBool gameShot(const char *fileName,int w,int h,bool withThread = true,float x = 0.0f,float y = 0.0f);

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
	{
	}
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
