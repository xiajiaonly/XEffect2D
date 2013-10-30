#ifndef __IMG_SAVETOPNG_H__
#define __IMG_SAVETOPNG_H__
//++++++++++++++++++++++++++++++++
//Author:	未知(这部分代码从网上获取)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#define IMG_COMPRESS_OFF 0
#define IMG_COMPRESS_MAX 9
#define IMG_COMPRESS_DEFAULT -1

#define Z_NO_COMPRESSION 0
#define Z_BEST_COMPRESSION 9
#define Z_DEFAULT_COMPRESSION -1

/**
 * Takes a filename, a surface to save, and a compression level.  The
 * compression level can be 0(min) through 9(max), or -1(default).
 */
extern int savePNG(const char *fileName,
			SDL_Surface *surf,
			int compression);
/**
 * Takes a SDL_RWops pointer, a surface to save, and a compression level.
 * compression can be 0(min) through 9(max), or -1(default).
 */
extern int IMG_SavePNG_RW(SDL_RWops *src,
			SDL_Surface *surf,
			int compression);


#endif/*__IMG_SAVETOPNG_H__*/
