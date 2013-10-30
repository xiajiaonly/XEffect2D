//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMovie.h"
#include "SDL_mixer.h"
#include "math.h"
#include "XBasicSDL.h"

PacketQueue _XMovie::audioq;
bool _XMovie::m_isStop = false;
bool _XMovie::m_isQuit = false;

_XMovie::_XMovie(void)
:videoStream(-1)
,audioStream(-1)
,m_isBuffing(true)
,m_isLoop(false)
,m_videoWidth(0)
,m_videoHeight(0)
,m_nowFrameNumber(0)
,m_needVoice(1)
,m_texture(NULL)
,m_isLoaded(0)
{
	videofilename[0] = '\0';
	videopictlist.nb_pict = 0;
	m_isEnd = false;
	m_isQuit = false;
	m_isStop = false;
}

_XMovie::~_XMovie(void)
{
}

//视频显示、视频同步线程
#ifdef OS_LINUX
void * drawThread(void *arg)
#endif
#ifdef OS_WINDOWS
DWORD WINAPI drawThread(void *arg)
#endif
{
	_XMovie *pthis = (_XMovie *)arg;
	float refreshtime = 1000.0f/(* pthis).m_nowFrameRate;//影片显示一帧的时间，单位：毫秒
	float temptime = refreshtime;

	//第一播放时需要等缓冲满
	pthis->waitingForBuffOver();

	int startTime = SDL_GetTicks();		//记录开始播放的时间
	pthis->m_nowPlayedTime = 0;			//播放的时间通过这个来计算

	while(1)
	{	
		if(pthis->m_isQuit)
		{
		//	SDL_CloseAudio();	//关闭声音设备并退出
			Mix_HookMusic(NULL,NULL);	//播放完成之后取消声音
			break;
		}
		if(pthis->m_isEnd)
		{//如果播放已经结束
			if(!pthis->m_isLoop)
			{//如果不需要循环播放
			//	SDL_CloseAudio();	//关闭声音设备并退出
				Mix_HookMusic(NULL,NULL);
				printf("NO Loop!\n");
				break;
			}
			printf("All times:%d\n",pthis->m_nowPlayedTime);
			pthis->m_isEnd = false;
			pthis->m_nowFrameNumber = 0;
			pthis->m_nowPlayedTime = 0;
			startTime = SDL_GetTicks();	
			printf("Loop!\n");
		}
		if(pthis->m_isStop)
		{//停止状态
			mySleep(1);
			startTime = SDL_GetTicks();
			continue;
		}
		if(pthis->videopictlist.nb_pict > 0)
		{//视频队列中有数据
			pthis->m_nowPlayedTime += SDL_GetTicks() - startTime;
			startTime = SDL_GetTicks();
		//	playedTime = SDL_GetTicks() - startTime;
			if(pthis->m_nowPlayedTime>= pthis->m_nowFrameNumber * temptime)
			{//如果达到时间点则开始播放
				pthis->draw();
				if(pthis->m_nowFrameNumber >= pthis->m_allFrameSum)
				{//如果时间越界，则播放完成
					pthis->m_isEnd = true;
					printf("Time play over!\n");
				}
			}else
			{//否则则等到时间点再播放
				mySleep(pthis->m_nowFrameNumber * temptime - pthis->m_nowPlayedTime);
			}
		}else
		{
			printf("-->Some Wrong!\n");
			pthis->m_isStop = true;
			mySleep(5000);
			pthis->m_isStop = false;
			startTime = SDL_GetTicks();
		}
		mySleep(1);
	}
//	_endthread();
#ifdef OS_WINDOWS
	return 1;
#endif
}

