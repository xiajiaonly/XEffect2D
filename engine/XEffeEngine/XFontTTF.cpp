//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFontTTF.h"
#include "windows.h"
#include "XResourcePack.h"

unsigned int loadTextureFormSDL(SDL_Surface *surface,_XVector2& size)
{
	GLuint texture;
	int w, h;
	SDL_Surface *image;
	SDL_Rect area;
	Uint32 saved_flags;
	Uint8  saved_alpha;

	/* Use the surface width and height expanded to powers of 2 */
	w = getMinWellSize2n(surface->w);
	h = getMinWellSize2n(surface->h);
	size.x = w;
	size.y = h;

	image = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			w, h,
			32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			0x000000FF, 
			0x0000FF00, 
			0x00FF0000, 
			0xFF000000
#else
			0xFF000000,
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF
#endif
		       );
	if ( image == NULL ) {
		return 0;
	}

	/* Save the alpha blending attributes */
	saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
#if SDL_VERSION_ATLEAST(1, 3, 0)
	SDL_GetSurfaceAlphaMod(surface, &saved_alpha);
	SDL_SetSurfaceAlphaMod(surface, 0xFF);
#else
	saved_alpha = surface->format->alpha;
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, 0, 0);
	}
#endif

	/* Copy the surface into the GL texture image */
	area.x = 0;
	area.y = 0;
	area.w = surface->w;
	area.h = surface->h;
	SDL_BlitSurface(surface, &area, image, &area);

	/* Restore the alpha blending attributes */
#if SDL_VERSION_ATLEAST(1, 3, 0)
	SDL_SetSurfaceAlphaMod(surface, saved_alpha);
#else
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, saved_flags, saved_alpha);
	}
