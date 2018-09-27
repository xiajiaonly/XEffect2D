#include "XStdHead.h"
#include "XDatasPool.h"
namespace XE {
bool XDatasPool::init(int maxBuffSum, int dataSize)
{
	if (m_isInited || maxBuffSum <= 0 || dataSize <= 0) return false;

	//为了效率而定义成一个整体，但是不便于扩展，
	m_allDatas.resize(maxBuffSum);
	for (int i = 0; i < m_allDatas.size(); ++i)
	{
		m_allDatas[i] = XMem::createMem<XBuffer>();
		if (m_allDatas[i] == nullptr) return false;
		m_allDatas[i]->fitSize(dataSize);
		m_freeBuff.push_back(m_allDatas[i]);
	}

	m_isInited = true;
	return false;
}
void XDatasPool::release()
{
	if (!m_isInited) return;
	m_isInited = false;
	for (int i = 0; i < m_allDatas.size(); ++i)
	{
		XMem::XDELETE(m_allDatas[i]);
	}
	m_freeBuff.clear();
	m_usedBuff.clear();
}
}