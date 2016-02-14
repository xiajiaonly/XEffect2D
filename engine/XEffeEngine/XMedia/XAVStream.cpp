#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XAVStream.h"  

#include "../XMath/XColorSpace.h"
#include "XTimer.h"
namespace XE{
//float STREAM_DURATION = 5.0f;
//int STREAM_NB_FRAMES = 0;//  ((int)(STREAM_DURATION * STREAM_FRAME_RATE))
//这个函数中的音频转换的数据存在问题，需要考量
void XAVStream::addFrameAudio(const unsigned char * data,int size)
{//将音频数据写入到文件中
	if(!m_isOpen ||
		data == NULL ||
		m_audioST == NULL) return;
	//++++++++++++++++++++++++++++++++++
	//下面对音频buff进行处理
	if(m_audioDataPos + size <= m_audioOutbufSize)
	{//临时保存音频数据
		if(m_audioBuffFlag) memcpy(m_audioTempBuff + m_audioDataPos,data,size);
		else memcpy(m_audioOutbuf + m_audioDataPos,data,size);
		m_audioDataPos += size;
		m_inputAudioDataSum += size;	//记录数据
	}else
	{//分多次将数据考入
		int tempSize = 0;
		int offset = 0;
		while(true)
		{
			if(size - offset + m_audioDataPos > m_audioOutbufSize) tempSize = m_audioOutbufSize - m_audioDataPos;
			else 
			{
				tempSize = size - offset;
				if(tempSize <= 0) break;
			}
			addFrameAudio(data + offset,tempSize);
			offset += tempSize;
			Sleep(1);
		}
		//printf("Data Over!\n");	//数据溢出
	}
	//----------------------------------
	//printf("%d\n",m_audioDataPos);
	while(m_audioDataPos >= m_audioFrameInSize)	//由于不同的音频格式这里的m_audioFrameSize不对
	{//数据足够，则写入数据
		AVCodecContext *c = m_audioST->codec;
		int ret;
		if(c->channel_layout != av_get_default_channel_layout(m_audioInfo.channelSum)
			|| c->sample_fmt != m_audioInfo.sampleFormat 
			|| c->sample_rate != m_audioInfo.sampleRate)
		{//音频格式转换
			if(m_audioSwr == NULL)
			{//没有进行初始化，则这里进行初始化
				m_audioSwr = swr_alloc();
				if(m_audioSwr == NULL)
				{
					LogStr("ERROR:swr_alloc!");
					return;
				}
				swr_alloc_set_opts(m_audioSwr,c->channel_layout,c->sample_fmt,c->sample_rate,
					av_get_default_channel_layout(m_audioInfo.channelSum),m_audioInfo.sampleFormat,m_audioInfo.sampleRate,0,NULL);
				if(swr_init(m_audioSwr)<0)
				{
					LogStr("swr_init() for AV_SAMPLE_FMT_S16P fail");
					return;
				}
			}
		//	int audioBufferSize = av_samples_get_buffer_size(NULL, c->channels, c->frame_size,
        //                                   c->sample_fmt, 0);
			if(m_audioBuffFlag)
			{
				uint8_t *out[] = {m_audioOutbuf,m_audioOutbuf + (m_audioFrame->nb_samples << 1)};
				const uint8_t *in[] = {m_audioTempBuff};
				swr_convert(m_audioSwr,out,m_audioFrame->nb_samples,in,m_audioFrame->nb_samples);
				ret = avcodec_fill_audio_frame(m_audioFrame, c->channels,c->sample_fmt,m_audioOutbuf,m_audioFrameSize,0); 
			}else
			{
				uint8_t *out[] = {m_audioTempBuff,m_audioTempBuff + (m_audioFrame->nb_samples << 1)};
				const uint8_t *in[] = {m_audioOutbuf};
				swr_convert(m_audioSwr,out,m_audioFrame->nb_samples,in,m_audioFrame->nb_samples);
				ret = avcodec_fill_audio_frame(m_audioFrame, c->channels,c->sample_fmt,m_audioTempBuff,m_audioFrameSize,0);  
			}
		}else
		{
			if(m_audioBuffFlag) ret = avcodec_fill_audio_frame(m_audioFrame, c->channels,c->sample_fmt,m_audioTempBuff,m_audioFrameSize,0);  
			else ret = avcodec_fill_audio_frame(m_audioFrame, c->channels,c->sample_fmt,m_audioOutbuf,m_audioFrameSize,0);  
		}
		if(ret < 0) 
		{  
			LogStr("Could not setup audio frame");  
			return; 
		} 

		int haveData;	//是否有获得数据
		av_init_packet(&m_audioPkt);
		m_audioPkt.data = NULL;
		m_audioPkt.size = 0;
		//注释掉可以避免长时间录的error，但是会造成一开始的warning，不知道尚不知为什么
		//m_audioFrame->pts = m_audioFrameIndex;	
		//++m_audioFrameIndex;

		if(avcodec_encode_audio2(c,&m_audioPkt,m_audioFrame,&haveData) < 0) 
		{  
            LogStr("Error encoding audio frame");  
            return;
        }
		if(haveData)
		{  
			if(m_audioPkt.pts != AV_NOPTS_VALUE) m_audioPkt.pts = av_rescale_q(m_audioPkt.pts,	c->time_base, m_audioST->time_base);
			if(m_audioPkt.dts != AV_NOPTS_VALUE) m_audioPkt.dts = av_rescale_q(m_audioPkt.dts,	c->time_base, m_audioST->time_base);
			if(m_audioPkt.duration > 0) m_audioPkt.duration = av_rescale_q(m_audioPkt.duration,	c->time_base, m_audioST->time_base);

			m_audioPkt.flags |= AV_PKT_FLAG_KEY;
			m_audioPkt.stream_index = m_audioST->index;
			if(m_aacbsfc != NULL)
				av_bitstream_filter_filter(m_aacbsfc, m_audioST->codec, NULL, &m_audioPkt.data, &m_audioPkt.size, m_audioPkt.data, m_audioPkt.size, 0);  
            //if(av_write_frame(m_pOutContext, &m_audioPkt) != 0) 
			if(av_interleaved_write_frame(m_pOutContext, &m_audioPkt) != 0)
				LogStr("Error:音频数据写入失败!");
		}

		m_audioDataPos -= m_audioFrameInSize;
		if(m_audioBuffFlag) memcpy(m_audioOutbuf,m_audioTempBuff + m_audioFrameInSize,m_audioDataPos);
		else memcpy(m_audioTempBuff,m_audioOutbuf + m_audioFrameInSize,m_audioDataPos);
		m_audioBuffFlag = !m_audioBuffFlag;

		av_free_packet(&m_audioPkt);  
	//	av_frame_free(&frame);
	}
}
//下面是音频相关的
AVStream *XAVStream::openAudio()
{
	AVStream *st = avformat_new_stream(m_pOutContext, NULL); //建立一个音频流
    if(!st) 
	{
		LogStr("Error:音频流建立失败!");
		return NULL;
    }
    AVCodecContext * c = st->codec;
    //寻找音频编码器
	AVCodec *codec = avcodec_find_encoder(m_pFormat->audio_codec);
    if(!codec) 
	{
        LogStr("Error:音频编码器寻找失败!");
        return NULL;
    }
	st->time_base.den = 1;
	st->time_base.num = 1;
	avcodec_get_context_defaults3(c,codec);
	c->bit_rate = 128000;
	switch(c->codec_id)
	{
	case CODEC_ID_MP3:
		c->sample_fmt = AV_SAMPLE_FMT_S16P;
		break;//不同的音频格式这里不同，需要考虑
	case CODEC_ID_MP2:
		c->sample_fmt = AV_SAMPLE_FMT_S16;
		break;
	case CODEC_ID_AAC:
		m_aacbsfc = av_bitstream_filter_init("aac_adtstoasc");
		if(m_aacbsfc == NULL) return false;
		c->sample_fmt = AV_SAMPLE_FMT_S16;
		break;
	default:
		c->sample_fmt = AV_SAMPLE_FMT_S16;
		break;
	}
	c->sample_rate		= m_audioInfo.sampleRate;
	c->channels			= m_audioInfo.channelSum;
	c->channel_layout	= av_get_default_channel_layout(m_audioInfo.channelSum);

	//打开音频编码器
    if(avcodec_open2(c, codec,NULL) < 0) 
	{
        LogStr("Error:音频编码器打开失败!");
        return NULL;
    }
	//为音频数据建立缓存
    //m_audioOutbufSize = 20000;
    m_audioOutbufSize = 5 * av_samples_get_buffer_size(NULL, c->channels, c->frame_size,c->sample_fmt, 0);
    m_audioOutbuf = XMem::createArrayMem<uint8_t>(m_audioOutbufSize);//(uint8_t *)malloc(m_audioOutbufSize);
	m_audioTempBuff = XMem::createArrayMem<uint8_t>(m_audioOutbufSize);
	//建立音频转码的临时数据
	m_audioFrame = av_frame_alloc();
	if(m_audioFrame == NULL)
	{
		LogStr("ERROR:av_frame_alloc!");
		return NULL;
	}
	m_audioFrame->nb_samples = c->frame_size;  
	m_audioFrame->format = c->sample_fmt;  
	m_audioFrame->channel_layout = c->channel_layout; 
	if(c->channel_layout != av_get_default_channel_layout(m_audioInfo.channelSum)
		|| c->sample_fmt != m_audioInfo.sampleFormat 
		|| c->sample_rate != m_audioInfo.sampleRate)
	{//需要进行转码
		m_audioSwr = swr_alloc();
		if(m_audioSwr == NULL)
		{
			LogStr("ERROR:swr_alloc!");
			return NULL;
		}
		swr_alloc_set_opts(m_audioSwr,c->channel_layout,c->sample_fmt,c->sample_rate,
			av_get_default_channel_layout(m_audioInfo.channelSum),m_audioInfo.sampleFormat,m_audioInfo.sampleRate,0,NULL);//av_get_default_channel_layout(XEE::audioChannel)
		if(swr_init(m_audioSwr)<0)
		{
			LogStr("swr_init() for AV_SAMPLE_FMT_S16P fail");
			return NULL;
		}
	}
	av_init_packet(&m_audioPkt);
	return st;
}
AVStream *XAVStream::openVideo(XAVStreamQuality quality)
{
	AVStream *st = avformat_new_stream(m_pOutContext, NULL);
    if(!st) 
	{
        LogStr("Error:视频数据流建立失败!");
        return NULL;
    }
    AVCodecContext *c = st->codec;
    //find the video encoder
    AVCodec * codec = avcodec_find_encoder(m_pFormat->video_codec);
    if(!codec) 
	{
		LogStr("Error:视频编码器失败!");
		return NULL;
    }
	st->time_base.den = m_videoFrameRate;
	st->time_base.num = 1;
	avcodec_get_context_defaults3(c,codec);
	c->bit_rate = 9000000;	//码率，越大文件越大，压缩比越小
    c->width = m_videoWidth;
    c->height = m_videoHight;
	c->time_base.den = m_videoFrameRate;
	c->time_base.num = 1;
    c->gop_size = 12;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;
	//+++++++++++++++++++++++++
	//这些参数是为了提升画质
	switch(quality)
	{
	case AVS_QUALITY_HEIGHT:	//高质量
		c->qcompress = 0.5f;
		c->pre_me = 2;
		c->lmin = 1;
		c->lmax = 5;
		c->qmin = 1;
		c->qmax = 5;
		c->qblur = 0.0f;
		c->spatial_cplx_masking = 0.3f;
		c->me_pre_cmp = 2;
		c->rc_qsquish = 1;
		c->b_quant_factor = 1.25f;
		c->b_quant_offset = 1.25f;
		c->i_quant_factor = -0.8f;
		c->i_quant_offset = 0.0f;
		c->rc_strategy = 2;
		c->b_frame_strategy = 0;
		c->dct_algo = 0;
		c->lumi_masking = 0.0f;
		c->dark_masking = 0.0f; 
		break;
	case AVS_QUALITY_MIDDLE:	//低质量
		c->qcompress = 0.5f;
		c->pre_me = 0;
		c->lmin = 20;
		c->lmax = 200;
		c->qmin = 4;
		c->qmax = 20;
		c->qblur = 0.5f;
		c->spatial_cplx_masking = 0.3f;
		c->me_pre_cmp = 2;
		c->rc_qsquish = 1;
		c->b_quant_factor = 1.25f;
		c->b_quant_offset = 1.25f;
		c->i_quant_factor = -0.8f;
		c->i_quant_offset = 0.0f;
		c->rc_strategy = 2;
		c->b_frame_strategy = 0;
		c->dct_algo = 0;
		c->lumi_masking = 0.0f;
		c->dark_masking = 0.0f;
		break;
	case AVS_QUALITY_LOW:	//低质量，默认值
		break;
	}

    if(m_pFormat->video_codec == AV_CODEC_ID_H264)
	{
		m_bsfc = av_bitstream_filter_init("h264_mp4toannexb");
		if(m_bsfc == NULL) return false;
        av_opt_set(c->priv_data, "preset", "slow", 0);
	}

    if(avcodec_open2(c, codec,NULL) < 0) 
	{
		LogStr("Error:视频编码器打开失败!");
		return NULL;
    }

    m_videoOutbuf = NULL;
    if(!(m_pOutContext->oformat->flags & AVFMT_RAWPICTURE)) 
	{
        /* allocate output buffer */
        /* XXX: API change will be done */
        m_videoOutbufSize = 8000000;
        m_videoOutbuf = XMem::createArrayMem<uint8_t>(m_videoOutbufSize);//(uint8_t *)malloc(m_videoOutbufSize);
    }
	av_init_packet(&m_videoPkt);
	return st;
}    
void XAVStream::addFrameAudio(XMicrophone &mic,XAudioStream &aStr)
{
	//第一步:从麦克风中获取数据，这里逻辑上有问题，没有丢弃不需要的数据
	int ret = 0;
	unsigned char * audioData = mic.getCaptureData(&ret);
	if(ret == 0) return;			//从麦克风中提取输出失败
	if(!audioNeedData()) return;	//如果不需要数据则这里直接丢弃数据
	ret = ret * mic.getByteRate();	//音频数据的数量
	if(m_audioBuffSize[0] - m_curUsage[0] < ret)
	{//内存空间不够，这里分配内存空间
		if(!XMem::resizeMem<unsigned char>(m_audioBuff[0],m_curUsage[0],m_curUsage[0] + ret))
		{
			LogStr("内存分配失败");
			return;
		}
		m_audioBuffSize[0] = m_curUsage[0] + ret;
	}
	memcpy(m_audioBuff[0] + m_curUsage[0],audioData,ret);	//拷贝新数据
	m_curUsage[0] += ret;
	//第二步:从文件中获取声音数据
	while(audioNeedData())
	{
		if(m_curUsage[1] == 0 && aStr.getAFrame())
		{
			if(m_audioBuffSize[1] - m_curUsage[1] < aStr.getDataLen())
			{//内存空间不够，这里分配内存空间
				if(!XMem::resizeMem<unsigned char>(m_audioBuff[1],m_curUsage[1],m_curUsage[1] + aStr.getDataLen()))
				{
					LogStr("内存分配失败");
					return;
				}
				m_audioBuffSize[1] = m_curUsage[1] + aStr.getDataLen();
			}
			memcpy(m_audioBuff[1] + m_curUsage[1],aStr.getData(),aStr.getDataLen());	//拷贝新数据
			m_curUsage[1] += aStr.getDataLen();
		}
		if(m_curUsage[0] == 0) break;	//如果麦克风中已经没有数据，则这里出现了数据欠缺的问题
		int tmpSize = XEE_Min(m_curUsage[0],m_curUsage[1]);
		if(tmpSize == 0) continue;	//如果没有数据则继续解压数据
		//第三步:将两个音频数据进行混合
		if(m_audioBuffSize[2] - m_curUsage[2] < tmpSize)
		{//内存空间不够，这里分配内存空间
			if(!XMem::resizeMem<unsigned char>(m_audioBuff[2],m_curUsage[2],m_curUsage[2] + tmpSize))
			{
				LogStr("内存分配失败");
				return;
			}
			m_audioBuffSize[2] = m_curUsage[2] + tmpSize;
		}
		switch(m_audioInfo.sampleFormat)
		{
		case AV_SAMPLE_FMT_S16:
		case AV_SAMPLE_FMT_S16P:	//未测试
			{
				short *p0 = (short *)m_audioBuff[0];
				short *p1 = (short *)m_audioBuff[1];
				short *p2 = (short *)m_audioBuff[2];
				p2 += m_curUsage[2] >> 1;
				int timers = tmpSize >> 1;
				for(int i = 0;i < timers;++ i)
				{
					//方案1：简单混合算法
					//*p2 = (*p0 + *p1) >> 1;
					//方案2：正规做法
					if(*p0 < 0 && *p1 < 0) *p2 = *p0 + *p1 - ((*p0 * *p1) / -32767);
					else *p2 = *p0 + *p1 - ((*p0 * *p1) / 32768);
					
					++p0;++p1;++p2;
				}
			}
			break;
		case AV_SAMPLE_FMT_U8:	//未测试
		case AV_SAMPLE_FMT_U8P:	//未测试
			{
				unsigned char *p0 = m_audioBuff[0];
				unsigned char *p1 = m_audioBuff[1];
				unsigned char *p2 = m_audioBuff[2];
				p2 += m_curUsage[2];
				for(int i = 0;i < tmpSize;++ i)
				{
					//方案1：简单混合算法
					//*p2 = (*p0 + *p1) >> 1;
					//方案2：正规做法
					*p2 = *p0 + *p1 - ((*p0 * *p1) / 255);
					
					++p0;++p1;++p2;
				}
			}
			break;
		default:	//其他格式目前并不支持
			break;
		}
		m_curUsage[2] += tmpSize;
		//处理已经完成的数据
		if(m_curUsage[0] <= tmpSize) m_curUsage[0] = 0;
		else
		{//内存移动
			m_curUsage[0] -= tmpSize;
			memmove(m_audioBuff[0],m_audioBuff[0] + tmpSize,m_curUsage[0]);
		}
		if(m_curUsage[1] <= tmpSize) m_curUsage[1] = 0;
		else
		{//内存移动
			m_curUsage[1] -= tmpSize;
			memmove(m_audioBuff[1],m_audioBuff[1] + tmpSize,m_curUsage[1]);
		}
			
		addFrameAudio(m_audioBuff[2],m_curUsage[2]);
		m_curUsage[2] = 0;	//数据添加完成之后清空数据
		//Sleep(1);
	}
}
XBool XAVStream::openFile(const char *filename,int width,int height,int frameRate,XAudioInfo *info,XAVStreamQuality quality)
{	
	//记录相关的数据
	m_videoWidth = width;
	m_videoHight = height;
	m_videoFrameRate = frameRate;
	if(info == NULL)
	{
		m_audioInfo.channelSum = XEG.getAudioChannelSum();
		m_audioInfo.sampleFormat = AV_SAMPLE_FMT_S16;
		m_audioInfo.sampleRate = XEG.getAudioSampleRate();
	}else
	{
		m_audioInfo = * info;
	}
	//初始化
	av_register_all();
	avcodec_register_all();
	m_pFormat = av_guess_format(NULL,filename,NULL); //根据文件名获取文件格式信息，默认格式为MPEG
	if(m_pFormat)
	{
		if(avformat_alloc_output_context2(&m_pOutContext,NULL,NULL,filename) < 0)
		{
			LogStr("内存错误!");
			return XFalse;
		}
	}else
	{
		m_pFormat = av_guess_format("mpeg", NULL, NULL);
		if(!m_pFormat)
		{
			LogStr("文件格式不支持!");
			return XFalse;
		}
		if(avformat_alloc_output_context2(&m_pOutContext,NULL,"mpeg",filename) < 0)
		{
			LogStr("内存错误!");
			return XFalse;
		}
	}
	//m_pFormat->video_codec = AV_CODEC_ID_MPEG2VIDEO;
	//m_pFormat->audio_codec = CODEC_ID_MP2;		//CODEC_ID_MP3会存在连带问题，所以这里先确认
	m_pOutContext->oformat = m_pFormat;
	strcpy(m_pOutContext->filename,filename);	//复制文件名
	//根据视频格式建立对应的视频音频数据
	m_videoST = NULL;
    m_audioST = NULL;
    if(m_pFormat->video_codec != CODEC_ID_NONE) m_videoST = openVideo(quality);
    if(m_pFormat->audio_codec != CODEC_ID_NONE) m_audioST = openAudio();
	if(m_videoST == NULL || m_audioST == NULL) return XFalse;	//注意：遇到有图像没声音或者是有声音没图像的视频会有问题！！！
	av_dump_format(m_pOutContext, 0, filename, 1);	//将设置的所有数据显示出来

	m_pictureYUV = av_frame_alloc();
	if(m_pictureYUV == NULL) return XFalse;
//	m_pictureYUV->data[0] = XMem::createArrayMem<uint8_t>(m_videoST->codec->width * m_videoST->codec->height);
//	m_pictureYUV->data[1] = XMem::createArrayMem<uint8_t>(m_videoST->codec->width * m_videoST->codec->height);
//	m_pictureYUV->data[2] = XMem::createArrayMem<uint8_t>(m_videoST->codec->width * m_videoST->codec->height);
//	m_pictureYUV->linesize[0] = m_videoST->codec->width;  
//	m_pictureYUV->linesize[1] = m_videoST->codec->width / 2;  
//	m_pictureYUV->linesize[2] = m_videoST->codec->width / 2;  
	m_pictureYUV->format = m_videoST->codec->pix_fmt;
	m_pictureYUV->width  = m_videoST->codec->width;
	m_pictureYUV->height = m_videoST->codec->height;
	if(av_image_alloc(m_pictureYUV->data,m_pictureYUV->linesize,m_videoST->codec->width,m_videoST->codec->height,
		m_videoST->codec->pix_fmt,32) < 0) return XFalse;

	m_pictureRGB = av_frame_alloc();
	m_pictureRGB->linesize[0] = m_videoWidth * 3; 

	//打开输出文件
	if(!(m_pFormat->flags & AVFMT_NOFILE)
		&& avio_open(&m_pOutContext->pb,filename,AVIO_FLAG_WRITE) < 0) 
	{
        LogStr("文件打开失败!");
        return XFalse;
    }
	m_audioDataPos = 0;	//标记音频数据才开始读取
	if(m_audioST->codec->frame_size == 0) 
	{
		m_audioFrameSize = 2048 * av_get_bytes_per_sample(m_audioST->codec->sample_fmt) * m_audioST->codec->channels;
		m_audioFrameInSize = 2048 * av_get_bytes_per_sample(m_audioInfo.sampleFormat) * m_audioInfo.channelSum;
	}else
	{
		m_audioFrameSize = m_audioST->codec->frame_size * av_get_bytes_per_sample(m_audioST->codec->sample_fmt) * m_audioST->codec->channels;
		m_audioFrameInSize = m_audioST->codec->frame_size * av_get_bytes_per_sample(m_audioInfo.sampleFormat) * m_audioInfo.channelSum;
	}

	m_audioBuffFlag = XTrue;

	if(avformat_write_header(m_pOutContext,NULL) != 0) //写入文件头
	{
		LogStr("文件头写入失败!");
		return XFalse;
	}
	m_pSwsContext = sws_getContext(m_videoST->codec->width,m_videoST->codec->height,PIX_FMT_RGB24,   
									m_videoST->codec->width,m_videoST->codec->height,PIX_FMT_YUV420P,  
									SWS_POINT,NULL,NULL,NULL);	//SWS_BICUBIC
	if(m_pSwsContext == NULL) return XFalse;
	return XTrue;
}
XBool XAVStream::open(const char *filename,int width,int height,int frameRate,XAudioInfo *info,XAVStreamQuality quality)  
{	
	if(m_isOpen) return XTrue;	//防止重复打开流
	if(filename == NULL || width <= 0 || height <= 0 || frameRate <= 0) return XFalse;
	if(!openFile(filename,width,height,frameRate,info,quality)) return XFalse;
	//所有的打开完成
	m_withThread = XFalse;
	m_withDevice = XFalse;
//	m_autoMode = autoMode;
	m_autoTimer = 0.0f;
	m_frameTime = 1000.0f / frameRate;
	m_isOpen = XTrue;
	m_videoFrameIndex = 0;
	//m_audioFrameIndex = 0;
	m_inputAudioDataSum = 0;
	return XTrue;
}
bool XAVStream::pushVideoFrame()
{
	AVCodecContext * c = m_videoST->codec;
	//m_pictureYUV->pts = m_videoFrameIndex;
/*	int outSize = avcodec_encode_video(c, m_videoOutbuf, m_videoOutbufSize,m_pictureYUV); 

	if(outSize > 0)
	{
		if(m_pOutContext->oformat->flags & AVFMT_RAWPICTURE)
		{
			m_videoPkt.flags |= AV_PKT_FLAG_KEY;
			m_videoPkt.stream_index = m_videoST->index;
			m_videoPkt.data = (uint8_t *)m_pictureYUV;
			m_videoPkt.size = sizeof(AVPicture);
		}else
		{
			m_videoPkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, m_videoST->time_base);
			if(c->coded_frame->key_frame)
				m_videoPkt.flags |= AV_PKT_FLAG_KEY;
			m_videoPkt.stream_index = m_videoST->index;
			m_videoPkt.data = m_videoOutbuf;
			m_videoPkt.size = outSize;
		}
		//m_videoPkt.pts = m_videoFrameIndex;
		//++ m_videoFrameIndex;

		av_write_frame(m_pOutContext, &m_videoPkt);
		av_free_packet(&m_videoPkt);
	}*/
	av_init_packet(&m_videoPkt);
	m_videoPkt.data = NULL;
	m_videoPkt.size = 0;
	m_pictureYUV->pts = m_videoFrameIndex;
	++ m_videoFrameIndex;

	int haveData;
	if(m_withThread)
	{
		m_mutex.Lock();
		if(avcodec_encode_video2(c,&m_videoPkt,m_pictureYUV,&haveData) < 0)
		{
			LogStr("视频编码失败!");
			return false;
		}
		m_mutex.Unlock();
	}else
	{
		if(avcodec_encode_video2(c,&m_videoPkt,m_pictureYUV,&haveData) < 0)
		{
			LogStr("视频编码失败!");
			return false;
		}
	}
	if(haveData)
	{
		if(m_bsfc != NULL)
			av_bitstream_filter_filter(m_bsfc, m_videoST->codec, NULL, &m_videoPkt.data, &m_videoPkt.size, m_videoPkt.data, m_videoPkt.size, 0);
		av_interleaved_write_frame(m_pOutContext, &m_videoPkt);
		//av_write_frame(m_pOutContext, &m_videoPkt);
	}
	av_free_packet(&m_videoPkt);
	return haveData;
}
void XAVStream::addFrameRGB(unsigned char* p)
{
	if(!m_isOpen ||
		p == NULL ||
		m_videoST == NULL) return;
	//static int frames = 0;
	AVCodecContext * c = m_videoST->codec;
	m_pictureRGB->data[0] = p;  
	//RGB 2 YUV
	//img_convert((AVPicture *)m_pictureYUV,PIX_FMT_YUV420P,
	//	(AVPicture *)m_pictureRGB,PIX_FMT_RGB24,
	//	c->width,c->height);
	if(m_withThread)
	{
		m_mutex.Lock();
		imgConvert((AVPicture *)m_pictureYUV,(AVPicture *)m_pictureRGB,c->height);
		m_mutex.Unlock();
		return;
	}else
	{
		imgConvert((AVPicture *)m_pictureYUV,(AVPicture *)m_pictureRGB,c->height);
	}
	//下面是将图像数据压入到视频流，自动模式不会手动压入
	//这里做一个测试
	//RGB2YUV420(m_pictureYUV->data[0],m_pictureYUV->data[1],m_pictureYUV->data[2],p,c->width,c->height);
	//YUV420P2RGB(m_pictureYUV->data[0],m_pictureYUV->data[1],m_pictureYUV->data[2],p,c->width,c->height);
	
	pushVideoFrame();
	return;
}      
void XAVStream::flushAudioQueue()
{
	if(!m_isOpen ||
		m_inputAudioDataSum <= 0) return;
	AVCodecContext *c = m_audioST->codec;
	int haveData = 0;	//是否有获得数据
	while(true)
	{
		av_init_packet(&m_audioPkt);
		m_audioPkt.data = NULL;
		m_audioPkt.size = 0; 
		if(avcodec_encode_audio2(c,&m_audioPkt,NULL,&haveData) < 0) 
		{  
            LogStr("Error encoding audio frame");  
			break;
        }
		if(haveData)
		{  
			if(m_audioPkt.pts != AV_NOPTS_VALUE)
				m_audioPkt.pts = av_rescale_q(m_audioPkt.pts,	c->time_base, m_audioST->time_base);
			if(m_audioPkt.dts != AV_NOPTS_VALUE) 
				m_audioPkt.dts = av_rescale_q(m_audioPkt.dts,	c->time_base, m_audioST->time_base);
			if(m_audioPkt.duration > 0) m_audioPkt.duration = av_rescale_q(m_audioPkt.duration,	c->time_base, m_audioST->time_base);
			m_audioPkt.flags |= AV_PKT_FLAG_KEY;
			m_audioPkt.stream_index = m_audioST->index;
			if(m_aacbsfc != NULL)
				av_bitstream_filter_filter(m_aacbsfc, m_audioST->codec, NULL, &m_audioPkt.data, &m_audioPkt.size, m_audioPkt.data, m_audioPkt.size, 0);  
            //if(av_write_frame(m_pOutContext, &m_audioPkt) != 0) 
            if(av_interleaved_write_frame(m_pOutContext, &m_audioPkt) != 0) 
				LogStr("Error:音频数据写入失败!");
			av_free_packet(&m_audioPkt); 
		}else
		{
			av_free_packet(&m_audioPkt); 
			break;
		}
		Sleep(1);
	}
}
void XAVStream::flushVideoQueue()
{
	if(!m_isOpen) return;
	AVCodecContext * c = m_videoST->codec;
	int haveData;
	while(true)
	{
		av_init_packet(&m_videoPkt);
		m_videoPkt.data = NULL;
		m_videoPkt.size = 0;
		if(avcodec_encode_video2(c,&m_videoPkt,NULL,&haveData) < 0)
		{
			LogStr("视频编码失败!");
			return;
		}
		if(haveData)
		{
			if(m_bsfc != NULL)
				av_bitstream_filter_filter(m_bsfc, m_videoST->codec, NULL, &m_videoPkt.data, &m_videoPkt.size, m_videoPkt.data, m_videoPkt.size, 0);
			//av_write_frame(m_pOutContext, &m_videoPkt);
			av_interleaved_write_frame(m_pOutContext, &m_videoPkt);
			av_free_packet(&m_videoPkt);
		}else
		{
			av_free_packet(&m_videoPkt);
			break;
		}
		Sleep(1);
	}
}
XBool XAVStream::close()  
{
	if(!m_isOpen) return XFalse;
	if(m_withThread)
	{//这里需要关闭线程,等待线程关闭
		waitThreadEnd(m_threadState);
		//if(m_threadState == 1)
		//{
		//	m_threadState = 2;
		//	while(m_threadState != 3)
		//	{
		//		//if(m_threadState == 3) break;
		//		Sleep(1);
		//	}
		//}
	}
	if(m_withDevice)
	{
		m_micDevice.release();
		m_AudioStream.close();
	}
	flushAudioQueue();//这里清空音频队列里面的数据
	flushVideoQueue();

	//根据实际情况关闭音频视频
	av_write_trailer(m_pOutContext);	//写入文件尾

	if(m_bsfc != NULL) av_bitstream_filter_close(m_bsfc);
	if(m_aacbsfc != NULL) av_bitstream_filter_close(m_aacbsfc);
	//关闭输出文件
    if(!(m_pFormat->flags & AVFMT_NOFILE))
        avio_close(m_pOutContext->pb);
	//释放资源
 	if (m_videoST) closeVideo();
    if (m_audioST) closeAudio();
	//下面释放相关的资源
    for(unsigned int i = 0; i < m_pOutContext->nb_streams;++ i) 
	{
        av_freep(&m_pOutContext->streams[i]->codec);
        av_freep(&m_pOutContext->streams[i]);
    }

	//XMem::XDELETE_ARRAY(m_pictureYUV->data[0]);
	//XMem::XDELETE_ARRAY(m_pictureYUV->data[1]);
	//XMem::XDELETE_ARRAY(m_pictureYUV->data[2]);
	av_freep(&m_pictureYUV->data[0]);
//	av_freep(m_pictureYUV->data[1]);
//	av_freep(m_pictureYUV->data[2]);
	av_frame_free(&m_pictureYUV);
	av_frame_free(&m_pictureRGB);

	av_free_packet(&m_videoPkt);
	av_free_packet(&m_audioPkt);
	if(m_audioSwr != NULL) swr_free(&m_audioSwr);
	if(m_audioFrame != NULL) av_frame_free(&m_audioFrame);  

	av_free(m_pOutContext);
	sws_freeContext(m_pSwsContext);

	XMem::XDELETE_ARRAY(m_audioBuff[0]);
	XMem::XDELETE_ARRAY(m_audioBuff[1]);
	XMem::XDELETE_ARRAY(m_audioBuff[2]);
	m_audioBuffSize[0] = 0;
	m_audioBuffSize[1] = 0;
	m_audioBuffSize[2] = 0;
	m_curUsage[0] = 0;
	m_curUsage[1] = 0;
	m_curUsage[2] = 0;

	m_isOpen = XFalse;
	return XTrue;
}  
XBool XAVStream::open(const char *filename,int width,int height,int frameRate,
	void * audioDevice,XAudioDeviceType deviceType,XAudioInfo *info,XAVStreamQuality quality)
{
	if(m_isOpen) return XTrue;	//防止重复打开流
	if(filename == NULL || width <= 0 || height <= 0 || frameRate <= 0) return XFalse;
	if(!openFile(filename,width,height,frameRate,info,quality)) return XFalse;
	//所有的打开完成
	m_withThread = XTrue;
	m_withDevice = XFalse;
	m_autoTimer = 0.0f;
	m_frameTime = 1000.0f / frameRate;
	m_audioDevice = audioDevice;
	m_audioDeviceType = deviceType;

	setStop();
	//下面开启线程
	m_threadState = STATE_BEFORE_START;
	if(CreateThread(0,0,encodeThread,this,0,&m_encodeThread) == 0)
	{//线程建立失败
		LogStr("Encode thread create error!");
		return XFalse;
	}

	m_isOpen = XTrue;
	m_videoFrameIndex = 0;
	//m_audioFrameIndex = 0;
	m_inputAudioDataSum = 0;
	return XTrue;
}
DWORD WINAPI XAVStream::encodeThread(void * pParam)
{
	XAVStream &pPar = *(XAVStream *)pParam;
	pPar.m_threadState = STATE_START;

	int upTime = XTime::getCurrentTicks();
	int curTime = XTime::getCurrentTicks();

	while(pPar.m_threadState != STATE_SET_TO_END)
	{
		//if(pPar.m_threadState == STATE_SET_TO_END) break;
		if(pPar.m_isStop)
		{//这里需要丢弃麦克风的声音
			if(pPar.m_audioDeviceType == AUDIO_DEVICE_TYPE_MIC && pPar.m_audioDevice != NULL)
			{
				((XMicrophone *)pPar.m_audioDevice)->getCaptureData();
			}
			curTime = XTime::getCurrentTicks();
			upTime = curTime;
			continue;
		} 
		curTime = XTime::getCurrentTicks();
		pPar.m_autoTimer += curTime - upTime;	//这里是时间差
		upTime = curTime;
		if(pPar.m_autoTimer >= pPar.m_frameTime)
		{//需要增加一帧数据
			if(pPar.pushVideoFrame())
				pPar.m_autoTimer -= pPar.m_frameTime;
		}
		//下面加入音频数据
		if(pPar.m_audioDevice != NULL)
		{
			switch(pPar.m_audioDeviceType)
			{
			case AUDIO_DEVICE_TYPE_MIC: 
				pPar.addFrameAudio(*((XMicrophone *)pPar.m_audioDevice));
				break;
			case AUDIO_DEVICE_TYPE_FILE:
				pPar.addFrameAudio(*((XAudioStream *)pPar.m_audioDevice));
				break;
			}
		}
		Sleep(1);
	}
	pPar.m_threadState = STATE_END;
	return 1;
}
XBool XAVStream::openEx(const char *filename,int width,int height,int frameRate,
	const char * deviceName,XAudioDeviceType deviceType)
{//尚未完成
	if(m_isOpen) return XTrue;	//防止重复打开流
	//下面打开设备
	if(deviceType == AUDIO_DEVICE_TYPE_MIC) 
	{
		if(!m_micDevice.openDevice(deviceName,XEG.getAudioSampleRate(),AL_FORMAT_STEREO16,20000)) return XFalse;
		m_micDevice.captureStop();
		if(!open(filename,width,height,frameRate,&m_micDevice,deviceType))
		{
			m_micDevice.release();
			return XFalse;
		}
	}else 
	if(deviceType == AUDIO_DEVICE_TYPE_FILE)
	{
		if(!m_AudioStream.load(deviceName)) return XFalse;
		if(!open(filename,width,height,frameRate,&m_AudioStream,deviceType))
		{
			m_AudioStream.close();
			return XFalse;
		}
	}
	m_withDevice = XTrue;
	return XTrue;
}
#if !WITH_INLINE_FILE
#include "XAVStream.inl"
#endif
}