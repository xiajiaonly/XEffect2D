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
//#include "../XMath/XFColor.h"
namespace XE {
enum X3DDrawMode
{
	DRAW_MODE_FILL,
	DRAW_MODE_LINE,
	DRAW_MODE_POINT,
};
//#define WITH_GLUT
namespace XRender
{
	extern void drawLine(const XVec3& ps, const XVec3& pe, const XFColor& color = XFColor::white, float w = 0.5f);							//画一条指定线段
	extern void drawLine(const XVec3& ps, const XVec3& n, float len, const XFColor& color = XFColor::white, float w = 0.5f);				//画一条指定线段
	extern void drawTriangle(const XVec3& p0, const XVec3& p1, const XVec3& p2, const XFColor& color = XFColor::white);	//描绘一个指定的三角形
	extern void drawTriangle(const XVec3& p0, const XVec3& p1, const XVec3& p2, const XVec3& n, const XFColor& color = XFColor::white);	//描绘一个指定的三角形
	//size:半尺寸
	extern void drawCuboid(const XVec3& center, const XVec3& size, const XVec3& angle, const XFColor& color = XFColor::white);	//描绘一个盒子
	//中心位置半尺寸
	extern void drawCuboid(const XVec3& center, const XVec3& size, const XVec3& angle, unsigned int tex);	//描绘一个带贴图的盒子
#ifdef WITH_GLUT
//注意：下面这个函数是使用glut来实现的，一般情况下，下面这个函数是不开放的，因为开放的话会造成需要引入glut的dll，这个不符合设计宗旨
	extern void drawBall(const XVec3& center, float r, const XVec3& angle, const XFColor& color);
#endif
	//p：为w*h的点阵，
	//函数描绘p构成的平面
	extern void drawArray(const float *p, int w, int h);
	extern void drawArray(const float *p, int w, int h, unsigned int tex);
	//位置在原点
	//描绘单位立方体，长宽高为1
	extern void drawCuboidBase();
	//带贴图参数，长宽高为1
	extern void drawCuboidWithTexBase();
	//不使用矩阵计算的长方体
	//size:半尺寸
	extern void drawCuboidBase(const XVec3& size);
	inline void drawCuboidBase(GLfloat size) { drawCuboidBase(XVec3(size)); }
	//位置不在原点
	//pos中心位置，size半尺寸，不使用矩阵运算
	extern void drawCuboidBase(const XVec3& center, const XVec3& size);
	extern void drawCuboid(const XVec3& center, const XVec3& size);
	//x左上角，yz中心，全尺寸
	extern void drawCuboidEx(const XVec3& pos,	//位置
		const XVec3& size,		//长方体的尺寸，长宽高
		const XVec3& angle,		//长方体的角度
		const XFColor& color);	//描绘长方体
	extern void drawPlane(const XVec3& p0, const XVec3& p1, const XVec3& p2, const XVec3& p3,
		const XVec3& n, const XFColor& color = XFColor::white);
	extern void drawOrigin();	//描绘原点
	extern void drawFloor(float distance = 2, float w = 0.5f);	//描绘0平面(地面)
}
namespace XMath
{
	//gluLookAt
	extern XMat4 calLookAtMatrix(const XVec3& eyePos, const XVec3& LookAtPos, const XVec3& up);
	//gluPerspective
	extern XMat4 calPerspectiveMatrix(float fovy, float rat, float zNear, float zFar);
	//glOrtho	//没有经过测试
	inline XMat4 calOrthoMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		return XMat4(2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
			0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
			0.0f, 0.0f, -2.0 / (zFar - zNear), -(zFar + zNear) / (zFar - zNear),
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	//glOrtho2D //没有经过测试
	inline XMat4 calOrtho2DMatrix(float left, float right, float bottom, float top) { return calOrthoMatrix(left, right, bottom, top, -1.0, 1.0); }
	//glFrustum
	inline XMat4 calFrustumMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		//这个从微软过来的算法好像错了
		//return XMat4(	2.0*zNear/(right-left),	0.0f,					(right+left)/(right-left),		0.0f,
		//				0.0f,					2.0*zNear/(top-bottom), (top+bottom)/(top-bottom),		0.0f,
		//				0.0f,					0.0f,					-(zFar+zNear)/(zFar-zNear),		-2.0*zFar*zNear/(zFar-zNear),
		//				0.0f,					0.0f,					-1.0f,							0.0f);
		//下面是时间正确的算法
		return XMat4(2.0*zNear / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
			0.0f, 2.0*zNear / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
			0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -1.0f,
			0.0f, 0.0f, -2.0*zFar*zNear / (zFar - zNear), 0.0f);
	}

