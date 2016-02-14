#ifndef _JIA_XFONTTTF_
#define _JIA_XFONTTTF_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.22
//--------------------------------
//这是一个封装了SDL_TTF的类，关系如下：XFontTTF封装自SDL_TTF,SDL_TTF封装自Freetype
#include "../SDL_ttf/SDL_ttf.h"
#include "../XMath/XVector2.h"
namespace XE{
extern unsigned int loadTextureFromSDL(SDL_Surface *surface,XVector2& size);	//从sdl转换成OpenGL
//extern int power_of_two(int input);

#define MAX_TTF_FONT_FILE_SUM (16)	//允许载入的最大字体库数量
#ifdef _WIN64
//#pragma comment(lib, "../../engine/lib/FreeType/libfreetype64.lib")
#pragma comment(lib, "../../engine/lib/FreeType/freetype253_64.lib")
#else
#pragma comment(lib, "../../engine/lib/FreeType/freetype253.lib")
//#pragma comment(lib, "../../engine/lib/FreeType/libfreetype32.lib")
#endif

enum XTTFRenderType
{
	RENDER_LATIN1,
	RENDER_UTF8,
	RENDER_UNICODE
};
struct XFontTTFInfo
{
	XBool isEnable;							//是否有效
	//char filename[MAX_FILE_NAME_LENGTH];	//文件名
	std::string filename;
	TTF_Font *font;
	int fontSize;							//尺寸
	int widthOfSpace;						//空格键的宽度，字体也是这个宽度
	XTTFRenderType type;								//字体的类型
	SDL_Color color;						//字体的颜色

	void setType(XTTFRenderType t);
	void setColor(SDL_Color c){color = c;}
	XFontTTFInfo()
		:isEnable(XFalse)
		,font(NULL)
		,type(RENDER_UTF8)
	{
		color.r = 0xFF;
		color.g = 0xFF;
		color.b = 0xFF;
		color.unused = 0x00;
	}
};
class XFontTTF
{
public:
	XFontTTF();
	virtual ~XFontTTF(); 
protected:
	XFontTTF(const XFontTTF&);
	XFontTTF &operator= (const XFontTTF&);
private:
	XBool m_isInited;

	XFontTTFInfo m_fontInfo[MAX_TTF_FONT_FILE_SUM];
	//TTF_Font *m_font[MAX_TTF_FONT_FILE_SUM];	//字体库的指针
	int m_curLoadTTFFontFileSum;				//当前载入的字体库的数量
	
	//字体的一些基本属性
	//XTTFRenderType m_renderType;	//字体的渲染类型
	//SDL_Color m_forecol;	//字体的颜色

	SDL_Surface * getSurface(int index,const char * str);

	bool loadFromFolder(int i,const char *filename,int ptsize);	//从文件夹中载入资源
	bool loadFromPacker(int i,const char *filename,int ptsize);	//从压缩包中载入资源
	bool loadFromWeb(int i,const char *filename,int ptsize);		//从网页中读取资源
public:
	int getTTFFileOrder(const char * filename,int ptsize);	//获取符合要求的ttf字体库文件的编号
	int loadTTFFile(const char * filename,int ptsize,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);		//载入字体库文件，-1表示载入失败，成功返回编号
	int releaseTTFFile(int order);	//释放某个字体库文件
	void setAttitude(int fontOrder,XTTFRenderType type,SDL_Color color);	//设置字体的渲染参数
	void setOutline(int fontOrder,int outLine);
	void setStyle(int fontOrder,int style);
	unsigned int getTexture(int fontOrder,const char * str,XVector2& size);
	void release();	//释放资源
	//下面是为了支持XNumber,XFont,XFontUnicode而定义的接口
	XBool getTextureNumber(int fontOrder,unsigned int &tex,XVector2 &layout);
	XBool getTextureFont(int fontOrder,unsigned int &tex,XVector2 &layout);
	XBool getTextureFontUnicode(int fontOrder,unsigned int *tex,int& texSum,XVector2 &layout,XBool withBlackOutLine = XFalse);
};
#if WITH_INLINE_FILE
#include "XFontTTF.inl"
#endif
}
#endif