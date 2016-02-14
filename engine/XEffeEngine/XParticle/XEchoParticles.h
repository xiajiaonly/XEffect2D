#ifndef _JIA_XECHOPARTICLES_
#define _JIA_XECHOPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XParticle.h"
#define WITH_DLIST
namespace XE{
//最简单最基础的粒子系统模板.残影效果
class XEchoParticles
{
public:
	char m_isInited;
	char m_isEnd;
private:
	static const int m_maxEchoParticlesSum = 16;		//残影效果的粒子数量
	char m_isSetEnd;	//是否设置结束
	char m_blendRGBScr;
	char m_blendRGBDst;
	char m_blendAScr;
	char m_blendADst;
	char m_blendTypeScr;
	char m_blendTypeDst;
	XGL::XBlendType m_blendType;	//混合方式：0：简单混合2因子混合、1：复杂的4因子混合
	int m_insertSum;	//差值的数量
	int m_density;		//残影密度		说明：残影密度和最大差值是相互制约的，在最大差值的制约下，取符合密度的要求，
	//也就是如果符合密度小于最大差值则符合要求，如果超过最大差值，则取最大差值
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
	int m_haveGetmemory;
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
	void setBlendType(int rgbScr,int rgbDst,int aScr,int aDst)
	{
		if(rgbScr < 0) rgbScr = 0;
		if(rgbScr >= 9 ) rgbScr = 8;
		if(aScr < 0) aScr = 0;
		if(aScr >= 9 ) aScr = 8;

		if(rgbDst < 0) rgbDst = 0;
		if(rgbDst >= 8 ) rgbDst = 7;
		if(aDst < 0) aDst = 0;
		if(aDst >= 8 ) aDst = 7;
		m_blendRGBScr = rgbScr;
		m_blendRGBDst = rgbDst;
		m_blendAScr = aScr;
		m_blendADst = aDst;
	}
	void setBlendMode(XGL::XBlendType mode) {m_blendType = mode;}

public:
	XParentParticle m_parentParticle;	//一个母粒子
	XBasicParticle *m_particles;		//若干个子粒子

	int init(const XTexture *texture,
		int insertSum,
		int density,
		const XVector2& position,
		float dSize,
		float dAlpha,
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
	void setPosition(const XVector2 &position);
	void setPosition(float x,float y);
	void setPosition(float x,float y,float size);
	void setPosition(float x,float y,float size,float udR,float udG = 1.0f,float udB = 1.0f);//float udA = 1.0f);
	void setAlpha(float temp);
	void move(float timeDelay);
	void moveEx(float sizeX,float sizeY,float alpha);	//将数据提取出来，优化了速度
	//描绘粒子
	void draw() const;
	//下面这组方法效率高，补贴贴图，忽略旋转和缩放
	void drawAsLine() const;
	//下面这组方法效率较高，效果不影响
	void drawExBegin() const;
	void drawExBody() const;
	void getDrawData(std::vector<float> &v,std::vector<float> &t,std::vector<float> &c) const;	//这个不考虑旋转，使用vbo绘图
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
	int getMemory(int atomSum);
	void setEnd();
	void reset();
	void resetState(const XVector2& position,float initSize = 1.0f);	//重新设置初始位置
	int getIsEnd();
};
inline void XEchoParticles::setEnd()
{
	if(m_isInited != 0 && m_isEnd == 0 && m_isSetEnd == 0) m_isSetEnd = 1;
}
inline void XEchoParticles::reset()
{
	if(m_isInited != 0 && m_isEnd != 0) 
	{
		m_isEnd = 0;
		m_isSetEnd = 0;
	}
}
inline void XEchoParticles::resetState(const XVector2& position,float initSize)	//重新设置初始位置
{
	if(m_isInited == 0) return;
	m_parentParticle.m_initColor.setColor(1.0f,1.0f,1.0f,1.0f);
	m_parentParticle.m_initPosition = position;
	m_parentParticle.m_curPosition = position;
	m_parentParticle.m_initSize.set(initSize,initSize);
	m_curPoint = 0;
	for(int i = 0;i < m_atomSum;++ i)
	{
		m_particles[i].m_isEnable = 0;
	}
}
inline int XEchoParticles::getIsEnd()
{
	return m_isEnd;
}
inline void XEchoParticles::setAlpha(float temp)
{
	if(temp < 0) m_parentParticle.m_initColor.setA(0.0f);
	else if(temp > 1) m_parentParticle.m_initColor.setA(1.0f);
	else m_parentParticle.m_initColor.setA(temp);
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
	if(insertSum < 0) insertSum = 0;
	m_insertSum = insertSum;
}
inline void XEchoParticles::setDensity(float density)
{
	if(density < 1) density = 1;
	m_density = density;
}
inline void XEchoParticles::setPosition(const XVector2 &position)
{
	setPosition(position.x,position.y);
}
}
#endif