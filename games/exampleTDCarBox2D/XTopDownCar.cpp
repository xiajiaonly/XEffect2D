#include "XTopDownCar.h"
using namespace XE;
float gBox2DSizerate = 5.0f;
XVec2 gBox2DOffset(300.0f,300.0f);
bool XTDTire::init(b2World* world)
{
	if(m_isInited) return false;
	if(world == NULL) return false;

	b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(1.2f,2.6f);
    b2Fixture* fixture = m_body->CreateFixture(&polygonShape,0.5);
    fixture->SetUserData(XMem::createMem<CarTireFUD>());

    m_body->SetUserData(this);
    m_currentTraction = 1;

	m_isInited = true;
	return true;
}
void XTDTire::updateTraction()
{
    if(m_groundAreas.empty()) m_currentTraction = 1;
    else
	{

        m_currentTraction = 0;
        std::set<GroundAreaFUD*>::iterator it = m_groundAreas.begin();
        while(it != m_groundAreas.end())
		{
            GroundAreaFUD* ga = *it;
            if(ga->frictionModifier > m_currentTraction)
                m_currentTraction = ga->frictionModifier;
            ++it;
        }
    }
}
void XTDTire::updateFriction()
{
	if(!m_isInited) return;

    b2Vec2 impulse = m_body->GetMass() * -getLateralVelocity();
    if ( impulse.Length() > m_maxLateralImpulse )
        impulse *= m_maxLateralImpulse / impulse.Length();
    m_body->ApplyLinearImpulse( m_currentTraction * impulse, m_body->GetWorldCenter() ,true);

    m_body->ApplyAngularImpulse( m_currentTraction * 0.1f * m_body->GetInertia() * -m_body->GetAngularVelocity()  ,true);

    b2Vec2 currentForwardNormal = getForwardVelocity();
    float currentForwardSpeed = currentForwardNormal.Normalize();
    float dragForceMagnitude = -2 * currentForwardSpeed;
    m_body->ApplyForce( m_currentTraction * dragForceMagnitude * currentForwardNormal, m_body->GetWorldCenter()  ,true);
}

