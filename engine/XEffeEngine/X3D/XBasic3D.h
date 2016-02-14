#ifndef _JIA_XBASIC3D_
#define _JIA_XBASIC3D_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
//这里是一些基本的3D操作的原操作
//#include "XBasicOpenGL.h"
//#include "XBasicClass.h"
//#include "XCam.h"
#include "../XMath/XVector3.h"
#include "../XMath/XMatrix4x4.h"
namespace XE{
enum X3DDrawMode
{
	DRAW_MODE_FILL,
	DRAW_MODE_LINE,
	DRAW_MODE_POINT,
};
namespace XRender
{
	extern void drawLine(const XVector3 &ps,const XVector3 &pe,const XFColor & color = XFColor::white,float w = 0.5f);							//画一条指定线段
	extern void drawLine(const XVector3 &ps,const XVector3 &n,float len,const XFColor & color = XFColor::white,float w = 0.5f);				//画一条指定线段
	extern void drawTriangle(const XVector3 &p0,const XVector3 &p1,const XVector3 &p2,const XFColor & color = XFColor::white);	//描绘一个指定的三角形
	extern void drawTriangle(const XVector3 &p0,const XVector3 &p1,const XVector3 &p2,const XVector3 &n,const XFColor & color = XFColor::white);	//描绘一个指定的三角形
	extern void drawBox(const XVector3 &center,const XVector3 &size,const XVector3 &angle,const XFColor & color = XFColor::white);	//描绘一个盒子
	extern void drawBox(const XVector3 &center,const XVector3 &size,const XVector3 &angle,unsigned int tex);	//描绘一个带贴图的盒子
	//注意：下面这个函数是使用glut来实现的，一般情况下，下面这个函数是不开放的，因为开放的话会造成需要引入glut的dll，这个不符合设计宗旨
	//extern void drawBall(const XVector3 &center,float r,const XVector3 &angle,const XFColor & color);
	extern void drawArray(const float *p,int w,int h);
	extern void drawArray(const float *p,int w,int h,unsigned int tex);
	extern void drawBox(GLfloat size);
	extern void drawPlane(const XVector3 &p0,const XVector3 &p1,const XVector3 &p2,const XVector3 &p3,
						  const XVector3 &n,const XFColor & color = XFColor::white);
	extern void drawCuboid(const XVector3 &pos,	//位置
		const XVector3 &size,	//长方体的尺寸，长宽高
		const XVector3 &angle,//长方体的角度
		const XFColor &color);	//描绘长方体
	extern void drawOrigin();	//描绘原点
	extern void drawFloor(float distance = 2,float w = 0.5f);	//描绘0平面(地面)
}
namespace XMath
{
	//gluLookAt
	extern XMatrix4x4 calLookAtMatrix(const XVector3 &eyePos,const XVector3 &LookAtPos,const XVector3 &up);
	//gluPerspective
	extern XMatrix4x4 calPerspectiveMatrix(float fovy,float rat,float zNear,float zFar);
	//glOrtho
	extern XMatrix4x4 calOrthoMatrix(float left,float right,float bottom,float top,float zNear,float zFar);
	//glOrtho2D
	extern XMatrix4x4 calOrtho2DMatrix(float left,float right,float bottom,float top);
	//glFrustum
	extern XMatrix4x4 calFrustumMatrix(float left,float right,float bottom,float top,float zNear,float zFar);

