#ifndef _JIA_XECHOPARTICLES_
#define _JIA_XECHOPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XParticle.h"
#if WITH_LOG
#else
#include <vector>
#endif
#define WITH_DLIST
namespace XE{
//最简单最基础的粒子系统模板.残影效果
class XEchoParticles
{
public:
	bool m_isInited;
	bool m_isEnd;
private:
	bool m_isSetEnd;	//是否设置结束
	bool m_haveGetmemory;
	static const int m_maxEchoParticlesSum = 16;		//残影效果的粒子数量

	XGL::XBlendModel m_blendModel;
	int m_insertSum;	//插值的数量
	int m_density;		//残影密度		说明：残影密度和最大插值是相互制约的，在最大插值的制约下，取符合密度的要求，
	//也就是如果符合密度小于最大插值则符合要求，如果超过最大插值，则取最大插值
	int m_curPoint;		//当前插入点的指针

	//下面的变量是为了优化速度而定义的
	const XTexture *m_texture;
	int w;
	int h;
	int halfW;
	int halfH;
#ifdef WITH_DLIST
	int m_glListOrder;
#endif

	int m_atomSum;
public:
	void setBlendModel(XGL::XBlendModel model) { m_blendModel = model; }
public:
	XParentParticle m_parentParticle;	//一个母粒子
	XBasicParticle *m_particles;		//若干个子粒子

	int getAtomSum()const { return m_atomSum; }
	bool init(const XTexture* texture,
		int insertSum,		//中间插入的数量
		int density,		//密度
		const XVec2& position,
		float dSize,		//大小变化的量
		float dAlpha,		//透明度变化的量
		int atomSum = 0,
		float initSize = 1.0f);	//atomSum = 0 为使用默认值
	void setDSize(float dSize);
	void setDAlpha(float dAlpha);
	void setInsertSum(float insertSum);
	void setDensity(float density);
	void setTexture(const XTexture *texture)
	{
		if(texture == NULL || !glIsTexture(texture->m_texture)) return;
		m_texture = texture;
	}
	void setPosition(const XVec2& position);
	void setPosition(float x,float y);
	void setPosition(float x,float y,float size);
	void setPosition(float x,float y,float size,float udR,float udG = 1.0f,float udB = 1.0f);//float udA = 1.0f);
	void setAlpha(float temp);
	void setColor(float r,float g,float b);
	void move(float timeDelay)
	{
		if (!m_isInited || m_isEnd) return;
		_moveEx(pow(m_parentParticle.m_dSize.x, (timeDelay * 0.05f)), 
			m_parentParticle.m_dColor.a * timeDelay);
	}
	void moveEx(float sizeX, float alpha)	//将数据提取出来，优化了速度
	{
		if (!m_isInited || m_isEnd) return;
		_moveEx(sizeX, alpha);
	}
	void _moveEx(float sizeX,float alpha);	//将数据提取出来，优化了速度
	//描绘粒子
	void draw() const;
	//下面这组方法效率高，补贴贴图，忽略旋转和缩放
	void drawAsLine() const;
	//下面这组方法效率较高，效果不影响
	void drawExBegin() const;
	void drawExBody() const;
	void getDrawData(std::vector<float> &v,std::vector<float> &t,std::vector<float> &c) const;	//这个不考虑旋转，使用vbo绘图
	void getDrawData(std::vector<float> &v,std::vector<float> &c) const;	//这个不考虑旋转，使用vbo绘图
	void getDrawData(float* v, float* c, int& sum) const;	//这个不考虑旋转，使用vbo绘图
	void drawExEnd() const
	{
		glPopMatrix();
		XGL::DisableBlend();
	}
	//下面这一组方法效率最高，但是忽略旋转
	void drawExBeginEx() const;
	void drawExBodyEx() const;	//不考虑尺寸和角度的因素
	void drawExEndEx() const
	{
		glEnd();
		XGL::DisableBlend();
	}

	XEchoParticles();
	void release();
	bool getMemory(int atomSum);
	void setEnd();
	void reset();
	void resetState(const XVec2& position,float initSize = 1.0f);	//重新设置初始位置
	void resetStateEx(const XVec2& position);	//重新设置初始位置
	bool getIsEnd()const { return m_isEnd; }
};
inline void XEchoParticles::setEnd()
{
	if (m_isInited && !m_isEnd && !m_isSetEnd) m_isSetEnd = true;
}
inline void XEchoParticles::reset()
{
	if (m_isInited && m_isEnd)
	{
		m_isEnd = false;
		m_isSetEnd = false;
	}
}
inline void XEchoParticles::resetState(const XVec2& position,float initSize)	//重新设置初始位置
{
	if(!m_isInited) return;
	m_parentParticle.m_initColor.set(1.0f,1.0f);
	m_parentParticle.m_initPosition = position;
	m_parentParticle.m_curPosition = position;
	m_parentParticle.m_initSize.set(initSize);
	m_curPoint = 0;
	for(int i = 0;i < m_atomSum;++ i)
	{
		m_particles[i].m_isEnable = false;
	}
}
inline void XEchoParticles::resetStateEx(const XVec2& position)	//重新设置初始位置
{
	//if(!m_isInited) return;
	//m_parentParticle.m_initColor.set(1.0f,1.0f);
	m_parentParticle.m_initPosition = m_parentParticle.m_curPosition = position;
	//m_parentParticle.m_initSize.set(initSize);
	m_curPoint = 0;
	m_particles[m_atomSum - 1].m_isEnable = false;
}
inline void XEchoParticles::setAlpha(float temp)
{
	if(temp < 0) m_parentParticle.m_initColor.setA(0.0f);
	else if(temp > 1) m_parentParticle.m_initColor.setA(1.0f);
	else m_parentParticle.m_initColor.setA(temp);
}
inline void XEchoParticles::setColor(float r,float g,float b)
{
	m_parentParticle.m_initColor.setR(r);
	m_parentParticle.m_initColor.setG(g);
	m_parentParticle.m_initColor.setB(b);
}
inline void XEchoParticles::setDSize(float dSize)
{
	m_parentParticle.m_dSize.x = dSize;
}
inline void XEchoParticles::setDAlpha(float dAlpha)
{
	m_parentParticle.m_dColor.setA(dAlpha);
}
inline void XEchoParticles::setInsertSum(float insertSum)
{
	if(insertSum < 0.0f) insertSum = 0.0f;
	m_insertSum = insertSum;
}
inline void XEchoParticles::setDensity(float density)
{
	if(density < 1) density = 1;
	m_density = density;
}
inline void XEchoParticles::setPosition(const XVec2& position)
{
	setPosition(position.x,position.y);
}
}
#endif