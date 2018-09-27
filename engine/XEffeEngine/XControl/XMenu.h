#ifndef _JIA_XMENU_
#define _JIA_XMENU_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.7.4
//--------------------------------
#include "XControlBasic.h"
namespace XE{
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

enum XMenuType
{//菜单的类型
	MENU_TYPE_VERTICAL,		//垂直的
	MENU_TYPE_HORIZONTA,	//水平的
};
class XMenuItem;
class XMenuGroup	//一个菜单组
{
public:
	XVec2 m_insertPos;	//插入菜单项的位置
	int m_width;				//组的宽度
	XMenuType m_type;
	XMenuItem *m_parent;		//父物体的指针
	std::vector<XMenuItem *> m_items;
	void draw();
	XMenuGroup()
		:m_width(-1)	//根据字体长度自行确定
		,m_type(MENU_TYPE_HORIZONTA)
		,m_parent(NULL)
		,m_chooseIndex(-1)
	{}
	~XMenuGroup()
	{
		for(unsigned int i = 0;i < m_items.size();++ i)
		{
			XMem::XDELETE(m_items[i]);
		}
		m_items.clear();
	}
	void setOpen();	//设置打开
	void disOpen();
	XBool isInRect(const XVec2& p);
	XMenuItem * getItem(const XVec2& p);	//获取当前在x，y位置的物件的指针
	XMenuItem * getItemByNameInGroup(const std::string& name);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);
	void postPross();
	void updatePos();	//更新组的位置变化
	void updateSize();	//更新组的缩放变化
	void updateColor();
	void setHotChooseActive(XBool flag);
	XMenuItem *getItemByHotChooseKey(char k);
	XMenuItem *getItemByHotKey(char k);
	void setAllCallBackFun(void (*callbackFun)(void *,const std::string& ),void *p);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是左右上下键控制的代码
	int m_chooseIndex;
	void addIndex();//增加编号	
	void desIndex();	//减少编号
	void initIndex(XBool orderFlag = XTrue);	//初始化编号 orderFlag是否正序初始化
	void resetChoose();	//取消选择
	void openChoose();	//打开选择的项
	void closeChoose();	//关闭选择的项
	XBool setChooseItem(const XMenuItem *it);
	XMenuItem *getChooseItem();	//获取当前组中被选择的菜单项
	//-----------------------------------------------------
};
enum XMenuItemType
{//尚未完成
	MENU_ITEM_TYPE_BTN,	//按键类的菜单项
	MENU_ITEM_TYPE_CHK,	//选择类的菜单项
	MENU_ITEM_TYPE_DLT,	//分隔符类的菜单项
};
class XMenuItem	//一个菜单项
{
public:
	static const int m_maxMenuItemLength = 1024;	//菜单中文字的最大长度

	int m_levelIndex;		//所处的层次的编号
	XVec2 m_position;	//放的位置
	XVec2 m_scaleSize;	//缩放大小
	XVec2 m_size;		//像素尺寸大小
	XMenuGroup *m_owner;			//所属的组
	XMenuGroup *m_child;			//子物体的指针
	std::string m_originalName;		//原始的名称，在组内具有唯一性,最大长度受到m_maxMenuItemLength限制
	std::string m_showName;			//显示的名字
	XMenuItemType m_type;			//菜单项的类型
	XBool m_isOpen;	//是否处于打开状态
	XBool m_isChecked;				//是否被点击	//选择类的控件才能设置这个选项
	XFColor m_color;
	//void changeCheckedState();		//改变选择状态
	void setCheckState(XBool state);	//设置选择类菜单项的选择状态
	void setString(const std::string& name);
	void setPosition(const XVec2& p);
	void setScale(const XVec2& s);
	void setColor(const XFColor& c);
	//++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了剔除对按钮的使用而定义的变量
	XFontUnicode m_font;			//显示的字体
	XBool m_isEnable;				//是否有效状态
	XBool m_isVisible;				//是否可见
	XBool m_isMouseOn;				//是否处于鼠标悬浮状态
	XBool mouseProc(const XVec2& p,XMouseState mouseState);	//返回是否触发的事件
	//----------------------------------------------
	//++++++++++++++++++++++++++++++++++++++++++++++
	//下面加入对icon的支持
	XBool m_withIcon;	//是否有图标的支持
	XSprite m_spriteIcon;	//icon的贴图
	XBool setIcon(const char *filename,XResPos res = RES_SYS_DEF);
	void disIcon();	//取消Icon
	//----------------------------------------------
	//++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了实现键盘操作而定义的变量：2014年7月30日 注意：由于字体太大，显示控件不足不能支持热键操作
	char m_hotKey;			//当前菜单项的快捷键,ctrl + m_hotkey使得操作直接生效
	char m_hotChooseKey;	//按下alt之后按下m_hotChooseKey激活当前菜单
	XBool m_isHotChooseActive;
	XBool setHotKey(char k);	//设置快捷键//-1为无效
	XBool setHotChooseKey(char k);	//设置快选键//-1为无效
	XMenuItem *getItemByHotKey(char k);//通过快捷键来查找菜单项
	XMenuItem *getItemByHotChooseKey(char k);//从子菜单中寻找指定的热键的菜单项
	void setHotChooseActive(XBool flag);//设置热键激活状态
	//----------------------------------------------
private:
	void (*m_callbackFun)(void *pClass,const std::string& pos);
	void *m_pClass;
public:
	XMenuItem()
		:m_owner(NULL)
		,m_child(NULL)
		,m_isOpen(XFalse)
		,m_position(0.0f)
		,m_scaleSize(1.0f)
		,m_size(0.0f)
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
		,m_color(1.0f,1.0f)
	{}
	~XMenuItem(){XMem::XDELETE(m_child);}
	void draw();
	void setCallbackFun(void (*callbackFun)(void *,const std::string& ),void *p);	//设置回调函数，当菜单项被触发时会调用相关的回调函数
	void setAllCallBackFun(void (*callbackFun)(void *,const std::string& ),void *p);	//设置所有的子物体的回调函数
	void beTriggered();	//菜单项被触发时调用这个函数
	std::string getFullStr() const;	//获取当前菜单项的全路径