//视频解码线程，在DrawThread线程不显示视频的时候把视频帧解码到队列，把音频包放到队列
#ifdef OS_WINDOWS
DWORD WINAPI decoderThread(void *arg)
#endif
#ifdef OS_LINUX
void * decoderThread(void *arg)
#endif
{
	_XMovie *pthis = (_XMovie *)arg;
	//AVPicture pict;
	int bufernum = (int)(2.0 * pthis->m_nowFrameRate); //设置缓冲区大小为影片2秒播放的帧数
	int frameFinished;		//解码动作是否完成
	if(bufernum >= MAX_BUFF_DEEP) bufernum = MAX_BUFF_DEEP;

	while(1)
	{
		if(pthis ->m_isQuit) 
		{
			break;
		}

		if((pthis->videopictlist.nb_pict) < bufernum)//刚开始缓冲2秒
		{//如果没有缓冲满则进行缓冲
			if(av_read_frame(pthis->pFormatCtx , &pthis->packet) >= 0)
			{//如果读取帧成功
				if(pthis->packet.stream_index == pthis->videoStream)
				{			
					avcodec_decode_video(pthis->pCodeCtx , pthis->pFrame , &frameFinished , pthis->packet.data , pthis->packet.size);//视频解码
					if(frameFinished)
					{//如果解码成功		
						SDL_LockMutex(pthis->m_mutex);
						//使用RGB24的方式保存帧显示数据
						img_convert(pthis->m_bmp[pthis->m_nowBuffPoint]->pict, PIX_FMT_RGB24,(AVPicture *)pthis->pFrame, pthis->pCodeCtx->pix_fmt,pthis->pCodeCtx->width,pthis->pCodeCtx->height); 
						pthis->putIntoVideoPicList(&pthis->videopictlist ,pthis->m_bmp[pthis->m_nowBuffPoint]);
						++ pthis->m_nowBuffPoint;
						if(pthis->m_nowBuffPoint >= MAX_BUFF_DEEP) pthis->m_nowBuffPoint = 0;
						SDL_UnlockMutex(pthis->m_mutex);
					}
				}
				if(pthis->packet.stream_index == pthis->audioStream &&  pthis->m_needVoice != 0)
				{//将取得的音频包放进队列
					pthis->putIntoPacketQueue(&pthis->audioq , &pthis->packet);
				}else
				{
					av_free_packet(&pthis->packet);
				}
			}else	//如果提取帧失败，则默认已经播放完 
			if(pthis->m_isLoop)//如果视频播放完毕，若设置循环播放则循环播放
			{
				av_free(pthis->pFormatCtx);
				if(av_open_input_file(&pthis->pFormatCtx ,pthis->videofilename,NULL,0,NULL) != 0)
				{//重新打开播放文件
#ifdef OS_WINDOWS
					return 0;
#endif
#ifdef OS_LINUX
					break;
#endif
				}
			}else
			{//视频解码完毕，退出解码线程
				break;
			}
		}else//如果显示缓存满了 
		if(pthis->m_isBuffing)	//第一次播放的时候一定要等缓冲满
		{//缓冲完毕，设置bplay , 进行播放
			pthis->m_isBuffing = false;
		}
		mySleep(1);
	}
//	_endthread();
#ifdef OS_WINDOWS
	return 1;
#endif
}


//在音频队列里取出音频包
int getFromPacketQueue(PacketQueue *q, AVPacket *pkt, int block)
{ 
	AVPacketList *pkt1; 
	int ret; 

	SDL_LockMutex(q->mutex); 
	while(1)
	{ 
		if(_XMovie::m_isStop)
		{//暂停的话这里停止播放声音
			SDL_UnlockMutex(q->mutex); 
			return -1;
		}

		pkt1 = q->first_pkt; 
		if (pkt1) 
		{ 
			q->first_pkt = pkt1->next; 
			if (!q->first_pkt) 
				q->last_pkt = NULL; 
			--(q->nb_packets); 
			q->size -= pkt1->pkt.size; 
			*pkt = pkt1->pkt; 
			av_free(pkt1); 
			ret = 1; 
			break; 
		}else 
		if(!block)
		{ 
			ret = 0; 
			break; 
		}else 
		{ 
			SDL_CondWait(q->cond, q->mutex); 
		} 
		mySleep(1);
	} 
	SDL_UnlockMutex(q->mutex); 
	return ret; 
} 

