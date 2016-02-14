#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XSound.h"
//#include "SDL.h"

#include "XResourcePack.h"
#include "XTimer.h"
namespace XE{
void channelFinished(int channel)
{//当某个通道的音效播放完成的时候会调用这个函数
	printf("%d finished!\n",channel);
}
//void effectFunc(int chan,void *stream,int len,void *udata)
void effectFunc(int,void *stream,int len,void *)
{//下面在这里进行转码(现在这里假设实现一个慢播放的功能):会crash，还不知道怎么弄呢。
	if (gFrameworkData.pFrameWork == NULL) return;
	SDL_AudioCVT  wav_cvt;
	int ret = SDL_BuildAudioCVT(&wav_cvt,
		gFrameworkData.pFrameWork->getAudioFormat(), gFrameworkData.pFrameWork->getAudioChannelSum(), 
		gFrameworkData.pFrameWork->getAudioSampleRate(),
		gFrameworkData.pFrameWork->getAudioFormat(), gFrameworkData.pFrameWork->getAudioChannelSum(), 
		gFrameworkData.pFrameWork->getAudioSampleRate() * 2);
	if(ret == -1) printf("Couldn't build converter!\n");
	wav_cvt.buf = (Uint8 *)malloc(len * wav_cvt.len_mult);
	if(wav_cvt.buf == NULL) return;
	wav_cvt.len = len;
	memcpy(wav_cvt.buf,stream,wav_cvt.len);
	SDL_ConvertAudio(&wav_cvt);
	SDL_MixAudio((Uint8 *)stream,wav_cvt.buf, wav_cvt.len_cvt,128);
	free(wav_cvt.buf);
}
XSound::XSound()
	:m_firstAddSoundFlag(XFalse)
	,m_resoursePosition(RESOURCE_AUTO)
{
	m_sound.clear();
//	Mix_ChannelFinished(channelFinished);
}
XSound::~XSound()
{
	clearUp();
}
XSndHandle XSound::addSound(const char* filename,XResourcePosition resoursePosition)
{
	if(!m_firstAddSoundFlag)
	{//这里相当于是初始化
		m_soundVolume = XCurSndCore.getVolume(0);
		m_firstAddSoundFlag = XTrue;
	}
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = getDefResPos();
	m_resoursePosition = resoursePosition;
	XSoundInfo *temp = XMem::createMem<XSoundInfo>();
	if(temp == NULL) return -1;
	temp->resInfo = XResManager.loadResource(filename,RESOURCE_TYPE_SOUND,m_resoursePosition);
	if(temp->resInfo == NULL)
	{
		XMem::XDELETE(temp);
		return -1;
	}
	temp->sound = ((XResourceSound *)temp->resInfo->m_pointer)->m_handle;
	m_sound.push_back(temp);
	return (int)(m_sound.size()) - 1;
}
void XSound::clearUp()
{//资源被资源管理器管理之后，自身不能释放资源
	for(unsigned int i = 0;i < m_sound.size();++ i)
	{
		if(m_sound[i] != NULL && m_sound[i]->resInfo != NULL)
		{
			XResManager.releaseResource(m_sound[i]->resInfo);
			m_sound[i]->resInfo = NULL;
			XMem::XDELETE(m_sound[i]);
		}
	}
	m_sound.clear();
}
void XSound::clearOneSound(XSndHandle soundHandle)
{
	if(soundHandle < 0 || soundHandle >= m_sound.size()
		|| m_sound[soundHandle] == NULL) return;
	XSoundInfo *temp = m_sound[soundHandle];
	if(temp->resInfo != NULL)
	{
		XResManager.releaseResource(temp->resInfo);	//尚未实现
		temp->resInfo = NULL;
		XMem::XDELETE(m_sound[soundHandle]);
	}
}
XBool XSound::play(XSndHandle soundHandle,int loops)
{
	if(soundHandle < 0 || soundHandle >= m_sound.size()
		|| m_sound[soundHandle] == NULL || m_sound[soundHandle]->sound == NULL) return XFalse;
	XSoundInfo *temp = m_sound[soundHandle];
	if(temp->soundTime == -1 || XTime::getCurrentTicks() - temp->soundTime >= MIN_SOUND_DELAY)
	{
		temp->soundTime = XTime::getCurrentTicks();
#ifdef DEBUG_MODE
		printf("sound play %d\n",soundHandle);
#endif
		int channel = XCurSndCore.playSound(temp->sound,loops);
		XCurSndCore.setVolume(channel,m_soundVolume);
		temp->lateChannel = channel;
	//	Mix_RegisterEffect(channel,effectFunc,NULL,this);
		return XTrue;
	}
	return XFalse;
}
XBool XSound::slowDownInit(int rate,XSndHandle handle)
{
	if(rate <= 0 || rate >= 10) return XFalse;
	if(handle < 0 || handle >= m_sound.size()
		|| m_sound[handle] == NULL) return XFalse;
	XSoundInfo *temp = m_sound[handle];

	if(temp->isSlowDownInit) return XFalse;	//如果已经放慢过，需要重新放慢才能播放
	if(temp->sound == NULL) return XFalse;
	//记录原有数据
	temp->slowDownOldData = XCurSndCore.getData(temp->sound);
	temp->slowDownOldLen = XCurSndCore.getDataLen(temp->sound);
	//产生新的数据
	temp->slowDownNewLen = temp->slowDownOldLen * rate;
	temp->slowDownNewData = XMem::createArrayMem<unsigned char>(temp->slowDownNewLen);
	temp->slowDownRate = rate;
	//产生新的数据(这个数据是针对于双声道,16bits的)
	for(int i = 0;i < temp->slowDownOldLen >> 2;++ i)
	{
		for(int j = 0;j < 4 * rate;++ j)
		{
			temp->slowDownNewData[i * 4 * rate + j] = temp->slowDownOldData[i * 4 + (j % 4)];
		}
	}
	//记录状态
	temp->isEnableSlowDown = XFalse;
	temp->isSlowDownInit = XTrue;
	return XTrue;
}
void XSound::enableSlowDown(XSndHandle handle)	//使能延长
{
	if(handle < 0 || handle >= m_sound.size()
		|| m_sound[handle] == NULL) return;
	XSoundInfo *temp = m_sound[handle];
	if(!temp->isEnableSlowDown && temp->isSlowDownInit)
	{
		XCurSndCore.setData(temp->sound,temp->slowDownNewData);
		XCurSndCore.setDataLen(temp->sound,temp->slowDownNewLen);
		temp->isEnableSlowDown = XTrue;
	}
}
void XSound::disableSlowDown(XSndHandle handle)	//取消延长
{
	if(handle < 0 || handle >= m_sound.size()
		|| m_sound[handle] == NULL) return;
	XSoundInfo *temp = m_sound[handle];
	if(temp->isEnableSlowDown)
	{
		XCurSndCore.setData(temp->sound,temp->slowDownOldData);
		XCurSndCore.setDataLen(temp->sound,temp->slowDownOldLen);
		temp->isEnableSlowDown = XFalse;
	}
}
void XSound::slowDownRelease(XSndHandle handle)	//释放资源
{
	if(handle < 0 || handle >= m_sound.size()
		|| m_sound[handle] == NULL) return;
	XSoundInfo *temp = m_sound[handle];
	if(temp->isSlowDownInit)
	{
		if(temp->isEnableSlowDown) disableSlowDown(handle);
		XMem::XDELETE_ARRAY(temp->slowDownNewData);
		temp->isSlowDownInit = XFalse;
	}
}
XBool XSound::slowDownAllInit(int rate)
{
	if(rate <= 0 || rate >= 10) return XFalse;
	for(unsigned int i = 0;i < m_sound.size();++ i)
	{
		if(m_sound[i] == NULL) continue;
		if(m_sound[i]->isSlowDownInit)
		{
			if(m_sound[i]->slowDownRate != rate)
			{
				slowDownRelease(i);
				slowDownInit(rate,i);
			}
		}else
		{
			slowDownInit(rate,i);
		}
	}
	return XTrue;
}
void XSound::enableAllSlowDown()	//使能延长
{
	for(unsigned int i = 0;i < m_sound.size();++ i)
	{
		enableSlowDown(i);
	}
}
void XSound::disableAllSlowDown()	//取消延长
{
	for(unsigned int i = 0;i < m_sound.size();++ i)
	{
		disableSlowDown(i);
	}
}
void XSound::slowDownAllRelease()	//释放资源
{
	for(unsigned int i = 0;i < m_sound.size();++ i)
	{
		slowDownRelease(i);
	}
}
XBool XSound::fadeInSound(XSndHandle s,int loops,int ms)
{
	if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL
		|| m_sound[s]->sound == NULL) return XFalse;

	if(m_sound[s]->soundTime == -1 || XTime::getCurrentTicks() - m_sound[s]->soundTime >= MIN_SOUND_DELAY)
	{
		m_sound[s]->soundTime = XTime::getCurrentTicks();
#ifdef DEBUG_MODE
		printf("sound play %d\n",s);
#endif
		int channel = XCurSndCore.soundFadeIn(m_sound[s]->sound,loops,ms);
		XCurSndCore.setVolume(channel,m_soundVolume);
		m_sound[s]->lateChannel = channel;
	//	Mix_RegisterEffect(channel,effectFunc,NULL,this);
		return XTrue;
	}
	return XFalse;
}
}