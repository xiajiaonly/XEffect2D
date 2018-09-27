#ifndef _JIA_XSIMPLETRACKER_
#define _JIA_XSIMPLETRACKER_
#include "XMath\XVector2.h"
namespace XE {
//被追踪的物体的信息
enum XTObjState
{
	STATE_NULL,	//无效的状态
	STATE_BORN,	//正在出生
	STATE_LIVE,	//正在存活
	STATE_DIE,	//正在死亡
};
struct XTObjInfo
{
	int typeID;			//物件的ID
	int id;				//是别的物体的ID
	XVec2 pos;			//物体的位置
	XVec2 speed;		//物体的速度
	float life;			//物体的生命
	float bornTimer;	//出生计时
	float dieTimer;		//死亡计时
	float upSetTime;	//上次更新数据的时间点
	XTObjState state;	//物体的状态
	XTObjState upState;	//上一次的状态
	//下面是计算的中间变量
	float steptime;		//两次数据更新之间的时间差
	XVec2 estimatedPos;	//预估的位置
	bool isTracked;		//是否做过跟踪计算
	float maxDis;		//可能移动的最大距离
	XTObjInfo()
		:id(-1)
		, life(0.0f)
		, bornTimer(0.0f)
		, upSetTime(0.0f)
		, state(STATE_NULL)
	{}
};
class XSimpleTracker
{
private:
	int m_curObjID;
	float m_minBornTime;	//出生时间的阈值(秒)
	float m_minDieTime;		//死亡时间的阈值(秒)
	float m_maxSpeed;		//物体运动的最大速度(秒)
	std::vector<XTObjInfo> m_curObjs;	//当前的物体信息
	std::vector<XTObjInfo> m_oldObjs;	//原来的物体信息
	void bornAObj(int typeID, XVec2 pos);
public:
	int getCurObjID()const { return m_curObjID; }
	//需要预先将物体的状态设置为STATE_NULL
	void tracker(std::vector<XTObjInfo>& objs);
	//需要预先将物体的状态设置为STATE_NULL
	void trackerByID(std::vector<XTObjInfo>& objs);
	void update(float steptime);
	const std::vector<XTObjInfo>& getCurObjs()const { return m_curObjs; }

	//下面是参数的操作
	float getMinBornTime()const { return m_minBornTime; }
	float getMinDieTime()const { return m_minDieTime; }
	float getMaxSpeed()const { return m_maxSpeed; }
	void setMinBornTime(float s) { m_minBornTime = s; }
	void setMinDieTime(float s) { m_minDieTime = s; }
	void setMaxSpeed(float s) { m_maxSpeed = s; }
	XSimpleTracker()
		:m_minBornTime(0.25f)
		, m_minDieTime(0.25f)
		, m_maxSpeed(512.0f)
		, m_curObjID(0)
	{}
};
}
#endif