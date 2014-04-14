#include "XAction.h"
//++++++++++++++++++++++++++++++++
//Author:	  贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.1
//--------------------------------
#include "XResourcePack.h"
void _XActionEx::setStart()			//设置动作开始进行
{
	if(m_actionState == ACTION_STATE_END)
	{
		m_actionState = ACTION_STATE_ACTION;
		if(m_type == ACTION_TYPE_ATOMIC)
		{
			if(m_funResetData != NULL) (*m_funResetData)(m_pClass);	
		}else
		if(m_type == ACTION_TYPE_COMPLEX)
		{//开始播放第一个动作
			m_pAction[0]->setStart();
			m_nowActionIndex = 0;
		}
	}
}
void _XActionEx::move(int stepTime)
{
	if(m_isInited == 0) return;
	if(m_actionState != ACTION_STATE_ACTION) return;
	if(m_type == ACTION_TYPE_ATOMIC)
	{
		if(m_funMove != NULL) (* m_funMove)(stepTime,m_pClass);	
		if((* m_funIsEnd)(m_pClass)) 
		{
			m_actionState = ACTION_STATE_END;	//标记动作结束
			if(m_funAtEnd != NULL) (* m_funAtEnd)(m_pClass);
		}
		if(m_actionIsLoop && m_actionState == ACTION_STATE_END)
		{
			setStart();
		}
	}else
	if(m_type == ACTION_TYPE_COMPLEX)
	{
		if(m_pAction[m_nowActionIndex]->getIsEnd())
		{
			++ m_nowActionIndex;
			if(m_nowActionIndex >= m_actionSum)
			{//结束
				m_actionState = ACTION_STATE_END;
				if(m_actionIsLoop && m_actionState == ACTION_STATE_END)
				{
					setStart();
				}
			}else
			{//播放下一个动作
				m_pAction[m_nowActionIndex]->setStart();
			}
		}
	}
}

void _XActionEx::draw()
{
	if(m_isInited == 0) return;
	if(m_actionState != ACTION_STATE_ACTION) return;	//这个条件有待磋商
	if(m_type == ACTION_TYPE_ATOMIC)
	{
		if(m_funDraw != NULL) (* m_funDraw)(m_pClass);
	}else
	if(m_type == ACTION_TYPE_COMPLEX)
	{//可以遍历所有组件进行显示
		m_pAction[m_nowActionIndex]->draw();
	}
}

int _XActionEx::init(_XResourcePosition resoursePosition,
	const char *actionName,
	bool (*funIsEnd)(void *),
	void (*funMove)(int,void *),		
	void (*funDraw)(void *),		
	void (*funResetData)(void *),
	void (*funAtEnd)(void *),
	int (*funInit)(int,void *),
	void (*funRelease)(void *),
	void * pClass)
{
	if(m_isInited != 0) return 1;
	if(funIsEnd == NULL) return 0;
	m_funIsEnd = funIsEnd;
	m_funMove = funMove;
	m_funDraw = funDraw;
	m_funResetData = funResetData;
	m_funAtEnd = funAtEnd;
	m_funInit = funInit;
	m_funRelease = funRelease;
	m_pClass = pClass;

	m_resoursePosition = resoursePosition;

	if(m_funInit != NULL) if((*m_funInit)(resoursePosition,m_pClass) == 0) return 0;

	m_actionState = ACTION_STATE_END;
	if(actionName == NULL)
	{
		m_actionName[0] = '\0';
	}else
	{
		strcpy(m_actionName,actionName);
	}

	m_type = ACTION_TYPE_ATOMIC;
	m_isInited = 1;
	return 1;
}

#include <stdarg.h>

