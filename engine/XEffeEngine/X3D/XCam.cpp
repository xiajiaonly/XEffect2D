//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XEffeEngine.h"
#include "XCam.h"
#include "XBasic3D.h"

_XBool _XCamRoam::calculate()
{
	if(m_needCalculate) m_needCalculate = XFalse;
	else return XFalse;
	float sinX = sin(m_angle.x * DEGREE2RADIAN);
	float cosX = cos(m_angle.x * DEGREE2RADIAN);
	float sinY = sin(m_angle.y * DEGREE2RADIAN);
	float cosY = cos(m_angle.y * DEGREE2RADIAN);
	float sinZ = sin(m_angle.z * DEGREE2RADIAN);
	float cosZ = cos(m_angle.z * DEGREE2RADIAN);
	//不考虑z角度的影响
	//m_lookAtPosition.z = m_position.z + m_len * cosX * cosY;
	//m_lookAtPosition.y = m_position.y + m_len * sinX;
	//m_lookAtPosition.x = m_position.x + m_len * cosX * sinY;
	//m_direction.z = -sinX * cosY;
	//m_direction.y = cosX;
	//m_direction.x = -sinX * sinY;
	//完整正确的值
	m_lookAtPosition.z = m_position.z + m_len * cosX * cosY;
	m_lookAtPosition.y = m_position.y + m_len * (sinX * cosZ - cosX * sinY * sinZ);
	m_lookAtPosition.x = m_position.x + m_len * (cosX * sinY * cosZ + sinX * sinZ);
	m_direction.z = -sinX * cosY;
	m_direction.y = cosX * cosZ + sinX * sinY * sinZ;
	m_direction.x = -sinX * sinY * cosZ + cosX * sinZ;

	//这里计算灯的世界矩阵和观察矩阵
	m_fov = XEE::viewAngle3D;
	m_projectMatrix = calPerspectiveMatrix(m_fov,(float)XEE::windowWidth/XEE::windowHeight,m_near,m_far);
	m_viewMatrix = calLookAtMatrix(m_position,m_lookAtPosition,m_direction);

	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();
	//gluPerspective(XEE::viewAngle3D,(float)XEE::windowWidth/XEE::windowHeight,m_near,m_far);
	//glGetFloatv(GL_MODELVIEW_MATRIX,m_projectMatrix);	//获取摄像头的世界矩阵//GL_PROJECTION_MATRIX
	//glLoadIdentity();
	//gluLookAt(m_position.x,m_position.y,m_position.z,
	//	m_lookAtPosition.x,m_lookAtPosition.y,m_lookAtPosition.z,
	//	m_direction.x,m_direction.y,m_direction.z);
	//glGetFloatv(GL_MODELVIEW_MATRIX,m_viewMatrix);		//获取摄像头的观察矩阵
	//glPopMatrix();
	m_neadCalProjXView = XTrue;
	return XTrue;
}
void _XCamRoam::calculateFrustumPlanes() //计算视锥体
{
	_XMatrix4x4 tempM = m_projectMatrix * m_viewMatrix;
	m_face[0] = normalize(tempM.getRow(3) - tempM.getRow(0));	//right
	m_face[1] = normalize(tempM.getRow(3) + tempM.getRow(0));	//left
	m_face[2] = normalize(tempM.getRow(3) - tempM.getRow(1));	//top
	m_face[3] = normalize(tempM.getRow(3) + tempM.getRow(1));	//bottom
	m_face[4] = normalize(tempM.getRow(3) - tempM.getRow(2));	//far
	m_face[5] = normalize(tempM.getRow(3) + tempM.getRow(2));	//near
//	m_face[4] = normalize(tempM.getRow(2) - tempM.getRow(3));	//far
//	m_face[5] = normalize(_XVector4::zero - tempM.getRow(2) - tempM.getRow(3));	//near
}
bool _XCamRoam::isInFrustum(float x,float y,float z)  
{//判断点是否在视锥体内部  
	if(m_neadCalFrustum)
	{
		m_neadCalFrustum = XFalse;
		calculate();
		calculateFrustumPlanes();
		calculateFrustumPoint();
	}
	for(int i = 0;i < 6;++ i)  
	{  
		if(m_face[i].x * x + m_face[i].y * y +  
			m_face[i].z * z + m_face[i].w <= 0)  
			return false; 
	}
	return true;  
}
void _XCamRoam::calculateFrustumPoint()
{
	m_point[0] = getPoint(m_face[0],m_face[2],m_face[4]);
	m_point[1] = getPoint(m_face[0],m_face[3],m_face[4]);
	m_point[2] = getPoint(m_face[1],m_face[3],m_face[4]);
	m_point[3] = getPoint(m_face[1],m_face[2],m_face[4]);
	m_point[4] = getPoint(m_face[0],m_face[2],m_face[5]);
	m_point[5] = getPoint(m_face[0],m_face[3],m_face[5]);
	m_point[6] = getPoint(m_face[1],m_face[3],m_face[5]);
	m_point[7] = getPoint(m_face[1],m_face[2],m_face[5]);
	m_point[8] = m_position;
}
void _XCamRoam::drawFrustum()	//描绘视锥体
{
	if(m_neadCalFrustum)
	{
		m_neadCalFrustum = XFalse;
		calculate();
		calculateFrustumPlanes();
		calculateFrustumPoint();
	}
	drawLine(m_point[0],m_point[1]);
	drawLine(m_point[1],m_point[2]);
	drawLine(m_point[2],m_point[3]);
	drawLine(m_point[3],m_point[0]);
	drawLine(m_point[4],m_point[5]);
	drawLine(m_point[5],m_point[6]);
	drawLine(m_point[6],m_point[7]);
	drawLine(m_point[7],m_point[4]);
	drawLine(m_point[0],m_point[4]);
	drawLine(m_point[1],m_point[5]);
	drawLine(m_point[2],m_point[6]);
	drawLine(m_point[3],m_point[7]);
	drawLine(m_point[8],m_point[4]);
	drawLine(m_point[8],m_point[5]);
	drawLine(m_point[8],m_point[6]);
	drawLine(m_point[8],m_point[7]);
}