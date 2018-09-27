#ifndef _JIA_XCLOUDPARTICLES_
#define _JIA_XCLOUDPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//流云的粒子效果
//需要注意的是,这只是一个效果的实现,目前没有考虑他的通用性
//在后继的扩展中将会考虑他的通用性
#include "XParticle.h"
namespace XE{
class XCloudParticles
{
private:
	char m_isInited;				//是否初始化
	char m_isEnd;				//是否结束
	char m_isSetEnd;				//是否设置结束

	float m_curCenterPoint;		//当前流云产生中心点所在的位置	(变化需要连贯性)
	float m_maxCenterPoint;		//流云产生中心点的最大活动范围
	float m_minCenterPoint;		//流云产生中心点的最小活动范围
	float m_maxSize;			//流云产生范围的上限
	float m_minSize;			//流云产生范围的下限
	float m_curSize;			//流云产生的当前范围	(变化需要连贯性)
	float m_density;			//当前流云产生的密度	(变化需要连贯性)
	XRect m_liveArea;			//流云粒子生存的空间
	XVec2 m_speed;			//流云移动的速度	(变化范围不能太大)

	int m_maxAtomSum;			//最大粒子数
	XAloneParticles *m_atom;	//粒子

	const XTexture *m_texture;			//贴图
//	int m_w;
//	int m_h;

public:
	int init(float maxCenterPoint,float minCenterPoint,float maxSize,float minSize,
		float density,const XRect& liveArea,const XVec2& speed,int maxAtomSum,const XTexture *tex);
	void reset();
	void move(float timeDelay);
	void draw() const;
	void release();
	void setEnd();
	XCloudParticles();
};
inline void XCloudParticles::release()
{
	if(m_isInited == 0) return;
	XMem::XDELETE_ARRAY(m_atom);
	m_isInited = 0;		//防止release之后调用其它成员函数而造成错误
}

inline void XCloudParticles::setEnd()
{
	if(m_isInited == 0 ||
		m_isEnd != 0 ||
		m_isSetEnd != 0) return;
	m_isSetEnd = 1;
}
}
#endif