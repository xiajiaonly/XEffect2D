//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XPressDataMode.h"
#include "math.h"

void _XPressDataModeS::init(float normalValue,float maxDeformValue,int sensitivity)
{
	m_normalData = normalValue;
	m_maxDeformData = maxDeformValue;
	m_nowData = normalValue;
	if(m_normalData <= m_maxDeformData) m_dataFlag = 1.0f;
	else m_dataFlag = -1.0f;
	m_pressState = PRESS_STATE_NORMAL;
	m_sensitivity = sensitivity;
	if(m_sensitivity < 0) m_sensitivity = 0;
	m_nowPressTimer = 0;

	//下面的几个数据使用默认值
	m_startChangeData = 1.0f;
	m_minChangeData = 0.01f;
	m_maxChangeData = 0.1f;
	//m_isRecover = XTrue;

	m_nowDeformData = m_minChangeData;
}

void _XPressDataModeS::setPress()
{
	m_pressState = PRESS_STATE_PRESS;
	m_nowPressTimer = 0;
	if(m_maxDeformData > m_normalData)
	{
		if(m_nowData < m_maxDeformData)
		{
			m_nowData += m_dataFlag * m_startChangeData;
			if(m_nowData > m_maxDeformData) m_nowData = m_maxDeformData;
		}
	}else
	{
		if(m_nowData > m_maxDeformData)
		{
			m_nowData += m_dataFlag * m_startChangeData;
			if(m_nowData < m_maxDeformData) m_nowData = m_maxDeformData;
		}
	}
}

void _XPressDataModeS::move(int stepTime)
{
	if(m_pressState == PRESS_STATE_PRESS)
	{//处于按下状态
		if(m_nowPressTimer < m_sensitivity)
		{//尚且不要做出反应
			m_nowPressTimer += stepTime;
			if(m_nowPressTimer > m_sensitivity) m_nowPressTimer = m_sensitivity;
		}else
		{//开始做出反应
			if(m_nowDeformData < m_maxChangeData)
			{
				m_nowDeformData += 0.01f * stepTime;
				if(m_nowDeformData > m_maxChangeData) m_nowDeformData = m_maxChangeData;
			}
			if(m_maxDeformData > m_normalData)
			{
				if(m_nowData < m_maxDeformData)
				{
					m_nowData += m_dataFlag * m_nowDeformData;
					if(m_nowData > m_maxDeformData) m_nowData = m_maxDeformData;
				}
			}else
			{
				if(m_nowData > m_maxDeformData)
				{
					m_nowData += m_dataFlag * m_nowDeformData;
					if(m_nowData < m_maxDeformData) m_nowData = m_maxDeformData;
				}
			}
		}
	}else
	{//处于弹起状态
		//准备恢复
		if(m_nowDeformData > m_minChangeData)
		{
			m_nowDeformData -= 0.01f * stepTime;
			if(m_nowDeformData < m_minChangeData) m_minChangeData = m_minChangeData;
		}
		//当前数值恢复
		if(abs(m_normalData - m_nowData) < 1.0f)
		{
			m_nowData = m_normalData;
		}else
		{
			m_nowData = m_nowData + (m_normalData - m_nowData) * 0.01f * stepTime;
		}
	}
}

void _XPressDataModeD::init(float normalValue,float maxDeformValue,float minDeformValue,int sensitivity,char canRecover)
{
	m_normalData = normalValue;
	if(maxDeformValue > minDeformValue)
	{
		m_maxDeformData = maxDeformValue;
		m_minDeformData = minDeformValue;
	}else
	{
		m_maxDeformData = minDeformValue;
		m_minDeformData = maxDeformValue;
	}
	m_nowData = normalValue;

	m_pressMinState = PRESS_STATE_NORMAL;
	m_pressMaxState = PRESS_STATE_NORMAL;
	m_sensitivity = sensitivity;
	if(m_sensitivity < 0) m_sensitivity = 0;
	m_nowPressTimer = 0;

	m_startChangeData = 1.0f;
	m_minChangeData = 0.01f;	//这个数值需要可变
	m_maxChangeData = 0.1f;		//这个数值需要可变
	m_isRecover = XTrue;
	m_canRecover = canRecover;

	m_nowDeformData = m_minChangeData;
}

