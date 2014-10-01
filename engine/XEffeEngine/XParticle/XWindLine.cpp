#include "XEffeEngine.h"
#include "XWindLine.h"

void _XWindLine::draw()
{
	if(!m_isInited) return;
	if(m_points.size() < 2) return;
	float rate = 0;
	//for(int i = 0;i < m_points.size() - 1;++ i)
	//{
	//	rate = m_pointsLife[i] / m_maxLife * m_points[i + 1]->m_percentage;
	//	if(rate > 0.666f) rate = (1.0f - rate) * 3.0f;
	//	else rate = rate * 1.5f;
	//	drawLine(m_points[i]->m_position,m_points[i + 1]->m_position,rate * m_wRate,1.0f,1.0f,1.0f,rate * m_aRate);
	//}

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	for(int i = 0;i < m_points.size();++ i)
	{
		rate = m_pointsLife[i] / m_maxLife * m_points[i]->m_percentage;
		if(rate > 0.666f) rate = (1.0f - rate) * 3.0f;
		else rate = rate * 1.5f;
		glColor4f(0.6f,0.6f,1.0f,rate * m_aRate);
		glVertex2fv(m_points[i]->m_position);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void _XWindLine::move(int stepTime)	//生命衰减
{
	int sum = 0;
	for(int i = 0;i < m_pointsLife.size();++ i)
	{
		m_pointsLife[i] -= stepTime;
		if(m_pointsLife[i] <= 0.0f) ++ sum;	//死亡的点的数量
	}
	if(sum > 0)
	{
	//	_XSmoothPoint *tmp = NULL;
		for(int i = 0;i < sum;++ i)
		{
			m_pointsLife.pop_front();
			XDELETE(m_points[0]);
			m_points.pop_front();
		}
		updateData();	//更新点的状态
	}
}
void _XWindLine::addAPoint(const _XVector2 & p)	//插入一个点
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
		m_pointsLife.push_back(m_maxLife);
	}else
	{
		//方案1:线性插值
		_XSmoothPoint * last = m_points[m_points.size() - 1];	//获取最后一个点
		float lastLife = m_pointsLife[m_points.size() - 1];
		float len = p.getLength(last->m_position);
		if(len <= 0.0f)	return;	//与最后一个点重合不需要加入新的点
		int sum = len / m_insertLength;
		sum = XEE_Min(m_insertSum,sum);
		//尚未完成
		float d = 1.0f / (sum + 1.0f);
		float per = d;
		for(int i = 1;i < sum + 1; ++ i)
		{//下面插入点
			pushAPoint(lineSlerp<_XVector2>(last->m_position,p,per),
				lineSlerp<float>(lastLife,m_maxLife,per));
			per += d;
		}
		//方案2:贝塞尔插值 (尚未实现)
		//更新内部数据
		updateData();
	}
}