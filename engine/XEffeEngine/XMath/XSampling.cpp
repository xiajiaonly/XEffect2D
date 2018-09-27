#include "XStdHead.h"
#include "XSampling.h"
namespace XE{
	void XSamplingFilter::popAIDn()
	{
		int id = m_ids[0];
		m_ids.pop_front();
		for(int i = 0;i < m_sampleIDs.size();++ i)
		{
			if (m_sampleIDs[i] != id) continue;
			if(m_idSum[i] > 1)
			{
				-- m_idSum[i];
			}else
			{//计数为零，取消无效的
				m_sampleIDs.erase(m_sampleIDs.begin() + i);
				m_idSum.erase(m_idSum.begin() + i);
			}
			break;
		}
	}
	void XSamplingFilter::updateID()	//更新ID
	{
		int curMaxSum = 0;
		m_curID = -1;
		if (m_isIgnoreInvalidID)
		{
			for (int i = 0; i < m_idSum.size(); ++i)
			{
				if (m_idSum[i] >= m_threshold && m_idSum[i] > curMaxSum && m_sampleIDs[i] >= 0)
				{
					curMaxSum = m_idSum[i];
					m_curID = m_sampleIDs[i];
				}
			}
		}
		else
		{
			for (int i = 0; i < m_idSum.size(); ++i)
			{
				if (m_idSum[i] >= m_threshold && m_idSum[i] > curMaxSum)
				{
					curMaxSum = m_idSum[i];
					m_curID = m_sampleIDs[i];
				}
			}
		}
	}
	void XSamplingFilter::pushAID(int id)	//推入一个采样数据
	{
		m_ids.push_back(id);	//推入一个数据
		int index = -1;
		for(int i = 0;i < m_sampleIDs.size();++ i)
		{
			if (m_sampleIDs[i] != id) continue;
			index = i;
			break;
		}
		if(index >= 0)
		{//ID已经存在
			++ m_idSum[index];
		}else
		{//新的ID
			m_sampleIDs.push_back(id);
			m_idSum.push_back(1);
		}
		if(m_ids.size() > m_size) popAIDn();
		updateID();
		//for(auto it = m_ids.begin();it != m_ids.end();++ it)
		//{
		//	printf("%d,",*it);
		//}
		//printf("\n");
	}
	void XSamplingFilter::setSize(int size)
	{
		m_size = size;
		if (m_ids.size() <= m_size) return;
		while(m_ids.size() <= m_size)
		{
			popAIDn();
		}
		updateID();
	}
}