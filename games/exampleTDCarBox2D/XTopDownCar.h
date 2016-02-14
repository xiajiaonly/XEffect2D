#ifndef _JIA_XTOPDOWNCAR_
#define _JIA_XTOPDOWNCAR_

#include "XEffeEngine.h"
#include "Box2D.h"
#include <vector>
#include <set>
#include "X3D/XBasic3D.h"
extern float gBox2DSizerate;
extern XE::XVector2 gBox2DOffset;
enum
{
    TDC_LEFT     = 0x1,
    TDC_RIGHT    = 0x2,
    TDC_UP       = 0x4,
    TDC_DOWN     = 0x8
};
enum fixtureUserDataType
{
    FUD_CAR_TIRE,
    FUD_GROUND_AREA
};
class FixtureUserData
{
private:
    fixtureUserDataType m_type;
protected:
    FixtureUserData(fixtureUserDataType type)
		:m_type(type)
	{}
public:
    virtual fixtureUserDataType getType() {return m_type;}
    virtual ~FixtureUserData() {}
};
class CarTireFUD : public FixtureUserData
{
public:
    CarTireFUD()
		:FixtureUserData(FUD_CAR_TIRE)
	{}
};
class GroundAreaFUD : public FixtureUserData
{
public:
    float frictionModifier;
    bool outOfCourse;

    GroundAreaFUD(float fm,bool ooc)
		:FixtureUserData(FUD_GROUND_AREA)
		,frictionModifier(fm)
		,outOfCourse(ooc)
	{}
};

class XTDCar;
class XTDTire
{
	friend XTDCar;
private:
	b2Body* m_body;
    float m_maxForwardSpeed;
    float m_maxBackwardSpeed;
    float m_maxDriveForce;
    float m_maxLateralImpulse;
    std::set<GroundAreaFUD*> m_groundAreas;
    float m_currentTraction;

	bool m_isInited;
public:
	XTDTire()
		:m_isInited(false)
		,m_body(NULL)
		,m_maxForwardSpeed(250)
		,m_maxBackwardSpeed(-40)
		,m_maxDriveForce(300)
		,m_maxLateralImpulse(8.5)
		,m_currentTraction(1)
	{}

	bool init(b2World* world);
	~XTDTire()
	{
		if(!m_isInited) return;
		m_body->GetWorld()->DestroyBody(m_body);
		m_groundAreas.clear();
		m_isInited = false;
	}

    void setCharacteristics(float maxForwardSpeed,
		float maxBackwardSpeed,
		float maxDriveForce,
		float maxLateralImpulse)
	{
        m_maxForwardSpeed = maxForwardSpeed;
        m_maxBackwardSpeed = maxBackwardSpeed;
        m_maxDriveForce = maxDriveForce;
        m_maxLateralImpulse = maxLateralImpulse;
    }

    void addGroundArea(GroundAreaFUD* ga)
	{
		if(ga == NULL) return;
		m_groundAreas.insert(ga);
		updateTraction();
	}

    void removeGroundArea(GroundAreaFUD* ga)
	{
		if(ga == NULL) return;
		m_groundAreas.erase(ga);
		updateTraction();
	}

    void updateTraction();
    b2Vec2 getLateralVelocity()
	{
		if(!m_isInited) return b2Vec2(0.0f,0.0f);
        b2Vec2 currentRightNormal = m_body->GetWorldVector(b2Vec2(1,0));
        return b2Dot(currentRightNormal,m_body->GetLinearVelocity()) * currentRightNormal;
    }
    b2Vec2 getForwardVelocity()
	{
		if(!m_isInited) return b2Vec2(0.0f,0.0f);
        b2Vec2 currentForwardNormal = m_body->GetWorldVector( b2Vec2(0,1) );
        return b2Dot( currentForwardNormal, m_body->GetLinearVelocity() ) * currentForwardNormal;
    }

    void updateFriction();

    void updateDrive(int controlState);

    void updateTurn(int controlState)
	{
		if(!m_isInited) return;
        float desiredTorque = 0;
        switch(controlState & (TDC_LEFT|TDC_RIGHT))
		{
            case TDC_LEFT:  desiredTorque = 15;  break;
            case TDC_RIGHT: desiredTorque = -15; break;
            default: ;
        }
        m_body->ApplyTorque( desiredTorque,true );
    }
	void draw()
	{
		XE::XRender::drawBox(gBox2DOffset.x + m_body->GetPosition().x * gBox2DSizerate,
			gBox2DOffset.y + m_body->GetPosition().y * gBox2DSizerate,0.5f * gBox2DSizerate,1.25f * gBox2DSizerate,-m_body->GetAngle());
	}
	void draw3D()
	{
		XE::XVector2 pos(m_body->GetPosition().x,m_body->GetPosition().y);
		XE::XVector2 size(0.6f,1.3f);
		float angle = -m_body->GetAngle();
		XE::XVector2 P1(pos.x - (size.x * cos(angle) - size.y * sin(angle)),
			pos.y + (size.x * sin(angle) + size.y * cos(angle)));
		XE::XVector2 P2(pos.x - (size.x * cos(angle) + size.y * sin(angle)),
			pos.y + (size.x * sin(angle) - size.y * cos(angle)));

		XE::XVector2 P3(pos.x + (size.x * cos(angle) - size.y * sin(angle)),
			pos.y - (size.x * sin(angle) + size.y * cos(angle)));
		XE::XVector2 P4(pos.x + (size.x * cos(angle) + size.y * sin(angle)),
			pos.y - (size.x * sin(angle) - size.y * cos(angle)));

		XE::XRender::drawLine(XE::XVector3(P1.x,0.0f,P1.y),XE::XVector3(P2.x,0.0f,P2.y));
		XE::XRender::drawLine(XE::XVector3(P2.x,0.0f,P2.y),XE::XVector3(P3.x,0.0f,P3.y));
		XE::XRender::drawLine(XE::XVector3(P3.x,0.0f,P3.y),XE::XVector3(P4.x,0.0f,P4.y));
		XE::XRender::drawLine(XE::XVector3(P4.x,0.0f,P4.y),XE::XVector3(P1.x,0.0f,P1.y));
	}
};

