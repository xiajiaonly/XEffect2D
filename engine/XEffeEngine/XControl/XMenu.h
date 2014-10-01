#ifndef _JIA_XMENU_
#define _JIA_XMENU_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.7.4
//--------------------------------
#include "XControlBasic.h"
#include "../XBasicWindow.h"

//开始进行这部分的工作，控制功能尚未实现
//修改建议
//1、二级以上菜单文字左对齐				*
//2、有子菜单的菜单项需要有特殊标记		*
//3、可以支持状态类菜单项				*
//4、支持图标							*
//5、支持无效菜单项						*
//6、支持快捷键操作等键盘操作			* (部分完成)
//7、支持菜单超出窗口边界时的自适应处理
//8、支持动态修改菜单项的显示文字		*
//9、优化代码提升效率
//10、弹出窗口的配置项有额外标记
//11、增加分隔符						*
//12、考虑工具栏的实现					+已经有方案
//13、当菜单超长时增加菜单上下翻滚支持
//14、菜单样式加个边框

#define MAX_MENU_TEXT_LENGTH (1024)	//菜单中文字的最大长度
enum _XMenuType
{//菜单的类型
	MENU_TYPE_VERTICAL,		//垂直的
	MENU_TYPE_HORIZONTA,	//水平的
};
class _XMenuItem;
class _XMenuGroup	//一个菜单组
{
public:
	_XVector2 m_insertPos;	//插入菜单项的位置
	int m_width;				//组的宽度
	_XMenuType m_type;
	_XMenuItem *m_parent;		//父物体的指针
	std::vector<_XMenuItem *> m_items;
	void draw();
	_XMenuGroup()
		:m_width(-1)	//根据字体长度自行确定
		,m_type(MENU_TYPE_HORIZONTA)
		,m_parent(NULL)
		,m_chooseIndex(-1)
	{}
	~_XMenuGroup()
	{
		for(int i = 0;i < m_items.size();++ i)
		{
			XDELETE(m_items[i]);
		}
		m_items.clear();
	}
	void setOpen();	//设置打开
	void disOpen();
	_XBool isInRect(float x,float y);
	_XMenuItem * getItem(float x,float y);	//获取当前在x，y位置的物件的指针
	_XMenuItem * getItemByNameInGroup(const std::string &name);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);
	void postPross();
	void updatePos();	//更新组的位置变化
	void updateSize();	//更新组的缩放变化
	void updateColor();
	void setHotChooseActive(_XBool flag);
	_XMenuItem *getItemByHotChooseKey(char k);
	_XMenuItem *getItemByHotKey(char k);
	void setAllCallBackFun(void (*callbackFun)(void *,const std::string &),void *p);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是左右上下键控制的代码
	int m_chooseIndex;
	void addIndex();//增加编号	
	void desIndex();	//减少编号
	void initIndex(_XBool orderFlag = XTrue);	//初始化编号 orderFlag是否正序初始化
	void resetChoose();	//取消选择
	void openChoose();	//打开选择的项
	void closeChoose();	//关闭选择的项
	_XBool setChooseItem(const _XMenuItem *it);
	_XMenuItem *getChooseItem();	//获取当前组中被选择的菜单项
	//-----------------------------------------------------
};
enum _XMenuItemType
{//尚未完成
	MENU_ITEM_TYPE_BTN,	//按键类的菜单项
	MENU_ITEM_TYPE_CHK,	//选择类的菜单项
	MENU_ITEM_TYPE_DLT,	//分隔符类的菜单项
};
class _XMenuItem	//一个菜单项
{
public:
	int m_levelIndex;		//所处的层次的编号
	_XVector2 m_position;	//放的位置
	_XVector2 m_scaleSize;	//缩放大小
	_XVector2 m_size;		//像素尺寸大小
	_XBool m_isOpen;	//是否处于打开状态
	_XMenuGroup *m_owner;			//所属的组
	_XMenuGroup *m_child;			//子物体的指针
	std::string m_originalName;		//原始的名称，在组内具有唯一性,最大长度受到MAX_MENU_TEXT_LENGTH限制
	std::string m_showName;			//显示的名字
	_XMenuItemType m_type;			//菜单项的类型
	_XBool m_isChecked;				//是否被点击	//选择类的控件才能设置这个选项
	_XFColor m_color;
	//void changeCheckedState();		//改变选择状态
	void setCheckState(_XBool state);	//设置选择类菜单项的选择状态
	void setString(const std::string &name);
	void setPosition(float x,float y);
	void setSize(float x,float y);
	void setColor(const _XFColor &c);
	//++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了剔除对按钮的使用而定义的变量
	_XFontUnicode m_font;			//显示的字体
	_XBool m_isEnable;				//是否有效状态
	_XBool m_isVisible;				//是否可见
	_XBool m_isMouseOn;				//是否处于鼠标悬浮状态
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//返回是否触发的事件
	//----------------------------------------------
	//++++++++++++++++++++++++++++++++++++++++++++++
	//下面加入对icon的支持
	_XBool m_withIcon;	//是否有图标的支持
	_XSprite m_spriteIcon;	//icon的贴图
	_XBool setIcon(const char *filename,_XResourcePosition res = RESOURCE_SYSTEM_DEFINE);
	void disIcon();	//取消Icon
	//----------------------------------------------
	//++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了实现键盘操作而定义的变量：2014年7月30日 注意：由于字体太大，显示控件不足不能支持热键操作
	char m_hotKey;			//当前菜单项的快捷键,ctrl + m_hotkey使得操作直接生效
	char m_hotChooseKey;	//按下alt之后按下m_hotChooseKey激活当前菜单
	_XBool m_isHotChooseActive;
	_XBool setHotKey(char k);	//设置快捷键//-1为无效
	_XBool setHotChooseKey(char k);	//设置快选键//-1为无效
	_XMenuItem *getItemByHotKey(char k);//通过快捷键来查找菜单项
	_XMenuItem *getItemByHotChooseKey(char k);//从子菜单中寻找指定的热键的菜单项
	void setHotChooseActive(_XBool flag);//设置热键激活状态
	//----------------------------------------------
private:
	void (*m_callbackFun)(void *pClass,const std::string &pos);
	void *m_pClass;
public:
	_XMenuItem()
		:m_owner(NULL)
		,m_child(NULL)
		,m_isOpen(XFalse)
		,m_position(0.0f,0.0f)
		,m_scaleSize(1.0f,1.0f)
		,m_size(0.0f,0.0f)
		,m_callbackFun(NULL)
		,m_pClass(NULL)
		,m_isEnable(XTrue)
		,m_isVisible(XTrue)
		,m_isMouseOn(XFalse)
		,m_levelIndex(0)
		,m_type(MENU_ITEM_TYPE_BTN)
		,m_withIcon(XFalse)
		,m_hotKey(-1)
		,m_hotChooseKey(-1)
		,m_isHotChooseActive(XFalse)
		,m_color(1.0f,1.0f,1.0f,1.0f)
	{}
	~_XMenuItem(){XDELETE(m_child);}
	void draw();
	void setCallbackFun(void (*callbackFun)(void *,const std::string &),void *p);	//设置回调函数，当菜单项被触发时会调用相关的回调函数
	void setAllCallBackFun(void (*callbackFun)(void *,const std::string &),void *p);	//设置所有的子物体的回调函数
	void beTriggered();	//菜单项被触发时调用这个函数
	std::string getFullStr() const;	//获取当前菜单项的全路径

