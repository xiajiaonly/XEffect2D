#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
//#include "XEffeEngine.h"
#include "XCam.h"
#include "XBasic3D.h"
namespace XE{
void XCamRoam::setCamState(const XVec3& pos, const XVec3& lookAt, const XVec3& dir)
{
	m_rotateMat = XMath::getRotateMatByLookAt(pos, lookAt, dir);
	m_position = pos;
	m_lookAtPosition = lookAt;
	m_direction = dir;
	m_projectMatrix = XMath::calPerspectiveMatrix(m_fov, (float)getWindowWidth() / getWindowHeight(), m_near, m_far);
	m_viewMatrix = XMath::calLookAtMatrix(m_position, m_lookAtPosition, m_direction);
}
XBool XCamRoam::calculate()
{
	if (m_needCalculate) m_needCalculate = XFalse;
	else return XFalse;
#ifdef WITH_LEFT_MULT
	m_lookAtPosition = m_position + XMath::transform3x3(XVec3(0.0f, 0.0f, m_len), m_rotateMat);
	m_direction = XMath::transform3x3(XVec3(0.0f, 1.0f, 0.0f), m_rotateMat);
#else
	m_lookAtPosition = m_position + m_rotateMat * XVec3(0, 0, m_len);
	m_direction = m_rotateMat * XVec3(0, 1, 0);
#endif

	//方案1：通过计算自己的方式计算数据
	//这里计算灯的世界矩阵和观察矩阵
	m_projectMatrix = XMath::calPerspectiveMatrix(m_fov, (float)getWindowWidth() / getWindowHeight(), m_near, m_far);
	m_viewMatrix = XMath::calLookAtMatrix(m_position, m_lookAtPosition, m_direction);
	//方式2：通过OpenGL计算并获取计算结果
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();
	//gluPerspective(m_fov,(float)getWindowWidth()/getWindowHeight(),m_near,m_far);
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
void XCamRoam::calculateFrustumPlanes() //计算视锥体
{
	XMat4 tempM = m_projectMatrix * m_viewMatrix;
	m_face[0] = XMath::normalize(tempM.getRow(3) - tempM.getRow(0));	//right
	m_face[1] = XMath::normalize(tempM.getRow(3) + tempM.getRow(0));	//left
	m_face[2] = XMath::normalize(tempM.getRow(3) - tempM.getRow(1));	//top
	m_face[3] = XMath::normalize(tempM.getRow(3) + tempM.getRow(1));	//bottom
	m_face[4] = XMath::normalize(tempM.getRow(3) - tempM.getRow(2));	//far
	m_face[5] = XMath::normalize(tempM.getRow(3) + tempM.getRow(2));	//near
//	m_face[4] = normalize(tempM.getRow(2) - tempM.getRow(3));	//far
//	m_face[5] = normalize(XVec4::zero - tempM.getRow(2) - tempM.getRow(3));	//near
}
bool XCamRoam::isInFrustum(float x, float y, float z)
{//判断点是否在视锥体内部  
	if (m_neadCalFrustum)
	{
		m_neadCalFrustum = XFalse;
		calculate();
		calculateFrustumPlanes();
		calculateFrustumPoint();
	}
	for (int i = 0; i < 6; ++i)
	{
		if (m_face[i].x * x + m_face[i].y * y +
			m_face[i].z * z + m_face[i].w <= 0.0f)
			return false;
	}
	return true;
}
void XCamRoam::calculateFrustumPoint()
{
	m_point[0] = XMath::getPoint(m_face[0], m_face[2], m_face[4]);
	m_point[1] = XMath::getPoint(m_face[0], m_face[3], m_face[4]);
	m_point[2] = XMath::getPoint(m_face[1], m_face[3], m_face[4]);
	m_point[3] = XMath::getPoint(m_face[1], m_face[2], m_face[4]);
	m_point[4] = XMath::getPoint(m_face[0], m_face[2], m_face[5]);
	m_point[5] = XMath::getPoint(m_face[0], m_face[3], m_face[5]);
	m_point[6] = XMath::getPoint(m_face[1], m_face[3], m_face[5]);
	m_point[7] = XMath::getPoint(m_face[1], m_face[2], m_face[5]);
	m_point[8] = m_position;
}
void XCamRoam::drawFrustum()	//描绘视锥体
{
	if (m_neadCalFrustum)
	{
		m_neadCalFrustum = XFalse;
		calculate();
		calculateFrustumPlanes();
		calculateFrustumPoint();
	}
	XGL::setBlendAlpha();

	glColor4fv(XFColor::white);
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glVertex3fv(m_point[0]); glVertex3fv(m_point[1]);
	glVertex3fv(m_point[1]); glVertex3fv(m_point[2]);
	glVertex3fv(m_point[2]); glVertex3fv(m_point[3]);
	glVertex3fv(m_point[3]); glVertex3fv(m_point[0]);

	glVertex3fv(m_point[4]); glVertex3fv(m_point[5]);
	glVertex3fv(m_point[5]); glVertex3fv(m_point[6]);
	glVertex3fv(m_point[6]); glVertex3fv(m_point[7]);
	glVertex3fv(m_point[7]); glVertex3fv(m_point[4]);

	glVertex3fv(m_point[0]); glVertex3fv(m_point[4]);
	glVertex3fv(m_point[1]); glVertex3fv(m_point[5]);
	glVertex3fv(m_point[2]); glVertex3fv(m_point[6]);
	glVertex3fv(m_point[3]); glVertex3fv(m_point[7]);

	glVertex3fv(m_point[8]); glVertex3fv(m_point[4]);
	glVertex3fv(m_point[8]); glVertex3fv(m_point[5]);
	glVertex3fv(m_point[8]); glVertex3fv(m_point[6]);
	glVertex3fv(m_point[8]); glVertex3fv(m_point[7]);
	glEnd();
	XGL::DisableBlend();
}
XVec3 XCamRoam::worldToScreen(const XVec3& pos, const XRect& view)
{
	//XMat4 temp = m_worldCam.getProjectMatrix() * m_worldCam.getViewMatrix();
	XVec3 cameraXYZ = pos * getProjXView();
	return XVec3((cameraXYZ.x + 1.0f) * 0.5f * view.getWidth() + view.left,
		(1.0f - cameraXYZ.y) * 0.5f * view.getHeight() + view.top,
		(1.0 + cameraXYZ.z) * 0.5f);

	//double x,y,z;
	//int viewport[4];
	//viewport[0] = view.left;
	//viewport[1] = view.top;
	//viewport[2] = view.right;
	//viewport[3] = view.bottom;
	//double mvmatrix[16],projmatrix[16];
	//for(int i = 0;i < 16;++ i)
	//{
	//	mvmatrix[i] = m_worldCam.getViewMatrix().data[i];
	//	projmatrix[i] = m_worldCam.getProjectMatrix().data[i];
	//}

	//gluProject(point.x,point.y,point.z,mvmatrix,projmatrix,viewport,&x,&y,&z);
	//return XVec3(x, view.bottom - y + view.top, z);
}
}