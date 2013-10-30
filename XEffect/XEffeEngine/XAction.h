#ifndef _JIA_XACTION_
#define _JIA_XACTION_

#include "stdlib.h"

enum _XACTION_STATE
{
	_XA_STATE_NULL,		//动作状态未定义
	_XA_STATE_ACTION,	//动作进行中
	_XA_STATE_PAUSE,	//动作暂停
	_XA_STATE_END,		//动作结束
};

//这是一个关于动作类的设计
class _XAction
{
private:
protected:
	_XACTION_STATE m_actionState;	//动作的状态
	bool m_actionIsLoop;			//动作是否循环
public:
	virtual void setLoop(bool loop)	//设置动作是否循环
	{
		m_actionIsLoop = loop;	
	}
	virtual bool getIsLoop()		//获取动作是否循环
	{
		return m_actionIsLoop;
	}
	virtual void setStart()			//设置动作开始进行
	{
		if(m_actionState == _XA_STATE_END)
		{
			m_actionState = _XA_STATE_ACTION;
			resetData();
		}
	}
	virtual void setPause()			//设置动作暂停
	{
		if(m_actionState == _XA_STATE_ACTION)
			m_actionState = _XA_STATE_PAUSE;
	}
	virtual void setResume()		//设置动作恢复
	{
		if(m_actionState == _XA_STATE_PAUSE)
			m_actionState = _XA_STATE_ACTION;
	}
	virtual void move(int timeStep);
	virtual void resetData() = 0;		//初始化数据
	_XACTION_STATE getActionState()
	{
		return m_actionState;
	}
	bool getIsEnd()
	{
		if(m_actionState == _XA_STATE_END) return true;
		else return false;
	}

	_XAction()
		:m_actionState(_XA_STATE_NULL)
		,m_actionIsLoop(false)
	{
	}
};
//使用回调函数的形式显示动作
class _XActionEx
{
private:
	char m_isInited;	//是否初始化
	_XACTION_STATE m_actionState;	//动作的状态
	bool m_actionIsLoop;			//动作是否循环

	void (*m_funResetData)(void *);		//重新设置数据
	void (*m_funMove)(void *);		//步进
	void (*m_funDraw)(void *);		//描绘
	void (*m_funAtEnd)(void *);		//动作结束之后调用
	bool (*m_funIsEnd)(void *);		//判断是否动作结束

	void * m_pClass;
public:
	virtual void setLoop(bool loop)	//设置动作是否循环
	{
		m_actionIsLoop = loop;	
	}
	virtual bool getIsLoop()		//获取动作是否循环
	{
		return m_actionIsLoop;
	}
	virtual void setStart()			//设置动作开始进行
	{
		if(m_actionState == _XA_STATE_END)
		{
			m_actionState = _XA_STATE_ACTION;
			if(m_funResetData != NULL) (*m_funResetData)(m_pClass);
		}
	}
	virtual void setPause()			//设置动作暂停
	{
		if(m_actionState == _XA_STATE_ACTION)
			m_actionState = _XA_STATE_PAUSE;
	}
	virtual void setResume()		//设置动作恢复
	{
		if(m_actionState == _XA_STATE_PAUSE)
			m_actionState = _XA_STATE_ACTION;
	}
	virtual void move();
	_XACTION_STATE getActionState()
	{
		return m_actionState;
	}
	bool getIsEnd()
	{
		if(m_actionState == _XA_STATE_END) return true;
		else return false;
	}

	_XActionEx()
		:m_isInited(0)
		,m_actionState(_XA_STATE_NULL)
		,m_actionIsLoop(false)
		,m_funResetData(NULL)		//重新设置数据
		,m_funMove(NULL)		//步进
		,m_funDraw(NULL)		//描绘
		,m_funAtEnd(NULL)		//动作结束之后调用
		,m_pClass(NULL)
	{
	}
	int init(bool (*funIsEnd)(void *),
		void (*funMove)(void *),		
		void (*funDraw)(void *),		
		void (*funResetData)(void *),
		void (*funAtEnd)(void *),	
		void * pClass);	
	void draw();
};
#endif