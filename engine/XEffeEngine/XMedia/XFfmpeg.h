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

#include "SDL.h"

namespace XE{
#ifndef AVCODEC_MAX_AUDIO_FRAME_SIZE
#define AVCODEC_MAX_AUDIO_FRAME_SIZE (192000)
#endif

//视频帧数据，链表
struct XVideoFrameData
{
	AVPicture *pict;		//当前帧的规制后的像素数据
	unsigned char *data;	//当前帧的像素数据
	XVideoFrameData *next;	//下一帧
	bool isOverFrame;	//是否是结束帧，流的结束会插入一个结束帧，用于标记流已经结束
	XVideoFrameData()
		:pict(NULL)
		,data(NULL)
		,next(NULL)
		,isOverFrame(false)
	{}
};
struct XVideoFrameDataEx	//强化版
{
	AVFrame *pict;		//当前帧的规制后的像素数据
	XVideoFrameDataEx *next;	//下一帧
	bool isOverFrame;	//是否是结束帧，流的结束会插入一个结束帧，用于标记流已经结束
	XVideoFrameDataEx()
		:pict(NULL)
		,next(NULL)
		,isOverFrame(false)
	{}
};
//视频帧队列
struct XVideoQueue
{
	XVideoFrameData *first_pict;	//第一帧
	XVideoFrameData *last_pict;	//最后一帧
	int nb_pict;			//序列中总的帧的数量
	SDL_mutex *mutex;		//线程锁
	XVideoQueue()
		:first_pict(NULL)
		,last_pict(NULL)
		,mutex(NULL)
		,nb_pict(0)
	{}
};
struct XVideoQueueEx	//强化版
{
	XVideoFrameDataEx *first_pict;	//第一帧
	XVideoFrameDataEx *last_pict;	//最后一帧
	int nb_pict;			//序列中总的帧的数量
	SDL_mutex *mutex;		//线程锁
	XVideoQueueEx()
		:first_pict(NULL)
		,last_pict(NULL)
		,mutex(NULL)
		,nb_pict(0)
	{}
};
//音频帧数据
struct XAudioFrameData 
{
    AVPacket pkt;
    XAudioFrameData *next;
	bool isOverFrame;
	XAudioFrameData()
		:next(NULL)
		,isOverFrame(false)
	{}
};
//音频帧队列
struct XAudioQueueEx
{
	XAudioFrameData *first_pkt;//第一个音频报的指针
	XAudioFrameData *last_pkt;	//最后一个音频包的指针
	int nb_packets;			//音频包的数量
	int size;				//总的音频数据的尺寸
	SDL_mutex *mutex;		//线程锁
//	SDL_cond *cond;			//用于同步
	XAudioQueueEx()
		:first_pkt(NULL)
		,last_pkt(NULL)
		,mutex(NULL)
//		,cond(NULL)
		,nb_packets(0)
		,size(0)
	{}
};
//音频帧队列
struct XAudioQueue
{
	AVPacketList *first_pkt;//第一个音频报的指针
	AVPacketList *last_pkt;	//最后一个音频包的指针
	int nb_packets;			//音频包的数量
	int size;				//总的音频数据的尺寸
	SDL_mutex *mutex;		//线程锁
	SDL_cond *cond;			//用于同步
	XAudioQueue()
		:first_pkt(NULL)
		,last_pkt(NULL)
		,mutex(NULL)
		,cond(NULL)
		,nb_packets(0)
		,size(0)
	{}
};
#define MAX_BUFF_DEEP (40)	//缓存的深度,这个数据要能动态设置,这个值必须大于1
inline AVHWAccel *ffFindHwaccel(AVCodecID codec_id,AVPixelFormat pix_fmt)
{
    AVHWAccel *hwaccel = NULL;
	//int i = 0;
    while((hwaccel= av_hwaccel_next(hwaccel)))
	{
        if(hwaccel->id == codec_id && hwaccel->pix_fmt == pix_fmt)
		{
		//	printf("%s\n",hwaccel->name);
		//	++i;
		//	if(i == 2) return hwaccel;
			return hwaccel;
		}
    }
    return NULL;
}
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
namespace XFfmpeg
{
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
}
}
#endif