void XTDTire::updateDrive(int controlState)
{
	if(!m_isInited) return;

    float desiredSpeed = 0;
    switch(controlState & (TDC_UP|TDC_DOWN) )
	{
        case TDC_UP:   desiredSpeed = m_maxForwardSpeed;  break;
        case TDC_DOWN: desiredSpeed = m_maxBackwardSpeed; break;
        default: return;
    }

    b2Vec2 currentForwardNormal = m_body->GetWorldVector( b2Vec2(0,1) );
    float currentSpeed = b2Dot( getForwardVelocity(), currentForwardNormal );

    float force = 0;
    if ( desiredSpeed > currentSpeed )
        force = m_maxDriveForce;
    else if ( desiredSpeed < currentSpeed )
        force = -m_maxDriveForce;
    else
        return;
    m_body->ApplyForce( m_currentTraction * force * currentForwardNormal, m_body->GetWorldCenter()  ,true);
}
void XTDCar::update(int controlState)
{
	if(!m_isInited) return;

    for (int i = 0; i < m_tires.size(); i++)
        m_tires[i]->updateFriction();
    for (int i = 0; i < m_tires.size(); i++)
        m_tires[i]->updateDrive(controlState);

    float lockAngle = 35 * DEGREE2RADIAN;
    float turnSpeedPerSec = 160 * DEGREE2RADIAN;
    float turnPerTimeStep = turnSpeedPerSec / 60.0f;
    float desiredAngle = 0;
    switch(controlState & (TDC_LEFT|TDC_RIGHT))
	{
    case TDC_LEFT:  desiredAngle = lockAngle;  break;
    case TDC_RIGHT: desiredAngle = -lockAngle; break;
    default: ;
    }
    float angleNow = m_flJoint->GetJointAngle();
    float angleToTurn = desiredAngle - angleNow;
    angleToTurn = b2Clamp( angleToTurn, -turnPerTimeStep, turnPerTimeStep );
    float newAngle = angleNow + angleToTurn;
    m_flJoint->SetLimits( newAngle, newAngle );
    m_frJoint->SetLimits( newAngle, newAngle );
}
bool XTDCar::init(b2World* world)
{
	if(m_isInited) return false;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);
    m_body->SetAngularDamping(3);

    b2Vec2 vertices[8];
	for(int i = 0;i < 4;++ i)
	{
		vertices[i].Set(m_vertices[i].x,m_vertices[i].y);
	}
    b2PolygonShape polygonShape;
    polygonShape.Set(vertices,4);
    b2Fixture* fixture = m_body->CreateFixture(&polygonShape, 0.05f);

    b2RevoluteJointDef jointDef;
    jointDef.bodyA = m_body;
    jointDef.enableLimit = true;
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorB.SetZero();

    float maxForwardSpeed = 250;
    float maxBackwardSpeed = -40;
    float backTireMaxDriveForce = 300;
    float frontTireMaxDriveForce = 500;
    float backTireMaxLateralImpulse = 8.5;
    float frontTireMaxLateralImpulse = 7.5;

    XTDTire* tire = XMem::createMem<XTDTire>();
	tire->init(world);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
    jointDef.bodyB = tire->m_body;
    jointDef.localAnchorA.Set( -3.5,-6.0f );
    world->CreateJoint( &jointDef );
    m_tires.push_back(tire);

    tire = XMem::createMem<XTDTire>();
	tire->init(world);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
    jointDef.bodyB = tire->m_body;
    jointDef.localAnchorA.Set( 3.5,-6.0f );
    world->CreateJoint( &jointDef );
    m_tires.push_back(tire);

    tire = XMem::createMem<XTDTire>();
	tire->init(world);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
    jointDef.bodyB = tire->m_body;
    jointDef.localAnchorA.Set( -3.5, 6.3f );
    m_flJoint = (b2RevoluteJoint*)world->CreateJoint( &jointDef );
    m_tires.push_back(tire);

    tire = XMem::createMem<XTDTire>();
	tire->init(world);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
    jointDef.bodyB = tire->m_body;
    jointDef.localAnchorA.Set( 3.5, 6.3f );
    m_frJoint = (b2RevoluteJoint*)world->CreateJoint( &jointDef );
    m_tires.push_back(tire);

	m_isInited = true;
	return true;
}
void XTDCar::draw()
{
	if(!m_isInited) return;
    for(int i = 0;i < m_tires.size();++ i)
	{
		m_tires[i]->draw();
	}
    XVec2 vertices[8];
    for(int i = 0;i < 4;++ i)
	{
		vertices[i] = m_vertices[i] * gBox2DSizerate;
	}
	XRender::drawFillPolygon(vertices,4,XVec2(gBox2DOffset.x + m_body->GetPosition().x * gBox2DSizerate,
		gBox2DOffset.y + m_body->GetPosition().y * gBox2DSizerate),
		m_body->GetAngle() * RADIAN2DEGREE,1,1,1);
}
void XTDCar::draw3D()
{
	if(!m_isInited) return;
    for(int i = 0;i < m_tires.size();++ i)
	{
		m_tires[i]->draw3D();
	}
	XVec2 pos(m_body->GetPosition().x,m_body->GetPosition().y);
	XVec2 size(3.5f,10.0f);
	float angle = -m_body->GetAngle();
	XVec2 P1(pos.x - (size.x * cos(angle) - size.y * sin(angle)),
		pos.y + (size.x * sin(angle) + size.y * cos(angle)));
	XVec2 P2(pos.x - (size.x * cos(angle) + size.y * sin(angle)),
		pos.y + (size.x * sin(angle) - size.y * cos(angle)));

	XVec2 P3(pos.x + (size.x * cos(angle) - size.y * sin(angle)),
		pos.y - (size.x * sin(angle) + size.y * cos(angle)));
	XVec2 P4(pos.x + (size.x * cos(angle) + size.y * sin(angle)),
		pos.y - (size.x * sin(angle) - size.y * cos(angle)));

	XRender::drawLine(XVec3(P1.x,0.0f,P1.y),XVec3(P2.x,0.0f,P2.y));
	XRender::drawLine(XVec3(P2.x,0.0f,P2.y),XVec3(P3.x,0.0f,P3.y));
	XRender::drawLine(XVec3(P3.x,0.0f,P3.y),XVec3(P4.x,0.0f,P4.y));
	XRender::drawLine(XVec3(P4.x,0.0f,P4.y),XVec3(P1.x,0.0f,P1.y));
}