#ifndef _JIA_XMUSIC_
#define _JIA_XMUSIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XSoundCommon.h"
#include <vector>
namespace XE{
//目前好像并不支持从内存中载入MP3格式，查看sdl_mixer中的Mix_LoadMUS_RW说明中好像意思是只支持Ogg和MikMod
//#define MAX_MUSIC_SUM 128	//最大的音乐
typedef int XMusicHandle;
class XMusic
{
public:
	XMusic()
		:m_firstAddMusicFlag(XFalse)
	{
		m_music.clear();
	}
	virtual ~XMusic();
protected:
	XMusic(const XMusic&);
	XMusic &operator= (const XMusic&);
//public:
//	int m_resoursePosition;	//资源位置 0:外部 1:内部
private:
	std::vector<void *> m_music;	//Mix_Music
	//Mix_Music *m_music[MAX_MUSIC_SUM];
	int m_musicVolume;
	XBool m_firstAddMusicFlag;
public:
	XMusicHandle addMusic(const char *fileName);
	void clearUp();												//清除掉所有载入的声音资源
	void clearOneMusic(XMusicHandle musicHandle);				//清除掉指定的音效资源
	XBool playMusic(XMusicHandle musicHandle,int loop = 0);	//-1无限循环
	void setMusicVolume(int volume);							//设置当前音效播放的音量(0 - 128)
	void fadeOutMusic(int ms);
	XBool fadeInMusic(XMusicHandle musicHandle,int loop = 0,int ms = 0);

	XBool isEnd();
	void pause();
	void resume();
	void rewind();
	XBool isPause();
	static bool getIsInvalid(XMusicHandle hangle){return hangle < 0;} 
};
}
#endif