INLINE XBool XTimeLines::addKeyFrame(const std::string& name,int time,int timeLen,XFrameDataBasic * data,XMoveDataMode mode)
{
	int index = getTimeLineIndexByName(name);
	if(index < 0) return XFalse;
	m_timeLines[index]->addAKeyFrame(time,timeLen,data,mode);
	return XTrue;
}
//向指定的时间线上增加关键帧
INLINE XBool XTimeLines::addKeyFrame(unsigned int index,int time,int timeLen,XFrameDataBasic * data,XMoveDataMode mode)
{
	if(index >= m_timeLines.size()) return XFalse;
	m_timeLines[index]->addAKeyFrame(time,timeLen,data,mode);
	return XTrue;
}
INLINE XBool XTimeLines::delCurKeyFrame(const std::string& name)
{
	int index = getTimeLineIndexByName(name);
	if(index < 0) return XFalse;
	m_timeLines[index]->delAKeyFrame(m_curTimer);
	return XTrue;
}
INLINE XBool XTimeLines::delCurKeyFrame(unsigned int index)
{
	if(index >= m_timeLines.size()) return XFalse;
	m_timeLines[index]->delAKeyFrame(m_curTimer);
	return XTrue;
}
INLINE XBool XTimeLines::clearAllFroms(const std::string& name)
{
	int index = getTimeLineIndexByName(name);
	if(index < 0) return XFalse;
	m_timeLines[index]->clearAllFrame();
	return XTrue;
}
INLINE XBool XTimeLines::clearAllFroms(unsigned int index)
{
	if(index >= m_timeLines.size()) return XFalse;
	m_timeLines[index]->clearAllFrame();
	return XTrue;
}
INLINE void XTimeLines::clearAllFroms()
{
	for(unsigned int i = 0;i < m_timeLines.size();++ i)
	{
		m_timeLines[i]->clearAllFrame();
	}
}
INLINE const XOneFrame *XTimeLines::getCurKeyFrame(const std::string& name)	//获取当前所在的关键帧
{
	int index = getTimeLineIndexByName(name);
	if(index < 0) return NULL;
	return m_timeLines[index]->getCurKeyFrame(m_curTimer);
}
INLINE const XOneFrame *XTimeLines::getNextKeyFrame(const std::string& name)	//获取下一个关键帧的指针
{
	int index = getTimeLineIndexByName(name);
	if(index < 0) return NULL;
	return m_timeLines[index]->getNextKeyFrame(m_curTimer);
}
INLINE const XOneFrame *XTimeLines::getCurKeyFrame(unsigned int index)		//获取当前所在的关键帧
{
	if(index >= m_timeLines.size()) return NULL;
	return m_timeLines[index]->getCurKeyFrame(m_curTimer);
}
INLINE const XOneFrame *XTimeLines::getNextKeyFrame(unsigned int index)	//获取下一个关键帧的指针
{
	if(index >= m_timeLines.size()) return NULL;
	return m_timeLines[index]->getNextKeyFrame(m_curTimer);
}
INLINE void XTimeLines::resetAllTimeLineFrameFlag()
{//重置所有的时间帧
	for(unsigned int  i = 0;i < m_timeLines.size();++ i)
	{
		m_timeLines[i]->resetAllFrameFlag();
	}
}
INLINE void XTimeLines::setAllTimeLineFrameFlag()
{//设置所有的时间帧
	for(unsigned int  i = 0;i < m_timeLines.size();++ i)
	{
		m_timeLines[i]->setAllFrameFlag();
	}
}
INLINE void XOneTimeLine::setPosition(const XVec2& p)
{
	m_position = p;
	m_title.setPosition(m_position);
}
INLINE void XOneTimeLine::setScale(const XVec2& s)
{
	m_scale = s;
	m_title.setScale(m_scale);
}