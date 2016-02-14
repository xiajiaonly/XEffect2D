#ifndef _JIA_XSDL_
#define _JIA_XSDL_
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "SDL.h"
#ifdef _WIN64
#pragma comment(lib, "../../engine/lib/SDL/SDL64.lib")
#pragma comment(lib, "../../engine/lib/SDL/SDLmain64.lib")
#pragma comment(lib, "../../engine/lib/SDL/libpng64.lib")
#pragma comment(lib, "../../engine/lib/SDL/zlib64.lib")
#pragma comment(lib, "../../engine/lib/SDL/dxguid64.lib")
#pragma comment(lib, "../../engine/lib/SDL/SDL_mixer64.lib")
#pragma comment(lib, "../../engine/lib/SDL/timidity64.lib")
#pragma comment(lib, "../../engine/lib/SDL/native_midi64.lib")
#pragma comment(lib, "../../engine/lib/SDL/SDL_image64.lib")
#else
#pragma comment(lib, "../../engine/lib/SDL/SDL.lib")
#pragma comment(lib, "../../engine/lib/SDL/SDLmain.lib")
#pragma comment(lib, "../../engine/lib/SDL/libpng.lib")
#pragma comment(lib, "../../engine/lib/SDL/zlib.lib")
#pragma comment(lib, "../../engine/lib/SDL/SDL_mixer.lib")
#pragma comment(lib, "../../engine/lib/SDL/dxguid.lib")
#pragma comment(lib, "../../engine/lib/SDL/timidity.lib")
#pragma comment(lib, "../../engine/lib/SDL/native_midi.lib")
#pragma comment(lib, "../../engine/lib/SDL/SDL_image.lib")
#endif
#pragma comment(lib, "../../engine/lib/SDL/libtiff.lib")
#pragma comment(lib, "../../engine/lib/SDL/libjpeg.lib")

#pragma comment(lib, "../../engine/lib/SDL/libogg_static.lib")
#pragma comment(lib, "../../engine/lib/SDL/libvorbis_static.lib")
#pragma comment(lib, "../../engine/lib/SDL/libvorbisfile_static.lib")
#pragma comment(lib, "../../engine/lib/SDL/smpeg.lib")
#endif