#include "XStdHead.h"
#include "XTailing.h"
namespace XE{
void XTailing::AddPoint(const XVec2& pos)	//增加新的点
{
	m_points[m_headPoint].life = m_maxLife;
	m_points[m_headPoint].position = pos;
	++ m_headPoint;
	if(m_headPoint >= m_maxPointSum) m_headPoint = 0;
	if(m_headPoint == m_endPoint)
	{
		++ m_endPoint;
		if(m_endPoint >= m_maxPointSum) m_endPoint = 0;
	}
	m_isEnd = false;
}
void XTailing::Update(const float &stepTime)				//更新状态
{
	if(m_isEnd) return;
	int index = m_endPoint;
	//m_mutex.Lock();
	for(int i = 0;i < m_maxPointSum;++ i)
	{
		if(m_points[index].life <= stepTime)
		{//生命结束
			m_points[index].life = 0.0f;
			++ m_endPoint;
			if(m_endPoint >= m_maxPointSum) m_endPoint = 0;
		}else
		{//生命衰减
			m_points[index].life -= stepTime;
		}
		//if(m_points[index].life <= 0.0f) m_points[index].life = 0.0f;
		++ index;
		if(index >= m_maxPointSum) index = 0;
		if(index == m_headPoint) break;
	}
	m_isEnd = (m_endPoint == m_headPoint);
	//m_mutex.Unlock();
}
void XTailingDPoint::DrawBody()
{
	if(m_isEnd) return;
	int index = m_endPoint;
	for(int i = 0;i < m_maxPointSum;++ i)
	{
		XTailingPoint &tmp = m_points[index];
		m_color.a = tmp.life;
		glColor4fv(m_color);
		glVertex2fv(tmp.position);
		//XRender::drawPoint(tmp.position,10,1,1,1,tmp.life * 0.001f);
		++ index;
		if(index >= m_maxPointSum) index = 0;
		if(index == m_headPoint) break;
	}
}
int XTailingDLine::UpdateData(float *v,float *c)
{
	if(m_isEnd || getParticlesSum() < 2) return 0;	//例子数量不够
	//m_mutex.Lock();
	int index = m_endPoint;
	int i = 0;
	for(i = 0;i < m_maxPointSum;++ i)
	{
		*(v++) = m_points[index].position.x;
		*(v++) = m_points[index].position.y;
		*(c++) = m_color.r;
		*(c++) = m_color.g;
		*(c++) = m_color.b;
		*(c++) = m_points[index].life;
		//v[indexV] = m_points[index].position.x;
		//v[indexV + 1] = m_points[index].position.y;
		//c[indexC] = m_color.r;
		//c[indexC + 1] = m_color.g;
		//c[indexC + 2] = m_color.b;
		//c[indexC + 3] = m_points[index].life;
		++ index;
		if(index >= m_maxPointSum) index = 0;
		if(index == m_headPoint) break;
	}
	//m_mutex.Unlock();
	if(i < 2) return 0;
	return i;
}
int XTailingDLine::UpdateDataF(float *v,float *c)
{
	if(m_isEnd || getParticlesSum() < 2) return 0;	//例子数量不够
	int index = m_headPoint - 1;
	if(index < 0) index = m_maxPointSum - 1;

	*(v++) = m_points[index].position.x;
	*(v++) = m_points[index].position.y;
	*(c++) = m_color.r;
	*(c++) = m_color.g;
	*(c++) = m_color.b;
	*(c++) = 1;//m_points[index].life;
	if(m_points[index].life >= 1.0f || m_points[index].life < 0.0f)
		printf("error");

	-- index;
	if(index < 0) index = m_maxPointSum - 1;
	*(v++) = m_points[index].position.x;
	*(v++) = m_points[index].position.y;
	*(c++) = m_color.r;
	*(c++) = m_color.g;
	*(c++) = m_color.b;
	*(c++) = 1;//m_points[index].life;
	if(m_points[index].life >= 1.0f || m_points[index].life < 0.0f)
		printf("error");
	return 2;
}
void XTailingDLine::DrawBody()
{
	if(m_isEnd || getParticlesSum() < 2) return;	//例子数量不够
	int index = m_endPoint;
	glLineWidth(m_size);
	glBegin(GL_LINE_STRIP);
	for(int i = 0;i < m_maxPointSum;++ i)
	{
#ifdef WITH_EX_EFFECT
		if(XRand::random(400) <= 1) m_color.a = 1.0f;//m_points[index].life * 2.0f;
		else m_color.a = m_points[index].life;
#else
		m_color.a = m_points[index].life;
#endif
		glColor4fv(m_color);
		glVertex2fv(m_points[index].position);
		//XRender::drawLine(m_points[oldIndex].position,m_points[index].position,2,1,1,1,m_points[index].life * 0.001f);
		++ index;
		if(index >= m_maxPointSum) index = 0;
		if(index == m_headPoint) break;
	}
	glEnd();
}
void XTailingDLine::DrawF()
{
	if(m_isEnd || getParticlesSum() < 2) return;	//例子数量不够
	int index = m_headPoint - 1;
	if(index < 0) index = m_maxPointSum - 1;

#ifdef WITH_EX_EFFECT
	if(XRand::random(400) <= 1) m_color.a = 1.0f;//m_points[index].life * 2.0f;
	else m_color.a = m_points[index].life;
#else
	m_color.a = m_points[index].life;
#endif
	glColor4fv(m_color);
	glVertex2fv(m_points[index].position);

	-- index;
	if(index < 0) index = m_maxPointSum - 1;
	glVertex2fv(m_points[index].position);
}
#if !WITH_INLINE_FILE
#include "XTailing.inl"
#endif
}