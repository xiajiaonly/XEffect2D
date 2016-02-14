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
class XTempSave
{
private:
	bool m_isInited;
	int m_coreSize;		//一个存储空间的大小
	int m_coreSum;		//存储空间的数量
	unsigned char **m_cores;	//存储空间
	std::deque<unsigned char *> m_freeBuff;	//未使用的空间
	std::deque<unsigned char *> m_usedBuff;	//使用的空间
	int m_curIndex;
public:
	XTempSave()
		:m_isInited(false)
		,m_cores(NULL)
		,m_coreSum(0)
	{}
	~XTempSave(){release();}
	bool init(int size,int sum);	//size:一个core的大小，sum：core的数量
	bool pushACore(const unsigned char *p);	//向对立中推入一个状态
	unsigned char *getPrevCore() const;		//获取上一个状态：如果没有上一个状态则返回NULL
	void moveToPrev(){if(m_curIndex > 0) --m_curIndex;/*forTest();*/}
	unsigned char *getNextCore() const;		//获取下一个状态：如果没有下一个状态则返回NULL
	void moveToNext(){if(m_curIndex >= 0 && m_curIndex + 1 < m_usedBuff.size()) ++ m_curIndex;/*forTest();*/}
	unsigned char *getCurCore() const;
	void release();
	void forTest();
};
#if WITH_INLINE_FILE
#include "XTempSave.inl"
#endif
}
#endif