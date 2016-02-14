#ifndef _JIA_XCONTROLBASIC_
#define _JIA_XCONTROLBASIC_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "../XObjectBasic.h" 
#include "XComment.h"
#include "../XFrameWork.h"
class TiXmlNode;
namespace XE{
#define MIN_FONT_CTRL_SIZE (XEG.m_windowData.systemFontSize + 2.0f)

//控件的模式
enum XCtrlMode
{
	CTRL_MODE_SIMPLE,	//简单模式，只描绘必要的部分
	CTRL_MODE_NORMAL,	//标准模式，描绘常用的信息
	CTRL_MODE_COMPLETE,	//完成模式，描绘所有的信息
};
enum XCtrlObjType
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
	CTRL_OBJ_PASSWORDPAD,	//密码输入板
	CTRL_OBJ_IMAGELIST,		//图像陈列框
	CTRL_OBJ_CHART,		//图标框
	CTRL_OBJ_SIMPLECHART,		//简单曲线图标
	CTRL_OBJ_TAB,			//标签控件
	CTRL_OBJ_TEXT,			//文字控件
	CTRL_OBJ_SIMPLELINE,	//分割线控件
	CTRL_OBJ_PROGRESSRING,	//进度条的物件
	CTRL_OBJ_CALENDAR,		//日历控件
	CTRL_OBJ_MENU,			//菜单类的控件
	CTRL_OBJ_IMAGE,		//图片的控件
	CTRL_OBJ_BTNBAR,		//按钮条的控件
	CTRL_OBJ_TOOLBAR,		//工具条的控件
	CTRL_OBJ_PROPERTYBOX,	//属性框的控件
	CTRL_OBJ_COLORCHOOSE,	//颜色选择的控件
	CTRL_OBJ_SUBWINDOW,	//子窗口控件
	CTRL_OBJ_SOFTBOARD,	//软键盘控件
	CTRL_OBJ_PARAMCTRL,	//参数控件
	CTRL_OBJ_FUNCTION,	//绘图函数
};
enum XCtrlSpecialType
{
	CTRL_TYPE_NORMAL,	//普通控件
	CTRL_TYPE_KYBOARD,	//具有键盘事件的优先处理级别
	CTRL_TYPE_MOUSE,	//具有鼠标事件的优先处理级别
};
//通过控件类型返回控件的类名如：CTRL_OBJ_BUTTON返回XButton
namespace XCtrl
{
	inline std::string getCtrlNameByType(XCtrlObjType type);
	inline std::string getCtrlTypeByString(XCtrlObjType type);
	inline XCtrlObjType getCtrlTypeByString(const std::string &name);
}
//说明：引入友元的目的是为了让控件管理器可以访问控件的绘图函数和消息处理函数，
//但是控件的实体被定义之后，定义的程序员不能自己调用这些函数，防止重复绘图和重
//复的消息处理，但是友元的引入造成了类结构设计的耦合度和复杂度，这里需要思考更
//好的解决方式
class XCtrlManagerBase;
class XToolBar;
class XTab;
class XPropertyLine;
class XSubWindow;
//控件类的基类
class XControlBasic:public XObjectBasic
{
	friend XCtrlManagerBase;
	friend XToolBar;
	friend XTab;
	friend XPropertyLine;
	friend XSubWindow;
protected:
	XCtrlSpecialType m_ctrlSpecialType;
	XCtrlObjType m_ctrlType;
	XRect m_mouseRect;		//控件的鼠标响应范围
	XRect m_curMouseRect;	//当前鼠标的响应范围，这个范围受到位置和大小的变化而变化	(这个值，没有在所有的控件中使用，造成效率下降，2014.4.30修正这个问题)
	XVector2 m_scale;		//控件的大小
	XVector2 m_position;	//控件的位置
	XFColor m_color;		//控件的颜色

	XBool m_isEnable;		//控件是否有效，无效的物件可以看到，但是却显示为不能操作的模式
	XBool m_isVisible;		//控件是否可见，可见的物件才会显示出来
	XBool m_isActive;		//控件是否处于激活状态，激活的物件才能接收控制信号,同时允许多个物体被激活
	XBool m_isBeChoose;		//控件是否被选中，接收键盘操作，例如用table键选择(暂时尚未实现)，但是同时只有一个物件能成为焦点
	int m_objectID;			//控件的ID
	XBool m_withAction;	//是否使用控件的动态效果
	XBool m_isInAction;	//是否正在处于动作过程中

