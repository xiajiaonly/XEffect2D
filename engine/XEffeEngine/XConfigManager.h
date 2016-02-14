#ifndef _JIA_XCONFIGMANAGER_
#define _JIA_XCONFIGMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "XOprateDes.h"
#include <vector>
#include "XControl\XGroup.h"
#include "XInputEventCommon.h"
namespace XE{
//目前是简单实现，之后需要丰富和强化，具体强化内容，参看设计说明
/*统一配置类细节设计：
1、每个配置项有ID
2、分组有组名
3、如果没组名则为基组
4、可以保存配置控件位置信息
5、可以自动排版
6、根据窗口尺寸自动适应
7、设置项数值的改变允许指针和回调两种方式
8、允许组建动态插入
9、数据类型，取值范围，默认值，当前值，名称，数值变化类型（线性或者其他），是否可变，关联bool型配置值
10、允许插入占位物件(空OBJ)以便于在这些地方显示图片或者其他的东西。

11、增加微调按钮
*/
//这是一个统一风格配置界面的类，类负责所有参数的图形化配置和保存读取，默认值回溯操作
//网络传输格式说明:
//GroupSum：
//Group0NameLen|Group0Name|
//GroupItemsSum|
//ItemID|ItemType|ItemRangeMin|ItemRangeMax|ItemDefValue|ItemCurValue|ItemNameLen|ItemName|
//.......
//if(ItemType == CFG_DATA_TYPE_CUSTOM)
//ItemID|ItemType|CItemSum|CItem0ID|CItem0Type|........
enum XConfigDataType
{
	CFG_DATA_TYPE_INT,
	CFG_DATA_TYPE_CHAR,
	CFG_DATA_TYPE_UCHAR,
	CFG_DATA_TYPE_FLOAT,
	CFG_DATA_TYPE_XBOOL,	//注意这个对应XBool类型，而不是bool类型，否则将会有可能造成错误
	CFG_DATA_TYPE_RADIOS,	//单选框
	CFG_DATA_TYPE_CUSTOM,	//自定义		网络不支持
	CFG_DATA_TYPE_XSPRITE,	//精灵的物件	网络不支持
	CFG_DATA_TYPE_NULL,		//无效的配置项
};

#define CFG_MANAGER_W_SPACE (5.0f)
#define CFG_MANAGER_H_SPACE (5.0f)
#define CFG_MNG_H_FONT	(32.0f)
#define CFG_MNG_H_SLD	(24.0f)
//一个配置项的结构
union XConfigValue
{
	int valueI;
	float valueF;
	XBool valueB;
};
#define CFG_DEFAULT_FILENAME "xConfig.dat"
#define CFG_INJECT_FILENAME "xCFGInject.dat"
#define CFG_DEFAULT_GROUPNAME "Default"
//用户自定义控件的基类
//如果用户需要自定义可被配置管理其管理的类，那么必须基于这个基类并实现相关接口
class XCFGItemBasic:public XObjectBasic,public XBasicOprate
{
public:
	virtual bool save(FILE *fp) = 0;	//保存当前数据
	virtual bool load(FILE *fp) = 0;	//读取当前设置
	virtual bool update() = 0;			//使得内部数据能体现在UI界面上
	virtual bool defValue() = 0;		//取默认值
	virtual bool enable() = 0;			//使能其可以被修改控制
	virtual bool disable() = 0;			//不能被控制