#endif

	/* Create an OpenGL texture for the image */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#if WITH_XSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	SDL_FreeSurface(image); /* No longer needed */

	return texture;
}
_XFontTTF::_XFontTTF()
	:m_isInited(XFalse)
	,m_nowLoadTTFFontFileSum(0)
	//,m_renderType(RENDER_UTF8)
{
//	m_forecol.r = 0xFF;
//	m_forecol.g = 0xFF;
//	m_forecol.b = 0xFF;
//	m_forecol.unused = 0x00;
}
_XFontTTF& _XFontTTF::GetInstance()
{
	static _XFontTTF m_instance;
	if(!m_instance.m_isInited)
	{//如果没有初始化则在这里初始化
		if(TTF_Init() < 0)
		{//字体库初始化失败
			printf("Error:ttf init error!\n");
		}else
		{
			m_instance.m_isInited = XTrue;	//标记初始化成功
		}
	}
	return m_instance;
}
_XFontTTF::~_XFontTTF()
{
	if(GetInstance().m_isInited)
	{
		TTF_Quit();
		GetInstance().m_isInited = XFalse;
	}
}
int _XFontTTF::getTTFFileOrder(const char * filename,int ptsize)
{
	if(filename == NULL) return -1;
	if(m_nowLoadTTFFontFileSum <= 0) return -1;
	int sum = 0;
	for(int i = 0;i < MAX_TTF_FONT_FILE_SUM;++ i)
	{
		if(m_fontInfo[i].isEnable)
		{
			if(ptsize == m_fontInfo[i].fontSize && fileNameCompare(filename,m_fontInfo[i].filename)) return i;
			++ sum;
			if(sum >= MAX_TTF_FONT_FILE_SUM) return -1;
		}
	}
	return -1;
}
int _XFontTTF::loadTTFFile(const char * filename,int ptsize,_XResourcePosition resoursePosition)
{//暂时不支持从网络读取资源
	if(!m_isInited) return 0;
	if(filename == NULL) return -1;
	if(m_nowLoadTTFFontFileSum >= MAX_TTF_FONT_FILE_SUM) return -1;
	int ret = getTTFFileOrder(filename,ptsize);
	if(ret >= 0) return ret;	//防止重复载入
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;

	for(int i = 0;i < MAX_TTF_FONT_FILE_SUM;++ i)
	{
		if(!m_fontInfo[i].isEnable)
		{
			if(resoursePosition == RESOURCE_LOCAL_PACK)
			{//从资源文件中读取资源，这里需要重新分装一下
				int lengthTemp = _XResourcePack::GetInstance().getFileLength(filename);
				unsigned char *p = createArrayMem<unsigned char>(lengthTemp + 1);
				if(p == NULL) return -1;
				_XResourcePack::GetInstance().unpackResource(filename,p);

				SDL_RWops *fileData = SDL_RWFromMem(p,lengthTemp);
				m_fontInfo[i].font = TTF_OpenFontRW(fileData,1,ptsize);
				XDELETE_ARRAY(p);
			}else
			{//从外部读取资源
				char * tmp = ANSIToUTF8(filename);
				m_fontInfo[i].font = TTF_OpenFont(tmp, ptsize);
				XDELETE_ARRAY(tmp);
			}
			if(m_fontInfo[i].font == NULL)
			{//如果字体文件加载失败的话，这里尝试从系统字体目录中加载字体文件
				LogStr("字体文件加载失败,下面尝试从系统字体目录下加载该字体!");
				std::string tmpSysFontPath = getWindowsSysFontPath();
				tmpSysFontPath += getFilenameFormPath(filename);
				m_fontInfo[i].font = TTF_OpenFont(ANSI2UTF8(tmpSysFontPath.c_str()).c_str(), ptsize);
				if(m_fontInfo[i].font == NULL)
				{
					LogNull("%s :字体文件加载失败!",tmpSysFontPath.c_str());
					return -1;
				}
			}
			m_fontInfo[i].isEnable = XTrue;
			m_fontInfo[i].fontSize = ptsize;
			m_fontInfo[i].setType(m_fontInfo[i].type);
			strcpy(m_fontInfo[i].filename,filename);
			++ m_nowLoadTTFFontFileSum;
			return i;
		}
	}

	return -1;
}
unsigned int _XFontTTF::getTexture(int fontOrder,const char * str,_XVector2& size)
{
	if(fontOrder < 0 || fontOrder >= MAX_TTF_FONT_FILE_SUM) return 0;
	if(!m_fontInfo[fontOrder].isEnable) return 0;
	SDL_Surface *surface = getSurface(fontOrder,str);
	if(surface == NULL) return 0;
	GLuint ret = loadTextureFormSDL(surface,size);
	SDL_FreeSurface(surface);
	//检查是否有错误
	GLenum gl_error = glGetError();
	if(gl_error != GL_NO_ERROR ) 
	{
		printf("Warning: Couldn't create texture: 0x%x\n",gl_error);
	}
	return ret;
}
SDL_Surface * _XFontTTF::getSurface(int index,const char * str)
{
	if(str == NULL) return NULL;
	SDL_Surface * surface = NULL;
	switch(m_fontInfo[index].type) 
	{
	    case RENDER_LATIN1:
			surface = TTF_RenderText_Blended(m_fontInfo[index].font,str,m_fontInfo[index].color);
		break;
	    case RENDER_UTF8:
			surface = TTF_RenderUTF8_Blended(m_fontInfo[index].font,str,m_fontInfo[index].color);
		break;

	    case RENDER_UNICODE:
		{
			/* This doesn't actually work because you can't pass
			   UNICODE text in via command line, AFAIK, but...
			 */
			Uint16 unicode_text[BUFSIZ];
			for(int i = 0;(str[0] || str[1]); ++i ) 
			{
				unicode_text[i]  = ((Uint8 *)str)[0];
				unicode_text[i] <<= 8;
				unicode_text[i] |= ((Uint8 *)str)[1];
				str += 2;
			}
			surface = TTF_RenderUNICODE_Blended(m_fontInfo[index].font,
				unicode_text,m_fontInfo[index].color);
		}
		break;
	    default:
		surface = NULL; /* This shouldn't happen */
		break;
	}
	return surface;
}
//计算如何排布可以获得最优的结果
void getMinSize(int& x,int& y,int sum,int sizeX,int sizeY)	//计算最合适的尺寸
{//使用遍历尝试的方法，比较傻，暂时先这样
	if(sum <= 0) return;
	int minX = sum;
	int minSize = getMinWellSize2n(sizeY) * getMinWellSize2n(sizeX * sum);
	int nowSize;
	for(int i = 1;i < sum;++ i)
	{
		if((sum % i) == 0)
		{
			int w = getMinWellSize2n(sizeX * i);
			int h = getMinWellSize2n(sizeY * (sum / i));
			nowSize = w * h;
			if(nowSize < minSize || (nowSize == minSize && h < w))	//这个优化条件需要仔细考虑
			{
				minSize = nowSize;
				minX = i;
			}
		}
	}
	x = minX;
	y = sum / x;
}
//下面的方法尚未实现
#include "XNumber.h"
_XBool _XFontTTF::getTextureNumber(int fontOrder,unsigned int &tex,_XVector2 &layout)
{
	if(fontOrder < 0 || fontOrder >= MAX_TTF_FONT_FILE_SUM) return XFalse;
	if(!m_fontInfo[fontOrder].isEnable) return XFalse;
	int len = sizeof(TEXT_MODE) - 1;
	//这里需要计算怎么样的排布是最好的
	int x,y;
	getMinSize(x,y,len,(m_fontInfo[fontOrder].fontSize >> 1),m_fontInfo[fontOrder].fontSize);
	int w = getMinWellSize2n(x * (m_fontInfo[fontOrder].fontSize >> 1));
	int h = getMinWellSize2n(y * (m_fontInfo[fontOrder].fontSize));
	//建立真实尺寸的贴图
	SDL_Surface *image = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			w, h,32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			0x000000FF,0x0000FF00,0x00FF0000,0xFF000000
#else
			0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
#endif
		       );
	if(image == NULL) return XFalse;
	//然后使用贴图
	char str[] = TEXT_MODE;
	char pStr[] = TEXT_MODE;
	for(int i = 0;i < y;++ i)
	{
		if(i * x + x <= strlen(str))
		{
			memcpy(pStr,str + i * x,x);
			pStr[x] = '\0';
			SDL_Surface * temp = getSurface(fontOrder,pStr);
			if(temp == NULL) return XFalse;
			//粘贴
			SDL_Rect dst = {0,i * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
			//当存在alpha通道时，这里这是不进行混合直接覆盖
			SDL_SetAlpha(temp,0,255);
			SDL_SetAlpha(image,0,255);
			SDL_BlitSurface(temp,0,image,&dst);	//这里会进行alpha混合(这里需要还原原有的情况)
			SDL_FreeSurface(temp);
		}else
		{
			memcpy(pStr,str + i * x,strlen(str) - i * x);
			pStr[strlen(str) - i * x] = '\0';
			SDL_Surface * temp = getSurface(fontOrder,pStr);
			if(temp == NULL) return XFalse;
			//粘贴
			SDL_Rect dst = {0,i * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
			//当存在alpha通道时，这里这是不进行混合直接覆盖
			SDL_SetAlpha(temp,0,255);
			SDL_SetAlpha(image,0,255);
			SDL_BlitSurface(temp,0,image,&dst);	//这里会进行alpha混合(这里需要还原原有的情况)
			SDL_FreeSurface(temp);
		}
	}
	//取图完毕，下面开始整理输出数据
	_XVector2 texSize;
	tex = loadTextureFormSDL(image,texSize);
	SDL_FreeSurface(image);
	layout.set(x,y);
	return XTrue;
}
#include "XFont.h"
_XBool _XFontTTF::getTextureFont(int fontOrder,unsigned int &tex,_XVector2 &layout)
{
	if(fontOrder < 0 || fontOrder >= MAX_TTF_FONT_FILE_SUM) return XFalse;
	if(!m_fontInfo[fontOrder].isEnable) return XFalse;
	int len = sizeof(TEXT_FONT_MODE) - 1;
	//这里需要计算怎么样的排布是最好的
	int x,y;
	getMinSize(x,y,len,(m_fontInfo[fontOrder].fontSize >> 1),m_fontInfo[fontOrder].fontSize);
	int w = getMinWellSize2n(x * (m_fontInfo[fontOrder].fontSize >> 1));
	int h = getMinWellSize2n(y * (m_fontInfo[fontOrder].fontSize));
	//建立真实尺寸的贴图
	SDL_Surface *image = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			w, h,32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			0x000000FF,0x0000FF00,0x00FF0000,0xFF000000
#else
			0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
#endif
		       );
	if(image == NULL) return XFalse;
	//然后使用贴图
	char str[] = TEXT_FONT_MODE;
	char pStr[] = TEXT_FONT_MODE;
	for(int i = 0;i < y;++ i)
	{
		if(i * x + x <= strlen(str))
		{
			memcpy(pStr,str + i * x,x);
			pStr[x] = '\0';
			SDL_Surface * temp = getSurface(fontOrder,pStr);
			if(temp == NULL) return XFalse;
			//粘贴
			SDL_Rect dst = {0,i * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
			//当存在alpha通道时，这里这是不进行混合直接覆盖
			SDL_SetAlpha(temp,0,255);
			SDL_SetAlpha(image,0,255);
			SDL_BlitSurface(temp,0,image,&dst);	//这里会进行alpha混合(这里需要还原原有的情况)
			SDL_FreeSurface(temp);
		}else
		{
			memcpy(pStr,str + i * x,strlen(str) - i * x);
			pStr[strlen(str) - i * x] = '\0';
			SDL_Surface * temp = getSurface(fontOrder,pStr);
			if(temp == NULL) return XFalse;
			//粘贴
			SDL_Rect dst = {0,i * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
			//当存在alpha通道时，这里这是不进行混合直接覆盖
			SDL_SetAlpha(temp,0,255);
			SDL_SetAlpha(image,0,255);
			SDL_BlitSurface(temp,0,image,&dst);	//这里会进行alpha混合(这里需要还原原有的情况)
			SDL_FreeSurface(temp);
			break;
		}
	}
	//取图完毕，下面开始整理输出数据
	_XVector2 texSize;
	tex = loadTextureFormSDL(image,texSize);
	SDL_FreeSurface(image);
	layout.set(x,y);
	return XTrue;
}
#include "XFontUnicode.h"
_XBool _XFontTTF::getTextureFontUnicode(int fontOrder,unsigned int *tex,int& texSum,_XVector2 &layout,_XBool withBlackOutLine)
{
	if(fontOrder < 0 || fontOrder >= MAX_TTF_FONT_FILE_SUM) return XFalse;
	if(!m_fontInfo[fontOrder].isEnable) return XFalse;
	//这里需要计算怎么样的排布是最好的
	int x = 1024 / m_fontInfo[fontOrder].fontSize;	//默认使用1024*1024的尺寸
	int y = 1024 / m_fontInfo[fontOrder].fontSize;
	int wByC = (x << 1);
	int hByC = (y << 1);
	int w = 1024;
	int h = 1024;
#if !WITH_FULL_ALL_CHINESE
	int len = getStrLen(TEXT_FONT_UNICODE_MODE);
#else
	int len = getStrLen(TEXT_FONT_UNICODE_MODE0);
	len += getStrLen(TEXT_FONT_UNICODE_MODE1);
	len += getStrLen(TEXT_FONT_UNICODE_MODE2);
	len += getStrLen(TEXT_FONT_UNICODE_MODE3);
#endif
	texSum = len / (x * y);
	if(texSum * x * y < len) ++ texSum;
	//扩展这个字符串，使得可以便于使用
#if !WITH_FULL_ALL_CHINESE
	int length = strlen(TEXT_FONT_UNICODE_MODE);
#else
	int length = strlen(TEXT_FONT_UNICODE_MODE0);
	length += strlen(TEXT_FONT_UNICODE_MODE1);
	length += strlen(TEXT_FONT_UNICODE_MODE2);
	length += strlen(TEXT_FONT_UNICODE_MODE3);
#endif
	char *str = createArrayMem<char>(length + 128);
	if(str == NULL) return XFalse;
#if !WITH_FULL_ALL_CHINESE
	char tempStr[] = TEXT_FONT_UNICODE_MODE;
#else
	std::string tmp = TEXT_FONT_UNICODE_MODE0;
	tmp += TEXT_FONT_UNICODE_MODE1;
	tmp += TEXT_FONT_UNICODE_MODE2;
	tmp += TEXT_FONT_UNICODE_MODE3;
	const char *tempStr = tmp.c_str();
#endif
	int j = 0;
	for(int i = 0;i < length;)
	{
		if(tempStr[i] < 0)
		{
			str[j] = tempStr[i];
			str[j + 1] = tempStr[i + 1];
			j += 2;
			i += 2;
		}else
		{//ASCII
			str[j] = tempStr[i];
			str[j + 1] = ' ';
			j += 2;
			++ i;
		}
	}
	str[j] = '\0';
	str[j + 1] = '\0';
	//下面依次建立贴图
	int maxLen = strlen(str);
#if !WITH_FULL_ALL_CHINESE
	char pStr[] = TEXT_FONT_UNICODE_MODE;
#else
	char *pStr = createArrayMem<char>(maxLen);
	if(pStr == NULL) 
	{
		XDELETE_ARRAY(str);
		return XFalse;
	}
#endif
	char *pStr1;
	for(int i = 0;i < texSum;++ i)
	{//建立真实尺寸的贴图
		SDL_Surface *image = SDL_CreateRGBSurface(
				SDL_SWSURFACE,
				w, h,32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
				0x000000FF,0x0000FF00,0x00FF0000,0xFF000000
#else
				0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
#endif
				   );
		if(image == NULL)
		{
			XDELETE_ARRAY(str);
#if WITH_FULL_ALL_CHINESE
			XDELETE_ARRAY(pStr);
#endif
			return XFalse;
		}
		//然后使用贴图
		for(j = 0;j < y;++ j)
		{
			if(j * (wByC) + i * x * (hByC) + (wByC) <= maxLen)
			{
				memcpy(pStr,str + j * (wByC) + i * x * (hByC),wByC);
				pStr[wByC] = '\0';
				pStr[(wByC) + 1] = '\0';
				if(m_fontInfo[fontOrder].widthOfSpace == m_fontInfo[fontOrder].fontSize)
				{//去掉不必要的空格
					int removeSum = 0;
					for(int tw = 0;tw < x;++ tw)
					{
						if(pStr[(tw << 1) + 1] == ' ' && pStr[(tw << 1)] > 0)
						{//ASCII
							pStr[(tw << 1) - removeSum] = pStr[(tw << 1)];
							++ removeSum;
						}else
						{
							pStr[(tw << 1) - removeSum] = pStr[(tw << 1)];
							pStr[(tw << 1) - removeSum + 1] = pStr[(tw << 1) + 1];
						}
					}
					pStr[(wByC) - removeSum] = '\0';
					pStr[(wByC) - removeSum + 1] = '\0';
				}
				pStr1 = ANSIToUTF8(pStr);
				SDL_Surface * temp = getSurface(fontOrder,pStr1);
				XDELETE_ARRAY(pStr1);
				if(temp == NULL)
				{
					XDELETE_ARRAY(str);
#if WITH_FULL_ALL_CHINESE
					XDELETE_ARRAY(pStr);
#endif
					return XFalse;
				}
				//if(withBlackOutLine)
				//{
				//	unsigned char * p = (unsigned char *)temp->pixels;
				//	for(int ti = 0;ti < temp->w*temp->h*4;ti += 4)
				//	{
				//		p[ti + 0] = 0;
				//		p[ti + 1] = 0;
				//		p[ti + 2] = 0;
				//	}
				//	SDL_Rect dst0 = {1,j * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
				//	SDL_SetAlpha(temp,0,255);
				//	SDL_SetAlpha(image,0,255);
				//	SDL_BlitSurface(temp,0,image,&dst0);	//这里直接拷贝
				//	SDL_SetAlpha(temp,SDL_SRCALPHA,0);
				//	SDL_Rect dst1 = {-1,j * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
				//	、、SDL_SetAlpha(image,SDL_SRCALPHA,0);
				//	SDL_BlitSurface(temp,0,image,&dst1);	//这里会进行alpha混合(这里需要还原原有的情况)
				//	SDL_Rect dst2 = {0,j * m_fontInfo[fontOrder].fontSize + 1,temp->w,temp->h};
				//	SDL_BlitSurface(temp,0,image,&dst2);	//这里会进行alpha混合(这里需要还原原有的情况)
				//	SDL_Rect dst3 = {0,j * m_fontInfo[fontOrder].fontSize - 1,temp->w,temp->h};
				//	SDL_BlitSurface(temp,0,image,&dst3);	//这里会进行alpha混合(这里需要还原原有的情况)
				//	for(int ti = 0;ti < temp->w*temp->h*4;ti += 4)
				//	{
				//		p[ti + 0] = 255;
				//		p[ti + 1] = 255;
				//		p[ti + 2] = 255;
				//	}
				//}
				//粘贴
				SDL_Rect dst = {0,j * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
				//当存在alpha通道时，这里这是不进行混合直接覆盖
				//if(!withBlackOutLine)
				{
					SDL_SetAlpha(temp,0,255);
					SDL_SetAlpha(image,0,255);
				}
				SDL_BlitSurface(temp,0,image,&dst);	//这里会进行alpha混合(这里需要还原原有的情况)
				SDL_FreeSurface(temp);
			}else
			{
				memcpy(pStr,str + j * (wByC) + i * x * (hByC),maxLen - (j * (wByC) + i * x * (hByC)));
				pStr[maxLen - (j * (wByC) + i * x * (hByC))] = '\0';
				pStr[maxLen - (j * (wByC) + i * x * (hByC)) + 1] = '\0';
				if(m_fontInfo[fontOrder].widthOfSpace == m_fontInfo[fontOrder].fontSize)
				{//去掉不必要的空格
					int removeSum = 0;
					for(int tw = 0;tw < x;++ tw)
					{
						if(pStr[(tw << 1) + 1] == ' ' && pStr[(tw << 1)] > 0)
						{//ASCII
							pStr[(tw << 1) - removeSum] = pStr[(tw << 1)];
							++ removeSum;
						}else
						{
							pStr[(tw << 1) - removeSum] = pStr[(tw << 1)];
							pStr[(tw << 1) - removeSum + 1] = pStr[(tw << 1) + 1];
						}
					}
					pStr[(wByC) - removeSum] = '\0';
					pStr[(wByC) - removeSum + 1] = '\0';
				}
				pStr1 = ANSIToUTF8(pStr);
				SDL_Surface * temp = getSurface(fontOrder,pStr1);
				XDELETE_ARRAY(pStr1);
				if(temp == NULL)
				{
					XDELETE_ARRAY(str);
#if WITH_FULL_ALL_CHINESE
					XDELETE_ARRAY(pStr);
#endif
					return XFalse;
				}
				//粘贴
				//if(withBlackOutLine)
				//{
				//	unsigned char * p = (unsigned char *)temp->pixels;
				//	for(int ti = 0;ti < temp->w*temp->h*4;ti += 4)
				//	{
				//		p[ti + 0] = 0;
				//		p[ti + 1] = 0;
				//		p[ti + 2] = 0;
				//	}
				//	SDL_Rect dst0 = {1,j * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
				//	SDL_SetAlpha(temp,0,255);
				//	SDL_SetAlpha(image,0,255);
				//	SDL_BlitSurface(temp,0,image,&dst0);	//这里会进行alpha混合(这里需要还原原有的情况)
				//	SDL_SetAlpha(temp,SDL_SRCALPHA,0);
				//	SDL_Rect dst1 = {-1,j * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
				//	//SDL_SetAlpha(image,0,255);
				//	SDL_BlitSurface(temp,0,image,&dst1);	//这里会进行alpha混合(这里需要还原原有的情况)
				//	SDL_Rect dst2 = {0,j * m_fontInfo[fontOrder].fontSize - 1,temp->w,temp->h};
				//	SDL_BlitSurface(temp,0,image,&dst2);	//这里会进行alpha混合(这里需要还原原有的情况)
				//	SDL_Rect dst3 = {0,j * m_fontInfo[fontOrder].fontSize + 1,temp->w,temp->h};
				//	SDL_BlitSurface(temp,0,image,&dst3);	//这里会进行alpha混合(这里需要还原原有的情况)
				//	for(int ti = 0;ti < temp->w*temp->h*4;ti += 4)
				//	{
				//		p[ti + 0] = 255;
				//		p[ti + 1] = 255;
				//		p[ti + 2] = 255;
				//	}
				//}
				SDL_Rect dst = {0,j * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
				//if(!withBlackOutLine)
				{
					//当存在alpha通道时，这里这是不进行混合直接覆盖
					SDL_SetAlpha(temp,0,255);
					SDL_SetAlpha(image,0,255);
				}
				SDL_BlitSurface(temp,0,image,&dst);	//这里会进行alpha混合(这里需要还原原有的情况)
				SDL_FreeSurface(temp);
				break;	//不完整的一行，结束
			}
		}
		//取图完毕，下面开始整理输出数据
		if(withBlackOutLine)
		{//增加黑色的外框,存在结果和源互相影响的问题
			unsigned char * p = (unsigned char *)image->pixels;
			unsigned char * px = createArrayMem<unsigned char>(w * h * 4);
			if(px == NULL) 
			{
				XDELETE_ARRAY(str);
#if WITH_FULL_ALL_CHINESE
				XDELETE_ARRAY(pStr);
#endif
				return XFalse;
			}
			memcpy(px,p,w * h * 4);
			int index = 0;
			int sum,a;
			int tempW = w << 2;
			for(int th = 0;th < h;++ th)
			{
				index = th * tempW;
				for(int tw = 0;tw < w;++ tw,index += 4)
				{
					if(px[index + 3] < 127)
					{
						if((tw > 0 && px[index - 4 + 3] >= 127)
							|| (tw < w - 1 && px[index + 4 + 3] >= 127)
							|| (th > 0 && px[index - tempW + 3] >= 127)
							|| (th < h - 1 && px[index + tempW + 3] >= 127)
							|| (tw > 0 && th > 0 && px[index - 4 - tempW + 3] >= 127)
							|| (tw > 0 && th < h - 1 && px[index - 4 + tempW + 3] >= 127)
							|| (tw < w - 1 && th > 0 && px[index + 4 - tempW + 3] >= 127)
							|| (tw < w - 1 && th < h - 1 && px[index + 4 + tempW + 3] >= 127))
						{
							sum = 1;
							a = px[index + 3];
							if(tw > 0){a += px[index - 4 + 3];++sum;}
							if(tw < w - 1){a += px[index + 4 + 3];++sum;}
							if(th > 0){a += px[index - tempW + 3];++sum;}
							if(th < h - 1){a += px[index + tempW + 3];++sum;}
							if(tw > 0 && th > 0){a += px[index - 4 - tempW + 3];++sum;}
							if(tw > 0 && th < h - 1){a += px[index - 4 + tempW + 3];++sum;}
							if(tw < w - 1 && th > 0){a += px[index + 4 - tempW + 3];++sum;}
							if(tw < w - 1 && th < h - 1){a += px[index + 4 + tempW + 3];++sum;}
							a = a / sum;
							if(a > 127) a = 127;
							p[index + 3] = a << 1;	//目前较为理想的值
							//p[index + 3] = 255;
							p[index + 0] = 0;
							p[index + 1] = 0;
							p[index + 2] = 0;
						}else
						{
							p[index + 0] = 0;
							p[index + 1] = 0;
							p[index + 2] = 0;
							//p[index + 3] = px[index + 3] << 1;
						}
					}else
					{
						p[index + 0] = p[index + 0] * px[index + 3] / 255;
						p[index + 1] = p[index + 1] * px[index + 3] / 255;
						p[index + 2] = p[index + 2] * px[index + 3] / 255;
						p[index + 3] = 255;
					}
				}
			}
			XDELETE_ARRAY(px);
		}
		_XVector2 texSize;
		tex[i] = loadTextureFormSDL(image,texSize);
		SDL_FreeSurface(image);
	}
	layout.set(x,y);
	XDELETE_ARRAY(str);
#if WITH_FULL_ALL_CHINESE
	XDELETE_ARRAY(pStr);
#endif
	return XTrue;
}