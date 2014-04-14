#ifndef _JIA_XACTION_
#define _JIA_XACTION_
//++++++++++++++++++++++++++++++++
//Author:	  贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.1
//--------------------------------
//Environment="PATH=%PATH%;..\dll\libfreetype;"	//需要设置dll路径
#include "XBasicFun.h"
#include "stdlib.h"
#include "string.h"

enum _XActionState
{
	ACTION_STATE_NULL,		//动作状态未定义
	ACTION_STATE_ACTION,	//动作进行中
	ACTION_STATE_PAUSE,	//动作暂停
	ACTION_STATE_END,		//动作结束
};

//使用回调函数的形式显示动作
#define MAX_ACTION_NAME_LENGTH (256)
enum _XActionType
{
	ACTION_TYPE_COMPLEX,	//串行复合的动作,由很多原子动作串起来过程的动作
	ACTION_TYPE_ATOMIC,		//原子的不可(必)细分的动作
};
class _XActionEx
{
private:
	_XResourcePosition m_resoursePosition;	//资源所在的位置
	char m_isInited;	//是否初始化
	_XActionType m_type;
	_XActionState m_actionState;	//动作的状态
	bool m_actionIsLoop;			//动作是否循环
	char m_actionName[MAX_ACTION_NAME_LENGTH];				//动作的名称，用于帮助记忆

	void (*m_funResetData)(void *);			//重新设置数据
	void (*m_funMove)(int stepTime,void *);	//步进
	void (*m_funDraw)(void *);				//描绘
	void (*m_funAtEnd)(void *);				//动作结束之后调用
	bool (*m_funIsEnd)(void *);				//判断是否动作结束
	//进行资源管理
	int (*m_funInit)(int,void*);	//资源初始化
	void (*m_funRelease)(void*);	//资源释放函数

	void * m_pClass;
public:
	virtual bool isName(const char *name)
	{
		if(name == NULL || m_actionName == NULL) return false;
		if(strcmp(name,m_actionName) == 0) return true;
		return false;
	}
	void setStop();	//终止动作，尚未实现
	virtual void setLoop(bool isLoop) {m_actionIsLoop = isLoop;}	//设置动作是否循环
	virtual bool getIsLoop() {return m_actionIsLoop;}		//获取动作是否循环
	virtual void setStart();			//设置动作开始进行
	virtual void setPause()			//设置动作暂停
	{
		if(m_actionState == ACTION_STATE_ACTION)
			m_actionState = ACTION_STATE_PAUSE;
	}
	virtual void setResume()		//设置动作恢复
	{
		if(m_actionState == ACTION_STATE_PAUSE)
			m_actionState = ACTION_STATE_ACTION;
	}
	virtual void move(int stepTime = 1);
	_XActionState getActionState()
	{
		return m_actionState;
	}
	bool getIsEnd()
	{
		if(m_actionState == ACTION_STATE_END) return true;
		else return false;
	}

	_XActionEx()
		:m_isInited(0)
		,m_actionState(ACTION_STATE_NULL)
		,m_actionIsLoop(false)
		,m_funResetData(NULL)		//重新设置数据
		,m_funMove(NULL)		//步进
		,m_funDraw(NULL)		//描绘
		,m_funAtEnd(NULL)		//动作结束之后调用
		,m_funInit(NULL)		//初始化资源加载
		,m_funRelease(NULL)		//资源释放
		,m_pClass(NULL)
		,m_actionSum(0)
		,m_pAction(NULL)
		,m_type(ACTION_TYPE_ATOMIC)
	{
		m_actionName[0] = '\0';
	}
	int init(_XResourcePosition resoursePosition,
		const char *actionName,
		bool (*funIsEnd)(void *),
		void (*funMove)(int,void *),		
		void (*funDraw)(void *),		
		void (*funResetData)(void *),
		void (*funAtEnd)(void *) = NULL,	
		int (*funInit)(int,void *) = NULL,
		void (*funRelease)(void *) = NULL,
		void * pClass = NULL);	
	void draw();

