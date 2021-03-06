#include "XStdHead.h"
#if AUDIO_MATHOD == 1
#include "XSoundFmod.h"
namespace XE{
bool XSoundFmod::init()
{
	FMOD_System_Create(&m_sys);
#ifdef XEE_OS_LINUX
	FMOD_System_SetOutput(sys,FMOD_OUTPUTTYPE_ALSA);
#endif
	unsigned int version;
	if(FMOD_System_GetVersion(m_sys, &version) != FMOD_OK) return false;

    if(version < FMOD_VERSION) return false;
	FMOD_System_Init(m_sys,32,FMOD_INIT_NORMAL,NULL);  //do we want just 32 channels?
	FMOD_System_GetMasterChannelGroup(m_sys,&m_channelGroup);
	return true;
}
void XSoundFmod::update(float stepTime)
{
	FMOD_System_Update(m_sys);
	if(m_musicState == SOUND_STATE_FADEIN)
	{//music淡入
		m_musicTimer += stepTime;
		if(m_musicTimer >= m_musicFadeTime)
		{//动作完成
			m_musicState = SOUND_STATE_NORMAL;
			setMusicVolume(m_curMusicVolume);
		}else
		{
			setMusicVolume((float)m_musicTimer/(float)m_musicFadeTime * (float)m_curMusicVolume);
		}
	}
	if(m_musicState == SOUND_STATE_FADEOUT)
	{//music淡出
		m_musicTimer += stepTime;
		if(m_musicTimer >= m_musicFadeTime)
		{//动作完成
			m_musicState = SOUND_STATE_NORMAL;
			setMusicVolume(m_curMusicVolume);
			haltMusic();
		}else
		{
			setMusicVolume((1.0f - m_musicTimer/(float)m_musicFadeTime) * (float)m_curMusicVolume);
		}
	}
	for(unsigned int i = 0;i < m_soundFadeList.size();++ i)
	{
		XSoundFadeData &tmp = m_soundFadeList[i];
		if(tmp.state == SOUND_STATE_FADEIN)
		{//music淡入
			tmp.soundTimer += stepTime;
			if(tmp.soundTimer >= tmp.soundFadeTime)
			{//动作完成
				tmp.state = SOUND_STATE_NORMAL;
				setVolume(tmp.channel,tmp.curSoundVolume);
				//清除队列
				//for(int j = i;j < (int)(m_soundFadeList.size()) - 1;++ j)
				//{
				//	m_soundFadeList[j] = m_soundFadeList[j + 1];
				//}
				//m_soundFadeList.pop_back();
				m_soundFadeList.erase(m_soundFadeList.begin() + i);
			}else
			{
				setVolume(tmp.channel,
					(float)tmp.soundTimer/(float)tmp.soundFadeTime * (float)tmp.curSoundVolume);
			}
		}
		if(tmp.state == SOUND_STATE_FADEOUT)
		{//music淡出
			tmp.soundTimer += stepTime;
			if(tmp.soundTimer >= tmp.soundFadeTime)
			{//动作完成
				tmp.state = SOUND_STATE_NORMAL;
				setVolume(tmp.channel,tmp.curSoundVolume);
				haltSound(tmp.channel);
				//清除队列
				//for(int j = i;j < (int)(m_soundFadeList.size()) - 1;++ j)
				//{
				//	m_soundFadeList[j] = m_soundFadeList[j + 1];
				//}
				//m_soundFadeList.pop_back();
				m_soundFadeList.erase(m_soundFadeList.begin() + i);
			}else
			{
				setVolume(tmp.channel,
					(1.0f - (float)tmp.soundTimer/(float)tmp.soundFadeTime) * (float)tmp.curSoundVolume);
			}
		}
	}
}
void* XSoundFmod::musicFadeIn(void * p,int loop,int ms) 
{
	if(m_musicState != SOUND_STATE_NORMAL) return nullptr;
	if(playMusic(p,loop) == nullptr) return nullptr;
	m_musicState = SOUND_STATE_FADEIN;
	m_musicTimer = 0;
	m_curMusicVolume = getMusicVolume();
	setMusicVolume(0);
	m_musicFadeTime = ms;
	return m_musicChannel;
}
int XSoundFmod::musicFadeOut(int ms) 
{
	if(m_musicState != SOUND_STATE_NORMAL) return 0;
	m_musicState = SOUND_STATE_FADEOUT;
	m_musicTimer = 0;
	m_curMusicVolume = getMusicVolume();
	m_musicFadeTime = ms;
	return 1;
}
void* XSoundFmod::soundFadeIn(void * p,int loop,int ms) 
{
	XSoundFadeData tmp;
	tmp.state = SOUND_STATE_FADEIN;
	tmp.soundTimer = 0;
	tmp.soundFadeTime = ms;
	tmp.channel = playSound(p,loop);
	if(tmp.channel < 0) return tmp.channel;
	tmp.curSoundVolume = getVolume(tmp.channel);
	setVolume(tmp.channel,0);
	m_soundFadeList.push_back(tmp);
	return tmp.channel;
}
int XSoundFmod::soundFadeOut(void* c,int ms)
{
	for(unsigned int i = 0;i < m_soundFadeList.size();++ i)
	{
		if(m_soundFadeList[i].channel == c && 
			m_soundFadeList[i].state != SOUND_STATE_NORMAL) return 0;
	}
	XSoundFadeData tmp;
	tmp.state = SOUND_STATE_FADEOUT;
	tmp.soundTimer = 0;
	tmp.soundFadeTime = ms;
	tmp.channel = c;
	tmp.curSoundVolume = getVolume(tmp.channel);
	m_soundFadeList.push_back(tmp);
	return 1;
}
int sum = 0;
//FMOD_RESULT F_CALLBACK pcmreadcallback(FMOD_SOUND *sound, void *data, unsigned int datalen)
FMOD_RESULT F_CALLBACK pcmreadcallback(FMOD_SOUND *,void *data,unsigned int datalen)
{
//	printf("%d,%d\n",datalen,sum += datalen);
//	if(XSoundFmod::GetInstance().m_isCallBackOpen)
	if(gFrameworkData.pSoundCore != NULL)
		((XSoundFmod *)gFrameworkData.pSoundCore)->m_cbFun(((XSoundFmod *)gFrameworkData.pSoundCore)->m_cbData,(unsigned char *)data,datalen);
    return FMOD_OK;
}
void XSoundFmod::setCallBack(SND_CALLBACK_FUN fun, void *arg) 
{//非NULL时候开启，NULL的时候关闭
	if(fun == NULL)
	{//关闭
		if(m_isCallBackOpen)
		{//关闭
			FMOD_Channel_Stop(m_cbChannel);
			FMOD_Sound_Release(m_cbSound);
			m_isCallBackOpen = false;
		}
	}
	else
	{//开启
		if (m_isCallBackOpen)
		{//关闭
			FMOD_Channel_Stop(m_cbChannel);
			FMOD_Sound_Release(m_cbSound);
			m_isCallBackOpen = false;
		}
		//开启
		m_cbFun = fun;
		m_cbData = arg;
		FMOD_CREATESOUNDEXINFO exinfo;
		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.decodebuffersize = 4096;//XEE::audioSampleRate;	//缓存区大小
		if (gFrameworkData.pFrameWork != NULL)
		{
			exinfo.length = gFrameworkData.pFrameWork->getAudioSampleRate() * gFrameworkData.pFrameWork->getAudioChannelSum() * sizeof(signed short) * 5;
			exinfo.numchannels = gFrameworkData.pFrameWork->getAudioChannelSum();
			exinfo.defaultfrequency = gFrameworkData.pFrameWork->getAudioSampleRate();
		}
		else
		{
			assert(0);
		}
		exinfo.format = FMOD_SOUND_FORMAT_PCM16;//XEE::audioFormat
		exinfo.userdata = arg;
		exinfo.pcmreadcallback = pcmreadcallback;
		exinfo.pcmsetposcallback = NULL;
		FMOD_RESULT result = FMOD_System_CreateSound(m_sys,
			NULL,FMOD_2D | FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_OPENUSER | FMOD_CREATESTREAM,&exinfo,&m_cbSound);	//这个函数需要缓冲满了才返回，这里有问题。
			//NULL,FMOD_2D | FMOD_LOOP_NORMAL | FMOD_OPENUSER,&exinfo,&m_cbSound);
		if(result != FMOD_OK) return;
		result = FMOD_System_PlaySound(m_sys,FMOD_CHANNEL_FREE,m_cbSound,0,&m_cbChannel);
		if(result != FMOD_OK) return;
		m_isCallBackOpen = true;
	}
}	//尚未实现
void XSoundFmod::setSpeed(void* c, float spd) { FMOD_Channel_SetFrequency((FMOD_CHANNEL *)c, XEG.getAudioSampleRate() * spd); }
}
#endif