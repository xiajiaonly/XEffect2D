#ifndef _JIA_XCONTROLMANAGER_
#define _JIA_XCONTROLMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.10
//--------------------------------
#include "stdlib.h"
#include "XControlBasic.h"

#define MAX_CTRL_OBJ_SUM (512)	//控件管理器中允许注册的总控件数量

enum _XCtrlObjType
{
	CTRL_OBJ_NULL,		//无效的物件
	CTRL_OBJ_BUTTON,	//按钮物件
	CTRL_OBJ_BUTTONEX,	//按钮物件
	CTRL_OBJ_CHECK,		//复选框的物件
	CTRL_OBJ_EDIT,		//输入框的物件
	CTRL_OBJ_MOUSERIGHTBUTTONMENU,	//鼠标右键菜单的物件
	CTRL_OBJ_SLIDER,	//滑动条的物件
	CTRL_OBJ_RADIOS,	//单选框的物件
	CTRL_OBJ_PROGRESS,	//进度条的物件
	CTRL_OBJ_MUTITEXT,	//多行文本的物件
	CTRL_OBJ_MUTILIST,	//多列列表框
	CTRL_OBJ_COMBO,		//下拉列表框的物件
	CTRL_OBJ_DIRECTORYLIST,	//路径列表框
	CTRL_OBJ_GROUP,		//群组框
	CTRL_OBJ_POINTCTRL,		//点原
	CTRL_OBJ_LINECTRL,		//线原
	CTRL_OBJ_SLIDEREX,	//滑动条的物件
	CTRL_OBJ_FUNCTION,	//绘图函数
};
struct _XCtrlObjetP
{
	void *pObject;
	void (*pFunction)(void);
};
//这个类需要使用到单子系统
class _XControlManager
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//这种单子模式的设计方式比之前的要好
protected:
	_XControlManager();
	_XControlManager(const _XControlManager&);
	_XControlManager &operator= (const _XControlManager&);
	virtual ~_XControlManager(); 
public:
	static _XControlManager& GetInstance();
	//-------------------------------------------
private:
	_XCtrlObjetP m_pObject[MAX_CTRL_OBJ_SUM];				//物件注册的序列
	_XCtrlObjType m_objectType[MAX_CTRL_OBJ_SUM];	//物件的类型
	int m_objectSum;	//物件的数量
	int m_objectDrawOrderMap[MAX_CTRL_OBJ_SUM];		//物件描绘顺序的映射表
	char m_isAutoDraw;	//是否自己管理按钮的绘制
	int m_focusOrder;	//当前获得焦点的控件的编号，只有获得焦点的控件才能对鼠标键盘事件作出响应

public:
	int getObjSum() {return m_objectSum;}
	int addAObject(void * object,_XCtrlObjType type);			//注册一个物件,返回注册的ID，实际上是序列编号，-1为注册失败
	int addAObject(void (* function)());			//注册一个物件,返回注册的ID，实际上是序列编号，-1为注册失败
	void decreaseAObject(int objectID);							//去除一个物件的注册
	void decreaseAObject(const void * object);					//去除一个物件的注册
	void decreaseAObject(void (* function)());					//去除一个物件的注册
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
	if(objectID < 0 || objectID >= m_objectSum) return CTRL_OBJ_NULL;	//输入参数非法
	return m_objectType[objectID];
}

inline void *_XControlManager::getObject(int objectID) const
{
	if(objectID < 0 || objectID >= m_objectSum) return NULL;	//输入参数非法
	return m_pObject[objectID].pObject;
}

inline void _XControlManager::changeTowObject(int object1,int object2)				//将object1与object2的两个物体对调
{
	if(object1 < 0 || object1 >= m_objectSum) return;	//数据非法
	if(object2 < 0 || object2 >= m_objectSum) return;	//数据非法
	if(object1 == object2) return;	//多余的操作
	xChangeTwoSum(m_objectDrawOrderMap[object1],m_objectDrawOrderMap[object2]);
}

#endif