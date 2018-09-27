#ifndef _JIA_XFIREWORKPARTICLES_
#define _JIA_XFIREWORKPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XEchoParticles.h"
namespace XE{
class XFireWorkParticles
{
private:
	char m_isInited;		//是否初始化
	char m_isEnd;			//是否结束

	XVec2 m_position;		//位置
	XVec2 m_initSpeed;	//初始速度
	float m_decreaseSpeed;	//速度衰减量
	float m_gravity;		//重力加速度
	int m_atomSum;			//粒子的数量

	XEchoParticles *m_atom;//原子
	const XTexture *m_texture;		//粒子的贴图
//	int m_w;	//贴图的宽
//	int m_h;	//贴图的高

public:
	int init(const XVec2& position,const XVec2& initSpeed,float decreaseSpeed,float gravity,int atomSum,const XTexture *texture);					//初始化
	void reset();				//设置动作开始
	void move(float timeDelay);	//改变
	void draw() const;				//描绘
	void release();
	int getIsEnd()
	{
		if(m_isEnd != 0) return 1;
		return 0;
	}
	//设置粒子发生器使用的帖图(注意这个贴图需要与初始化时的贴图使用相同的尺寸)
	void setTexture(const XTexture *texture)
	{
		if(m_isInited == 0) return;
		if(texture == NULL || !glIsTexture(texture->m_texture)) return;
		m_texture = texture;
		for(int i = 0;i < m_atomSum;++ i)
		{
			m_atom[i].setTexture(m_texture);
		}
	}
	//设置粒子发生器的位置
	void setPosition(const XVec2& position) { m_position = position;}

	XFireWorkParticles();		//构造函数
};
}
#endif