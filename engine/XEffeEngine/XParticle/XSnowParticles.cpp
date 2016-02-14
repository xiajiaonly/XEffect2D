#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XSnowParticles.h"
namespace XE{
XSnowParticles::XSnowParticles()
:m_isInited(0)				//是否初始化
,m_isEnd(0)				//是否结束
,m_isSetEnd(0)				//是否设置结束
,m_atom(NULL)
,m_texture(NULL)
{}
int XSnowParticles::init(const XVector2& productArea,const XRect& liveArea,const XVector2& flySpeed,
						  int maxAtomSum,float snowDensity,const XTexture *texture)
{
	if(m_isInited != 0) return 0;
	m_productArea = productArea;
	m_liveArea = liveArea;			
	m_flySpeed = flySpeed;			

	if(maxAtomSum <= 0) return 0;
	m_maxAtomSum = maxAtomSum;			
	if(snowDensity <= 0) return 0;
	m_snowDensity = snowDensity;		

	if(texture == NULL || !glIsTexture(texture->m_texture)) return 0;
	m_texture = texture;

	m_atom = XMem::createArrayMem<XAloneParticles>(m_maxAtomSum);	
//	m_atom = XMem::createArrayMem<XEchoParticles>(m_maxAtomSum);
	if(m_atom == NULL) return 0;

	m_isInited = 1;
	m_isEnd = 0;				
	m_isSetEnd = 0;				
	return 1;
}
void XSnowParticles::reset()
{
	if(m_isInited == 0 ||
		m_isEnd == 0) return;

	m_isEnd = 0;				
	m_isSetEnd = 0;				
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
	//	m_atom[i].m_isEnd = 1;
		m_atom[i].m_stage = STAGE_SLEEP;
	}
}
void XSnowParticles::move(float timeDelay)
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
	//	if(m_atom[i].m_stage == STAGE_MOVE)
	//	{//飘落
			if(XRand::random(1000 * timeDelay) < 10)
			{
				m_atom[i].m_dPosition.x += (XRand::random(1000) * 0.002f - 1) * m_flySpeed.x * 0.1f;
			}
			m_atom[i].move(timeDelay);
			if(!m_liveArea.isInRect(m_atom[i].m_curPosition))
			{
				m_atom[i].m_initColor.setA(0.0f);
				m_atom[i].m_stage = STAGE_SLEEP;
			}
	//	}
	}
	if(m_isSetEnd == 0)
	{//产生粒子
		if(XRand::random(10000) * 0.0001f <= m_snowDensity)
		{//产生粒子
			for(int i = 0;i < m_maxAtomSum;++ i)
			{
				if(m_atom[i].m_stage == STAGE_SLEEP)
				{
				//	m_atom[i].m_isInited = 0;
					float temp = XRand::random(75)/100.0f + 0.5f;
				//	m_atom[i].init(&(m_texture->m_texture),m_texture->m_w * temp,m_texture->m_h * temp);
					m_atom[i].init(m_texture);
					m_atom[i].m_curSize.set(temp,temp);
					m_atom[i].m_stage = STAGE_MOVE;
					m_atom[i].m_dPosition.x = (XRand::random(1000) * 0.002f - 1.0f) * m_flySpeed.x;
					m_atom[i].m_dPosition.y = m_flySpeed.y * (0.5f + XRand::random(1000) * 0.0005f);
					m_atom[i].m_curColor.setColor(1.0f,1.0f,1.0f,1.0f);
					m_atom[i].m_curPosition.set(m_productArea.x + XRand::random(10000) * 0.0001f * (m_productArea.y - m_productArea.x),m_liveArea.top);
					break;
				}
			}
		}
	}else
	{//检查是否结束
		for(int i = 0;i < m_maxAtomSum;++ i)
		{
			if(m_atom[i].m_stage == STAGE_MOVE) return;
		}
		m_isEnd = 1;
	}
}
void XSnowParticles::draw() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
	//	if(m_atom[i].m_stage == STAGE_MOVE)
	//	{
			m_atom[i].draw();
	//	}
	}
}
}