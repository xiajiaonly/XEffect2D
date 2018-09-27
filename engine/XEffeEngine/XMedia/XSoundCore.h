#ifndef _JIA_XSOUNDCORE_
#define _JIA_XSOUNDCORE_
//这是一个核心的声音的基类，所有的声音的类都需要从这个基类中继承，从而达到框架继承的目的
//这样设计的目的是为了能更方便的替换一个底层模块(尚未实现),考虑在重新架构时加入。
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.16
//--------------------------------
#include <string>
#include <vector>
namespace XE{
enum XSoundState
{
	SOUND_STATE_NORMAL,		//普通状态
	SOUND_STATE_FADEIN,		//淡入
	SOUND_STATE_FADEOUT,	//淡出
};
struct XSoundFadeData
{
	XSoundState state;
#if AUDIO_MATHOD == 0
	int channel;	//通道编号
#endif
#if AUDIO_MATHOD == 1
	void* channel;	//通道编号
#endif
	int soundTimer;
	int soundFadeTime;
	int curSoundVolume;	//当前的音量
	XSoundFadeData()
		:state(SOUND_STATE_NORMAL)
#if AUDIO_MATHOD == 0
		, channel(0)	//通道编号
#endif
#if AUDIO_MATHOD == 1
		, channel(nullptr)	//通道编号
#endif
	{}
};

typedef void (* SND_CALLBACK_FUN)(void *,unsigned char *,int);

class XSoundCore
{
public:
	XSoundCore(){}
	virtual ~XSoundCore() {}
public:
	virtual bool init() = 0;
	virtual void close() = 0;

	//music部分
	virtual bool loadMusic(const std::string& filename,void *&p) = 0;
	virtual void clearMusic(void *p) = 0;
	virtual int setMusicVolume(int v) = 0;
	virtual int getMusicVolume() = 0;
#if AUDIO_MATHOD == 0
	virtual int playMusic(void *p,int loop) = 0;
	virtual int musicFadeIn(void * p,int loop,int ms) = 0;
#endif
#if AUDIO_MATHOD == 1
	virtual void* playMusic(void *p,int loop) = 0;
	virtual void* musicFadeIn(void * p,int loop,int ms) = 0;
#endif
	virtual int musicFadeOut(int ms) = 0;

	virtual void pauseMusic() = 0;
	virtual void resumeMusic() = 0;
	virtual void rewindMusic() = 0;
	virtual bool isMusicPause() = 0;
	virtual bool isMusicPlaying() = 0;
	virtual int haltMusic() = 0;
	//sound部分
	virtual bool loadSound(const std::string& filename,void *&p) = 0;	//从文件中读取声音资源
	virtual bool loadSound(void *data,int len,void *&p) = 0;			//从内存中读取声音资源
	virtual void clearSound(void * p) = 0;
#if AUDIO_MATHOD == 0
	virtual int setVolume(int c,int v) = 0;
	virtual int getVolume(int c) = 0;
	virtual int playSound(void *p,int loop) = 0;

	virtual int soundFadeIn(void * p,int loop,int ms) = 0;
	virtual int soundFadeOut(int c,int ms) = 0;
	virtual void pauseSound(int c) = 0;
	virtual void resumeSound(int c) = 0;
	virtual bool isSoundPause(int c) = 0;
	virtual bool isSoundPlaying(int c) = 0;
	virtual int haltSound(int c) = 0;
#endif
#if AUDIO_MATHOD == 1
	virtual int setVolume(void* c,int v) = 0;
	virtual int getVolume(void* c) = 0;
	virtual void* playSound(void *p,int loop) = 0;

	virtual void* soundFadeIn(void* p,int loop,int ms) = 0;
	virtual int soundFadeOut(void* c,int ms) = 0;
	virtual void pauseSound(void* c) = 0;
	virtual void resumeSound(void* c) = 0;
	virtual bool isSoundPause(void* c) = 0;
	virtual bool isSoundPlaying(void* c) = 0;
	virtual int haltSound(void* c) = 0;
#endif
	virtual int haltSound() = 0;

	//下面是关于回调函数部分
	virtual void setCallBack(SND_CALLBACK_FUN fun,void *) = 0;
	//为了替换fadein和fadeout 接口下面定义
	virtual void update(float stepTime) = 0;

	virtual unsigned char * getData(void *p) = 0;
	virtual int getDataLen(void *p) = 0;
	virtual void setData(void *p,unsigned char * d) = 0;
	virtual void setDataLen(void *p,int len) = 0;
};
}
#endif