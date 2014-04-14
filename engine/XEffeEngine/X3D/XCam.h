#ifndef _JIA_XCAM_
#define _JIA_XCAM_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
#include "XBasicFun.h"
#include "XBasicClass.h"
#include "glu.h"

class _XBasicCam
{
public:
	virtual void useCam() = 0;
	virtual ~_XBasicCam() {}
};

class _XCam:public _XBasicCam	//普通摄像机
{
private:
	_XVector3 m_position;			//摄像机的位置
	_XVector3 m_lookAtPosition;		//摄像机目标点的位置
	_XVector3 m_direction;			//摄像机的方向
	//_XVector3 m_angle;				//摄像机的角度
public:
	void init(const _XVector3 &pos,const _XVector3 &lookAt,const _XVector3 &direction)
	{
		m_position = pos;
		m_lookAtPosition = lookAt;
		m_direction = direction;
	}
	void setPosition(float x,float y,float z) {m_position.set(x,y,z);}
	void setPosition(const _XVector3 &p) {m_position = p;}
	void setLookAt(float x,float y,float z) {m_lookAtPosition.set(x,y,z);}
	void setLookAt(const _XVector3 &l) {m_lookAtPosition = l;}
	void setDirection(float x,float y,float z) {m_direction.set(x,y,z);}
	void setDirection(const _XVector3 &d) {m_direction = d;}
	void useCam()	//使摄像机生效
	{
		gluLookAt(m_position.x,m_position.y,m_position.z,
			m_lookAtPosition.x,m_lookAtPosition.y,m_lookAtPosition.z,
			m_direction.x,m_direction.y,m_direction.z);
	}
	_XCam()
		:m_position(0.0f,0.0f,-100.0f)
		,m_lookAtPosition(0.0f,0.0f,0.0f)
		,m_direction(0.0f,1.0f,0.0f)
	{
	}
};
//用于漫游的摄像机
class _XCamRoam:public _XBasicCam
{
private:
	_XVector3 m_position;			//摄像机的位置
	float m_len;					//摄像机的视距
	_XVector3 m_angle;				//摄像机的角度[0 - 360]

	_XVector3 m_lookAtPosition;		//摄像机目标点的位置
	_XVector3 m_direction;			//摄像机的方向
	_XMatrix4x4 m_projectMatrix;	//摄像机的世界矩阵
	_XMatrix4x4 m_viewMatrix;		//摄像机的观察矩阵
	_XMatrix4x4 m_projXView;		//m_projectMatrix * m_viewMatrix

	_XBool m_needCalculate;			//是否需要计算
	_XBool m_neadCalFrustum;		//是否需要计算视锥
	_XBool m_neadCalProjXView;		//是否需要计算pXv
	float m_near;					//近距离
	float m_far;					//远距离
	float m_fov;					//开角
public:
	void setNear(float n)
	{
		m_near = n;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getNear()const {return m_near;}
	void setFar(float f)
	{
		m_far = f;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getFar()const {return m_far;}
	void setFov(float f)
	{
		m_fov = f;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getFov()const {return m_fov;}
public:
	_XMatrix4x4 getProjXView()
	{
		calculate();
		if(m_neadCalProjXView)
		{
			m_projXView = m_projectMatrix * m_viewMatrix;
			m_neadCalProjXView = XFalse;
		}
		return m_projXView;
	}
	_XMatrix4x4 getProjectMatrix() {return m_projectMatrix;}
	_XMatrix4x4 getViewMatrix() {return m_viewMatrix;}
	_XBool calculate();
	void init(const _XVector3 &pos,float len,_XVector3 &angle)
	{
		m_position = pos;
		m_len = len;
		m_angle = angle;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
		calculate();
	}
	void forward(float len)
	{//向前移动摄像机
		m_position.z += len * cos(m_angle.x * ANGLE_TO_RADIAN) * cos(m_angle.y * ANGLE_TO_RADIAN);
		m_position.y += len * sin(m_angle.x * ANGLE_TO_RADIAN);
		m_position.x += len * cos(m_angle.x * ANGLE_TO_RADIAN) * sin(m_angle.y * ANGLE_TO_RADIAN);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void backward(float len)
	{//向后移动摄像机
		m_position.z -= len * cos(m_angle.x * ANGLE_TO_RADIAN) * cos(m_angle.y * ANGLE_TO_RADIAN);
		m_position.y -= len * sin(m_angle.x * ANGLE_TO_RADIAN);
		m_position.x -= len * cos(m_angle.x * ANGLE_TO_RADIAN) * sin(m_angle.y * ANGLE_TO_RADIAN);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnLeft(float angle)
	{
		m_angle.y += angle;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnRight(float angle)
	{
		m_angle.y -= angle;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnUp(float angle)
	{
		m_angle.x += angle;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
		if(m_angle.x > 90.0f) m_angle.x = 90.0f;
	}
	void turnDown(float angle)
	{
		m_angle.x -= angle;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
		if(m_angle.x < -90.0f) m_angle.x = -90.0f;
	}
	_XVector3 getPosition() const {return m_position;}
	_XVector3 getLookAtPos() const {return m_lookAtPosition;}
	void setPosition(float x,float y,float z)
	{
		m_position.set(x,y,z);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void setPosition(const _XVector3 &pos)
	{
		m_position = pos;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getLength() const {return m_len;}
	void setLength(float len)
	{
		m_len = len;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void setAngle(float x,float y,float z)
	{
		m_angle.set(x,y,z);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	_XVector3 getAngle() const {return m_angle;}
	void useCam()	//使摄像机生效
	{
		calculate();
		gluLookAt(m_position.x,m_position.y,m_position.z,
			m_lookAtPosition.x,m_lookAtPosition.y,m_lookAtPosition.z,
			m_direction.x,m_direction.y,m_direction.z);
	}
	_XCamRoam()
		:m_position(0.0f,0.0f,-100.0f)
		,m_len(100.0f)
		,m_angle(0.0f,0.0f,0.0f)
		,m_near(10.0f)		
		,m_far(10000.0f)				
		,m_fov(45.0f)	
	{
	}
//下面是尚未实现的接口
private:
	_XVector4 m_face[6];	//视锥体的6个截面
	_XVector3 m_point[9];	//视锥体的8个顶点,最后一个点为摄像机所在的位置
public:
	//需要增加的接口（经过初步测试）
	void calculateFrustumPlanes(); //计算视锥体
	bool isInFrustum(float x,float y,float z);  //判断点是否在视锥体内部 
	void calculateFrustumPoint();
	void drawFrustum();	//描绘视锥体
};
#endif //_JIA_XCAM_