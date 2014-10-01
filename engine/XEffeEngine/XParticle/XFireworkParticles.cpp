//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XFireworkParticles.h"
#include "../XBasicFun.h"
enum
{
	STAGE_PRODUCE,	//产生阶段
	STAGE_DESTROY	//消亡阶段
};

_XFireWorkParticles::_XFireWorkParticles()
:m_isInited(0)
,m_isEnd(0)
{
}

int _XFireWorkParticles::init(const _XVector2& position,const _XVector2& initSpeed,float decreaseSpeed,float gravity,int atomSum,const _XTexture *texture)
{
	if(m_isInited != 0) return 0;

	m_position = position;
	m_initSpeed = initSpeed;			
	m_decreaseSpeed = decreaseSpeed;	
	m_gravity = gravity;				
	if(atomSum <= 0) return 0;
	m_atomSum = atomSum;				
	if(texture == NULL || !glIsTexture(texture->m_texture)) return 0;
	m_texture = texture;			
//	m_w = w;
//	m_h = h;

	m_atom = createArrayMem<_XEchoParticles>(m_atomSum);
	if(m_atom == NULL) return 0;
	
	m_isInited = 1;
	m_isEnd = 1;
	return 1;
}

void _XFireWorkParticles::reset()
{
	if(m_isInited == 0 ||
		m_isEnd == 0) return;
	float temp1;
	float temp2;
	float temp3;

	for(int i = 0;i < m_atomSum;++ i)
	{
		m_atom[i].m_isInited = 0;
		m_atom[i].init(m_texture,88,2,m_position,0.95f,0.0015f,256);
		//m_atom[i].init(m_texture,1,2,m_position,0.95f,0.0015f,2);	
		//m_atom[i].setAlpha(0);
		m_atom[i].m_parentParticle.m_initPosition = m_position;
		m_atom[i].m_parentParticle.m_nowPosition = m_position;
		//m_atom[i].m_parentParticle.m_initColor.fA = 0.01;
		m_atom[i].m_parentParticle.m_initColor.setColor(1.0f,1.0f,0.0f,0.01f);
		temp2 = random(4000) * PI/2000.0f;
		temp3 = random(100)/100.0f * 0.75f + 0.25f;
		temp1 = temp3 * sin(temp2);
		temp2 = temp3 * cos(temp2);
		m_atom[i].m_parentParticle.m_dPosition.set(m_initSpeed.x * temp1,m_initSpeed.y * temp2);
		m_atom[i].m_parentParticle.m_stage = STAGE_PRODUCE;		//点燃阶段
		_XVector2 tempA(0.0f,0.0f);
		m_atom[i].m_parentParticle.m_initAngle = tempA.getAngle(m_initSpeed.x * temp1,m_initSpeed.y * temp2) * RADIAN2DEGREE - 90;
	}

	m_isEnd = 0;
}

void _XFireWorkParticles::move(int timeDelay)
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	int flag = 0;
	float tempGravity = m_gravity * timeDelay * 0.00002f;
	for(int i = 0;i < m_atomSum;++ i)
	{
		if(m_atom[i].getIsEnd() != 1) flag = 1; 
		if(m_atom[i].m_parentParticle.m_stage == STAGE_PRODUCE)
		{//产生阶段
			m_atom[i].m_parentParticle.m_initColor.fA += 0.01f * timeDelay;
			if(m_atom[i].m_parentParticle.m_initColor.fA > 1) 
			{
				m_atom[i].m_parentParticle.m_initColor.fA = 1;
				m_atom[i].m_parentParticle.m_stage = STAGE_DESTROY;
			}
			m_atom[i].m_parentParticle.m_dPosition.y += tempGravity;
		//	m_atom[i].m_parentParticle.m_initPosition.x += m_atom[i].m_parentParticle.m_dPosition.x * timeDelay;
		//	m_atom[i].m_parentParticle.m_initPosition.y += m_atom[i].m_parentParticle.m_dPosition.y * timeDelay;
			m_atom[i].m_parentParticle.m_initPosition += m_atom[i].m_parentParticle.m_dPosition * timeDelay;
			m_atom[i].setPosition(m_atom[i].m_parentParticle.m_initPosition);
		}else
		if(m_atom[i].m_parentParticle.m_stage == STAGE_DESTROY)
		{//消失阶段
			m_atom[i].m_parentParticle.m_initColor.fA -= 0.001f * timeDelay;
			if(m_atom[i].m_parentParticle.m_initColor.fA < 0.2f)
			{
				m_atom[i].m_parentParticle.m_initColor.fA = 0;
				m_atom[i].setEnd();
			}else
			{
				m_atom[i].m_parentParticle.m_dPosition.y += tempGravity;
			//	m_atom[i].m_parentParticle.m_initPosition.x += m_atom[i].m_parentParticle.m_dPosition.x * timeDelay;
			//	m_atom[i].m_parentParticle.m_initPosition.y += m_atom[i].m_parentParticle.m_dPosition.y * timeDelay;
				m_atom[i].m_parentParticle.m_initPosition += m_atom[i].m_parentParticle.m_dPosition * timeDelay;
				m_atom[i].setPosition(m_atom[i].m_parentParticle.m_initPosition);
			}
		}
		m_atom[i].move(timeDelay);
	}
	if(flag == 0)
	{
		m_isEnd = 1;
	}
}

void _XFireWorkParticles::draw() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	for(int i = 0;i < m_atomSum;++ i)
	{
		m_atom[i].draw();
	}
}

void _XFireWorkParticles::release()
{
	if(m_isInited == 0) return;
	for(int i = 0;i < m_atomSum;++ i)
	{
		m_atom[i].release();
	}
	XDELETE_ARRAY(m_atom);
	m_isInited = 0;		//防止release之后调用其它成员函数而造成错误
}
