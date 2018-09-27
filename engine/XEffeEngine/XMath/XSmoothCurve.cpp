#include "XStdHead.h"
#include "XSmoothCurve.h"
namespace XE{
void XSmoothCurve::initCurve(const XVec2& s,const XVec2& e,int minS,int maxS,float offsetRate)
{
	m_infos.clear();
	m_start = s;
	m_end = e;
	if(minS <= 0) minS = 1;
	if(maxS < minS) maxS = minS;
	int sum = minS + XRand::random(maxS - minS);
	m_infos.resize(sum);
	for(int i = 0;i < sum;++ i)
	{
		XSmoothCurveInfo &tmp = m_infos[i];
		if(i == 0) tmp.m_cycleSum = 1.0f + XRand::random(2);
		else tmp.m_cycleSum = XRand::randomf(1.0f,5.0f);
		tmp.m_maxOffset = XRand::randomf(0.25f,0.35f) * m_start.getLength(m_end) / static_cast<float>(sum) * offsetRate;
		tmp.m_isAdd = XRand::random(100) < 50;
	}
	m_isInited = true;
}
//[0.0f - 1.0f]
XVec2 XSmoothCurve::getPosition(float rate)
{
	if(!m_isInited) return XVec2::zero;
	if(rate <= 0.0f) return m_start;
	if(rate >= 1.0f) return m_end;
	XVec2 ret;
	//计算基点
	ret = XMath::lineSlerp(m_start,m_end,rate);
	//方案1：
	//float angle = m_infos[0].m_cycleSum * rate;
	//if(m_infos[0].m_isAdd) ret.y += sin(angle * PI) * m_infos[0].m_maxOffset;
	//else ret.y -= sin(angle * PI) * m_infos[0].m_maxOffset;
	//方案2：
	float angle = m_infos[0].m_cycleSum * rate;
	float len = sin(angle * PI) * m_infos[0].m_maxOffset;
	if(!m_infos[0].m_isAdd) len = -len;
	XVec2 dir = XMath::normalize(m_end - m_start);
	ret += XVec2(dir.y,-dir.x) * len;

	for(int i = 1;i < m_infos.size();++ i)
	{
		angle = m_infos[i].m_cycleSum * rate * PI2;
		if(m_infos[i].m_isAdd) ret += sin(rate * PI) * XVec2(sin(angle),cos(angle)) * m_infos[i].m_maxOffset;
		else ret -= sin(rate * PI) * XVec2(sin(angle),cos(angle)) * m_infos[i].m_maxOffset;
	}
	return ret;
}
void XSmoothCurve::draw(int density)
{
	if(!m_isInited || density <= 0) return;
	float len = 1.0f / density;
	float tmpPos = 0.0f;
	for(int i = 0;i < density;++ i)
	{
		XRender::drawLine(getPosition(tmpPos),getPosition(tmpPos + len));
		tmpPos += len;
	}
}
}