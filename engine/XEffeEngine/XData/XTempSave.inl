INLINE unsigned char *XTempSave::getPrevCore()	const	//获取上一个状态：如果没有上一个状态则返回NULL
{
	if(m_curIndex <= 0) return NULL;	//没有之前的东西
	return m_usedBuff[m_curIndex - 1];
}
INLINE unsigned char *XTempSave::getNextCore()	const	//获取下一个状态：如果没有下一个状态则返回NULL
{
	if(m_curIndex < 0 || m_curIndex + 1 >= m_usedBuff.size()) return NULL;
	return m_usedBuff[m_curIndex + 1];
}
INLINE unsigned char *XTempSave::getCurCore() const
{
	if(m_curIndex < 0) return NULL;
	return m_usedBuff[m_curIndex];
}