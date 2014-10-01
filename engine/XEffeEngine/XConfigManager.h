#ifndef _JIA_XCONFIGMANAGER_
#define _JIA_XCONFIGMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "XEffeEngine.h"
#include "XMedia/XNetServer.h"
#include "XMedia/XNetClient.h"
#include <vector>
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
//ItemID|ItemType|ItemRangeMin|ItemRangeMax|ItemDefValue|ItemNowValue|ItemNameLen|ItemName|
//.......
//if(ItemType == CFG_DATA_TYPE_CUSTOM)
//ItemID|ItemType|CItemSum|CItem0ID|CItem0Type|........
enum _XConfigDataType
{
	CFG_DATA_TYPE_INT,
	CFG_DATA_TYPE_CHAR,
	CFG_DATA_TYPE_UCHAR,
	CFG_DATA_TYPE_FLOAT,
	CFG_DATA_TYPE_XBOOL,	//注意这个对应_XBool类型，而不是bool类型，否则将会有可能造成错误
	CFG_DATA_TYPE_RADIOS,	//单选框
	CFG_DATA_TYPE_CUSTOM,	//自定义		网络不支持
	CFG_DATA_TYPE_XSPRITE,	//精灵的物件	网络不支持
	CFG_DATA_TYPE_NULL,		//无效的配置项
};

#define CFG_MANAGER_W_SPACE (5.0f)
#define CFG_MANAGER_H_SPACE (5.0f)
#define CFG_MNG_H_FONT	(32.0f)
#define CFG_MNG_H_SLD	(24.0f)
#define CFG_MAX_ITEMS_SUM (10000)
//一个配置项的结构
union _XConfigValue
{
	int valueI;
	float valueF;
	_XBool valueB;
};
#define CFG_DEFAULT_FILENAME ("xConfig.dat")
#define CFG_INJECT_FILENAME ("xCFGInject.dat")
#define CFG_DEFAULT_GROUPNAME ("Default")
//用户自定义控件的基类
//如果用户需要自定义可被配置管理其管理的类，那么必须基于这个基类并实现相关接口
class _XCFGItemBasic:public _XObjectBasic,public _XBasicOprate
{
public:
	virtual bool save(FILE *fp) = 0;	//保存当前数据
	virtual bool load(FILE *fp) = 0;	//读取当前设置
	virtual bool update() = 0;			//使得内部数据能体现在UI界面上
	virtual bool defValue() = 0;		//取默认值
	virtual bool enable() = 0;			//使能其可以被修改控制
	virtual bool disable() = 0;			//不能被控制

	virtual ~_XCFGItemBasic() {}
	//为了支持网络而提供的接口
	virtual unsigned char *getInfo(int &len,int id) = 0;					//返回自身网络同步的字符串，len：内容
	virtual void setValueFromStr(unsigned char * str) = 0;	//从网络发送的字符串中提取数据
	virtual bool needSendStr() = 0;							//返回是否需要发送状态到客户端
	virtual unsigned char *sendStr(int &len,int id) = 0;					//内部数据中生成可以发送的数据
};
class _XConfigItem
{
private:
	int m_ID;
public:
	bool m_isEnable;
	_XConfigDataType m_type;		//配置项的类型
	_XConfigValue m_rangeMin;		//取值范围
	_XConfigValue m_rangeMax;	
	_XConfigValue m_defaultValue;	//默认值
	_XConfigValue m_nowValue;		//当前值
	void * m_pVariable;				//目标的指针(唯一)
	void (* m_changeProc)();		//发生变化的回调函数
	_XControlBasic *m_pCtrl;		//控件的指针
	string m_name;					//排位置项的名称(唯一)
	bool m_isActive;	//当前设置项是否是主动地，如果是被动的，则其值不能被外部改变
	_XCFGItemBasic * m_customIt;	//用户自定义的配置项(唯一)