int _XActionEx::init(_XResourcePosition resoursePosition,const char *actionName,int actionSum,...)	//复合动作的初始化
{
	if(m_isInited != 0) return 1;
	m_funIsEnd = NULL;
	m_funMove = NULL;
	m_funDraw = NULL;
	m_funResetData = NULL;
	m_funAtEnd = NULL;
	m_pClass = NULL;
	m_resoursePosition = resoursePosition;

	if(m_funInit != NULL) if((*m_funInit)(resoursePosition,m_pClass) == 0) return 0;

	XDELETE(m_pAction);
	m_actionSum = actionSum;
	if(m_actionSum <= 0) return 0;
	m_pAction = createArrayMem<_XActionEx *>(m_actionSum);
	if(m_pAction == NULL) return 0;
	//下面解析所有的动作
	va_list arg_ptr;
	va_start(arg_ptr,actionSum);
	for(int i = 0;i < m_actionSum;++ i)
	{
		m_pAction[i] = _XActionManager::GetInstance().getPAction(va_arg(arg_ptr,const char *));
	}
	va_end(arg_ptr);

	m_actionState = ACTION_STATE_END;
	if(actionName == NULL)
	{
		m_actionName[0] = '\0';
	}else
	{
		strcpy(m_actionName,actionName);
	}
	m_type = ACTION_TYPE_COMPLEX;
	m_isInited = 1;
	return 1;
}

