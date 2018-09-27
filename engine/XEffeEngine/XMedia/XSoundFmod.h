#ifndef _JIA_XSOUNDFMOD_
#define _JIA_XSOUNDFMOD_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.16
//--------------------------------

#include "XSoundCore.h"
//#include "../XFrameWork.h"
extern "C" 
{
	#include "fmod.h"
//	#include "fmod_errors.h"
}
#ifdef _WIN64
#pragma comment(lib, "fmodex/vs/fmodex64_vc.lib")
#else
#pragma comment(lib, "fmodex/vs/fmodex_vc.lib")
#endif
#if WITH_ALL_WARNING == 0
#pragma warning(disable: 4312)    //'type cast' : conversion from 'int' to 'FMOD_CHANNEL *' of greater size
#pragma warning(disable: 4311)    //'type cast' : pointer truncation from 'FMOD_CHANNEL *' to 'int'
#endif
namespace XE{
class XSoundFmod:public XSoundCore
{
public:
	XSoundFmod()
		:m_musicState(SOUND_STATE_NORMAL)
		, m_isCallBackOpen(false)
		, m_channelGroup(nullptr)
		, m_sys(nullptr)
		, m_musicChannel(NULL)
		, m_curMusic(nullptr)
		, m_cbSound(nullptr)
		, m_cbChannel(nullptr)
		, m_cbData(nullptr)
	{}
	virtual ~XSoundFmod() {}
protected:
	XSoundFmod(const XSoundFmod&);
	XSoundFmod &operator= (const XSoundFmod&);
private:
	XSoundState m_musicState;
	int m_curMusicVolume;	//当前音乐的音量
	int m_musicTimer;
	int m_musicFadeTime;
	std::vector<XSoundFadeData> m_soundFadeList;	//队列

	FMOD_CHANNELGROUP *m_channelGroup;
	FMOD_SYSTEM* m_sys;
	FMOD_CHANNEL* m_musicChannel;
	void *m_curMusic;
	bool m_isCallBackOpen;
	FMOD_SOUND *m_cbSound;
	FMOD_CHANNEL *m_cbChannel;
	SND_CALLBACK_FUN m_cbFun;
	void *m_cbData;
	friend FMOD_RESULT F_CALLBACK pcmreadcallback(FMOD_SOUND *,void *,unsigned int);
public:
	bool init();
	void close() 
	{
		if(m_isCallBackOpen)
		{
			FMOD_Channel_Stop(m_cbChannel);
			FMOD_Sound_Release(m_cbSound);
			m_isCallBackOpen = false;
		}
		FMOD_System_Close(m_sys);
		FMOD_System_Release(m_sys);
		m_sys = nullptr;
	}
	//music部分
	bool loadMusic(const std::string& filename,void *&p)
	{
		int fmodFlags = FMOD_HARDWARE | FMOD_CREATESTREAM;
		FMOD_SOUND *sound = NULL;
		FMOD_RESULT result = FMOD_System_CreateSound(m_sys,filename.c_str(),fmodFlags,NULL,&sound);
		if(result != FMOD_OK) return false;

		//FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_PCM);
		p = sound;
		return true;
	}

	void clearMusic(void *p)
	{
		if (m_sys == nullptr) return;
		FMOD_Sound_Release((FMOD_SOUND *)p);
	}
	int setMusicVolume(int v) {return FMOD_Channel_SetVolume(m_musicChannel,v / 128.0f);} 
	int getMusicVolume()
	{
		float v;
		if(m_musicChannel == NULL) FMOD_ChannelGroup_GetVolume(m_channelGroup,&v);
		else FMOD_Channel_GetVolume(m_musicChannel,&v);	//范围问题 0 - 1.0f;
		return (int)(v * 128.0f);
	}
	void* playMusic(void *p,int loop)
	{
		FMOD_RESULT result = FMOD_System_PlaySound(m_sys, FMOD_CHANNEL_FREE, (FMOD_SOUND *)p, false, &m_musicChannel);
		if(result != FMOD_OK) return nullptr;
		if(loop != 0)
		{
			FMOD_Channel_SetMode(m_musicChannel,FMOD_LOOP_NORMAL);	//这里不能控制循环次数
			FMOD_Channel_SetLoopCount(m_musicChannel,loop);	//loop为-1在fmod中是否为无限循环
		}else FMOD_Channel_SetMode(m_musicChannel,FMOD_LOOP_OFF);
		m_curMusic = p;
		return m_musicChannel;
	}
	void* musicFadeIn(void * p,int loop,int ms);
	int musicFadeOut(int ms);

