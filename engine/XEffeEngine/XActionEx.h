#ifndef _JIA_XACTIONEX_
#define _JIA_XACTIONEX_

#include "XActionSimple.h"
#include "XInputEventCore.h"
//++++++++++++++++++++++++++++++++
//Author:	  贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.5.20
//--------------------------------
//这个是对XActionSimple的整理进一步的规整
//这里是通过继承来实现的动作控制
class _XActionEx
{
protected:
	_XResourcePosition m_resoursePosition;	//资源所在的位置
	_XBool m_isInited;						//是否初始化
	_XActionSimpleType m_type;
	_XActionSimpleState m_actionState;		//动作的状态
	_XBool m_actionIsLoop;					//动作是否循环
	std::string m_actionName;				//动作的名称，用于帮助记忆
public:
	_XBool isName(const char *name);
	void setStop();	//终止动作，尚未实现
	void setLoop(_XBool isLoop) {m_actionIsLoop = isLoop;}	//设置动作是否循环
	_XBool getIsLoop() {return m_actionIsLoop;}		//获取动作是否循环
	void setStart();		//设置动作开始进行
	void setPause();		//设置动作暂停
	void setResume();		//设置动作恢复
	void move(int stepTime = 1);
	_XActionSimpleState getActionState(){return m_actionState;}
	_XBool getIsEnd() {return m_actionState == ACTION_STATE_END;}
	std::string getName() {return m_actionName;}

	_XActionEx()
		:m_isInited(XFalse)
		,m_actionState(ACTION_STATE_NULL)
		,m_actionIsLoop(XFalse)
		,m_actionSum(0)
		,m_pAction(NULL)
		,m_type(ACTION_TYPE_ATOMIC)
		,m_actionName(" ")
	{}
	_XBool init(_XResourcePosition resoursePosition,
		const char *actionName);	
	void draw();
	void keepDraw();	//不论是否结束都会保持绘制的函数
	void input(const _XInputEvent &inputEvent);

	~_XActionEx(){release();}
	void release();
//下面是复合动作的定义
private:
	_XActionEx **m_pAction;	//子动作
	int m_actionSum;		//子动作的数据量
	int m_nowActionIndex;
public:
	_XBool init(_XResourcePosition resoursePosition,const char *actionName,int actionSum,...);	//复合动作的初始化
public://下面是需要不同的动作实现的动作控制
	virtual void funResetData(){}				//重新设置数据
	virtual void funMove(int stepTime){}		//步进
	virtual void funDraw(){}					//描绘
	virtual void funKeepDraw(){}				//保持描绘，有些图像实在动作完成之后也需要绘制的，那么就在这个函数中绘制。
	virtual void funAtEnd(){}					//动作结束之后调用
	virtual _XBool funIsEnd(){return XTrue;}	//判断是否动作结束
	//进行资源管理
	virtual _XBool funInit(_XResourcePosition res){return XTrue;}	//资源初始化
	virtual void funRelease(){}	//资源释放函数
	//这里尚未加入输入控制
	virtual void funInput(const _XInputEvent &inputEvent){}
	//void * m_pClass;
};

#include <vector>
//动作管理的类
typedef int _XActionExHandle;		//动作的句柄
struct _XActionExData
{
	_XActionExHandle actionHandle;	//动作的编号
	_XBool actionIsEnable;			//动作是否有效
	_XActionEx *pAction;			//动作的指针

	_XActionExData()
		:actionIsEnable(XFalse)
		,pAction(NULL)
	{}
};
class _XActionExManager	//这个类并不负责资源释放，需要自己在外部释放资源
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//下面需要将其设计为Singleton模式
protected:
	_XActionExManager()
		:m_nowActionHandle(0)
	{}
	_XActionExManager(const _XActionExManager&);
	_XActionExManager &operator= (const _XActionExManager&);
	virtual ~_XActionExManager() {;} 
public:
	static _XActionExManager& GetInstance()
	{
		static _XActionExManager m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	int m_nowActionHandle;						//当前的动作句柄
	std::vector<_XActionExData> m_pActions;		//所有动作的表
	std::vector<int> m_actionOrder;		//用于描述动作的层叠关系，这里做一个映射(尚未生效)
	std::vector<int> m_actionOrderX;		//用于描述动作的层叠关系，这里做一个映射(尚未生效)
public:
	_XActionExHandle pushAction(_XActionEx *action);			//向表中推入一个动作，返回动作的句柄，-1表示动作已经存在

	_XActionExHandle getActionHandle(const char *actionName);	//通过名字获取动作句柄
	_XActionExHandle getActionHandle(const _XActionEx *action);	//通过名字获取动作句柄
	_XBool popAction(const char *actionName);					//从表中删除一个动作
	_XBool popAction(_XActionEx *action);
	_XBool popAction(_XActionExHandle actionHandle);	
	_XBool releaseAction(const char *actionName);			//从表中删除一个动作并释放掉相应的资源
	_XBool releaseAction(_XActionEx *action);
	_XBool releaseAction(_XActionExHandle actionHandle);
	_XBool setStartAction(const char *actionName);			//设置开始播放一个动作
	_XBool setStartAction(_XActionExHandle actionHandle);	//设置开始播放一个动作
	_XActionEx *getPAction(const char *actionName);
	_XActionEx *getPAction(_XActionExHandle actionHandle);

	_XBool getIsActionEnd(const char *actionName);			//获取这个动作是否播放完成
	_XBool getIsActionEnd(_XActionExHandle actionHandle);	//获取这个动作是否播放完成

	void move(int stepTime);		//遍历并推进所有的动作
	void draw();					//遍历并描绘所有的动作
	//这里尚未加入输入控制
	void input(const _XInputEvent &inputEvent);

	//下面的接口(尚未实现)
	void setAllActionStop();	//停止所有的动作
	void setAllActionPause();	//暂停所有的动作
	void setAllActionResume();	//恢复所有的动作
	void popAllAction();		//弹出所有的动作
	//下面是层叠关系的接口(尚未实现)
	//说明，下面会被上面遮挡
	_XBool moveToBottom(const char *actionName);			//移动到最下面
	_XBool moveToBottom(_XActionExHandle actionHandle);
	_XBool moveToTop(const char *actionName);				//移动到最上面
	_XBool moveToTop(_XActionExHandle actionHandle);
	_XBool moveUp(const char *actionName);					//向上移动一层,与上一层进行交换
	_XBool moveUp(_XActionExHandle actionHandle);
	_XBool moveDown(const char *actionName);					//向下移动一层
	_XBool moveDown(_XActionExHandle actionHandle);
	_XBool moveUpTo(const char *s,const char *d);			//s移动到d上面
	_XBool moveUpTo(_XActionExHandle s,_XActionExHandle d);
	_XBool moveDownTo(const char *s,const char *d);			//s移动到d上面
	_XBool moveDownTo(_XActionExHandle s,_XActionExHandle d);
	int getLayerIndex(const char *actionName);			//获取当前的在哪一层
	int getLayerIndex(_XActionExHandle actionHandle);
};
#define _XAExManager _XActionExManager::GetInstance()

#include "XActionEx.inl"

#endif