#ifndef _JIA_XCAR_
#define _JIA_XCAR_

#include "X3D/X3DWorld.h"
class XCar3D
{
private:
	XE::XModelObj m_modelCarBody;
	XE::XModelObj m_modelTire[4];
	XE::XVector3 m_angle;
	XE::XVector3 m_position;
	bool m_isInited;
	float m_tireSpeed[4];
	XE::XVector3 m_tireAngle[4];
public:
	XCar3D()
		:m_isInited(false)
		,m_angle(0.0f,0.0f,0.0)
		,m_position(0.0f,0.0f,0.0f)
	{}
	bool init()
	{
		if(m_isInited) return false;
		if(!m_modelCarBody.load("ResourcePack/car/body.obj")) return false;
		if(!m_modelTire[0].load("ResourcePack/car/tire.obj")) return false;
		if(!m_modelTire[1].load("ResourcePack/car/tire.obj")) return false;
		if(!m_modelTire[2].load("ResourcePack/car/tire.obj")) return false;
		if(!m_modelTire[3].load("ResourcePack/car/tire.obj")) return false;

		m_modelTire[3].setPosition(3.5f,0.0f,6.3f);
		m_modelTire[2].setPosition(-3.5f,0.0f,6.3f);
		m_modelTire[2].setAngle(0.0f,180.0f,0.0f);

		m_modelTire[1].setPosition(3.5f,0.0f,-6.0f);

		m_modelTire[0].setPosition(-3.5f,0.0f,-6.0f);
		m_modelTire[0].setAngle(0.0f,180.0f,0.0f);

		m_isInited = true;
		return true;
	}
	void draw()
	{
		if(!m_isInited) return;
		m_modelCarBody.draw();
		m_modelTire[0].draw(true,&m_modelCarBody);
		m_modelTire[1].draw(true,&m_modelCarBody);
		m_modelTire[2].draw(true,&m_modelCarBody);
		m_modelTire[3].draw(true,&m_modelCarBody);
	}
	void move(int stepTime)
	{
		if(!m_isInited) return;
		XE::XMatrix3x3 tmp;
		XE::XMatrix4x4 tmp4x4;
		for(int i = 0;i < 4;++ i)
		{
			m_tireAngle[i].x += m_tireSpeed[i] * stepTime;
			if(m_tireAngle[i].x >= 360.0f) m_tireAngle[i].x -= 360.0f;
			m_modelTire[i].setAngle(0.0f,m_tireAngle[i].y,0.0f);
			m_modelTire[i].setMultRotate(XE::XMath::getRotate(XE::XVector3(m_tireAngle[i].x,0.0f,0.0f)));
		}
	}
	void setTireAngle(int index,float y)
	{
		if(index < 0 || index >= 4) return;
		if(index != 0 && index != 2) m_tireAngle[index].y = y;
		else m_tireAngle[index].y = 180 + y;
	}
	void setTireSpeed(int index,float s)
	{
		if(index < 0 || index >= 4) return;
		if(index != 0 && index != 2) m_tireSpeed[index] = s;
		else m_tireSpeed[index] = -s;
	}
	void setAngle(const XE::XVector3& angle)
	{
		setAngle(angle.x,angle.y,angle.z);
	}
	void setAngle(float x,float y,float z)
	{
		m_angle.set(x,y,z);
		m_modelCarBody.setAngle(m_angle);

	}
	void setMultMatrix(const XE::XMatrix4x4 &tmp)
	{
		m_modelCarBody.setMultRotate(tmp);
	}
	void setPosition(const XE::XVector3& pos)
	{
		setPosition(pos.x,pos.y,pos.z);
	}
	void setPosition(float x,float y,float z)
	{
		m_position.set(x,y,z);
		m_modelCarBody.setPosition(m_position);

	}
	XE::XVector3 getPosition() const{return m_position;}
};
#endif