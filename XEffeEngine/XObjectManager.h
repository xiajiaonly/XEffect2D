#ifndef _JIA_XOBJECTMANAGER_
#define _JIA_XOBJECTMANAGER_

//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2012.11.9
//--------------------------------

#include "XOSDefine.h"
#include "XMouseAndKeyBoardDefine.h"
#include "XBasicSDL.h"
#include "XBasicFun.h"
#include "XFont.h"
#include "XControl/XControls.h"

#include "stdio.h"
#include <memory>
#include "string.h"

using namespace std;

//物件的类型的定义
enum _XObjectType
{
	OBJ_NULL,			//无效的物件
	OBJ_SPRITE,			//精灵的物件
	OBJ_FRAME,			//序列帧的物件
	OBJ_FRAMEEX,		//序列帧的物件
	OBJ_NUMBER,			//数字字符串的物件
	OBJ_FONTUNICODE,	//显示字符的物件
	OBJ_FONTX,			//显示字符的物件
	OBJ_NODELINE,		//节点曲线的物件
	OBJ_CONTROL,		//控件的物件
};

enum _XObjectOptionType
{
	OBJ_OPTION_SIZE_ON,			//物件放大
	OBJ_OPTION_SIZE_DOWN,			//物件缩小
	OBJ_OPTION_X_ON,			//物件右移
	OBJ_OPTION_X_DOWN,			//物件左移
	OBJ_OPTION_Y_ON,			//物件下移
	OBJ_OPTION_Y_DOWN,			//物件上移
	OBJ_OPTION_ROTATE_ON,		//旋转角度增大
	OBJ_OPTION_ROTATE_DOWN,		//旋转角度减小
	OBJ_OPTION_ROTATE_MODE_CHANGE,	//旋转模式变化
	OBJ_OPTION_TURNOVER_MODE_CHANGE,//翻转模式变化
};	

enum _XObjectOption
{
	OBJ_OPTION_NULL,			//无动作
	OBJ_OPTION_POSITION,		//位置变化
	OBJ_OPTION_SIZE,			//尺寸变化
	OBJ_OPTION_ROTATE,			//旋转
	OBJ_OPTION_ROTATEMODE,		//改变旋转模式
	OBJ_OPTION_TURNOVERMODE,	//改变翻转模式

	OBJ_OPTION_EDIT,			//物件处于编辑状态
};

enum _XObjectOptionState	//物件当前设置的状态
{
	OBJ_OPTION_STATE_NULL,					//没有设置任何状态
	OBJ_OPTION_STATE_CAN_NOT_CHOOSE,		//不被选择状态
	OBJ_OPTION_STATE_NO_MOUSE_PROC,			//不接受鼠标操作
	OBJ_OPTION_STATE_NO_KEY_PROC,			//不接受鼠键盘操作
};

#define MAX_OBJECT_SUM (8192)	//最大可以注册的物件数量

//物件需要使用到的信息
struct _XObjectInfo
{
	_XVector2 m_position;	//物件的位置
	_XVector2 m_size;		//物件的尺寸
	float m_angle;			//物件的角度
//	float m_alpha;			//物件的透明度
	float length;			//对于NodeLine来说还需要显示他的长度
};

enum _XObjectState	//物件的状态
{
	OBJ_STATE_NULL,			//无状态
	OBJ_STATE_BE_CHOOSE,	//选中状态
	OBJ_STATE_MOUSE_DOWN,	//鼠标按下状态
	OBJ_STATE_MOUSE_MOVE,	//鼠标拖动状态
	OBJ_STATE_MOUSE_ON,		//鼠标悬浮状态
//	OBJ_STATE_EDIT,			//编辑状态
};

//这个类是一个单子系统，在整个应用中只有一个实体
class _XObjectManager
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//这种单子模式的设计方式比之前的要好
protected:
	_XObjectManager();
	_XObjectManager(const _XObjectManager&);
	_XObjectManager &operator= (const _XObjectManager&);
	virtual ~_XObjectManager(); 
public:
	static _XObjectManager& GetInstance();
	//-------------------------------------------
