#ifndef _JIA_XAVSTREAM_ 
#define _JIA_XAVSTREAM_  
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
//这是一个视频音频流混合压缩的类，基于ffmpeg和OpenAL

#include "XFfmpeg.h" 
#include "XBasicFun.h"
#include "XBasicClass.h"
#include "XMicrophone.h"
#include "XAudioStream.h"
  
#include <stdio.h>  

struct _XAudioInfo
{
	int channelSum;
	AVSampleFormat sampleFormat;
	int sampleRate;
};
enum _XAudioDeviceType
{
	AUDIO_DEVICE_TYPE_MIC,
	AUDIO_DEVICE_TYPE_FILE,
};

class _XAVStream 
{
private:
	_XBool m_isOpen;				//是否打开流
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
	_XBool m_audioBuffFlag;		//用于内存交换，为了提升效率

	AVFrame *m_audioFrame;	//音频数据
	SwrContext *m_audioSwr;	//用于音频转码
	AVPacket m_audioPkt;	//用于音频数据包
	//视频
	int m_videoOutbufSize;	//视频缓存的大小
	uint8_t *m_videoOutbuf;	//视频的缓存
	AVPacket m_videoPkt;	//用于音频数据包

	AVStream *openAudio();	//打开音频流
	void closeAudio()		//关闭音频流
	{
		avcodec_close(m_audioST->codec);
		//av_free(m_audioOutbuf);
		XDELETE_ARRAY(m_audioOutbuf);
		XDELETE_ARRAY(m_audioTempBuff);
	}

	AVStream *openVideo();	//打开视频流
	void closeVideo()		//关闭视频流
	{
		avcodec_close(m_videoST->codec);
		//av_free(m_videoOutbuf);
		XDELETE_ARRAY(m_videoOutbuf);
	} 

	//下面增加一种自动同步的模式（去掉这一部分）
	//_XBool m_autoMode;	//是否使用自动同步的模式
	float m_autoTimer;	//自动同步模式的计时器 单位:ms
	float m_frameTime;	//每帧的时间

	SwsContext *m_pSwsContext;	//用于进行图像格式转换
	int imgConvert(AVPicture *dst,const AVPicture *src,int src_height)
	{
		if(m_pSwsContext == NULL) return 0;
		return sws_scale(m_pSwsContext, src->data, src->linesize,  
            0,src_height, dst->data, dst->linesize); 
	}
	_XBool audioNeedData() const
	{
		int len = 0;
		switch(m_audioInfo.sampleFormat)
		{
		case AV_SAMPLE_FMT_U8:len = 1;break;
		case AV_SAMPLE_FMT_S16:len = 2;break;
		case AV_SAMPLE_FMT_S32:len = 4;break;
		case AV_SAMPLE_FMT_FLT:len = 4;break;
		case AV_SAMPLE_FMT_U8P:len = 1;break;
		case AV_SAMPLE_FMT_S16P:len = 2;break;
		case AV_SAMPLE_FMT_S32P:len = 4;break;
		case AV_SAMPLE_FMT_FLTP:len = 4;break;
		}
		return m_videoFrameIndex * m_audioInfo.sampleRate * m_audioInfo.channelSum * len / m_videoFrameRate > m_inputAudioDataSum;
	}
	_XBool openFile(const char *filename,int width,int height,int frameRate,_XAudioInfo *info = NULL);
	void pushVideoFrame();
	//下面是用于清空队列的
	void flushAudioQueue();
	void flushVideoQueue();
	_XAudioInfo m_audioInfo;	//音频数据信息

	int m_inputAudioDataSum;	//总的音频数据推入数量
private:
	int m_videoFrameIndex;		//帧的编号
	//int m_audioFrameIndex;
public:	//对外接口
	//方案1，时间帧同步需要外部处理
	_XBool open(const char *filename,int width,int height,int frameRate,_XAudioInfo *info = NULL);   
	void addFrameAudio(const unsigned char * data,int size);	//这个函数传入的数据必须是LFLFLF....交错的数据，不能使LLLLL……FFFFF……的数据，否则会造成问题。
	void addFrameRGB(unsigned char*p);
	//方案1的一个延伸版本
	void addFrameAudio(_XMicrophone & mic)
	{
		int ret = 0;
		unsigned char * audioData = mic.getCaptureData(&ret);
		if(ret != 0 && audioNeedData())  addFrameAudio(audioData,ret * mic.getByteRate());//读取音频数据，并写入到音频文件中
	}
	void addFrameAudio(_XAudioStream & aStr)
	{
		while(audioNeedData())
		{
			if(aStr.getAFrame()) addFrameAudio(aStr.getData(),aStr.getDataLen());
			Sleep(1);
		}
	}
	//方案2：多线程，音频设备外部处理
	_XBool open(const char *filename,int width,int height,int frameRate,void * audioDevice,
		_XAudioDeviceType deviceType = AUDIO_DEVICE_TYPE_MIC,_XAudioInfo *info = NULL);   
	void updataFrameRGB(unsigned char*p){addFrameRGB(p);}
	void setStop()
	{
		if(m_audioDeviceType == AUDIO_DEVICE_TYPE_MIC)
			((_XMicrophone *)m_audioDevice)->captureStop();
		m_isStop = XTrue;
	}
	void setStart()
	{
		if(m_audioDeviceType == AUDIO_DEVICE_TYPE_MIC)
			((_XMicrophone *)m_audioDevice)->captureStart();
		m_isStop = XFalse;
	}
private:
	_XCritical m_mutex;
	void * m_audioDevice;
	_XAudioDeviceType m_audioDeviceType;
	_XBool m_isStop;
	_XBool m_withThread;
	int m_threadState;	//线程的状态 1：运行 2：设置退出，3：退出完成
	pthread_t m_encodeThread;
	static DWORD WINAPI encodeThread(void * pParam);	//接收线程
public:
	//方案3，音频设备内部处理
	_XBool openEx(const char *filename,int width,int height,int frameRate,const char * deviceName = NULL,
		_XAudioDeviceType deviceType = AUDIO_DEVICE_TYPE_MIC);
private:
	_XBool m_withDevice;
	_XMicrophone m_micDevice;	//麦克风的类
	_XAudioStream m_AudioStream;	//从音频文件中读取音频数据的类
public:
	//公用代码
	_XBool close();  
	float getFrameTime() const {return m_frameTime;}	//获取一帧的时间，单位ms

	_XAVStream()
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
	{
	}
};  

#endif 