#ifndef _JIA_XACTION_
#define _JIA_XACTION_
//++++++++++++++++++++++++++++++++
//Author:	  贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.1
//--------------------------------
#include "XBasicFun.h"
#include "stdlib.h"
#include "string.h"
#include <vector>
#include "XCommonDefine.h"
#include "XMemory.h"
namespace XE{
//这里是通过回调函数的形式来实现动作管理，是使用C的语法规则不符合C++的设计规则

enum XActionSimpleState
{
	ACTION_STATE_NULL,		//动作状态未定义
	ACTION_STATE_ACTION,	//动作进行中
	ACTION_STATE_PAUSE,	//动作暂停
	ACTION_STATE_END,		//动作结束
};

//使用回调函数的形式显示动作
enum XActionSimpleType
{
	ACTION_TYPE_COMPLEX,	//串行复合的动作,由很多原子动作串起来过程的动作
	ACTION_TYPE_ATOMIC,		//原子的不可(必)细分的动作
};
class XActionSimpleEx
{
private:
	//static const int m_maxActionNameLength = 256;
	XResPos m_resoursePosition;	//资源所在的位置
	XBool m_isInited;	//是否初始化
	XActionSimpleType m_type;
	XActionSimpleState m_actionState;	//动作的状态
	XBool m_actionIsLoop;			//动作是否循环
	//char m_actionName[m_maxActionNameLength];				//动作的名称，用于帮助记忆
	std::string m_actionName;

	void (*m_funResetData)(void *);			//重新设置数据
	void (*m_funMove)(float stepTime,void *);	//步进
	void (*m_funDraw)(void *);				//描绘
	void (*m_funAtEnd)(void *);				//动作结束之后调用
	XBool (*m_funIsEnd)(void *);				//判断是否动作结束
	//进行资源管理
	int (*m_funInit)(int,void*);	//资源初始化
	void (*m_funRelease)(void*);	//资源释放函数

	void * m_pClass;
public:
	virtual XBool isName(const char *name);
	void setStop();	//终止动作，尚未实现
	virtual void setLoop(XBool isLoop) {m_actionIsLoop = isLoop;}	//设置动作是否循环
	virtual XBool getIsLoop() const {return m_actionIsLoop;}		//获取动作是否循环
	virtual void setStart();			//设置动作开始进行
	virtual void setPause();			//设置动作暂停
	virtual void setResume();			//设置动作恢复
	virtual void move(float stepTime = 1.0f);
	XActionSimpleState getActionState() const {return m_actionState;}
	XBool getIsEnd() const {return m_actionState == ACTION_STATE_END;}

	XActionSimpleEx()
		:m_isInited(XFalse)
		,m_actionState(ACTION_STATE_NULL)
		,m_actionIsLoop(XFalse)
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
		,m_actionName("")
	{}
	XBool init(XResPos resPos,
		const char *actionName,
		XBool (*funIsEnd)(void *),
		void (*funMove)(float,void *),		
		void (*funDraw)(void *),		
		void (*funResetData)(void *),
		void (*funAtEnd)(void *) = NULL,	
		int (*funInit)(int,void *) = NULL,
		void (*funRelease)(void *) = NULL,
		void * pClass = NULL);	
	void draw();

	~XActionSimpleEx(){release();}
	void release();
//下面是复合动作的定义
private:
	XActionSimpleEx **m_pAction;	//子动作
	int m_actionSum;		//子动作的数据量
	int m_curActionIndex;
public:
	//后面传入的参数是复合的子动作的指针
	XBool initComplex(XResPos resPos,const char *actionName,int actionSum,...);	//复合动作的初始化
};

//动作管理的类
class XActionSimpleManager		//这个类并不负责资源释放，需要自己在外部释放资源
{
public:
	XActionSimpleManager()
		:m_curActionHandle(0)
	{}
	virtual ~XActionSimpleManager() {m_pActions.clear();} 
protected:
	XActionSimpleManager(const XActionSimpleManager&);
	XActionSimpleManager &operator= (const XActionSimpleManager&);
private:
	int m_curActionHandle;		//当前的动作句柄
	std::vector<XActionSimpleEx*> m_pActions;
	int getActionIndex(const char *actionName);	//通过名字获取动作句柄
	int getActionIndex(const XActionSimpleEx *action);	//通过名字获取动作句柄
public:
	bool pushAction(XActionSimpleEx *action);	//向表中推入一个动作，返回动作的句柄，-1表示动作已经存在

	XBool popAction(const char *actionName);				//从表中删除一个动作
	XBool popAction(XActionSimpleEx *action);
	XBool releaseAction(const char *actionName);	//从表中删除一个动作并释放掉相应的资源
	XBool releaseAction(XActionSimpleEx *action);
	XBool setStartAction(const char *actionName);			//设置开始播放一个动作
	XActionSimpleEx *getPAction(const char *actionName);

	XBool getIsActionEnd(const char *actionName);		//获取这个动作是否播放完成

	void move(float stepTime);		//遍历并推进所有的动作
	void draw();					//遍历并描绘所有的动作

	//下面的接口尚未实现
	void setAllActionStop();	//停止所有的动作
	void setAllActionPause();	//暂停所有的动作
	void setAllActionResume();	//恢复所有的动作
	void popAllAction();		//弹出所有的动作
};
#if WITH_INLINE_FILE
#include "XActionSimple.inl"
#endif
}
#endif