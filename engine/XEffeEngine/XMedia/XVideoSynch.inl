INLINE int XBroadcastUdp::getTimer()	//获取毫秒级的时间
{
	int ret = 0;
	m_mutex.Lock();
	ret = m_delayTimer + m_timer.getElapsedTimeInMilliSec();
	m_mutex.Unlock();
	return ret;
}
INLINE void XBroadcastUdp::release()
{
	if(!m_isInited) return;
	waitThreadEnd(m_recvThreadState);
	XMem::XDELETE_ARRAY(m_data);
	XMem::XDELETE_ARRAY(m_dataSend);
	closesocket(m_sendSocket);
	closesocket(m_recvSocket);
	m_dataLen = 0;
	m_isInited = false;
}
INLINE XBool XSimpleVideoSynch::init(const std::string& filename)
{
	if(m_isInited) return XFalse;
	if(!m_udp.init(filename)) return XFalse;
	m_udp.setCallbackFun(this,commandFun);
	m_offset.set(0.0f);
	m_size.set(0.0f);
	m_isInited = XTrue;
	return XTrue;
}
INLINE XBool XSimpleVideoSynch::load(const char *filename,XColorMode mode,bool withVoice) 
{
	if(m_udp.getRole() != VS_ROLE_SERVER) return XFalse;
	m_movie.closeClip();
	m_udp.setCurVideo(filename);
	m_udp.setCurFrame(STATE_LOAD);	//发送预加载命令
	if(!m_movie.load(filename,mode,withVoice))
	{
		m_udp.setCurFrame(STATE_CLOSE_CLIP);	//发送预加载命令
		return XFalse;
	}
	m_movie.m_movieSprite->setClipRect(m_offset.x,m_offset.y,m_offset.x + m_size.x,m_offset.y + m_size.y);
	return XTrue;
}
