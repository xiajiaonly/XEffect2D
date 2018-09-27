#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMusic.h"
#include "XResourcePack.h"
namespace XE{
XBool XMusic::playMusic(XMusicHandle musicHandle,int loop)
{
	if(musicHandle < 0 || musicHandle >= m_music.size()
		|| m_music[musicHandle] == NULL) return XFalse;
#if AUDIO_MATHOD == 0
	if(XCurSndCore.playMusic(m_music[musicHandle],loop) == -1)
#endif
#if AUDIO_MATHOD == 1
	if(XCurSndCore.playMusic(m_music[musicHandle],loop) == nullptr)
#endif
	{
		LogStr("Sound play error!");
		return XFalse;
	}
	return XTrue;
}
XMusicHandle XMusic::addMusic(const char *fileName)
{
	if(!m_firstAddMusicFlag)
	{//这里相当于是初始化
		m_musicVolume = XCurSndCore.getMusicVolume();
		m_firstAddMusicFlag = XTrue;
	}
	void *p = NULL;
	if(!XCurSndCore.loadMusic(fileName,p))
	{
		LogStr("Sound load error!");
		return -1;
	}
	m_music.push_back(p);
	return (int)(m_music.size()) - 1;
}
void XMusic::clearUp()
{
	for(unsigned int i = 0;i < m_music.size();++ i)
	{
		XCurSndCore.clearMusic(m_music[i]);
		m_music[i] = NULL;
	}
	m_music.clear();
}
XMusic::~XMusic()
{
	XCurSndCore.haltMusic();
	clearUp();
}
XBool XMusic::fadeInMusic(XMusicHandle musicHandle, int loop, int ms)
{
	if (musicHandle < 0 || musicHandle >= m_music.size()
		|| m_music[musicHandle] == NULL) return XFalse;
#if AUDIO_MATHOD == 0
	return XCurSndCore.musicFadeIn(m_music[musicHandle], loop, ms) != -1;
#endif
#if AUDIO_MATHOD == 1
	return XCurSndCore.musicFadeIn(m_music[musicHandle], loop, ms) != nullptr;
#endif
}
void XMusic::fadeOutMusic(int ms)
{
	XCurSndCore.musicFadeOut(ms);
}
void XMusic::clearOneMusic(XMusicHandle musicHandle)
{
	if (musicHandle < 0 || musicHandle >= m_music.size()
		|| m_music[musicHandle] == NULL) return;
	if (m_music[musicHandle] != NULL)
	{
		XCurSndCore.clearMusic(m_music[musicHandle]);
		m_music[musicHandle] = NULL;
	}
}
void XMusic::setMusicVolume(int volume)
{
	if (volume < 0) volume = 0;
	if (volume > 128) volume = 128;
	m_musicVolume = volume;
	XCurSndCore.setMusicVolume(m_musicVolume);
}
XBool XMusic::isEnd() { return !XCurSndCore.isMusicPlaying(); }
void XMusic::pause() { XCurSndCore.pauseMusic(); }
void XMusic::resume() { XCurSndCore.resumeMusic(); }
void XMusic::rewind() { XCurSndCore.rewindMusic(); }
XBool XMusic::isPause() { return XCurSndCore.isMusicPause(); }

}