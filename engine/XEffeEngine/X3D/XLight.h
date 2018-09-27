#ifndef _JIA_XLIGHT_
#define _JIA_XLIGHT_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
//这是一个3D场景中的灯光
#include "XBasic3D.h"
namespace XE{
class XLight
{
private:
	XVec4 m_ambient;	//灯的颜色
	XVec4 m_diffuse;	//散射的颜色？
	XVec4 m_position;	//灯光的位置
	XVec4 m_specular;	//目前尚不知道这个是干啥的
	int m_index;			//灯光的编号 [1 - n]

	//射灯的方向
	XVec3 m_lookAtPosition;		//射灯目标点的位置
	XVec3 m_direction;			//射灯的方向
	XMat4 m_projectMatrix;	//射灯的世界矩阵
	XMat4 m_viewMatrix;		//射灯的观察矩阵
	XVec3 m_angle;
	float m_len;
	XBool m_needCalculate;			//是否需要计算
public:
	XMat4 getProjectMatrix() {return m_projectMatrix;}
	XMat4 getViewMatrix() {return m_viewMatrix;}
	XLight();
	void calculate();
	void setLength(float len) 
	{
		m_len = len;
		m_needCalculate = XTrue;
	}
	void setAngle(float x,float y,float z)
	{
		m_angle.set(x,y,z);
		m_needCalculate = XTrue;
	}
	void setIndex(int index) {m_index = index;}
	void setPosition(float x,float y,float z,float w)
	{
		m_position.set(x,y,z,w);
		//m_needCalculate = XTrue;
	}
	void setPosition(const XVec4& p) {m_position = p;}
	XVec3 getPosition() const {return XVec3(m_position.x,m_position.y,m_position.z);}
	void setAmblient(float x,float y,float z,float w)
	{
		m_ambient.set(x,y,z,w);
	}
	void setAmblient(const XVec4& a) {m_ambient = a;}
	void setDiffuse(float x,float y,float z,float w)
	{
		m_diffuse.set(x,y,z,w);
	}
	void setDiffuse(const XVec4& d) {m_diffuse = d;}
	void setSpecular(float x,float y,float z,float w)
	{
		m_specular.set(x,y,z,w);
	}
	void setSpecular(const XVec4& s) {m_specular = s;}
	void useLight();	//使用灯光
	void disLight();
	static void disAllLight()
	{
		glDisable(GL_LIGHTING);
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL_EXT, GL_SINGLE_COLOR_EXT);
	}
};
}
#endif //_JIA_XLIGHT_
