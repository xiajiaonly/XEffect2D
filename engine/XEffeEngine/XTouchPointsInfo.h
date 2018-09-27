#ifndef _JIA_XTOUCHPOINTSINFO_
#define _JIA_XTOUCHPOINTSINFO_
#include "XMath\XVector2.h"
#include <vector>
namespace XE{
struct XTouchPointInfoBase
{
	int id;
	XVec2 pos;
};
class XTouchPointsManage
{
private:
	std::vector<XTouchPointInfoBase> m_touchPointsInfo;
public:
	int getTouchIndexByID(int id)const
	{
		for (int i = 0; i < m_touchPointsInfo.size(); ++i)
		{
			if (m_touchPointsInfo[i].id == id) return i;
		}
		return -1;
	}
	const std::vector<XTouchPointInfoBase> &getAllPoints()const { return m_touchPointsInfo; }
	void touchDown(float x, float y, int id)
	{
		int index = getTouchIndexByID(id);
		if (index >= 0)
		{
			m_touchPointsInfo[index].pos.set(x, y);
		}
		else
		{
			XTouchPointInfoBase tmp;
			tmp.id = id;
			tmp.pos.set(x, y);
			m_touchPointsInfo.push_back(tmp);
		}
	}
	void touchUp(float x, float y, int id)
	{
		int index = getTouchIndexByID(id);
		if (index < 0) return;
		m_touchPointsInfo.erase(m_touchPointsInfo.begin() + index);
	}
	void touchMove(float x, float y, int id)
	{
		int index = getTouchIndexByID(id);
		if (index >= 0)
		{
			m_touchPointsInfo[index].pos.set(x, y);
		}
		else
		{
			XTouchPointInfoBase tmp;
			tmp.id = id;
			tmp.pos.set(x, y);
			m_touchPointsInfo.push_back(tmp);
		}
	}
};
}
#endif