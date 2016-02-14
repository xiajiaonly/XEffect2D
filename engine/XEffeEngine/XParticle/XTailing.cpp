#include "XStdHead.h"
#include "XTailing.h"
namespace XE{
void XTailing::AddPoint(const XVector2 &pos)	//增加新的点
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
		++ index;
		if(index >= m_maxPointSum) index = 0;
		if(index == m_headPoint) break;
	}
	m_isEnd = (m_endPoint == m_headPoint);
}
void XTailingDPoint::DrawBody()
{
	if(m_isEnd) return;
	int index = m_endPoint;
	for(int i = 0;i < m_maxPointSum;++ i)
	{
		XTailingPoint &tmp = m_points[index];
		m_color.fA = tmp.life;
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
	if(m_isEnd) return 0;
	int index = m_endPoint;
	int i = 0;
	int indexV = 0;
	int indexC = 0;
	for(i = 0;i < m_maxPointSum;++ i,indexV += 2,indexC += 4)
	{
		v[indexV] = m_points[index].position.x;
		v[indexV + 1] = m_points[index].position.y;
		c[indexC] = m_color.fR;
		c[indexC + 1] = m_color.fG;
		c[indexC + 2] = m_color.fB;
		c[indexC + 3] = m_points[index].life;
		++ index;
		if(index >= m_maxPointSum) index = 0;
		if(index == m_headPoint) break;
	}
	if(i < 2) return 0;
	return i;
}
void XTailingDLine::DrawBody()
{
	if(m_isEnd) return;
	int index = m_endPoint;
	glLineWidth(m_size);
	glBegin(GL_LINE_STRIP);
	for(int i = 0;i < m_maxPointSum;++ i)
	{
#ifdef WITH_EX_EFFECT
		if(XRand::random(400) <= 1) m_color.fA = 1.0f;//m_points[index].life * 2.0f;
		else m_color.fA = m_points[index].life;
#else
		m_color.fA = m_points[index].life;
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
#if !WITH_INLINE_FILE
#include "XTailing.inl"
#endif
}