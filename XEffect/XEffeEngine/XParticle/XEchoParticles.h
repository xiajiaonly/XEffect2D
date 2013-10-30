#ifndef _JIA_XECHOPARTICLES_
#define _JIA_XECHOPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XParticle.h"

//最简单最基础的粒子系统模板.残影效果
#define MAX_ECHO_PARTICLES_SUM 512		//残影效果的粒子数量
class _XEchoParticles
{
public:
	char m_isInited;
	char m_isEnd;
private:
	int m_insertSum;	//差值的数量
	int m_density;		//残影密度		说明：残影密度和最大差值是相互制约的，在最大差值的制约下，取符合密度的要求，
	//也就是如果符合密度小于最大差值则符合要求，如果超过最大差值，则取最大差值
	char m_isSetEnd;	//是否设置结束
	int m_nowPoint;		//当前插入点的指针

	//下面的变量是为了优化速度而定义的
	const _XTexture *m_texture;
//	int w;
//	int h;
	int halfW;
	int halfH;
	int m_glListOrder;

	int m_atomSum;
	int m_haveGetmemory;

	char m_blendTypeScr;
	char m_blendTypeDst;
public:
	void setBlendType(int typeScr,int typeDst)
	{
		if(typeScr < 0) typeScr = 0;
		if(typeDst < 0) typeDst = 0;
		if(typeScr >= 9 ) typeScr = 8;
		if(typeDst >= 8 ) typeDst = 7;
		m_blendTypeScr = typeScr;
		m_blendTypeDst = typeDst;
	}

public:
	_XParentParticle m_parentParticle;	//一个母粒子
	_XBasicParticle *m_particles;		//若干个子粒子

	int init(const _XTexture *texture,
		int insertSum,
		int density,
		const _XVector2& position,
		float dSize,
		float dAlpha,
		int atomSum = 0,
		float initSize = 1.0f);	//atomSum = 0 为使用默认值
	void setDSize(float dSize);
	void setDAlpha(float dAlpha);
	void setInsertSum(float insertSum);
	void setDensity(float density);
	void setTexture(const _XTexture *texture)
	{
		if(texture == NULL || !glIsTexture(texture->m_texture)) return;
		m_texture = texture;
	}
	void setPosition(const _XVector2 &position);
	void setPosition(float x,float y);
	void setPosition(float x,float y,float size);
	void setPosition(float x,float y,float size,float udR,float udG = 1.0f,float udB = 1.0f);//float udA = 1.0f);
	void setAlpha(float temp);
	void move(int timeDelay);
	void moveEx(float sizeX,float sizeY,float alpha);	//将数据提取出来，优化了速度
	void draw() const;

	void drawExBegin() const;
	void drawExBody() const;
	void drawExEnd() const;

	_XEchoParticles();
	void release();
	int getMemory(int atomSum);
	void setEnd();
	void reset();
	void resetState(const _XVector2& position,float initSize = 1.0f);	//重新设置初始位置
	int getIsEnd();
};

inline void _XEchoParticles::setEnd()
{
	if(m_isInited != 0 && m_isEnd == 0 && m_isSetEnd == 0) m_isSetEnd = 1;
}

inline void _XEchoParticles::reset()
{
	if(m_isInited != 0 && m_isEnd != 0) 
	{
		m_isEnd = 0;
		m_isSetEnd = 0;
	}
}
inline void _XEchoParticles::resetState(const _XVector2& position,float initSize)	//重新设置初始位置
{
	if(m_isInited == 0) return;
	m_parentParticle.m_initColor.setColor(1.0f,1.0f,1.0f,1.0f);
	m_parentParticle.m_initPosition = position;
	m_parentParticle.m_nowPosition = position;
	m_parentParticle.m_initSize.set(initSize,initSize);
	m_nowPoint = 0;
	for(int i = 0;i < m_atomSum;++ i)
	{
		m_particles[i].m_isEnable = 0;
	}
}

inline int _XEchoParticles::getIsEnd()
{
	return m_isEnd;
}

inline void _XEchoParticles::setAlpha(float temp)
{
	if(temp < 0) m_parentParticle.m_initColor.setA(0.0f);
	else if(temp > 1) m_parentParticle.m_initColor.setA(1.0f);
	else m_parentParticle.m_initColor.setA(temp);
}

inline void _XEchoParticles::setDSize(float dSize)
{
	m_parentParticle.m_dSize.x = dSize;
}

inline void _XEchoParticles::setDAlpha(float dAlpha)
{
	m_parentParticle.m_dColor.setA(dAlpha);
}

inline void _XEchoParticles::setInsertSum(float insertSum)
{
	if(insertSum < 0) insertSum = 0;
	m_insertSum = insertSum;
}

inline void _XEchoParticles::setDensity(float density)
{
	if(density < 1) density = 1;
	m_density = density;
}
inline void _XEchoParticles::setPosition(const _XVector2 &position)
{
	setPosition(position.x,position.y);
}

#endif