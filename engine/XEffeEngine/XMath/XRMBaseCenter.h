#include "XVector2.h"
//这个类是一个基于某个中心点的随机运动
namespace XE {
class XRMBaseCenter
{
private:
	bool m_isInited;
	XVec2 m_center;
	float m_maxR;
	XVec2 m_startPos;
	XVec2 m_curPos;
	XVec2 m_targetPos;
	float m_speed;
	XMoveData m_md;

	void calNextPos();
	XVec2 randAPos()	//随机计算一个符合要求的位置
	{
		return m_center + XMath::getRotateRate(XRand::randomf(PI2)) * XRand::randomf(0.5f, 1.0f) * m_maxR;
	}
public:
	XRMBaseCenter()
		:m_isInited(false)
	{}
	bool init(const XVec2& c, float r, float speed);
	void update(float steptime);
	XVec2 getCurPos()const { return m_curPos; }
};
}