	XBool init(const std::string& name,const XFontUnicode& font,float fontsize,XMenuGroup *owner,XMenuItemType type = MENU_ITEM_TYPE_BTN);
	void setOpen();	//设置打开
	void disOpen();	//设置关闭
	XBool isInRect(const XVec2& p);
	XMenuItem * getItem(const XVec2& p);
	void postPross();
	void updateStr();	//更新字符串的显示
	void resetChoose();
};
class XMenu:public XControlBasic
{
private:
	XBool m_isInited;
	XMenuItem m_rootItem;	//根项
	XMenuType m_type;		//根菜单的类型

	XFontUnicode m_font;	//字体
	float m_fontSize;
//	XVec2 m_position;	//位置
//	XVec2 m_scale;
	XBool m_isClicked;		//是否被点击
	XMenuItem *m_oldChooseItem;	//之前选择的选项
	XBool m_needPostProcess;	//	是否需要后期处理

	void postProcess();	//后期处理的过程用于状态发生改变之后修正改变的状态
	std::string m_curChooseMenuStr;	//当前选择的菜单字符串
	static void ctrlProc(void *,const std::string& );
public:
	enum XMenuEvent
	{
		MNU_CHOOSE,	//发生了选择事件
	};
	const std::string& getCurChooseMenuStr(){return m_curChooseMenuStr;}	//获取当前选择的菜单完整字符串
public:
	//使用";"作为菜单项的分割符
	XBool addAItem(const std::string& pos,const std::string& name,int width = -1,XMenuItemType type = MENU_ITEM_TYPE_BTN);
	XBool getCheckState(const std::string& pos);
	XBool setCheckState(const std::string& pos,XBool state);
	XBool setDisable(const std::string& pos);
	XBool setEnable(const std::string& pos);
	XBool getEnable(const std::string& pos);
	XBool setMenuItemStr(const std::string& pos,const std::string& name);	//修改菜单项的名字，组中菜单项的名字必须要唯一，否则会造成逻辑问题
	XBool setItemIcon(const std::string& pos,const char *filename,XResPos res = RES_SYS_DEF);
	XBool setHotKey(const std::string& pos,char k);
	char getHotKey(const std::string& pos);
	XBool setHotChooseKey(const std::string& pos,char k);
	char getHotChooseKey(const std::string& pos);
	XMenuItem * getItemByName(const std::string& name);
	const std::string getItemFullStr(const XMenuItem * it);
private:
	XMenuItem * getItemFromGroup(char *p,XMenuGroup *g);	//在组g中查找名称为p的菜单项
	XBool getIsPath(XMenuItem *s,XMenuItem *d);	//判断s是否在d的路径上

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是对键盘操作的一些定义
	XBool m_hotChooseKeyIsActive;	//是否激活热键
	//------------------------------------------------------
	void closeAll();	//折叠所有的菜单项
public:
//	XBool setCBFunction(const std::string& pos,void (*callbackFun)(void *,const std::string& ),void *p);//设置菜单响应的回调函数
//	XBool setAllCBFunction(void (*callbackFun)(void *,const std::string& ),void *p);	//设置所有项的回调函数

	XBool initWithoutSkin(const XFontUnicode& font,float captionSize = 1.0f,XMenuType type = MENU_TYPE_HORIZONTA);
	XBool initWithoutSkin(XMenuType type = MENU_TYPE_HORIZONTA) {return initWithoutSkin(getDefaultFont(),1.0f,type);}
protected:
	void draw();
	void drawUp(){};							//do nothing
	XBool mouseProc(const XVec2& p,XMouseState mouseState);
	XBool keyboardProc(int keyOrder,XKeyState keyState);

	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);				//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&){return XTrue;}	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点
public:
	XMenu()
		:m_isInited(XFalse)
		,m_isClicked(XFalse)
		,m_oldChooseItem(NULL)
		,m_needPostProcess(XFalse)
		,m_hotChooseKeyIsActive(XFalse)
		,m_type(MENU_TYPE_HORIZONTA)
	{
		m_ctrlType = CTRL_OBJ_MENU;
	}
	~XMenu(){release();}
	void release();
	//为了与其他的控件接轨，还需要实现下面的接口
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color);	//设置字体的颜色
	const XFColor& getTextColor() const {return XFColor::white;}	//获取控件字体的颜色

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(const XFColor& c);
	void setAlpha(float a);

	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p) {return m_rootItem.isInRect(p);}		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	////virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XMenu(const XMenu &temp);
	XMenu& operator = (const XMenu& temp);
};
#if WITH_INLINE_FILE
#include "XMenu.inl"
#endif
}
#endif