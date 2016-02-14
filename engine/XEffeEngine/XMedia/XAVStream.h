#ifndef _JIA_XAVSTREAM_ 
#define _JIA_XAVSTREAM_  
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
//这是一个视频音频流混合压缩的类，基于ffmpeg和OpenAL
#include "XFfmpeg.h" 
//#include "XBasicFun.h"
//#include "XBasicClass.h"
#include "XMicrophone.h"
#include "XAudioStream.h"
#include "../XThread.h"
#include "../XCritical.h"

#include <stdio.h>  
namespace XE{
struct XAudioInfo
{
	int channelSum;		//通道数量
	AVSampleFormat sampleFormat;	//音频采样格式
	int sampleRate;		//音频采样率
};
enum XAudioDeviceType
{
	AUDIO_DEVICE_TYPE_MIC,
	AUDIO_DEVICE_TYPE_FILE,
};
enum XAVStreamQuality
{
	AVS_QUALITY_HEIGHT,	//高质量
	AVS_QUALITY_MIDDLE,	//中等质量
	AVS_QUALITY_LOW,	//低质量
};

class XAVStream 
{
private:
	XBool m_isOpen;				//是否打开流
	//下面是一些公共数据
	int m_videoWidth;			//视频的像素宽度
	int m_videoHight;			//视频的像素高度
	int m_videoFrameRate;		//视频的帧率
	AVStream * m_videoST;		//视频流的指针
    AVStream * m_audioST;		//音频流的指针
	AVFormatContext *m_pOutContext;		//视频格式的指针
	AVOutputFormat *m_pFormat;

	AVFrame * m_pictureYUV;		//YUV像素的图像颜色
	AVFrame * m_pictureRGB;		//RGB像素的图像颜色 
	//音频
	int m_audioOutbufSize;		//音频缓存的大小
	uint8_t *m_audioOutbuf;		//音频的缓存
	uint8_t *m_audioTempBuff;
	int m_audioDataPos;			//当前音频数据的位置
	int m_audioFrameSize;		//这个数据存在问题,一个数据包的大小
	int m_audioFrameInSize;		//要装满一个数据包，需要输入的数据数量
	//int m_audioOutFrameSize;	//音频一帧数据的大小
	//int m_audioInFrameSize;	//音频输入一帧的大小
	XBool m_audioBuffFlag;		//用于内存交换，为了提升效率

	AVFrame *m_audioFrame;	//音频数据
	SwrContext *m_audioSwr;	//用于音频转码
	AVPacket m_audioPkt;	//用于音频数据包
	//视频
	int m_videoOutbufSize;	//视频缓存的大小
	uint8_t *m_videoOutbuf;	//视频的缓存
	AVPacket m_videoPkt;	//用于音频数据包

	AVStream *openAudio();	//打开音频流
	void closeAudio();		//关闭音频流

	AVStream *openVideo(XAVStreamQuality quality = AVS_QUALITY_HEIGHT);	//打开视频流
	void closeVideo();		//关闭视频流

	//下面增加一种自动同步的模式（去掉这一部分）
	//XBool m_autoMode;	//是否使用自动同步的模式
	float m_autoTimer;	//自动同步模式的计时器 单位:ms
	float m_frameTime;	//每帧的时间

	SwsContext *m_pSwsContext;	//用于进行图像格式转换
	int imgConvert(AVPicture *dst,const AVPicture *src,int src_height);
	XBool audioNeedData() const;
	XBool openFile(const char *filename,int width,int height,int frameRate,
		XAudioInfo *info = NULL,XAVStreamQuality quality = AVS_QUALITY_HEIGHT);
	bool pushVideoFrame();
	//下面是用于清空队列的
	void flushAudioQueue();
	void flushVideoQueue();
	XAudioInfo m_audioInfo;	//音频数据信息

