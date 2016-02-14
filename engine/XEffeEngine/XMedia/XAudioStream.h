#ifndef _JIA_XAUDIOSTREAM_
#define _JIA_XAUDIOSTREAM_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2011.8.10
//--------------------------------
//这是一个从音频文件中提取音频数据包的类
//
//#include "XBasicFun.h"
#include "XFfmpeg.h"
namespace XE{
class XAudioStream
{
private:
	XBool m_isLoaded;		//是否完成载入
	AVFormatContext *m_pFormatCtx;		//文件数据格式描述的载体
	AVCodecContext *m_pAudioCodecCtx;	//音频压缩数据的信息
	AVFrame *m_pFrame;					//一个音频解码包的数据
	AVPacket m_dataPacket;
	int m_allFrameSum;					//总帧数

	//用于音频数据个是转换
	SwrContext *m_pSwrContext;
	uint8_t *m_audioBuf; 
	int m_dataLen;
	int m_frameDataSum;	//每一帧数据的数据量
	float m_speed;	//播放速度
public:
	float getSpeed() const{return m_speed;}
	void setSpeed(float speed);
	unsigned char * getData() {return m_audioBuf;}
	int getDataLen() const {return m_dataLen;}
	XBool load(const char * filename);
	XBool getAFrame();	//从流中提取一帧数据
	AVSampleFormat getSampleFormat()const{return AV_SAMPLE_FMT_S16;}
	void close()
	{
		if(!m_isLoaded) return;
		av_frame_free(&m_pFrame);
		av_free_packet(&m_dataPacket);
		avcodec_close(m_pAudioCodecCtx);
		avformat_close_input(&m_pFormatCtx);
		swr_free(&m_pSwrContext);
		m_isLoaded = XFalse;
	}
	void gotoFrame(float temp)	//[0.0 - 1.0]
	{
		XMath::clamp(temp,0.0f,1.0f);
		//if(temp < 0.0f) temp = 0.0f;
		//if(temp > 1.0f) temp = 1.0f;

		av_seek_frame(m_pFormatCtx,0,temp * m_allFrameSum,AVSEEK_FLAG_BACKWARD);
	}
	XAudioStream()
		:m_isLoaded(XFalse)
		,m_pFormatCtx(NULL)
		,m_pAudioCodecCtx(NULL)
		,m_pFrame(NULL)
		,m_pSwrContext(NULL)
		,m_audioBuf(NULL)
		,m_speed(1.0f)
	{}
	~XAudioStream() {close();}
};
}
#endif