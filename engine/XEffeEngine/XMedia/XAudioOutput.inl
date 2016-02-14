INLINE void XAudioOutput::release()
{
	stop();
	XMem::XDELETE_ARRAY(m_audioBuff);
	XMem::XDELETE_ARRAY(m_tmpBuffer);
	if(m_pSwrContext != NULL) 
		swr_free(&m_pSwrContext);
}
INLINE bool XAudioOutput::start()	//开始回放
{
	if(m_isStart) return false;	//不能重复开始
	//这里注册回调函数
	XCurSndCore.setCallBack((SND_CALLBACK_FUN)audioCallBack,this);
	m_isStart = true;
	return true;
}
INLINE bool XAudioOutput::stop()	//停止回放
{
	if(!m_isStart) return false;
	//注销回调函数
	XCurSndCore.setCallBack(NULL,NULL);
	m_curUsage = 0;
	m_isStart = false;
	return true;
}
INLINE float XAudioOutput::getCurCanPlayTime()const//返回当前缓存中的数据可以在播放多长的时间
{//单位秒
	return m_curUsage / (XEG.getAudioSampleRate() * XEG.getAudioChannelSum() * 2.0f);
}	