	virtual ~XCFGItemBasic() {}
	//为了支持网络而提供的接口
	virtual unsigned char *getInfo(int &len,int id) = 0;					//返回自身网络同步的字符串，len：内容
	virtual void setValueFromStr(unsigned char * str) = 0;	//从网络发送的字符串中提取数据
	virtual bool needSendStr() = 0;							//返回是否需要发送状态到客户端
	virtual unsigned char *sendStr(int &len,int id) = 0;					//内部数据中生成可以发送的数据
};
class XConfigItem
{
private:
	int m_ID;
public:
	bool m_isEnable;
	bool m_isActive;	//当前设置项是否是主动地，如果是被动的，则其值不能被外部改变
	XConfigDataType m_type;		//配置项的类型
	XConfigValue m_rangeMin;		//取值范围
	XConfigValue m_rangeMax;	
	XConfigValue m_defaultValue;	//默认值
	XConfigValue m_curValue;		//当前值
	void * m_pVariable;				//目标的指针(唯一)
	//void (* m_changeProc)();		//发生变化的回调函数
	void (* m_changeProc)(void *pObj,void *pClass);		//发生变化的回调函数
	void *m_pClass;
	XControlBasic *m_pCtrl;		//控件的指针
	std::string m_name;					//排位置项的名称(唯一)
	XCFGItemBasic * m_customIt;	//用户自定义的配置项(唯一)
	void release();	//注意：这个函数不能放在析构函数中因为并不是所有的情况都需要释放这个资源
	XConfigItem()
		:m_isEnable(false)
		,m_pVariable(NULL)
		,m_changeProc(NULL)
		,m_pClass(NULL)
		,m_pCtrl(NULL)
		,m_isActive(true)
		,m_customIt(NULL)
	{
		static int ID = 0;
		m_ID = ID ++;
	}
	~XConfigItem(){XMem::XDELETE(m_pCtrl);}
	int getID()const {return m_ID;}
	void setID(int ID){m_ID = ID;}
};
class XConfigGroup
{
private:
	void moveDownPretreatment(int pixelW,int pixelH);	//移动的预处理，在移动之前处理一些事情
	void moveDownPretreatment(const XVector2 &size){moveDownPretreatment(size.x,size.y);}
	void moveDown(int pixelW,int pixelH);	//向下移动插入点
	void moveDown(const XVector2 &size){moveDown(size.x,size.y);}
	void useANewRow();//使用新的一列
public:
	bool m_isEnable;		//组是否有效
	bool m_isMaxH;	//是否使用最大高度
	bool m_isNewRow;						//是否是新的一列
	XVector2 m_maxSize;	//包围盒子的大小：这个数值是没有经过缩放的像素大小
	XVector2 m_position;	//位置
	XVector2 m_scale;		//大小
	std::string m_name;			//组名
	XGroup m_group;
	//std::vector<int> m_itemsID;	//子组件的ID
	std::vector<XConfigItem *> m_items;	//子组件的指针
	XVector2 m_curInsertPos;	//当前插入的位置
	void relayout();	//重新自动布局
	float m_width;
	float m_maxHeight;
	float m_maxRowWidth;					//当前列的最大列宽
	XVector2 calculateMaxSize();	//计算当前组的范围

	XConfigGroup()
		:m_isEnable(false)
		,m_name(CFG_DEFAULT_GROUPNAME)
		,m_position(0.0f,0.0f)
		,m_scale(1.0f,1.0f)
	{}
};
enum XConfigMode
{
	CFG_MODE_NORMAL,	//普通模式
	CFG_MODE_SERVER,	//服务器模式
	CFG_MODE_CLIENT,	//客户端模式
};
enum XConfigNetOperate
{
	CFG_NET_OP_SAVE,	//保存
	CFG_NET_OP_LOAD,	//读取
	CFG_NET_OP_DEFAULT,	//默认
};
class XNetClient;
class XNetServer;
class XButton;
class XConfigManager//:public XDragObjectBase
{
public:
	static const int m_cfgMaxItemsSum = 10000;
public:
	XConfigManager();
	virtual ~XConfigManager();
protected:
	XConfigManager(const XConfigManager&);
	XConfigManager &operator= (const XConfigManager&);
public:
	static bool m_withConfigManager;	//是否使用了配置管理器
	void setWithBackground(bool flag)
	{
		m_withBackground = flag;
		for(unsigned int i = 0;i < m_pGroups.size();++ i)
		{
			m_pGroups[i]->m_group.setWithRect(!flag);
		}
	}
	bool getWithBackground()const{return m_withBackground;}
private:
	bool m_isInited;
	bool m_isVisble;	//是否可见
	bool m_isNewRow;						//是否是新的一列
	bool m_withBackground;					//是否显示背景
	std::vector<XConfigItem *> m_pItems;	//所有配置项的列表
	std::vector<XConfigGroup *> m_pGroups;	//组的列表

	//friend void callbackProcMD(void *pClass,int ID);	//鼠标按下事件的回调函数
	static void ctrlProc(void*,int,int);

	XVector2 m_position;
	XVector2 m_scale;
	float m_maxHeight;	//高度，这里只限制高度
	float m_width;		//配置项的宽度
	XVector2 m_curInsertPos;	//目前插入所在的位置
	XFColor m_textColor;	//字体的颜色

	XButton *m_saveBtn;
	XButton *m_loadBtn;
	XButton *m_defaultBtn;
	XButton *m_undoBtn;
	XButton *m_redoBtn;
	XButton *m_netUpdateBtn;	//从服务器同步配置信息
	XButton *m_netInjectBtn;	//向服务器注入配置数据

	void relayout();	//重新自动布局
	void release();
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//客户端的功能按键说明:
	//保存：要求服务器端保存数据，自身也保存数据			完成
	//读取：要求服务器端读取数据，并将数据同步到客户端		完成
	//默认：要求服务器端恢复默认数据，并将数据同步到客户端	完成
	//注入：客户端读取数据，并将数据同步到服务器端			完成
	//下面是网络相关的定义
	XConfigMode m_configMode;
	XNetClient *m_netClient;
	XNetServer *m_netServer;