//把音频包解码成音频数据
int audioDecodeFrame(AVCodecContext *aCodecCtx, uint8_t *audio_buf, int buf_size)
{ 
	static AVPacket pkt; 
	static uint8_t *audio_pkt_data = NULL; 
	static int audio_pkt_size = 0; 

	int len1 = 0;
	int data_size = 0; 
	while(1)
	{ 
		if(_XMovie::m_isQuit)
		{
			return -1;
		}
		while(audio_pkt_size > 0) 
		{ 
			if(_XMovie::m_isQuit)
			{
				return -1;
			}
			data_size = buf_size; 
			//从audio_pkt_data地址中读取audio_pkt_size的数据并解压到audio_buf这个地址中长度为data_size
			len1 = avcodec_decode_audio(aCodecCtx, (int16_t *)audio_buf, &data_size, audio_pkt_data, audio_pkt_size); //声音解码
			if(len1 < 0) 
			{//解码发生故障 
				audio_pkt_size = 0; 
				break; 
			} 
			audio_pkt_data += len1;		//如果一次音频解码没有完成则重复进行直到完成为止
			audio_pkt_size -= len1; 
			if(data_size <= 0) 
			{     
				continue; 
			} 
			return data_size; 
		} 
		if(pkt.data) 
			av_free_packet(&pkt); 
 
		if(_XMovie::m_isStop)
		{//暂停的话这里停止播放声音
			return -1;
		}
		if(getFromPacketQueue(&_XMovie::audioq, &pkt, 1) < 0)
		{//如果音频中没有数据，则返回 
			return -1; 
		} 
		audio_pkt_data = pkt.data; 
		audio_pkt_size = pkt.size; 
	} 
} 

//声音回调函数，解码音频数据,将音频数据推到缓存供SDL播放
void audioCallBack(void *userdata , Uint8 *stream , int len)
{
	AVCodecContext *aCodecCtx = (AVCodecContext *)userdata; 
	int len1, audio_size; 

//	static uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; 
	static uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; 
	static unsigned int audio_buf_size = 0; 
	static unsigned int audio_buf_index = 0; 

	while(len > 0)
	{//当有声音需要播放时 	
		if(audio_buf_index >= audio_buf_size)
		{//当解压的音频全部播放完成时才解压后面的音频
			if(_XMovie::m_isQuit)
			{
				return; 
			}
			audio_size = audioDecodeFrame(aCodecCtx, audio_buf, sizeof(audio_buf));//声音解码函数 
			if(audio_size < 0) 
			{//如果音频解码没有数据.则清空内存空间
				audio_buf_size = 1024; 
				memset(audio_buf, 0, audio_buf_size); 
			}else
			{//记录解压出来的音频数据量
				audio_buf_size = audio_size; 
			} 
			audio_buf_index = 0; 
		} 
		len1 = audio_buf_size - audio_buf_index;	//记录音频的长度
		if(len1 > len) 
			len1 = len;

		//下面需要对音频进行转码(未能完成)
		//if(XEE_audioFrequency != aCodecCtx->sample_rate
		////	|| aCodecCtx->sample_fmt != XEE_audioFormat
		//	|| aCodecCtx->channels != XEE_audioChannel)
		//{
		//	SDL_AudioCVT  wav_cvt;
		//	int ret = SDL_BuildAudioCVT(&wav_cvt,
		//	//	aCodecCtx->sample_fmt, aCodecCtx->channels, aCodecCtx->sample_rate,
		//		XEE_audioFormat,aCodecCtx->channels,aCodecCtx->sample_rate,
		//		XEE_audioFormat,XEE_audioChannel,XEE_audioFrequency);
		//	if(ret == -1)
		//	{
		//		fprintf(stderr, "Couldn't build converter!\n");
		//	}
		//	wav_cvt.buf = (Uint8 *)malloc(len1 * wav_cvt.len_mult);
		//	wav_cvt.len = len1;
		//	memcpy(wav_cvt.buf,audio_buf + audio_buf_index,wav_cvt.len);
		//	SDL_ConvertAudio(&wav_cvt);
		//	SDL_MixAudio(stream,wav_cvt.buf, wav_cvt.len_cvt,128);
		//	free(wav_cvt.buf);
		//}else
		//{
		//	SDL_MixAudio(stream, (uint8_t *)audio_buf + audio_buf_index, len1,128);
		//}
		SDL_MixAudio(stream, (uint8_t *)audio_buf + audio_buf_index, len1,128);
		len -= len1; 
		stream += len1; 
		audio_buf_index += len1; 
	//	mySleep(1);
	}
}

