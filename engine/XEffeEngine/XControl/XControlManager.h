#ifndef _JIA_XCONTROLMANAGER_
#define _JIA_XCONTROLMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.10
//--------------------------------
#include "stdlib.h"
#include "XControlBasic.h"

//#define MAX_CTRL_OBJ_SUM (512)	//控件管理器中允许注册的总控件数量

struct _XCtrlObjetInfo
{
	_XControlBasic *pObject;	//控件的指针
	void (*pFunction)(void);	//绘制函数的指针
//	_XCtrlObjType type;			//控件的类型
	_XBool needDraw;			//是否拥有绘制权限
	_XBool neadInput;			//是否拥有输入处理权限
	_XCtrlObjetInfo()
		:pObject(NULL)
		,pFunction(NULL)
//		,type(CTRL_OBJ_NULL)
		,needDraw(XFalse)
		,neadInput(XFalse)
	{}
};
//这个类需要使用到单子系统
class _XControlManager
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//这种单子模式的设计方式比之前的要好
protected:
	_XControlManager()
		:m_isAutoDraw(1)
		,m_focusOrder(-1)
		,m_isShiftDown(false)
	{
		//for(int i = 0;i < MAX_CTRL_OBJ_SUM;++ i)
		//{
		//	m_pObject[i].pObject = NULL;
		//	m_pObject[i].pFunction = NULL;
		//	m_objectType[i] = CTRL_OBJ_NULL;
		//	m_objectDrawOrderMap[i] = 0;
		//}
	}
	_XControlManager(const _XControlManager&);
	_XControlManager &operator= (const _XControlManager&);
	virtual ~_XControlManager(){;}

public:
	static _XControlManager& GetInstance()
	{
		static _XControlManager m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	std::vector<_XCtrlObjetInfo> m_ctrlObjInfos;
	std::vector<int> m_ctrlDrawOrderMap;
//	_XCtrlObjetP m_pObject[MAX_CTRL_OBJ_SUM];		//物件注册的序列
//	_XCtrlObjType m_objectType[MAX_CTRL_OBJ_SUM];	//物件的类型
//	int m_objectSum;	//物件的数量
//	int m_objectDrawOrderMap[MAX_CTRL_OBJ_SUM];		//物件描绘顺序的映射表
	char m_isAutoDraw;	//是否自己管理按钮的绘制
	int m_focusOrder;	//当前获得焦点的控件的编号，只有获得焦点的控件才能对鼠标键盘事件作出响应
	bool m_isShiftDown;

public:
	int getObjSum() const {return m_ctrlObjInfos.size();}
	_XBool addACtrl(_XControlBasic * object);			//注册一个物件,返回注册的ID，实际上是序列编号，-1为注册失败
	_XBool addAFunction(void (* function)());			//注册一个物件,返回注册的ID，实际上是序列编号，-1为注册失败
	void setNeedDraw(_XBool flag,int objectID);			//设置某个物件是否有管理器显示
	void setNeedDraw(_XBool flag,const void *object);	//设置某个物件是否有管理器显示
	void setNeedInput(_XBool flag,int objectID);			//设置某个物件是否有管理器显示
	void setNeedInput(_XBool flag,const void *object);	//设置某个物件是否有管理器显示
	void decreaseAObject(int objectID);					//去除一个物件的注册,传入物件的属顺序
	void decreaseAObject(const void * object);			//去除一个物件的注册
	void decreaseAObject(void (* function)());			//去除一个物件的注册
	int findObjectID(const void * object) const;		//获取指定物件的ID -1表示该物件没有注册
	int findObjectID(void (* function)()) const;		//获取指定物件的ID -1表示该物件没有注册
	_XCtrlObjType getObjectType(int objectID) const;	//获取物件的类型
	void *getObject(int objectID) const;				//获取物件的指针
	//为了达到绘图顺序修改以及消息处理顺序修改的目的，这里提供一组函数用于调整控件的注册顺序（尚未经过测试）
	void moveAndInsert(int objectID,int insertObjectID);	//将物件objectID插入到insertID的位置，并将两者之间的物件全部顺序移动
	void changeTowObject(int object1,int object2);				//将object1与object2的两个物体对调
	void setIsAutoDraw(_XBool isAutoDraw)
	{
		if(isAutoDraw) m_isAutoDraw = 1;
		else m_isAutoDraw = 0;
	}
public:
	void mouseProc(int x,int y,_XMouseState mouseState);		//鼠标事件的函数
	void keyProc(int keyOrder,_XKeyState keyState);			//按键事件的函数
	void insertChar(const char *ch,int len);
	void draw();
	void update(int stepTime);

	void draw(const void * obj);	//手动描绘某个物体
};
#define _XCtrlManger _XControlManager::GetInstance()
inline void _XControlManager::setNeedDraw(_XBool flag,int objectID)
{
	if(objectID < 0 || objectID >= m_ctrlObjInfos.size()) return;	//输入参数非法
	m_ctrlObjInfos[objectID].needDraw = flag;
}
inline void _XControlManager::setNeedDraw(_XBool flag,const void *object)
{
	setNeedDraw(flag,findObjectID(object));
}
inline void _XControlManager::setNeedInput(_XBool flag,int objectID)
{
	if(objectID < 0 || objectID >= m_ctrlObjInfos.size()) return;	//输入参数非法
	m_ctrlObjInfos[objectID].neadInput = flag;
}
inline void _XControlManager::setNeedInput(_XBool flag,const void *object)
{
	setNeedInput(flag,findObjectID(object));
}
inline void _XControlManager::decreaseAObject(const void * object)
{
	decreaseAObject(findObjectID(object));
}
inline void _XControlManager::decreaseAObject(void (* function)())
{
	decreaseAObject(findObjectID(function));
}
inline _XCtrlObjType _XControlManager::getObjectType(int objectID) const
{
	if(objectID < 0 || objectID >= m_ctrlObjInfos.size()) return CTRL_OBJ_NULL;	//输入参数非法
	if(m_ctrlObjInfos[objectID].pObject == NULL) return CTRL_OBJ_FUNCTION;
	else return m_ctrlObjInfos[objectID].pObject->getCtrlType();
}
inline void *_XControlManager::getObject(int objectID) const
{
	if(objectID < 0 || objectID >= m_ctrlObjInfos.size()) return NULL;	//输入参数非法
	return m_ctrlObjInfos[objectID].pObject;
}
inline void _XControlManager::changeTowObject(int object1,int object2)				//将object1与object2的两个物体对调
{
	if(object1 < 0 || object1 >= m_ctrlObjInfos.size() ||
		object2 < 0 || object2 >= m_ctrlObjInfos.size() ||
		object1 == object2) return;	//多余的操作
	xChangeTwoSum(m_ctrlDrawOrderMap[object1],m_ctrlDrawOrderMap[object2]);
}
inline void _XControlManager::insertChar(const char *ch,int len)
{
	if(m_focusOrder < 0 || m_focusOrder >= m_ctrlDrawOrderMap.size()) return;
	int index = m_ctrlDrawOrderMap[m_focusOrder];
	if(m_ctrlObjInfos[index].pObject != NULL) 
			m_ctrlObjInfos[index].pObject->insertChar(ch,len);
}
inline void _XControlManager::draw(const void * obj)	//手动描绘某个物体
{
	if(m_isAutoDraw != 0 ||
		findObjectID(obj) < 0) return;
	((_XControlBasic *)obj)->draw();
	((_XControlBasic *)obj)->drawUp();
}
#endif