	void pauseMusic(){FMOD_Channel_SetPaused(m_musicChannel,true);}
	void resumeMusic() {FMOD_Channel_SetPaused(m_musicChannel,false);}
	void rewindMusic() 
	{//没有测试过
		haltMusic();
		playMusic(m_curMusic,0);
	}
	bool isMusicPause() 
	{
		FMOD_BOOL b = 0;
		FMOD_Channel_GetPaused(m_musicChannel,&b);
		return b != 0;
	}
	bool isMusicPlaying() 
	{
		FMOD_BOOL b = 0;
		FMOD_Channel_IsPlaying(m_musicChannel,&b);
		return b != 0;
	}
	int haltMusic() 
	{
		m_musicState = SOUND_STATE_NORMAL;
		return FMOD_Channel_Stop(m_musicChannel);
	} 
	//sound部分
	bool loadSound(const std::string& filename,void *&p)	//从文件中读取声音资源
	{
		int fmodFlags = FMOD_HARDWARE;
		FMOD_SOUND *sound = NULL;
		FMOD_RESULT result = FMOD_System_CreateSound(m_sys,filename.c_str(),fmodFlags,NULL,&sound);
		if(result != FMOD_OK) return false;

		//FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_PCM);
		p = sound;
		return true;
	}
	bool loadSound(void *data,int,void *&p)
	{
		int fmodFlags = FMOD_HARDWARE | FMOD_OPENMEMORY;
		FMOD_SOUND *sound = NULL;
		FMOD_RESULT result = FMOD_System_CreateSound(m_sys,(char *)data,fmodFlags,NULL,&sound);
		if(result != FMOD_OK) return false;

		//FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_PCM);
		p = sound;
		return true;
	}
	void clearSound(void * p) {FMOD_Sound_Release((FMOD_SOUND *)p);}
	int setVolume(void* c,int v) 
	{
		if(c != nullptr)
			return FMOD_Channel_SetVolume((FMOD_CHANNEL *)c,v/128.0f);
		else
			return FMOD_ChannelGroup_SetVolume(m_channelGroup, v / 128.0f);
	}
	int getVolume(void* c) 
	{
		float v;
		if(c == NULL) FMOD_ChannelGroup_GetVolume(m_channelGroup,&v);
		else FMOD_Channel_GetVolume((FMOD_CHANNEL *)c,&v);	//范围问题
		return (int)(v * 128.0f);
	}
	void* playSound(void *p, int loop)
	{
		FMOD_CHANNEL* c = NULL;
		FMOD_RESULT result = FMOD_System_PlaySound(m_sys, FMOD_CHANNEL_FREE, (FMOD_SOUND *)p, false, &c);
		if (result != FMOD_OK) return nullptr;
		if (loop != 0) FMOD_Channel_SetMode(c, FMOD_LOOP_NORMAL);	//这里不能控制循环次数
		else FMOD_Channel_SetMode(c, FMOD_LOOP_OFF);
		return c;
	}

	void* soundFadeIn(void* p,int loop,int ms);
	int soundFadeOut(void* c,int ms);
	void pauseSound(void* c) {FMOD_Channel_SetPaused((FMOD_CHANNEL *)c,true);}
	void resumeSound(void* c) {FMOD_Channel_SetPaused((FMOD_CHANNEL *)c,false);}
	bool isSoundPause(void* c)
	{
		FMOD_BOOL b = 0;
		FMOD_Channel_GetPaused((FMOD_CHANNEL *)c,&b);
		return b != 0;
	}
	bool isSoundPlaying(void* c)
	{
		FMOD_BOOL b = 0;
		FMOD_Channel_IsPlaying((FMOD_CHANNEL *)c,&b);
		return b != 0;
	}
	int haltSound()	//这个存在问题，会关闭music
	{
		if (m_channelGroup == nullptr) return 0;
		return FMOD_ChannelGroup_Stop(m_channelGroup);
	}
	int haltSound(void* c)
	{
		return FMOD_Channel_Stop((FMOD_CHANNEL *)c);
	} 

	//下面是关于回调函数部分
	void setCallBack(SND_CALLBACK_FUN fun, void *arg);
	//为了替换fadein和fadeout 接口下面定义
	void update(float stepTime);

	//下面的接口无法实现,获取某个sound的数据和数据长度，并设置
	unsigned char * getData(void *) {return NULL;}
	int getDataLen(void *) {return 0;}
	void setData(void *,unsigned char * ) {;}
	void setDataLen(void *,int) {;}

	//个性化的接口
	//改变播放的位置vol [-1 +1]
	void setPan(void* c,float vol) {FMOD_Channel_SetPan((FMOD_CHANNEL *)c,vol);}
	//改变播放速度
	void setSpeed(void* c, float spd);
};
}
#endif