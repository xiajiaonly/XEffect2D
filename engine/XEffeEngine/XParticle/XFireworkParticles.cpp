#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XFireworkParticles.h"
namespace XE{
enum
{
	STAGE_PRODUCE,	//产生阶段
	STAGE_DESTROY	//消亡阶段
};
XFireWorkParticles::XFireWorkParticles()
	:m_isInited(0)
	, m_isEnd(0)
{}
int XFireWorkParticles::init(const XVec2& position, const XVec2& initSpeed, float decreaseSpeed,
	float gravity, int atomSum, const XTexture *texture)
{
	if (m_isInited != 0) return 0;

	m_position = position;
	m_initSpeed = initSpeed;
	m_decreaseSpeed = decreaseSpeed;
	m_gravity = gravity;
	if (atomSum <= 0) return 0;
	m_atomSum = atomSum;
	if (texture == NULL || !glIsTexture(texture->m_texture)) return 0;
	m_texture = texture;
	//	m_w = w;
	//	m_h = h;

	m_atom = XMem::createArrayMem<XEchoParticles>(m_atomSum);
	if (m_atom == NULL) return 0;

	m_isInited = m_isEnd = 1;
	return 1;
}
void XFireWorkParticles::reset()
{
	if (m_isInited == 0 ||
		m_isEnd == 0) return;
	float temp2;
	XVec2 tmp;

	for (int i = 0; i < m_atomSum; ++i)
	{
		m_atom[i].m_isInited = 0;
		m_atom[i].init(m_texture, 88, 2, m_position, 0.95f, 0.0015f * XRand::randomf(0.75f, 1.25f), 256);
		//m_atom[i].init(m_texture,1,2,m_position,0.95f,0.0015f,2);	
		//m_atom[i].setAlpha(0);
		m_atom[i].m_parentParticle.m_initPosition = m_position;
		m_atom[i].m_parentParticle.m_curPosition = m_position;
		//m_atom[i].m_parentParticle.m_initColor.a = 0.01;
		m_atom[i].m_parentParticle.m_initColor.set(1.0f, 1.0f, 0.0f, 0.01f);
		temp2 = XRand::randomf(2.0f) * PI;
		tmp = XVec2(sin(temp2), cos(temp2)) * XRand::randomf(1.0f, 0.25f);
		m_atom[i].m_parentParticle.m_dPosition = m_initSpeed * tmp;
		m_atom[i].m_parentParticle.m_stage = STAGE_PRODUCE;		//点燃阶段
		XVec2 tempA(0.0f);
		m_atom[i].m_parentParticle.m_initAngle = tempA.getAngle(m_initSpeed * tmp) * RADIAN2DEGREE - 90;
	}

	m_isEnd = 0;
}
void XFireWorkParticles::move(float timeDelay)
{
	if (m_isInited == 0 ||
		m_isEnd != 0) return;
	int flag = 0;
	float tempGravity = m_gravity * timeDelay * 0.00002f;
	for (int i = 0; i < m_atomSum; ++i)
	{
		if (m_atom[i].getIsEnd() != 1) flag = 1;
		XEchoParticles& ref = m_atom[i];
		switch (ref.m_parentParticle.m_stage)
		{
		case STAGE_PRODUCE:
			ref.m_parentParticle.m_initColor.a += 0.01f * timeDelay;
			if (ref.m_parentParticle.m_initColor.a > 1)
			{
				ref.m_parentParticle.m_initColor.a = 1;
				ref.m_parentParticle.m_stage = STAGE_DESTROY;
			}
			ref.m_parentParticle.m_dPosition.y += tempGravity;
		//	ref.m_parentParticle.m_initPosition.x += ref.m_parentParticle.m_dPosition.x * timeDelay;
		//	ref.m_parentParticle.m_initPosition.y += ref.m_parentParticle.m_dPosition.y * timeDelay;
			ref.m_parentParticle.m_initPosition += ref.m_parentParticle.m_dPosition * timeDelay;
			ref.setPosition(ref.m_parentParticle.m_initPosition);
			break;
		case STAGE_DESTROY:
			ref.m_parentParticle.m_initColor.a -= 0.001f * timeDelay;
			if (ref.m_parentParticle.m_initColor.a < 0.2f)
			{
				ref.m_parentParticle.m_initColor.a = 0;
				ref.setEnd();
			}
			else
			{
				ref.m_parentParticle.m_dPosition.y += tempGravity;
			//	ref.m_parentParticle.m_initPosition.x += ref.m_parentParticle.m_dPosition.x * timeDelay;
			//	ref.m_parentParticle.m_initPosition.y += ref.m_parentParticle.m_dPosition.y * timeDelay;
				ref.m_parentParticle.m_initPosition += ref.m_parentParticle.m_dPosition * timeDelay;
				ref.setPosition(ref.m_parentParticle.m_initPosition);
			}
			break;
		}
		ref.move(timeDelay);
	}
	if (flag == 0)
		m_isEnd = 1;
}
void XFireWorkParticles::draw() const
{
	if (m_isInited == 0 ||
		m_isEnd != 0) return;
	//m_atom[0].drawExBegin();
	//for (int i = 0; i < m_atomSum; ++i)
	//{
	//	//m_atom[i].draw();
	//	m_atom[i].drawExBody();
	//}
	//m_atom[0].drawExEnd();
	m_atom[0].drawExBeginEx();
	for (int i = 0; i < m_atomSum; ++i)
	{
		//m_atom[i].draw();
		m_atom[i].drawExBodyEx();
	}
	m_atom[0].drawExEndEx();
}
void XFireWorkParticles::release()
{
	if (m_isInited == 0) return;
	for (int i = 0; i < m_atomSum; ++i)
	{
		m_atom[i].release();
	}
	XMem::XDELETE_ARRAY(m_atom);
	m_isInited = 0;		//防止release之后调用其它成员函数而造成错误
}
}