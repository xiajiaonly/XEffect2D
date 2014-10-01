//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMusic.h"
#include "XBasicFun.h"
#include "XResourcePack.h"

XMusicHandle _XMusic::addMusic(const char *fileName)
{
	if(!m_firstAddMusicFlag)
	{//这里相当于是初始化
		m_musicVolume = _XSoundHandle.getMusicVolume();
		m_firstAddMusicFlag = XTrue;
	}
	void *p = NULL;
	if(!_XSoundHandle.loadMusic(fileName,p))
	{
		LogStr("Sound load error!");
		return -1;
	}
	m_music.push_back(p);
	return m_music.size() - 1;
}
void _XMusic::clearUp()
{
	for(int i = 0;i < m_music.size();++ i)
	{
		_XSoundHandle.clearMusic(m_music[i]);
		m_music[i] = NULL;
	}
	m_music.clear();
}