#ifndef _JIA_XMWFILTER_
#define _JIA_XMWFILTER_
#include <deque>
namespace XE {
class XMWFilter
{
private:
	std::deque<float> m_datas;
	int m_windowSize;
	float m_curValue;
	float m_totalValue;
public:
	void setWindowSize(int size);
	int getWindowSize()const { return m_windowSize; }
	void pushAData(float data);
	float getCurValue()const { return m_curValue; }
	XMWFilter()
		:m_windowSize(10)
		, m_curValue(0)
		, m_totalValue(0)
	{}
};
}
#endif