	XComment m_comment;	//注释
	XBool m_isMouseInRect;	//鼠标是否在范围内
public:
	XVector2 getPixelSize() const {return XVector2(m_mouseRect.getWidth(),m_mouseRect.getHeight());}
	XCtrlObjType getCtrlType()const{return m_ctrlType;}
	void setWithAction(XBool flag){m_withAction = flag;}
	XBool getWithAction()const{return m_withAction;}
	XBool getIsInAction()const{return m_isInAction;}
	XRect getMouseRect() const {return m_mouseRect;}
	float getMouseRectWidth() const {return m_mouseRect.getWidth();}
	float getMouseRectHeight() const {return m_mouseRect.getHeight();}
	int getControlID() const {return m_objectID;}	//获取控件的ID
	void setActive(){m_isActive = XTrue;}
	void disActive(){m_isActive = XFalse;}		//设置控件不接收控制信息
	XBool getActive() const {return m_isActive;}
	void setVisible()
	{
		m_isVisible = XTrue;
		m_comment.setVisible();
		updateChildVisible();
	}
	void disVisible()
	{
		m_isVisible = XFalse;
		m_isBeChoose = XFalse;
		m_comment.disVisible();
		updateChildVisible();
	}	//设置控件不可见
	XBool getVisible() const {return m_isVisible;}
	XBool getEnable() const {return m_isEnable;}
	XBool setACopy(const XControlBasic & temp);
	//++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是注释相关的接口
	void setComment(const char *str) {m_comment.setString(str);}
	void disComment() {m_comment.setString(NULL);}
	void setCommentPos(float x,float y){m_comment.setPosition(x,y);}
	//------------------------------------------------
protected:	//设计为下面的函数不允许在外部调用
	virtual void update(float){;}	//默认什么也不做
	virtual void draw() = 0;
	virtual void drawUp() = 0;	//用于描绘上一个层面的东西，控件分两个层面，底层和漂浮层，漂浮层也就是上层有：例如右键菜单，下拉菜单等
	virtual XBool mouseProc(float x,float y,XMouseState mouseState) = 0;	//鼠标响应函数,关于这个函数的返回值的意义，各个空间类的定义尚不统一，有返回控件是否发生状态变更的，等等
	virtual XBool keyboardProc(int keyOrder,XKeyState keyState) = 0;		//键盘响应函数
	virtual void insertChar(const char * ch,int len) = 0;

	virtual XBool canBeChoose()
	{
		return m_isActive &&		//没有激活的控件不接收控制
			m_isVisible &&		//如果不可见直接退出
			m_isEnable;		//如果无效则直接退出
	}
	virtual XBool canGetFocus(float x,float y) = 0;	//是否可以获得焦点
	virtual XBool canLostFocus(float x,float y) = 0;	//当前控件是否可以失去焦点，用于处理焦点抢夺问题
	virtual void setLostFocus() {m_isBeChoose = XFalse;}	//设置失去焦点
	virtual void setFocus() {m_isBeChoose = XTrue;}			//设置为焦点
public:
	virtual XBool isFocus() {return m_isBeChoose;}		//是否处于焦点
	using XObjectBasic::setPosition;	//避免覆盖的问题
//	virtual void setPosition(const XVector2& position) {setPosition(position.x,position.y);}
//	virtual void setPosition(float x,float y) = 0;
	virtual XVector2 getPosition() const {return m_position;}

	using XObjectBasic::setScale;	//避免覆盖的问题
	virtual XVector2 getScale() const {return m_scale;}

	using XObjectBasic::setColor;	//避免覆盖的问题
//	virtual void setColor(float r,float g,float b,float a) = 0;		//+Child处理
//	virtual void setColor(const XFColor& color) {setColor(color.fR,color.fG,color.fB,color.fA);}
	virtual XFColor getColor() const {return m_color;}	//获取控件字体的颜色

	float getAngle() const {return 0.0f;}				//获取物件的角度
	void setAngle(float){updateChildAngle();}					//设置物件的角度

	XControlBasic();
	virtual ~XControlBasic() {};
private:	//为了防止意外调用的错误，这里重载赋值操作符和赋值构造函数
	XControlBasic(const XControlBasic &temp);
	XControlBasic& operator = (const XControlBasic& temp);
protected:
	void *m_pClass;
	void (*m_eventProc)(void *,int ID,int eventID);
public:
	virtual void setEventProc(void (* eventProc)(void *,int,int),void *pClass = NULL)
	{
		m_eventProc = eventProc;
		if(pClass != NULL) m_pClass = pClass;
		else m_pClass = this;
	}
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面实现界面控件的自身状态的保存（这个功能尚未完全实现）
protected:
	XBool m_needSaveAndLoad;	//是否需要保存或者读取
	std::string m_ctrlName;		//该控件的名称
public:
	virtual void setNeedSaveAndLoad(bool flag) {m_needSaveAndLoad = flag;}
	virtual XBool getNeedSaveAndLoad() const {return m_needSaveAndLoad;}
	virtual void setCtrlName(const std::string& name){m_ctrlName = name;}
	virtual std::string getCtrlName() const {return m_ctrlName;}
	virtual XBool saveState(TiXmlNode &)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *)
	{
		if(!m_needSaveAndLoad) return XTrue;	//如果不需要保存则直接返回
		return XTrue;
	}
	//---------------------------------------------------------
};
/*
//需要实现的控件包括以下一些
1、按钮		Button			x
2、单行输入框	Edit		x	(目前还不能解决中文输入的问题)
3、进度条	Progress		x
4、滑动条	Slider			x
5、多列列表框 MutiList		x
6、单列列表框 MutiText		x
7、单选按钮	Radio			x
8、多选按钮 Check			x
9、下拉菜单	Combo			x
10、群组框	Group			x
11、页面（标签按钮） Tab
12、多行输入框	MutiEdit		(目前还不能解决中文输入的问题)
13、控件管理的类 用于统一管理所有空间直接的ID以及以内互斥关系
*/
#if WITH_INLINE_FILE
#include "XControlBasic.inl"
#endif
}
#endif