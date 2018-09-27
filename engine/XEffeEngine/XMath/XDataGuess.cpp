#include "XStdHead.h"
#include "XDataGuess.h"
namespace XE{
void XDataGuess::pushAData(const XVec2& data)
{
	if(!m_isInited) return;
	switch (m_mode)
	{
	case GUESS_MODLE_LINE:
		if(m_datas.size() == 0)
		{
			m_datas.push_back(data);
		}else
		{//检查数据是否发生重复
			XVec2& end = *(m_datas.end() - 1);
			if(end.x == data.x) end = data;
			else m_datas.push_back(data);
			if(m_datas.size() > 2) m_datas.pop_front();
		}
		break;
	case GUESS_MODLE_SECOND:
		if(m_datas.size() == 0)
		{
			m_datas.push_back(data);
		}else
		{//检查数据是否发生重复
			XVec2& end = *(m_datas.end() - 1);
			if(end.x == data.x) end = data;
			else m_datas.push_back(data);
			if(m_datas.size() > 3) m_datas.pop_front();
		}
		break;
	case GUESS_MODLE_LINE_FIT:
		if(m_datas.size() == 0)
		{
			m_datas.push_back(data);
		}else
		if(m_datas.size() == 1)
		{//检查数据是否发生重复
			XVec2& end = m_datas[0];
			if(end.x == data.x) end = data;
			else m_datas.push_back(data);
		}else
		{
			XVec2 tmp = data;
			tmp.y = (tmp.y + guessData(tmp.x)) * 0.5f;
			XVec2& end = *(m_datas.end() - 1);
			if(end.x == tmp.x) end = tmp;
			else m_datas.push_back(tmp);
			if(m_datas.size() > 2) m_datas.pop_front();
		}
		break;
	case GUESS_MODLE_SECOND_FIT:
		switch (m_datas.size())
		{
		case 0:m_datas.push_back(data);break;
		case 1:
		case 2:
			{
				XVec2& end = *(m_datas.end() - 1);
				if(end.x == data.x) end = data;
				else m_datas.push_back(data);
			}
			break;
		default:
			{
				XVec2 tmp = data;
				tmp.y = (tmp.y + guessData(tmp.x)) * 0.5f;
				XVec2& end = *(m_datas.end() - 1);
				if(end.x == tmp.x) end = tmp;
				else m_datas.push_back(tmp);
				if(m_datas.size() > 3) m_datas.pop_front();
			}
			break;
		}
		break;
	case GUESS_MODLE_PERIODIC:
		m_pDG->pushAData(data);	//为了弥补超过范围的值

		m_cycDatas.pushAData(data,m_cycleInfo);
		if(m_cycDatas.getHaveHead()) m_cycDatasTmp.pushAData(data,m_cycleInfo);
		if(m_cycDatas.getIsComplete())
		{//这里需要检查数据的合法性，以便于丢弃明显不正常的数据
			if(!getIsInvalid(m_cycDatas))
			{
				for(int i = 0;i < static_cast<int>(m_oldCycData.size());++ i)
				{
					if(getIsInvalid(m_oldCycData[i]))
					{
						m_oldCycData.erase(m_oldCycData.begin() + i);
						-- i;
					}
				}
				if(m_oldCycData.size() >= 5) m_oldCycData.pop_front();
				m_oldCycData.push_back(m_cycDatas);
				m_isCycDataUpdate = true;

				m_cycleInfo.addAMaxValue(m_cycDatas.getMaxValue());
				m_cycleInfo.addAMinValue(m_cycDatas.getMinValue());
				m_cycleInfo.addAUpTime(m_cycDatas.getUpTime());
				m_cycleInfo.addADownTime(m_cycDatas.getDownTime());
			}

			m_cycDatas.clear();
			m_cycDatas = m_cycDatasTmp;
			m_cycDatasTmp.clear();
		}
		break;
	}
}
bool XDataGuess::getIsInvalid(const XCycleData &data,bool bSimple)const
{
	if(bSimple)
	{
		if(m_cycleInfo.getIsEnable())
		{
			if(abs(data.getStartValue() - m_cycleInfo.m_minValue) >
				m_cycleInfo.getAmplitude() * 0.01f)
			{
#ifdef WITH_DEBUG_INFO
				std::cout << "无效的数据" << std::endl;
#endif
				return true;
			}
		}	
	}else
	{
		if(data.getDownDatasSum() < 10 || data.getUpDatasSum() < 10)
		{
#ifdef WITH_DEBUG_INFO
			std::cout << "无效的数据" << std::endl;
#endif
			return true;
		}
		if(abs(data.getStartValue() - data.getMinValue()) >
			(data.getMaxValue() - data.getMinValue()) * 0.01f ||
			abs(data.getEndValue() - data.getMinValue()) >
			(data.getMaxValue() - data.getMinValue()) * 0.01f)
		{
#ifdef WITH_DEBUG_INFO
			std::cout << "无效的数据" << std::endl;
#endif
			return true;
		}
		if(m_cycleInfo.getIsEnable())
		{
			if(abs(data.getStartValue() - m_cycleInfo.m_minValue) >
				m_cycleInfo.getAmplitude() * 0.01f ||
				abs(data.getEndValue() - m_cycleInfo.m_minValue) >
				m_cycleInfo.getAmplitude() * 0.01f)
			{
#ifdef WITH_DEBUG_INFO
				std::cout << "无效的数据" << std::endl;
#endif
				return true;
			}
		}
	}
	return false;
}
float XDataGuess::guessData(float x)const
{
	if(!m_isInited) return 0.0f;
	switch (m_mode)
	{
	case GUESS_MODLE_LINE:
	case GUESS_MODLE_LINE_FIT:
		{
			float speed = (m_datas[1].y - m_datas[0].y) / (m_datas[1].x - m_datas[0].x);
			float curT = x - m_datas[1].x;
			return speed * curT + m_datas[1].y;
		}
		break;
	case GUESS_MODLE_SECOND:
	case GUESS_MODLE_SECOND_FIT:
		{
			float speed = (m_datas[2].y - m_datas[1].y) / (m_datas[2].x - m_datas[1].x);
			float tmpSpeed = (m_datas[1].y - m_datas[0].y) / (m_datas[1].x - m_datas[0].x);
			float acc = (speed - tmpSpeed) / (m_datas[2].x - m_datas[1].x);
			float curT = x - m_datas[2].x;
			return speed * curT + acc * curT * curT * 0.5f + m_datas[2].y;
		}
		break;
	case GUESS_MODLE_PERIODIC:
		if(m_oldCycData.size() == 0 || getIsInvalid(m_cycDatas,true))
		{
			return m_pDG->guessData(x);
		}else
		{
			float value = 0.0f;
			float time = x - m_cycDatas.getStartTime();
			float sum = 0.0f;
			float tmpValue;
			for(int i = 0;i < m_oldCycData.size();++ i)
			{
				if(m_oldCycData[i].getValue(time,tmpValue))
				{
					value += tmpValue;
					sum += 1.0f;
				}
			}
			if(sum > 0.0f)
			{
				if(m_cycleInfo.getIsEnable())
				{//如果数据超过预期太多则使用较靠谱的估计
					float tmp = m_pDG->guessData(x);
					float result = value / sum;
					if(abs(tmp - result) > m_cycleInfo.getAmplitude() * 0.05f)
					{
						return tmp;	//如果数值超过太多，则使用较保守的方式
					}else
						return result;
				}else
				{
					return value / sum;
				}				
			}else
				return m_pDG->guessData(x);
		}
		break;
	}
	return 0.0f;
}
bool XDataGuess::getCanGuess()const
{
	if(!m_isInited) return false;
	switch (m_mode)
	{
	case GUESS_MODLE_LINE:
	case GUESS_MODLE_LINE_FIT:
		return m_datas.size() >= 2;
	case GUESS_MODLE_SECOND:
	case GUESS_MODLE_SECOND_FIT:
		return m_datas.size() >= 3;
	case GUESS_MODLE_PERIODIC:
		if(m_oldCycData.size() == 0)
			return m_pDG->getCanGuess();
		else 
			return true;
		break;
	}
	return false;
}
void XDataGuess::release()
{
	XMem::XDELETE(m_pDG);
}
bool XDataGuess::init(XGuessModle mode)
{
	if(m_isInited) return false;
	m_mode = mode;
	switch (m_mode)
	{
	case GUESS_MODLE_PERIODIC:
		m_pDG = XMem::createMem<XDataGuess>();
		if(!m_pDG->init(GUESS_MODLE_LINE_FIT)) return false;
		break;
	}
	m_isInited = true;
	return true;
}
//如果出现数据抖动，会造成逻辑出现问题，需要增加其健壮性
void XCycleData::pushAData(const XE::XVec2& data,const XCycleInfo &info)
{
	if(getIsComplete()) return;	//如果已经是完整的则不能再插入数据
	if(m_datas.size() == 0)
	{
		m_datas.push_back(data);
		m_maxValue = data.y;
		m_minValue = data.y;
	}else
	{
		XVec2& tmp = *(m_datas.end() - 1);
		if(tmp.x == data.x) tmp = data;
		else m_datas.push_back(data);
		if(data.y > m_maxValue) m_maxValue = data.y;
		if(data.y < m_minValue) m_minValue = data.y;

		if(m_datas.size() >= 2)
		{//下面判断周期的走势
			XVec2 endOne = *(m_datas.end() - 1);
			XVec2 endTwo = *(m_datas.end() - 2);
			if(endOne.y < endTwo.y)
			{//这里需要计算顶峰的信息
				m_state = CYC_STA_DOWN;
				if(calTop())//下面计算顶点
					m_upTime = m_topTime - m_startTime;
			}else
			if(endOne.y > endTwo.y)
			{
				if(calEnd())
				{
					m_downTime = m_endTime - m_topTime;
					//这里计算m_downDatasSum 和 m_upDatasSum
					m_downDatasSum = m_endIndex - m_topIndex;
					m_upDatasSum = m_topIndex - m_startIndex;
				}
				calStart(info);

				m_state = CYC_STA_UP;
			}
		}
	}
}
bool XCycleData::getValue(float x,float &y)const
{
	float startT = getStartTime();
	x += startT;
	for(auto it = m_datas.begin();it != m_datas.end() - 1;++ it)
	{
		auto nextT = it + 1;
		if(it->x <= x && nextT->x >= x)
		{
			y = XMath::mapping1D(x, it->x, nextT->x, it->y, nextT->y);
			return true;
		}
	}
	return false;
}
bool XCycleData::calStart(const XCycleInfo &info)	//计算开头
{
	if(m_haveStart) return false;
	int downSum = 0;
	int upSum = 0;
	int errorSum = 0;	//发生异常的数据数量
	for(auto it = m_datas.begin();it != m_datas.end() - 1;++ it)
	{
		auto nextT = it + 1;
		if(it->y < nextT->y)
		{
			downSum = 0;
			//可以丢弃之前的数据
			m_datas.erase(m_datas.begin(), it);
			continue;
		}
		if(it->y == nextT->y) continue;
		if(it->y > nextT->y) ++ downSum;
		if(downSum >= m_turnThreshold)
		{
			int flag = 0;
			errorSum = 0;
			upSum = 0;
			for(auto jt = it + 1;jt != m_datas.end() - 1;++ jt)
			{
				auto nextJT = jt + 1;
				if(it->y == nextJT->y) continue;
				if(jt->y < nextJT->y)
				{//满足要求
					++ upSum;
					errorSum = 0;
					if(upSum == 1)
						m_startEndIndex = nextJT - m_datas.begin();
					if(upSum >= m_turnThreshold)
					{
						flag = 1;
						break;
					}
				}else
				{//存在多余的数据
					upSum = 0;
					++ errorSum;
					if(errorSum >= m_turnThreshold)
					{//发生异常的数据数量超过限制，则抛弃这个数据
						flag = 2;
						break;
					}
				}
			}
			if(flag == 0) return false;	//数据数量不够，没有足够多的正确，也没有足够多的错误
			if(flag == 1)
			{
				m_haveStart = true;
#ifdef WITH_DEBUG_INFO
				std::cout<<"Start"<<std::endl;
#endif
				m_startIndex = ((it - m_datas.begin()) + m_startEndIndex) >> 1;
				auto startEndT = m_datas.begin() + m_startEndIndex;
				//这里需要计算起始地时间
				if(info.getIsEnable())
				{
					float a = it->y - info.m_minValue;
					float b = (startEndT->y - info.m_minValue) * info.m_upDownRate;
					if(a < 0.0f) a = 0.0f;
					if(b < 0.0f) b = 0.0f;
					float c = a + b;
					if(c == 0.0f)
						m_startTime = (it->x + startEndT->x) * 0.5f;
					else
					{
						m_startTime = it->x + (startEndT->x - it->x) * a / c;
					}
				}else
				{
					m_startTime = (it->x + startEndT->x) * 0.5f;
				}
				m_startValue = it->y;
				for(auto jt = it + 1;jt != startEndT;++ jt)
				{
					if(jt->y < m_startValue) m_startValue = jt->y;
				}
				break;
			}else
			if(flag == 2)
			{//需要抛弃掉多余的值
				m_datas.pop_front();
				downSum = 0;
				it = m_datas.begin();
			}
		}
	}
	//assert(m_haveStart);
	return m_haveStart;
}
bool XCycleData::calEnd()		//计算结束
{
	if(m_haveEnd || !m_haveStart || !m_haveTop) return false;
	int downSum = 0;
	int upSum = 0;
	int errorSum = 0;	//发生异常的数据数量
	for(auto it = m_datas.begin() + m_startEndIndex;it != m_datas.end() - 1;++ it)
	{
		auto nextT = it + 1;
		if(it->y < nextT->y)
		{
			downSum = 0;
			continue;
		}
		if(it->y == nextT->y) continue;
		if(it->y > nextT->y) ++ downSum;
		if(downSum >= m_turnThreshold)
		{//下降次数符合要求
			bool flag = false;
			int endIndex = 0;
			errorSum = 0;
			upSum = 0;
			for(auto jt = it + 1;jt != m_datas.end() - 1;++ jt)
			{
				auto nextJT = jt + 1;
				if(jt->y == nextJT->y) continue;
				if(jt->y < nextJT->y)
				{//满足要求
					errorSum = 0;
					++ upSum;
					if(upSum == 1)
						endIndex = nextJT - m_datas.begin();
					if(upSum >= m_turnThreshold)
					{//找到结果
						flag = true;
						m_haveEnd = true;
#ifdef WITH_DEBUG_INFO
						std::cout<<"End"<<std::endl;
#endif
						m_endIndex = ((it - m_datas.begin()) + endIndex) >> 1;
						m_endTime = (it->x + m_datas[m_endIndex].x) * 0.5f;	//这里可以优化
						m_endValue = it->y;
						auto et = m_datas.begin() + endIndex;
						for(auto kt = it + 1;kt != et;++ kt)
						{
							if(kt->y < m_endValue) m_endValue = kt->y;
						}
						return true;
					}
				}else
				{
					upSum = 0;
					++ errorSum;
					if(errorSum >= m_turnThreshold)
					{//太多的错误
						flag = true;
						-- downSum;
						break;
					}
				}
			}
			if(!flag)
			{//没有足够多的正确或者错误，则认为数据数量不够
				return false;
			}
		}
	}
	//assert(m_haveEnd);
	return m_haveEnd;
}
bool XCycleData::calTop()
{
	if(m_haveTop || !m_haveStart) return false;
	int endIndex = 0;
	int downSum = 0;
	int upSum = 0;
	int errorSum = 0;	//发生异常的数据数量
	for(auto it = m_datas.end() - 1;it != m_datas.begin();-- it)
	{
		auto nextT = it - 1;
		if(it->y >= nextT->y) return false;	//不需要再继续遍历
		if(it->y < nextT->y) ++ downSum;
		if(downSum >= m_turnThreshold)
		{
			upSum = 0;
			errorSum = 0;
			for(auto jt = it - 1;jt != m_datas.begin();-- jt)
			{
				auto nextJT = jt - 1;
				if(jt->y < nextJT->y)
				{
					upSum = 0;
					++ errorSum;
					if(errorSum >= m_turnThreshold)
						return false;
				}else
				if(jt->y > nextJT->y)
				{
					++ upSum;
					errorSum = 0;
					if(upSum == 1) endIndex = nextJT - m_datas.begin();
					if(upSum >= m_turnThreshold)
					{//符合要求
						m_haveTop = true;
#ifdef WITH_DEBUG_INFO
						std::cout<<"Top"<<std::endl;
#endif
						m_topIndex = ((it - m_datas.begin()) + endIndex) >> 1;
						m_topTime = (it->x + m_datas[m_topIndex].x) * 0.5f;	//这里可以优化
						return true;
					}
				}
			}
			return false;
		}
	}
	return m_haveTop;
}
}