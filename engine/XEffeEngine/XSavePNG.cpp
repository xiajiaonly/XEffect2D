#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	未知(这部分代码从网上获取)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//#include <stdlib.h>
#include <SDL.h>
#include <SDL_byteorder.h>
#include <png.h>
#include "XSavePNG.h"
namespace XE{
static void pngWriteData(png_structp pStruct,png_bytep pData, png_size_t length)
{
	SDL_RWwrite((SDL_RWops*) png_get_io_ptr(pStruct),pData,1,length);
}
int IMG_SavePNG_RW(SDL_RWops *src, SDL_Surface *surf,int compression)
{
	if(src == NULL || surf == NULL) return -1;//传入参数错误
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	SDL_PixelFormat *fmt = NULL;
	SDL_Surface *tempsurf = NULL;
	int ret = -1;
	int funky_format = 0;
	int used_alpha,i;
	unsigned int temp_alpha;
	png_colorp palette = NULL;
	Uint8 *palette_alpha = NULL;
	png_byte **row_pointers = NULL;

	row_pointers = (png_byte **)malloc(surf->h * sizeof(png_byte*));
	if(row_pointers == NULL) 
	{//内存分配失败
		SDL_SetError("Couldn't allocate memory for rowpointers");
		return ret;
	}
	
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL,NULL,NULL);
	if(png_ptr == NULL)
	{//建立png结构失败
		SDL_SetError("Couldn't allocate memory for PNG file");
		free(row_pointers);
		row_pointers = NULL;
		return ret;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL)
	{//建立png信息结构失败
		SDL_SetError("Couldn't allocate image information for PNG file");
		png_destroy_write_struct(&png_ptr,&info_ptr);
		free(row_pointers);
		row_pointers = NULL;
		return ret;
	}
	/* setup custom writer functions */
	png_set_write_fn(png_ptr,(png_voidp)src,pngWriteData,NULL);

	if(_setjmp(png_jmpbuf(png_ptr)))
	{
		SDL_SetError("Unknown error writing PNG");
		goto savedone;
	}

	if(compression > Z_BEST_COMPRESSION) compression = Z_BEST_COMPRESSION;
	if(compression == Z_NO_COMPRESSION) // No compression
	{
		png_set_filter(png_ptr,0,PNG_FILTER_NONE);
		png_set_compression_level(png_ptr,Z_NO_COMPRESSION);
	}else 
	if(compression < 0) png_set_compression_level(png_ptr,Z_DEFAULT_COMPRESSION); // Default compression
	else png_set_compression_level(png_ptr,compression);

	//根据色深处理图像
	fmt = surf->format;
	if(fmt->BitsPerPixel == 8)
	{ /* Paletted */
		png_set_IHDR(png_ptr,info_ptr,
			surf->w,surf->h,8,PNG_COLOR_TYPE_PALETTE,
			PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,
			PNG_FILTER_TYPE_DEFAULT);
		palette = (png_colorp) malloc(fmt->palette->ncolors * sizeof(png_color));
		if(palette == NULL) 
		{//内存分配失败
			SDL_SetError("Couldn't create memory for palette");
			goto savedone;
		}
		for(i = 0;i < fmt->palette->ncolors;++ i) 
		{
			palette[i].red = fmt->palette->colors[i].r;
			palette[i].green = fmt->palette->colors[i].g;
			palette[i].blue = fmt->palette->colors[i].b;
		}
		png_set_PLTE(png_ptr,info_ptr,palette,fmt->palette->ncolors);
		if(surf->flags&SDL_SRCCOLORKEY) 
		{
			palette_alpha = (Uint8 *)malloc((fmt->colorkey+1) * sizeof(Uint8));
			if(palette_alpha == NULL) 
			{//内存分配失败
				SDL_SetError("Couldn't create memory for palette transparency");
				goto savedone;
			}
			/* FIXME: memset? */
		//	for (i = 0;i < (fmt->colorkey + 1);++i) 
		//	{
		//		palette_alpha[i] = 255;
		//	}
			memset(palette_alpha,255,(fmt->colorkey+1) * sizeof(Uint8));
			palette_alpha[fmt->colorkey] = 0;
			png_set_tRNS(png_ptr,info_ptr,palette_alpha,fmt->colorkey+1,NULL);
		}
	}else
	{ /* Truecolor */
		if(fmt->Amask != 0) 
		{//带有alpha通道
			png_set_IHDR(png_ptr,info_ptr,
				surf->w,surf->h,8,PNG_COLOR_TYPE_RGB_ALPHA,
				PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);
		}else 
		{
			png_set_IHDR(png_ptr,info_ptr,
				surf->w,surf->h,8,PNG_COLOR_TYPE_RGB,
				PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);
		}
	}
	png_write_info(png_ptr, info_ptr);

