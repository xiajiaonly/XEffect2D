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
	if(XCurSndCore.playMusic(m_music[musicHandle],loop) == -1)
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
}