#ifndef _JIA_XMOVENUMBER_
#define _JIA_XMOVENUMBER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.3
//--------------------------------
#include "../XOSDefine.h"
namespace XE{
//下面是一个用于实现动态数字变化的类,就是看起来是一个数字滚动
class XMoveNumber
{
private:
	int m_curNumber;	//当前的数值
	float m_armNumber;	//目标需要达到的数值
public:
	void addArmNumber(float number){m_armNumber += number;}	//设置目标数值的增幅
	void setArmNumber(float number){m_armNumber = number;}	//设置目标数值
	int getCurNumber(){return m_curNumber;}				//返回当前的数值
	int getArmNumber(){return (int)m_armNumber;}
	XBool move(float delay);			//返回数据是否发生变化0：没有变化，1：发生了变化
	void reset()
	{
		m_curNumber = 0;
		m_armNumber = 0.0f;
	}
	XMoveNumber()
	:m_curNumber(0)
	,m_armNumber(0.0f)
	{}
};
}
#endif