#ifndef _JIA_XOBJECTMANAGER_
#define _JIA_XOBJECTMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2012.11.9
//--------------------------------
#include "XCommonDefine.h"
#include "XMath\XVector2.h"
#include "XObjectBasic.h"
#include "XInputEventCommon.h"
namespace XE{
/**********************************
//编辑界面的功能键说明
1、‘ctrl + h’：是否显示属性窗口
2、‘ctrl + u’：是否开始编辑功能
3、‘ctrl + p’：改变属性窗口的位置
4、‘Q’：去除所有操作
5、‘E’：NodeLine物件的编辑转换
6、‘S’：尺寸缩放
7、‘R’：旋转
8、‘M’：移动
9、‘D’：NodeLine中删除当前 选中的点
10、‘A’：NodeLine中在鼠标位置增加一个点
11、‘C’：改变旋转基点
12、‘T’：设置反转状态
13、‘left’‘right’‘up’‘down’：移动物体
14、‘N’:是否允许点选物件
15、‘B’：是否显示物件的ID
16、‘Delete’：从列表中删除选择的物件
17、‘Shift’：片选物件，可以同时改变多个物件的状态
***********************************/

#define OBJ_MANAGER_WITH_ID (1)	//显示物件ID会由于字体的建立而递归调用，会造成问题

//物件的类型的定义
enum XObjectOptionType
{
	OBJ_OPTION_SIZE_ON,			//物件放大
	OBJ_OPTION_SIZE_DOWN,		//物件缩小
	OBJ_OPTIONX_ON,			//物件右移
	OBJ_OPTIONX_DOWN,			//物件左移
	OBJ_OPTION_Y_ON,			//物件下移
	OBJ_OPTION_Y_DOWN,			//物件上移
	OBJ_OPTION_ROTATE_ON,		//旋转角度增大
	OBJ_OPTION_ROTATE_DOWN,		//旋转角度减小
	OBJ_OPTION_ROTATE_MODE_CHANGE,	//旋转模式变化
	OBJ_OPTION_TURNOVER_MODE_CHANGE,//翻转模式变化
};	
enum XObjectOption
{
	OBJ_OPTION_NULL,			//无动作
	OBJ_OPTION_POSITION,		//位置变化
	OBJ_OPTION_SIZE,			//尺寸变化
	OBJ_OPTION_ROTATE,			//旋转
	OBJ_OPTION_ROTATEMODE,		//改变旋转模式
	OBJ_OPTION_TURNOVERMODE,	//改变翻转模式

	OBJ_OPTION_EDIT,			//物件处于编辑状态
};
enum XObjectOptionState	//物件当前设置的状态
{
	OBJ_OPTION_STATE_NULL,					//没有设置任何状态
	OBJ_OPTION_STATE_CAN_NOT_CHOOSE,		//不被选择状态
	OBJ_OPTION_STATE_NO_MOUSE_PROC,			//不接受鼠标操作
	OBJ_OPTION_STATE_NO_KEY_PROC,			//不接受鼠键盘操作
};
enum XObjectMouseState	//物件的鼠标状态状态
{
	OBJ_STATE_NULL,			//无状态
	OBJ_STATE_MOUSE_DOWN,	//鼠标按下状态
	OBJ_STATE_MOUSE_MOVE,	//鼠标拖动状态
	OBJ_STATE_MOUSE_ON,		//鼠标悬浮状态
};
class XFontUnicode;
class XMultiListBasic;
//物件需要使用到的信息
struct XObjectInfo
{
	XVec2 m_position;	//物件的位置
	XVec2 m_scale;		//物件的尺寸
	float m_angle;			//物件的角度
//	float m_alpha;			//物件的透明度
	float length;			//对于NodeLine来说还需要显示他的长度

	XObjectBasic *m_pObject;	//物件的指针
	XObjectMouseState m_objectMouseState;	//物件的鼠标状态
	XVec2 m_objectMousePoint;//鼠标按键的位置
	XObjectOption m_objectKeyOption;	//物件当前按键按下对应的功能 0：无效，1：移动，2：缩放，3：旋转，4：改变旋转模式，5：设置翻转状态
	XObjectOptionState m_objectSetState;	//物件当前被设置的状态，0：没有设置状态，1：设置为不能选择状态，2：设置为不接受鼠标操作状态，3：设置为不接受键盘操作状态
	int m_objectEditParm;	//物件进行编辑的时候的辅助参数，对于NodeLine就是鼠标选中的点的编号
	//下面这两个数据没有与列表框的操作相对应，因而存在问题
	XBool m_objCanEdit;		//物件是否可以编辑		
	XBool m_objBeSelect;		//物件是否被选择