	_XConfigItem()
		:m_isEnable(false)
		,m_pVariable(NULL)
		,m_changeProc(NULL)
		,m_pCtrl(NULL)
		,m_isActive(true)
		,m_customIt(NULL)
	{
		static int ID = 0;
		m_ID = ID ++;
	}
	~_XConfigItem(){XDELETE(m_pCtrl);}
	int getID()const {return m_ID;}
	void setID(int ID){m_ID = ID;}
};
class _XConfigGroup
{
private:
	void moveDownPretreatment(int pixelW,int pixelH);	//移动的预处理，在移动之前处理一些事情
	void moveDownPretreatment(const _XVector2 &size){moveDownPretreatment(size.x,size.y);}
	void moveDown(int pixelW,int pixelH);	//向下移动插入点
	void moveDown(const _XVector2 &size){moveDown(size.x,size.y);}
	void useANewRow()
	{//使用新的一列
		m_nowInsertPos.x += (m_maxRowWidth + CFG_MANAGER_W_SPACE) * m_size.x;
		m_nowInsertPos.y = m_position.y + (32.0f + CFG_MANAGER_H_SPACE) * m_size.y;
		m_isNewRow = true;
	}
public:
	bool m_isEnable;		//组是否有效
	_XVector2 m_maxSize;	//包围盒子的大小：这个数值是没有经过缩放的像素大小
	_XVector2 m_position;	//位置
	_XVector2 m_size;		//大小
	string m_name;			//组名
	_XGroup m_group;
	//std::vector<int> m_itemsID;	//子组件的ID
	std::vector<_XConfigItem *> m_items;	//子组件的指针
	_XVector2 m_nowInsertPos;	//当前插入的位置
	void relayout();	//重新自动布局
	float m_width;
	float m_maxHeight;
	bool m_isMaxH;	//是否使用最大高度
	float m_maxRowWidth;					//当前列的最大列宽
	bool m_isNewRow;						//是否是新的一列
	_XVector2 calculateMaxSize();	//计算当前组的范围

	_XConfigGroup()
		:m_isEnable(false)
		,m_name(CFG_DEFAULT_GROUPNAME)
		,m_position(0.0f,0.0f)
		,m_size(1.0f,1.0f)
	{}
};
enum _XConfigMode
{
	CFG_MODE_NORMAL,	//普通模式
	CFG_MODE_SERVER,	//服务器模式
	CFG_MODE_CLIENT,	//客户端模式
};
enum _XConfigNetOperate
{
	CFG_NET_OP_SAVE,	//保存
	CFG_NET_OP_LOAD,	//读取
	CFG_NET_OP_DEFAULT,	//默认
};
class _XConfigManager
{
protected:
	_XConfigManager()
		:m_isInited(false)
		,m_isVisble(true)
		,m_position(0.0f,0.0f)
		,m_size(1.0,1.0f)
		,m_maxHeight(300.0f)
		,m_width(256.0f)
		,m_nowInsertPos(0.0f,0.0f)
		,m_configMode(CFG_MODE_NORMAL)
//		,m_minuteIndex(-1)
	//	,m_operateIndex(-1)	//没有动作
		,m_textColor(0.0f,0.0f,0.0f,1.0f)
		,m_isMouseDown(false)
	{}
	_XConfigManager(const _XConfigManager&);
	_XConfigManager &operator= (const _XConfigManager&);
	virtual ~_XConfigManager(){release();}
public:
	static _XConfigManager& GetInstance()
	{
		static _XConfigManager m_instance;
		return m_instance;
	}
private:
	bool m_isInited;
	bool m_isVisble;	//是否可见
	std::vector<_XConfigItem *> m_pItems;	//所有配置项的列表
	std::vector<_XConfigGroup *> m_pGroups;	//组的列表

	friend void callbackProc(void *pClass,int ID);	//回调函数
	//friend void callbackProcMD(void *pClass,int ID);	//鼠标按下事件的回调函数

	_XVector2 m_position;
	_XVector2 m_size;
	float m_maxHeight;	//高度，这里只限制高度
	float m_width;		//配置项的宽度
	_XVector2 m_nowInsertPos;	//目前插入所在的位置
	_XFColor m_textColor;	//字体的颜色

	_XButton m_saveBtn;
	_XButton m_loadBtn;
	_XButton m_defaultBtn;
	_XButton m_undoBtn;
	_XButton m_redoBtn;
	_XButton m_netUpdateBtn;	//从服务器同步配置信息
	_XButton m_netInjectBtn;	//向服务器注入配置数据

