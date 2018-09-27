#ifndef _JIA_XVIRTUALMTOUCH_
#define _JIA_XVIRTUALMTOUCH_
#include "windows.h"

namespace XE {
enum XVTouchState
{
	VT_STATE_DOWN,
	VT_STATE_MOVE,
	VT_STATE_UP,
};
struct XVTouchInfo
{
	int id;
	int x;
	int y;
	XVTouchState state;	//0:down,1:move;2:up
	XVTouchInfo(int _id, int _x, int _y, XVTouchState _state)
		:id(_id)
		, x(_x)
		, y(_y)
		, state(_state)
	{}
	bool operator <= (const XVTouchInfo& rhs) const
	{
		return id <= rhs.id;
	}
	bool operator >= (const XVTouchInfo& rhs) const
	{
		return id >= rhs.id;
	}
	XVTouchInfo()
		:id(-1)
	{}
};
class XVirMTouch
{
private:
	int m_maxPointSum;

	bool m_isInited;
	std::vector<POINTER_TOUCH_INFO> m_contacts;
public:
	XVirMTouch()
		:m_isInited(false)
	{}
	~XVirMTouch() { release(); }
	void release();
	bool init(int maxPointsSum = 1);

	bool pointDown(float x, float y, int id);
	bool pointUp(float x, float y, int id);
	bool pointMove(float x, float y, int id);
	//调用这些函数之前，请根据ID进行排序
	bool pointsDown(const std::vector<XVTouchInfo>& info);
	bool pointsUp(const std::vector<XVTouchInfo>& info);
	bool pointsMove(const std::vector<XVTouchInfo>& info);
	//推荐使用下面这个接口
	bool upDateData(const std::vector<XVTouchInfo>& info);
};
}
#endif