	extern bool getOrtho(const XMat4& mtrx, float& left, float& right, float& bottom, float& top, float& zNear, float& zFar);
	extern bool getFrustum(const XMat4& mtrx, float& left, float& right, float& bottom, float& top, float& zNear, float& zFar);
	extern bool getPerspective(const XMat4& mtrx, float& fovy, float& aspectRatio, float& zNear, float& zFar);
	extern void getLookAt(const XMat4& mtrx, XVec3& eye, XVec3& center, XVec3& up, float lookDistance = 1.0f);
	//沿v向量旋转angle的旋转矩阵
	extern XMat4 getRotate(float angle, XVec3 v);	// 沿某个轴旋转
	//3D空间中矩阵的生成顺序为glTranslatef / glRotatef / glScalef
	inline XMat4 getRotateX(float angle)
	{
		float sinA = sin(angle * DEGREE2RADIAN);
		float cosA = cos(angle * DEGREE2RADIAN);
		return XMat4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cosA, sinA, 0.0f,
			0.0f, -sinA, cosA, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	inline XMat4 getRotateY(float angle)
	{
		float sinA = sin(angle * DEGREE2RADIAN);
		float cosA = cos(angle * DEGREE2RADIAN);
		return XMat4(cosA, 0.0f, -sinA, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sinA, 0.0f, cosA, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	inline XMat4 getRotateZ(float angle)
	{
		float sinA = sin(angle * DEGREE2RADIAN);
		float cosA = cos(angle * DEGREE2RADIAN);
		return XMat4(cosA, sinA, 0.0f, 0.0f,
			-sinA, cosA, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	inline XMat4 getRotate(const XVec3& a)
	{
		XMat4 ret = getRotateX(a.x);
		ret = ret * getRotateY(a.y);
		ret = ret * getRotateZ(a.z);
		return ret;
	}
	inline XMat4 getTranslate(const XVec3& p)
	{
		return XMat4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			p.x, p.y, p.z, 1.0f);
	}
	inline XMat4 getScale(const XVec3& s)
	{
		return XMat4(s.x, 0.0f, 0.0f, 0.0f,
			0.0f, s.y, 0.0f, 0.0f,
			0.0f, 0.0f, s.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	//根据方向向量，计算旋转角度，基准是(1,0,0);也就是将v(1,0,0)转到d的方向上需要的角度
	inline XVec3 getAngleFromDirection(const XVec3&  d)
	{
		float z = atan2(d.y, _hypot(d.x, d.z));
		float y = -atan2(d.z, d.x);
		return XVec3(0.0f, y, z);
	}
	//near:近截点，far:远截点，v:三角面的顶点数据，n:三角面的法线数据，out:交点,返回交点在远近截点中的归一化距离，< 0 为没有交点
	extern float canPickingByRay(const XVec3& nearP, const XVec3& farP, XVec3 *v, const XVec3& n, XVec3& out);
	extern XVec3 worldToScreen(const XVec3& pos, const XMat4& modelview, const XMat4 &projection,
		const XRect& view);
	//gluUnProject()
	extern bool getUnProject(float x, float y, float z, const XMat4& modelview, const XMat4 &projection,
		const XVec4& viewport, XVec3& out);
	//传输屏幕坐标，直接返回远近两个截面
	extern bool getUnProject(const XVec2& pos, XVec3& nearP, XVec3& farP);
}
class XBasic3DObject
{
protected:
	XVec3 m_position;
	XVec3 m_scale;
	XVec3 m_angle;
	XFColor m_color;
	bool m_needUpdateMatrix;
	XMat4 m_matrix;
	XMat4 m_multMatrix;

	XVec3 m_areaPoint[8];	//碰撞盒子的8个点，用于裁剪计算
public:
	virtual XVec3 getAreaPoint(int index) const
	{
		if (index < 0 || index >= 8) return XVec3::zero;
		return m_areaPoint[index];
	}
	virtual XMat4 getMatrix()const { return m_matrix; }
	virtual void setPosition(const XVec3& pos)
	{
		m_position = pos;
		m_needUpdateMatrix = true;
	}
	virtual void setPosition(float x, float y, float z)
	{
		m_position.set(x, y, z);
		m_needUpdateMatrix = true;
	}
	virtual XVec3 getPosition()const { return m_position; }
	virtual void setScale(const XVec3& scale)
	{
		m_scale = scale;
		m_needUpdateMatrix = true;
	}
	virtual void setScale(float x, float y, float z)
	{
		m_scale.set(x, y, z);
		m_needUpdateMatrix = true;
	}
	virtual void setScale(float s)
	{
		m_scale.set(s, s, s);
		m_needUpdateMatrix = true;
	}
	virtual XVec3 getScale()const { return m_scale; }
	virtual void setAngle(const XVec3& angle)
	{
		m_angle = angle;
		m_needUpdateMatrix = true;
	}
	virtual void setAngle(float x, float y, float z)
	{
		m_angle.set(x, y, z);
		m_needUpdateMatrix = true;
	}
	virtual void setMultRotate(const XMat4 & mtrx)
	{
		m_multMatrix = mtrx;
		m_needUpdateMatrix = true;
	}
	virtual XVec3 getAngle()const { return m_angle; }
	virtual void setColor(const XFColor& color) { m_color = color; }
	virtual void setColor(float r, float g, float b, float a) { m_color.set(r, g, b, a); }
	virtual XFColor getColor()const { return m_color; }
	virtual void updateMatrix()
	{
		if (!m_needUpdateMatrix) return;
		m_matrix = XMath::getTranslate(m_position);
		m_matrix = m_matrix * XMath::getRotate(m_angle);
		m_matrix = m_matrix * m_multMatrix;
		m_matrix = m_matrix * XMath::getScale(m_scale);
		m_needUpdateMatrix = false;
	}

	XBasic3DObject()
		:m_position(0.0f, 0.0f, 0.0f)
		, m_scale(1.0f, 1.0f, 1.0f)
		, m_angle(0.0f, 0.0f, 0.0f)
		, m_color(1.0f)
		, m_needUpdateMatrix(false)
	{
		m_matrix.loadIdentity();
		m_multMatrix.loadIdentity();
	}
	virtual ~XBasic3DObject() {}
};
}
#endif