class XTDCar
{
private:
    b2Body* m_body;
    std::vector<XTDTire*> m_tires;
    b2RevoluteJoint *m_flJoint;
	b2RevoluteJoint *m_frJoint;
	bool m_isInited;
	XE::XVector2 m_vertices[4];
public:
	XTDCar()
		:m_isInited(false)
		,m_body(NULL)
		,m_flJoint(NULL)
		,m_frJoint(NULL)
	{
		m_vertices[0].set( 3.5,-10.0);
        m_vertices[1].set( 3.5, 10.0);
        m_vertices[2].set(-3.5, 10.0);
        m_vertices[3].set(-3.5,-10.0);
	}
	XE::XVector3 getPosition()const
	{
		if(!m_isInited) return XE::XVector3::zero;
		return XE::XVector3(m_body->GetPosition().x,
			0.0f,
			m_body->GetPosition().y);
	}
	XE::XVector3 getPosition(int index)const
	{
		if(!m_isInited) return XE::XVector3::zero;
		if(index < 0 || index >= 4) return XE::XVector3::zero;
		return XE::XVector3(m_tires[index]->m_body->GetPosition().x,
			0.0f,
			m_tires[index]->m_body->GetPosition().y);
	}
	XE::XVector3 getAngle()const
	{
		if(!m_isInited) return XE::XVector3::zero;
		return XE::XVector3(0.0f,
			-m_body->GetAngle() * RADIAN2DEGREE,
			0.0f);
	}
	XE::XVector3 getAngle(int index)const
	{
		if(!m_isInited) return XE::XVector3::zero;
		if(index < 0 || index >= 4) return XE::XVector3::zero;
		return XE::XVector3(0.0f,
			(m_body->GetAngle() - m_tires[index]->m_body->GetAngle()) * RADIAN2DEGREE,
			0.0f);
	}
	float getTireSpeed(int index) const
	{
		if(!m_isInited) return 0.0f;
		if(index < 0 || index >= 4) return 0.0f;

		float angle = m_tires[index]->m_body->GetAngle();

		b2Vec2 temp = m_tires[index]->m_body->GetLinearVelocity();

		XE::XVector2 tmpV0(temp.x,temp.y);
		XE::XVector2 tmpV1(sin(-angle),cos(-angle));
		if(tmpV0.dot(tmpV1) <= 0) return -temp.Length();
		else return temp.Length();
	}

	bool init(b2World* world);
    ~XTDCar()
	{
		if(!m_isInited) return;
        for(int i = 0;i < m_tires.size();++ i)
            XE::XMem::XDELETE(m_tires[i]);
		m_tires.clear();
		m_body->GetWorld()->DestroyBody(m_body);
		m_isInited = false;
    }

    void update(int controlState);
	void draw();
	void draw3D();
};

class XMyDestructionListener:public b2DestructionListener
{
    void SayGoodbye(b2Fixture* fixture)
    {
        if(FixtureUserData* fud = (FixtureUserData*)fixture->GetUserData() )
			XE::XMem::XDELETE(fud);
    }

    void SayGoodbye(b2Joint* joint) {}
};
class XMyContactListener:public b2ContactListener
{
	void handleContact(b2Contact* contact, bool began)
	{
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();
		FixtureUserData* fudA = (FixtureUserData*)a->GetUserData();
		FixtureUserData* fudB = (FixtureUserData*)b->GetUserData();

		if(!fudA || !fudB) return;

		if(fudA->getType() == FUD_CAR_TIRE || fudB->getType() == FUD_GROUND_AREA)
			tireVsGroundArea(a,b,began);
		else
		if(fudA->getType() == FUD_GROUND_AREA || fudB->getType() == FUD_CAR_TIRE)
			tireVsGroundArea(b, a, began);
	}

	void BeginContact(b2Contact* contact)
	{
		handleContact(contact,true);

	}
	void EndContact(b2Contact* contact)
	{
		handleContact(contact, false);

	}

	void tireVsGroundArea(b2Fixture* tireFixture, b2Fixture* groundAreaFixture, bool began)
	{
		XTDTire* tire = (XTDTire*)tireFixture->GetBody()->GetUserData();
		GroundAreaFUD* gaFud = (GroundAreaFUD*)groundAreaFixture->GetUserData();
		if(began) tire->addGroundArea(gaFud);
		else tire->removeGroundArea(gaFud);
	}
};
#endif