	_XBool init(const std::string &name,const _XFontUnicode &font,float fontsize,_XMenuGroup *owner,_XMenuItemType type = MENU_ITEM_TYPE_BTN);
	void setOpen();	//设置打开
	void disOpen();	//设置关闭
	_XBool isInRect(float x,float y);
	_XMenuItem * getItem(float x,float y);
	void postPross();
	void updateStr();	//更新字符串的显示
	void resetChoose();
};
class _XMenu:public _XControlBasic
{
private:
	_XBool m_isInited;
	_XMenuItem m_rootItem;	//根项
	_XMenuType m_type;		//根菜单的类型

	_XFontUnicode m_font;	//字体
	float m_fontSize;
//	_XVector2 m_position;	//位置
//	_XVector2 m_size;
	_XBool m_isClicked;		//是否被点击
	_XMenuItem *m_oldChooseItem;	//之前选择的选项
	_XBool m_needPostProcess;	//	是否需要后期处理

	void postProcess();	//后期处理的过程用于状态发生改变之后修正改变的状态
public:
	//使用";"作为菜单项的分割符
	_XBool addAItem(const std::string &pos,const std::string &name,int width = -1,_XMenuItemType type = MENU_ITEM_TYPE_BTN);
	_XBool getCheckState(const std::string &pos);
	_XBool setCheckState(const std::string &pos,_XBool state);
	_XBool setDisable(const std::string &pos);
	_XBool setEnable(const std::string &pos);
	_XBool getEnable(const std::string &pos);
	_XBool setMenuItemStr(const std::string &pos,const std::string &name);	//修改菜单项的名字，组中菜单项的名字必须要唯一，否则会造成逻辑问题
	_XBool setItemIcon(const std::string &pos,const char *filename,_XResourcePosition res = RESOURCE_SYSTEM_DEFINE);
	_XBool setHotKey(const std::string &pos,char k);
	char getHotKey(const std::string &pos);
	_XBool setHotChooseKey(const std::string &pos,char k);
	char getHotChooseKey(const std::string &pos);
	_XMenuItem * getItemByName(const std::string &name);
	std::string getItemFullStr(const _XMenuItem * it);
private:
	_XMenuItem * getItemFromGroup(char *p,_XMenuGroup *g);	//在组g中查找名称为p的菜单项
	_XBool getIsPath(_XMenuItem *s,_XMenuItem *d);	//判断s是否在d的路径上

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对键盘操作的一些定义
	_XBool m_hotChooseKeyIsActive;	//是否激活热键
	//------------------------------------------------------
	void closeAll();	//折叠所有的菜单项
public:
	_XBool setCBFunction(const std::string &pos,void (*callbackFun)(void *,const std::string &),void *p);//设置菜单响应的回调函数
	_XBool setAllCBFunction(void (*callbackFun)(void *,const std::string &),void *p);	//设置所有项的回调函数

	_XBool initWithoutTex(const _XFontUnicode &font,float captionSize = 1.0f,_XMenuType type = MENU_TYPE_HORIZONTA);
	_XBool initWithoutTex(_XMenuType type = MENU_TYPE_HORIZONTA) {return initWithoutTex(XEE::systemFont,1.0f,type);}
protected:
	void draw();
	void drawUp(){};							//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);

	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);				//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点
public:
	_XMenu()
		:m_isInited(XFalse)
		,m_isClicked(XFalse)
		,m_oldChooseItem(NULL)
		,m_needPostProcess(XFalse)
		,m_hotChooseKeyIsActive(XFalse)
		,m_type(MENU_TYPE_HORIZONTA)
	{
		m_ctrlType = CTRL_OBJ_XMENU;
	}
	~_XMenu(){release();}
	void release();
	//为了与其他的控件接轨，还需要实现下面的接口
	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//避免覆盖的问题
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color);	//设置字体的颜色
	_XFColor getTextColor() const {return _XFColor::white;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	_XBool isInRect(float x,float y) {return m_rootItem.isInRect(x,y);}		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	////virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	_XMenu(const _XMenu &temp);
	_XMenu& operator = (const _XMenu& temp);

};
#include "XMenu.inl"
#endif