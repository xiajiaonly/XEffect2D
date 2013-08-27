#ifndef _JIA_XBASICOPENGL_
#define _JIA_XBASICOPENGL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
extern unsigned int srcBlendMode[9];
extern unsigned int dstBlendMode[8];
extern char XEE_isOpenGL2Support;	//OpenGL是否支持2.0是否支持GLSL
extern int XEE_MaxTetureSize;		//显卡支持的最大贴图尺寸

extern void printShaderInfoLog(GLuint obj);						//输出shader的信息的函数
extern int printOglError(char *file, int line);					//显示OpenGL的错误信息
#define printOpenGLError() printOglError(__FILE__, __LINE__)	//打印程序执行错误的函数
extern void printProgramInfoLog(GLuint obj);					//打印程序信息的函数

extern char *textFileRead(const char *filename,int resoursePosition = 0);	//从filename的文件中读取shader代码	//资源位置，0为外部资源，默认为外部资源
extern int textFileWrite(const char *filename, const char *s);					//将shader代码s写入到filename的文件中
extern void setShader(const char* vertFile,const char* fragFile,int &pHandle,int resoursePosition = 0);	//资源位置，0为外部资源，默认为外部资源

extern void changeSize(int h, int w);							//SDL中窗口改变事件的响应函数

extern int SDL_Texture(GLuint &texture,const char *filename,int withAlpha,int * w = NULL,int * h = NULL,int resoursePosition = 0);	//使用SDL载入贴图,默认的资源位置为外部
extern int SDL_TextureEx(GLuint &texture,const char *filename,int * w = NULL,int * h = NULL,int resoursePosition = 0);	//使用SDL载入贴图,默认的资源位置为外部
//这是一个没有进行更多检查的函数接口，调用之前确认资源符合各种约束
extern int SDL_TextureQuarter(GLuint &texture,const char *filename);	//以四分之一尺寸读取资源

extern int initOpenGL(int width,int height);
//在制定位置与指定位置之间划一条直线 type 0:实线，1:虚线
extern void drawLine(int ax,int ay,int bx,int by,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0);
class _XVector2;
extern void drawLine(const _XVector2& p0,const _XVector2& p1,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0);
class _XRect;
extern void drawRect(const _XRect& rect,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0);
extern void drawBox(float x,float y,float sizeX,float sizeY);
extern void drawBox(float x,float y,float sizeX,float sizeY,float angle);
//建立一张空白贴图
extern GLuint EmptyTexture(int x,int y);

extern int gameShot(const char *fileName,int w,int h);	//将屏幕上0 - w ，0 - h矩形内的图像保存成图片png文件
extern void clearScreen();					//清除屏幕的内容
extern void updateScreen();					//更新屏幕的内容

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

class _XTexture
{
private:
	_XSCounter *m_cp;	//引用计数器
	void release();	//释放图片资源
	char m_isLoaded;	//是否载入
	char * m_fileName;	//贴图资源的文件名
public:
	GLuint m_texture;	//贴图的编号:0无效，大约0有效
	int m_w;			//贴图的像素宽度
	int m_h;			//贴图的像素高度
	char getIsEnable()	//检查当前贴图是否有效
	{
		if(m_isLoaded == 0) return 0;
		return 1;
	}
public:
	//resoursePosition表示资源的位置：0表示外部资源；1：表示内部资源，将从资源文件包中读取资源	
	int load(const char *filename,int resoursePosition = 0);//从指定文件中载入图片资源

	_XTexture::_XTexture()
	:m_texture(0)
	,m_isLoaded(0)
	,m_cp(NULL)
	,m_fileName(NULL)
	{
	}
	//_XTexture(GLuint tex,int w,int h);
	_XTexture::~_XTexture()
	{

//		if(m_cp != NULL)
//		{
//			printf("|%d+%d\n",m_cp->m_counter,m_texture);
//		}
//#endif
		if(m_cp != NULL && -- m_cp->m_counter <= 0)
		{
			release();
#if TEX_DEBUG1
			CPSum --;
			printf("%d-\n",CPSum);
#endif
			XDELETE(m_cp);
			XDELETE_ARRAY(m_fileName);
		}
	}

