#ifndef _JIA_XCAM_
#define _JIA_XCAM_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
//#include "XBasicFun.h"
//#include "XBasicClass.h"
#include "glu.h"
#include "../XMath/XVector3.h"
#include "../XMath/XMatrix4x4.h"
#define WITH_LEFT_MULT	//适配OpenCV的标定外参时需要取消这个宏
namespace XE{
//fov为摄像机垂直方向的视角开角或者是水平和垂直开角中较小的一个，尚须确认。
class XBasicCam
{
public:
	virtual void useCam() = 0;
	virtual ~XBasicCam() {}
};
class XCam :public XBasicCam	//普通摄像机
{
private:
	XVec3 m_position;			//摄像机的位置
	XVec3 m_lookAtPosition;		//摄像机目标点的位置
	XVec3 m_direction;			//摄像机的方向
	//XVec3 m_angle;				//摄像机的角度
public:
	void init(const XVec3& pos, const XVec3& lookAt, const XVec3& direction)
	{
		m_position = pos;
		m_lookAtPosition = lookAt;
		m_direction = direction;
	}
	void setPosition(float x, float y, float z) { m_position.set(x, y, z); }
	void setPosition(const XVec3& p) { m_position = p; }
	void setLookAt(float x, float y, float z) { m_lookAtPosition.set(x, y, z); }
	void setLookAt(const XVec3& l) { m_lookAtPosition = l; }
	void setDirection(float x, float y, float z) { m_direction.set(x, y, z); }
	void setDirection(const XVec3& d) { m_direction = d; }
	void useCam()	//使摄像机生效
	{
		gluLookAt(m_position.x, m_position.y, m_position.z,
			m_lookAtPosition.x, m_lookAtPosition.y, m_lookAtPosition.z,
			m_direction.x, m_direction.y, m_direction.z);
	}
	XCam()
		:m_position(0.0f, 0.0f, -100.0f)
		, m_lookAtPosition(0.0f, 0.0f, 0.0f)
		, m_direction(0.0f, 1.0f, 0.0f)
	{}
};
//用于漫游的摄像机
class XCamRoam :public XBasicCam
{
private:
	XVec3 m_position;			//摄像机的位置
	float m_len;				//摄像机的视距
	//XVec3 m_angle;			//摄像机的角度[0 - 360]
	XMat3 m_rotateMat;			//旋转矩阵

	XVec3 m_lookAtPosition;		//摄像机目标点的位置
	XVec3 m_direction;			//摄像机的方向
	XMat4 m_projectMatrix;	//摄像机的世界矩阵
	XMat4 m_viewMatrix;		//摄像机的观察矩阵
	XMat4 m_projXView;		//m_projectMatrix * m_viewMatrix