	extern bool getOrtho(const XMatrix4x4& mtrx,float& left,float& right,float& bottom,float& top,float& zNear,float& zFar);
	extern bool getFrustum(const XMatrix4x4& mtrx,float& left,float& right,float& bottom,float& top,float& zNear,float& zFar);
	extern bool getPerspective(const XMatrix4x4& mtrx,float& fovy,float& aspectRatio,float& zNear, float& zFar);
	extern void getLookAt(const XMatrix4x4& mtrx,XVector3& eye,XVector3& center,XVector3& up,float lookDistance = 1.0f);
	//沿v向量旋转angle的旋转矩阵
	extern XMatrix4x4 getRotate(float angle,XVector3 v);	// 沿某个轴旋转
	//3D空间中矩阵的生成顺序为glTranslatef / glRotatef / glScalef
	extern XMatrix4x4 getRotate(const XVector3 &a);
	extern XMatrix4x4 getRotateX(float angle);	
	extern XMatrix4x4 getRotateY(float angle);	
	extern XMatrix4x4 getRotateZ(float angle);	
	extern XMatrix4x4 getTranslate(const XVector3 &p);
	extern XMatrix4x4 getScale(const XVector3 &s);
	//根据方向向量，计算旋转角度，基准是(1,0,0);也就是将v(1,0,0)转到d的方向上需要的角度
	inline XVector3 getAngleFromDirection(const XVector3 & d)
	{
		float z = atan2(d.y,sqrt(d.x * d.x + d.z * d.z));
		float y = -atan2(d.z,d.x);
		return XVector3(0.0f,y,z);
	}
	//near:近截点，far:远截点，v:三角面的顶点数据，n:三角面的法线数据，out:交点,返回交点在远近截点中的归一化距离，< 0 为没有交点
	extern float canPickingByRay(const XVector3 &nearP,const XVector3 &farP,XVector3 *v,const XVector3 &n,XVector3 &out);
	//gluUnProject()
	extern int getUnProject(float x,float y,float z,const XMatrix4x4& modelview,const XMatrix4x4 &projection,
					  const XVector4 &viewport,XVector3 &out);
	//传输屏幕坐标，直接返回远近两个截面
	extern int getUnProject(const XVector2 &pos,XVector3 &nearP,XVector3 &farP);
}
class XBasic3DObject
{
protected:
	XVector3 m_position;
	XVector3 m_scale;
	XVector3 m_angle;
	XFColor m_color;
	bool m_needUpdateMatrix;
	XMatrix4x4 m_matrix;
	XMatrix4x4 m_multMatrix;

	XVector3 m_areaPoint[8];	//碰撞盒子的8个点，用于裁剪计算
public:
	virtual XVector3 getAreaPoint(int index) const
	{
		if(index < 0 || index >= 8) return XVector3::zero;
		return m_areaPoint[index];
	}
	virtual XMatrix4x4 getMatrix()const{return m_matrix;}
	virtual void setPosition(const XVector3 &pos) 
	{
		m_position = pos;
		m_needUpdateMatrix = true;
	}
	virtual void setPosition(float x,float y,float z)
	{
		m_position.set(x,y,z);
		m_needUpdateMatrix = true;
	}
	virtual XVector3 getPosition()const{return m_position;}
	virtual void setScale(const XVector3 &scale) 
	{
		m_scale = scale;
		m_needUpdateMatrix = true;
	}
	virtual void setSize(float x,float y,float z)
	{
		m_scale.set(x,y,z);
		m_needUpdateMatrix = true;
	}
	virtual XVector3 getScale()const{return m_scale;}
	virtual void setAngle(const XVector3 &angle) 
	{
		m_angle = angle;
		m_needUpdateMatrix = true;
	}
	virtual void setAngle(float x,float y,float z)
	{
		m_angle.set(x,y,z);
		m_needUpdateMatrix = true;
	}
	virtual void setMultRotate(const XMatrix4x4 & mtrx) 
	{
		m_multMatrix = mtrx;
		m_needUpdateMatrix = true;
	}
	virtual XVector3 getAngle()const{return m_angle;}
	virtual void setColor(const XFColor &color) {m_color = color;}
	virtual void setColor(float r,float g,float b,float a){m_color.setColor(r,g,b,a);}
	virtual XFColor getColor()const{return m_color;}
	virtual void updateMatrix()
	{
		if(!m_needUpdateMatrix) return;
		m_matrix = XMath::getTranslate(m_position);
		m_matrix = m_matrix * XMath::getRotate(m_angle);
		m_matrix = m_matrix * m_multMatrix;
		m_matrix = m_matrix * XMath::getScale(m_scale);
		m_needUpdateMatrix = false;
	}

	XBasic3DObject()
		:m_position(0.0f,0.0f,0.0f)
		,m_scale(1.0f,1.0f,1.0f)
		,m_angle(0.0f,0.0f,0.0f)
		,m_color(1.0f,1.0f,1.0f,1.0f)
		,m_needUpdateMatrix(false)
	{
		m_matrix.loadIdentity();
		m_multMatrix.loadIdentity();
	}
};
}
#endif