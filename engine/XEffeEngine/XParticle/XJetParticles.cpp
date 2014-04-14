//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XJetParticles.h"

_XJetParticles::_XJetParticles()
:m_isInited(0)				//是否初始化
,m_isEnd(0)				//是否结束
,m_isSetEnd(0)				//是否设置结束
,m_texture(NULL)
,m_textureAtom(NULL)
{
}

int _XJetParticles::init(const _XVector2& position,float jetSpeed,float jetAngle,float jetDensity,
						 const _XTexture *texture,const _XTexture *textureAton)
{
	if(m_isInited != 0) return 0; 

	m_nowPosition = position;
	m_initSpeed = jetSpeed;
	m_jetAngle = jetAngle;
	if(jetDensity <= 0) jetDensity = 0.01f;
	m_jetDensity = jetDensity;
	m_directionAngle = 0;
	if(texture == NULL || !glIsTexture(texture->m_texture)) return 0;
	m_texture = texture;
	if(textureAton == NULL)
	{
		m_textureAtom = texture;
	}else
	{
		if(!glIsTexture(texture->m_texture))
		{
			return 0;
		}else
		{
			m_textureAtom = textureAton;
		}
	}
	m_atomBasic = createMem<_XEchoParticles>();
	if(m_atomBasic == NULL) return 0;

	m_maxAtomSum = jetDensity * 10;
	if(m_maxAtomSum < 2) m_maxAtomSum = 2;
	m_atom = createArrayMem<_XEchoParticles>(m_maxAtomSum);
	if(m_atom == NULL) 
	{
		XDELETE(m_atomBasic);
		return 0;
	}

	m_nowAtomPoint = 0;
	m_timer = 0;

	m_isInited = 1;
	m_isEnd = 1;
	m_isSetEnd = 0;
	return 1;
}

void _XJetParticles::reset(const _XVector2& position)
{
	if(m_isInited == 0) return;
	if(m_isEnd == 0) return;
	m_atomBasic->m_isInited = 0;
	m_atomBasic->init(m_texture,100,2,position,0.95f,-0.001f);
	m_atomBasic->m_parentParticle.m_initColor.setColor(1.0f,1.0f,1.0f,1.0f);
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
		m_atom[i].m_isInited = 0;
		m_atom[i].m_isEnd = 1;
	}
	m_nowAtomPoint = 0;
	m_timer = 0;
	m_isEnd = 0;
	m_isSetEnd = 0;
}

void _XJetParticles::reset(float x,float y)
{
	if(m_isInited == 0) return;
	if(m_isEnd == 0) return;
	m_atomBasic->m_isInited = 0;
	m_atomBasic->init(m_texture,100,2,_XVector2(x,y),0.95f,-0.001f);
	m_atomBasic->m_parentParticle.m_initColor.setColor(1.0f,1.0f,1.0f,1.0f);
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
		m_atom[i].m_isInited = 0;
		m_atom[i].m_isEnd = 1;
	}
	m_nowAtomPoint = 0;
	m_timer = 0;
	m_isEnd = 0;
	m_isSetEnd = 0;
}

void _XJetParticles::move(int timeDelay)
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return;
	m_atomBasic->move(timeDelay);
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
		m_atom[i].move(timeDelay);
		if(m_atom[i].m_parentParticle.m_stage == STAGE_MOVE)
		{
			m_atom[i].m_parentParticle.m_initColor.fA -= 0.00125f * timeDelay;
		//	m_atom[i].m_parentParticle.m_initPosition.x += m_atom[i].m_parentParticle.m_dPosition.x * timeDelay;
		//	m_atom[i].m_parentParticle.m_initPosition.y += m_atom[i].m_parentParticle.m_dPosition.y * timeDelay;
			m_atom[i].m_parentParticle.m_initPosition += m_atom[i].m_parentParticle.m_dPosition * timeDelay;
			m_atom[i].setPosition(m_atom[i].m_parentParticle.m_initPosition);
			if(m_atom[i].m_parentParticle.m_initColor.fA <= 0)
			{
				m_atom[i].m_parentParticle.m_initColor.fA = 0;
				m_atom[i].m_parentParticle.m_stage = STAGE_SLEEP;
				m_atom[i].setEnd();
			}
		}
	}
	//产生新的粒子
	if(m_isSetEnd == 0)
	{
		m_timer += timeDelay;
		int temp = 120 / m_jetDensity;
		if(m_timer > temp || random(temp - m_timer) < temp * 0.1)
		{//产生新的喷射粒子
			m_timer = 0;
			if(m_atom[m_nowAtomPoint].getIsEnd() != 0)
			{//有新的粒子
				m_atom[m_nowAtomPoint].m_isInited = 0;
				float tempR = 1.0;//random(75)/100.0 + 0.5;
				m_atom[m_nowAtomPoint].m_parentParticle.m_nowPosition = m_nowPosition;//_XVector2(m_nowPosition.x + m_w * 0.5,m_nowPosition.y + m_h * 0.5);
			//	m_atom[m_nowAtomPoint].init(&(m_textureAtom->m_texture),64,2,m_atom[m_nowAtomPoint].m_parentParticle.m_nowPosition,
			//		0.92,-0.001,m_textureAtom->m_w * tempR,m_textureAtom->m_h * tempR,64);
				m_atom[m_nowAtomPoint].init(m_textureAtom,64,2,m_atom[m_nowAtomPoint].m_parentParticle.m_nowPosition,
					0.92f,-0.001f,64);
				m_atom[m_nowAtomPoint].m_parentParticle.m_initSize.set(0.75f,0.75f);
				m_atom[m_nowAtomPoint].m_parentParticle.m_stage = STAGE_MOVE;
				tempR = (random(4000) / 2000.0f - 1.0f) * m_jetAngle + m_directionAngle + PI;
				m_atom[m_nowAtomPoint].m_parentParticle.m_dPosition.x = m_initSpeed * cos(tempR);
				m_atom[m_nowAtomPoint].m_parentParticle.m_dPosition.y = m_initSpeed * sin(tempR);
				m_atom[m_nowAtomPoint].m_parentParticle.m_initColor.setColor(1.0f,1.0f,1.0f,1.0f);

				++ m_nowAtomPoint;
				if(m_nowAtomPoint >= m_maxAtomSum)
				{
					m_nowAtomPoint = 0;
				}
			}
		}
	}else
	{//判断是否结束
		int flag = 0;
		if(m_atomBasic->getIsEnd() == 0) return;
		for(int i = 0;i < m_maxAtomSum;++ i)
		{
			if(m_atom[i].getIsEnd() == 0) 
			{
				flag = 1;
				break;
			}
		}
		if(flag == 0) m_isEnd = 1;
	}
}

void _XJetParticles::draw() const
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return;
	m_atomBasic->draw();
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
		m_atom[i].draw();
	}
}

void _XJetParticles::release()
{
	if(m_isInited == 0) return;
	XDELETE(m_atomBasic);
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
		m_atom[i].release();
	}
	XDELETE_ARRAY(m_atom);
	m_isInited = 0;		//防止release之后调用其它成员函数而造成错误
}
