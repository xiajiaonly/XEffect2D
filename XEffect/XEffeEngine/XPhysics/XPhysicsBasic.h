#ifndef _XPHYSICSBASIC_
#define _XPHYSICSBASIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XBasicClass.h"

//extern _XVector2 constGravity(0,9.82);		//重力常量

union _XPhYsicObjectAttribute
{
	unsigned short canMove:1;		//物体是否能移动
	unsigned short canCollide:1;	//物体是否能碰撞
	unsigned short canFlag00:1;		//标志
	unsigned short canFlag01:1;		//标志
	unsigned short canFlag02:1;		//标志
	unsigned short canFlag03:1;		//标志
	unsigned short canFlag04:1;		//标志
	unsigned short canFlag05:1;		//标志
	unsigned short canFlag06:1;		//标志
	unsigned short canFlag07:1;		//标志
	unsigned short canFlag08:1;		//标志
	unsigned short canFlag09:1;		//标志
	unsigned short canFlag0b:1;		//标志
	unsigned short canFlag0c:1;		//标志
	unsigned short canFlag0d:1;		//标志
};

class _XBasicPhysic2DObject
{
private:
	char m_isEnable;						//物体是否有效
	_XPhYsicObjectAttribute m_attribute;	//物体的一些属性，比如说是否能运动，是否可以穿透等
	float m_mass;							//物体的质量
	_XVector2 m_speed;						//物体的速度
	float m_density;						//物体的密度
	float m_volume;							//物体的体积
	float m_collideEnergyWastageRadio;		//物体碰撞时的能量损耗系数[0 - 1]
	float m_airResistanceRadio;				//运动过程中由于空气阻力系数：飞行的速度 × 空气阻力系数 = 空气阻力
	float m_surfaceFrictionCoefficient;		//物体的表面摩擦系数，当这个物体在别的物体表面运动时这个摩擦系数与接触面的摩擦系数相乘或者总的摩擦系数

	_XVector2 m_position;					//物体现在的位置
	float m_rotate;							//物体的旋转角度
	_XVector2 m_centreOfGravity;			//重心的位置（默认在体心）
	float m_rotationSpeed;					//自身旋转的角速度
public:
	_XBasicPhysic2DObject()
	{
	}
};

#endif