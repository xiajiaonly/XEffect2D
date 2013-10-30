#ifndef _XMOVIE_
#define _XMOVIE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//由于音频采样率转换的问题，造成视频使用的音频采样率必须为44.1k否则将会造成视频声画不一致。
//视频中的音频需要有下面的要求，2声道，16bit的位宽，44.1的采样率
#include "glew.h"
#include "SDL.h"
#include "SDL_thread.h"

#include <time.h>
#include "XBasicFun.h"
#include "XOSDefine.h"

#ifdef OS_WINDOWS
#include <Windows.h>
#include "avformat.h"
#include "avcodec.h"
#endif
#ifdef OS_LINUX
#include <pthread.h>
#include "avformat.h"
#include "avcodec.h"
#endif

//1、加入声音的控制
//2、将贴图装换成OpenGL

#ifdef CONFIG_WIN32
#undef main /* We don't want SDL to override our main() */
#endif

#define SDL_AUDIO_BUFFER_SIZE 1024
#define VIDEO_FILE_NAME_LENGTH 256

#pragma comment(lib, "SDL.lib")

typedef struct VideoPict
{
	AVPicture *pict;
	//SDL_Overlay *bmp;
	unsigned char *data;
	struct VideoPict *next;
}VideoPict;

typedef struct VideoPictList
{
	VideoPict *first_pict;
	VideoPict *last_pict;
	int nb_pict;
	SDL_mutex *mutex;
}VideoPictList;

typedef struct PacketQueue
{
	AVPacketList *first_pkt , *last_pkt;
	int nb_packets;
	int size;
	SDL_mutex *mutex;
	SDL_cond *cond;
}PacketQueue;

#define MAX_BUFF_DEEP 128

class _XMovie
{
private:
	char m_isLoaded;	//是否完成载入
public:
	_XMovie();
	~_XMovie();

	void init(SDL_Surface *temp);		//连接上窗口
	//打开影片，ffmpeg库等
	int load(const char *filename);		//载入影片文件并作相关的初始化
	void waitingForBuffOver(void);		//等待影片缓冲满

	//unsigned char* GetData(void);
	int getClipWide(void);				//获取影片的实际宽度
	int getClipHigh(void);				//获取影片的实际高度

	//int GetClipChannel(void);
	void setLoop(bool Replay);			//设置影片是否循环播放 , TRUE则循环 ， FALSE则否
	double getCurrentPlayingTime();		//获取影片当前播放时间(单位：毫秒)

	//void SetCurrentFrame(double Frame);	//设置当前播放帧数
	
	int getAllTime(void);			//获取影片总时间(单位：毫秒)
	float getVideoFrameRate();		//获得影片的帧率
	bool getUpdateFlag(void);		//获取视频缓冲中是否为空

	void setPauseOrPlay(void);		//暂停或继续播放影片
	void closeClip(void);			//关闭视频播放，并释放相关的资源
	void gotoFrame(float temp);		//跳到当前视频的某一帧
	unsigned char *m_nowFrame;		//影片当前播放的frame
	SDL_mutex* m_mutex;
	VideoPict *m_bmp[MAX_BUFF_DEEP];
	unsigned char *m_frameDataBuff[MAX_BUFF_DEEP];
	AVPicture m_framePic[MAX_BUFF_DEEP];
	int m_nowBuffPoint;
	int m_needVoice;				//影片是否需要声音
	GLuint m_texture;
	GLuint * getTexture();

private:
	VideoPictList videopictlist;	//视频帧序列
	static PacketQueue audioq;		//音频帧序列
	AVFormatContext *pFormatCtx;	//视频格式描述内容
	AVCodecContext *pCodeCtx;		//视频解码描述内容
	AVCodecContext *aCodeCtx;		//音频解码描述内容
	AVFrame *pFrame;				//帧数据
	AVPacket packet;
	int videoStream;			//视频流的数量
	int audioStream;			//音频流的数量
	SDL_AudioSpec wanted_spec;	//声音设备的描述
	SDL_Surface *m_screen;		//视口句柄
//	BOOL bdecdraw;				//是否处于后台绘图阶段??
	bool m_isBuffing;			//当前是否出于缓存状态

	static bool m_isStop;		//是否暂停播放
	bool m_isEnd;				//是否播放完成
	static bool m_isQuit;		//是否退出播放
	int m_nowPlayedTime;		//当前播放的时间

	int m_allFrameSum;			//影片的总帧数
	int m_nowFrameNumber;		//当前播放的帧号
	float m_nowFrameRate;		//当前视频的帧率
	int m_videoWidth;			//视频的宽度
	int m_videoHeight;			//视频的高度
	bool m_isLoop;				//视频是否循环播放
	char videofilename[VIDEO_FILE_NAME_LENGTH];		//视频的文件名

	void draw(void);								//绘图函数，将视频缓冲中的图片描绘道屏幕上
#ifdef OS_LINUX
	friend void *decoderThread(void *arg);			//视频解码线程
	friend void *drawThread(void *arg);				//绘图进程
	friend void audioCallBack(void *userdata , Uint8 *stream , int len);	//声音回调函数
#endif
#ifdef OS_WINDOWS
	friend DWORD WINAPI decoderThread(void *arg);	//视频解码线程
	friend DWORD WINAPI drawThread(void *arg);		//绘图进程
	friend void audioCallBack(void *userdata , Uint8 *stream , int len);	//声音回调函数
#endif
	void initPacketQueue(PacketQueue *q);			//初始化音频队列
	void initVideoPictList(VideoPictList *vpl);		//初始化视频队列
	void releaseVideoPictList();
	void releasePacketQueue();
	int putIntoPacketQueue(PacketQueue *q , AVPacket *pkt);						//向音频队列中推入数据
	friend int getFromPacketQueue(PacketQueue *q , AVPacket *pkt ,int block);	//从音频队列中提取数据
	friend int audioDecodeFrame(AVCodecContext *aCodecCtx, uint8_t *audio_buf, int buf_size);	//音频解码帧
	void putIntoVideoPicList(VideoPictList *vpl,VideoPict *picTemp);			//向视频队列中插入一帧数据
	unsigned char * getFormVideoPicList(VideoPictList *vpl);					//从视频帧队列中提取一帧数据
};
#endif
