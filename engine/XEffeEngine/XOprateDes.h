#ifndef _JIA_XOPRATEDES_
#define _JIA_XOPRATEDES_
//这个是针对于undo和redo而定义的动作描述和动作管理的类，如果类需要使用undo和redo功能只需要基于该基类即可
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.23
//--------------------------------
#include "XBasicClass.h"
#include <vector>
namespace XE{
class XBasicOprate;
//这个结构用于描述一个动作
struct XOperateDes
{
	XBasicOprate *pObj;	//动作发生的实体指针
	void *pData;			//描述这个动作状态的数据，不同的实体数据结构和内容不同
	XOperateDes()
		:pObj(NULL)
		, pData(NULL)
	{}
	~XOperateDes()
	{}
	void release();	//这里可能存在问题，当pObj的实体被释放时，这里会造成crash。
};
//这个是动作管理器，动作管理器的设计原理为三层设计
//状态队列：0、1、2、3、4、5、6、7、8、9……	：所有受管理的物件将会根据时间先后将状态变化推入这个队列
//动作索引队列：5、7、9……						：一个动作可能由一系列的状态变化组成，当明确一个动作完成时，将状态序列的编号推入这个队列
//当前动作的索引号：0、							：用于记录当前处于哪个动作，通过改变这个值来实现动作的回溯
class XOperateManager
{
public:
	XOperateManager()
		:m_curStateIndex(-1)
	{}
	virtual~XOperateManager() { clearAll(); }
protected:
	XOperateManager(const XOperateManager&);
	XOperateManager &operator= (const XOperateManager&);
public:
	std::vector<XOperateDes> m_operateDes;			//状态描述的队列
	std::vector<int> m_operateIndex;				//动作相应的状态序号的队列
	int m_curStateIndex;							//当前动作的编号
public:
	void clearAll();	//清除所有的动作数据
	void undo();		//撤销操作
	void redo();		//重做操作
	XBool canUndo() { return (m_curStateIndex > 0 && m_operateIndex.size() > 0); }	//判断是否可以做撤销操作
	XBool canRedo() { return (m_curStateIndex < (int)(m_operateIndex.size()) - 1); }		//判断是否可以做重做操作
	void addAOperate(XBasicOprate *obj);	//向状态序列中添加一个状态，如果之前做过撤销操作则会丢弃掉当前状态之后的状态
	void setOperateOver();		//设置动作完成，将一系列的状态变化打包成一个动作
	void decreaseObj(XBasicOprate *obj);	//从序列中将指定物件的所有操作清除掉
};
class XBasicOprate	//所有需要进行undo和redo的类都需要继承自这个类
{
protected:
	bool m_withUndoAndRedo;				//表示变量用于标记动作是否需要经过动作管理器管理，如果开启的话，动作将会推入动作管理器
	void *m_pStateClass;				//回调函数中的指针指向回调函数的实体
	void(*m_funStateChange)(void *);	//状态发生变化是调用的回调函数
public:
	virtual void setWithUndo(bool flag) { m_withUndoAndRedo = flag; }	//开启或者关闭动作管理，
	virtual void setOprateState(void * data) = 0;		//设置物件的当前状态，data为描述当前状态的数据，不同的实体data的结构和内容不同
	virtual void *getOprateState() const = 0;			//获取用于描述当前状态的数据，返回数据的指针，这个数据的销毁由动作管理器进行
	virtual void releaseOprateStateData(void *) = 0;	//释放getOprateState分派的内存空间，由于只有分配的函数知道类型，为了能够调用析构函数，而如此设计
	virtual void stateChange() = 0;			//状态发生变化时需要将该动作推入动作管理器是调用，函数负责将当前动作保存到动作管理器，并调用状态变化的回调函数
//	{//有bug
//		if(m_withUndoAndRedo) XOpManager.addAOperate(this);	//如果需要记录当前状态则将调用动作管理器的相关函数
//		if(m_funStateChange != NULL) m_funStateChange(m_pStateClass);	//调用相关的回调函数
//	}
	virtual void setStateChangeCB(void(*funStateChange)(void *), void * p)	//设置状态变化时调用的回调函数
	{
		m_funStateChange = funStateChange;
		m_pStateClass = p;
	}
	virtual bool isSameState(void * data) = 0;	//判断data所描述的状态和当前状态是否一致	//为了可扩展性应该改成判断两个动作是否一致(尚未进行)

	XBasicOprate()
		:m_withUndoAndRedo(false)
		, m_pStateClass(NULL)
		, m_funStateChange(NULL)
	{}
	//物件注销的时候需要在这里整理动作列表将相关的动作注销掉以保证操作的合理性
	virtual ~XBasicOprate() {}
//	{//有bug
//		if(gFrameworkData.pOperateManager != NULL)
//			gFrameworkData.pOperateManager->decreaseObj(this);
//	}
};
#if WITH_INLINE_FILE
#include "XOprateDes.inl"
#endif
}
#endif