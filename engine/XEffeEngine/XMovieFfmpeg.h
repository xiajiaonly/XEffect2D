#ifndef _XMOVIE_
#define _XMOVIE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//由于音频采样率转换的问题，造成视频使用的音频采样率必须为44.1k否则将会造成视频声画不一致。
//视频中的音频需要有下面的要求，2声道，16bit的位宽，44.1的采样率
#include "XMovieCore.h"

#include "XFfmpeg.h"

//改进意见：提供支持播放纯声音文件

//#define SDL_AUDIO_BUFFER_SIZE 1024
#define BUFF_TIMER (1.0f)		//缓存的时间

//#pragma comment(lib, "SDL.lib")

struct _XVideoFrameData
{
	AVPicture *pict;		//当前帧的规制后的像素数据
	unsigned char *data;	//当前帧的像素数据
	struct _XVideoFrameData *next;	//下一帧
};
struct _XVideoQueue
{
	_XVideoFrameData *first_pict;	//第一帧
	_XVideoFrameData *last_pict;	//最后一帧
	int nb_pict;			//序列中总的帧的数量
	SDL_mutex *mutex;		//线程锁
};
struct _XAudioQueue
{
	AVPacketList *first_pkt;//第一个音频报的指针
	AVPacketList *last_pkt;	//最后一个音频包的指针
	int nb_packets;			//音频包的数量
	int size;				//总的音频数据的尺寸
	SDL_mutex *mutex;		//线程锁
	SDL_cond *cond;			//用于同步
};

#define MAX_BUFF_DEEP 32	//缓存的深度

class _XMovieFfmpeg:public _XMovieCore
{
private:
	_XBool m_isLoaded;	//是否完成载入
	void release() {}	//进行资源释放
public:
	_XMovieFfmpeg();
	~_XMovieFfmpeg(){closeClip();}

	//打开影片，ffmpeg库等
	_XBool load(const char *filename);		//载入影片文件并作相关的初始化
	//void waitingForBuffOver();			//等待影片缓冲满

	int getClipWide() const {return m_videoWidth;}		//获取影片的实际宽度
	int getClipHeigh() const {return m_videoHeight;}	//获取影片的实际高度

	void setLoop(_XBool isLoop) {m_isLoop = isLoop;}		//设置影片是否循环播放,XTrue则循环,XFalse则否
	double getCurrentPlayingTime() const	//获取影片当前播放时间(单位：毫秒)
	{
		return m_nowFrameNumber*(1000.0/m_nowFrameRate);
	}		
	int getAllTime(void) const				//获取影片总时间(单位：毫秒)
	{
		return ((double)pFormatCtx->duration * 0.001f);
	}
	float getVideoFrameRate() const {return m_nowFrameRate;}	//获得影片的帧率
	_XBool getUpdateFlag(void)		//获取视频缓冲中是否为空
	{
		if(m_videoQueue.nb_pict > 0) return XTrue;
		else return XFalse;
	}