	unsigned char *getConfigInfo(int &len);			//将配置类的结构转换成字符串
	void setOperateToServer(XConfigNetOperate op);	//客户端向服务器发送操作命令
	void sendSynchToServer();						//向服务器发送同步请求
	void sendCFGInfo();
	void sendItemValue(const XConfigItem * it);
	void sendInject();	//向服务器发送注入信息
	void updateNet();
	void updateInfo(unsigned char *data);
	void setItemValue(int ID,const XConfigValue value);
	void updateItemToCFG(XConfigItem * it);		//从内部数据更新到界面
	void updateItemFromCFG(XConfigItem * it);		//从界面更新到内部数据

	//从字符串中建立一个Item,返回这个item的指针和这个item原来的ID
	bool createAItemFromStr(const unsigned char * str,int &offset,unsigned char *groupName,
		std::vector<XConfigItem *> *itemsList,std::vector<int> *itemsIDListD);
//-----------------------------------------------------
private:
	float m_maxRowWidth;					//当前的最大列宽
	void moveDown(int pixelW,int pixelH);	//更具偏移量计算下一个插入点
	void useANewRow();//使用新的一列
	void relayoutGroup(int index,bool flag = true);	//flag用于标记Group是否需要重新排版
public:
	bool init(XConfigMode mode = CFG_MODE_NORMAL);
	void update();	//将被动组件的内部数据在UI界面上体现
	void draw();
	void setItemActive(bool isActive,void * p);	//设置某个配置项是否为主动形式(默认全部为主动形式)
	void setPosition(float x,float y);
	void setScale(float s){ setScale(s, s); }
	void setScale(float x,float y);
	void setTextColor(const XFColor& color);	//设置字体的颜色
	XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色
	void setMaxHeight(float h);
	void setVisible();	//设置显示
	void disVisible();	//设置不显示
	bool getIsVisble()const{return m_isVisble;}

	template<typename T>
	int addAItem(T *p,XConfigDataType type,const char * name,
		T max,T min,T def,
		void (* changeProc)(void *,void *) = NULL,const char * groupName = NULL,void *pClass = NULL);	//返回ID，-1为失败
	int addCustomItem(XCFGItemBasic *it,const char * groupName);
	int addSpecialItem(void * it,XConfigDataType type,const char * groupName);
	bool isSpecialItemExist(void *it);

	bool isItemExist(void * p);					//检查配置项是否已经存在
	bool isCustomItemExist(XCFGItemBasic *it);

private:
	bool getItemValueFromStr(XConfigItem *it,const char *str);	//从字符串中读取指定配置项的数据
public:
	bool save(const char *filename = NULL);
	bool load(const char *filename = NULL){return loadEx(filename);}
	bool loadEx(const char *filename = NULL);	//加强版的读取，如果出现文件错误依然最大限度的读取数据
	void setDefault();	//恢复默认值
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了实现鼠标拖动而定义的变量
private:
	bool m_isMouseDown;		//是否鼠标按下
	XVector2 m_mousePos;	//鼠标按下的位置
public:
	XBool mouseProc(float x,float y,XMouseState mouseState);		//对于鼠标动作的响应函数
	XBool inputProc(const XInputEvent &input)
	{
		if(!m_isInited) return XFalse;
		if(input.type == EVENT_MOUSE)
			return mouseProc(input.mouseX,input.mouseY,input.mouseState);
		return XFalse;
	}
	//------------------------------------------------------------
	XConfigItem *getItemByID(int ID);			//通过ID获取配置项的指针
	XConfigItem *getItemByVariable(void *p);	//通过变量指针获得配置项的指针
	XConfigItem *getItemByName(const char *name,int start = 0);	//通过配置项的名称查找配置项，如果存在同名的配置项，则返回第一个

	bool decreaseAItem(void *p);	//减少一个配置项：尚未经过测试
	bool clear();					//删除所有的群组和配置项
	//下面的接口都尚未实现
	//bool setCurValue(int ID);	//设置指定项的当前值
	//bool addAItem(XConfigItem * it);	//增加一个配置项

	//下面是与组相关的操作
	bool addGroup(const char * name);
	bool renameGroup(const char *oldName,const char *newName);	//改变组的名字
	bool isGroupExist(const char * name);	//判断组件是否存在
	XConfigGroup *getGroup(const char * name);
};
#if WITH_INLINE_FILE
#include "XConfigManager.inl"
#endif
}
#endif