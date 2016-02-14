#include "XStdHead.h"
#include "XActionSimple.h"
//++++++++++++++++++++++++++++++++
//Author:	  贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.1
//--------------------------------
#include "XResourcePack.h"
#include <stdarg.h>
namespace XE{
void XActionSimpleEx::setStart()			//设置动作开始进行
{
	if(m_actionState != ACTION_STATE_END) return;
	m_actionState = ACTION_STATE_ACTION;
	switch(m_type)
	{
	case ACTION_TYPE_ATOMIC:
		if(m_funResetData != NULL) m_funResetData(m_pClass);	
		return;
	case ACTION_TYPE_COMPLEX://开始播放第一个动作
		if(m_pAction[0] != NULL) m_pAction[0]->setStart();
		m_curActionIndex = 0;
		return;
	}
}
void XActionSimpleEx::move(float stepTime)
{
	if(!m_isInited || 
		m_actionState != ACTION_STATE_ACTION) return;
	switch(m_type)
	{
	case ACTION_TYPE_ATOMIC:
		if(m_funMove != NULL) m_funMove(stepTime,m_pClass);	
		if(m_funIsEnd(m_pClass)) 
		{
			m_actionState = ACTION_STATE_END;	//标记动作结束
			if(m_funAtEnd != NULL) m_funAtEnd(m_pClass);
		}
		if(m_actionIsLoop && 
			m_actionState == ACTION_STATE_END) setStart();
		return;
	case ACTION_TYPE_COMPLEX:
		if(!m_pAction[m_curActionIndex]->getIsEnd()) return;
		++ m_curActionIndex;
		if(m_curActionIndex >= m_actionSum)
		{//结束
			m_actionState = ACTION_STATE_END;
			if(m_actionIsLoop && 
				m_actionState == ACTION_STATE_END) setStart();
		}else
		{//播放下一个动作
			m_pAction[m_curActionIndex]->setStart();
		}
		return;
	}
}
void XActionSimpleEx::draw()
{
	if(!m_isInited) return;
	if(m_actionState != ACTION_STATE_ACTION) return;	//这个条件有待磋商
	switch(m_type)
	{
	case ACTION_TYPE_ATOMIC:
		if(m_funDraw != NULL) m_funDraw(m_pClass);
		return;
	case ACTION_TYPE_COMPLEX://可以遍历所有组件进行显示
		m_pAction[m_curActionIndex]->draw();
		return;
	}
}
XBool XActionSimpleEx::init(XResourcePosition resoursePosition,
	const char *actionName,
	XBool (*funIsEnd)(void *),
	void (*funMove)(float,void *),		
	void (*funDraw)(void *),		
	void (*funResetData)(void *),
	void (*funAtEnd)(void *),
	int (*funInit)(int,void *),
	void (*funRelease)(void *),
	void * pClass)
{
	if(m_isInited) return XTrue;
	if(funIsEnd == NULL) return XFalse;
	m_funIsEnd = funIsEnd;
	m_funMove = funMove;
	m_funDraw = funDraw;
	m_funResetData = funResetData;
	m_funAtEnd = funAtEnd;
	m_funInit = funInit;
	m_funRelease = funRelease;
	m_pClass = pClass;

	m_resoursePosition = resoursePosition;

	if(m_funInit != NULL) if(m_funInit(resoursePosition,m_pClass) == 0) return XFalse;

	m_actionState = ACTION_STATE_END;
	if(actionName == NULL) m_actionName = "";
	else m_actionName = actionName;

	m_type = ACTION_TYPE_ATOMIC;
	m_isInited = XTrue;
	return XTrue;
}
XBool XActionSimpleEx::initComplex(XResourcePosition resoursePosition,const char *actionName,int actionSum,...)	//复合动作的初始化
{
	if(m_isInited) return XTrue;
	m_funIsEnd = NULL;
	m_funMove = NULL;
	m_funDraw = NULL;
	m_funResetData = NULL;
	m_funAtEnd = NULL;
	m_pClass = NULL;
	m_resoursePosition = resoursePosition;

	if(m_funInit != NULL) if(m_funInit(resoursePosition,m_pClass) == 0) return XFalse;

	XMem::XDELETE(m_pAction);
	m_actionSum = actionSum;
	if(m_actionSum <= 0) return XFalse;
	m_pAction = XMem::createArrayMem<XActionSimpleEx *>(m_actionSum);
	if(m_pAction == NULL) return XFalse;
	//下面解析所有的动作
	va_list arg_ptr;
	va_start(arg_ptr,actionSum);
	for(int i = 0;i < m_actionSum;++ i)
	{
		//m_pAction[i] = XASManager.getPAction(va_arg(arg_ptr,const char *));
		m_pAction[i] = va_arg(arg_ptr,XActionSimpleEx *);
	}
	va_end(arg_ptr);

	m_actionState = ACTION_STATE_END;
	if(actionName == NULL) m_actionName = "";
	else m_actionName = actionName;

	m_type = ACTION_TYPE_COMPLEX;
	m_isInited = XTrue;
	return XTrue;
}
//下面是关于新的动作管理的类的接口
bool XActionSimpleManager::pushAction(XActionSimpleEx *action)	//向表中推入一个动作，返回动作的句柄，-1表示动作已经存在
{
	if(action == NULL) return false;
	int ret = getActionIndex(action);
	if(ret != -1) return false;	//防止重复注册
	m_pActions.push_back(action);

	return true;
}
int XActionSimpleManager::getActionIndex(const char *actionName)
{
	if(actionName == NULL) return -1;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i] != NULL &&
			m_pActions[i]->isName(actionName)) 
			return i;
	}
	return -1;	//没有找到这个动作
}
int XActionSimpleManager::getActionIndex(const XActionSimpleEx *action)	//通过名字获取动作句柄
{
	if(action == NULL) return -1;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i] == action) 
			return i;
	}
	return -1;	//没有找到这个动作
}
XBool XActionSimpleManager::popAction(const char *actionName)
{
	if(actionName == NULL) return XFalse;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i] != NULL
			&& m_pActions[i]->isName(actionName)) 
		{
			m_pActions.erase(m_pActions.begin() + i);
			return XTrue;
		}
	}
	return XFalse;	//没有找到这个动作
}
XBool XActionSimpleManager::popAction(XActionSimpleEx *action)		//从表中删除一个动作
{
	if(action == NULL) return XFalse;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i] == action) 
		{
			m_pActions.erase(m_pActions.begin() + i);
			return XTrue;
		}
	}
	return XFalse;	//没有找到这个动作
}
XBool XActionSimpleManager::releaseAction(const char *actionName)	//从表中删除一个动作并释放掉相应的资源
{
	if(actionName == NULL) return XFalse;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i] != NULL
			&& m_pActions[i]->isName(actionName)) 
		{
			m_pActions.erase(m_pActions.begin() + i);
			return XTrue;
		}
	}
	return XFalse;	//没有找到这个动作
}
XBool XActionSimpleManager::releaseAction(XActionSimpleEx *action)
{
	if(action == NULL) return XFalse;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i] == action) 
		{
			m_pActions.erase(m_pActions.begin() + i);
			return XTrue;
		}
	}
	return XFalse;	//没有找到这个动作
}
#if !WITH_INLINE_FILE
#include "XActionSimple.inl"
#endif
}