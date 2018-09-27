#include "XStdHead.h"
#include "XVegetationSystem.h"
namespace XE{
void XVegetationSystem::init(const XVec2& area,int coreSum,float coreSize,int maxVegetationsSum)
{
	if(area.x <= 0 || area.y <= 0 || maxVegetationsSum <= 0) return;
	if(coreSize <= 0.0f) coreSize = 0.0f;
	if(coreSum < 0 || coreSize <= 0.0f) coreSum = 0;
	m_area = area;
	m_coreSum = coreSum;
	m_coreSize = coreSize;
	m_maxVegetationsSum = maxVegetationsSum;
	reset();
}
void XVegetationSystem::draw()
{
	for(auto it = m_cores.begin();it != m_cores.end();++ it)
	{
		XRender::drawCross(it->pos, 10.0f, 2.0f, XFColor::red);
		XRender::drawCircleLine(it->pos, it->r, 60, 2, XFColor::red);
	}
	for(auto it = m_vegetations.begin();it != m_vegetations.end();++ it)
	{
		XRender::drawCross(it->pos, 10.0f, 2.0f, XFColor::blue);
		XRender::drawCircleLine(it->pos, it->size * m_maxVegetationSize, 60, 2, XFColor::blue);
	}
}
void XVegetationSystem::reset()
{
	//下面开始计算植被并开始分布
	if(m_coreSum > 0)
	{//下面计算核心的分布
		m_cores.resize(m_coreSum);
		for(int i = 0;i < m_coreSum;++ i)
		{
			m_cores[i].pos.set(XRand::random(m_area.x),XRand::random(m_area.y));
			m_cores[i].r = XRand::randomf(1.2f,0.5f) * m_coreSize;
		}
	}
	//方案1：随机分布之后随机裁剪
	//m_vegetations.resize(m_maxVegetationsSum);
	//for(int i = 0;i < m_vegetations.size();++ i)
	//{
	//	m_vegetations[i].pos.set(XRand::random(area.x),XRand::random(area.y));
	//	m_vegetations[i].size = XRand::randomf(0.1f,1.0f);
	//}
	//float rate = 0.0f;
	//for(int i = 0;i < m_vegetations.size();++ i)
	//{
	//	rate = 0.1f;
	//	for(int j = 0;j < m_cores.size();++ j)
	//	{
	//		rate += XMath::mapping1D(m_vegetations[i].pos.getLength(m_cores[j].pos),0.0f,m_cores[j].r,1.0f,0.0f);
	//	}
	//	if(XRand::randomf(1.0f) > rate)
	//	{
	//		m_vegetations.erase(m_vegetations.begin() + i);
	//		-- i;
	//	}
	//}
	//方案2：随机分配之后随机分布
	std::vector<float> cRate;
	float allRate = 0.0f;
	for (auto it = m_cores.begin(); it != m_cores.end(); ++it)
	{
		cRate.push_back(it->r * it->r);
		allRate += cRate[cRate.size() - 1];
	}
	cRate.push_back(m_area.x * m_area.y * 0.0625f);
	allRate += cRate[cRate.size() - 1];
	m_vegetations.clear();
	for(int i = 0;i < m_maxVegetationsSum;++ i)
	{
		float tmpRate = XRand::randomf(allRate);
		int index = m_cores.size();
		for(int j = 0;j < m_cores.size();++ j)
		{
			if(tmpRate < cRate[j])
			{
				index = j;
				break;
			}else
			{
				tmpRate -= cRate[j];
			}
		}
		if(index >= m_cores.size())
		{//不在覆盖范围内，则为随机分布，大小偏小
			XVegetation tmp;
			tmp.pos.set(XRand::random(m_area.x),XRand::random(m_area.y));
			//tmp.size = XRand::randomf(0.1f,1.0f);
			tmp.size = XRand::gaussRandMin() * 0.5f + 0.1f;
			m_vegetations.push_back(tmp);
		}else
		{//中心分布
			XVegetation tmp;
			//float r = (XRand::gaussRand() - 0.5f) * m_cores[index].r;	//正态分布，过于集中
			float r = XRand::randomf(m_cores[index].r);
			tmp.pos = m_cores[index].pos + r * XMath::getRotateRate(XRand::randomf(PI2));
			tmp.size = XRand::randomf(0.1f,1.0f - (r / m_cores[index].r));
			m_vegetations.push_back(tmp);
		}
	}
	//合理距离裁剪
	for(int i = 0;i < int(m_vegetations.size()) - 1;++ i)
	{
		for(int j = i + 1;j < m_vegetations.size();++ j)
		{
			if (m_vegetations[i].pos.getLengthSqure(m_vegetations[j].pos) < m_maxVegetationSize * m_maxVegetationSize * 25.0f)
			{
				m_vegetations.erase(m_vegetations.begin() + j);
				-- j;
			}
		}
	}
	//范围裁剪
	for(int i = 0;i < int(m_vegetations.size());++ i)
	{
		if(m_vegetations[i].pos.x < 0.0f || m_vegetations[i].pos.x >= m_area.x ||
			m_vegetations[i].pos.y < 0.0f || m_vegetations[i].pos.y >= m_area.y)
		{
			m_vegetations.erase(m_vegetations.begin() + i);
			-- i;
		}
	}
}
}