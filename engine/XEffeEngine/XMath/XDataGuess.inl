INLINE void XCycleInfo::addAMaxValue(float maxV)
{
	float all = m_maxValue * static_cast<float>(m_maxValueSum);
	all += maxV;
	++ m_maxValueSum;
	m_maxValue = all / static_cast<float>(m_maxValueSum);
#ifdef WITH_DEBUG_INFO
	std::cout << "max:" << m_maxValue << std::endl; 
#endif
}
INLINE void XCycleInfo::addAMinValue(float minV)
{
	float all = m_minValue * static_cast<float>(m_minValueSum);
	all += minV;
	++ m_minValueSum;
	m_minValue = all / static_cast<float>(m_minValueSum);
#ifdef WITH_DEBUG_INFO
	std::cout << "min:" << m_minValue << std::endl;
#endif
}
INLINE void XCycleInfo::addAUpTime(float upTime)
{
	float all = m_upTime * static_cast<float>(m_upTimeSum);
	all += upTime;
	++ m_upTimeSum;
	m_upTime = all / static_cast<float>(m_upTimeSum);
	if(m_downTime != 0.0f)
		m_upDownRate = m_upTime / m_downTime;
#ifdef WITH_DEBUG_INFO
	std::cout << "upTime:" << m_upTime << std::endl; 
#endif
}
INLINE void XCycleInfo::addADownTime(float upTime)
{
	float all = m_downTime * static_cast<float>(m_downTimeSum);
	all += upTime;
	++ m_downTimeSum;
	m_downTime = all / static_cast<float>(m_downTimeSum);
	if(m_downTime != 0.0f)
		m_upDownRate = m_upTime / m_downTime;
#ifdef WITH_DEBUG_INFO
	std::cout << "downTime:" << m_downTime << std::endl;
#endif
}
INLINE void XCycleInfo::reset()
{
	m_minValue = 0.0f;
	m_minValueSum = 0;
	m_maxValue = 0.0f;
	m_maxValueSum = 0;
	m_upTime = 0.0f;
	m_upTimeSum = 0;
	m_downTime = 0.0f;
	m_downTimeSum = 0;
	m_cycleTime = 0.0f;
}
INLINE void XCycleData::clear()
{
	m_datas.clear();
	m_haveStart = false;
	m_haveEnd = false;
	m_haveTop = false;
	m_downDatasSum = 0;
	m_upDatasSum = 0;
	m_state = CYC_STA_NULL;
}
INLINE bool XDataGuess::getIsCycDataUpdated()
{
	if(!m_isCycDataUpdate) return false;
	m_isCycDataUpdate = false;
	return true;
}