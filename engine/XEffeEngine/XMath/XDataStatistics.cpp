#include "XStdHead.h"
#include "XDataStatistics.h"
namespace XE {
bool XDataStatistics::init(int depth)
{
	if (depth <= 0) false;
	setDataDepth(depth);
	reset();
	return true;
}
void XDataStatistics::pushAData(double data)
{
	m_datas.push_back(data);
	if (m_datas.size() > m_dataDepth)
		m_datas.pop_front();
	++m_allSataSums;
	m_allDataAdd += data;
	if (m_datas.size() == 1)
	{
		m_allMin = m_allMax = data;
	}
	else
	{
		if (m_allMin > data) m_allMin = data;
		if (m_allMax < data) m_allMax = data;
	}
}
void XDataStatistics::reset()
{
	m_allSataSums = 0;
	m_allDataAdd = 0.0;
	m_datas.clear();
	m_allMin = 0.0;
	m_allMax = 0.0f;
}
void XDataStatistics::setDataDepth(int depth)
{
	if (depth <= 0 || depth == m_dataDepth) return;
	m_dataDepth = depth;
	if (m_datas.size() > m_dataDepth)
		m_datas.erase(m_datas.begin(), m_datas.begin() + (m_datas.size() - m_dataDepth));
}
}