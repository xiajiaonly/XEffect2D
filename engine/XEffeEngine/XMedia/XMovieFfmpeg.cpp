#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMovieFfmpeg.h"
#include "XSoundCommon.h"
#if AUDIO_MATHOD == 1
#include "SDL_audio.h"
#endif
#include "XTimer.h"
namespace XE{
XMovieFfmpeg::XMovieFfmpeg(void)
	:videoStream(-1)
	, audioStream(-1)
	, m_isBuffing(XTrue)
	, m_isLoop(XFalse)
	, m_videoWidth(0)
	, m_videoHeight(0)
	, m_curFrameNumber(0)
	, m_needVoice(XTrue)
	, m_autoTimer(XTrue)
	//,m_texture(NULL)
	, m_isNewFrame(false)
	, m_movieTex(NULL)
	, m_movieSprite(NULL)
	, m_curFrameData(NULL)
	, m_isLoaded(XFalse)
	, m_isEnd(XFalse)
	, m_isQuit(XFalse)
	, m_isStop(XFalse)
	, m_isThreadDecoderExit(XTrue)
	, m_isThreadDrawExit(XTrue)
	, m_pSwsContext(NULL)
	, m_pSwrContext(NULL)
	, m_outColorMode(COLOR_RGB)
	, videofilename("")
	, pFormatCtx(NULL)	//视频格式描述内容
	, pCodeCtx(NULL)		//视频解码描述内容
	, aCodeCtx(NULL)		//音频解码描述内容
	, pFrame(NULL)
	, pAudioFrame(NULL)
	, m_isGetFirstFrame(false)
	, m_audioBuf(NULL)
#ifdef DEBUG_MODE
	, m_vpSum(0)
	, m_vfSum(0)
#endif
	, m_playSpeed(1.0f)
{
	m_videoQueue.nb_pict = 0;
	//	for(int i = 0;i < MAX_BUFF_DEEP;++ i)
	//	{
	//		m_bmp[i] = NULL;
	//	}
	memset(m_bmp, 0, MAX_BUFF_DEEP * sizeof(XVideoFrameData *));
}
//这个线程用于更具时间将视频数据更新到贴图
#ifdef XEE_OS_LINUX
void * XMovieFfmpeg::drawThread(void *arg)
#endif
#ifdef XEE_OS_WINDOWS
DWORD WINAPI XMovieFfmpeg::drawThread(void *arg)
#endif
{
	XMovieFfmpeg *pthis = (XMovieFfmpeg *)arg;
	float refreshtime = 1000.0f/(* pthis).m_curFrameRate;//影片显示一帧的时间，单位：毫秒
	float temptime = refreshtime;
	pthis->m_isThreadDrawExit = XFalse;

	////第一播放时需要等缓冲满
	//while(true)
	//{//等待视频缓冲结束
	//	if(!pthis->m_isBuffing) break;
	//	if(pthis->m_isQuit) break;	//当程序退出时，这里必须要退出，否则就是死循环，造成程序停止响应。
	//	mySleep(1);
	//}	

	int startTime = XTime::getCurrentTicks();		//记录开始播放的时间
	pthis->m_curPlayedTime = 0.0f;			//播放的时间通过这个来计算

	while(true)
	{	
		if(pthis->m_isQuit) break;
		if (pthis->m_isBuffing && pthis->m_isDecodeOver)
			pthis->m_isBuffing = false;

		if(pthis->m_isStop || pthis->m_isEnd || pthis->m_isBuffing)
		{//停止状态,或者播放完成则等待
			XEE::sleep(1);
			startTime = XTime::getCurrentTicks();
			continue;
		}
		if(pthis->m_videoQueue.nb_pict > 0 || pthis->videoStream == -1)
		{//视频队列中有数据
			pthis->m_gotoMutex1.Lock();
			if(pthis->m_autoTimer) pthis->m_curPlayedTime += (XTime::getCurrentTicks() - startTime) * pthis->m_playSpeed;
			startTime = XTime::getCurrentTicks();
		//	playedTime = getCurrentTicks() - startTime;
			if(pthis->m_curPlayedTime >= pthis->m_curFrameNumber * temptime)
			{//如果达到时间点则开始播放
				pthis->getPixelData();
				if(pthis->m_curFrameNumber >= pthis->m_allFrameSum)
				{//如果时间越界，则播放完成
					pthis->m_isEnd = XTrue;
					//pthis->m_audioQueue.cFlag = true;
					//if(pthis->m_audioQueue.cond != NULL) 
					//	SDL_CondSignal(pthis->m_audioQueue.cond);
#ifdef WITH_DEBUG_INFO
					LogStr("Time play over+!");
#endif
					if(pthis->m_isLoop) pthis->replay();
					startTime = XTime::getCurrentTicks();
				}
			}else if(pthis->m_autoTimer)
			{//否则则等到时间点再播放
				int tempTime = pthis->m_curFrameNumber * temptime - pthis->m_curPlayedTime;
#ifdef WITH_DEBUG_INFO
				if(tempTime < 0 || tempTime > 2000)
				{
					LogStr("haha");
				}
#endif
				XEE::sleep(tempTime);
			} 
			pthis->m_gotoMutex1.Unlock();
		}else
		{
			if(pthis->m_isDecodeOver)	//视频解码完成
			{//已经解码完成
				pthis->m_isEnd = XTrue;
				//pthis->m_audioQueue.cFlag = true;
#ifdef WITH_DEBUG_INFO
				LogStr("Time play over-!");
#endif
				if(pthis->m_isLoop) pthis->replay();
				startTime = XTime::getCurrentTicks();
			}else
			{//等待继续解码
#ifdef WITH_DEBUG_INFO
				LogStr("-->buff over!");
#endif
				pthis->m_isBuffing = XTrue;	//设置开始缓冲数据
			//	mySleep(5000);
			//	pthis->m_isStop = XFalse;
				startTime = XTime::getCurrentTicks();
			}
		}
		XEE::sleep(1);
	}
	pthis->m_isThreadDrawExit = XTrue;
//	_endthread();
#ifdef XEE_OS_WINDOWS
	return 1;
#endif
}
//视频解码线程，在DrawThread线程不显示视频的时候把视频帧解码到队列，把音频包放到队列
#ifdef XEE_OS_WINDOWS
DWORD WINAPI XMovieFfmpeg::decoderThread(void *arg)
#endif
#ifdef XEE_OS_LINUX
void * XMovieFfmpeg::decoderThread(void *arg)
#endif
{
	XMovieFfmpeg *pthis = (XMovieFfmpeg *)arg;
	//AVPicture pict;
	int bufernum = (int)(BUFF_TIMER * pthis->m_curFrameRate); //设置缓冲区大小为影片2秒播放的帧数
	//int frameFinished;		//解码动作是否完成
	if(pthis->videoStream == -1) bufernum = 1;
	if(bufernum >= MAX_BUFF_DEEP) bufernum = MAX_BUFF_DEEP;
	//int picIndex = 0;
	unsigned long audioindex = 0;
	pthis->m_isThreadDecoderExit = XFalse;

	while(!pthis->m_isQuit)//如果强制线程退出，则这里退出
	{
		if(pthis->m_isDecodeOver)
		{//如果已经完成一次解码则等待
			if(pthis->videoStream == -1 && pthis->m_audioQueue.nb_packets <= 0 && 
				!pthis->m_isEnd)
			{//如果没有图像的话直接这里设置播放完成
				pthis->m_isEnd = XTrue;
				//pthis->m_audioQueue.cFlag = true;
#ifdef WITH_DEBUG_INFO
				LogStr("Time play over-!");
#endif
				if(pthis->m_isLoop) pthis->replay();
			}
			XEE::sleep(1);
			continue;
		}
		//图片缓冲完成，或者是没有视频信息
		if((pthis->m_videoQueue.nb_pict < bufernum && pthis->videoStream >= 0) || 
			(pthis->videoStream == -1 && pthis->m_audioQueue.nb_packets < bufernum))	//最大帧的缓存数量为2秒
		{//如果没有缓冲满则进行缓冲
			av_init_packet(&pthis->packet);
			pthis->m_gotoMutex.Lock();
			int ret = av_read_frame(pthis->pFormatCtx, &pthis->packet);
			if(ret == 0)
			{//如果读取帧成功
				if(pthis->packet.stream_index == pthis->videoStream)// && pthis->packet.duration > 0)
				{//读取的是视频帧	
					pthis->putIntoVideoPicList();
#ifdef DEBUG_MODE
					++pthis->m_vpSum;
#endif
				}else if(pthis->packet.stream_index == pthis->audioStream)// && pthis->packet.duration > 0)
				{//如果读取的是音频帧
					audioindex += pthis->packet.duration;
					if(pthis->m_needVoice) pthis->putIntoPacketQueue();
					else av_free_packet(&pthis->packet);	//如果不需要播放音频这直接放弃掉音频数据(估计这里会出错)
				}else
				{//不知道读取的是啥数据
					av_free_packet(&pthis->packet);	//(估计这里会出错)
				}
			}else if(ret < 0)	//如果提取帧失败，则默认已经播放完else
			{//视频解码完毕
				pthis->m_isDecodeOver = XTrue;
#ifdef WITH_DEBUG_INFO
				LogStr("DecodeOver");
#endif
			}
			pthis->m_gotoMutex.Unlock();
		}else if(pthis->m_isBuffing)	//第一次播放的时候一定要等缓冲满
		{//缓冲完毕，设置bplay , 进行播放
			pthis->m_isBuffing = XFalse;
		}
		XEE::sleep(1);
	}
	pthis->m_isThreadDecoderExit = XTrue;
#ifdef WITH_DEBUG_INFO
	LogStr("DecodeThread over");
#endif
	//	_endthread();
#ifdef XEE_OS_WINDOWS
	return 1;
#endif
}
//int createNumber = 0;
//在音频队列里取出音频包
int XMovieFfmpeg::getFromPacketQueue(AVPacket *pkt)
{ 
	AVPacketList *pkt1; 
	int ret; 

	while(true)
	{ 
		if(m_isStop) return -1;//暂停的话这里停止播放声音

//#ifdef WITH_DEBUG_INFO
//		LogStr("57");
//#endif
		gUnlock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//		LogStr("58");
//#endif
		pkt1 = m_audioQueue.first_pkt; 
		if(pkt1 != NULL) 
		{//有数据
			m_audioQueue.first_pkt = pkt1->next; 
			if(m_audioQueue.first_pkt == NULL) m_audioQueue.last_pkt = NULL;	//所有数据都已经取出
			--m_audioQueue.nb_packets; 
			m_audioQueue.size -= pkt1->pkt.size; 
			*pkt = pkt1->pkt; 
			XMem::XDELETE(pkt1);	//删除临时数据
		//	-- createNumber;
		//	printf("%d,%d\n",createNumber,pClass->m_audioQueue.nb_packets);
//#ifdef WITH_DEBUG_INFO
//			LogStr("59");
//#endif
			gUnlock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//			LogStr("60");
//#endif
			ret = 1; 
			break; 
		}else	//这后面的代码有可能会造成声音越来越慢
		{//等待同步(原因在这里)
//#ifdef WITH_DEBUG_INFO
//			LogStr("61");
//#endif
			gUnlock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//			LogStr("61");
//#endif
			//这里设置为不等待同步
			if(m_isQuit || m_isEnd) return -1;	//不等待同步
		} 
		XEE::sleep(1);
	} 
	return ret;
} 
//把音频包解码成音频数据
int XMovieFfmpeg::audioDecodeFrame(uint8_t *audio_buf,int buf_size)
{
	AVPacket pkt,pkt1;
	uint8_t *out[] = {audio_buf}; 
	int pkt_pos,pkt_len;
	int len = 0;
	int data_size = 0; 
	int isFinished;
	if(getFromPacketQueue(&pkt) < 0) return -1;//如果音频中没有数据，则返回  
	pkt1 = pkt;  
    pkt_pos = 0;  
    pkt_len = pkt.size;
	int frameDataSum = XEG.getAudioChannelSum() * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

	if(pkt.size > 0)
	{
		while(pkt1.size > 0)
		{
			len = avcodec_decode_audio4(aCodeCtx,pAudioFrame,&isFinished,&pkt1);
			if(len < 0)
			{//解压失败
				av_free_packet(&pkt);
#ifdef WITH_DEBUG_INFO
				LogStr("音频中没有数据!");
#endif
				return -1;
			}
			if(isFinished)
			{//格式转换
			//	SwrContext *swr = swr_alloc();
			//	swr_alloc_set_opts(swr,av_get_default_channel_layout(XEE::audioChannel),AV_SAMPLE_FMT_S16,XEE::audioSampleRate,
			//		c->channel_layout,c->sample_fmt,c->sample_rate,0,NULL);
			//	if(swr_init(pClass->m_pSwrContext)<0)
			//	{
			//		LogStr("swr_init() fail");
			//		return -1;
			//	}
				int outSize = buf_size / frameDataSum;
				len = swr_convert(m_pSwrContext,out,outSize,(const uint8_t **)pAudioFrame->extended_data,pAudioFrame->nb_samples);  
				len = len * frameDataSum;
 
	            av_free_packet(&pkt);
			//	swr_free(&swr);
	            return len;		//逻辑上这里可能存在数据包丢失,如果此时数据包的数据并没有完全解码成功的话这里会被抛弃
	        }else
			{  
	            if(!pkt1.data && aCodeCtx->codec->capabilities & CODEC_CAP_DELAY)
				{  
	                break;  
	            }  
	        }  
			pkt_pos += len;//已经解码的长度  
			pkt1.data = pkt.data + pkt_pos;  
			pkt1.size = pkt.size - pkt_pos; 

			if(m_isQuit ||
				m_isStop) return -1;//暂停的话这里停止播放声音
		}
	}
	av_free_packet(&pkt);
	return data_size;
}
//int audioDecodeFrame(XMovieFfmpeg *pClass,uint8_t *audio_buf,int buf_size)
//{ 
//	static AVPacket pkt;
//	static uint8_t *audio_pkt_data = NULL; 
//	static int audio_pkt_size = 0; 
//	int len = 0;
//	int data_size = 0; 
//	AVCodecContext * c= pClass->aCodeCtx;
//	while(true)
//	{ 
//		if(pClass->m_isQuit) return -1;
//		//while(audio_pkt_size > 0) 
//		if(audio_pkt_size > 0)
//		{ 
//			if(pClass->m_isQuit) return -1;
//			data_size = buf_size; 
//
//			len = avcodec_decode_audio3(c,(int16_t *)audio_buf,&data_size,&pkt);//audio_pkt_data,audio_pkt_size); //声音解码
//			if(len < 0) 
//			{//解码发生故障 
//				av_free_packet(&pkt);
//				audio_pkt_size = 0; 
//				return -1;
//			} 
//			audio_pkt_data += len;		//如果一次音频解码没有完成则重复进行直到完成为止
//			audio_pkt_size -= len; 
//			if(pkt.data != NULL) av_free_packet(&pkt);//释放资源
//			audio_pkt_size = 0;	//丢弃多出来的数据
//			if(data_size <= 0) return -1; 
//
//			//这里进行转码
//			if(c->sample_fmt != AV_SAMPLE_FMT_S16)
//			{
//				SwrContext *swr = swr_alloc();
//				swr_alloc_set_opts(swr,av_get_default_channel_layout(XEE::audioChannel),AV_SAMPLE_FMT_S16,XEE::audioSampleRate,
//					c->channel_layout,c->sample_fmt,c->sample_rate,0,NULL);
//
//				if(swr_init(swr)<0)
//				{
//					printf("swr_init() for AV_SAMPLE_FMT_S16P fail");
//					return -1;
//				}
//				uint8_t *out[] = {audio_buf};
//				uint8_t tempTest[50000];	//注意这个数值
//				int ret = 0;
//				memcpy(tempTest,audio_buf,data_size);
//				if(c->sample_fmt == AV_SAMPLE_FMT_S16P)
//				{
//					const uint8_t *in[] = {tempTest,tempTest + (data_size >> 1)};
//					ret = swr_convert(swr,out,data_size,in,(data_size >> 2));
//				}else
//				if(c->sample_fmt == AV_SAMPLE_FMT_FLTP)
//				{//这里存在问题,严重的噪声
//					const uint8_t *in[] = {tempTest};
//					ret = swr_convert(swr,out,data_size,in,data_size);
//				}
//				swr_free(&swr);
//			}
//			return data_size;
//		} 
//		if(pkt.data != NULL) av_free_packet(&pkt);	//释放掉内存空间 
// 
//		if(pClass->m_isStop) return -1;//暂停的话这里停止播放声音
//		if(getFromPacketQueue(pClass,&pkt, 1) < 0) return -1;//如果音频中没有数据，则返回  
//
//		audio_pkt_data = pkt.data; 
//		audio_pkt_size = pkt.size; 
//	} 
//}
//声音回调函数，解码音频数据,将音频数据推到缓存供SDL播放
void XMovieFfmpeg::audioCallBack(void *userdata,Uint8 *stream,int len)
{
	XMovieFfmpeg &pPar = *(XMovieFfmpeg *)userdata;

	int len1 = 0;
	int audio_size = 0; 
	while(len > 0)
	{//当有声音需要播放时 	
		if(pPar.m_audioBufIndex >= pPar.m_audioBufSize)
		{//当解压的音频全部播放完成时才解压后面的音频
			if(pPar.m_isQuit || pPar.m_isBuffing || 
				pPar.m_isStop || pPar.m_isEnd)
			{//不需要播放声音的时候播放空白数据(这里如果不填空白数据的话会造成主显示被阻塞，需要详细查明原因)
			//	if(len > 1024) pPar.m_audioBufSize = 1024;	//如果没有数据则填充空数据,主要是暂停的时候
			//	else pPar.m_audioBufSize = len;
			//	memset(pPar.m_audioBuf, 0, pPar.m_audioBufSize); 
				memset(stream,0,len);
				break;
			}else
			{
				audio_size = pPar.audioDecodeFrame(pPar.m_audioBuf,(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2);//声音解码函数 
				if(audio_size < 0) 
				{//如果音频解码没有数据.则清空内存空间
				//	if(len > 1024) pPar.m_audioBufSize = 1024;	//如果没有数据则填充空数据,主要是暂停的时候
				//	else pPar.m_audioBufSize = len;
				//	memset(pPar.m_audioBuf, 0, pPar.m_audioBufSize); 
					memset(stream,0,len);
					return;
				}else
				{//记录解压出来的音频数据量
					pPar.m_audioBufSize = audio_size; 
				} 
			}
			pPar.m_audioBufIndex = 0; 
		} 
		len1 = pPar.m_audioBufSize - pPar.m_audioBufIndex;	//记录音频的长度
		if(len1 > len) len1 = len;
		//下面两行代码在目前的框架下功能一样
		memcpy(stream,(uint8_t *)pPar.m_audioBuf + pPar.m_audioBufIndex,len1);
		
		len -= len1; 
		stream += len1; 
		pPar.m_audioBufIndex += len1; 
	}
}
//void audioCallBack(void *userdata,Uint8 *stream,int len)
//{
//	XMovieFfmpeg * pClass = (XMovieFfmpeg *)userdata;
//	//AVCodecContext *aCodecCtx = (AVCodecContext *)userdata; 
//	int len1 = 0;
//	int audio_size = 0; 
//
//	//static uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; 
//	//static unsigned int audio_buf_size = 0; 
//	//static unsigned int audio_buf_index = 0; 
//	//uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; 
//	//unsigned int audio_buf_size = 0; 
//	//unsigned int audio_buf_index = 0; 
//
//	while(len > 0)
//	{//当有声音需要播放时 	
//		if(pClass->m_audioBufIndex >= pClass->m_audioBufSize)
//		{//当解压的音频全部播放完成时才解压后面的音频
//			if(pClass->m_isQuit) return; 
//			audio_size = audioDecodeFrame(pClass,pClass->m_audioBuf,(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2);//声音解码函数 
//			if(audio_size < 0) 
//			{//如果音频解码没有数据.则清空内存空间
//			//	pClass->m_audioBufSize = 1024; 
//			//	memset(pClass->m_audioBuf,0,pClass->m_audioBufSize); 
//				pClass->m_audioBufSize = 0;
//			}else
//			{//记录解压出来的音频数据量
//				pClass->m_audioBufSize = audio_size; 
//			} 
//			pClass->m_audioBufIndex = 0; 
//		} 
//		len1 = pClass->m_audioBufSize - pClass->m_audioBufIndex;	//记录音频的长度
//		if(len1 > len) len1 = len;
//		
//		//下面需要对音频进行转码,SDL好像有问题
//	/*	if(XEE::audioSampleRate != pClass->aCodeCtx->sample_rate && len1 > 0)
//		{
//			SDL_AudioCVT  wav_cvt;
//			int ret = SDL_BuildAudioCVT(&wav_cvt,
//				XEE::audioFormat,XEE::audioChannel,pClass->aCodeCtx->sample_rate,
//				XEE::audioFormat,XEE::audioChannel,XEE::audioSampleRate);
//			if(ret == -1) printf("Couldn't build converter!\n");
//			wav_cvt.buf = (Uint8 *)malloc(len1 * wav_cvt.len_mult);
//			wav_cvt.len = len1;
//			memcpy(wav_cvt.buf,pClass->m_audioBuf + pClass->m_audioBufIndex,wav_cvt.len);
//			if(SDL_ConvertAudio(&wav_cvt) == -1)
//			{
//				printf("conver error!");
//			}
//			SDL_MixAudio(stream,wav_cvt.buf, wav_cvt.len_cvt,128);
//			free(wav_cvt.buf);
//		}else
//		if(len1 > 0)
//		{
//			SDL_MixAudio(stream, (uint8_t *)pClass->m_audioBuf + pClass->m_audioBufIndex, len1,128);
//		}*/
//		SDL_MixAudio(stream, (uint8_t *)pClass->m_audioBuf + pClass->m_audioBufIndex, len1,128);
//		//下面使用FFMPEG的方式进行转码
//		//AVCodecContext * c= pClass->aCodeCtx;
//		//if(XEE::audioSampleRate != c->sample_rate 
//		//	|| c->sample_fmt != AV_SAMPLE_FMT_S16
//		//	|| c->channels != XEE::audioChannel)
//		//{
//		//	int dataPackSize = av_samples_get_buffer_size(NULL,c->channels,c->frame_size,c->sample_fmt, 1);
//		//	SwrContext *swr = swr_alloc();
//		//	swr_alloc_set_opts(swr,av_get_default_channel_layout(XEE::audioChannel),AV_SAMPLE_FMT_S16,XEE::audioSampleRate,
//		//		c->channel_layout,c->sample_fmt,c->sample_rate,0,NULL);
//		//	if(swr_init(swr)<0)
//		//	{
//		//		printf("swr_init() for AV_SAMPLE_FMT_S16P fail");
//		//		return;
//		//	}
//		//	uint8_t tempTest[50000];	//注意这个数值
//		//	memcpy(tempTest + pClass->m_audioBufIndex,pClass->m_audioBuf,dataPackSize);
//		//	uint8_t *out[] = {tempTest};
//		//	const uint8_t *in[] = {pClass->m_audioBuf + pClass->m_audioBufIndex,pClass->m_audioBuf + pClass->m_audioBufIndex + (dataPackSize >> 1)};
//		//	swr_convert(swr,out,dataPackSize,in,(dataPackSize >> 2));
//		//	swr_free(&swr);
//
//		//	SDL_MixAudio(stream,tempTest,dataPackSize,128);
//		//}else
//		//{
//		//	SDL_MixAudio(stream, (uint8_t *)pClass->m_audioBuf + pClass->m_audioBufIndex, len1,128);
//		//}
//		len -= len1; 
//		stream += len1; 
//		pClass->m_audioBufIndex += len1; 
//	//	mySleep(1);
//	}
//}
int selectChannelLayout(AVCodec *codec) 
{ 
    const uint64_t *p; 
    uint64_t bestChLayout = 0; 
    int bestNbChannells   = 0; 
 
    if (!codec->channel_layouts) 
        return AV_CH_LAYOUT_STEREO; 
 
    p = codec->channel_layouts; 
    while (*p)
	{ 
        int nb_channels = av_get_channel_layout_nb_channels(*p); 
 
        if (nb_channels > bestNbChannells) 
		{ 
            bestChLayout    = *p; 
            bestNbChannells = nb_channels; 
        } 
       ++ p; 
    } 
    return bestNbChannells; 
} 	
//打开影片 ，初始化SDL , ffmpeg库等
XBool XMovieFfmpeg::load(const char *filename,XColorMode mode,bool withVoice)
{
	if(m_isLoaded || filename == NULL) return XFalse;
	m_isTexInit = false;
	m_outColorMode = mode;
	videoStream = -1;
	audioStream = -1;
	m_isBuffing = XTrue;
	m_isStop = XTrue;
//	m_isLoop = FALSE;
	m_videoWidth = 0;
	m_videoHeight = 0;
	m_curFrameNumber = 0;
	m_videoQueue.nb_pict = 0;
	m_isEnd = XFalse;
	m_isQuit = XFalse;
	m_needVoice = withVoice;
	m_isGetFirstFrame = false;
	XMem::XDELETE_ARRAY(m_audioBuf);
	m_audioBuf = XMem::createArrayMem<uint8_t>((AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2);

	int i = 0;
	videofilename = filename;		//记录视频文件名
	av_register_all();					//初始化ffmpeg库
//	avcodec_register_all();
//	avformat_network_init();
	pFormatCtx = NULL;
	if(avformat_open_input(&pFormatCtx,videofilename.c_str(),NULL,NULL) != 0)	//打开视频文件
	{
		LogStr("can not open input file!");
		return XFalse;
	}
	if(avformat_find_stream_info(pFormatCtx,NULL) < 0)		//检查视频流信息
	{
		LogStr("can not find stream information!");
		return XFalse;
	}

	for(i = 0;i < pFormatCtx->nb_streams;++ i)
	{
		if((pFormatCtx->streams[i]->codec->codec_type) == AVMEDIA_TYPE_VIDEO && videoStream < 0)
		{//在视频信息中查找视频流编解码信息
			videoStream = i;
		}
		if((pFormatCtx->streams[i]->codec->codec_type) == AVMEDIA_TYPE_AUDIO && audioStream < 0)
		{//在视频信息中查找音频流编解码信息
			audioStream = i;
		}
	}
	if(videoStream == -1 && audioStream == -1) return XFalse;	//文件中没有必要的流信息

	m_videoWidth = 0;			//获得视频的宽度
	m_videoHeight = 0;			//获得视频的高度
	if(videoStream != -1)
	{//如果文件中存在视频流
		pCodeCtx = pFormatCtx->streams[videoStream]->codec;		//得到视频流的编解码信息
		AVCodec *pCodec = avcodec_find_decoder(pCodeCtx->codec_id);		//在ffmpeg库中查找与视频编解码信息相匹配的编解码器

		if(pCodec == NULL)
		{//找不到相应的解码方式
			fprintf(stderr , "Unsupported codec!");
			return XFalse;
		}
		if(avcodec_open2(pCodeCtx ,pCodec,NULL) < 0)
		{//打开与视频编解码信息相匹配的编解码器
			LogStr("can not open video decoder!");
			return XFalse;
		}
		initVideoPictList();	//初始化视频帧队列
		m_videoWidth = pCodeCtx->width;			//获得视频的宽度
		m_videoHeight = pCodeCtx->height;			//获得视频的高度
	}

	if(audioStream != -1 && m_needVoice)
	{//如果存在音频流，下面开启音频解码
		aCodeCtx = pFormatCtx->streams[audioStream]->codec;	//得到音频流的编解码信息
		AVCodec *aCodec = avcodec_find_decoder(aCodeCtx->codec_id);	//在ffmpeg库中查找与音频编解码信息相匹配的编解码器
		if(aCodec == NULL)
		{//找不到音频解码器
			LogStr("can not find audio decoder information!");
		}else 
		if(avcodec_open2(aCodeCtx ,aCodec,NULL) < 0)
		{//打开与音频编解码信息相匹配的编解码器
			LogStr("can not open audio decoder!");
		}else
		{//根据查找到的音频编解码信息告诉SDL需播放的音频信息
			initAudioQueue();//初始化声音包队列
		}
		//由于音频格式的定义非常混乱,所以这里进行统一
		m_sreAudioFmt = AUDIO_S16;
		switch(aCodeCtx->sample_fmt)	//进行不同的定义转换
		{
		case AV_SAMPLE_FMT_U8:m_sreAudioFmt = AUDIO_S8;break;
		case AV_SAMPLE_FMT_S16:m_sreAudioFmt = AUDIO_S16LSB;break;
		//case AV_SAMPLE_FMT_S24:m_sreAudioFmt = AUDIO_S16LSB;break;	//不支持
		case AV_SAMPLE_FMT_S32:m_sreAudioFmt = AUDIO_S16LSB;break;	//不支持
		case AV_SAMPLE_FMT_FLT:m_sreAudioFmt = AUDIO_S16LSB;break;	//不支持
		}
		m_pSwrContext = swr_alloc();
		if(m_pSwrContext == NULL) return XFalse;
		if(aCodeCtx->channel_layout == 0) aCodeCtx->channel_layout = av_get_default_channel_layout(aCodeCtx->channels);	//如果这个参数没有设置的话，这里进行设置
		swr_alloc_set_opts(m_pSwrContext,av_get_default_channel_layout(XEG.getAudioChannelSum()),AV_SAMPLE_FMT_S16,XEG.getAudioSampleRate(),
			aCodeCtx->channel_layout,aCodeCtx->sample_fmt,aCodeCtx->sample_rate,0,NULL);
		if(swr_init(m_pSwrContext) < 0)
		{
			LogStr("swr_init() fail");
			return XFalse;
		}
		m_audioBufSize = 0; 
		m_audioBufIndex = 0; 
	}

	pFrame = av_frame_alloc();//分配一帧的内存空间，用于存放解码后的视频信息
	if(pFrame == NULL)
	{//如果分配内存失败，则这里提示
		LogStr("malloc Frame failed!");
		return XFalse;
	}
	pAudioFrame = av_frame_alloc();//分配一帧的内存空间，用于存放解码后的视频信息
	if(pAudioFrame == NULL)
	{//如果分配内存失败，则这里提示
		LogStr("malloc Frame failed!");
		return XFalse;
	}
	AVStream* curStr = nullptr;
	if(videoStream != -1) curStr = pFormatCtx->streams[videoStream];
	else curStr = pFormatCtx->streams[audioStream];

	if(curStr->avg_frame_rate.den <= 0)
		m_curFrameRate = (double)(curStr->time_base.den)/(double)(curStr->time_base.num);
	else
		m_curFrameRate = (double)(curStr->avg_frame_rate.num)/(double)(curStr->avg_frame_rate.den);

	if (curStr->nb_frames != 0) m_allFrameSum = curStr->nb_frames;
	else
	{
		if (curStr->duration < 0)
		{
			m_allFrameSum = pFormatCtx->duration * 0.000001 * m_curFrameRate;
		}
		else
		{
			m_allFrameSum = curStr->duration * (double)(curStr->time_base.num) /
				(double)(curStr->time_base.den) * m_curFrameRate;
		}
		//m_allFrameSum = curStr->duration;
	}

	//建立OpenGL贴图
	if (videoStream != -1)
	{
		XMem::XDELETE(m_movieTex);
		XMem::XDELETE(m_movieSprite);
		m_movieTex = XMem::createMem<XTexture>();
		if (m_movieTex == NULL) return XFalse;
		m_movieSprite = XMem::createMem<XSprite>();
		if (m_movieSprite == NULL) return XFalse;
		m_movieSprite->init(m_videoWidth, m_videoHeight, 0);
		XMem::XDELETE_ARRAY(m_curFrameData);
		switch (m_outColorMode)
		{
		case COLOR_RGB:
		case COLOR_BGR:
			m_movieTex->createTexture(m_videoWidth, m_videoHeight, m_outColorMode);
			m_curFrameData = XMem::createArrayMem<unsigned char>(m_videoWidth * m_videoHeight * 3);
			memset(m_curFrameData, 0, m_videoWidth * m_videoHeight * 3);
			break;
		case COLOR_RGBA:
		case COLOR_BGRA:
			m_movieTex->createTexture(m_videoWidth, m_videoHeight, m_outColorMode);
			m_curFrameData = XMem::createArrayMem<unsigned char>(m_videoWidth * (m_videoHeight << 2));
			memset(m_curFrameData, 0, m_videoWidth * (m_videoHeight << 2));
			break;
		case COLOR_GRAY:
			m_movieTex->createTexture(m_videoWidth, m_videoHeight, m_outColorMode);
			m_curFrameData = XMem::createArrayMem<unsigned char>(m_videoWidth * m_videoHeight);
			memset(m_curFrameData, 0, m_videoWidth * m_videoHeight);
			break;
		default:
			return XFalse;	//其他格式目前不支持
			break;
		}
		if (m_curFrameData == NULL) return XFalse;

		//为缓存分配内存空间,将图像数据转换成RGB24的形式
		int dataSize;
		AVPixelFormat avColorMode;
		switch (m_outColorMode)
		{
		case COLOR_RGB:
			dataSize = pCodeCtx->width * pCodeCtx->height * 3;
			avColorMode = AV_PIX_FMT_RGB24;
			break;
		case COLOR_BGR:
			dataSize = pCodeCtx->width * pCodeCtx->height * 3;
			avColorMode = AV_PIX_FMT_BGR24;
			break;
		case COLOR_RGBA:
			dataSize = pCodeCtx->width * (pCodeCtx->height << 2);
			avColorMode = AV_PIX_FMT_RGBA;
			break;
		case COLOR_BGRA:
			dataSize = pCodeCtx->width * (pCodeCtx->height << 2);
			avColorMode = AV_PIX_FMT_BGRA;
			break;
		case COLOR_GRAY:
			dataSize = pCodeCtx->width * pCodeCtx->height;
			avColorMode = AV_PIX_FMT_GRAY8;
			break;
		default:
			return XFalse;
			break;
		}
		m_pSwsContext = sws_getContext(pCodeCtx->width, pCodeCtx->height, pCodeCtx->pix_fmt,
			pCodeCtx->width, pCodeCtx->height, avColorMode, SWS_POINT, NULL, NULL, NULL);	//SWS_BICUBIC
		if (m_pSwsContext == NULL) return XFalse;
		//int numBytes = avpicture_get_size(PIX_FMT_RGB24,pCodeCtx->width,pCodeCtx->height);	//获取内存尺寸
		m_curBuffPoint = 0;
		for (i = 0; i < MAX_BUFF_DEEP; ++i)
		{
			m_bmp[i] = XMem::createMem<XVideoFrameData>();
			if (m_bmp[i] == NULL) return XFalse;

			m_bmp[i]->data = XMem::createArrayMem<unsigned char>(dataSize);
			if (m_bmp[i]->data == NULL) return XFalse;
			avpicture_fill(&m_framePic[i], m_bmp[i]->data, avColorMode,
				pCodeCtx->width, pCodeCtx->height);
			m_bmp[i]->pict = &m_framePic[i];
		}
	}

	m_isDecodeOver = XFalse;	//标记开始解码
    //建立线程
#ifdef XEE_OS_LINUX
	pthread_t tempP;
    if(pthread_create(&tempP, NULL, &decoderThread, (void*) this) != 0) 
	{
		LogStr("open thread error!");
	}
	if(audioStream != -1 &&  m_needVoice)
	{//如果存在音频流，这里开启音频播放流
	//	XCurSndCore.pauseMusic();
		XCurSndCore.setCallBack((SND_CALLBACK_FUN)audioCallBack,this);
	//	XCurSndCore.resumeMusic();
	}
    if(pthread_create(&tempP, NULL, &drawThread, (void*) this) != 0) 
	{
		LogStr("open thread error!");
	}
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,decoderThread,this,0,NULL) == 0) 
	{
		LogStr("open thread error!");
	}
	if(audioStream != -1 &&  m_needVoice)
	{//如果存在音频流，这里开启音频播放流
	//	XCurSndCore.pauseMusic();
		XCurSndCore.setCallBack((SND_CALLBACK_FUN)audioCallBack,this);
	//	XCurSndCore.resumeMusic();
	}
    if(CreateThread(0,0,drawThread,this,0,NULL) == 0) 
	{
		LogStr("open thread error!");
	}
#endif
#ifdef WITH_DEBUG_INFO
	LogNull("AllFrames:%u,FPS:%f", m_allFrameSum, getVideoFrameRate());
#endif
	//m_mutex = SDL_CreateMutex();

	m_isLoaded = XTrue;

	return XTrue;
}
////开始播放影片（这个函数应该是在不停的播解压出来的音频片断）
//void XMovieFfmpeg::waitingForBuffOver(void)
//{
//	while(true)
//	{//等待视频缓冲结束
//		if(!m_isBuffing)
//		{//缓冲满了之后打开声音设备
//			if(m_isQuit) break;	//当程序退出时，这里必须要退出，否则就是死循环，造成程序停止响应。
//			break;
//		}
//		mySleep(1);
//	}	
//}
//关闭影片播放
void XMovieFfmpeg::closeClip(void)
{
	if (!m_isLoaded) return;
	m_isQuit = XTrue;
	//m_audioQueue.cFlag = true;

	//mySleep(20);	//等待线程退出
	while (!m_isThreadDecoderExit || !m_isThreadDrawExit)
	{
		Sleep(1);
	}
	XCurSndCore.setCallBack(NULL, NULL);	//播放完成之后取消声音
	if (pFrame != NULL) av_frame_free(&pFrame);
	if (pAudioFrame != NULL) av_frame_free(&pAudioFrame);
	if (pCodeCtx != NULL) avcodec_close(pCodeCtx);
	if (audioStream != -1 && m_needVoice) avcodec_close(aCodeCtx);
	avformat_close_input(&pFormatCtx);
	releaseVideoPictList();
	releaseAudioQueue();

	XMem::XDELETE(m_movieTex);
	XMem::XDELETE(m_movieSprite);
	XMem::XDELETE_ARRAY(m_curFrameData);

	XMem::XDELETE_ARRAY(m_audioBuf);
	//if(glIsTexture(m_texture))
	//{
	//	printf("delete texture:%d\n",m_texture);
	//	glDeleteTextures(1,&m_texture);
	//}
	//SDL_DestroyMutex(m_mutex);
//#ifdef WITH_DEBUG_INFO
//	LogStr("69");
//#endif
	XMem::XDELETE(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//	LogStr("70");
//#endif
	XMem::XDELETE(m_videoQueue.mutex);
	sws_freeContext(m_pSwsContext);
	if (audioStream != -1 && m_needVoice) swr_free(&m_pSwrContext);
	m_isLoaded = XFalse;
}
void XMovieFfmpeg::gotoFrameIndex(int index, bool isForce)
{
	if((!isForce && index == m_curFrameNumber) || index < 0 || index > m_allFrameSum) return;
	m_gotoMutex.Lock();
	m_gotoMutex1.Lock();

	if (av_seek_frame(pFormatCtx, -1, index, AVSEEK_FLAG_BACKWARD) < 0)	//这个方法可能会造成不能读取到数
		av_seek_frame(pFormatCtx, -1, index, AVSEEK_FLAG_ANY);
	//if(index == m_curFrameNumber)
	//	av_seek_frame(pFormatCtx, -1, index, AVSEEK_FLAG_ANY);
	//else
	//{
	//	if(av_seek_frame(pFormatCtx, -1, index, AVSEEK_FLAG_BACKWARD) < 0)
	//		av_seek_frame(pFormatCtx, -1, index, AVSEEK_FLAG_ANY);
	//}

	if (videoStream >= 0)
	{
		m_curFrameNumber = index - m_videoQueue.nb_pict;	//让缓冲中的数据快速播放完成
		m_curPlayedTime = m_curFrameNumber * 1000.0f / m_curFrameRate;	//重新设置时间
	}
	m_gotoMutex.Unlock();
	m_gotoMutex1.Unlock();
}
//把YUV格式的视频帧显示到窗口
void XMovieFfmpeg::getPixelData()
{
	gLock(m_videoQueue.mutex);
	unsigned char * p = getFormVideoPicList();
	if (p == NULL)
	{
		gUnlock(m_videoQueue.mutex);
		return;
	}
	m_mutex.Lock();
	//将数据拷贝出来
	switch (m_outColorMode)
	{
	case COLOR_RGB:
	case COLOR_BGR:
		memcpy(m_curFrameData, p, m_videoWidth * m_videoHeight * 3);
		break;
	case COLOR_RGBA:
	case COLOR_BGRA:
		memcpy(m_curFrameData, p, m_videoWidth * (m_videoHeight << 2));
		break;
	case COLOR_GRAY:
		memcpy(m_curFrameData, p, m_videoWidth * m_videoHeight);
		break;
	}
	m_mutex.Unlock();
	gUnlock(m_videoQueue.mutex);
	m_isNewFrame = true;
	++m_curFrameNumber;
}
XBool XMovieFfmpeg::getFirstPixelData()
{
	gLock(m_videoQueue.mutex);
	unsigned char * p = getAVideoFrame();	//从视频队列中取出一帧，不改变队列的数据
	if(p == NULL) 
	{
		gUnlock(m_videoQueue.mutex);
		return false;
	}
	m_mutex.Lock();
	//将数据拷贝出来
	switch(m_outColorMode)
	{
	case COLOR_RGB:
	case COLOR_BGR:
		memcpy(m_curFrameData,p,m_videoWidth * m_videoHeight * 3);
		break;
	case COLOR_RGBA:
	case COLOR_BGRA:
		memcpy(m_curFrameData,p,m_videoWidth * (m_videoHeight << 2));
		break;
	case COLOR_GRAY:
		memcpy(m_curFrameData,p,m_videoWidth * m_videoHeight);
		break;
	}
	m_mutex.Unlock();
	gUnlock(m_videoQueue.mutex);
	m_isNewFrame = true;
	return true;
}
void XMovieFfmpeg::releaseVideoPictList()
{
	for(int i = 0;i < MAX_BUFF_DEEP;++ i)
	{
		if(m_bmp[i] != NULL)
			XMem::XDELETE_ARRAY(m_bmp[i]->data);
		XMem::XDELETE(m_bmp[i]);
	}
} 
void XMovieFfmpeg::putIntoVideoPicList()
{
	int isFinished = 0;
	while (packet.size > 0)
	{
		int ret = avcodec_decode_video2(pCodeCtx, pFrame, &isFinished, &packet);
		if (ret < 0)
		{//视频解码失败
			av_free_packet(&packet);
			return;
		}
		packet.data += ret;
		packet.size -= ret;
		if (isFinished == 0) continue;
		//如果解码成功		
	//	SDL_LockMutex(m_mutex);
		gLock(m_videoQueue.mutex);
		//将视频数据格式转换成RGB24格式
		imgConvert(m_bmp[m_curBuffPoint]->pict, (AVPicture *)pFrame, pCodeCtx->height);
		//将视频数据推入队列中
		m_bmp[m_curBuffPoint]->next = NULL;
		if (m_videoQueue.last_pict == NULL) m_videoQueue.first_pict = m_bmp[m_curBuffPoint];//如果是空列
		else m_videoQueue.last_pict->next = m_bmp[m_curBuffPoint];//否则添加一帧

		m_videoQueue.last_pict = m_bmp[m_curBuffPoint];
		++m_videoQueue.nb_pict;

		++m_curBuffPoint;
		if (m_curBuffPoint >= MAX_BUFF_DEEP) m_curBuffPoint = 0;	//循环插入
	//	SDL_UnlockMutex(m_mutex);
		gUnlock(m_videoQueue.mutex);
#ifdef DEBUG_MODE
		++m_vfSum;
#endif
	}
	av_free_packet(&packet);
}
//在队列里取出视频帧的像素数据
unsigned char* XMovieFfmpeg::getFormVideoPicList()
{
	XVideoFrameData *vp = NULL;
	if(m_videoQueue.nb_pict <= 0) return NULL;
	vp = m_videoQueue.first_pict;
	if(vp != NULL)
	{
		m_videoQueue.first_pict = vp->next;
		if(m_videoQueue.first_pict == NULL)
		{
			m_videoQueue.last_pict = NULL;
#ifdef WITH_DEBUG_INFO
			LogStr("buffer no data!");
#endif
		}
		--m_videoQueue.nb_pict;
	}
	if(vp != NULL) return vp->data;
	else return NULL;
}
//把音频包pkt放到队列q里
int XMovieFfmpeg::putIntoPacketQueue()
{
	if(av_dup_packet(&packet) < 0)
	{
		av_free_packet(&packet); 
		LogStr("audio decode error!");
		return -1;	//音频解码
	}

	AVPacketList* pkt_temp = XMem::createMem<AVPacketList>();
//	++ createNumber;
//	printf("%d,%d\n",createNumber,m_audioQueue.nb_packets);
	if(pkt_temp == NULL) return -1;	//内存分配失败

	pkt_temp->pkt = packet;
	pkt_temp->next = NULL;

//#ifdef WITH_DEBUG_INFO
//	LogStr("51");
//#endif
	gLock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//	LogStr("52");
//#endif
	if(m_audioQueue.last_pkt == NULL) m_audioQueue.first_pkt = pkt_temp;	//如果音频队列为空
	else m_audioQueue.last_pkt->next = pkt_temp;		
	m_audioQueue.last_pkt = pkt_temp;
	++ m_audioQueue.nb_packets;
	m_audioQueue.size += pkt_temp->pkt.size;
	//m_audioQueue.cFlag = true;
//#ifdef WITH_DEBUG_INFO
//	LogStr("53");
//#endif
	gUnlock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//	LogStr("54");
//#endif
	return 0;
}
//释放所有缓存的视频数据
void XMovieFfmpeg::releaseAudioQueue()
{
	AVPacketList *pkt1; 
	while(true)
	{ 
		pkt1 = m_audioQueue.first_pkt; 
		if(pkt1 == NULL) break;  
		m_audioQueue.first_pkt = pkt1->next; 
		if(m_audioQueue.first_pkt == NULL) m_audioQueue.last_pkt = NULL;	//音频队列中已经没有数据 
		-- m_audioQueue.nb_packets; 
		m_audioQueue.size -= pkt1->pkt.size; 
		av_free_packet(&(pkt1->pkt));
		XMem::XDELETE(pkt1); 
	//	-- createNumber;
	//	printf("%d,%d\n",createNumber,m_audioQueue.nb_packets);
	}
}
void XMovieFfmpeg::replay()
{
	m_gotoMutex.Lock();	//防止時間偷跑
	m_gotoMutex1.Lock();	//防止時間偷跑
	//这里需要清空序列
//	printf("All times:%d\n",m_curPlayedTime);
//	printf("Loop!\n");
	gotoFrame(0.0f, true);
	m_isDecodeOver = XFalse;
	m_isEnd = XFalse;
	//这里需要清空缓存
	clearAudioQueue();
	clearVideoQueue();
	m_curFrameNumber = 0;
	m_curPlayedTime = 0.0f;
	m_audioBufSize = 0;
	m_audioBufIndex = 0;
#ifdef DEBUG_MODE
	m_vpSum = 0;
	m_vfSum = 0;
	LogStr("replay()");
#endif
	m_gotoMutex.Unlock();
	m_gotoMutex1.Unlock();
}
void XMovieFfmpeg::clearAudioQueue()	//清空音频队列
{
//#ifdef WITH_DEBUG_INFO
//	LogStr("71");
//#endif
	gLock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//	LogStr("72");
//#endif
	AVPacketList *pkt1 = NULL;
	while(true)
	{ 
		pkt1 = m_audioQueue.first_pkt; 
		if(pkt1 == NULL) break;  
		m_audioQueue.first_pkt = pkt1->next; 
		if(m_audioQueue.first_pkt == NULL) m_audioQueue.last_pkt = NULL;	//音频队列中已经没有数据 
		-- m_audioQueue.nb_packets; 
		m_audioQueue.size -= pkt1->pkt.size; 
		av_free_packet(&(pkt1->pkt));
		XMem::XDELETE(pkt1); 
	}
//#ifdef WITH_DEBUG_INFO
//	LogStr("73");
//#endif
	gUnlock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//	LogStr("74");
//#endif
}
void XMovieFfmpeg::getData(unsigned char * p)
{
	m_mutex.Lock();
	switch (m_outColorMode)
	{
	case COLOR_RGB:
	case COLOR_BGR:
		memcpy(p, m_curFrameData, m_videoWidth * m_videoHeight * 3);
		break;
	case COLOR_RGBA:
	case COLOR_BGRA:
		memcpy(p, m_curFrameData, m_videoWidth * (m_videoHeight << 2));
		break;
	case COLOR_GRAY:
		memcpy(p, m_curFrameData, m_videoWidth * m_videoHeight);
		break;
	default:	//其他格式暂时不支持
		break;
	}
	m_mutex.Unlock();
}
XBool XMovieFfmpeg::updateFrame()
{
	if(!m_isGetFirstFrame && getFirstPixelData())
		m_isGetFirstFrame = true;
	if(!m_isNewFrame) return XFalse;
	m_isNewFrame = XFalse;
	if(m_movieTex != NULL) 
	{
		m_mutex.Lock();
		m_movieTex->updateTexture(m_curFrameData);
		m_mutex.Unlock();
		if(!m_isTexInit) m_isTexInit = true;
	}
	return XTrue;
}
#if !WITH_INLINE_FILE
#include "XMovieFfmpeg.inl"
#endif
}