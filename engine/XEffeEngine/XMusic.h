#ifndef _JIA_XMUSIC_
#define _JIA_XMUSIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XSoundCommon.h"
#include "XLogBook.h"
#include <vector>

//目前好像并不支持从内存中载入MP3格式，查看sdl_mixer中的Mix_LoadMUS_RW说明中好像意思是只支持Ogg和MikMod
//#define MAX_MUSIC_SUM 128	//最大的音乐
typedef int XMusicHandle;
class _XMusic
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//下面需要将其设计为Singleton模式
protected:
	_XMusic()
		:m_firstAddMusicFlag(XFalse)
	{
		m_music.clear();
	}
	_XMusic(const _XMusic&);
	_XMusic &operator= (const _XMusic&);
	virtual ~_XMusic()
	{
		_XSoundHandle.haltMusic();
		clearUp();
	}
public:
	static _XMusic& GetInstance()
	{
		static _XMusic m_instance;
		return m_instance;
	}
	//-------------------------------------------
//public:
//	int m_resoursePosition;	//资源位置 0:外部 1:内部
private:
	std::vector<void *> m_music;	//Mix_Music
	//Mix_Music *m_music[MAX_MUSIC_SUM];
	int m_musicVolume;
	_XBool m_firstAddMusicFlag;
public:
	XMusicHandle addMusic(const char *fileName);
	void clearUp();												//清除掉所有载入的声音资源
	void clearOneMusic(XMusicHandle musicHandle);				//清除掉指定的音效资源
	_XBool playMusic(XMusicHandle musicHandle,int loop = 0);	//-1无限循环
	void setMusicVolume(int volume);							//设置当前音效播放的音量(0 - 128)
	void fadeOutMusic(int ms);
	_XBool fadeInMusic(XMusicHandle musicHandle,int loop = 0,int ms = 0);

	_XBool isEnd() {return !_XSoundHandle.isMusicPlaying();}
	void pause(){_XSoundHandle.pauseMusic();}
	void resume(){_XSoundHandle.resumeMusic();}
	void rewind(){_XSoundHandle.rewindMusic();}
	_XBool isPause() {return _XSoundHandle.isMusicPause();}
};
inline _XBool _XMusic::fadeInMusic(XMusicHandle musicHandle,int loop,int ms)
{
	if(musicHandle < 0 || musicHandle >= m_music.size()
		|| m_music[musicHandle] == NULL) return XFalse;
	if(_XSoundHandle.musicFadeIn(m_music[musicHandle],loop,ms) == -1) return XFalse;
	return XTrue;
}
inline void _XMusic::fadeOutMusic(int ms)
{
	_XSoundHandle.musicFadeOut(ms);
}
inline void _XMusic::clearOneMusic(XMusicHandle musicHandle)
{
	if(musicHandle < 0 || musicHandle >= m_music.size()
		|| m_music[musicHandle] == NULL) return;
	if(m_music[musicHandle] != NULL)
	{
		_XSoundHandle.clearMusic(m_music[musicHandle]);
		m_music[musicHandle] = NULL;
	}
}
inline _XBool _XMusic::playMusic(XMusicHandle musicHandle,int loop)
{
	if(musicHandle < 0 || musicHandle >= m_music.size()
		|| m_music[musicHandle] == NULL) return XFalse;
	if(_XSoundHandle.playMusic(m_music[musicHandle],loop) == -1)
	{
		AddLogInfoStr("Sound play error!\n");
		return XFalse;
	}
	return XTrue;
}
inline void _XMusic::setMusicVolume(int volume)
{
	if(volume < 0) volume = 0;
	if(volume > 128) volume = 128;
	m_musicVolume = volume;
	_XSoundHandle.setMusicVolume(m_musicVolume);
}
#endif