	int m_inputAudioDataSum;	//总的音频数据推入数量
private:
	int m_videoFrameIndex;		//帧的编号
	//int m_audioFrameIndex;
public:	//对外接口
	//方案1，时间帧同步需要外部处理
	XBool open(const char *filename,int width,int height,int frameRate,
		XAudioInfo *info = NULL,XAVStreamQuality quality = AVS_QUALITY_HEIGHT);   
	void addFrameAudio(const unsigned char * data,int size);	//这个函数传入的数据必须是LFLFLF....交错的数据，不能使LLLLL……FFFFF……的数据，否则会造成问题。
	void addFrameRGB(unsigned char*p);
	//方案1的一个延伸版本
	void addFrameAudio(XMicrophone & mic);		//从麦克风中提取音频数据
	void addFrameAudio(XAudioStream & aStr);	//从音频文件中提取音频数据
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面支持两路音频输入混合
	unsigned char *m_audioBuff[3];	//两路音频的缓冲,0:输入1、1:输入2、2:混合结果
	int m_audioBuffSize[3];			//两路音频的缓冲大小
	int m_curUsage[3];				//当前缓存的使用量
	void addFrameAudio(XMicrophone &mic,XAudioStream &aStr);		
	void addFrameAudio(XAudioStream &aStr0,XAudioStream &aStr1);	//尚未实现
	void addFrameAudio(XMicrophone &mic0,XMicrophone &mic1);		//尚未实现
	//---------------------------------------------------------------------
	//方案2：多线程，音频设备外部处理
	XBool open(const char *filename,int width,int height,int frameRate,void * audioDevice,
		XAudioDeviceType deviceType = AUDIO_DEVICE_TYPE_MIC,XAudioInfo *info = NULL,
		XAVStreamQuality quality = AVS_QUALITY_HEIGHT);   
	void updataFrameRGB(unsigned char*p){addFrameRGB(p);}
	void setStop();
	void setStart();
	AVSampleFormat getSampleFormat()const{return m_audioInfo.sampleFormat;}
private:
	XCritical m_mutex;
	void * m_audioDevice;
	XAudioDeviceType m_audioDeviceType;
	XBool m_isStop;
	XBool m_withThread;
	XThreadState m_threadState;	//线程的状态 1：运行 2：设置退出，3：退出完成
	pthread_t m_encodeThread;
	static DWORD WINAPI encodeThread(void * pParam);	//接收线程
public:
	//方案3，音频设备内部处理
	XBool openEx(const char *filename,int width,int height,int frameRate,const char * deviceName = NULL,
		XAudioDeviceType deviceType = AUDIO_DEVICE_TYPE_MIC);
private:
	XBool m_withDevice;
	XMicrophone m_micDevice;	//麦克风的类
	XAudioStream m_AudioStream;	//从音频文件中读取音频数据的类
public:
	//公用代码
	XBool close();  
	float getFrameTime() const {return m_frameTime;}	//获取一帧的时间，单位ms

	XAVStream()
		:m_isOpen(XFalse)
		,m_videoWidth(0)		
		,m_videoHight(0)		
		,m_videoFrameRate(30)	
		,m_videoST(NULL)			
		,m_audioST(NULL)			
		,m_pOutContext(NULL)				
		,m_pFormat(NULL)
		,m_pictureYUV(NULL)		
		,m_pictureRGB(NULL)		

		,m_audioOutbufSize(0)
		,m_audioOutbuf(NULL)
		,m_audioTempBuff(NULL)

		,m_videoOutbufSize(0)
		,m_videoOutbuf(NULL)
		,m_audioFrame(NULL)
		,m_audioSwr(NULL)
		,m_bsfc(NULL)
		,m_aacbsfc(NULL)
	{
		m_audioBuff[0] = NULL;
		m_audioBuff[1] = NULL;
		m_audioBuff[2] = NULL;
		m_audioBuffSize[0] = 0;
		m_audioBuffSize[1] = 0;
		m_audioBuffSize[2] = 0;
		m_curUsage[0] = 0;
		m_curUsage[1] = 0;
		m_curUsage[2] = 0;
	}
	~XAVStream(){close();}
private:
	AVBitStreamFilterContext *m_bsfc;
	AVBitStreamFilterContext *m_aacbsfc;
	//AVBitStreamFilterContext* bsfc = av_bitstream_filter_init("h264_mp4toannexb");
    //AVBitStreamFilterContext* aacbsfc = av_bitstream_filter_init("aac_adtstoasc");
};  
#if WITH_INLINE_FILE
#include "XAVStream.inl"
#endif
}
#endif 