//打开影片 ，初始化SDL , ffmpeg库等
int _XMovie::load(const char *filename)
{
	if(m_isLoaded != 0) return 0;
	videoStream = -1;
	audioStream = -1;
	m_isBuffing = true;
	m_isStop = true;
//	m_isLoop = FALSE;
	m_videoWidth = 0;
	m_videoHeight = 0;
	m_nowFrameNumber = 0;
	videopictlist.nb_pict = 0;
	m_isEnd = false;
	m_isQuit = false;
	m_nowFrame = NULL;
//	m_needVoice = 1;

	int i = 0;
	strcpy(videofilename,filename);		//记录视频文件名
	av_register_all();					//初始化ffmpeg库
	if(av_open_input_file(&pFormatCtx ,videofilename , NULL , 0 , NULL) != 0)	//打开视频文件
	{
		printf("can not open input file!\n");
		return 0;
	}
	if(av_find_stream_info(pFormatCtx) < 0)		//检查视频流信息
	{
		printf("can not find stream information!\n");
		return 0;
	}

	for(i = 0;i < pFormatCtx->nb_streams;++ i)
	{
		if((pFormatCtx->streams[i]->codec->codec_type) == CODEC_TYPE_VIDEO && videoStream < 0)
		{//在视频信息中查找视频流编解码信息
			videoStream = i;
		}
		if((pFormatCtx->streams[i]->codec->codec_type) == CODEC_TYPE_AUDIO && audioStream < 0)
		{//在视频信息中查找音频流编解码信息
			audioStream = i;
		}
	}

	m_videoWidth = 0;				//获得视频的宽度
	m_videoHeight = 0;			//获得视频的高度
	if(videoStream != -1)
	{//如果文件中存在视频流
		pCodeCtx = pFormatCtx->streams[videoStream]->codec;		//得到视频流的编解码信息
		AVCodec *pCodec = avcodec_find_decoder(pCodeCtx->codec_id);		//在ffmpeg库中查找与视频编解码信息相匹配的编解码器

		if(pCodec == NULL)
		{//找不到相应的解码方式
			fprintf(stderr , "Unsupported codec!\n");
			return 0;
		}
		if(avcodec_open(pCodeCtx ,pCodec) < 0)
		{//打开与视频编解码信息相匹配的编解码器
			printf("can not open video decoder!\n");
			return 0;
		}
		initVideoPictList(&videopictlist);	//初始化视频帧队列
		m_videoWidth = pCodeCtx->width;			//获得视频的宽度
		m_videoHeight = pCodeCtx->height;			//获得视频的高度
	}

	if(audioStream != -1 && m_needVoice != 0)
	{//如果存在音频流，下面开启音频解码
		aCodeCtx = pFormatCtx->streams[audioStream]->codec;	//得到音频流的编解码信息
		AVCodec *aCodec = avcodec_find_decoder(aCodeCtx->codec_id);	//在ffmpeg库中查找与音频编解码信息相匹配的编解码器
		if(aCodec == NULL)
		{//找不到音频解码器
			printf("can not find audio decoder information!\n");
		}else 
		if(avcodec_open(aCodeCtx ,aCodec) < 0)
		{//打开与音频编解码信息相匹配的编解码器
			printf("can not open audio decoder!\n");
		}
		else
		{//根据查找到的音频编解码信息告诉SDL需播放的音频信息
		/*	wanted_spec.freq = aCodeCtx->sample_rate;	//音频的频率
			wanted_spec.format = AUDIO_S16SYS;			//音频格式
			wanted_spec.channels = aCodeCtx->channels;	//音频声道数
			wanted_spec.silence = 0;					//是否静音
			wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;//音频缓冲区大小
			wanted_spec.callback = audioCallBack;		//音频解码回调函数
			wanted_spec.userdata = aCodeCtx;			//回调函数的音频数据来源*/

			//注意：这里存在bug，由于视频的音频需求与实际的音频需求存在差距，需要修改
			//Mix_CloseAudio();
			//const int    TMP_FREQ = aCodeCtx->sample_rate;
			//const Uint16 TMP_FORMAT = AUDIO_S16SYS;
			//const int    TMP_CHAN = aCodeCtx->channels;
			//const int    TMP_CHUNK_SIZE = SDL_AUDIO_BUFFER_SIZE;
			//Mix_OpenAudio(TMP_FREQ,TMP_FORMAT,TMP_CHAN,TMP_CHUNK_SIZE);
			//Mix_AllocateChannels(128);

			initPacketQueue(&audioq);//初始化声音包队列
		}
	}

	pFrame = avcodec_alloc_frame();//分配一帧的内存空间，用于存放解码后的视频信息
	if(pFrame == NULL)
	{//如果分配内存失败，则这里提示
		printf("malloc Frame failed!\n");
		return 0;
	}

	m_nowFrameRate = (double)(pFormatCtx->streams[videoStream]->time_base.den)/(double)(pFormatCtx->streams[videoStream]->time_base.num);
	m_allFrameSum = pFormatCtx->streams[videoStream]->duration;
	//建立OpenGL贴图
	if(m_texture != NULL)
	{
		glDeleteTextures(1,&m_texture);
	}
	glGenTextures(1,&m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//这里修改建立一个符合2的n次方的贴图
	int wR = (int)powf(2.0, ceilf(logf((float)m_videoWidth)/logf(2.0f)));
	int hR = (int)powf(2.0, ceilf(logf((float)m_videoHeight)/logf(2.0f)));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,wR,hR, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,m_videoWidth,m_videoHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    //建立线程
#ifdef OS_LINUX
	pthread_t tempP;
    if(pthread_create(&tempP, NULL, &decoderThread, (void*) this) != 0) 
	{
		printf("open thread error!\n");
	}
    if(pthread_create(&tempP, NULL, &drawThread, (void*) this) != 0) 
	{
		printf("open thread error!\n");
	}
#endif
#ifdef OS_WINDOWS
    if(CreateThread(0,0,decoderThread,this,0,NULL) == 0) 
	{
		printf("open thread error!\n");
	}
    if(CreateThread(0,0,drawThread,this,0,NULL) == 0) 
	{
		printf("open thread error!\n");
	}
#endif

//	_beginthread(decoderThread , 0 , this);	//开启解码的线程
//	_beginthread(drawThread , 0 , this);	//开启绘图的线程

	int numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodeCtx->width,pCodeCtx->height);	//获取内存尺寸
	m_nowBuffPoint = 0;
	for(int i = 0;i < MAX_BUFF_DEEP;++ i)
	{
		m_bmp[i] = (VideoPict*)av_malloc(sizeof(VideoPict));
		if(m_bmp[i] == NULL) return 0;

		m_frameDataBuff[i] = (unsigned char *)av_malloc(numBytes*sizeof(unsigned char));
		avpicture_fill(&m_framePic[i],m_frameDataBuff[i], PIX_FMT_RGB24,pCodeCtx->width,pCodeCtx->height);

		m_bmp[i]->pict = &m_framePic[i];
		m_bmp[i]->data = m_frameDataBuff[i];
	}

	printf("AllFrames:%d,FPS:%f\n",(* this).m_allFrameSum,(* this).getVideoFrameRate());
	m_mutex = SDL_CreateMutex();

	m_isLoaded = 1;

	return 1;
}

