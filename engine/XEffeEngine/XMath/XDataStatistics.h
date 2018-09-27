#ifndef _JIA_XDATASTATISTICS_
#define _JIA_XDATASTATISTICS_
//这个类用于统计无限的连续数据的当前特征值
//比如说用于判断帧率或者数据流的特征
#include <deque>
namespace XE {
class XDataStatistics
{
private:
	std::deque<double> m_datas;
	double m_allDataAdd;
	int m_allSataSums;
	int m_dataDepth;
	double m_allMin;
	double m_allMax;
public:
	XDataStatistics()
		:m_dataDepth(30)
		, m_allSataSums(0)
		, m_allDataAdd(0.0)
		, m_allMin(0.0)
		, m_allMax(0.0)
	{}
	bool init(int depth);
	void pushAData(double data);
	void reset();
	void setDataDepth(int depth);
	int getAllDataSum()const { return m_allSataSums; }
	float getMinData()const { return m_allMin; }
	float getMaxData()const { return m_allMax; }
	float getAvgData()const
	{
		if (m_allSataSums <= 0) return 0.0f;
		return m_allDataAdd / double(m_allSataSums);
	}
	float getCurAvg()const
	{//可以优化
		if (m_datas.size() == 0) return 0.0f;
		double all = 0.0;
		for (auto it = m_datas.begin(); it != m_datas.end(); ++it)
		{
			all += *it;
		}
		return all / double(m_datas.size());
	}
	float getCurMin()const
	{
		if (m_datas.size() == 0) return 0.0f;
		double minData = m_datas[0];
		for (auto it = m_datas.begin() + 1; it != m_datas.end(); ++it)
		{
			if (minData > *it) minData = *it;
		}
		return minData;
	}
	float getCurMax()const
	{
		if (m_datas.size() == 0) return 0.0f;
		double maxData = m_datas[0];
		for (auto it = m_datas.begin() + 1; it != m_datas.end(); ++it)
		{
			if (maxData < *it) maxData = *it;
		}
		return maxData;
	}
};
}
#endif