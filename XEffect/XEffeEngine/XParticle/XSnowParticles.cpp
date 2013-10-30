//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XSnowParticles.h"

_XSnowParticles::_XSnowParticles()
:m_isInited(0)				//是否初始化
,m_isEnd(0)				//是否结束
,m_isSetEnd(0)				//是否设置结束
,m_atom(NULL)
,m_texture(NULL)
{
}

int _XSnowParticles::init(const _XVector2& productArea,const _XRect& liveArea,const _XVector2& flySpeed,
						  int maxAtomSum,float snowDensity,const _XTexture *texture)
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

	m_atom = NULL;	
//	m_atom = new _XEchoParticles[m_maxAtomSum]; 
	try
	{
		m_atom = new _XAloneParticles[m_maxAtomSum]; 
		if(m_atom == NULL) return 0;
	}catch(...)
	{
		return 0;
	}

	m_isInited = 1;
	m_isEnd = 0;				
	m_isSetEnd = 0;				
	return 1;
}

void _XSnowParticles::reset()
{
	if(m_isInited == 0) return;
	if(m_isEnd == 0) return;

	m_isEnd = 0;				
	m_isSetEnd = 0;				
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
	//	m_atom[i].m_isEnd = 1;
		m_atom[i].m_stage = STAGE_SLEEP;
	}
}

void _XSnowParticles::move(int timeDelay)
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return;
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
	//	if(m_atom[i].m_stage == STAGE_MOVE)
	//	{//飘落
			if(random(1000 * timeDelay) < 10)
			{
				m_atom[i].m_dPosition.x += (random(1000) * 0.002f - 1) * m_flySpeed.x * 0.1f;
			}
			m_atom[i].move(timeDelay);
			if(!m_liveArea.isInRect(m_atom[i].m_nowPosition))
			{
				m_atom[i].m_initColor.setA(0.0f);
				m_atom[i].m_stage = STAGE_SLEEP;
			}
	//	}
	}
	if(m_isSetEnd == 0)
	{//产生粒子
		if(random(10000) * 0.0001f <= m_snowDensity)
		{//产生粒子
			for(int i = 0;i < m_maxAtomSum;++ i)
			{
				if(m_atom[i].m_stage == STAGE_SLEEP)
				{
				//	m_atom[i].m_isInited = 0;
					float temp = random(75)/100.0f + 0.5f;
				//	m_atom[i].init(&(m_texture->m_texture),m_texture->m_w * temp,m_texture->m_h * temp);
					m_atom[i].init(m_texture);
					m_atom[i].m_nowSize.set(temp,temp);
					m_atom[i].m_stage = STAGE_MOVE;
					m_atom[i].m_dPosition.x = (random(1000) * 0.002f - 1.0f) * m_flySpeed.x;
					m_atom[i].m_dPosition.y = m_flySpeed.y * (0.5f + random(1000) * 0.0005f);
					m_atom[i].m_nowColor.setColor(1.0f,1.0f,1.0f,1.0f);
					m_atom[i].m_nowPosition.set(m_productArea.x + random(10000) * 0.0001f * (m_productArea.y - m_productArea.x),m_liveArea.top);
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

void _XSnowParticles::draw() const
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return;
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
	//	if(m_atom[i].m_stage == STAGE_MOVE)
	//	{
			m_atom[i].draw();
	//	}
	}
}

void _XSnowParticles::release()
{
	if(m_isInited == 0) return;
	XDELETE_ARRAY(m_atom);
	m_isInited = 0;		//防止release之后调用其它成员函数而造成错误
}

void _XSnowParticles::setEnd()
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return;
	if(m_isSetEnd != 0) return;

	m_isSetEnd = 1;
}
