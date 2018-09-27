#ifndef _JIA_XDATASPOOL_
#define _JIA_XDATASPOOL_
#include "XMemory.h"
#include <vector>
#include <deque>
namespace XE {
class XDatasPool
{
private: 
	bool m_isInited;
	std::vector<XBuffer*> m_allDatas;
	std::deque<XBuffer*> m_freeBuff;
	std::deque<XBuffer*> m_usedBuff;
public:
	XDatasPool()
		:m_isInited(false)
	{}
	~XDatasPool() { release(); }
	bool init(int maxBuffSum,int dataSize);
	void release();
	XBuffer* popAFreeData()
	{
		if (!m_isInited || m_freeBuff.size() <= 0) return nullptr;
		XBuffer* ret = m_freeBuff[0];
		m_freeBuff.pop_front();
		return ret;
	}
	void pushAFreeData(XBuffer* data)
	{
		if (!m_isInited) return;
		m_freeBuff.push_back(data);
	}
	XBuffer* popAUsedData()
	{
		if (!m_isInited || m_usedBuff.size() <= 0) return nullptr;
		XBuffer* ret = m_usedBuff[0];
		m_usedBuff.pop_front();
		return ret;
	}
	void pushAUsedData(XBuffer* data)
	{
		if (!m_isInited) return;
		m_usedBuff.push_back(data);
	}
};
}
#endif