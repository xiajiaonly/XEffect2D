#include "XStdHead.h"
#include "XMWFilter.h"
namespace XE {
void XMWFilter::setWindowSize(int size)
{
	if (m_windowSize == size || size <= 0) return;
	if (m_windowSize < size || m_datas.size() <= size)
	{
		m_windowSize = size;
		return;
	}
	for (int i = 0; m_datas.size() > size; ++i)
	{
		m_totalValue -= m_datas[0];
		m_datas.pop_front();
	}
	m_windowSize = size;
}
void XMWFilter::pushAData(float data)
{
	m_totalValue += data;
	m_datas.push_back(data);
	for (int i = 0; m_datas.size() > m_windowSize; ++i)
	{
		m_totalValue -= m_datas[0];
		m_datas.pop_front();
	}
	m_curValue = m_totalValue / m_datas.size();
}
}