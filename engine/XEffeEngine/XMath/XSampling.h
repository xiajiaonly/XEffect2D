#ifndef _JIA_XSAMPLING_
#define _JIA_XSAMPLING_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
//采样滤波，一个队列，根据队列中出现的样本的统计阈值和最多采样样本来进行抽象，以便于数据的稳定性
#include <deque>
#include <vector>
namespace XE{
class XSamplingFilter
{
private:
	int m_size;	//采样队列的深度
	int m_threshold;	//最少的使能标准
	std::deque<int> m_ids;	//所有识别的ID的队列
	std::vector<int> m_sampleIDs;	//目前拥有的ID
	std::vector<int> m_idSum;	//ID的统计数据
	int m_curID;	//当前的ID，默认-1为无效的ID
	bool m_isIgnoreInvalidID;	//是否忽略无效的ID，如果为true，则无效的ID不参与计算
	void updateID();	//更新ID
	void popAIDn();
public:
	XSamplingFilter()
		:m_curID(-1)
		,m_size(10)
		,m_threshold(5)
	{}
	void clear()	//清空当前的统计数据
	{
		m_ids.clear();
		m_sampleIDs.clear();
		m_idSum.clear();
	}
	void popAID()
	{
		popAIDn();
		updateID();
	}
	const std::deque<int> &getIds()const{return m_ids;}
	void pushAID(int id);	//推入一个采样数据
	int getCurID()const{return m_curID;}	//获取当前的统计ID
	bool init(int size,int threshold)
	{
		if(size <= 0) return false;
		if(threshold < 1) return false;
		setSize(size);
		setThreshold(threshold);
		return true;
	}
	void setSize(int size);
	void setThreshold(int threshold)
	{
		m_threshold = threshold;
		updateID();
	}
};
}
#endif