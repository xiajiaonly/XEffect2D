#include "XStdHead.h"
#include "XRMBaseCenter.h"
namespace XE {
bool XRMBaseCenter::init(const XVec2& c, float r, float speed)
{
	if (m_isInited) return false;

	m_center = c;
	m_maxR = r;
	m_speed = speed;
	//初始化一个位置
	m_curPos = randAPos();
	calNextPos();

	m_isInited = true;
	return true;
}
void XRMBaseCenter::update(float steptime)
{
	if (!m_isInited) return;
	m_md.move(steptime);
	if (m_md.getIsEnd())
	{
		m_curPos = m_targetPos;
		calNextPos();
		return;
	}
	else
	{
		m_curPos = XMath::lineSlerp(m_startPos, m_targetPos, m_md.getCurData());
	}
}
void XRMBaseCenter::calNextPos()
{
	m_startPos = m_curPos;
	m_targetPos = randAPos();
	float timer = m_targetPos.getLength(m_curPos) / m_speed;
	m_md.setEx(0.0f, 1.0f, timer, MD_MODE_DCOS_MULT);
}
}