void _XMovie::init(SDL_Surface *temp)
{
	m_screen = temp;
}

//取得影片的宽度
int _XMovie::getClipWide(void)
{
	return m_videoWidth;
}

//取得影片的高度
int _XMovie::getClipHigh(void)
{
	return m_videoHeight;
}

//设置影片是否循环播放，TRUE循环播放，FALSE则否
void _XMovie::setLoop(bool isLoop)
{
	m_isLoop = isLoop;
}

//开始播放影片（这个函数应该是在不停的播解压出来的音频片断）
void _XMovie::waitingForBuffOver(void)
{
	while(1)
	{//等待视频缓冲结束
		if(!m_isBuffing)
		{//缓冲满了之后打开声音设备
			if(audioStream != -1 &&  m_needVoice == 1)
			{//如果存在音频流
			/*	SDL_AudioSpec spec;
				if(SDL_OpenAudio(&wanted_spec , &spec) < 0)
				{//打开声音设备
					fprintf(stderr ,"SDL_OpenAudio: %s\n" , SDL_GetError());
				}
				SDL_PauseAudio(0); */
				Mix_HookMusic(audioCallBack,aCodeCtx);
			}
			break;
		}
		mySleep(1);
	}	
}

double _XMovie::getCurrentPlayingTime()
{//返回影片当前播放时间，单位：毫秒
	return m_nowFrameNumber*(1000.0/m_nowFrameRate);
}

