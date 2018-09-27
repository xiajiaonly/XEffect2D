#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XLight.h"
namespace XE{
XLight::XLight()
	:m_index(1)
	, m_needCalculate(XFalse)
	, m_position(0.0f, 0.0f, 0.0f, 0.0f)
	, m_ambient(1.0f, 1.0f, 1.0f, 1.0f)
	, m_diffuse(1.0f, 1.0f, 1.0f, 1.0f)
	, m_specular(0.2f, 0.2f, 0.2f, 1.0f)
{}
void XLight::useLight()	//使用灯光
{
	calculate();
	unsigned int lightName = GL_LIGHT0;
	switch(m_index)
	{
	case 0:lightName = GL_LIGHT0;break;
	case 1:lightName = GL_LIGHT1;break;
	case 2:lightName = GL_LIGHT2;break;
	case 3:lightName = GL_LIGHT3;break;
	case 4:lightName = GL_LIGHT4;break;
	case 5:lightName = GL_LIGHT5;break;
	case 6:lightName = GL_LIGHT6;break;
	case 7:lightName = GL_LIGHT7;break;
	}
	glEnable(lightName);
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL_EXT, GL_SEPARATE_SPECULAR_COLOR_EXT);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, XFColor::gray);//设置全局环境光
	glLightfv(lightName,GL_AMBIENT,m_ambient);			
	glLightfv(lightName,GL_DIFFUSE,m_diffuse);			
	glLightfv(lightName,GL_POSITION,m_position);	
	glLightfv(lightName,GL_SPECULAR,m_specular);		//Set Light1 Specular
	glEnable(GL_LIGHTING);
}
void XLight::disLight()
{
	unsigned int lightName = GL_LIGHT0;
	switch(m_index)
	{
	case 1:lightName = GL_LIGHT1;break;
	case 2:lightName = GL_LIGHT2;break;
	case 3:lightName = GL_LIGHT3;break;
	case 4:lightName = GL_LIGHT4;break;
	case 5:lightName = GL_LIGHT5;break;
	case 6:lightName = GL_LIGHT6;break;
	case 7:lightName = GL_LIGHT7;break;
	}
	glDisable(lightName);
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL_EXT, GL_SINGLE_COLOR_EXT);
}
void XLight::calculate()
{
	if(m_needCalculate) m_needCalculate = XFalse;
	else return;
	float cosX = cos(m_angle.x * DEGREE2RADIAN);
	float cosY = cos(m_angle.y * DEGREE2RADIAN);
	float sinX = sin(m_angle.x * DEGREE2RADIAN);
	float sinY = sin(m_angle.y * DEGREE2RADIAN);
	m_lookAtPosition.z = m_position.z + m_len * cosX * cosY;
	m_lookAtPosition.y = m_position.y + m_len * sinX;
	m_lookAtPosition.x = m_position.x + m_len * cosX * sinY;
	m_direction.z = -sinX * cosY;
	m_direction.y = cosX;
	m_direction.x = -sinX * sinY;
	//这里计算灯的世界矩阵和观察矩阵
	m_projectMatrix = XMath::calPerspectiveMatrix(XEG.getViewAngle(),1.0f,1000.0f,2000.0f);
	m_viewMatrix = XMath::calLookAtMatrix(XVec3(m_position.x,m_position.y,m_position.z),m_lookAtPosition,m_direction);

	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();
	//gluPerspective(XEE::viewAngle3D,1.0f,200.0f, 1500.0f);
	//glGetFloatv(GL_MODELVIEW_MATRIX,m_projectMatrix);	//获取摄像头的世界矩阵
	//glLoadIdentity();
	//gluLookAt(m_position.x,m_position.y,m_position.z,
	//	m_lookAtPosition.x,m_lookAtPosition.y,m_lookAtPosition.z,
	//	m_direction.x,m_direction.y,m_direction.z);
	//glGetFloatv(GL_MODELVIEW_MATRIX,m_viewMatrix);		//获取摄像头的观察矩阵
	//glPopMatrix();
}
}