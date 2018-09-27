#ifndef _JIA_XTEMPSAVE_
#define _JIA_XTEMPSAVE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include "..\XCommonDefine.h"
#include <deque>
namespace XE{
//这个类用于解决临时保存的功能
class XStepSave
{
private:
	bool m_isInited;
	int m_coreSize;		//一个存储空间的大小
	int m_coreSum;		//存储空间的数量
	unsigned char **m_cores;	//存储空间
	std::deque<unsigned char *> m_freeBuff;	//未使用的空间
	std::deque<unsigned char *> m_usedBuff;	//使用的空间
	//std::deque<unsigned char *>::iterator m_curIndex;	//由于是deque，这样的设计性能不是很好(需要优化)
	int m_curIndex;	//由于是deque，这样的设计性能不是很好(需要优化)
public:
	XStepSave()
		:m_isInited(false)
		,m_cores(NULL)
		,m_coreSum(0)
	{}
	~XStepSave(){release();}
	bool init(int size,int sum);	//size:一个core的大小，sum：core的数量
	bool pushACore(const void *p);	//向对立中推入一个状态
	unsigned char *getPrevCore() const;		//获取上一个状态：如果没有上一个状态则返回NULL
	void moveToPrev()
	{
		if(m_curIndex > 0) 
			--m_curIndex;
		//forTest();
	}
	unsigned char *getNextCore() const;		//获取下一个状态：如果没有下一个状态则返回NULL
	void moveToNext()
	{
		if(m_curIndex >= 0 && m_curIndex + 1 < m_usedBuff.size()) 
			++ m_curIndex;
		//forTest();
	}
	unsigned char *getCurCore() const;
	void release();
	void forTest();
	void reset() 
	{ 
		m_curIndex = -1;
		for (auto it = m_usedBuff.begin(); it != m_usedBuff.end(); ++it)
		{
			m_freeBuff.push_back(*it);
		}
		m_usedBuff.clear();
	}
	bool haveAction()const { return m_curIndex >= 0; }
};
#if WITH_INLINE_FILE
#include "XStepSave.inl"
#endif
}
#endif