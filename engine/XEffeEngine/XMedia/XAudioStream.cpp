#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XAudioStream.h"
namespace XE{
XBool XAudioStream::load(const char * filename)
{
	if(m_isLoaded ||
		filename == NULL) return XFalse;
	av_register_all();	
	m_pFormatCtx = NULL;
	if(avformat_open_input(&m_pFormatCtx,filename,NULL,NULL) != 0)
	{
		LogStr("File open error!");
		return XFalse;
	}
	if(avformat_find_stream_info(m_pFormatCtx,NULL) < 0)		//检查视频流信息
	{
		LogStr("can not find stream information!");
		return XFalse;
	}
	m_pAudioCodecCtx = m_pFormatCtx->streams[0]->codec;
	AVCodec *aCodec = avcodec_find_decoder(m_pAudioCodecCtx->codec_id);
	if(aCodec == NULL)
	{//找不到音频解码器
		LogStr("can not find audio decoder information!");
		return XFalse;
	} 
	if(avcodec_open2(m_pAudioCodecCtx,aCodec,NULL) < 0)
	{//找不到音频解码包
		LogStr("can not open audio decoder!");
		return XFalse;
	}
	m_frameDataSum = XEG.getAudioChannelSum() * av_get_bytes_per_sample(getSampleFormat());
	XMem::XDELETE_ARRAY(m_audioBuf);
	m_audioBuf = XMem::createArrayMem<uint8_t>((AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2);
	if(m_audioBuf == NULL) return XFalse;
	//m_pFrame = av_frame_alloc();
	//if(m_pFrame == NULL)
	//{
	//	printf("malloc Frame failed!\n");
	//	return XFalse;
	//}
	m_allFrameSum = m_pFormatCtx->streams[0]->duration;
	av_init_packet(&m_dataPacket);

	m_pSwrContext = swr_alloc();
	if(m_pSwrContext == NULL) return XFalse;
	if(m_pAudioCodecCtx->channel_layout == 0)
	{
		swr_alloc_set_opts(m_pSwrContext,av_get_default_channel_layout(XEG.getAudioChannelSum()),getSampleFormat(),XEG.getAudioSampleRate() * m_speed,
			av_get_default_channel_layout(m_pAudioCodecCtx->channels),m_pAudioCodecCtx->sample_fmt,m_pAudioCodecCtx->sample_rate,0,NULL);
	}else
	{
		swr_alloc_set_opts(m_pSwrContext,av_get_default_channel_layout(XEG.getAudioChannelSum()),getSampleFormat(),XEG.getAudioSampleRate() * m_speed,
			m_pAudioCodecCtx->channel_layout,m_pAudioCodecCtx->sample_fmt,m_pAudioCodecCtx->sample_rate,0,NULL);
	}
	if(swr_init(m_pSwrContext) < 0)
	{
		LogStr("swr_init() fail");
		return XFalse;
	}

	m_isLoaded = XTrue;
	return XTrue;
}
void XAudioStream::setSpeed(float speed)
{
	if(m_speed == speed || speed <= 0.0f) return;
	m_speed = speed;
	swr_free(&m_pSwrContext);
	m_pSwrContext = swr_alloc();
	if(m_pSwrContext == NULL) return;
	if(m_pAudioCodecCtx->channel_layout == 0)
	{
		swr_alloc_set_opts(m_pSwrContext,av_get_default_channel_layout(XEG.getAudioChannelSum()),getSampleFormat(),XEG.getAudioSampleRate() * m_speed,
			av_get_default_channel_layout(m_pAudioCodecCtx->channels),m_pAudioCodecCtx->sample_fmt,m_pAudioCodecCtx->sample_rate,0,NULL);
	}else
	{
		swr_alloc_set_opts(m_pSwrContext,av_get_default_channel_layout(XEG.getAudioChannelSum()),getSampleFormat(),XEG.getAudioSampleRate() * m_speed,
			m_pAudioCodecCtx->channel_layout,m_pAudioCodecCtx->sample_fmt,m_pAudioCodecCtx->sample_rate,0,NULL);
	}
	if(swr_init(m_pSwrContext) < 0)
	{
		LogStr("swr_init() fail");
		return;
	}
}
XBool XAudioStream::getAFrame()	//从流中提取一帧数据
{
	if(!m_isLoaded) return XFalse;
	av_free_packet(&m_dataPacket);	//释放上一次获得的数据
	av_init_packet(&m_dataPacket);
	if(av_read_frame(m_pFormatCtx,&m_dataPacket) == 0)
	{//这里见音频数据解码
		m_pFrame = av_frame_alloc();

		int isFinished;
		if(avcodec_decode_audio4(m_pAudioCodecCtx,m_pFrame,&isFinished,&m_dataPacket) < 0) return XFalse;
		if(isFinished)
		{
			uint8_t *out[] = {m_audioBuf}; 
			int outSize = ((AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2) / m_frameDataSum;
			m_dataLen = swr_convert(m_pSwrContext,out,outSize,(const uint8_t **)m_pFrame->extended_data,m_pFrame->nb_samples);  
			m_dataLen = m_dataLen * m_frameDataSum;
			av_frame_free(&m_pFrame);
			return XTrue;//解码成功
		}
		av_frame_free(&m_pFrame);
	}else 
	{
		gotoFrame(0.0f);	//跳到头
		LogStr("File end!");
		return XFalse;
	}
	return XFalse;
}
}