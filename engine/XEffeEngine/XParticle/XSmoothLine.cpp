#include "XEffeEngine.h"
#include "XSmoothLine.h"

void _XSmoothLine::release()
{
	if(!m_isInited) return;
	for(int i = 0;i < m_points.size(); ++ i)
	{
		XDELETE(m_points[i]);
	}
	m_points.clear();
	m_isInited = false;
}
void _XSmoothLine::pushAPoint(const _XVector2 &p)	//这个点是已经经过插值运算了的
{
	_XSmoothPoint *last = m_points[m_points.size() - 1];
	_XSmoothPoint *ip = createMem<_XSmoothPoint>();
	if(ip == NULL) return;
	ip->m_length = p.getLength(last->m_position);
	ip->m_percentage = 0.0f;	//这里暂时不计算，等待一次性处理
	ip->m_position = p;
	m_points.push_back(ip);
}
void _XSmoothLine::updateData()
{//更新内部数据
	if(m_points.size() < 2) return;
	float len = 0.0f;
	for(int i = 0;i < m_points.size();++ i)
	{
		len += m_points[i]->m_length;
		m_points[i]->m_percentage = len;
	}
	len = 1.0f / len;
	for(int i = 0;i < m_points.size();++ i)
	{
		m_points[i]->m_percentage = m_points[i]->m_percentage * len;
	}
}
void _XSmoothLine::draw()
{
	if(!m_isInited) return;
	if(m_points.size() < 2) return;
	for(int i = 0;i < m_points.size() - 1;++ i)
	{
		drawLine(m_points[i]->m_position,m_points[i + 1]->m_position);
		drawPoint(m_points[i]->m_position.x,m_points[i]->m_position.y,1.0f,0.0f,0.0f,1.0f);
	}
}
void _XSmoothLine::addAPoint(const _XVector2 & p)	//插入一个点
{
	if(!m_isInited) return;
	if(m_points.size() == 0)
	{//第一个点，不需要插值
		_XSmoothPoint *ip = createMem<_XSmoothPoint>();
		if(ip == NULL) return;
		ip->m_length = 0.0f;
		ip->m_percentage = 0.0f;
		ip->m_position = p;
		m_points.push_back(ip);
	}else
	{
		//方案1:线性插值
		_XSmoothPoint * last = m_points[m_points.size() - 1];	//获取最后一个点
		float len = p.getLength(last->m_position);
		if(len <= 0.0f)	return;	//与最后一个点重合不需要加入新的点
		int sum = len / m_insertLength;
		sum = XEE_Min(m_insertSum,sum);
		//尚未完成
		float d = 1.0f / (sum + 1.0f);
		float per = d;
		for(int i = 1;i < sum + 1; ++ i)
		{//下面插入点
			pushAPoint(lineSlerp<_XVector2>(last->m_position,p,per));
			per += d;
		}
		//方案2:贝塞尔插值 (尚未实现)
		//更新内部数据
		updateData();
	}
}
bool _XSmoothLine::init(int insertSum,float insertLength)
{
	if(m_isInited) return false;
	if(insertSum < 0 || insertLength <= 0.0f) return false;
	m_insertSum = insertSum;
	m_insertLength = insertLength;

	m_isInited = true;
	return true;
}