	void relayout();	//重新自动布局
	void release();
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//客户端的功能按键说明:
	//保存：要求服务器端保存数据，自身也保存数据			完成
	//读取：要求服务器端读取数据，并将数据同步到客户端		完成
	//默认：要求服务器端恢复默认数据，并将数据同步到客户端	完成
	//注入：客户端读取数据，并将数据同步到服务器端			完成
	//下面是网络相关的定义
	_XConfigMode m_configMode;
	_XNetClient m_netClient;
	_XNetServer m_netServer;

	unsigned char *getConfigInfo(int &len);			//将配置类的结构转换成字符串
	void setOperateToServer(_XConfigNetOperate op);	//客户端向服务器发送操作命令
	void sendSynchToServer();						//向服务器发送同步请求
	void sendCFGInfo();
	void sendItemValue(const _XConfigItem * it);
	void sendInject();	//向服务器发送注入信息
	void updateNet();
	void updateInfo(unsigned char *data);
	void setItemValue(int ID,const _XConfigValue value);
	void updateItemToCFG(_XConfigItem * it);		//从内部数据更新到界面
	void updateItemFromCFG(_XConfigItem * it);		//从界面更新到内部数据

	//从字符串中建立一个Item,返回这个item的指针和这个item原来的ID
	bool createAItemFromStr(const unsigned char * str,int &offset,unsigned char *groupName,
		std::vector<_XConfigItem *> *itemsList,std::vector<int> *itemsIDListD);
//-----------------------------------------------------
private:
	float m_maxRowWidth;					//当前的最大列宽
	bool m_isNewRow;						//是否是新的一列
	void moveDown(int pixelW,int pixelH)	//更具偏移量计算下一个插入点
	{
		m_nowInsertPos.y += (pixelH + CFG_MANAGER_H_SPACE) * m_size.y;
		if(m_isNewRow || pixelW > m_maxRowWidth) m_maxRowWidth = pixelW;
		m_isNewRow = false;
		if(m_nowInsertPos.y >= m_position.y + m_maxHeight)
		{//这里需要换列
			useANewRow();
		}
	}
	void useANewRow()
	{//使用新的一列
		m_nowInsertPos.x += (m_maxRowWidth + CFG_MANAGER_W_SPACE) * m_size.x;
		m_nowInsertPos.y = m_position.y + (32.0f + CFG_MANAGER_H_SPACE) * m_size.x;
		m_isNewRow = true;
	}
	void relayoutGroup(int index,bool flag = true);	//flag用于标记Group是否需要重新排版
public:
	bool init(_XConfigMode mode = CFG_MODE_NORMAL);
	void update();	//将被动组件的内部数据在UI界面上体现
	void draw();
	void setItemActive(bool isActive,void * p);	//设置某个配置项是否为主动形式(默认全部为主动形式)
	void setPosition(float x,float y)
	{
		if(!m_isInited) return;
		m_position.set(x,y);
		relayout();	//重新布局
	}
	void setSize(float x,float y)
	{
		if(!m_isInited) return;
		m_size.set(x,y);
		relayout();	//重新布局
	}
	void setTextColor(const _XFColor& color);	//设置字体的颜色
	_XFColor getTextColor() const {return m_textColor;}	//获取控件字体的颜色
	void setMaxHeight(float h)
	{
		if(h < 1) h = 1;
		m_maxHeight = h;
		relayout();
	}
	void setVisible();	//设置显示
	void disVisible();	//设置不显示
	bool getIsVisble(){return m_isVisble;}

	template<typename T>
	int addAItem(T *p,_XConfigDataType type,const char * name,
		T max,T min,T def,
		void (* changeProc)() = NULL,const char * groupName = NULL);	//返回ID，-1为失败
	int addCustomItem(_XCFGItemBasic *it,const char * groupName);
	int addSpecialItem(void * it,_XConfigDataType type,const char * groupName);
	bool isSpecialItemExist(void *it)
	{
		if(it == NULL) return false;
		for(int i = 0;i < m_pItems.size();++ i)
		{
			if(m_pItems[i]->m_pVariable == it) return true;
		}
		return false;
	}

