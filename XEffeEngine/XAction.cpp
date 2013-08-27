#include "XAction.h"

void _XAction::move(int timeStep)
{
	if(m_actionState != _XA_STATE_ACTION) return;
	//TODO: add the action function or object

	if(m_actionIsLoop && m_actionState == _XA_STATE_END)
	{//loop to restart
		//TODO: do something at the action end
		setStart();
	}
}

void _XActionEx::move()
{
	if(m_isInited == 0) return;
	if(m_actionState != _XA_STATE_ACTION) return;
	if(m_funMove != NULL) (* m_funMove)(m_pClass);
	
	if((* m_funIsEnd)(m_pClass)) 
	{
		m_actionState = _XA_STATE_END;	//标记动作结束
		if(m_funAtEnd != NULL) (* m_funAtEnd)(m_pClass);
	}
	if(m_actionIsLoop && m_actionState == _XA_STATE_END)
	{
		setStart();
	}
}

void _XActionEx::draw()
{
	if(m_isInited == 0) return;
	if(m_actionState != _XA_STATE_ACTION) return;	//这个条件有待磋商
	if(m_funDraw != NULL) (* m_funDraw)(m_pClass);
}

int _XActionEx::init(bool (*funIsEnd)(void *),
	void (*funMove)(void *),		
	void (*funDraw)(void *),		
	void (*funResetData)(void *),
	void (*funAtEnd)(void *),	
	void * pClass)
{
	if(m_isInited != 0) return 1;
	if(funIsEnd == NULL) return 0;
	m_funIsEnd = funIsEnd;
	m_funMove = funMove;
	m_funDraw = funDraw;
	m_funResetData = funResetData;
	m_funAtEnd = funAtEnd;
	m_pClass = pClass;

	m_actionState = _XA_STATE_END;

	m_isInited = 1;
	return 1;
}