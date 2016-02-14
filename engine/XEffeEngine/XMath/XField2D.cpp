#include "XStdHead.h"
#include "XField2D.h"
#include "XBasicOpenGL.h"
namespace XE{
void XField2D::Draw(const XVector2 &size)	//描绘场
{
	XVector2 posB;
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glLineWidth(1);
	glBegin(GL_LINES);
	int index = 0;
	XVector2 tmpSize = size / XVector2(m_w,m_h);
	for(int h = 0;h < m_h;++ h)
	{
		for(int w = 0;w < m_w;++ w,++ index)
		{
			posB = tmpSize * XVector2(w,h);
			glColor3fv(XFColor::blue);
			glVertex2fv(posB);
			glColor3fv(XFColor::red);
			posB += m_pVectors[index];
			glVertex2fv(posB);
		}
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void XField2D::SetField2DType(XField2DType type)
{
	if(!m_isInited) return;
	if(m_type == type) return;
	m_type = type;
	float len;
	//下面初始化场的相关数据
	switch(m_type)
	{
	case FIELD2D_NULL:break;
	case FIELD2D_DOWN:
		for(int i = 0;i < m_w * m_h;++ i)
		{
			m_pVectors[i].set(0.0f,75.0f);
		}
		break;
	case FIELD2D_WHIRL:	//尚未完成
		{
			int index = 0;
			XVector2 tmp(m_w >> 1,m_h >> 1);
			XVector2 tmpx;
			for(int h = 0;h < m_h;++ h)
			{
				for(int w = 0;w < m_w;++ w,++ index)
				{
					len = 1.0f + tmp.getLength(w,h);
					len = 50.0f * pow(1.0 / len,0.25);
				//	len = 10.0f;
					tmpx = tmp - XVector2(w,h);
					tmpx.set(tmpx.y,-tmpx.x);
					tmpx.normalize();
					m_pVectors[index] = tmpx * len;
				}
			}
		}
		break;
	case FIELD2D_RAND:
		for(int i = 0;i < m_w * m_h;++ i)
		{
			m_pVectors[i].set(XRand::randomf(-20,20),XRand::randomf(-20,20));
		}
		break;
	}
}
}