	if (fmt->BitsPerPixel == 8) 
	{ /* Paletted */
		for(i = 0;i < surf->h;++ i)
		{
			row_pointers[i] = ((png_byte*)surf->pixels) + i * surf->pitch;
		}
		if(SDL_MUSTLOCK(surf)) SDL_LockSurface(surf);
		png_write_image(png_ptr, row_pointers);
		if(SDL_MUSTLOCK(surf)) SDL_UnlockSurface(surf);
	}else
	{ /* Truecolor */
		if(fmt->BytesPerPixel == 3)
		{
			if(fmt->Amask != 0)
			{ /* check for 24 bit with alpha */
				funky_format = 1;
			}else
			{
				/* Check for RGB/BGR/GBR/RBG/etc surfaces.*/
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				if(fmt->Rmask!=0xFF0000 
				|| fmt->Gmask!=0x00FF00
				|| fmt->Bmask!=0x0000FF)
#else
				if(fmt->Rmask!=0x0000FF 
				|| fmt->Gmask!=0x00FF00
				|| fmt->Bmask!=0xFF0000)
#endif
				{
					funky_format=1;
				}
			}
		}else 
		if(fmt->BytesPerPixel == 4)
		{
			if (fmt->Amask == 0) 
			{ /* check for 32bit but no alpha */
				funky_format = 1; 
			}else
			{
				/* Check for ARGB/ABGR/GBAR/RABG/etc surfaces.*/
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				if(fmt->Rmask!=0xFF000000
				|| fmt->Gmask!=0x00FF0000
				|| fmt->Bmask!=0x0000FF00
				|| fmt->Amask!=0x000000FF)
#else
				if(fmt->Rmask!=0x000000FF
				|| fmt->Gmask!=0x0000FF00
				|| fmt->Bmask!=0x00FF0000
				|| fmt->Amask!=0xFF000000)
#endif
				{
					funky_format=1;
				}
			}
		}else
		{ /* 555 or 565 16 bit color */
			funky_format = 1;
		}
		if(funky_format) 
		{
			/* Allocate non-funky format, and copy pixeldata in*/
			if(fmt->Amask != 0)
			{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				tempsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surf->w, surf->h, 24,
										0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#else
				tempsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surf->w, surf->h, 24,
										0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
#endif
			}else
			{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				tempsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surf->w, surf->h, 24,
										0xff0000, 0x00ff00, 0x0000ff, 0x00000000);
#else
				tempsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surf->w, surf->h, 24,
										0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000);
#endif
			}
			if(tempsurf == NULL)
			{//建立surface失败
				SDL_SetError("Couldn't allocate temp surface");
				goto savedone;
			}
			if(surf->flags & SDL_SRCALPHA)
			{
				temp_alpha = fmt->alpha;
				used_alpha = 1;
				SDL_SetAlpha(surf,0,255); /* Set for an opaque blit */
			}else
			{
				used_alpha = 0;
			}
			if(SDL_BlitSurface(surf,NULL,tempsurf,NULL) != 0)
			{
				SDL_SetError("Couldn't blit surface to temp surface");
				SDL_FreeSurface(tempsurf);
				tempsurf = NULL;
				goto savedone;
			}
			if(used_alpha) SDL_SetAlpha(surf,SDL_SRCALPHA,(Uint8)temp_alpha); /* Restore alpha settings*/
			for(i = 0;i < tempsurf->h;++ i)
			{
				row_pointers[i] = ((png_byte*)tempsurf->pixels) + i * tempsurf->pitch;
			}
			if(SDL_MUSTLOCK(tempsurf)) SDL_LockSurface(tempsurf);
			png_write_image(png_ptr, row_pointers);
			if(SDL_MUSTLOCK(tempsurf)) SDL_UnlockSurface(tempsurf);
			SDL_FreeSurface(tempsurf);
			tempsurf = NULL;
		}else
		{
			for(i = 0;i < surf->h;++ i)
			{
				row_pointers[i] = ((png_byte*)surf->pixels) + i * surf->pitch;
			}
			if(SDL_MUSTLOCK(surf)) SDL_LockSurface(surf);
			png_write_image(png_ptr, row_pointers);
			if(SDL_MUSTLOCK(surf)) SDL_UnlockSurface(surf);
		}
	}

	png_write_end(png_ptr, NULL);
	ret = 0; /* got here, so nothing went wrong. YAY! */

