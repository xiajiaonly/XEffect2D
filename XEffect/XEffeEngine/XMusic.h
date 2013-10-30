#ifndef _JIA_XMUSIC_
#define _JIA_XMUSIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "SDL_mixer.h"
#include "XLogBook.h"

//目前好像并不支持从内存中载入MP3格式，查看sdl_mixer中的Mix_LoadMUS_RW说明中好像意思是只支持Ogg和MikMod
#define MAX_MUSIC_SUM 128	//最大的音乐

typedef int XMusicHandle;

class _XMusic
{
//public:
//	int m_resoursePosition;	//资源位置 0:外部 1:内部
private:
	Mix_Music *m_music[MAX_MUSIC_SUM];
	int m_musicVolume;
	int firstAddMusicFlag;
public:
	XMusicHandle addMusic(const char *fileName);
	void clearUp();											//清除掉所有载入的声音资源
	void clearOneMusic(XMusicHandle musicHandle);			//清除掉指定的音效资源
	int playMusic(XMusicHandle musicHandle,int loop = 0);
	void setMusicVolume(int volume);						//设置当前音效播放的音量(0 - 128)
	void fadeOutMusic(int ms);
	int fadeInMusic(XMusicHandle musicHandle,int loop = 0,int ms = 0);
	_XMusic();
	~_XMusic();

	int isEnd();
};

inline int _XMusic::fadeInMusic(XMusicHandle musicHandle,int loop,int ms)
{
	if(musicHandle < 0 || musicHandle >= MAX_MUSIC_SUM) return 0;
	if(Mix_FadeInMusic(m_music[musicHandle],loop,ms) == -1) return 0;
	return 1;
}

inline void _XMusic::fadeOutMusic(int ms)
{
	Mix_FadeOutMusic(ms);
}

inline void _XMusic::clearOneMusic(XMusicHandle musicHandle)
{
	if(musicHandle < 0 || musicHandle >= MAX_MUSIC_SUM) return;
	if(m_music[musicHandle] != NULL)
	{
		Mix_FreeMusic(m_music[musicHandle]);
		m_music[musicHandle] = NULL;
	}
}

inline int _XMusic::playMusic(XMusicHandle musicHandle,int loop)
{
	if(musicHandle < 0 || musicHandle >= MAX_MUSIC_SUM) return 0;
	if(Mix_PlayMusic(m_music[musicHandle],loop) == -1)
	{
		AddLogInfoStr("Sound play error!\n");
		return 0;
	}
	return 1;
}

inline void _XMusic::setMusicVolume(int volume)
{
	if(volume < 0) volume = 0;
	if(volume > 128) volume = 128;
	m_musicVolume = volume;
	Mix_VolumeMusic(m_musicVolume);
}

inline int _XMusic::isEnd()
{
	if(Mix_PlayingMusic() == 1)
	{
		return 0;
	}else
	{
		return 1;
	}
}

#endif