int _XMovie::getAllTime(void)
{//返回影片总时间，单位：毫秒
	return ((double)pFormatCtx->duration * 0.001f);
}

float _XMovie::getVideoFrameRate()
{//返回影片的播放帧率
	return m_nowFrameRate;
}

bool _XMovie::getUpdateFlag(void)
{//视频缓冲队列不空返回TRUE，否则返回FALSE
	if(videopictlist.nb_pict > 0)
	{
		return true;
	}else
	{
		return false;
	}
}

//暂停或继续播放影片
void _XMovie::setPauseOrPlay(void)
{	
	m_isStop = !m_isStop;
}

//关闭影片播放
void _XMovie::closeClip(void)
{
	if(m_isLoaded == 0) return;
	m_isQuit = true;
	mySleep(20);
	av_free(pFrame);
	avcodec_close(pCodeCtx);
	av_close_input_file(pFormatCtx);
	releaseVideoPictList();
	releasePacketQueue();
	if(m_texture != NULL)
	{
		glDeleteTextures(1,&m_texture);
	}
	m_isLoaded = 0;
}

void _XMovie::gotoFrame(float temp)	//跳到当前视频的某一帧
{
	if(temp < 0) temp = 0;
	if(temp > 1) temp = 1;

	int armTime = temp * m_allFrameSum;
	int rt = av_seek_frame(pFormatCtx,videoStream,armTime,AVSEEK_FLAG_BACKWARD);
	printf("%d,%d,%d,%d\n",rt,m_allFrameSum,armTime,pFormatCtx->timestamp);
	m_nowFrameNumber = temp * m_allFrameSum - videopictlist.nb_pict;	//让缓冲中的数据快速播放完成
	m_nowPlayedTime = m_nowFrameNumber * 1000.0f / m_nowFrameRate;	//重新设置时间
}

//把YUV格式的视频帧显示到窗口
void _XMovie::draw(void)
{  
	SDL_LockMutex(m_mutex);
	m_nowFrame = getFormVideoPicList(&videopictlist);
	SDL_UnlockMutex(m_mutex);
	if(m_nowFrame == NULL) return;

	++ m_nowFrameNumber;
}

