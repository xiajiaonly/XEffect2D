#include "GGame3D.h"
using namespace XE;
void GGame3D::draw3D(XBool withTex)
{

	m_car3D.draw();
	m_pcCar3D.draw();

	m_modelMap.draw();

	drawObject();
}
bool GGame3D::init()
{
	XEE::setVSync(false);
	if(!m_car3D.init()) return false;
	if(!m_pcCar3D.init()) return false;

	m_world.SetGravity(b2Vec2(0,0));
	m_world.SetDestructionListener(&m_destructionListener);
	m_world.SetContactListener(&m_contactListener);

	{
		b2BodyDef bodyDef;
		m_groundBody = m_world.CreateBody( &bodyDef );

		b2PolygonShape polygonShape;
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polygonShape;
		fixtureDef.isSensor = true;

		polygonShape.SetAsBox( 9, 7, b2Vec2(-10,15), 20*DEGREE2RADIAN );
		b2Fixture* groundAreaFixture = m_groundBody->CreateFixture(&fixtureDef);
		groundAreaFixture->SetUserData( new GroundAreaFUD( 0.5f, false ) );

		polygonShape.SetAsBox( 9, 5, b2Vec2(5,20), -40*DEGREE2RADIAN );
		groundAreaFixture = m_groundBody->CreateFixture(&fixtureDef);
		groundAreaFixture->SetUserData( new GroundAreaFUD( 0.2f, false ) );
	}

	m_carPhysics = XMem::createMem<XTDCar>();
	m_carPhysics->init(&m_world);
	m_controlState = 0;

	XCurPixel tmpPixel;
	tmpPixel.load("ResourcePack/RoadD.png");
	m_mapData = XMem::createArrayMem<float>(1024 * 1024);
	unsigned char * p = tmpPixel.getPixel();
	for(int i = 0;i < 1024 * 1024;++ i)
	{
		m_mapData[i] = p[i << 2] * 0.5f;
	}

	if(!m_modelMap.load("ResourcePack/map.obj")) return false;

	readMapInfo();
	readResourceInfo();
	readNodeLineInfo();

	for(int i = 0;i < m_nodeLineInfo.size();++ i)
	{
		m_nodeLineInfo[i]->setSpecialPointSpeed(0.08f);
		m_nodeLineInfo[i]->resetSpecialPoint(XTrue);

	}

	return true;
}
void GGame3D::move(float stepTime)
{
	m_carPhysics->update(m_controlState);
	m_world.Step(stepTime * 0.001f, 6, 2);
	for(int i = 0;i < m_nodeLineInfo.size();++ i)
	{
		m_nodeLineInfo[i]->moveSpecialPoint(stepTime,1,XTrue);
		if(m_nodeLineInfo[i]->getSpecialPointIsEnd())
			m_nodeLineInfo[i]->resetSpecialPoint(XTrue);
	}

	m_car3D.setTireAngle(0,m_carPhysics->getAngle(0).y);
	m_car3D.setTireAngle(1,m_carPhysics->getAngle(1).y);
	m_car3D.setTireAngle(2,m_carPhysics->getAngle(2).y);
	m_car3D.setTireAngle(3,m_carPhysics->getAngle(3).y);
	m_car3D.setTireSpeed(0,m_carPhysics->getTireSpeed(0) * 0.03);
	m_car3D.setTireSpeed(1,m_carPhysics->getTireSpeed(1) * 0.03);
	m_car3D.setTireSpeed(2,m_carPhysics->getTireSpeed(2) * 0.03);
	m_car3D.setTireSpeed(3,m_carPhysics->getTireSpeed(3) * 0.03);

	XVector3 tmp[4];

	float height = 0.0f;
	for(int i = 0;i < 4;++ i)
	{
		tmp[i] = m_carPhysics->getPosition(i);
		tmp[i].y = getMapHeight(tmp[i].x,tmp[i].z);
		height += tmp[i].y;
	}
	m_car3D.setPosition(m_carPhysics->getPosition().x,height * 0.25 + 1.4f,m_carPhysics->getPosition().z);

	XMatrix4x4 tmpMat = XMath::getRotate(XVector3(XVector2(12.3f,((tmp[1].y + tmp[0].y) - (tmp[3].y + tmp[2].y)) * 0.5f).getAngle() * RADIAN2DEGREE,0.0f,0.0f));
	tmpMat = tmpMat * XMath::getRotate(XVector3(0.0f,0.0f,-(XVector2(7.0f,((tmp[2].y + tmp[0].y) - (tmp[3].y + tmp[1].y)) * 0.5f).getAngle() * RADIAN2DEGREE)));
	m_car3D.setAngle(0.0f,m_carPhysics->getAngle().y,0.0f);
	m_car3D.setMultMatrix(tmpMat);

	if(m_needDraCar)
	{
		X3DWld.m_worldCam.setPosition(m_car3D.getPosition() +
			XVector3(-50.0f * sin(m_carPhysics->getAngle().y * DEGREE2RADIAN),10.0f,-50.0f * cos(m_carPhysics->getAngle().y * DEGREE2RADIAN)));
		X3DWld.m_worldCam.setAngle(0.0f,m_carPhysics->getAngle().y,0.0f);
	}

	m_car3D.move(stepTime);

	XVector2 pos = m_nodeLineInfo[0]->getSpecialPointPosition();
	m_pcCar3D.setPosition(pos.x,getMapHeight(pos.x,pos.y) + 1.4f,pos.y);
	m_pcCar3D.setAngle(0.0f,-m_nodeLineInfo[0]->getSpecialPointAngle() * RADIAN2DEGREE + 90.0f,0.0f);
	m_pcCar3D.setTireAngle(0,0.0f);
	m_pcCar3D.setTireAngle(1,0.0f);
	m_pcCar3D.setTireAngle(2,0.0f);
	m_pcCar3D.setTireAngle(3,0.0f);
	m_pcCar3D.setTireSpeed(0,48.0f * 0.03);
	m_pcCar3D.setTireSpeed(1,48.0f * 0.03);
	m_pcCar3D.setTireSpeed(2,48.0f * 0.03);
	m_pcCar3D.setTireSpeed(3,48.0f * 0.03);
	m_pcCar3D.move(stepTime);
}
void GGame3D::draw()
{
}
void GGame3D::input(const XInputEvent &event)
{
	if(event.type == EVENT_KEYBOARD)
	{
		if(event.keyState == KEY_STATE_DOWN)
		{
			switch (event.keyValue)
			{
			case XKEY_K: m_controlState |= TDC_LEFT; break;
			case XKEY_H: m_controlState |= TDC_RIGHT; break;
			case XKEY_U: m_controlState |= TDC_UP; break;
			case XKEY_J: m_controlState |= TDC_DOWN; break;

			case XKEY_SPACE:m_needDraCar = !m_needDraCar;break;
			case XKEY_L:
				{
					m_needLight = !m_needLight;
					X3DWld.setWithShadow(m_needLight);
					X3DWld.setWithDumpMap(m_needLight);
				}
				break;
			}
		}else
		if(event.keyState == KEY_STATE_UP)
		{
			switch (event.keyValue)
			{
			case XKEY_K: m_controlState &= ~TDC_LEFT; break;
			case XKEY_H: m_controlState &= ~TDC_RIGHT; break;
			case XKEY_U: m_controlState &= ~TDC_UP; break;
			case XKEY_J: m_controlState &= ~TDC_DOWN; break;
			}
		}
	}

}
void GGame3D::release()
{
	for(int i = 0;i < m_objModels.size();++ i)
	{
		XMem::XDELETE(m_objModels[i]);
	}
	for(int i = 0;i < m_nodeLineInfo.size();++ i)
	{
		XMem::XDELETE(m_nodeLineInfo[i]);
	}
	m_objModels.clear();
}
bool GGame3D::readMapInfo()
{
	TiXmlDocument doc(MAP_FILE_NAME);
	if(!doc.LoadFile()) return false;
	TiXmlNode *rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return false;
	TiXmlNode *keyNode = NULL;

	std::string tmpString;
	if(XXml::getXmlAsInt(rootNode,"gridSum",m_mapSize) == NULL) return false;

	keyNode = rootNode->FirstChildElement("objectInfo");
	while(1)
	{
		if(keyNode != NULL)
		{
			TiXmlNode *tmpNode = NULL;
			GObjectInfo tmp;
			int x,y;
			XXml::getXmlAsInt(keyNode,"index",tmp.index);
			XXml::getXmlAsInt(keyNode,"posX",x);
			XXml::getXmlAsInt(keyNode,"posY",y);
			XXml::getXmlAsFloat(keyNode,"angle",tmp.angle);
			XXml::getXmlAsFloat(keyNode,"size",tmp.size);
			x = x * 1024.0f / m_mapSize;
			y = y * 1024.0f / m_mapSize;
			if(x >= 0 && x < 1024 && y >= 0 && y < 1024)
			{
				tmp.pos.set(x,m_mapData[x + y * 1024],y);
			}else
			{
				tmp.pos.set(x,0.0f,y);
			}
			m_objInfo.push_back(tmp);
		}else
		{
			break;
		}
		keyNode = keyNode->NextSiblingElement("objectInfo");
	}
	return true;
}
bool GGame3D::readResourceInfo()
{
	TiXmlDocument doc("resourceInfo.xml");
	if(!doc.LoadFile()) return false;
	TiXmlNode *rootNode = NULL;
	rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return false;
	TiXmlNode *keyNode = NULL;
	std::string tmpString;

	keyNode = rootNode->FirstChildElement("resource");
	while(1)
	{
		if(keyNode != NULL)
		{
			XXml::getXmlAsString(keyNode,"filename",tmpString);
			XModelObj *tmpModel = XMem::createMem<XModelObj>();
			if(tmpModel == NULL || !tmpModel->load(tmpString.c_str()))
				printf("×ÊÔ´ÔØÈëÊ§°Ü:%s\n",tmpString.c_str());
			else
				m_objModels.push_back(tmpModel);
		}else break;
		keyNode = keyNode->NextSiblingElement("resource");
	}
	return true;
}
bool GGame3D::readNodeLineInfo()
{
	TiXmlDocument doc("path.xml");
	if(!doc.LoadFile()) return false;
	TiXmlNode *rootNode = NULL;
	rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return false;
	TiXmlNode *keyNode = NULL;
	TiXmlNode *keyNodePoint = NULL;
	std::string tmpString;

	keyNode = rootNode->FirstChildElement("path");
	while(1)
	{
		if(keyNode != NULL)
		{
			XVector2 point;
			keyNodePoint = XXml::getXmlAsString(keyNode,"point",tmpString);
			XNodeLine *tmpNodeLine = XMem::createMem<XNodeLine>();
			if(tmpNodeLine == NULL) return false;
			m_nodeLineInfo.push_back(tmpNodeLine);
			while(1)
			{
				if(keyNodePoint != NULL)
				{
					sscanf(tmpString.c_str(),"%f,%f",&point.x,&point.y);
					tmpNodeLine->addOneNode(point);
				}else break;
				keyNodePoint = XXml::getNextXmlAsString(keyNodePoint,"point",tmpString);
			}
			tmpNodeLine->setLoop();
		}else break;
		keyNode = keyNode->NextSiblingElement("path");
	}
	return true;
}