savedone: /* clean up and return */
	png_destroy_write_struct(&png_ptr,&info_ptr);
	if(palette != NULL) 
	{
		free(palette);
		palette = NULL;
	}
	if(palette_alpha != NULL) 
	{
		free(palette_alpha);
		palette_alpha = NULL;
	}
	if(row_pointers != NULL) 
	{
		free(row_pointers);
		row_pointers = NULL;
	}
	return ret;
}
namespace XPng
{
	int savePNG(const char *fileName, SDL_Surface *surf,int compression)
	{
		SDL_RWops *fp = NULL;
		if((fp = SDL_RWFromFile(fileName,"wb")) == NULL) return (-1);

		int ret = IMG_SavePNG_RW(fp,surf,compression);
		SDL_RWclose(fp);
		return ret;
	}
	XBool savePngRGB2RGB(const char *fileName,
				const unsigned char * data,
				int w,int h,
				int compression)
	{
		if(data == NULL) return XFalse;
		//将数据保存成图片
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,24,0xff000000, 0x00ff0000, 0x0000ff00, 0x00000000);
	#else
		SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,24,0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000);
	#endif
		//这里可以考虑保存成24位色提升效率
		unsigned char *pData1 = (unsigned char *)picArm->pixels;
		memcpy(pData1,data,w * h * 3);
		savePNG(fileName,picArm,compression);
		//释放资源
		SDL_FreeSurface(picArm);
		picArm = NULL;
		return XTrue;
	}
	XBool savePngRGBA2RGBA(const char *fileName,
				const unsigned char * data,
				int w,int h,
				int compression)
	{
		if(data == NULL) return XFalse;
		//将数据保存成图片
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,32,0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	#else
		SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,32,0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	#endif
		//这里可以考虑保存成32位色提升效率
		unsigned char *pData1 = (unsigned char *)picArm->pixels;
		memcpy(pData1,data,w * h * 4);
		savePNG(fileName,picArm,compression);
		//释放资源
		SDL_FreeSurface(picArm);
		picArm = NULL;
		return XTrue;
	}
	XBool savePngRGB2RGBA(const char *fileName,
				const unsigned char * data,
				int w,int h,
				int compression)
	{
		if(data == NULL) return XFalse;
		//将数据保存成图片
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,32,0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	#else
		SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,32,0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	#endif
		//这里可以考虑保存成24位色提升效率
		unsigned char *pData1 = (unsigned char *)picArm->pixels;
		//int tempData1 = 0;
		int maxIndex = w * h;
		for(int i = 0,id = 0,is = 0;i < maxIndex;++ i,id += 4,is += 3)
		{
			*(pData1 + id + 0) = *(data + is + 0);
			*(pData1 + id + 1) = *(data + is + 1);
			*(pData1 + id + 2) = *(data + is + 2);
			*(pData1 + id + 3) = 255;
		}
		savePNG(fileName,picArm,compression);
		//释放资源
		SDL_FreeSurface(picArm);
		picArm = NULL;
		return XTrue;
	}
	XBool savePngRGBA2RGB(const char *fileName,
				const unsigned char * data,
				int w,int h,
				int compression)
	{
		if(data == NULL) return XFalse;
		//将数据保存成图片
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,24,0xff000000, 0x00ff0000, 0x0000ff00, 0x00000000);
	#else
		SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,24,0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000);
	#endif
		//这里可以考虑保存成24位色提升效率
		unsigned char *pData1 = (unsigned char *)picArm->pixels;
		//int tempData1 = 0;
		int maxIndex = w * h;
		for(int i = 0,id = 0,is = 0;i < maxIndex;++ i,id += 3,is += 4)
		{
			*(pData1 + id + 0) = *(data + is + 0);
			*(pData1 + id + 1) = *(data + is + 1);
			*(pData1 + id + 2) = *(data + is + 2);
		}
		savePNG(fileName,picArm,compression);
		//释放资源
		SDL_FreeSurface(picArm);
		picArm = NULL;
		return XTrue;
	}
	int savePNG(const std::string &filename,
				const unsigned char * data,int w,int h,XColorMode color,
				int compression)
	{
		if(data == NULL) return XFalse;
		SDL_RWops *fp;
		int ret;

		if((fp = SDL_RWFromFile(filename.c_str(),"wb")) == NULL) return -1;
		SDL_Surface * picArm = NULL;
		switch(color)
		{
		case COLOR_RGB:
		case COLOR_BGR:
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,24,0xff000000, 0x00ff0000, 0x0000ff00, 0x00000000);
	#else
			picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,24,0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000);
	#endif
			break;
		case COLOR_RGBA:
		case COLOR_BGRA:
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,32,0xff000000, 0x00ff0000, 0x0000ff00, 0x00000000);
	#else
			picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,32,0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000);
	#endif
			break;
		default:	//其他格式不支持
			return -1;
		}
		//这里可以考虑保存成24位色提升效率
		memcpy(picArm->pixels,data,w * h * picArm->format->BytesPerPixel);

		ret = IMG_SavePNG_RW(fp,picArm,compression);
		SDL_FreeSurface(picArm);
		picArm = NULL;
		SDL_RWclose(fp);
		return ret;
	}
}
}