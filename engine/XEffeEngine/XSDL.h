#ifndef _JIA_XSDL_
#define _JIA_XSDL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "SDL.h"
#ifdef _WIN64
	#pragma comment(lib, "SDL/dxguid64.lib")
	#ifdef WITH_VS2015
		//#pragma comment(lib, "SDL/libFLAC15_64.lib")
		//#pragma comment(lib, "SDL/libFLAC++15_64.lib")
		#pragma comment(lib, "SDL/libvorbis15_64.lib")
		#pragma comment(lib, "SDL/libvorbisfile15_64.lib")
		#pragma comment(lib, "SDL/libogg15_64.lib")
		#pragma comment(lib, "SDL/libmikmod15_64.lib")
		#pragma comment(lib, "SDL/smpeg15_64.lib")	//载入MP3时会crash
		#pragma comment(lib, "SDL/libtiff15_64.lib")
		#pragma comment(lib, "SDL/libjpeg15_64.lib")
		#pragma comment(lib, "SDL/SDL15_64.lib")
		#pragma comment(lib, "SDL/SDLmain15_64.lib")
		#pragma comment(lib, "SDL/libpng15_64.lib")
		#pragma comment(lib, "SDL/zlib15_64.lib")
		#pragma comment(lib, "SDL/SDL_mixer15_64.lib")
		#pragma comment(lib, "SDL/timidity15_64.lib")
		#pragma comment(lib, "SDL/native_midi15_64.lib")
		#pragma comment(lib, "SDL/SDL_image15_64.lib")
	#else
		#pragma comment(lib, "SDL/smpeg64.lib")
		#pragma comment(lib, "SDL/libtiff64.lib")
		#pragma comment(lib, "SDL/libjpeg64.lib")
		#pragma comment(lib, "SDL/SDL64.lib")
		#pragma comment(lib, "SDL/SDLmain64.lib")
		#pragma comment(lib, "SDL/libpng64.lib")
		#pragma comment(lib, "SDL/zlib64.lib")
		#pragma comment(lib, "SDL/SDL_mixer64.lib")
		#pragma comment(lib, "SDL/timidity64.lib")
		#pragma comment(lib, "SDL/native_midi64.lib")
		#pragma comment(lib, "SDL/SDL_image64.lib")
	#endif
#else //_WIN64
	#pragma comment(lib, "SDL/dxguid.lib")
	#ifdef WITH_VS2015
		//#pragma comment(lib, "SDL/libFLAC15.lib")
		//#pragma comment(lib, "SDL/libFLAC++15.lib")
		#pragma comment(lib, "SDL/libvorbis15.lib")
		#pragma comment(lib, "SDL/libvorbisfile15.lib")
		#pragma comment(lib, "SDL/libogg15.lib")
		#pragma comment(lib, "SDL/libmikmod15.lib")
		#pragma comment(lib, "SDL/smpeg15.lib")
#ifdef _DEBUG
		#pragma comment(lib, "SDL/libtiff15d.lib")
#else
		#pragma comment(lib, "SDL/libtiff15.lib")
#endif
		#pragma comment(lib, "SDL/libjpeg15.lib")
		#pragma comment(lib, "SDL/SDL15.lib")
		#pragma comment(lib, "SDL/SDLmain15.lib")
		#pragma comment(lib, "SDL/SDL_mixer15.lib")
		#pragma comment(lib, "SDL/SDL_image15.lib")
		#pragma comment(lib, "SDL/timidity15.lib")
		#pragma comment(lib, "SDL/native_midi15.lib")
		#pragma comment(lib, "SDL/zlib15.lib")
		#pragma comment(lib, "SDL/libpng15.lib")
	#else //WITH_VS2015
		#pragma comment(lib, "SDL/smpeg.lib")
		#pragma comment(lib, "SDL/libtiff.lib")
		#pragma comment(lib, "SDL/libjpeg.lib")
		#pragma comment(lib, "SDL/SDL.lib")
		#pragma comment(lib, "SDL/SDLmain.lib")
		#pragma comment(lib, "SDL/SDL_mixer.lib")
		#pragma comment(lib, "SDL/SDL_image.lib")
		#pragma comment(lib, "SDL/timidity.lib")
		#pragma comment(lib, "SDL/native_midi.lib")
		#pragma comment(lib, "SDL/zlib.lib")
		#pragma comment(lib, "SDL/libpng.lib")
	#endif //WITH_VS2015
#endif //_WIN64

#ifndef WITH_VS2015
#pragma comment(lib, "SDL/libogg_static.lib")
#pragma comment(lib, "SDL/libvorbis_static.lib")
#pragma comment(lib, "SDL/libvorbisfile_static.lib")
#endif

#endif