#ifndef __IMG_SAVETOPNG_H__
#define __IMG_SAVETOPNG_H__
//++++++++++++++++++++++++++++++++
//Author:	未知(这部分代码从网上获取)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//#define IMG_COMPRESS_OFF 0
//#define IMG_COMPRESS_MAX 9
//#define IMG_COMPRESS_DEFAULT -1

#define Z_NO_COMPRESSION 0
#define Z_BEST_COMPRESSION 9
#define Z_DEFAULT_COMPRESSION -1
#include "SDL.h"
#include "XOSDefine.h"

/**
 * Takes a filename, a surface to save, and a compression level.  The
 * compression level can be 0(min) through 9(max), or -1(default).
 */
extern int savePNG(const char *fileName,
			SDL_Surface *surf,
			int compression = Z_BEST_COMPRESSION);

//下面是对这个模块的一个扩展
//贾胜华
#include <string>
#include "XBasicFun.h"
//compression 0 - 9,-1 default
extern int savePNG(const std::string &filename,
			const unsigned char * data,int w,int h,_XColorMode color,
			int compression = Z_BEST_COMPRESSION);
//直接将数据保存成文件
extern _XBool savePngRGB2RGB(const char *fileName,
			const unsigned char * data,
			int w,int h,
			int compression = Z_BEST_COMPRESSION);	//传入的数据为RGB，保存的文件为RGB
extern _XBool savePngRGB2RGBA(const char *fileName,
			const unsigned char * data,
			int w,int h,
			int compression = Z_BEST_COMPRESSION);	//传入的数据位RGB，保存的文件为RGBA，A = 255
extern _XBool savePngRGBA2RGBA(const char *fileName,
			const unsigned char * data,
			int w,int h,
			int compression = Z_BEST_COMPRESSION);	//传入的数据位RGB，保存的文件为RGBA，A = 255
extern _XBool savePngRGBA2RGB(const char *fileName,
			const unsigned char * data,
			int w,int h,
			int compression = Z_BEST_COMPRESSION);	//传入的数据位RGB，保存的文件为RGBA，A = 255

#endif/*__IMG_SAVETOPNG_H__*/
