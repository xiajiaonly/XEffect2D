#include "XStdHead.h"
#include "XMoveData.h"
namespace XE{
float XMoveData::getValue(float time)
{
	if(time < 0.0f) time = 0.0f;
	float curTime = time * m_speed;
	if(m_isLoop == 0)
	{//如果不循环
		if(curTime >= 1.0f)
		{
			return m_endData;
		}else
		{
			switch(m_mode)
			{
			case MOVE_DATA_MODE_LINE:
				return m_startData + (m_endData - m_startData) * curTime;
				break;
			case MOVE_DATA_MODE_SIN:
				return m_startData + (m_endData - m_startData) * sin(curTime * PI_HALF);
				break;
			case MOVE_DATA_MODE_SIN_MULT:
				return m_startData + (m_endData - m_startData) * (1.0f - (curTime - 1.0f) * (curTime - 1.0f));
				break;
			case MOVE_DATA_MODE_SQRT2:
				return m_startData + (m_endData - m_startData) * pow(curTime,0.25f);
				break;
			case MOVE_DATA_MODE_SQRT2_MULT:
				return m_startData + (m_endData - m_startData) * (1.0f - (curTime - 1.0f) * (curTime - 1.0f) * (curTime - 1.0f) * (curTime - 1.0f));
				break;
			case MOVE_DATA_MODE_COS:
				return m_startData + (m_endData - m_startData) * (1.0f - cos(curTime * PI_HALF));
				break;
			case MOVE_DATA_MODE_COS_MULT:
				return m_startData + (m_endData - m_startData) * curTime * curTime;
				break;
			case MOVE_DATA_MODE_DSIN:
				if(curTime < 0.5f) return m_startData + (m_endData - m_startData) * sin(curTime * PI) * 0.5f;
				else return m_startData + (m_endData - m_startData) * (2.0f - sin(curTime * PI)) * 0.5f;
				break;
			case MOVE_DATA_MODE_DCOS:
				return m_startData + (m_endData - m_startData) * (1.0f - cos(curTime * PI)) * 0.5f;
				break;
			case MOVE_DATA_MODE_DSIN_MULT:
				if(curTime < 0.5f) return m_startData + (m_endData - m_startData) * (0.25f - (curTime - 0.5f) * (curTime - 0.5f)) * 2.0f;
				else return m_startData + (m_endData - m_startData) * (0.5f + (curTime - 0.5f) * (curTime - 0.5f) * 2.0f);
				break;
			case MOVE_DATA_MODE_DCOS_MULT:
				if(curTime < 0.5f)return m_startData + (m_endData - m_startData) * curTime * curTime * 2.0f;
				else return m_startData + (m_endData - m_startData) * (1.0f - (curTime - 1.0f) * (curTime - 1.0f) * 2.0f);
				break;
			case MOVE_DATA_MODE_SHAKE:
				//return (m_startData - m_endData) * (1.0f - curTime) * cos(curTime * PI * 2.5f) + m_endData;	//这里需要重新设计
				return (m_startData - m_endData) * (1.0f - curTime) * (1.0f - curTime) * (1.0f - curTime) * cos(curTime * PI * 2.5f) + m_endData;	//这里需要重新设计
				break;
			}
		}
	}else
	{//如果循环
		if(curTime >= 2.0f)
		{
			curTime = 0.0f;
			int loopTimes = curTime * 0.5f;
			if(loopTimes > m_isLoop) return m_endData;	//结束
			curTime -= loopTimes * 2.0f;
		}
		if(curTime >= 1.0f)
		{
			switch(m_mode)
			{
			case MOVE_DATA_MODE_LINE:
				return m_startData + (m_endData - m_startData) * (2.0f - curTime);
				break;
			case MOVE_DATA_MODE_SIN:
				return m_startData + (m_endData - m_startData) * sin((2.0f - curTime) * PI_HALF);
				break;
			case MOVE_DATA_MODE_SIN_MULT:
				return m_startData + (m_endData - m_startData) * (1.0f - (curTime - 1.0f) * (curTime - 1.0f));
				break;
			case MOVE_DATA_MODE_SQRT2:
				return m_startData + (m_endData - m_startData) * pow(2.0f - curTime,0.25f);
				break;
			case MOVE_DATA_MODE_SQRT2_MULT:
				return m_startData + (m_endData - m_startData) * (1.0f - (curTime - 1.0f) * (curTime - 1.0f) * (curTime - 1.0f) * (curTime - 1.0f));
				break;
			case MOVE_DATA_MODE_DSIN:
				if(curTime < 1.5f) return m_startData + (m_endData - m_startData) * (2.0f - sin((2.0f - curTime) * PI)) * 0.5f;
				else return m_startData + (m_endData - m_startData) * sin((2.0f - curTime) * PI) * 0.5f;
				break;
			case MOVE_DATA_MODE_COS:
				return m_startData + (m_endData - m_startData) * (1.0f - cos((2.0f - curTime) * PI_HALF));
				break;
			case MOVE_DATA_MODE_COS_MULT:
				return m_startData + (m_endData - m_startData) * (2.0f - curTime) * (2.0f - curTime);
				break;
			case MOVE_DATA_MODE_DCOS:
				return m_startData + (m_endData - m_startData) * (1.0f - cos((2.0f - curTime) * PI)) * 0.5f;
				break;
			case MOVE_DATA_MODE_DSIN_MULT:
				if(curTime < 1.5f) return m_startData + (m_endData - m_startData) * (0.5f + (curTime - 1.5f) * (curTime - 1.5f) * 2.0f);
				else return m_startData + (m_endData - m_startData) * (0.25f - (curTime - 1.5f) * (curTime - 1.5f)) * 2.0f;
				break;
			case MOVE_DATA_MODE_DCOS_MULT:
				if(curTime < 1.5f) return m_startData + (m_endData - m_startData) * (1.0f - (curTime - 1.0f) * (curTime - 1.0f) * 2.0f);
				else return m_startData + (m_endData - m_startData) * (curTime - 2.0f) * (curTime - 2.0f) * 2.0f;
				break;
			case MOVE_DATA_MODE_SHAKE:
				return (m_startData - m_endData) * (curTime - 1.0f) * (curTime - 1.0f) * (curTime - 1.0f) * cos((2.0f - curTime) * PI * 2.5f) + m_endData;
				break;
			}
		}else
		{
			switch(m_mode)
			{
			case MOVE_DATA_MODE_LINE:
				return m_startData + (m_endData - m_startData) * curTime;
				break;
			case MOVE_DATA_MODE_SIN:
				return m_startData + (m_endData - m_startData) * sin(curTime * PI_HALF);
				break;
			case MOVE_DATA_MODE_SIN_MULT:
				return m_startData + (m_endData - m_startData) * (1.0f - (curTime - 1.0f) * (curTime - 1.0f));
				break;
			case MOVE_DATA_MODE_SQRT2:
				return m_startData + (m_endData - m_startData) * pow(curTime,0.25f);
				break;
			case MOVE_DATA_MODE_SQRT2_MULT:
				return m_startData + (m_endData - m_startData) * (1.0f - (curTime - 1.0f) * (curTime - 1.0f) * (curTime - 1.0f) * (curTime - 1.0f));
				break;
			case MOVE_DATA_MODE_DSIN:
				if(curTime < 0.5f) return m_startData + (m_endData - m_startData) * sin(curTime * PI) * 0.5f;
				else return m_startData + (m_endData - m_startData) * (2.0f - sin(curTime * PI)) * 0.5f;
				break;
			case MOVE_DATA_MODE_COS:
				return m_startData + (m_endData - m_startData) * (1.0f - cos(curTime * PI_HALF));
				break;
			case MOVE_DATA_MODE_COS_MULT:
				return m_startData + (m_endData - m_startData) * curTime * curTime;
				break;
			case MOVE_DATA_MODE_DCOS:
				return m_startData + (m_endData - m_startData) * (1.0f - cos(curTime * PI)) * 0.5f;
				break;
			case MOVE_DATA_MODE_DSIN_MULT:
				if(curTime < 0.5f) return m_startData + (m_endData - m_startData) * (0.25f - (curTime - 0.5f) * (curTime - 0.5f)) * 2.0f;
				else return m_startData + (m_endData - m_startData) * (0.5f + (curTime - 0.5f) * (curTime - 0.5f) * 2.0f);
				break;
			case MOVE_DATA_MODE_DCOS_MULT:
				if(curTime < 0.5f) return m_startData + (m_endData - m_startData) * curTime * curTime * 2.0f;
				else return m_startData + (m_endData - m_startData) * (1.0f - (curTime - 1.0f) * (curTime - 1.0f) * 2.0f);
				break;
			case MOVE_DATA_MODE_SHAKE:
				return (m_startData - m_endData) * (1.0f - curTime) * (1.0f - curTime) * (1.0f - curTime) * cos(curTime * PI * 2.5f) + m_endData;
				break;
			}
		}
	}
	return m_startData;
}
void XMoveData::move(float delay)
{
	if(m_isEnd) return;
	m_timer += delay * m_speed;
	if(m_isLoop == 0)
	{//如果不循环
		if(m_timer >= 1.0f)
		{
			m_isEnd = XTrue;
			m_timer = 1.0f;
			m_curData = m_endData;
		}else
		{
			switch(m_mode)
			{
			case MOVE_DATA_MODE_LINE:
				m_curData = m_startData + (m_endData - m_startData) * m_timer;
				break;
			case MOVE_DATA_MODE_SIN:
				m_curData = m_startData + (m_endData - m_startData) * sin(m_timer * PI_HALF);
				break;
			case MOVE_DATA_MODE_SIN_MULT:
				m_curData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f));
				break;
			case MOVE_DATA_MODE_SQRT2:
				m_curData = m_startData + (m_endData - m_startData) * pow(m_timer,0.25f);
				break;
			case MOVE_DATA_MODE_SQRT2_MULT:
				m_curData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f) * (m_timer - 1.0f) * (m_timer - 1.0f));
				break;
			case MOVE_DATA_MODE_COS:
				m_curData = m_startData + (m_endData - m_startData) * (1.0f - cos(m_timer * PI_HALF));
				break;
			case MOVE_DATA_MODE_COS_MULT:
				m_curData = m_startData + (m_endData - m_startData) * m_timer * m_timer;
				break;
			case MOVE_DATA_MODE_DSIN:
				if(m_timer < 0.5f)m_curData = m_startData + (m_endData - m_startData) * sin(m_timer * PI) * 0.5f;
				else m_curData = m_startData + (m_endData - m_startData) * (2.0f - sin(m_timer * PI)) * 0.5f;
				break;
			case MOVE_DATA_MODE_DCOS:
				m_curData = m_startData + (m_endData - m_startData) * (1.0f - cos(m_timer * PI)) * 0.5f;
				break;
			case MOVE_DATA_MODE_DSIN_MULT:
				if(m_timer < 0.5f) m_curData = m_startData + (m_endData - m_startData) * (0.25f - (m_timer - 0.5f) * (m_timer - 0.5f)) * 2.0f;
				else m_curData = m_startData + (m_endData - m_startData) * (0.5f + (m_timer - 0.5f) * (m_timer - 0.5f) * 2.0f);
				break;
			case MOVE_DATA_MODE_DCOS_MULT:
				if(m_timer < 0.5f)m_curData = m_startData + (m_endData - m_startData) * m_timer * m_timer * 2.0f;
				else m_curData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f) * 2.0f);
				break;
			case MOVE_DATA_MODE_SHAKE:
				//m_curData = (m_startData - m_endData) * (1.0f - m_timer) * cos(m_timer * PI * 2.5f) + m_endData;	//这里需要重新设计
				m_curData = (m_startData - m_endData) * (1.0f - m_timer) * (1.0f - m_timer) * (1.0f - m_timer) * cos(m_timer * PI * 2.5f) + m_endData;	//这里需要重新设计
				break;
			}
		}
	}else
	{//如果循环
		if(m_timer >= 2.0f)
		{
			//m_timer = 0.0f;
			//m_curData = m_startData;
			//if(m_loopTimer > 0)
			//{
			//	-- m_loopTimer;
			//	if(m_loopTimer == 0) m_isEnd = XTrue;
			//}
			int loopTimes = m_timer * 0.5f;
			m_timer -= loopTimes * 2.0f;
			if(m_loopTimer > 0)
			{
				m_loopTimer -= loopTimes;
				if(m_loopTimer <= 0) 
				{
					m_isEnd = XTrue;
					m_curData = m_startData;
					return;
				}
			}
		}
		if(m_timer >= 1.0f)
		{
			switch(m_mode)
			{
			case MOVE_DATA_MODE_LINE:
				m_curData = m_startData + (m_endData - m_startData) * (2.0f - m_timer);
				break;
			case MOVE_DATA_MODE_SIN:
				m_curData = m_startData + (m_endData - m_startData) * sin((2.0f - m_timer) * PI_HALF);
				break;
			case MOVE_DATA_MODE_SIN_MULT:
				m_curData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f));
				break;
			case MOVE_DATA_MODE_SQRT2:
				m_curData = m_startData + (m_endData - m_startData) * pow(2.0f - m_timer,0.25f);
				break;
			case MOVE_DATA_MODE_SQRT2_MULT:
				m_curData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f) * (m_timer - 1.0f) * (m_timer - 1.0f));
				break;
			case MOVE_DATA_MODE_DSIN:
				if(m_timer < 1.5f) m_curData = m_startData + (m_endData - m_startData) * (2.0f - sin((2.0f - m_timer) * PI)) * 0.5f;
				else m_curData = m_startData + (m_endData - m_startData) * sin((2.0f - m_timer) * PI) * 0.5f;
				break;
			case MOVE_DATA_MODE_COS:
				m_curData = m_startData + (m_endData - m_startData) * (1.0f - cos((2.0f - m_timer) * PI_HALF));
				break;
			case MOVE_DATA_MODE_COS_MULT:
				m_curData = m_startData + (m_endData - m_startData) * (2.0f - m_timer) * (2.0f - m_timer);
				break;
			case MOVE_DATA_MODE_DCOS:
				m_curData = m_startData + (m_endData - m_startData) * (1.0f - cos((2.0f - m_timer) * PI)) * 0.5f;
				break;
			case MOVE_DATA_MODE_DSIN_MULT:
				if(m_timer < 1.5f) m_curData = m_startData + (m_endData - m_startData) * (0.5f + (m_timer - 1.5f) * (m_timer - 1.5f) * 2.0f);
				else m_curData = m_startData + (m_endData - m_startData) * (0.25f - (m_timer - 1.5f) * (m_timer - 1.5f)) * 2.0f;
				break;
			case MOVE_DATA_MODE_DCOS_MULT:
				if(m_timer < 1.5f) m_curData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f) * 2.0f);
				else m_curData = m_startData + (m_endData - m_startData) * (m_timer - 2.0f) * (m_timer - 2.0f) * 2.0f;
				break;
			case MOVE_DATA_MODE_SHAKE:
				m_curData = (m_startData - m_endData) * (m_timer - 1.0f) * (m_timer - 1.0f) * (m_timer - 1.0f) * cos((2.0f - m_timer) * PI * 2.5f) + m_endData;
				break;
			}
		}else
		{
			switch(m_mode)
			{
			case MOVE_DATA_MODE_LINE:
				m_curData = m_startData + (m_endData - m_startData) * m_timer;
				break;
			case MOVE_DATA_MODE_SIN:
				m_curData = m_startData + (m_endData - m_startData) * sin(m_timer * PI_HALF);
				break;
			case MOVE_DATA_MODE_SIN_MULT:
				m_curData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f));
				break;
			case MOVE_DATA_MODE_SQRT2:
				m_curData = m_startData + (m_endData - m_startData) * pow(m_timer,0.25f);
				break;
			case MOVE_DATA_MODE_SQRT2_MULT:
				m_curData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f) * (m_timer - 1.0f) * (m_timer - 1.0f));
				break;
			case MOVE_DATA_MODE_DSIN:
				if(m_timer < 0.5f) m_curData = m_startData + (m_endData - m_startData) * sin(m_timer * PI) * 0.5f;
				else m_curData = m_startData + (m_endData - m_startData) * (2.0f - sin(m_timer * PI)) * 0.5f;
				break;
			case MOVE_DATA_MODE_COS:
				m_curData = m_startData + (m_endData - m_startData) * (1.0f - cos(m_timer * PI_HALF));
				break;
			case MOVE_DATA_MODE_COS_MULT:
				m_curData = m_startData + (m_endData - m_startData) * m_timer * m_timer;
				break;
			case MOVE_DATA_MODE_DCOS:
				m_curData = m_startData + (m_endData - m_startData) * (1.0f - cos(m_timer * PI)) * 0.5f;
				break;
			case MOVE_DATA_MODE_DSIN_MULT:
				if(m_timer < 0.5f) m_curData = m_startData + (m_endData - m_startData) * (0.25f - (m_timer - 0.5f) * (m_timer - 0.5f)) * 2.0f;
				else m_curData = m_startData + (m_endData - m_startData) * (0.5f + (m_timer - 0.5f) * (m_timer - 0.5f) * 2.0f);
				break;
			case MOVE_DATA_MODE_DCOS_MULT:
				if(m_timer < 0.5f) m_curData = m_startData + (m_endData - m_startData) * m_timer * m_timer * 2.0f;
				else m_curData = m_startData + (m_endData - m_startData) * (1.0f - (m_timer - 1.0f) * (m_timer - 1.0f) * 2.0f);
				break;
			case MOVE_DATA_MODE_SHAKE:
				m_curData = (m_startData - m_endData) * (1.0f - m_timer) * (1.0f - m_timer) * (1.0f - m_timer) * cos(m_timer * PI * 2.5f) + m_endData;
				break;
			}
		}
	}
}
#if !WITH_INLINE_FILE
#include "XMoveData.inl"
#endif
}