	bool isItemExist(void * p);					//检查配置项是否已经存在
	bool isCustomItemExist(_XCFGItemBasic *it);

private:
	bool getItemValueFromStr(_XConfigItem *it,const char *str);	//从字符串中读取指定配置项的数据
public:
	bool save(const char *filename = NULL);
	bool load(const char *filename = NULL){return loadEx(filename);}
	bool loadEx(const char *filename = NULL);	//加强版的读取，如果出现文件错误依然最大限度的读取数据
	void setDefault();	//恢复默认值
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了实现鼠标拖动而定义的变量
private:
	bool m_isMouseDown;		//是否鼠标按下
	_XVector2 m_mousePos;	//鼠标按下的位置
public:
	_XBool mouseProc(float x,float y,_XMouseState mouseState)		//对于鼠标动作的响应函数
	{//尚未完成
		_XRect tmpRect(m_position.x,m_position.y,
			m_position.x + 32.0f * m_size.x,m_position.y + 32.0f * m_size.y);
		switch(mouseState)
		{
		case MOUSE_MOVE:
			if(m_isMouseDown)
			{//发生鼠标拖拽事件
				_XVector2 tmp = _XVector2(x,y) - m_mousePos;
				setPosition(m_position.x + tmp.x,m_position.y + tmp.y);
				m_mousePos.set(x,y);
				//if(!tmpRect.isInRect(x,y)) m_isMouseDown = false;
			}
			break;
		case MOUSE_LEFT_BUTTON_DOWN:
		case MOUSE_LEFT_BUTTON_DCLICK:
			if(tmpRect.isInRect(x,y))
			{
				m_isMouseDown = true;
				m_mousePos.set(x,y);
			}
			break;
		case MOUSE_LEFT_BUTTON_UP:
			m_isMouseDown = false;
			break;
		}
		return XFalse;
	}
	//------------------------------------------------------------
	_XConfigItem *getItemByID(int ID);			//通过ID获取配置项的指针
	_XConfigItem *getItemByVariable(void *p);	//通过变量指针获得配置项的指针
	_XConfigItem *getItemByName(const char *name,int start = 0);	//通过配置项的名称查找配置项，如果存在同名的配置项，则返回第一个

	bool decreaseAItem(void *p);	//减少一个配置项：尚未经过测试
	bool clear();					//删除所有的群组和配置项
	//下面的接口都尚未实现
	//bool setNowValue(int ID);	//设置指定项的当前值
	//bool addAItem(_XConfigItem * it);	//增加一个配置项

	//下面是与组相关的操作
	bool addGroup(const char * name);
	bool renameGroup(const char *oldName,const char *newName)	//改变组的名字
	{
		if(oldName == NULL || newName == NULL) return false;
		if(isGroupExist(newName)) return false;
		_XConfigGroup *gp = getGroup(oldName);
		if(gp == NULL) return false;
		gp->m_name = newName;
		gp->m_group.setCaptionText(newName);
		return true;
	}
	bool isGroupExist(const char * name);	//判断组件是否存在
	_XConfigGroup *getGroup(const char * name);
};

//将addBuff的数据接到baseBuff之后，如果内存空间不够则分配足够的内存空间
extern unsigned char *spliceData(unsigned char * baseBuff,int &baseLen,int &offset,const unsigned char * addBuff,int len);

