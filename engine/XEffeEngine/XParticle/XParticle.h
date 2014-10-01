#ifndef _JIA_XPARTICLE_
#define _JIA_XPARTICLE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "../XBasicClass.h"
#include "gl.h"
#include "../XSprite.h"
#include "../XBasicOpenGL.h"

//基本粒子中的子粒子，是粒子系统中的最多的基本粒子构成成分
class _XBasicParticle
{
public:
	char m_isEnable;			//基本粒子是否有效

	_XVector2 m_initPosition;	//粒子诞生时的位置
	_XVector2 m_initSize;		//粒子诞生时的尺寸
	float m_initAngle;		//粒子诞生时的角度
	_XFColor m_initColor;	//粒子诞生时的初始颜色

	_XVector2 m_dPosition;	//粒子位置变化的变化量
	_XVector2 m_dSize;		//粒子尺寸变化的变化量
	float m_dAngle;			//粒子角度变化的变化量
	_XFColor m_dColor;		//粒子颜色变化的变化量

	_XVector2 m_nowPosition;	//粒子的当前位置
	_XVector2 m_nowSize;		//粒子的当前尺寸
	float m_nowAngle;		//粒子的当前角度
	_XFColor m_nowColor;	//粒子的当前颜色
	_XBasicParticle()
		:m_isEnable(0)
	{
	}
	virtual~_XBasicParticle(){}
};

enum
{
	STAGE_SLEEP,	//粒子处于休息状态，休息状态不显示也不运动
	STAGE_MOVE,		//粒子处于运动状态，
	STAGE_STILL,	//粒子处于静止状态，静止状态只显示不运动
	STAGE_STEALTHY	//粒子处于隐身状态，隐身状态只运动不显示
};

//母粒子，粒子系统中最小团体的母体
class _XParentParticle:public _XBasicParticle
{
public:
	char m_stage;		//粒子阶段标记

	_XParentParticle();
	virtual~_XParentParticle(){}
};

class _XAloneParticles:public _XParentParticle
{
public:
	char m_isInited;
private:
	const _XTexture *m_texture;	//粒子的贴图
//	int m_w;
//	int m_h;

	int halfW;
	int halfH;
	int m_glListOrder;
public:
	int init(const _XTexture *texture);
	void draw() const;
	_XAloneParticles();
	
	void move(int timeDelay);
};

inline void _XAloneParticles::move(int timeDelay)
{
	if(m_isInited == 0 ||
		m_stage == STAGE_SLEEP) return;
	//颜色的变化
	//尺寸的变化
	//位置的变化
//	m_nowPosition.x += m_dPosition.x * timeDelay;
//	m_nowPosition.y += m_dPosition.y * timeDelay;
	m_nowPosition += m_dPosition * (float)timeDelay;
}

#endif