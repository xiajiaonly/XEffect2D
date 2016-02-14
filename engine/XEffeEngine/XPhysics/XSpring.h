#ifndef XSPRING_
#define XSPRING_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.12.10
//--------------------------------
#include "XPhysicsBasic.h"
namespace XE{
class XSpring:public XBasicPhysic2DObject
{
private:
	float m_normalLength;	//弹簧的自然伸展长度
	float m_minLength;		//弹簧压缩的最小长度（超过最小长度的压缩就成了一个刚体3）
	float m_maxLength;		//弹簧伸展的最大长度（超过这个长度将会不能伸展或者断裂，具体情况视设置而定）
	float m_springRatio;	//弹簧的弹性系数
	float m_curLength;		//弹簧的当前长度

	float m_curForce;		//弹簧当前的力量
public:
	int init();					//初始化弹簧的属性
	int move(float timeDelay);	//弹簧的运动

	XSpring()
	{
	}
};
}
#endif