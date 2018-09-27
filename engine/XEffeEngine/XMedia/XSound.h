#ifndef _JIA_XSOUND_
#define _JIA_XSOUND_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XSoundCommon.h"
//#include "XBasicFun.h"
#include "XResourceManager.h"
//修改可以从资源包里面提取资源
#include <vector>
namespace XE{
//#define MAX_SOUND_SUM (128)	//最大音效数量
#define MIN_SOUND_DELAY (50)	//相同音效播放的最小时间间隔，单位为毫秒

struct XSoundInfo
{
	void * sound;	//Mix_Chunk
	XResourceInfo *resInfo;
	int soundTime;						//这个成员用于控制音效播放的间隔，防止同一个音效在短时间内不断地播放

	XBool isSlowDownInit;
	XBool isEnableSlowDown;
	unsigned char *slowDownOldData;
	int slowDownOldLen;
	unsigned char *slowDownNewData;
	int slowDownNewLen;
	int slowDownRate;
#if AUDIO_MATHOD == 0
	int lateChannel;	//记录最近播放当前音效的声道，以便于可以控制这个音效的停止等
#endif
#if AUDIO_MATHOD == 1
	void* lateChannel;	//记录最近播放当前音效的声道，以便于可以控制这个音效的停止等
#endif
	XSoundInfo()
		:sound(NULL)
		,resInfo(NULL)
		,isSlowDownInit(XFalse)
		,isEnableSlowDown()
		,slowDownOldData(NULL)
		,slowDownNewData(NULL)
		,soundTime(-1)
#if AUDIO_MATHOD == 0
		,lateChannel(-1)
#endif
#if AUDIO_MATHOD == 1
		,lateChannel(nullptr)
#endif
	{}
};

typedef int XSndHandle;
//这个设计为单子系统
class XSound
{
public:
	XSound();
	virtual ~XSound();
protected:
	XSound(const XSound&);
	XSound &operator= (const XSound&);
private:
	XResPos m_resoursePosition;	//资源位置
private:
	std::vector<XSoundInfo *> m_sound;

	int m_soundVolume;
	XBool m_firstAddSoundFlag;
public:
	XSndHandle addSound(const char* filename,XResPos resPos = RES_SYS_DEF);					//载入一个音效，返回这个音效的句柄，用于播放
	void clearUp();											//清除掉所有载入的声音资源
	void clearOneSound(XSndHandle soundHandle);			//清除掉指定的音效资源
	XBool play(XSndHandle soundHandle,int loops = 0);	//-1为无限循环
	void setSoundVolume(int volume);						//设置当前音效播放的音量
	void setAllVolume(int volume);							//设置所有声音播放的音量

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是关于声音延长的方法，只在SDL_Mixer下有小
	XBool slowDownInit(int rate,XSndHandle handle);
	void enableSlowDown(XSndHandle handle);	//使能延长
	void disableSlowDown(XSndHandle handle);	//取消延长
	void slowDownRelease(XSndHandle handle);	//释放资源
	XBool slowDownAllInit(int rate);
	void enableAllSlowDown();	//使能延长
	void disableAllSlowDown();	//取消延长
	void slowDownAllRelease();	//释放资源
	//-------------------------------------------------------

	void pause(XSndHandle s);
	void resume(XSndHandle s);
	XBool isPause(XSndHandle s);
	void stop(XSndHandle s);
	XBool isEnd(XSndHandle s);
	XBool fadeInSound(XSndHandle s,int loops,int ms);
	void fadeOutSound(XSndHandle s, int ms);
	void release();	//注意这里这个类不会自动在析构函数里面调用释放函数，所以一定要记得主动调用释放函数，自动调用的话主要是怕重复调用
	static bool getIsInvalid(XSndHandle hangle){return hangle < 0;} 
};
inline void XSound::setSoundVolume(int volume)
{
	if(volume < 0) volume = 0;
	if(volume > 128) volume = 128;
	m_soundVolume = volume;
}
}
#endif