#ifndef _JIA_GGAME3D_
#define _JIA_GGAME3D_
#include "XTopDownCar.h"
#include "X3D/X3DWorld.h"
#include "XCar3D.h"

#define MAP_FILE_NAME "mapInfo.xml"
struct GObjectInfo
{
	int index;
	XE::XVector3 pos;
	float angle;
	float size;
};

class GGame3D:public XE::XGameBasic3D
{
public:
	GGame3D()
		:m_world(b2Vec2(0,0))
		,m_needDraCar(true)
		,m_needLight(true)
	{}
	virtual ~GGame3D(){}
protected:
	GGame3D(const GGame3D&);
	GGame3D &operator= (const GGame3D&);
public:
	bool init();
	void move(float stepTime);
	void draw();
	void draw3D(XE::XBool withTex);
	void input(const XE::XInputEvent &event);
	void release();
	void ctrlEventProc(int id,int eventID) {}

	XCar3D m_car3D;
	XCar3D m_pcCar3D;

	XTDCar *m_carPhysics;
	b2World m_world;
	int m_controlState;
	int m_controlStateO;
    XMyDestructionListener m_destructionListener;
	XMyContactListener m_contactListener;
    b2Body* m_groundBody;

	bool m_needDraCar;
	bool m_needLight;

	XE::XModelObj m_modelMap;
	float *m_mapData;
	int m_mapSize;
	std::vector<XE::XModelObj *> m_objModels;
	std::vector<GObjectInfo> m_objInfo;
	std::vector<XE::XNodeLine *> m_nodeLineInfo;

	bool readMapInfo();

	bool readResourceInfo();
	bool readNodeLineInfo();
	void drawObject()
	{
		for(int i = 0;i < m_objInfo.size();++ i)
		{
			int index = m_objInfo[i].index;
			if(index >= 0 && index <= m_objModels.size())
			{

				m_objModels[index]->setSize(m_objInfo[i].size,m_objInfo[i].size,m_objInfo[i].size);
				m_objModels[index]->setPosition(m_objInfo[i].pos);
				m_objModels[index]->draw();
			}
		}
	}
	float getMapHeight(int x,int y)
	{
		if(x < 0 || x >= 1024
			|| y < 0 || y >= 1024)
			return 0.0f;
		else return  m_mapData[x + y * 1024];
	}
};
#endif