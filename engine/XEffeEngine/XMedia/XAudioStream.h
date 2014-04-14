#ifndef _JIA_XAUDIOSTREAM_
#define _JIA_XAUDIOSTREAM_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.8.10
//--------------------------------
//这是一个从音频文件中提取音频数据包的类
#include "XBasicWindow.h"
#include "XBasicFun.h"
#include "XFfmpeg.h"

class _XAudioStream
{
private:
	_XBool m_isLoaded;		//是否完成载入
	AVFormatContext *m_pFormatCtx;		//文件数据格式描述的载体
	AVCodecContext *m_pAudioCodecCtx;	//音频压缩数据的信息
	AVFrame *m_pFrame;					//一个音频解码包的数据
	AVPacket m_dataPacket;
	int m_allFrameSum;					//总帧数

	//用于音频数据个是转换
	SwrContext *m_pSwrContext;
	uint8_t m_audioBuf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; 
	int m_dataLen;
public:
	unsigned char * getData() {return m_audioBuf;}
	int getDataLen() const {return m_dataLen;}
	_XBool load(const char * filename);
	_XBool getAFrame();	//从流中提取一帧数据
	void close()
	{
		if(!m_isLoaded) return;
		avcodec_free_frame(&m_pFrame);
		av_free_packet(&m_dataPacket);
		avcodec_close(m_pAudioCodecCtx);
		av_close_input_file(m_pFormatCtx);
		swr_free(&m_pSwrContext);
		m_isLoaded = XFalse;
	}
	void gotoFrame(float temp)	//[0.0 - 1.0]
	{
		if(temp < 0.0f) temp = 0.0f;
		if(temp > 1.0f) temp = 1.0f;

		int armTime = temp * m_allFrameSum;
		av_seek_frame(m_pFormatCtx,0,armTime,AVSEEK_FLAG_BACKWARD);
	}

	_XAudioStream()
		:m_isLoaded(XFalse)
		,m_pFormatCtx(NULL)
		,m_pAudioCodecCtx(NULL)
		,m_pFrame(NULL)
		,m_pSwrContext(NULL)
	{
	}
	~_XAudioStream() {close();}
};

#endif