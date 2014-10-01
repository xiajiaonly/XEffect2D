#ifndef _JIA_XFONTTTF_
#define _JIA_XFONTTTF_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.22
//--------------------------------
//这是一个封装了SDL_TTF的类，关系如下：_XFontTTF封装自SDL_TTF,SDL_TTF封装自Freetype
#include "XEffeEngine.h"
#include "../SDL_ttf/SDL_ttf.h"


extern unsigned int loadTextureFormSDL(SDL_Surface *surface,_XVector2& size);	//从sdl转换成OpenGL
//extern int power_of_two(int input);

#define MAX_TTF_FONT_FILE_SUM (128)	//允许载入的最大字体库数量

#pragma comment(lib, "../../engine/lib/FreeType/freetype253.lib")

enum _XTTFRenderType
{
	RENDER_LATIN1,
	RENDER_UTF8,
	RENDER_UNICODE
};
struct _XFontTTFInfo
{
	_XBool isEnable;							//是否有效
	char filename[MAX_FILE_NAME_LENGTH];	//文件名
	TTF_Font *font;
	int fontSize;							//尺寸
	int widthOfSpace;						//空格键的宽度，字体也是这个宽度
	_XTTFRenderType type;								//字体的类型
	SDL_Color color;						//字体的颜色

	void setType(_XTTFRenderType t)
	{
		type = t;
		if(font == NULL) return;
		switch(t)
		{
		case RENDER_LATIN1:
			TTF_SizeText(font," ",&(widthOfSpace),NULL);
			break;
		case RENDER_UTF8:
			TTF_SizeUTF8(font," ",&(widthOfSpace),NULL);
			break;
		case RENDER_UNICODE:
			{//这里可能存在问题
				char tempStr[4] = "  ";
				tempStr[2] = '\0';
				tempStr[3] = '\0';
				TTF_SizeUNICODE(font,(Uint16 *)tempStr,&(widthOfSpace),NULL);
			}
			break;
		}
	}
	void setColor(SDL_Color c){color = c;}
	_XFontTTFInfo()
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
class _XFontTTF
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//下面需要将其设计为Singleton模式
protected:
	_XFontTTF();
	_XFontTTF(const _XFontTTF&);
	_XFontTTF &operator= (const _XFontTTF&);
	virtual ~_XFontTTF(); 
public:
	static _XFontTTF& GetInstance();
	//-------------------------------------------
private:
	_XBool m_isInited;

	_XFontTTFInfo m_fontInfo[MAX_TTF_FONT_FILE_SUM];
	//TTF_Font *m_font[MAX_TTF_FONT_FILE_SUM];	//字体库的指针
	int m_nowLoadTTFFontFileSum;				//当前载入的字体库的数量
	
	//字体的一些基本属性
	//_XTTFRenderType m_renderType;	//字体的渲染类型
	//SDL_Color m_forecol;	//字体的颜色

	SDL_Surface * getSurface(int index,const char * str);
public:
	int getTTFFileOrder(const char * filename,int ptsize);	//获取符合要求的ttf字体库文件的编号
	int loadTTFFile(const char * filename,int ptsize,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);		//载入字体库文件，-1表示载入失败，成功返回编号
	int releaseTTFFile(int order)	//释放某个字体库文件
	{
		if(order < 0 || order >= MAX_TTF_FONT_FILE_SUM) return 0;
		if(!m_fontInfo[order].isEnable) return 0;

		TTF_CloseFont(m_fontInfo[order].font);
		m_fontInfo[order].isEnable = XFalse;
		m_fontInfo[order].font = NULL;
		-- m_nowLoadTTFFontFileSum;
		return 1;
	}
	void setAttitude(int fontOrder,_XTTFRenderType type,SDL_Color color)	//设置字体的渲染参数
	{//这里调用会造成逻辑上的不完整
		if(fontOrder >= 0 && fontOrder < MAX_TTF_FONT_FILE_SUM && m_fontInfo[fontOrder].isEnable) 
		{
			m_fontInfo[fontOrder].setType(type);
			m_fontInfo[fontOrder].setColor(color);
		}
	//	m_renderType = type;
	//	m_forecol = color;
	}
	void setOutline(int fontOrder,int outLine)
	{
		if(fontOrder >= 0 && fontOrder < MAX_TTF_FONT_FILE_SUM && m_fontInfo[fontOrder].isEnable) 
		{
			TTF_SetFontOutline(m_fontInfo[fontOrder].font,outLine);
		}
	}
	void setStyle(int fontOrder,int style)
	{
		if(fontOrder >= 0 && fontOrder < MAX_TTF_FONT_FILE_SUM && m_fontInfo[fontOrder].isEnable) 
		{
			TTF_SetFontStyle(m_fontInfo[fontOrder].font,style);
		}
	}
	unsigned int getTexture(int fontOrder,const char * str,_XVector2& size);
	void release()	//释放资源
	{
		for(int i = 0;i < MAX_TTF_FONT_FILE_SUM;++ i)
		{
			if(m_fontInfo[i].isEnable) 
			{
				TTF_CloseFont(m_fontInfo[i].font);
				m_fontInfo[i].isEnable = XFalse;
				m_fontInfo[i].font = NULL;
			}
		}
		m_nowLoadTTFFontFileSum = 0;
	}
	//下面是为了支持_XNumber,_XFont,_XFontUnicode而定义的接口
	_XBool getTextureNumber(int fontOrder,unsigned int &tex,_XVector2 &layout);
	_XBool getTextureFont(int fontOrder,unsigned int &tex,_XVector2 &layout);
	_XBool getTextureFontUnicode(int fontOrder,unsigned int *tex,int& texSum,_XVector2 &layout,_XBool withBlackOutLine = XFalse);
};
#endif