void _XPressDataModeD::setPressMax()
{
	if(m_pressMinState == PRESS_STATE_NORMAL && 
		m_pressMaxState == PRESS_STATE_NORMAL) 
	{
		m_nowPressTimer = 0;
		if(m_nowData < m_maxDeformData)
		{
			m_nowData += m_startChangeData;
			if(m_nowData > m_maxDeformData) m_nowData = m_maxDeformData;
			m_isRecover = XFalse;
		}
	}
	m_pressMaxState = PRESS_STATE_PRESS;
}

void _XPressDataModeD::setPressMin()
{
	if(m_pressMinState == PRESS_STATE_NORMAL && 
		m_pressMaxState == PRESS_STATE_NORMAL) 
	{
		m_nowPressTimer = 0;
		if(m_nowData > m_minDeformData)
		{
			m_nowData -= m_startChangeData;
			if(m_nowData < m_minDeformData) m_nowData = m_minDeformData;
			m_isRecover = XFalse;
		}
	}
	m_pressMinState = PRESS_STATE_PRESS;
}

void _XPressDataModeD::move(int stepTime)
{
	if(m_pressMaxState == PRESS_STATE_PRESS && 
		m_pressMinState == PRESS_STATE_PRESS)	return;//同时按下不做反应
	if(m_pressMaxState == PRESS_STATE_PRESS)
	{//按下增加
		if(m_nowPressTimer < m_sensitivity)
		{//尚且不要做出反应
			m_nowPressTimer += stepTime;
			if(m_nowPressTimer > m_sensitivity) m_nowPressTimer = m_sensitivity;
		}else
		{//开始做出反应
			if(m_nowDeformData < m_maxChangeData)
			{
				m_nowDeformData += 0.01f * stepTime;
				if(m_nowDeformData > m_maxChangeData) m_nowDeformData = m_maxChangeData;
			}
			if(m_nowData < m_maxDeformData)
			{
				m_nowData += m_nowDeformData;
				if(m_nowData > m_maxDeformData) m_nowData = m_maxDeformData;
				m_isRecover = XFalse;	//逻辑上这一行可以不必
			}
		}
	}else
	if(m_pressMinState == PRESS_STATE_PRESS)
	{//按下减小
		if(m_nowPressTimer < m_sensitivity)
		{//尚且不要做出反应
			m_nowPressTimer += stepTime;
			if(m_nowPressTimer > m_sensitivity) m_nowPressTimer = m_sensitivity;
		}else
		{//开始做出反应
			if(m_nowDeformData < m_maxChangeData)
			{
				m_nowDeformData += 0.01f * stepTime;
				if(m_nowDeformData > m_maxChangeData) m_nowDeformData = m_maxChangeData;
			}
			if(m_nowData > m_minDeformData)
			{
				m_nowData -= m_nowDeformData;
				if(m_nowData < m_minDeformData) m_nowData = m_minDeformData;
				m_isRecover = XFalse;	//逻辑上这一行可以不必
			}
		}
	}else
	{//准备恢复
		//准备恢复
		if(m_nowDeformData > m_minChangeData)
		{
			m_nowDeformData -= 0.01f * stepTime;
			if(m_nowDeformData < m_minChangeData) m_minChangeData = m_minChangeData;
		}
		if(m_canRecover == 1 && !m_isRecover)
		{//当前数值缓慢恢复
			if(abs(m_normalData - m_nowData) < 1.0f)
			{
				m_nowData = m_normalData;
				m_isRecover = XTrue;
			}else
			{
				m_nowData = m_nowData + (m_normalData - m_nowData) * 0.01f * stepTime;
			}
		}else
		if(m_canRecover == 2 && !m_isRecover)
		{//立即回复
			m_nowData = m_normalData;
			m_isRecover = XTrue;
		}
	}
}