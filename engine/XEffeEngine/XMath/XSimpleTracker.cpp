#include "XStdHead.h"
#include "XSimpleTracker.h"
namespace XE {
void XSimpleTracker::tracker(std::vector<XTObjInfo>& objs)
{
	//计算生存的
	for (int i = 0; i < m_curObjs.size(); ++i)
	{
		XTObjInfo& ref = m_curObjs[i];
		ref.steptime = ref.life - ref.upSetTime;
		//ref.upSetTime = ref.life;
		ref.estimatedPos = ref.pos + ref.speed * ref.steptime;
		ref.isTracked = false;
		ref.maxDis = m_maxSpeed * ref.steptime;
	}
	for (int j = 0; j < objs.size(); ++j)
	{
		XTObjInfo& ref = objs[j];
		int minIndex = -1;
		float minDis, curDis;
		for (int i = 0; i < m_curObjs.size(); ++i)
		{
			if (m_curObjs[i].isTracked) continue;
			curDis = m_curObjs[i].estimatedPos.getLengthSqure(ref.pos);
			if (minIndex < 0 || curDis < minDis)
			{
				minIndex = i;
				minDis = curDis;
			}
		}

		if (minIndex >= 0 && minDis <= m_curObjs[minIndex].maxDis * m_curObjs[minIndex].maxDis)
		{//符合生存要求
			XTObjInfo& refx = m_curObjs[minIndex];
			refx.speed = (ref.pos - refx.pos) / refx.steptime;
			refx.pos = ref.pos;
			refx.typeID = ref.typeID;
			refx.isTracked = true;
			if (refx.state == STATE_DIE) 
				refx.state = refx.upState;
			ref.upSetTime = ref.life;
		}
		else
		{//不符合生存要求，做新生处理
			bornAObj(ref.typeID, ref.pos);
		}
	}
	//计算死亡的
	for (int i = 0; i < m_curObjs.size(); ++i)
	{
		if (m_curObjs[i].isTracked) continue;
		if (m_curObjs[i].state != STATE_DIE)
		{
			m_curObjs[i].upState = m_curObjs[i].state;
			m_curObjs[i].state = STATE_DIE;
			m_curObjs[i].dieTimer = 0.0f;
		}
	}
}
void XSimpleTracker::trackerByID(std::vector<XTObjInfo>& objs)
{
	//计算生存的
	for (int i = 0; i < m_curObjs.size(); ++i)
	{
		XTObjInfo& ref = m_curObjs[i];
		ref.steptime = ref.life - ref.upSetTime;
		ref.upSetTime = ref.life;
		ref.estimatedPos = ref.pos + ref.speed * ref.steptime;
		ref.isTracked = false;
		ref.maxDis = m_maxSpeed * ref.steptime;
	}
	for (int j = 0; j < objs.size(); ++j)
	{
		XTObjInfo& ref = objs[j];
		int minIndex = -1;
		float minDis, curDis;
		for (int i = 0; i < m_curObjs.size(); ++i)
		{
			if (m_curObjs[i].isTracked || m_curObjs[i].typeID != ref.typeID) continue;
			curDis = m_curObjs[i].estimatedPos.getLengthSqure(ref.pos);
			if (minIndex < 0 || curDis < minDis)
			{
				minIndex = i;
				minDis = curDis;
			}
		}

		if (minIndex >= 0 && minDis <= m_curObjs[minIndex].maxDis * m_curObjs[minIndex].maxDis)
		{//符合生存要求
			XTObjInfo& refx = m_curObjs[minIndex];
			refx.speed = (ref.pos - refx.pos) / refx.steptime;
			refx.pos = ref.pos;
			refx.typeID = ref.typeID;
			refx.isTracked = true;
			if (refx.state == STATE_DIE)
				refx.state = refx.upState;
		}
		else
		{//不符合生存要求，做新生处理
			bornAObj(ref.typeID, ref.pos);
		}
	}
	//计算死亡的
	for (int i = 0; i < m_curObjs.size(); ++i)
	{
		if (m_curObjs[i].isTracked) continue;
		if (m_curObjs[i].state != STATE_DIE)
		{
			m_curObjs[i].upState = m_curObjs[i].state;
			m_curObjs[i].state = STATE_DIE;
			m_curObjs[i].dieTimer = 0.0f;
		}
	}
}
void XSimpleTracker::bornAObj(int typeID, XVec2 pos)
{
	m_curObjs.push_back(XTObjInfo());
	XTObjInfo& ref = m_curObjs[m_curObjs.size() - 1];
	ref.typeID = typeID;
	ref.pos = pos;
	ref.state = STATE_BORN;
}
void XSimpleTracker::update(float steptime)
{
	float curTime = steptime * 0.001f;
	for (auto it = m_curObjs.begin(); it != m_curObjs.end();)
	{
		it->life += curTime;
		switch (it->state)
		{
		case STATE_BORN:
			it->bornTimer += curTime;
			if (it->bornTimer >= m_minBornTime)
			{
				it->state = STATE_LIVE;
				it->id = m_curObjID;
				++m_curObjID;
			}
			break;
		case STATE_DIE:
			it->dieTimer += curTime;
			if (it->dieTimer >= m_minDieTime)
			{//物件死亡
				it = m_curObjs.erase(it);
				continue;
			}
			break;
		}
		++it;
	}
}
}