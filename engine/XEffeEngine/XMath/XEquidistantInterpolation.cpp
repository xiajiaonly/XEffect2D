#include "XStdHead.h"
#include "XEquidistantInterpolation.h"
namespace XE{
int XEquidistantIterpolation::calPoint(const XVector2 &p,std::vector<XVector2> &data)
{
	data.clear();
	if (m_isFirstData)
	{//第一个点位有效点
		m_isFirstData = false;
		m_upPoint = p;
		data.push_back(p);
		return data.size();
	}
	//下面计算插入的点
	float len = p.getLength(m_upPoint);
	if (len <= 0.0f) return data.size();	//非法的临界数据
	if (m_upLength + len >= m_distance)
	{//长度超过范围这里需要差值,这里依次计算插入点的位置
		float tmpLen = m_distance - m_upLength;
		XVector2 point;
		while (true)
		{
			point = XMath::lineSlerp<XVector2>(m_upPoint, p, tmpLen / len);
			data.push_back(point);
			tmpLen += m_distance;
			if (tmpLen > len) break;
		}
		m_upPoint = p;
		m_upLength = len - (tmpLen - m_distance);
		return data.size();
	}
	else
	{//长度尚未超过范围，这里不需要差值
		m_upLength += len;
		m_upPoint = p;
		return data.size();
	}
}
}