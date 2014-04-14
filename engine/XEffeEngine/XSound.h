#ifndef _JIA_XSOUND_
#define _JIA_XSOUND_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XSoundCommon.h"
#include "XBasicFun.h"
#include "XResourceManager.h"
//修改可以从资源包里面提取资源
#include <vector>
//#define MAX_SOUND_SUM 128	//最大音效数量
#define MIN_SOUND_DELAY 50	//相同音效播放的最小时间间隔，单位为毫秒

struct _XSoundInfo
{
	void * sound;	//Mix_Chunk
	_XResourceInfo *resInfo;
	int soundTime;						//这个成员用于控制音效播放的间隔，防止同一个音效在短时间内不断地播放

	_XBool isSlowDownInit;
	_XBool isEnableSlowDown;
	unsigned char *slowDownOldData;
	int slowDownOldLen;
	unsigned char *slowDownNewData;
	int slowDownNewLen;
	int slowDownRate;
	int lateChannel;	//记录最近播放当前音效的声道，以便于可以控制这个音效的停止等

	_XSoundInfo()
		:sound(NULL)
		,resInfo(NULL)
		,isSlowDownInit(XFalse)
		,isEnableSlowDown()
		,slowDownOldData(NULL)
		,slowDownNewData(NULL)
		,soundTime(-1)
		,lateChannel(-1)
	{}
};

typedef int XSoundHandle;
//这个设计为单子系统
class _XSound
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//下面需要将其设计为Singleton模式
protected:
	_XSound();
	_XSound(const _XSound&);
	_XSound &operator= (const _XSound&);
	virtual ~_XSound();
public:
	static _XSound& GetInstance()
	{
		static _XSound m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	_XResourcePosition m_resoursePosition;	//资源位置
private:
	std::vector<_XSoundInfo *> m_sound;

	int m_soundVolume;
	_XBool m_firstAddSoundFlag;
public:
	XSoundHandle addSound(const char* filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);					//载入一个音效，返回这个音效的句柄，用于播放
	void clearUp();											//清除掉所有载入的声音资源
	void clearOneSound(XSoundHandle soundHandle);			//清除掉指定的音效资源
	_XBool play(XSoundHandle soundHandle,int loops = 0);	//-1为无限循环
	void setSoundVolume(int volume);						//设置当前音效播放的音量
	void setAllVolume(int volume);							//设置所有声音播放的音量

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是关于声音延长的方法，只在SDL_Mixer下有小
	_XBool slowDownInit(int rate,XSoundHandle handle);
	void enableSlowDown(XSoundHandle handle);	//使能延长
	void disableSlowDown(XSoundHandle handle);	//取消延长
	void slowDownRelease(XSoundHandle handle);	//释放资源
	_XBool slowDownAllInit(int rate);
	void enableAllSlowDown();	//使能延长
	void disableAllSlowDown();	//取消延长
	void slowDownAllRelease();	//释放资源
	//-------------------------------------------------------

	void pause(XSoundHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return;
		_XSoundHandle.pauseSound(m_sound[s]->lateChannel);
	}
	void resume(XSoundHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return;
		_XSoundHandle.resumeSound(m_sound[s]->lateChannel);
	}
	_XBool isPause(XSoundHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return XFalse;
		return _XSoundHandle.isSoundPause(m_sound[s]->lateChannel);
	}
	void stop(XSoundHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return;
		_XSoundHandle.soundFadeOut(m_sound[s]->lateChannel,100);
	}
	_XBool isEnd(XSoundHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return XTrue;
		return !_XSoundHandle.isSoundPlaying(m_sound[s]->lateChannel);
	}
	_XBool fadeInSound(XSoundHandle s,int loops,int ms);
	void fadeOutSound(XSoundHandle s,int ms)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return;
		_XSoundHandle.soundFadeOut(m_sound[s]->lateChannel,ms);
	}
	void release()	//注意这里这个类不会自动在析构函数里面调用释放函数，所以一定要记得主动调用释放函数，自动调用的话主要是怕重复调用
	{
		_XSoundHandle.haltSound();//关闭所有的channel
		clearUp();
	}
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
	_XSoundHandle.setVolume(-1,m_soundVolume);
}
#endif