	XBool m_needCalculate;			//是否需要计算
	XBool m_neadCalFrustum;		//是否需要计算视锥
	XBool m_neadCalProjXView;		//是否需要计算pXv
	float m_near;					//近距离
	float m_far;					//远距离
	float m_fov;					//开角
	bool m_isSetFov;
public:
	void setNear(float n)
	{
		m_near = n;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getNear()const { return m_near; }
	void setFar(float f)
	{
		m_far = f;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getFar()const { return m_far; }
	void setFov(float f)
	{
		m_fov = f;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
		m_isSetFov = false;
	}
	float getFov()const { return m_fov; }
	void setCamState(const XVec3& pos, const XVec3& lookAt, const XVec3& dir);
public:
	XMat4 getProjXView()
	{
		calculate();
		if (m_neadCalProjXView)
		{
			m_projXView = m_projectMatrix * m_viewMatrix;
			m_neadCalProjXView = XFalse;
		}
		return m_projXView;
	}
	XMat4 getProjectMatrix() { return m_projectMatrix; }
	XMat4 getViewMatrix() { return m_viewMatrix; }
	XBool calculate();
	void init(const XVec3& pos, float len, const XVec3& angle)
	{
		if (!m_isSetFov)
			m_fov = XEG.getViewAngle();
		m_position = pos;
		m_len = len;
		m_rotateMat = XMath::getRotate3D(angle);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
		calculate();
	}
	void forward(float len)
	{//向前移动摄像机
#ifdef WITH_LEFT_MULT
		m_position += XMath::transform3x3(XVec3(0.0f, 0.0f, len), m_rotateMat);
#else
		m_position += m_rotateMat * XVec3(0.0f, 0.0f, len);
#endif
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void backward(float len)
	{//向后移动摄像机
#ifdef WITH_LEFT_MULT
		m_position -= XMath::transform3x3(XVec3(0.0f, 0.0f, len), m_rotateMat);
#else
		m_position -= m_rotateMat * XVec3(0.0f, 0.0f, len);
#endif
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnLeft(float angle)
	{
#ifdef WITH_LEFT_MULT
		m_rotateMat = m_rotateMat * XMath::getRotate3D(XVec3(0.0f, angle, 0.0f));
#else
		m_rotateMat = XMath::getRotate3D(XVec3(0.0f, angle, 0.0f)) * m_rotateMat;
#endif
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnRight(float angle)
	{
#ifdef WITH_LEFT_MULT
		m_rotateMat = m_rotateMat * XMath::getRotate3D(XVec3(0.0f, -angle, 0.0f));
#else
		m_rotateMat = XMath::getRotate3D(XVec3(0.0f, -angle, 0.0f)) * m_rotateMat;
#endif
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnUp(float angle)
	{
#ifdef WITH_LEFT_MULT
		m_rotateMat = XMath::getRotate3D(XVec3(-angle, 0.0f, 0.0f)) * m_rotateMat;
#else
		m_rotateMat = m_rotateMat * XMath::getRotate3D(XVec3(-angle, 0.0f, 0.0f));
#endif
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnDown(float angle)
	{
#ifdef WITH_LEFT_MULT
		m_rotateMat = XMath::getRotate3D(XVec3(angle, 0.0f, 0.0f)) * m_rotateMat;
#else
		m_rotateMat = m_rotateMat * XMath::getRotate3D(XVec3(angle, 0.0f, 0.0f));
#endif
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	XVec3 getPosition() const { return m_position; }
	XVec3 getLookAtPos() const { return m_lookAtPosition; }
	XVec3 getDirection() const { return m_direction; }
	void setPosition(float x, float y, float z)
	{
		m_position.set(x, y, z);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	XVec3 worldToScreen(const XVec3& pos, const XRect& view);
	void setPosition(const XVec3& pos)
	{
		m_position = pos;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getLength() const { return m_len; }
	void setLength(float len)
	{
		m_len = len;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	//[0 360]
	void setAngle(const XVec3& angle)
	{
		m_rotateMat = XMath::getRotate3D(angle);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	//[0 360]
	void setAngle(float x, float y, float z)
	{
		m_rotateMat = XMath::getRotate3D(XVec3(x, y, z));
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	//[0 - PI2]
	XVec3 getAngle() const { return XMath::getEulerian(m_rotateMat); }
	XMat3 getRotateMat()const { return m_rotateMat; }
	void setRotateMat(const XMat3& mat3)
	{
		m_rotateMat = mat3;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void useCam()	//使摄像机生效
	{
		calculate();
		gluLookAt(m_position.x, m_position.y, m_position.z,
			m_lookAtPosition.x, m_lookAtPosition.y, m_lookAtPosition.z,
			m_direction.x, m_direction.y, m_direction.z);
	}
	XCamRoam()
		:m_position(0.0f, 0.0f, -100.0f)
		, m_len(100.0f)
		, m_near(10.0f)
		, m_far(10000.0f)
		, m_fov(45.0f)
		, m_isSetFov(false)
	{}
	//下面是尚未实现的接口
private:
	XVec4 m_face[6];	//视锥体的6个截面
	XVec3 m_point[9];	//视锥体的8个顶点,最后一个点为摄像机所在的位置
public:
	//需要增加的接口（经过初步测试）
	void calculateFrustumPlanes(); //计算视锥体
	bool isInFrustum(float x, float y, float z);  //判断点是否在视锥体内部 
	void calculateFrustumPoint();
	void drawFrustum();	//描绘视锥体
};
}
#endif //_JIA_XCAM_
