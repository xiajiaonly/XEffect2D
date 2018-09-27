#include "XStdHead.h"
#include "XActionEx.h"
//++++++++++++++++++++++++++++++++
//Author:	  贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2015.5.20
//--------------------------------
#include "XResourcePack.h"
#include <stdarg.h>
namespace XE {
void XActionEx::setStart()			//设置动作开始进行
{
	if (m_actionState != ACTION_STATE_END) return;
	m_actionState = ACTION_STATE_ACTION;
	switch (m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funResetData();
		return;
	case ACTION_TYPE_COMPLEX://开始播放第一个动作
		if (m_pAction[0] != NULL) m_pAction[0]->setStart();
		m_curActionIndex = 0;
		return;
	}
}
void XActionEx::move(float stepTime)
{
	if (!m_isInited ||
		m_actionState != ACTION_STATE_ACTION) return;
	switch (m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funMove(stepTime);
		if (funIsEnd())
		{
			m_actionState = ACTION_STATE_END;	//标记动作结束
			funAtEnd();
		}
		if (m_actionIsLoop &&
			m_actionState == ACTION_STATE_END) setStart();
		return;
	case ACTION_TYPE_COMPLEX:
		if (!m_pAction[m_curActionIndex]->getIsEnd()) return;
		++m_curActionIndex;
		if (m_curActionIndex >= m_actionSum)
		{//结束
			m_actionState = ACTION_STATE_END;
			if (m_actionIsLoop && m_actionState == ACTION_STATE_END)
			{
				setStart();
			}
		}
		else
		{//播放下一个动作
			m_pAction[m_curActionIndex]->setStart();
		}
		return;
	}
}
void XActionEx::draw()
{
	if (!m_isInited ||
		m_actionState != ACTION_STATE_ACTION) return;	//这个条件有待磋商
	switch (m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funDraw();
		return;
	case ACTION_TYPE_COMPLEX://可以遍历所有组件进行显示
		m_pAction[m_curActionIndex]->draw();
		return;
	}
}
void XActionEx::keepDraw()
{
	if (!m_isInited) return;	//这个条件有待磋商
	switch (m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funKeepDraw();
		return;
	case ACTION_TYPE_COMPLEX://可以遍历所有组件进行显示
		if ((m_curActionIndex >= m_actionSum || m_curActionIndex < 0) && m_actionSum >= 1)
			m_pAction[m_actionSum - 1]->keepDraw();
		else
			m_pAction[m_curActionIndex]->keepDraw();
		return;
	}
}
void XActionEx::input(const XInputEvent &inputEvent)
{
	if (!m_isInited ||
		m_actionState != ACTION_STATE_ACTION) return;	//这个条件有待磋商
	switch (m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funInput(inputEvent);
		return;
	case ACTION_TYPE_COMPLEX://可以遍历所有组件进行显示
		m_pAction[m_curActionIndex]->input(inputEvent);
		return;
	}
}
XBool XActionEx::init(XResPos resPos,
	const char *actionName)
{
	if (m_isInited) return XFalse;
	m_resoursePosition = resPos;

	if (!funInit(resPos)) return XFalse;

	m_actionState = ACTION_STATE_END;
	if (actionName == NULL) m_actionName = " ";
	else m_actionName = actionName;

	m_type = ACTION_TYPE_ATOMIC;
	m_isInited = XTrue;
	return XTrue;
}
XBool XActionEx::init(XResPos resPos, const char *actionName, int actionSum, ...)	//复合动作的初始化
{
	if (m_isInited) return XTrue;
	m_resoursePosition = resPos;

	if (!funInit(resPos)) return XFalse;

	XMem::XDELETE(m_pAction);
	m_actionSum = actionSum;
	if (m_actionSum <= 0) return XFalse;
	m_pAction = XMem::createArrayMem<XActionEx *>(m_actionSum);
	if (m_pAction == NULL) return XFalse;
	//下面解析所有的动作
	va_list arg_ptr;
	va_start(arg_ptr, actionSum);
	for (int i = 0; i < m_actionSum; ++i)
	{
		m_pAction[i] = XAExManager.getPAction(va_arg(arg_ptr, const char *));
	}
	va_end(arg_ptr);

	m_actionState = ACTION_STATE_END;
	if (actionName == NULL) m_actionName = " ";
	else m_actionName = actionName;

	m_type = ACTION_TYPE_COMPLEX;
	m_isInited = XTrue;
	return XTrue;
}
//下面是关于新的动作管理的类的接口
XActionExHandle XActionExManager::pushAction(XActionEx *action)	//向表中推入一个动作，返回动作的句柄，-1表示动作已经存在
{
	if (action == NULL) return 0;
	int ret = getActionHandle(action);
	if (ret != -1) return ret;	//防止重复注册
	ret = getActionHandle(action->getName().c_str());//这里要防止重名
	//填补原有的空缺
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (!m_pActions[i].actionIsEnable)
		{
			m_pActions[i].actionHandle = i;
			m_pActions[i].actionIsEnable = XTrue;
			m_pActions[i].pAction = action;
			return m_pActions[i].actionHandle;
		}
	}
	//如果没有空缺则这里加入新的
	XActionExData tmp;
	tmp.actionHandle = m_pActions.size();
	m_actionOrder.push_back(m_pActions.size());
	m_actionOrderX.push_back(m_pActions.size());
	tmp.actionIsEnable = XTrue;
	tmp.pAction = action;
	m_pActions.push_back(tmp);
	return tmp.actionHandle;
}
XActionExHandle XActionExManager::getActionHandle(const char *actionName)
{
	if (actionName == NULL) return -1;
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (m_pActions[i].actionIsEnable && m_pActions[i].pAction != NULL
			&& m_pActions[i].pAction->isName(actionName))
			return m_pActions[i].actionHandle;
	}
	return -1;	//没有找到这个动作
}
XActionExHandle XActionExManager::getActionHandle(const XActionEx *action)	//通过名字获取动作句柄
{
	if (action == NULL) return -1;
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (m_pActions[i].actionIsEnable && m_pActions[i].pAction == action)
			return m_pActions[i].actionHandle;
	}
	return -1;	//没有找到这个动作
}
XBool XActionExManager::popAction(const char *actionName)
{
	if (actionName == NULL) return XFalse;
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (m_pActions[i].actionIsEnable && m_pActions[i].pAction != NULL
			&& m_pActions[i].pAction->isName(actionName))
		{
			m_pActions[i].actionHandle = 0;
			m_pActions[i].actionIsEnable = XFalse;
			m_pActions[i].pAction = NULL;
			return XTrue;
		}
	}
	return XFalse;	//没有找到这个动作
}
XBool XActionExManager::popAction(XActionEx *action)		//从表中删除一个动作
{
	if (action == NULL) return XFalse;
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (m_pActions[i].actionIsEnable && m_pActions[i].pAction == action)
		{
			m_pActions[i].actionHandle = 0;
			m_pActions[i].actionIsEnable = XFalse;
			m_pActions[i].pAction = NULL;
			return XTrue;
		}
	}
	return XFalse;	//没有找到这个动作
}
XBool XActionExManager::releaseAction(const char *actionName)	//从表中删除一个动作并释放掉相应的资源
{
	if (actionName == NULL) return XFalse;
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (m_pActions[i].actionIsEnable && m_pActions[i].pAction != NULL
			&& m_pActions[i].pAction->isName(actionName))
		{
			m_pActions[i].actionHandle = 0;
			m_pActions[i].actionIsEnable = XFalse;
			m_pActions[i].pAction->release();
			m_pActions[i].pAction = NULL;
			return XTrue;
		}
	}
	return XFalse;	//没有找到这个动作
}
XBool XActionExManager::releaseAction(XActionEx *action)
{
	if (action == NULL) return XFalse;
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (m_pActions[i].actionIsEnable && m_pActions[i].pAction == action)
		{
			m_pActions[i].actionHandle = 0;
			m_pActions[i].actionIsEnable = XFalse;
			m_pActions[i].pAction->release();
			m_pActions[i].pAction = NULL;
			return XTrue;
		}
	}
	return XFalse;	//没有找到这个动作
}
//下面的函数使用两次映射，效率不高，要考虑优化
XBool XActionExManager::moveToBottom(XActionExHandle actionHandle)
{
	if (actionHandle < 0 || actionHandle >= m_pActions.size()) return XFalse;
	for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
	{
		if (m_actionOrder[i] < m_actionOrder[actionHandle])
		{
			++m_actionOrder[i];
			m_actionOrderX[m_actionOrder[i]] = i;
		}
	}
	m_actionOrder[actionHandle] = 0;
	m_actionOrderX[0] = actionHandle;
	return XTrue;
}
XBool XActionExManager::moveToTop(XActionExHandle actionHandle)
{
	if (actionHandle < 0 || actionHandle >= m_pActions.size()) return XFalse;
	for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
	{
		if (m_actionOrder[i] > m_actionOrder[actionHandle])
		{
			--m_actionOrder[i];
			m_actionOrderX[m_actionOrder[i]] = i;
		}
	}
	m_actionOrder[actionHandle] = (int)(m_actionOrder.size()) - 1;
	m_actionOrderX[(int)(m_actionOrder.size()) - 1] = actionHandle;

	return XTrue;
}
XBool XActionExManager::moveUp(XActionExHandle actionHandle)
{
	if (actionHandle < 0 || actionHandle >= m_pActions.size()) return XFalse;
	for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
	{
		if (m_actionOrder[i] == m_actionOrder[actionHandle] + 1)
		{
			m_actionOrder[i] = m_actionOrder[actionHandle];
			++m_actionOrder[actionHandle];

			m_actionOrderX[m_actionOrder[i]] = i;
			m_actionOrderX[m_actionOrder[actionHandle]] = actionHandle;
			return XTrue;
		}
	}
	return XFalse;
}
XBool XActionExManager::moveDown(XActionExHandle actionHandle)
{
	if (actionHandle < 0 || actionHandle >= m_pActions.size()) return XFalse;
	for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
	{
		if (m_actionOrder[i] == m_actionOrder[actionHandle] - 1)
		{
			m_actionOrder[i] = m_actionOrder[actionHandle];
			--m_actionOrder[actionHandle];

			m_actionOrderX[m_actionOrder[i]] = i;
			m_actionOrderX[m_actionOrder[actionHandle]] = actionHandle;
			return XTrue;
		}
	}
	return XFalse;
}
XBool XActionExManager::moveUpTo(XActionExHandle s, XActionExHandle d)
{
	if (s < 0 || s >= m_pActions.size() ||
		d < 0 || d >= m_pActions.size() || s == d) return XFalse;
	if (m_actionOrder[s] < m_actionOrder[d])
	{
		for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
		{
			if (m_actionOrder[i] > m_actionOrder[s] && m_actionOrder[i] <= m_actionOrder[d])
			{
				--m_actionOrder[i];
				m_actionOrderX[m_actionOrder[i]] = i;
			}
		}
		m_actionOrder[s] = m_actionOrder[d] + 1;
		m_actionOrderX[m_actionOrder[s]] = s;
	}
	else
	{
		for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
		{
			if (m_actionOrder[i] > m_actionOrder[d] && m_actionOrder[i] < m_actionOrder[s])
			{
				++m_actionOrder[i];
				m_actionOrderX[m_actionOrder[i]] = i;
			}
		}
		m_actionOrder[s] = m_actionOrder[d] + 1;
		m_actionOrderX[m_actionOrder[s]] = s;
	}
	return XFalse;
}
XBool XActionExManager::moveDownTo(XActionExHandle s, XActionExHandle d)
{
	if (s < 0 || s >= m_pActions.size() ||
		d < 0 || d >= m_pActions.size() || s == d) return XFalse;
	if (m_actionOrder[s] < m_actionOrder[d])
	{
		for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
		{
			if (m_actionOrder[i] > m_actionOrder[s] && m_actionOrder[i] < m_actionOrder[d])
			{
				--m_actionOrder[i];
				m_actionOrderX[m_actionOrder[i]] = i;
			}
		}
		m_actionOrder[s] = m_actionOrder[d] - 1;
		m_actionOrderX[m_actionOrder[s]] = s;
	}
	else
	{
		for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
		{
			if (m_actionOrder[i] >= m_actionOrder[d] && m_actionOrder[i] < m_actionOrder[s])
			{
				++m_actionOrder[i];
				m_actionOrderX[m_actionOrder[i]] = i;
			}
		}
		m_actionOrder[s] = m_actionOrder[d] - 1;
		m_actionOrderX[m_actionOrder[s]] = s;
	}
	return XFalse;
}
#if !WITH_INLINE_FILE
#include "XActionEx.inl"
#endif
}