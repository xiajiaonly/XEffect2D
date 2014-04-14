//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XAVStream.h"  
#include "XBasicWindow.h"
#include <stdlib.h>  
#include "math.h"
#include "string.h"
#include "XColorSpace.h"
#include "XTimer.h"

//float STREAM_DURATION = 5.0f;
//int STREAM_NB_FRAMES = 0;//  ((int)(STREAM_DURATION * STREAM_FRAME_RATE))
//这个函数中的音频转换的数据存在问题，需要考量
void _XAVStream::addFrameAudio(const unsigned char * data,int size)
{//将音频数据写入到文件中
	if(!m_isOpen) return;
	if(data == NULL) return;
	if(m_audioST == NULL) return;
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
		while(1)
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
					printf("ERROR:swr_alloc!\n");
					return;
				}
				swr_alloc_set_opts(m_audioSwr,c->channel_layout,c->sample_fmt,c->sample_rate,
					av_get_default_channel_layout(m_audioInfo.channelSum),m_audioInfo.sampleFormat,m_audioInfo.sampleRate,0,NULL);
			}
			if(swr_init(m_audioSwr)<0)
			{
				printf("swr_init() for AV_SAMPLE_FMT_S16P fail");
				return;
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
			printf("Could not setup audio frame\n");  
			return; 
		} 

		int haveData;	//是否有获得数据
		av_init_packet(&m_audioPkt);
		m_audioPkt.data = NULL;
		m_audioPkt.size = 0;
		//注释掉可以避免长时间录的error，但是会造成一开始的warning，不知道尚不知为什么
		//m_audioFrame->pts = m_audioFrameIndex;	
		//++m_audioFrameIndex;

		ret = avcodec_encode_audio2(c,&m_audioPkt,m_audioFrame,&haveData);  
		if(ret < 0) 
		{  
            printf("Error encoding audio frame\n");  
            return;
        }
		if(haveData)
		{  
			if(m_audioPkt.pts != AV_NOPTS_VALUE) m_audioPkt.pts = av_rescale_q(m_audioPkt.pts,	c->time_base, m_audioST->time_base);
			if(m_audioPkt.dts != AV_NOPTS_VALUE) m_audioPkt.dts = av_rescale_q(m_audioPkt.dts,	c->time_base, m_audioST->time_base);
			if(m_audioPkt.duration > 0) m_audioPkt.duration = av_rescale_q(m_audioPkt.duration,	c->time_base, m_audioST->time_base);

			m_audioPkt.flags |= AV_PKT_FLAG_KEY;
			m_audioPkt.stream_index = m_audioST->index;
            if(av_write_frame(m_pOutContext, &m_audioPkt) != 0) 
				printf("Error:音频数据写入失败!\n");
		}

		m_audioDataPos -= m_audioFrameInSize;
		if(m_audioBuffFlag) memcpy(m_audioOutbuf,m_audioTempBuff + m_audioFrameInSize,m_audioDataPos);
		else memcpy(m_audioTempBuff,m_audioOutbuf + m_audioFrameInSize,m_audioDataPos);
		m_audioBuffFlag = !m_audioBuffFlag;

		av_free_packet(&m_audioPkt);  
	//	avcodec_free_frame(&frame);
	}
}
//下面是音频相关的
AVStream *_XAVStream::openAudio()
{
	AVStream *st = av_new_stream(m_pOutContext, 1); //建立一个音频流
    if(!st) 
	{
		printf("Error:音频流建立失败!\n");
		return NULL;
    }
    AVCodecContext * c = st->codec;
    //寻找音频编码器
	AVCodec *codec = avcodec_find_encoder(m_pFormat->audio_codec);
    if(!codec) 
	{
        printf("Error:音频编码器寻找失败!\n");
        return NULL;
    }
	avcodec_get_context_defaults3(c,codec);
	c->bit_rate = 64000;
	if(c->codec_id == CODEC_ID_MP3) c->sample_fmt = AV_SAMPLE_FMT_S16P;else	//不同的音频格式这里不同，需要考虑
	if(c->codec_id == CODEC_ID_MP2) c->sample_fmt = AV_SAMPLE_FMT_S16; 
	else c->sample_fmt = AV_SAMPLE_FMT_S16;

	c->sample_rate		= m_audioInfo.sampleRate;
	c->channels			= m_audioInfo.channelSum;
	c->channel_layout	= av_get_default_channel_layout(m_audioInfo.channelSum);

	//打开音频编码器
    if(avcodec_open2(c, codec,NULL) < 0) 
	{
        printf("Error:音频编码器打开失败!\n");
        return NULL;
    }
	//为音频数据建立缓存
    //m_audioOutbufSize = 20000;
    m_audioOutbufSize = 5 * av_samples_get_buffer_size(NULL, c->channels, c->frame_size,c->sample_fmt, 0);
    m_audioOutbuf = createArrayMem<uint8_t>(m_audioOutbufSize);//(uint8_t *)malloc(m_audioOutbufSize);
	m_audioTempBuff = createArrayMem<uint8_t>(m_audioOutbufSize);
	//建立音频转码的临时数据
	m_audioFrame = avcodec_alloc_frame();
	if(m_audioFrame == NULL)
	{
		printf("ERROR:avcodec_alloc_frame!\n");
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
			printf("ERROR:swr_alloc!\n");
			return NULL;
		}
		swr_alloc_set_opts(m_audioSwr,c->channel_layout,c->sample_fmt,c->sample_rate,
			av_get_default_channel_layout(m_audioInfo.channelSum),m_audioInfo.sampleFormat,m_audioInfo.sampleRate,0,NULL);//av_get_default_channel_layout(XEE::audioChannel)
	}
	av_init_packet(&m_audioPkt);
	return st;
}
AVStream *_XAVStream::openVideo()
{
	AVStream *st = av_new_stream(m_pOutContext, 0);
    if(!st) 
	{
        printf("Error:视频数据流建立失败!\n");
        return NULL;
    }
    AVCodecContext *c = st->codec;
    //find the video encoder
    AVCodec * codec = avcodec_find_encoder(m_pFormat->video_codec);
    if(!codec) 
	{
		printf("Error:视频编码器失败！\n");
		return NULL;
    }
	avcodec_get_context_defaults3(c,codec);
	c->bit_rate = 800000;
    c->width = m_videoWidth;
    c->height = m_videoHight;
	c->time_base.den = m_videoFrameRate;
	c->time_base.num = 1;
    c->gop_size = 12;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;
	//+++++++++++++++++++++++++
	//这些参数是为了提升画质
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
	//-----------------------

    if(m_pFormat->video_codec == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "slow", 0);

    if(avcodec_open2(c, codec,NULL) < 0) 
	{
		printf("Error:视频编码器打开失败！\n");
		return NULL;
    }

    m_videoOutbuf = NULL;
    if(!(m_pOutContext->oformat->flags & AVFMT_RAWPICTURE)) 
	{
        /* allocate output buffer */
        /* XXX: API change will be done */
        m_videoOutbufSize = 8000000;
        m_videoOutbuf = createArrayMem<uint8_t>(m_videoOutbufSize);//(uint8_t *)malloc(m_videoOutbufSize);
    }
	av_init_packet(&m_videoPkt);
	return st;
}    
_XBool _XAVStream::openFile(const char *filename,int width,int height,int frameRate,_XAudioInfo *info)
{	
	//记录相关的数据
	m_videoWidth = width;
	m_videoHight = height;
	m_videoFrameRate = frameRate;
	if(info == NULL)
	{
		m_audioInfo.channelSum = XEE::audioChannel;
		m_audioInfo.sampleFormat = AV_SAMPLE_FMT_S16;
		m_audioInfo.sampleRate = XEE::audioFrequency;
	}else
	{
		m_audioInfo = * info;
	}
	//初始化
	av_register_all();
	m_pFormat = av_guess_format(NULL,filename,NULL); //根据文件名获取文件格式信息，默认格式为MPEG
	if(m_pFormat)
	{
		m_pOutContext = avformat_alloc_output_context(NULL,NULL,filename);
		if(!m_pOutContext)
		{
			printf("内存错误！\n");
			return XFalse;
		}
	}else
	{
		m_pFormat = av_guess_format("mpeg", NULL, NULL);
		if(!m_pFormat)
		{
			printf("文件格式不支持!\n");
			return XFalse;
		}
		m_pOutContext = avformat_alloc_output_context("mpeg",NULL,filename);
		if(!m_pOutContext)
		{
			printf("内存错误！\n");
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
    if(m_pFormat->video_codec != CODEC_ID_NONE) m_videoST = openVideo();
    if(m_pFormat->audio_codec != CODEC_ID_NONE) m_audioST = openAudio();
	if(m_videoST == NULL || m_audioST == NULL) return XFalse;	//注意：遇到有图像没声音或者是有声音没图像的视频会有问题！！！
	av_dump_format(m_pOutContext, 0, filename, 1);	//将设置的所有数据显示出来

	m_pictureYUV = avcodec_alloc_frame();
	if(m_pictureYUV == NULL) return XFalse;
//	m_pictureYUV->data[0] = createArrayMem<uint8_t>(m_videoST->codec->width * m_videoST->codec->height);
//	m_pictureYUV->data[1] = createArrayMem<uint8_t>(m_videoST->codec->width * m_videoST->codec->height);
//	m_pictureYUV->data[2] = createArrayMem<uint8_t>(m_videoST->codec->width * m_videoST->codec->height);
//	m_pictureYUV->linesize[0] = m_videoST->codec->width;  
//	m_pictureYUV->linesize[1] = m_videoST->codec->width / 2;  
//	m_pictureYUV->linesize[2] = m_videoST->codec->width / 2;  
	m_pictureYUV->format = m_videoST->codec->pix_fmt;
	m_pictureYUV->width  = m_videoST->codec->width;
	m_pictureYUV->height = m_videoST->codec->height;
	if(av_image_alloc(m_pictureYUV->data,m_pictureYUV->linesize,m_videoST->codec->width,m_videoST->codec->height,
		m_videoST->codec->pix_fmt,32) < 0) return XFalse;

	m_pictureRGB = avcodec_alloc_frame();
	m_pictureRGB->linesize[0] = m_videoWidth * 3; 

	//打开输出文件
	if(!(m_pFormat->flags & AVFMT_NOFILE)) 
	{
        if(avio_open(&m_pOutContext->pb,filename,AVIO_FLAG_WRITE) < 0) 
		{
            printf("文件打开失败!\n");
            return XFalse;
        }
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
		printf("文件头写入失败!\n");
		return XFalse;
	}
	m_pSwsContext = sws_getContext(m_videoST->codec->width,m_videoST->codec->height,PIX_FMT_RGB24,   
									m_videoST->codec->width,m_videoST->codec->height,PIX_FMT_YUV420P,  
									SWS_POINT,NULL,NULL,NULL);	//SWS_BICUBIC
	if(m_pSwsContext == NULL) return XFalse;
	return XTrue;
}
_XBool _XAVStream::open(const char *filename,int width,int height,int frameRate,_XAudioInfo *info)  
{	
	if(m_isOpen) return XTrue;	//防止重复打开流
	if(filename == NULL || width <= 0 || height <= 0 || frameRate <= 0) return XFalse;
	if(!openFile(filename,width,height,frameRate,info)) return XFalse;
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
void _XAVStream::pushVideoFrame()
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
	int ret = avcodec_encode_video2(c,&m_videoPkt,m_pictureYUV,&haveData);
	if(ret < 0)
	{
		printf("ERROR!\n");
		return;
	}
	if(haveData) av_write_frame(m_pOutContext, &m_videoPkt);
	av_free_packet(&m_videoPkt);
}
void _XAVStream::addFrameRGB(unsigned char* p)
{
	if(!m_isOpen) return;
	if(p == NULL) return;
	if(m_videoST == NULL) return;
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
void _XAVStream::flushAudioQueue()
{
	if(!m_isOpen) return;
	if(m_inputAudioDataSum <= 0) return;
	AVCodecContext *c = m_audioST->codec;
	int haveData = 0;	//是否有获得数据
	int ret;
	while(1)
	{
		av_init_packet(&m_audioPkt);
		m_audioPkt.data = NULL;
		m_audioPkt.size = 0;
		ret = avcodec_encode_audio2(c,&m_audioPkt,NULL,&haveData);  
		if(ret < 0) 
		{  
            printf("Error encoding audio frame\n");  
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
            if(av_write_frame(m_pOutContext, &m_audioPkt) != 0) 
				printf("Error:音频数据写入失败!\n");
			av_free_packet(&m_audioPkt); 
		}else
		{
			av_free_packet(&m_audioPkt); 
			break;
		}
		Sleep(1);
	}
}
void _XAVStream::flushVideoQueue()
{
	if(!m_isOpen) return;
	AVCodecContext * c = m_videoST->codec;
	int haveData;
	int ret;
	while(1)
	{
		av_init_packet(&m_videoPkt);
		m_videoPkt.data = NULL;
		m_videoPkt.size = 0;
		ret = avcodec_encode_video2(c,&m_videoPkt,NULL,&haveData);
		if(ret < 0)
		{
			printf("ERROR!\n");
			return;
		}
		if(haveData)
		{
			av_write_frame(m_pOutContext, &m_videoPkt);
			av_free_packet(&m_videoPkt);
		}else
		{
			av_free_packet(&m_videoPkt);
			break;
		}
		Sleep(1);
	}
}
_XBool _XAVStream::close()  
{
	if(!m_isOpen) return XFalse;
	if(m_withThread)
	{//这里需要关闭线程,等待线程关闭
		if(m_threadState == 1)
		{
			m_threadState = 2;
			while(1)
			{
				if(m_threadState == 3) break;
				Sleep(1);
			}
		}
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
	//关闭输出文件
    if(!(m_pFormat->flags & AVFMT_NOFILE))
        avio_close(m_pOutContext->pb);
	//释放资源
 	if (m_videoST) closeVideo();
    if (m_audioST) closeAudio();
	//下面释放相关的资源
    for(int i = 0; i < m_pOutContext->nb_streams;++ i) 
	{
        av_freep(&m_pOutContext->streams[i]->codec);
        av_freep(&m_pOutContext->streams[i]);
    }

	//XDELETE_ARRAY(m_pictureYUV->data[0]);
	//XDELETE_ARRAY(m_pictureYUV->data[1]);
	//XDELETE_ARRAY(m_pictureYUV->data[2]);
	av_freep(&m_pictureYUV->data[0]);
//	av_freep(m_pictureYUV->data[1]);
//	av_freep(m_pictureYUV->data[2]);
	avcodec_free_frame(&m_pictureYUV);
	avcodec_free_frame(&m_pictureRGB);

	av_free_packet(&m_videoPkt);
	av_free_packet(&m_audioPkt);
	if(m_audioSwr != NULL) swr_free(&m_audioSwr);
	if(m_audioFrame != NULL) avcodec_free_frame(&m_audioFrame);  

	av_free(m_pOutContext);
	sws_freeContext(m_pSwsContext);

	m_isOpen = XFalse;
	return XTrue;
}  
_XBool _XAVStream::open(const char *filename,int width,int height,int frameRate,
	void * audioDevice,_XAudioDeviceType deviceType,_XAudioInfo *info)
{
	if(m_isOpen) return XTrue;	//防止重复打开流
	if(filename == NULL || width <= 0 || height <= 0 || frameRate <= 0) return XFalse;
	if(!openFile(filename,width,height,frameRate,info)) return XFalse;
	//所有的打开完成
	m_withThread = XTrue;
	m_withDevice = XFalse;
	m_autoTimer = 0.0f;
	m_frameTime = 1000.0f / frameRate;
	m_audioDevice = audioDevice;
	m_audioDeviceType = deviceType;

	setStop();
	//下面开启线程
	if(CreateThread(0,0,encodeThread,this,0,&m_encodeThread) == 0)
	{//线程建立失败
		printf("Encode thread create error!\n");
		return XFalse;
	}

	m_isOpen = XTrue;
	m_videoFrameIndex = 0;
	//m_audioFrameIndex = 0;
	m_inputAudioDataSum = 0;
	return XTrue;
}
DWORD WINAPI _XAVStream::encodeThread(void * pParam)
{
	_XAVStream *pPar = (_XAVStream *)pParam;
	pPar->m_threadState = 1;

	int upTime = getCurrentTicks();
	int nowTime = getCurrentTicks();

	while(1)
	{
		if(pPar->m_threadState == 2) break;
		if(pPar->m_isStop)
		{//这里需要丢弃麦克风的声音
			if(pPar->m_audioDeviceType == AUDIO_DEVICE_TYPE_MIC && pPar->m_audioDevice != NULL)
			{
				((_XMicrophone *)pPar->m_audioDevice)->getCaptureData();
			}
			nowTime = getCurrentTicks();
			upTime = nowTime;
			continue;
		} 
		nowTime = getCurrentTicks();
		pPar->m_autoTimer += nowTime - upTime;	//这里是时间差
		upTime = nowTime;
		if(pPar->m_autoTimer >= pPar->m_frameTime)
		{//需要增加一帧数据
			pPar->m_autoTimer -= pPar->m_frameTime;

			pPar->m_mutex.Lock();
			pPar->pushVideoFrame();
			pPar->m_mutex.Unlock();
		}
		//下面加入音频数据
		if(pPar->m_audioDeviceType == AUDIO_DEVICE_TYPE_MIC && pPar->m_audioDevice != NULL) pPar->addFrameAudio(*((_XMicrophone *)pPar->m_audioDevice)); else
		if(pPar->m_audioDeviceType == AUDIO_DEVICE_TYPE_FILE && pPar->m_audioDevice != NULL) pPar->addFrameAudio(*((_XAudioStream *)pPar->m_audioDevice));
		Sleep(1);
	}
	pPar->m_threadState = 3;
	return 1;
}
_XBool _XAVStream::openEx(const char *filename,int width,int height,int frameRate,
	const char * deviceName,_XAudioDeviceType deviceType)
{//尚未完成
	if(m_isOpen) return XTrue;	//防止重复打开流
	//下面打开设备
	if(deviceType == AUDIO_DEVICE_TYPE_MIC) 
	{
		if(m_micDevice.openDevice(deviceName,XEE::audioFrequency,AL_FORMAT_STEREO16,20000) == 0) return XFalse;
		m_micDevice.captureStop();
		if(open(filename,width,height,frameRate,&m_micDevice,deviceType) == 0)
		{
			m_micDevice.release();
			return XFalse;
		}
	}else 
	if(deviceType == AUDIO_DEVICE_TYPE_FILE)
	{
		if(!m_AudioStream.load(deviceName)) return XFalse;
		if(open(filename,width,height,frameRate,&m_AudioStream,deviceType) == 0)
		{
			m_AudioStream.close();
			return XFalse;
		}
	}
	m_withDevice = XTrue;
	return XTrue;
}