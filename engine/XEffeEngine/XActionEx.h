#ifndef _JIA_XACTIONEX_
#define _JIA_XACTIONEX_
//++++++++++++++++++++++++++++++++
//Author:	  贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.5.20
//--------------------------------
#include "XActionSimple.h"
#include "XInputEventCore.h"
#include <vector>
namespace XE{
//这个是对XActionSimple的整理进一步的规整
//这里是通过继承来实现的动作控制
class XActionEx
{
protected:
	XResourcePosition m_resoursePosition;	//资源所在的位置
	XBool m_isInited;						//是否初始化
	XActionSimpleType m_type;
	XActionSimpleState m_actionState;		//动作的状态
	XBool m_actionIsLoop;					//动作是否循环
	std::string m_actionName;				//动作的名称，用于帮助记忆
public:
	XBool isName(const char *name);
	void setStop();	//终止动作，尚未实现
	void setLoop(XBool isLoop) {m_actionIsLoop = isLoop;}	//设置动作是否循环
	XBool getIsLoop() const {return m_actionIsLoop;}		//获取动作是否循环
	void setStart();		//设置动作开始进行
	void setPause();		//设置动作暂停
	void setResume();		//设置动作恢复
	void move(float stepTime = 1.0f);
	XActionSimpleState getActionState()const{return m_actionState;}
	XBool getIsEnd() const {return m_actionState == ACTION_STATE_END;}
	std::string getName() const {return m_actionName;}

	XActionEx()
		:m_isInited(XFalse)
		,m_actionState(ACTION_STATE_NULL)
		,m_actionIsLoop(XFalse)
		,m_actionSum(0)
		,m_pAction(NULL)
		,m_type(ACTION_TYPE_ATOMIC)
		,m_actionName(" ")
	{}
	XBool init(XResourcePosition resoursePosition,
		const char *actionName);	
	void draw();
	virtual void keepDraw();	//不论是否结束都会保持绘制的函数
	void input(const XInputEvent &inputEvent);

	~XActionEx(){release();}
	void release();
//下面是复合动作的定义
private:
	XActionEx **m_pAction;	//子动作
	int m_actionSum;		//子动作的数据量
	int m_curActionIndex;
public:
	XBool init(XResourcePosition resoursePosition,const char *actionName,int actionSum,...);	//复合动作的初始化
public://下面是需要不同的动作实现的动作控制
	virtual void funResetData(){}				//重新设置数据
	virtual void funMove(float stepTime){}		//步进
	virtual void funDraw(){}					//描绘
	virtual void funKeepDraw(){}				//保持描绘，有些图像实在动作完成之后也需要绘制的，那么就在这个函数中绘制。
	virtual void funAtEnd(){}					//动作结束之后调用
	virtual XBool funIsEnd(){return XTrue;}	//判断是否动作结束
	//进行资源管理
	virtual XBool funInit(XResourcePosition res){return XTrue;}	//资源初始化
	virtual void funRelease(){}	//资源释放函数
	//这里尚未加入输入控制
	virtual void funInput(const XInputEvent &inputEvent){}
	//void * m_pClass;
};

//动作管理的类
typedef int XActionExHandle;		//动作的句柄
struct XActionExData
{
	XActionExHandle actionHandle;	//动作的编号
	XBool actionIsEnable;			//动作是否有效
	XActionEx *pAction;			//动作的指针

	XActionExData()
		:actionIsEnable(XFalse)
		,pAction(NULL)
	{}
};
class XActionExManager	//这个类并不负责资源释放，需要自己在外部释放资源
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//下面需要将其设计为Singleton模式
protected:
	XActionExManager()
		:m_curActionHandle(0)
	{}
	XActionExManager(const XActionExManager&);
	XActionExManager &operator= (const XActionExManager&);
	virtual ~XActionExManager() {;} 
public:
	static XActionExManager& GetInstance()
	{
		static XActionExManager m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	int m_curActionHandle;						//当前的动作句柄
	std::vector<XActionExData> m_pActions;		//所有动作的表
	std::vector<int> m_actionOrder;		//用于描述动作的层叠关系，这里做一个映射(尚未生效)
	std::vector<int> m_actionOrderX;		//用于描述动作的层叠关系，这里做一个映射(尚未生效)
public:
	XActionExHandle pushAction(XActionEx *action);			//向表中推入一个动作，返回动作的句柄，-1表示动作已经存在

	XActionExHandle getActionHandle(const char *actionName);	//通过名字获取动作句柄
	XActionExHandle getActionHandle(const XActionEx *action);	//通过名字获取动作句柄
	XBool popAction(const char *actionName);					//从表中删除一个动作
	XBool popAction(XActionEx *action);
	XBool popAction(XActionExHandle actionHandle);	
	XBool releaseAction(const char *actionName);			//从表中删除一个动作并释放掉相应的资源
	XBool releaseAction(XActionEx *action);
	XBool releaseAction(XActionExHandle actionHandle);
	XBool setStartAction(const char *actionName);			//设置开始播放一个动作
	XBool setStartAction(XActionExHandle actionHandle);	//设置开始播放一个动作
	XActionEx *getPAction(const char *actionName);
	XActionEx *getPAction(XActionExHandle actionHandle);

	XBool getIsActionEnd(const char *actionName);			//获取这个动作是否播放完成
	XBool getIsActionEnd(XActionExHandle actionHandle);	//获取这个动作是否播放完成

	void move(float stepTime);		//遍历并推进所有的动作
	void draw();					//遍历并描绘所有的动作
	//这里尚未加入输入控制
	void input(const XInputEvent &inputEvent);

	//下面的接口(尚未实现)
	void setAllActionStop();	//停止所有的动作
	void setAllActionPause();	//暂停所有的动作
	void setAllActionResume();	//恢复所有的动作
	void popAllAction();		//弹出所有的动作
	//下面是层叠关系的接口(尚未实现)
	//说明，下面会被上面遮挡
	XBool moveToBottom(const char *actionName);			//移动到最下面
	XBool moveToBottom(XActionExHandle actionHandle);
	XBool moveToTop(const char *actionName);				//移动到最上面
	XBool moveToTop(XActionExHandle actionHandle);
	XBool moveUp(const char *actionName);					//向上移动一层,与上一层进行交换
	XBool moveUp(XActionExHandle actionHandle);
	XBool moveDown(const char *actionName);					//向下移动一层
	XBool moveDown(XActionExHandle actionHandle);
	XBool moveUpTo(const char *s,const char *d);			//s移动到d上面
	XBool moveUpTo(XActionExHandle s,XActionExHandle d);
	XBool moveDownTo(const char *s,const char *d);			//s移动到d上面
	XBool moveDownTo(XActionExHandle s,XActionExHandle d);
	int getLayerIndex(const char *actionName);			//获取当前的在哪一层
	int getLayerIndex(XActionExHandle actionHandle);
};
#define XAExManager XActionExManager::GetInstance()
#if WITH_INLINE_FILE
#include "XActionEx.inl"
#endif
}
#endif