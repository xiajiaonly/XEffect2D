INLINE void XMoveData::set(float start,float end,float speed,XMoveDataMode mode,int isLoop)
{
	m_isEnd = XFalse;
	m_startData = start;
	m_endData = end;
	m_speed = speed;
	m_curData = start;
	m_timer = 0.0f;
	m_mode = mode;
	m_isLoop = isLoop;
	if(isLoop >= 1) m_loopTimer = isLoop;
	else m_loopTimer = -1;	//无限循环
}
INLINE void XMoveData::reset()	//设置重新开始
{
	m_curData = m_startData;
	m_timer = 0.0f;
	m_isEnd = XFalse;
	if(m_isLoop >= 1) m_loopTimer = m_isLoop;
	else m_loopTimer = -1;	//无限循环
}
INLINE void XMoveData::setEnd()
{
	m_isEnd = XTrue;
	m_curData = m_endData;
}
INLINE void XMoveData::getParamStr(char * str)
{
	if(str == NULL) return;
	sprintf(str,"%f,%f,%f,%d,%d,",m_startData,m_endData,m_speed,m_mode,m_isLoop);
}