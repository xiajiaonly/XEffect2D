//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.7.2
//--------------------------------
#ifndef _JIA_XFFMPEG_
#define _JIA_XFFMPEG_

#include "XOSDefine.h"

#ifdef XEE_OS_WINDOWS
#include <Windows.h>
extern "C" 
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
}
#endif
#ifdef XEE_OS_LINUX
#include <pthread.h>
#include "avformat.h"
#include "avcodec.h"
#endif

#pragma comment(lib, "../../engine/lib/ffmpeg/avcodec.lib")
#pragma comment(lib, "../../engine/lib/ffmpeg/avformat.lib")
#pragma comment(lib, "../../engine/lib/ffmpeg/avutil.lib")
#pragma comment(lib, "../../engine/lib/ffmpeg/swresample.lib")
#pragma comment(lib, "../../engine/lib/ffmpeg/swscale.lib")

#ifndef AVCODEC_MAX_AUDIO_FRAME_SIZE
#define AVCODEC_MAX_AUDIO_FRAME_SIZE (192000)
#endif

/*
//这个函数的使用存在问题，需要谨慎使用
inline int img_convert(AVPicture *dst, AVPixelFormat dst_pix_fmt,  
                const AVPicture *src, AVPixelFormat src_pix_fmt,  
                int src_width, int src_height)  
{  
	static int isInited = 0;
    static SwsContext *pSwsCtx = NULL;  
	if(isInited == 0)
	{
		pSwsCtx = sws_getContext(src_width,src_height,src_pix_fmt,   
                            src_width,src_height,dst_pix_fmt,  
                            SWS_BICUBIC,NULL,NULL,NULL); 
		isInited = 1;
	}
    sws_scale(pSwsCtx, src->data, src->linesize,  
            0,src_height, dst->data, dst->linesize);  
	//sws_freeContext(pSwsCtx);  //最终如何释放资源，
    return 0;  
}
*/
//通用性好,但是效率慢
inline int img_convert(AVPicture *dst, AVPixelFormat dst_pix_fmt,  
                const AVPicture *src, AVPixelFormat src_pix_fmt,  
                int src_width, int src_height)  
{  
	SwsContext *pSwsCtx = sws_getContext(src_width,src_height,src_pix_fmt,   
                            src_width,src_height,dst_pix_fmt,  
                            SWS_BICUBIC,NULL,NULL,NULL); 
    sws_scale(pSwsCtx, src->data, src->linesize,  
            0,src_height, dst->data, dst->linesize);  
	sws_freeContext(pSwsCtx);  
    return 0;  
}

#endif