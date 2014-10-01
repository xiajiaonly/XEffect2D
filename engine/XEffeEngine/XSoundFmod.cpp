#include "XSoundFmod.h"

bool _XSoundFmod::init()
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
void _XSoundFmod::update(int stepTime)
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
	for(int i = 0;i < m_soundFadeList.size();++ i)
	{
		_XSoundFadeData &tmp = m_soundFadeList[i];
		if(tmp.state == SOUND_STATE_FADEIN)
		{//music淡入
			tmp.soundTimer += stepTime;
			if(tmp.soundTimer >= tmp.soundFadeTime)
			{//动作完成
				tmp.state = SOUND_STATE_NORMAL;
				setVolume(tmp.channel,tmp.curSoundVolume);
				//清除队列
				for(int j = i;j < m_soundFadeList.size() - 1;++ j)
				{
					m_soundFadeList[j] = m_soundFadeList[j + 1];
				}
				m_soundFadeList.pop_back();
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
				for(int j = i;j < m_soundFadeList.size() - 1;++ j)
				{
					m_soundFadeList[j] = m_soundFadeList[j + 1];
				}
				m_soundFadeList.pop_back();
			}else
			{
				setVolume(tmp.channel,
					(1.0f - (float)tmp.soundTimer/(float)tmp.soundFadeTime) * (float)tmp.curSoundVolume);
			}
		}
	}
}
int _XSoundFmod::musicFadeIn(void * p,int loop,int ms) 
{
	if(m_musicState != SOUND_STATE_NORMAL) return 0;
	if(playMusic(p,loop) < 0) return -1;
	m_musicState = SOUND_STATE_FADEIN;
	m_musicTimer = 0;
	m_curMusicVolume = getMusicVolume();
	setMusicVolume(0);
	m_musicFadeTime = ms;
	return (int)m_musicChannel;
}
int _XSoundFmod::musicFadeOut(int ms) 
{
	if(m_musicState != SOUND_STATE_NORMAL) return 0;
	m_musicState = SOUND_STATE_FADEOUT;
	m_musicTimer = 0;
	m_curMusicVolume = getMusicVolume();
	m_musicFadeTime = ms;
	return 1;
}
int _XSoundFmod::soundFadeIn(void * p,int loop,int ms) 
{
	_XSoundFadeData tmp;
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
int _XSoundFmod::soundFadeOut(int c,int ms) 
{
	for(int i = 0;i < m_soundFadeList.size();++ i)
	{
		if(m_soundFadeList[i].channel == c && 
			m_soundFadeList[i].state != SOUND_STATE_NORMAL) return 0;
	}
	_XSoundFadeData tmp;
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
//	if(_XSoundFmod::GetInstance().m_isCallBackOpen)
		_XSoundFmod::GetInstance().m_cbFun(_XSoundFmod::GetInstance().m_cbData,(unsigned char *)data,datalen);
    return FMOD_OK;
}
void _XSoundFmod::setCallBack(SND_CALLBACK_FUN fun, void *arg) 
{//非NULL时候开启，NULL的时候关闭
	if(fun == NULL)
	{//关闭
		if(m_isCallBackOpen)
		{//关闭
			FMOD_Channel_Stop(m_cbChannel);
			FMOD_Sound_Release(m_cbSound);
			m_isCallBackOpen = false;
		}
	}else
	{//开启
		if(m_isCallBackOpen)
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
		exinfo.decodebuffersize = 1024;//XEE::audioFrequency;	//缓存区大小
		exinfo.length = XEE::audioFrequency * XEE::audioChannel * sizeof(signed short) * 5;
		exinfo.numchannels = XEE::audioChannel;
		exinfo.defaultfrequency = XEE::audioFrequency;
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