GLuint * _XMovie::getTexture()
{
	if(m_nowFrame != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,m_texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
			m_videoWidth,m_videoHeight, GL_RGB,//GL_LUMINANCE,	
			GL_UNSIGNED_BYTE,m_nowFrame);
		glDisable( GL_TEXTURE_2D );
		m_nowFrame = NULL;
	}
	return &m_texture;
}

//初始化视频队列
void _XMovie::initVideoPictList(VideoPictList *vpl)
{
	memset(vpl,0,sizeof(VideoPictList));
	vpl->mutex = SDL_CreateMutex();
}

void _XMovie::releaseVideoPictList()
{
	for(int i = 0;i < MAX_BUFF_DEEP;++ i)
	{
	//	SDL_FreeYUVOverlay(m_bmp[i]->bmp);
		av_free(m_bmp[i]->data);
		av_free(m_bmp[i]);
	}
}

//把解码后的视频帧放到队列里
void _XMovie::putIntoVideoPicList(VideoPictList *vpl,VideoPict *picTemp)
{
	picTemp->next = NULL;
	SDL_LockMutex(vpl->mutex);
	if(vpl->last_pict == NULL)
	{//如果是空列
		vpl->first_pict = picTemp;
	}else
	{//否则添加一帧
		vpl->last_pict->next = picTemp;
	}
	vpl->last_pict = picTemp;
	++ vpl->nb_pict;
	SDL_UnlockMutex(vpl->mutex);
}

//在队列里取出视频帧
unsigned char* _XMovie::getFormVideoPicList(VideoPictList *vpl)
{
	VideoPict *vp = NULL;
	if(vpl == NULL)
	{//如果影片帧序列为空则,直接返回
		return NULL;
	}
	if(vpl->nb_pict <= 0) return NULL;
	SDL_LockMutex(vpl->mutex);
	vp = vpl->first_pict;
	if(vp)
	{
		vpl->first_pict = vp->next;
		if(vpl->first_pict == NULL)
		{
			vpl->last_pict = NULL;
			printf("buffer no data!\n");
		}
		--(vpl->nb_pict);
	}
	SDL_UnlockMutex(vpl->mutex);
	return vp->data;
}

//初始化音频包队列
void _XMovie::initPacketQueue(PacketQueue *q)
{
	memset(q,0,sizeof(PacketQueue));
	q->mutex = SDL_CreateMutex();
	q->cond = SDL_CreateCond();
}

//把音频包pkt放到队列q里
int _XMovie::putIntoPacketQueue(PacketQueue *q , AVPacket *pkt)
{
	AVPacketList *pkt_temp;
	//下面需要在这里修改将音频数据转化成需要的格式放入
	if(av_dup_packet(pkt) < 0)
	{
		return -1;
	}

	pkt_temp = (AVPacketList*)av_malloc(sizeof(AVPacketList));
	if(!pkt_temp)
	{
		return -1;
	}
	pkt_temp->pkt = *pkt;
	pkt_temp->next = NULL;

	SDL_LockMutex(q->mutex);
	if(!q->last_pkt)
	{
		q->first_pkt = pkt_temp;
	}
	else
	{
		q->last_pkt->next = pkt_temp;
	}
	q->last_pkt = pkt_temp;
	++ q->nb_packets;
	q->size += pkt_temp->pkt.size;
	SDL_CondSignal(q->cond);
	SDL_UnlockMutex(q->mutex);
	return 0;
}

void _XMovie::releasePacketQueue()
{
	AVPacketList *pkt1; 
	while(1)
	{ 
		pkt1 = audioq.first_pkt; 
		if(pkt1) 
		{ 
			audioq.first_pkt = pkt1->next; 
			if (!audioq.first_pkt) 
				audioq.last_pkt = NULL; 
			--(audioq.nb_packets); 
			audioq.size -= pkt1->pkt.size; 
			av_free(pkt1); 
		}else
		{
			break;
		}
	}
}