template<typename T>
int _XConfigManager::addAItem(T *p,_XConfigDataType type,const char * name,
	T max,T min,T def,
	void (* changeProc)(),const char * groupName)	//返回ID，-1为失败
{
	if(type >= CFG_DATA_TYPE_CUSTOM || p == NULL) return -1;	//数据不合法
	if(p != NULL && isItemExist(p)) return -1; 
	_XConfigItem *pItem = createMem<_XConfigItem>();
	if(pItem == NULL) return -1;
	if(type == CFG_DATA_TYPE_CUSTOM) return -1;	//自定义控件不能使用这种方式添加
	if(name == NULL) pItem->m_name = " ";
	else pItem->m_name = name;
	pItem->m_changeProc = changeProc;
	char tempStr[1024];
	switch(type)
	{
	case CFG_DATA_TYPE_INT:
		pItem->m_defaultValue.valueI = (int)def;
		pItem->m_nowValue.valueI = * (int *)p;
		pItem->m_rangeMin.valueI = (int)min;
		pItem->m_rangeMax.valueI = (int)max;
		{
			_XSliderEx *pCtrl = createMem<_XSliderEx>();
			if(pCtrl == NULL) return -1;
			if(m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutTex(_XRect(0.0f,CFG_MNG_H_FONT,m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD),
				pItem->m_rangeMax.valueI,pItem->m_rangeMin.valueI,SLIDER_TYPE_HORIZONTAL,_XVector2(0.0,16.0f));
			pCtrl->setSize(m_size);
			pCtrl->setTextColor(m_textColor);
			if(name == NULL) pCtrl->setFontEx("%%.0f",XEE::systemFont,1.0f);
			else
			{
				sprintf(tempStr,"%s:%%.0f",name);
				pCtrl->setFontEx(tempStr,XEE::systemFont,1.0f);
			}

			pCtrl->setCallbackFun(NULL,NULL,NULL,NULL,NULL,callbackProc,callbackProc,this);
			pCtrl->setNowValue(* (int *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_TYPE_CHAR:
		pItem->m_defaultValue.valueI = (char)def;
		pItem->m_nowValue.valueI = * (char *)p;
		pItem->m_rangeMin.valueI = (char)min;
		pItem->m_rangeMax.valueI = (char)max;
		{
			_XSliderEx *pCtrl = createMem<_XSliderEx>();
			if(pCtrl == NULL) return -1;
			if(m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutTex(_XRect(0.0f,CFG_MNG_H_FONT,m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD),
				pItem->m_rangeMax.valueI,pItem->m_rangeMin.valueI,SLIDER_TYPE_HORIZONTAL,_XVector2(0.0,16.0f));
			pCtrl->setSize(m_size);
			pCtrl->setTextColor(m_textColor);
			if(name == NULL) pCtrl->setFontEx("%%.0f",XEE::systemFont,1.0f);
			else
			{
				sprintf(tempStr,"%s:%%.0f",name);
				pCtrl->setFontEx(tempStr,XEE::systemFont,1.0f);
			}

			pCtrl->setCallbackFun(NULL,NULL,NULL,NULL,NULL,callbackProc,callbackProc,this);
			pCtrl->setNowValue(* (char *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_TYPE_UCHAR:
		pItem->m_defaultValue.valueI = (unsigned char)def;
		pItem->m_nowValue.valueI = * (unsigned char *)p;
		pItem->m_rangeMin.valueI = (unsigned char)min;
		pItem->m_rangeMax.valueI = (unsigned char)max;
		{
			_XSliderEx *pCtrl = createMem<_XSliderEx>();
			if(pCtrl == NULL) return -1;
			if(m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutTex(_XRect(0.0f,CFG_MNG_H_FONT,m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD),
				pItem->m_rangeMax.valueI,pItem->m_rangeMin.valueI,SLIDER_TYPE_HORIZONTAL,_XVector2(0.0,16.0f));
			pCtrl->setSize(m_size);
			pCtrl->setTextColor(m_textColor);
			if(name == NULL) pCtrl->setFontEx("%%.0f",XEE::systemFont,1.0f);
			else
			{
				sprintf(tempStr,"%s:%%.0f",name);
				pCtrl->setFontEx(tempStr,XEE::systemFont,1.0f);
			}

			pCtrl->setCallbackFun(NULL,NULL,NULL,NULL,NULL,callbackProc,callbackProc,this);
			pCtrl->setNowValue(* (unsigned char *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_TYPE_FLOAT:
		pItem->m_defaultValue.valueF = (float)def;
		pItem->m_nowValue.valueF = * (float *)p;
		pItem->m_rangeMin.valueF = (float)min;
		pItem->m_rangeMax.valueF = (float)max;
		{
			_XSliderEx *pCtrl = createMem<_XSliderEx>();
			if(pCtrl == NULL) return -1;
			if(m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutTex(_XRect(0.0f,CFG_MNG_H_FONT,m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD),
				pItem->m_rangeMax.valueF,pItem->m_rangeMin.valueF,SLIDER_TYPE_HORIZONTAL,_XVector2(0.0,16.0f));
			pCtrl->setSize(m_size);
			pCtrl->setTextColor(m_textColor);
			if(name == NULL) pCtrl->setFontEx("%%.4f",XEE::systemFont,1.0f);
			else
			{
				sprintf(tempStr,"%s:%%.4f",name);
				pCtrl->setFontEx(tempStr,XEE::systemFont,1.0f);
			}

			pCtrl->setCallbackFun(NULL,NULL,NULL,NULL,NULL,callbackProc,callbackProc,this);
			//这里不能连接数据，如果连接数据的话将会造成数据变化的时候调用回调函数的时候数据已经改变而不会执行回调函数中的相关代码
			//pCtrl->setConnectVar((float *)p);
			pCtrl->setNowValue(* (float *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_TYPE_XBOOL:
		pItem->m_defaultValue.valueB = (_XBool)def;
		pItem->m_nowValue.valueB = *(_XBool *)p;
		pItem->m_rangeMin.valueB = (_XBool)min;
		pItem->m_rangeMax.valueB = (_XBool)max;
		{
			_XCheck *pCtrl = createMem<_XCheck>();
			if(pCtrl == NULL) return -1;
			if(m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			if(name == NULL) pCtrl->initWithoutTex(" ",XEE::systemFont,1.0f,_XRect(0.0f,0.0f,CFG_MNG_H_FONT,CFG_MNG_H_FONT),
				_XVector2(CFG_MNG_H_FONT,CFG_MNG_H_FONT * 0.5f));
			else pCtrl->initWithoutTex(name,XEE::systemFont,1.0f,_XRect(0.0f,0.0f,CFG_MNG_H_FONT,CFG_MNG_H_FONT),
				_XVector2(CFG_MNG_H_FONT,CFG_MNG_H_FONT * 0.5f));
			pCtrl->setSize(m_size);
			pCtrl->setCallbackFun(NULL,NULL,NULL,NULL,NULL,callbackProc,this);
			pCtrl->setConnectVar((_XBool *)p);
			pCtrl->setTextColor(m_textColor);
			if(*(_XBool *)p) pCtrl->setState(XTrue);
			else pCtrl->setState(XFalse);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_TYPE_RADIOS:
		pItem->m_defaultValue.valueI = (int)def;
		pItem->m_nowValue.valueI = *(int *)p;
		pItem->m_rangeMin.valueI = (int)min;
		pItem->m_rangeMax.valueI = (int)max;
		{
			_XRadios *pCtrl = createMem<_XRadios>();
			if(pCtrl == NULL) return -1;
			if(m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutTex(1,_XVector2(0.0f,CFG_MNG_H_FONT + 2.0f),_XRect(0.0f,0.0f,CFG_MNG_H_FONT,CFG_MNG_H_FONT),XEE::systemFont,1.0f,
				_XVector2(CFG_MNG_H_FONT + 2.0f,CFG_MNG_H_FONT * 0.5f));
			pCtrl->setRadiosText(name);
			pCtrl->setSize(m_size);
			pCtrl->setTextColor(m_textColor);
			pCtrl->setCallbackFun(callbackProc,this);
			pCtrl->setChoosed(*(int *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	}
	pItem->m_isEnable = true;
	pItem->m_pVariable = p;
	pItem->m_type = type;
	m_pItems.push_back(pItem);
	if(groupName == NULL)
	{//使用默认组
		_XConfigGroup *gp = m_pGroups[0];
		gp->m_items.push_back(pItem);
		if(pItem->m_pCtrl != NULL) 
		{
			gp->m_group.pushChild(pItem->m_pCtrl);
			if(gp->m_group.getState() == STATE_MINISIZE) pItem->m_pCtrl->disVisible();
		}
		relayout();
	}else
	{
		_XConfigGroup *gp = getGroup(groupName);
		if(gp != NULL)
		{
			gp->m_items.push_back(pItem);
			if(pItem->m_pCtrl != NULL) 
			{
				gp->m_group.pushChild(pItem->m_pCtrl);
				if(gp->m_group.getState() == STATE_MINISIZE) pItem->m_pCtrl->disVisible();
			}
			relayout();
		}
	}
	return pItem->getID();
}

#define _XCFGManager _XConfigManager::GetInstance()
#endif