	~_XActionEx()
	{
		release();
	}
	void release()
	{
		if(m_isInited == 0) return;
		if(m_funRelease != NULL) (*m_funRelease)(m_pClass);
		if(m_type == ACTION_TYPE_COMPLEX) XDELETE_ARRAY(m_pAction);
		m_isInited = 0;
	}
//下面是复合动作的定义
private:
	_XActionEx **m_pAction;	//子动作
	int m_actionSum;		//子动作的数据量
	int m_nowActionIndex;
public:
	int init(_XResourcePosition resoursePosition,const char *actionName,int actionSum,...);	//复合动作的初始化
};

//动作管理的类
typedef int _XActionHandle;	//动作的句柄
struct _XActionData
{
	_XActionHandle actionHandle;	//动作的编号
	char actionIsEnable;	//动作是否有效
	_XActionEx *pAction;	//动作的指针

	_XActionData()
		:actionIsEnable(0)
		,pAction(NULL)
	{
	}
};
#define MAX_ACTION_SUM (512)	//最多允许同时存在的动作数量
class _XActionManager
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//下面需要将其设计为Singleton模式
protected:
	_XActionManager()
		:m_nowActionHandle(0)
		,m_actionSum(0)
	{
		m_pActions = createArrayMem<_XActionData>(MAX_ACTION_SUM);
	}
	_XActionManager(const _XActionManager&);
	_XActionManager &operator= (const _XActionManager&);
	virtual ~_XActionManager(){;} 
public:
	static _XActionManager& GetInstance()
	{
		static _XActionManager m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	int m_nowActionHandle;		//当前的动作句柄
	_XActionData *m_pActions;	//所有动作的表
	int m_actionSum;	//当前拥有的动作的数量
public:
	_XActionHandle pushAction(_XActionEx *action);	//向表中推入一个动作，返回动作的句柄，-1表示动作已经存在

	_XActionHandle getActionHandle(const char *actionName);	//通过名字获取动作句柄
	_XActionHandle getActionHandle(const _XActionEx *action);	//通过名字获取动作句柄
	int popAction(const char *actionName);				//从表中删除一个动作
	int popAction(_XActionEx *action);
	int popAction(_XActionHandle actionHandle);	
	int releaseAction(const char *actionName);	//从表中删除一个动作并释放掉相应的资源
	int releaseAction(_XActionEx *action);
	int releaseAction(_XActionHandle actionHandle);
	int setStartAction(const char *actionName);			//设置开始播放一个动作
	int setStartAction(_XActionHandle actionHandle);	//设置开始播放一个动作
	_XActionEx *getPAction(const char *actionName);
	_XActionEx *getPAction(_XActionHandle actionHandle);

	bool getIsActionEnd(const char *actionName);		//获取这个动作是否播放完成
	bool getIsActionEnd(_XActionHandle actionHandle);	//获取这个动作是否播放完成

	void move(int stepTime)		//遍历并推进所有的动作
	{
		for(int i = 0;i < MAX_ACTION_SUM;++ i)
		{
			if(m_pActions[i].actionIsEnable != 0) m_pActions[i].pAction->move(stepTime);	//这个不需要顺序
		}
	}
	void draw()					//遍历并描绘所有的动作
	{
		for(int i = 0;i < MAX_ACTION_SUM;++ i)
		{
			if(m_pActions[i].actionIsEnable != 0) m_pActions[i].pAction->draw();	//draw的顺序问题，这里估计需要进行一个映射
		}
	}
	//下面的接口尚未实现
	void setAllActionStop();	//停止所有的动作
	void setAllActionPause();	//暂停所有的动作
	void setAllActionResume();	//恢复所有的动作
	void popAllAction();		//弹出所有的动作
};
#endif