	int m_objLineOrder;		//物件与列表框行的对应关系
	int m_lineObjOrder;		//列表框行与物件的对应关系
#if OBJ_MANAGER_WITH_ID
	XFontUnicode *m_fontID;		//显示物件的ID，这个非常消耗性能，需要斟酌
#endif
	XObjectInfo()
		:m_pObject(NULL)
		, m_objectMouseState(OBJ_STATE_NULL)
		, m_objectKeyOption(OBJ_OPTION_NULL)
		, m_objectSetState(OBJ_OPTION_STATE_NULL)
		, m_objLineOrder(-1)
		, m_lineObjOrder(-1)
		, m_objCanEdit(XFalse)	//初始化所有的物件都是不可编辑的
		, m_objBeSelect(XFalse)	//初始化物件不被选择
#if OBJ_MANAGER_WITH_ID
		, m_fontID(NULL)
#endif
	{}
};
//这个类是一个单子系统，在整个应用中只有一个实体
class XObjectManager
{
public:
	XObjectManager();
	virtual ~XObjectManager();
protected:
	XObjectManager(const XObjectManager&);
	XObjectManager &operator= (const XObjectManager&);
private:
	int m_curMouseOnObjectSum;			//当前处于鼠标on状态下的物件数量
	//下面这个变量需要加一个接口来设置
	XVec2 m_curMousePosition;			//鼠标当前的位置

	void objectKeyOption(unsigned int order, XObjectOptionType optionType);	//对相应的物件进行操作，OptionType，是动作

	//需要一个字体的支持用于显示物件信息
	std::vector<XObjectInfo> m_objInfo;
	//下面这两个数据没有与列表框的操作相对应，因而存在问题
	XBool m_canAddObj;		//是否允许注册物件,设置这个变量用于使得注册物件失败
#if OBJ_MANAGER_WITH_ID
	char m_showObjID;		//是否显示ObjID
	void setShowObjID();
#endif
	void release();
	XFontUnicode *m_font;		//显示的字符
	XBool m_isInited;	//是否初始化
	XBool checkNeedUpdate(int order);	//检查物件信息是否需要更新
	void updateObjInfo(int order);		//更新物件信息
	void updateObjStateInfo(int order);	//更新物件的状态信息
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了编辑而定义的内容
	XMultiListBasic *m_mutiList;	//多列列表框
	void upDateMultiListData();		//更新多行列表框中的类容
	//--------------------------------------------------
	static void ctrlProc(void*, int, int);
	XBool m_isShowUI;		//是否显示界面元素
	XBool m_isOption;		//是否可以操作
	XBool m_canSelect;		//是否允许鼠标点选物件
	char m_editWindowsPos;	//编辑窗口的位置	1	0
	void setEditWidowsPos();					//	2	3

	void chengeCanSelectState();	//改变物件是否可以被点选的标签
	void setShow();//设置显示
	void disShow();//设置不显示
	void setOption();
	void disOption();
public:
	//下面是为了实现按键的连续动作而定义的
	XKeyState m_keyState[4];//对应于上下左右四个按键
	int m_keyTimer[4];	//四个按键的计时
	int m_keyCurTime[4];	//四个按键的当前时间间隔
	int m_keyMaxTime;	//按键的最长响应时间
	int m_keyMinTime;	//按键的最短响应时间

	XKeyState m_ctrlKeyState;	//ctrl按键的案件状态
public:
	XBool init(XResPos resPos = RES_SYS_DEF);
	int addAObject(XObjectBasic * object);			//注册一个物件,返回注册的ID，实际上是序列编号，-1为注册失败
	void decreaseAObject(unsigned int objectID);							//去除一个物件的注册
	void decreaseAObject(const void * object);					//去除一个物件的注册
	int findObjectID(const void * object) const;		//获取指定物件的ID -1表示该物件没有注册
	XObjectType getObjectType(unsigned int objectID) const;	//获取物件的类型
	void *getObject(unsigned int objectID) const;				//获取物件的指针

	void mouseProc(const XVec2& p, XMouseState eventType);	//物件对鼠标事件的响应
	void keyProc(int keyID, XKeyState keyState);		//keyState:0按下，1弹起
	int getCurObjectSum() const;				//获取当前注册的物件数量
	int getCurMouseOnObjectSum() const;			//获取当前处于鼠标On状态下的物件数量
	int getCurChooseObjectSum() const;			//获取当前被选中的物件数量
	void getCurChooseObjectID(int *ID) const;	//获取当前被选择的所有物件的ID序列

	void draw();			//描绘物件的一些信息
	void move(float delay);	//动作
private:	//2014.4.18这里增加实时显示鼠标位置
	XFontUnicode *m_mousePosFont;	//实时显示鼠标位置的字符串
};
#if WITH_INLINE_FILE
#include "XObjectManager.inl"
#endif
}
#endif