//下面是关于新的动作管理的类的接口
_XActionHandle _XActionManager::pushAction(_XActionEx *action)	//向表中推入一个动作，返回动作的句柄，-1表示动作已经存在
{
	if(action == NULL) return 0;
	int ret = getActionHandle(action);
	if(ret != -1) return ret;	//防止重复注册
	for(int i = 0;i < MAX_ACTION_SUM;++ i)
	{
		if(m_pActions[i].actionIsEnable == 0)
		{
			m_pActions[i].actionHandle = i;
			m_pActions[i].actionIsEnable = 1;
			m_pActions[i].pAction = action;
			++ m_actionSum;
			return m_pActions[i].actionHandle;
		}
	}
	return -1;
}
_XActionHandle _XActionManager::getActionHandle(const char *actionName)
{
	if(actionName == NULL) return -1;
	for(int i = 0;i < MAX_ACTION_SUM;++ i)
	{
		if(m_pActions[i].actionIsEnable != 0 && m_pActions[i].pAction != NULL) 
		{
			if(m_pActions[i].pAction->isName(actionName)) return m_pActions[i].actionHandle;
		}
	}
	return -1;	//没有找到这个动作
}
_XActionHandle _XActionManager::getActionHandle(const _XActionEx *action)	//通过名字获取动作句柄
{
	if(action == NULL) return -1;
	for(int i = 0;i < MAX_ACTION_SUM;++ i)
	{
		if(m_pActions[i].actionIsEnable != 0 && m_pActions[i].pAction == action) return m_pActions[i].actionHandle;
	}
	return -1;	//没有找到这个动作
}
int _XActionManager::popAction(const char *actionName)
{
	if(actionName == NULL) return 0;
	for(int i = 0;i < MAX_ACTION_SUM;++ i)
	{
		if(m_pActions[i].actionIsEnable != 0 && m_pActions[i].pAction != NULL
			&& m_pActions[i].pAction->isName(actionName)) 
		{
			m_pActions[i].actionHandle = 0;
			m_pActions[i].actionIsEnable = 0;
			m_pActions[i].pAction = NULL;
			-- m_actionSum;
			return 1;
		}
	}
	return 0;	//没有找到这个动作
}
int _XActionManager::popAction(_XActionEx *action)		//从表中删除一个动作
{
	if(action == NULL) return 0;
	if(m_actionSum <= 0) return 0;
	for(int i = 0;i < MAX_ACTION_SUM;++ i)
	{
		if(m_pActions[i].actionIsEnable != 0 && m_pActions[i].pAction == action) 
		{
			m_pActions[i].actionHandle = 0;
			m_pActions[i].actionIsEnable = 0;
			m_pActions[i].pAction = NULL;
			-- m_actionSum;
			return 1;
		}
	}
	return 0;	//没有找到这个动作
}
int _XActionManager::popAction(_XActionHandle actionHandle)
{
	if(actionHandle < 0 || actionHandle >= MAX_ACTION_SUM) return 0;
	if(m_pActions[actionHandle].actionIsEnable != 0)
	{
		m_pActions[actionHandle].actionHandle = 0;
		m_pActions[actionHandle].actionIsEnable = 0;
		m_pActions[actionHandle].pAction = NULL;
		-- m_actionSum;
		return 1;
	}
	return 0;
}
int _XActionManager::releaseAction(const char *actionName)	//从表中删除一个动作并释放掉相应的资源
{
	if(actionName == NULL) return 0;
	for(int i = 0;i < MAX_ACTION_SUM;++ i)
	{
		if(m_pActions[i].actionIsEnable != 0 && m_pActions[i].pAction != NULL
			&& m_pActions[i].pAction->isName(actionName)) 
		{
			m_pActions[i].actionHandle = 0;
			m_pActions[i].actionIsEnable = 0;
			m_pActions[i].pAction->release();
			m_pActions[i].pAction = NULL;
			-- m_actionSum;
			return 1;
		}
	}
	return 0;	//没有找到这个动作
}
int _XActionManager::releaseAction(_XActionEx *action)
{
	if(action == NULL) return 0;
	if(m_actionSum <= 0) return 0;
	for(int i = 0;i < MAX_ACTION_SUM;++ i)
	{
		if(m_pActions[i].actionIsEnable != 0 && m_pActions[i].pAction == action) 
		{
			m_pActions[i].actionHandle = 0;
			m_pActions[i].actionIsEnable = 0;
			m_pActions[i].pAction->release();
			m_pActions[i].pAction = NULL;
			-- m_actionSum;
			return 1;
		}
	}
	return 0;	//没有找到这个动作
}
int _XActionManager::releaseAction(_XActionHandle actionHandle)
{
	if(actionHandle < 0 || actionHandle >= MAX_ACTION_SUM) return 0;
	if(m_pActions[actionHandle].actionIsEnable != 0)
	{
		m_pActions[actionHandle].actionHandle = 0;
		m_pActions[actionHandle].actionIsEnable = 0;
		m_pActions[actionHandle].pAction->release();
		m_pActions[actionHandle].pAction = NULL;
		-- m_actionSum;
		return 1;
	}
	return 0;
}
int _XActionManager::setStartAction(const char *actionName)
{
	int ret = getActionHandle(actionName);
	if(ret < 0) return 0;
	m_pActions[ret].pAction->setStart();
	return 1;
}
int _XActionManager::setStartAction(_XActionHandle actionHandle)
{
	if(actionHandle >= 0 && actionHandle < MAX_ACTION_SUM
		&& m_pActions[actionHandle].actionIsEnable != 0)
	{
		m_pActions[actionHandle].pAction->setStart();
		return 1;
	}
	return 0;
}
_XActionEx *_XActionManager::getPAction(const char *actionName)
{
	int ret = getActionHandle(actionName);
	if(ret < 0) return NULL;
	return m_pActions[ret].pAction;
}
_XActionEx *_XActionManager::getPAction(_XActionHandle actionHandle)
{
	if(actionHandle >= 0 && actionHandle < MAX_ACTION_SUM
		&& m_pActions[actionHandle].actionIsEnable != 0) return m_pActions[actionHandle].pAction;
	return NULL;
}
bool _XActionManager::getIsActionEnd(const char *actionName)
{
	_XActionEx * temp = getPAction(actionName);
	if(temp == NULL) return true;
	return temp->getIsEnd();
}
bool _XActionManager::getIsActionEnd(_XActionHandle actionHandle)
{
	if(actionHandle >= 0 && actionHandle < MAX_ACTION_SUM
		&& m_pActions[actionHandle].actionIsEnable != 0) return m_pActions[actionHandle].pAction->getIsEnd();
	return true;
}