	_XTexture(const _XTexture& temp)	//拷贝构造函数
	{
		if(this == &temp) return;		//防止自生拷贝
		if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
		m_texture = temp.m_texture;
		m_w = temp.m_w;
		m_h = temp.m_h;
		m_isLoaded = temp.m_isLoaded;
		m_cp = temp.m_cp;
		m_fileName = temp.m_fileName;
		return;	
	}
	_XTexture& operator = (const _XTexture &temp)	//赋值操作符
	{
		if(this == &temp) return *this;		//防止自生拷贝
		if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
		if(m_cp != NULL && -- m_cp->m_counter <= 0)
		{
			release();	//引用计数结束，释放资源
#if TEX_DEBUG1
			CPSum --;
			printf("%d-\n",CPSum);
#endif
			XDELETE(m_cp);
			XDELETE_ARRAY(m_fileName);
		}
		m_texture = temp.m_texture;
		m_w = temp.m_w;
		m_h = temp.m_h;
		m_isLoaded = temp.m_isLoaded;
		m_cp = temp.m_cp;
		m_fileName = temp.m_fileName;
		return *this;	
	}
};
inline int _XTexture::load(const char *filename,int resoursePosition)//从指定文件中载入图片资源
{
	if(m_isLoaded != 0) return 1;	//如果资源已经载入了，则不能重复载入
	try
	{
#if TEX_DEBUG1
		CPSum ++;
		printf("%d-\n",CPSum);
#endif
		m_cp = new _XSCounter;
		if(m_cp == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	try
	{
		m_fileName = new char[MAX_FILE_NAME_LENGTH];
		if(m_fileName == NULL) 
		{
			XDELETE(m_cp);
			return 0;
		}
	}catch(...)
	{
		XDELETE(m_cp);
		return 0;
	}
	strcpy(m_fileName,filename);
	if(SDL_TextureEx(m_texture,filename,&m_w,&m_h,resoursePosition) != 0)
	{
#if TEX_DEBUG
		++ XTSum;
		printf("%d:%d+%s\n",XTSum,m_texture,filename);
#endif
		m_isLoaded = 1;
		return 1;
	}
	XDELETE(m_cp);
	XDELETE_ARRAY(m_fileName);
	m_isLoaded = 0;
	return 0;
	//return SDL_TextureEx(m_texture,filename,&m_w,&m_h,resoursePosition);
}

inline void _XTexture::release()	//释放图片资源
{
	if(m_isLoaded == 0) return;
#if TEX_DEBUG
	-- XTSum;
	printf("%d-%d\n",XTSum,m_texture);
#endif
	if(glIsTexture(m_texture))
	{
		glDeleteTextures(1, &m_texture);
	}
	m_isLoaded = 0;
}

#define MAX_FBO_TEXTURE_SUM (256)
//下面是对FBO的支持
class _XFBO
{
private:
	GLuint m_fboId;								//FBO的ID
	//GLuint m_rboId;							//RBO的ID
	GLuint m_textureId[MAX_FBO_TEXTURE_SUM];    //FBO对应贴图的ID
	int m_nowFBOTextureSum;
public:
	int init(int w,int h); //w和h是纹理的尺寸，返回初始化是否成功
	void useFBO(int windowsW,int windowsH);
	void attachTex(int order);
	void removeFBO();
	void addOneTexture(int w,int h);
	GLuint getTexture(int order)	//存在问题
	{
		if(order >= 0 && order < m_nowFBOTextureSum)
		{
			return m_textureId[order];
		}
		return -1;
	}
	void release();
	_XFBO()
		:m_nowFBOTextureSum(0)
	{
	}
	~_XFBO()
	{
		release();
	}
};

extern int isFBOSupported();		//判断底层是否支持FBO
extern int isFramebufferReady();	//判断FBO状态是否就绪

#endif
