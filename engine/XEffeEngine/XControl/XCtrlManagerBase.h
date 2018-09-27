#ifndef _JIA_XCTRLMANAGERBASE_
#define _JIA_XCTRLMANAGERBASE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.9
//--------------------------------
#include "stdlib.h"
#include "XControlBasic.h"
namespace XE{
//这是一个控件管理的基类，所有带有控件管理能力的类都要继承自这个类或者是包含这个类的成员
struct XCtrlObjetInfo
{
	XControlBasic *pObject;	//控件的指针
	void (*pFunction)(void);	//绘制函数的指针
	XBool needDraw;			//是否拥有绘制权限
	XBool needInput;			//是否拥有输入处理权限
	XCtrlObjetInfo()
		:pObject(NULL)
		,pFunction(NULL)
		,needDraw(XFalse)
		,needInput(XFalse)
	{}
};
class XGameBasic;
class XCtrlManagerBase
{
public:
	XCtrlManagerBase()
		:m_isAutoDraw(1)
		,m_focusOrder(-1)
		,m_isShiftDown(false)
		,m_pGame(NULL)
		,m_withWireframe(true)
	{}
	XCtrlManagerBase(const XCtrlManagerBase&);
	virtual ~XCtrlManagerBase(){;}
protected:
	std::vector<XControlBasic *> m_specialCtrls;	//特殊的控件，目前该功能尚未实现
	std::vector<XCtrlObjetInfo> m_ctrlObjInfos;
	std::vector<int> m_ctrlDrawOrderMap;	//这个数据在使用的时候存在问题！！
	char m_isAutoDraw;	//是否自己管理按钮的绘制
	bool m_isShiftDown;
	bool m_withWireframe;	//是否在控件外面描绘一个线框
	int m_focusOrder;	//当前获得焦点的控件的编号，只有获得焦点的控件才能对鼠标键盘事件作出响应,序号为原始序号，非映射序号
public:
	int findSpecialObjectID(const void * object) const;	//寻找特殊的物件

	unsigned int getObjSum() const {return m_ctrlObjInfos.size();}
	XBool addACtrl(XControlBasic * object);			//注册一个物件,返回注册的ID，实际上是序列编号，-1为注册失败
	XBool addAFunction(void (* function)());			//注册一个物件,返回注册的ID，实际上是序列编号，-1为注册失败
	void setNeedDraw(XBool flag,unsigned int objectID);			//设置某个物件是否有管理器显示
	void setNeedDraw(XBool flag,const void *object);	//设置某个物件是否有管理器显示
	void setNeedInput(XBool flag,unsigned int objectID);			//设置某个物件是否有管理器显示
	void setNeedInput(XBool flag,const void *object);	//设置某个物件是否有管理器显示
	void setNeedDrawAndInput(bool needDraw,bool needInput,const void *object)
	{
		int id = findObjectID(object);
		setNeedDraw(needDraw,id);
		setNeedInput(needInput,id);
	}
	void setNeedDrawAndInput(bool needDraw,bool needInput,unsigned int objectID)
	{
		setNeedDraw(needDraw,objectID);
		setNeedInput(needInput,objectID);
	}
	void decreaseAObject(int objectID);					//去除一个物件的注册,传入物件的属顺序
	void decreaseAObject(const void * object);			//去除一个物件的注册
	void decreaseAObject(void (* function)());			//去除一个物件的注册
	void clearAllObject();								//清除所有的物件
	int findObjectID(const void * object) const;		//获取指定物件的ID -1表示该物件没有注册
	int findObjectID(void (* function)()) const;		//获取指定物件的ID -1表示该物件没有注册
	XCtrlObjType getObjectType(unsigned int objectID) const;	//获取物件的类型
	void *getObject(unsigned int objectID) const;				//获取物件的指针
	//为了达到绘图顺序修改以及消息处理顺序修改的目的，这里提供一组函数用于调整控件的注册顺序（尚未经过测试）
	void moveAndInsert(int objectID,int insertObjectID);	//将物件objectID插入到insertID的位置，并将两者之间的物件全部顺序移动
	void moveToFirst(int objectID){moveAndInsert(objectID,m_ctrlDrawOrderMap[0]);}
	void moveToEnd(int objectID){moveAndInsert(objectID,m_ctrlDrawOrderMap[(int)(m_ctrlDrawOrderMap.size()) - 1]);}
	void changeTowObject(unsigned int object1,unsigned int object2);				//将object1与object2的两个物体对调
	void setIsAutoDraw(XBool isAutoDraw)
	{
		if(isAutoDraw) m_isAutoDraw = 1;
		else m_isAutoDraw = 0;
	}
public:
	void setWithWireframe(bool flag){m_withWireframe = flag;}
	void mouseProc(const XVec2& p,XMouseState mouseState);		//鼠标事件的函数
	void keyProc(int keyOrder,XKeyState keyState);			//按键事件的函数
	void insertChar(const char *ch,int len);
	void draw();
	void drawUp();
	void update(float stepTime);

