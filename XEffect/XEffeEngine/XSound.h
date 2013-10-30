#ifndef _JIA_XSOUND_
#define _JIA_XSOUND_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include <SDL_mixer.h>
#include "XBasicFun.h"
//修改可以从资源包里面提取资源

#define MAX_SOUND_SUM 128	//最大音效数量
#define MIN_SOUND_DELAY 50	//相同音效播放的最小时间间隔，单位为毫秒

typedef int XSoundHandle;

class _XSound
{
private:
	int m_resoursePosition;	//资源位置 0:外部 1:内部
private:
	Mix_Chunk *m_sound[MAX_SOUND_SUM];
	int m_soundTime[MAX_SOUND_SUM];						//这个成员用于控制音效播放的间隔，防止同一个音效在短时间内不断地播放
	int m_soundVolume;
	int firstAddSoundFlag;

	char m_isSlowDownInit[MAX_SOUND_SUM];
	char m_isEnableSlowDown[MAX_SOUND_SUM];
	unsigned char *m_slowDownOldData[MAX_SOUND_SUM];
	int m_slowDownOldLen[MAX_SOUND_SUM];
	unsigned char *m_slowDownNewData[MAX_SOUND_SUM];
	int m_slowDownNewLen[MAX_SOUND_SUM];
	int m_slowDownRate[MAX_SOUND_SUM];
public:
	XSoundHandle addSound(const char* filename,int resoursePosition = 0);					//载入一个音效，返回这个音效的句柄，用于播放
	void clearUp();										//清除掉所有载入的声音资源
	void clearOneSound(XSoundHandle soundHandle);		//清除掉指定的音效资源
	int play(XSoundHandle soundHandle);
	void setSoundVolume(int volume);					//设置当前音效播放的音量
	void setAllVolume(int volume);						//设置所有声音播放的音量
	_XSound();
	~_XSound();

	//下面是关于声音延长的方法
	int slowDownInit(int rate,XSoundHandle handle);
	void enableSlowDown(XSoundHandle handle);	//使能延长
	void disableSlowDown(XSoundHandle handle);	//取消延长
	void slowDownRelease(XSoundHandle handle);	//释放资源

	int slowDownAllInit(int rate);
	void enableAllSlowDown();	//使能延长
	void disableAllSlowDown();	//取消延长
	void slowDownAllRelease();	//释放资源
	//forTest，测试声音延长在一定范围内有效
	void slowDown();
};

inline void _XSound::setSoundVolume(int volume)
{
	if(volume < 0) volume = 0;
	if(volume > 128) volume = 128;
	m_soundVolume = volume;
}

inline void _XSound::setAllVolume(int volume)
{
	if(volume < 0) volume = 0;
	if(volume > 128) volume = 128;
	m_soundVolume = volume;
	Mix_Volume(-1,m_soundVolume);
}
#endif