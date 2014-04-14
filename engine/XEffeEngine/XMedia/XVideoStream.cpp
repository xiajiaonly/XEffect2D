//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XVideoStream.h"
#include "XBasicFun.h"

_XBool _XVideoEncode::open(int w,int h,int rate)
{
	if(m_isOpen) return XFalse;
	av_register_all();
	AVCodec *codec = avcodec_find_encoder(VEDIO_STREAM_FORMAT);
	if(codec == NULL) return XFalse;
	m_videoCodec = avcodec_alloc_context3(codec);
	if(m_videoCodec == NULL) return XFalse;
	AVCodecContext *c = m_videoCodec;

	c->bit_rate = 800000;
    c->width = w;
    c->height = h;
	c->time_base.num = 1;
	c->time_base.den = rate;
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
//	c->qmin = 10;	//视频画面质量
//	c->qmax = 30;
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

    if(VEDIO_STREAM_FORMAT == AV_CODEC_ID_H264)  av_opt_set(m_videoCodec->priv_data, "preset", "slow", 0);

    if(avcodec_open2(m_videoCodec,codec,NULL) < 0) return XFalse;

	m_pFrameRGB = avcodec_alloc_frame();
	if(m_pFrameRGB == NULL) return XFalse;
	m_pFrameRGB->linesize[0] = w * 3;
	m_pFrameYUV = avcodec_alloc_frame();
	if(m_pFrameYUV == NULL) return XFalse;
//	m_pFrameYUV->data[0] = createArrayMem<uint8_t>(w * h);
//	m_pFrameYUV->data[1] = createArrayMem<uint8_t>(w * h);
//	m_pFrameYUV->data[2] = createArrayMem<uint8_t>(w * h);
//	m_pFrameYUV->linesize[0] = w;  
//	m_pFrameYUV->linesize[1] = w >> 1;  
//	m_pFrameYUV->linesize[2] = w >> 1;  
	m_pFrameYUV->format = c->pix_fmt;
    m_pFrameYUV->width  = c->width;
    m_pFrameYUV->height = c->height;
	if(av_image_alloc(m_pFrameYUV->data,m_pFrameYUV->linesize,c->width,c->height,
		c->pix_fmt,32) < 0) return 0;

	av_init_packet(&m_packet);
	m_packet.data = NULL;
	m_packet.size = 0;

	m_videoOutbufSize = w * h * 4;
    m_videoOutbuf = createArrayMem<uint8_t>(m_videoOutbufSize);
	m_pSwsContext = sws_getContext(w,h,PIX_FMT_RGB24,   
									w,h,PIX_FMT_YUV420P,  
									SWS_POINT,NULL,NULL,NULL);//SWS_BICUBIC
	if(m_pSwsContext == NULL) return XFalse;

	m_videoFrameIndex = 0;
	m_isOpen = XTrue;
	return XTrue;
}
_XBool _XVideoEncode::encodeDataRGB(unsigned char *in)
{
	if(!m_isOpen) return XFalse;
	if(in == NULL) return XFalse;
	m_pFrameRGB->data[0] = in;  
	//RGB 2 YUV
	//img_convert((AVPicture *)m_pFrameYUV,PIX_FMT_YUV420P,
	//	(AVPicture *)m_pFrameRGB,PIX_FMT_RGB24,
	//	m_videoCodec->width,m_videoCodec->height);
	imgConvert((AVPicture *)m_pFrameYUV,(AVPicture *)m_pFrameRGB,m_videoCodec->height); 

	av_free_packet(&m_packet);
	av_init_packet(&m_packet);
	m_packet.data = NULL;
	m_packet.size = 0;
	//m_pFrameYUV->pts = m_videoFrameIndex;

	//int finished = 0;
	//int ret = avcodec_encode_video2(m_videoCodec,&m_packet,m_pFrameYUV,&finished);
	//if(ret < 0) return XFalse;
	//if(finished)
	//{//压缩成功,这里并没有立即释放数据，注意
	//	++m_videoFrameIndex;
	//	return XTrue;
	//}
	//++m_videoFrameIndex;

	int outSize = avcodec_encode_video(m_videoCodec,m_videoOutbuf,m_videoOutbufSize,m_pFrameYUV); 
	if(outSize > 0)
	{
		m_packet.pts= m_videoCodec->coded_frame->pts;
		if(m_videoCodec->coded_frame->key_frame) m_packet.flags |= AV_PKT_FLAG_KEY;
		m_packet.stream_index = 0;
		m_packet.data = m_videoOutbuf;
		m_packet.size = outSize;
		return XTrue;
	}

	return XFalse;
}
void _XVideoEncode::release()
{
	if(!m_isOpen) return;
//	XDELETE_ARRAY(m_pFrameYUV->data[0]);
//	XDELETE_ARRAY(m_pFrameYUV->data[1]);
//	XDELETE_ARRAY(m_pFrameYUV->data[2]);
	av_freep(&m_pFrameYUV->data[0]);
	avcodec_free_frame(&m_pFrameRGB);
	avcodec_free_frame(&m_pFrameYUV);
	av_free(m_videoCodec);
	m_isOpen = XFalse;
}
_XBool _XVideoDecode::open(int w,int h)
{
	if(m_isOpen) return XFalse;
	//根据视频流信息寻找释放的解码器
	av_register_all();
	AVCodec *pCodec = avcodec_find_decoder(VEDIO_STREAM_FORMAT);
	if(pCodec == NULL) return XFalse;
	m_videoCodec = avcodec_alloc_context3(pCodec);
	m_videoCodec->width = w;
	m_videoCodec->height = h;
	if(m_videoCodec == NULL) return 0;
	if(pCodec->capabilities & CODEC_CAP_TRUNCATED) m_videoCodec->flags|= CODEC_FLAG_TRUNCATED;
	if(avcodec_open2(m_videoCodec,pCodec,NULL) < 0) return XFalse;
	//分配零时数据
	if((m_pFrame = avcodec_alloc_frame()) == NULL) return XFalse;
	m_pixelData = createArrayMem<unsigned char>(m_videoCodec->width * m_videoCodec->height * 3);	//存储像素数据
	if(m_pixelData == NULL) return XFalse;
	avpicture_fill(&m_frameData,m_pixelData,PIX_FMT_RGB24,m_videoCodec->width,m_videoCodec->height);

	m_pSwsContext = sws_getContext(w,h,PIX_FMT_YUV420P,//m_videoCodec->pix_fmt,    
									w,h,PIX_FMT_RGB24, 
									SWS_POINT,NULL,NULL,NULL);//SWS_BICUBIC
	if(m_pSwsContext == NULL) return XFalse;
	m_isOpen = XTrue;
	return XTrue;
}
_XBool _XVideoDecode::decodeData(void * data,int len)
{
	if(!m_isOpen) return XFalse;
	if(data == NULL) return XFalse;
	m_packet.data = (uint8_t *)data;
	m_packet.size = len;
	_XBool flag = XFalse;
	while(1)
	{
		int isFinished = 0;
		int ret = avcodec_decode_video2(m_videoCodec,m_pFrame,&isFinished,&m_packet);
		if(ret < 0) return XFalse;	//解码失败
		if(isFinished != 0)
		{//解码完成
			//img_convert(&m_frameData,PIX_FMT_RGB24,(AVPicture *)m_pFrame,m_videoCodec->pix_fmt,
			//	m_videoCodec->width,m_videoCodec->height);
			imgConvert(&m_frameData,(AVPicture *)m_pFrame,m_videoCodec->height);
			flag = XTrue;
		}
		m_packet.size -= ret;
		m_packet.data += ret;
		if(m_packet.size <= 0) break;
	}
	return flag;
}
void _XVideoDecode::release()
{
	if(!m_isOpen) return;
	avcodec_free_frame(&m_pFrame);
	XDELETE_ARRAY(m_pixelData);
	av_free(m_videoCodec);
	sws_freeContext(m_pSwsContext);
	m_isOpen = XFalse;
}