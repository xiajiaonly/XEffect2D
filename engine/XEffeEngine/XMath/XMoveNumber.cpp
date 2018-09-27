#include "XStdHead.h"
#include "XMoveNumber.h"
namespace XE{
XBool XMoveNumber::move(float delay)
{
	float tempSum;
	if((int)(m_armNumber) != m_curNumber)
	{
		tempSum = m_armNumber - m_curNumber;
		if(0.0075f * delay < 1.0f)	//变化的比例不能超过1，否则就会放大了
		{
			tempSum *= 0.0075f * delay;
		}
		if(tempSum < 1.0f && tempSum > -1.0f)	//最小的变化值不能太小
		{
			if(tempSum < 0) tempSum = -1.0f;
			else tempSum = 1.0f;
		}
		m_curNumber += tempSum;
		return XTrue;
	}
	return XFalse;
}
}