#include "XStdHead.h"
#include "XAutoShrinkBase.h"
namespace XE{
void XAutoShrinkBase::updateShrinkState(float stepTime)	//更新收缩的状态
{
	if(!m_withAutoShrink) return;
	if(getIsInShrinkRect())
	{
		if(!m_oldIsInRect && m_isInShrink) m_shrinkTimer = 0.0f;
		m_oldIsInRect = true;
	}else
	{
		if(m_oldIsInRect && !m_isInShrink) m_shrinkTimer = 0.0f;
		m_oldIsInRect = false;
	}
	if(m_isInShrink)
	{//判断是否要打开
		if(m_oldIsInRect)
		{
			m_shrinkTimer += stepTime;
			if(m_shrinkTimer >= 200.0f)
			{//播放打开动作
				disShrink();
			}
		}
	}else
	{//判断是否要收缩
		XShrinkDirection tmp = calShrinkDir();
		if(tmp != DIR_NULL && !m_oldIsInRect)
		{//有方向可以收缩
			m_shrinkTimer += stepTime;
			if(m_shrinkTimer >= 200.0f)
			{//播放收缩动作
				startShrink(tmp);
			}
		}
	}
	if(!m_shrinkMD.getIsEnd())
	{
		m_shrinkMD.move(stepTime);
		m_shrinkRate = m_shrinkMD.getCurData();
		updateShrinkPosition();
	}
}
}