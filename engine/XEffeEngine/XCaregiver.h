#ifndef _JIA_XCAREGIVER_
#define _JIA_XCAREGIVER_
#include "XBabyBase.h"
#include <vector>
//这一层概念好像变得多余，最为baby应该有自身的约束
//这个设计主要是为了解决网络或者设备打开之后，由于外部原因造成一段时间设备故障之后，
//程序能重新打开设备或者链接测试被做出相应的提示
#include "XTimer.h"
namespace XE{
struct XBabyInfo
{
	XBabyBase *pBaby;
	XSimpleTimer timer;
};
class XCaregiver
{
private:
	std::vector<XBabyInfo> m_babys;	//所以被保护对象的列表
public:
	bool addABaby(XBabyBase *baby, int resetTimer = 1000)
	{
		if (getBabyIndex(baby) >= 0) return false;
		XBabyInfo tmp;
		tmp.pBaby = baby;
		tmp.timer.setStepTime(resetTimer);
		m_babys.push_back(tmp);
		return true;
	}
	int getBabyIndex(XBabyBase *baby)	//获取指定baby的编号，如果不存在则返回-1
	{
		for (int i = 0; i < m_babys.size(); ++i)
		{
			if (m_babys[i].pBaby == baby) return i;
		}
		return -1;
	}
	void releaseAllBaby() { m_babys.clear(); }		//释放所有的照顾对象
	bool freeABaby(XBabyBase *baby)	//释放一个baby的管理
	{
		for (int i = 0; i < m_babys.size(); ++i)
		{
			if (m_babys[i].pBaby == baby)
			{
				m_babys.erase(m_babys.begin() + i);
				return true;
			}
		}
		return false;
	}
	void update(float stepTime)		//更新所有baby的状态
	{//下面根据被照顾对象的状态来照顾baby
		if (m_babys.size() <= 0) return;
		for (int i = 0; i < m_babys.size(); ++i)
		{
			if (m_babys[i].pBaby->needReset() && m_babys[i].timer.update(stepTime))
			{
				m_babys[i].pBaby->reset();
				m_babys[i].timer.reset();
			}
		}
	}
};
}
#endif