	void draw(const void * obj);	//手动描绘某个物体
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了优化掉控件的回调函数处理机制而定义的变量
	XGameBasic *m_pGame;
	void eventProc(int id,int eventID);
	void setGame(XGameBasic *pGame){m_pGame = pGame;}
	//---------------------------------------------------------
};
inline void XCtrlManagerBase::setNeedDraw(XBool flag,unsigned int objectID)
{
	if(objectID >= m_ctrlObjInfos.size()) return;	//输入参数非法
	m_ctrlObjInfos[objectID].needDraw = flag;
}
inline void XCtrlManagerBase::setNeedDraw(XBool flag,const void *object)
{
	setNeedDraw(flag,findObjectID(object));
}
inline void XCtrlManagerBase::setNeedInput(XBool flag,unsigned int objectID)
{
	if(objectID >= m_ctrlObjInfos.size()) return;	//输入参数非法
	m_ctrlObjInfos[objectID].needInput = flag;
}
inline void XCtrlManagerBase::setNeedInput(XBool flag,const void *object)
{
	setNeedInput(flag,findObjectID(object));
}
inline void XCtrlManagerBase::decreaseAObject(const void * object)
{
	int ret = findObjectID(object);
	if(ret >= 0) decreaseAObject(ret);
	else
	{
		ret = findSpecialObjectID(object);
		if(ret < 0) return;
		m_specialCtrls.erase(m_specialCtrls.begin() + ret);
	}
}
inline void XCtrlManagerBase::decreaseAObject(void (* function)())
{
	decreaseAObject(findObjectID(function));
}
inline XCtrlObjType XCtrlManagerBase::getObjectType(unsigned int objectID) const
{
	if(objectID >= m_ctrlObjInfos.size()) return CTRL_OBJ_NULL;	//输入参数非法
	if(m_ctrlObjInfos[objectID].pObject == NULL) return CTRL_OBJ_FUNCTION;
	else return m_ctrlObjInfos[objectID].pObject->getCtrlType();
}
inline void *XCtrlManagerBase::getObject(unsigned int objectID) const
{
	if(objectID >= m_ctrlObjInfos.size()) return NULL;	//输入参数非法
	return m_ctrlObjInfos[objectID].pObject;
}
inline void XCtrlManagerBase::changeTowObject(unsigned int object1,unsigned int object2)				//将object1与object2的两个物体对调
{
	if(object1 >= m_ctrlObjInfos.size() ||
		object2 >= m_ctrlObjInfos.size() ||
		object1 == object2) return;	//多余的操作
	int index1 = -1;
	int index2 = -1;
	for(unsigned int i = 0;i < m_ctrlDrawOrderMap.size();++ i)
	{
		if(m_ctrlDrawOrderMap[i] < 0) continue;
		if((unsigned int)m_ctrlDrawOrderMap[i] == object1) index1 = i;
		if((unsigned int)m_ctrlDrawOrderMap[i] == object2) index2 = i;
		if(index1 >= 0 && index2 >= 0) break;
	}
	XMath::swapTwoSum(m_ctrlDrawOrderMap[index1],m_ctrlDrawOrderMap[index2]);
	//XMath::swapTwoSum(m_ctrlDrawOrderMap[object1],m_ctrlDrawOrderMap[object2]);
}
inline void XCtrlManagerBase::insertChar(const char *ch,int len)
{
	if(m_focusOrder < 0 || m_focusOrder >= m_ctrlDrawOrderMap.size()) return;
	if(m_ctrlObjInfos[m_focusOrder].pObject != NULL) 
			m_ctrlObjInfos[m_focusOrder].pObject->insertChar(ch,len);
}
inline void XCtrlManagerBase::draw(const void * obj)	//手动描绘某个物体
{
//	if(m_isAutoDraw != 0 ||
//		findObjectID(obj) < 0) return;
	if(obj == NULL) return;
	((XControlBasic *)obj)->draw();
	((XControlBasic *)obj)->drawUp();
}
}
#endif