	_XBool getIsEnd() {return m_isEnd;}
	void setPauseOrPlay(void) {m_isStop = !m_isStop;}		//暂停或继续播放影片
	void closeClip(void);			//关闭视频播放，并释放相关的资源
	void gotoFrame(float temp);		//跳到当前视频的某一帧(0 - 1的百分比)
	//GLuint * getTexture();			//获取贴图的编号
private:
	unsigned char *m_nowFrameData;		//影片当前播放的frame
	//SDL_mutex* m_mutex;				//线程锁
	_XVideoFrameData *m_bmp[MAX_BUFF_DEEP];	//图像
	AVPicture m_framePic[MAX_BUFF_DEEP];
	int m_nowBuffPoint;
	_XBool m_needVoice;				//影片是否需要声音
	//GLuint m_texture;
	//2014-3-20修改
	_XTexture *m_movieTex;	//贴图
	bool m_isNewFrame;
	_XCritical m_mutex;
public:
	_XSprite *m_movieSprite;	//用于绘图的精灵
	_XBool upDateFrame()
	{
		if(m_isNewFrame)
		{
			m_isNewFrame = XFalse;
			if(m_movieTex != NULL) 
			{
				m_mutex.Lock();
				m_movieTex->updateTexture(m_nowFrameData);
				m_mutex.Unlock();
			}
			return XTrue;
		}
		return XFalse;
	}
	void pixelLock() {m_mutex.Lock();}
	unsigned char * getDataP() {return m_nowFrameData;}	//注意在使用这个数据是一定要使用线程锁，否则会造成线程不安全
	void pixelUnlock() {m_mutex.Unlock();}
	void getData(unsigned char * p)
	{
		m_mutex.Lock();
		memcpy(p,m_nowFrameData,m_videoWidth * m_videoHeight * 3);
		m_mutex.Unlock();
	}
	void updateTex(_XTexture &tex)
	{
		m_mutex.Lock();
		tex.updateTexture(m_nowFrameData,m_videoWidth,m_videoHeight);
		m_mutex.Unlock();
	}
	void draw()
	{
		if(!m_isLoaded) return;
		if(m_movieSprite != NULL && m_movieTex != NULL) 
			m_movieSprite->draw(&m_movieTex->m_texture);
	}
	unsigned int * getTexture() {return (unsigned int *)&m_movieTex->m_texture;}
private:
	//用于像素数据转换
	SwsContext *m_pSwsContext;	//用于进行图像格式转换
	int imgConvert(AVPicture *dst,const AVPicture *src,int src_height)
	{
		if(m_pSwsContext == NULL) return 0;
		return sws_scale(m_pSwsContext, src->data, src->linesize,  
            0,src_height, dst->data, dst->linesize); 
	}
	//用于音频数据个是转换
	SwrContext *m_pSwrContext;
private:
	_XVideoQueue m_videoQueue;		//视频帧序列
	_XAudioQueue m_audioQueue;		//音频帧序列
	AVFormatContext *pFormatCtx;	//视频格式描述内容
	AVCodecContext *pCodeCtx;		//视频解码描述内容
	AVCodecContext *aCodeCtx;		//音频解码描述内容
	int m_sreAudioFmt;
	AVFrame *pFrame;				//帧数据
	AVPacket packet;				//从视频流中取出的数据包
	int videoStream;				//视频流的数量
	int audioStream;				//音频流的数量
//	SDL_AudioSpec wanted_spec;		//声音设备的描述
//	BOOL bdecdraw;					//是否处于后台绘图阶段??
	_XBool m_isBuffing;				//当前是否出于缓存状态

	_XBool m_isStop;				//是否暂停播放
	_XBool m_isEnd;				//是否播放完成
	_XBool m_isQuit;				//是否退出播放
	int m_nowPlayedTime;		//当前播放的时间
	_XBool m_isDecodeOver;		//是否解码完成

	int m_allFrameSum;			//影片的总帧数
	int m_nowFrameNumber;		//当前播放的帧号
	float m_nowFrameRate;		//当前视频的帧率
	int m_videoWidth;			//视频的宽度
	int m_videoHeight;			//视频的高度
	_XBool m_isLoop;				//视频是否循环播放
	char videofilename[MAX_FILE_NAME_LENGTH];		//视频的文件名
	_XBool m_isThreadDecoderExit;	//解码线程是否退出
	_XBool m_isThreadDrawExit;	//绘图线程是否退出

	void getPixelData();								//绘图函数，将视频缓冲中的图片描绘道屏幕上
#ifdef XEE_OS_LINUX
	friend void *decoderThread(void *arg);			//视频解码线程
	friend void *drawThread(void *arg);				//绘图进程
	friend void audioCallBack(void *userdata,Uint8 *stream,int len);	//声音回调函数
#endif
#ifdef XEE_OS_WINDOWS
	friend DWORD WINAPI decoderThread(void *arg);	//视频解码线程
	friend DWORD WINAPI drawThread(void *arg);		//绘图进程
	friend void audioCallBack(void *userdata,Uint8 *stream,int len);	//声音回调函数
#endif
	void initAudioQueue();			//初始化音频队列
	void initVideoPictList();		//初始化视频队列
	void releaseAudioQueue();		//释放音频队列
	void releaseVideoPictList();	//释放视频队列
	int putIntoPacketQueue();					//将当前获得的音频数据推入队列
	friend int getFromPacketQueue(_XMovieFfmpeg *pClass,AVPacket *pkt,int block);			//从音频队列中提取数据
	friend int audioDecodeFrame(_XMovieFfmpeg *pClass,uint8_t *audio_buf,int buf_size);	//音频解码帧
	void putIntoVideoPicList();					//将当前获得的视频数据推入到队列
	unsigned char * getFormVideoPicList();		//从视频帧队列中提取一帧图像数据

	//下面是关于声音播放的一些变量
	uint8_t m_audioBuf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; 
	unsigned int m_audioBufSize; 
	unsigned int m_audioBufIndex; 
};
#endif
