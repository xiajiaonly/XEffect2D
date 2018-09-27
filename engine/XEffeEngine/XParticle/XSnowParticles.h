#ifndef _JIA_XSNOWPARTICLES_
#define _JIA_XSNOWPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//雪花缓缓飘落的效果
#include "XEchoParticles.h"
namespace XE{
class XSnowParticles
{
private:
	char m_isInited;				//是否初始化
	char m_isEnd;				//是否结束
	char m_isSetEnd;				//是否设置结束

	XVec2 m_productArea;		//产生范围（生存区域上方的一条直线）
	XRect m_liveArea;			//生存范围
	XVec2 m_flySpeed;			//飘落的速度

	int m_maxAtomSum;			//最大粒子数
	float m_snowDensity;		//雪飘落的密度
//	XEchoParticles *m_atom;	//雪花粒子
	XAloneParticles *m_atom;	//雪花粒子

	const XTexture *m_texture;
	//int m_w;
	//int m_h;
public:
	int init(const XVec2& productArea,const XRect& liveArea,const XVec2& flySpeed,
		int maxAtomSum,float snowDensity,const XTexture *texture);
	void reset();
	void move(float timeDelay);
	void draw() const;
	void release()
	{
		if(m_isInited == 0) return;
		XMem::XDELETE_ARRAY(m_atom);
		m_isInited = 0;		//防止release之后调用其它成员函数而造成错误
	}
	void setEnd()
	{
		if(m_isInited == 0 ||
			m_isEnd != 0 ||
			m_isSetEnd != 0) return;

		m_isSetEnd = 1;
	}
	XSnowParticles();
};
}
#endif