private:
	void *m_pObject[MAX_OBJECT_SUM];	//物件的指针
	_XObjectState m_objectState[MAX_OBJECT_SUM];	//物件的状态信息 0:无状态 1：被选中状态 2：鼠标按下状态 3：鼠标悬浮状态
	_XObjectState m_objectUpState[MAX_OBJECT_SUM];	//物件的上一次状态信息
	_XVector2 m_objectMousePoint[MAX_OBJECT_SUM];		//鼠标按键的位置
	_XObjectType m_objectType[MAX_OBJECT_SUM];	//物件类型的标记
	int m_nowObjectSum;					//当前的注册的物件数量
	int m_nowMouseOnObjectSum;			//当前处于鼠标on状态下的物件数量
	_XObjectOption m_objectKeyOption[MAX_OBJECT_SUM];	//物件当前按键按下对应的功能 0：无效，1：移动，2：缩放，3：旋转，4：改变旋转模式，5：设置翻转状态
	//下面这个变量需要加一个接口来设置
	_XObjectOptionState m_objectSetState[MAX_OBJECT_SUM];	//物件当前被设置的状态，0：没有设置状态，1：设置为不能选择状态，2：设置为不接受鼠标操作状态，3：设置为不接受键盘操作状态

	int m_objectEditParm[MAX_OBJECT_SUM];	//物件进行编辑的时候的辅助参数，对于NodeLine就是鼠标选中的点的编号
	_XVector2 m_nowMousePosition;			//鼠标当前的位置

	void objectKeyOption(int order,_XObjectOptionType optionType);	//对相应的物件进行操作，OptionType，是动作

	//需要一个字体的支持用于显示物件信息
	_XObjectInfo m_objInfo[MAX_OBJECT_SUM];	//物件的信息
	//下面这两个数据没有与列表框的操作相对应，因而存在问题
	char m_objCanEdit[MAX_OBJECT_SUM];		//物件是否可以编辑
	int m_objLineOrder[MAX_OBJECT_SUM];		//物件与列表框行的对应关系
	int m_lineObjOrder[MAX_OBJECT_SUM];		//列表框行与物件的对应关系

	//_XFontUnicode m_font[MAX_OBJECT_SUM];		//显示的字符
	_XFontUnicode m_font;		//显示的字符
	char m_isInited;	//是否初始化
	bool checkNeedUpdate(int order);	//检查物件信息是否需要更新
	void updateObjInfo(int order);		//更新物件信息
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了编辑而定义的内容
	_XSliderTexture m_sliderTextureH;
	_XSlider m_sliderH;
	_XSliderTexture m_sliderTextureV;
	_XSlider m_sliderV;
	_XCheckTexture m_checkTex0;
	_XCheckTexture m_checkTex1;
	_XMultiListTexture m_mutiListTexture;
	_XMultiListBasic m_mutiList;	//多列列表框
	//--------------------------------------------------
	friend void funObjectSelectChange(void *,int);
	friend void funObjectStateChange(void *,int);
	char m_isShowUI;		//是否显示界面元素
	char m_isOption;		//是否可以操作
	void setShow() 
	{
		m_isShowUI = 1;
		m_mutiList.setVisiable();
	}	//设置显示
	void disShow() 
	{
		m_isShowUI = 0;
		m_mutiList.disVisiable();
	}	//设置不显示
	void setOption()
	{
		m_isOption = 1;
		setShow();
	}
	void disOption()
	{
		m_isOption = 0;
		disShow();
		//取消掉所有的动作
		for(int i = 0;i < m_nowObjectSum;++ i)
		{
			m_objectKeyOption[i] = OBJ_OPTION_NULL;
		}
	}
public:
	//下面是为了实现按键的连续动作而定义的
	_XKeyState m_keyState[4];	//对应于上下左右四个按键
	int m_keyTimer[4];	//四个按键的计时
	int m_keyNowTime[4];	//四个按键的当前时间间隔
	int m_keyMaxTime;	//按键的最长响应时间
	int m_keyMinTime;	//按键的最短响应时间
public:
	int init(int resoursePosition = 0);
	int addAObject(void * object,_XObjectType type);			//注册一个物件,返回注册的ID，实际上是序列编号，-1为注册失败
	void decreaseAObject(int objectID);							//去除一个物件的注册
	void decreaseAObject(const void * object);					//去除一个物件的注册
	int findObjectID(const void * object) const;		//获取指定物件的ID -1表示该物件没有注册
	_XObjectType getObjectType(int objectID) const;	//获取物件的类型
	void *getObject(int objectID) const;				//获取物件的指针

	void mouseProc(int x,int y,_XMouseState eventType);	//物件对鼠标事件的响应
	void keyProc(int keyID,_XKeyState keyState);		//keyState:0按下，1弹起
	int getNowObjectSum() const;				//获取当前注册的物件数量
	int getNowMouseOnObjectSum() const;			//获取当前处于鼠标On状态下的物件数量
	int getNowChooseObjectSum() const;			//获取当前被选中的物件数量
	void getNowChooseObjectID(int *ID) const;	//获取当前被选择的所有物件的ID序列

	void draw();			//描绘物件的一些信息
	void move(int delay);	//动作
};

#include "XObjectManager.inl"

#endif