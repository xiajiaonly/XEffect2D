#ifndef _JIA_XJETPARTICLES_
#define _JIA_XJETPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//喷射的粒子效果，也就是向前进方向的后方的一定角度内按照一定的密度喷射粒子的效果
#include "XEchoParticles.h"

class _XJetParticles
{
private:
	char m_isInited;			//是否初始化
	char m_isEnd;				//是否结束
	char m_isSetEnd;			//是否设置结束

	float m_directionAngle;		//喷射体前进的方向
	float m_jetAngle;			//喷射体喷射的夹角
	float m_jetDensity;			//喷射体喷射的密度
	_XVector2 m_nowPosition;		//喷射体的当前位置

	float m_initSpeed;			//喷射的初始速度

	_XEchoParticles *m_atomBasic;	//喷射的本体效果
	_XEchoParticles *m_atom;		//向后喷射的粒子
	int m_maxAtomSum;				//最大粒子数量
	int m_nowAtomPoint;				//当前使用的粒子的指针	
	int m_timer;

	const _XTexture *m_texture;				//喷射本体的贴图
//	int m_w;
//	int m_h;
	const _XTexture *m_textureAtom;			//喷射粒子的贴图
//	int m_wA;
//	int m_hA;
public:
	int init(const _XVector2& position,float jetSpeed,float jetAngle,float jetDensity,
		const _XTexture *texture,const _XTexture *textureAton = NULL);
	void reset(const _XVector2& position);
	void reset(float x,float y);
	void move(int timeDelay);
	void setPosition(const _XVector2& position);
	void setPosition(float x,float y);
	void draw() const;
	void release();
	void setEnd();

	_XJetParticles();
};

inline void _XJetParticles::setEnd()
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return;
	if(m_isSetEnd != 0) return;
	m_isSetEnd = 1;
	m_atomBasic->setEnd();
}

inline void _XJetParticles::setPosition(const _XVector2& position)
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return;
	//计算角度
	m_directionAngle = m_nowPosition.getAngle(position);
	m_nowPosition = position;
	m_atomBasic->setPosition(position);
}

inline void _XJetParticles::setPosition(float x,float y)
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return;
	//计算角度
	m_directionAngle = m_nowPosition.getAngle(x,y);
	m_nowPosition.set(x,y);
	m_atomBasic->setPosition(x,y);
}

#endif