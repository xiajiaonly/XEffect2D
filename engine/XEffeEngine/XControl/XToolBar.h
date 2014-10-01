#ifndef _JIA_XTOOLBAR_
#define _JIA_XTOOLBAR_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.7.31
//--------------------------------
#include "XControlBasic.h"
//这是一个工具栏的类，目前尚未实现功能
//工具栏是控件的集合
struct _XToolBarUnit
{
	_XBool enable;	//是否有效
	_XControlBasic *obj;
	_XVector2 size;	//原始大小
	int width;
	_XToolBarUnit()
		:obj(NULL)
		,enable(XTrue)
		,size(1.0f,1.0f)
		,width(0.0f)
	{}
};
class _XToolBar:public _XControlBasic
{
private:
	_XBool m_isInited;	//是否已经初始化
//	_XVector2 m_position;
//	_XVector2 m_size;
	float m_insertPoint;	//插入点的位置

	int m_height;
	std::vector<_XToolBarUnit> m_objects;

	_XBool m_isMouseDown;	//鼠标是否按下
	_XVector2 m_oldMousePosition;	//鼠标按下的位置
public:
	_XBool initWithoutTex(int height = 32);
protected:
	void draw();
	void drawUp();
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);		//返回是否触发按键动作
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);				//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}	//应该是可以随时失去焦点的
	//void setLostFocus(){return XTrue;}	//设置失去焦点
public:
	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);
	using _XObjectBasic::setSize;	//避免覆盖的问题
	void setSize(float x,float y);

	using _XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a)
	{
		m_color.setColor(r,g,b,a);
	}
	void setAlpha(float a)
	{
		m_color.setA(a);
	}

	//这个类不会对资源进行管理，所以不会自动释放资源，如果需要外部没有
	//主动释放资源的话需要手动调用资源释放函数
	void addAObject(_XControlBasic *obj);
	_XControlBasic *getObjByIndex(int index);
	int getIndexByP(const _XControlBasic *obj);
	void releaseMem();
	_XToolBar()
		:m_isInited(XFalse)
		,m_insertPoint(10.0f)
	{
		m_ctrlType = CTRL_OBJ_XTOOLBAR;
	}
	~_XToolBar(){release();}
	void release();

	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	void setVisible();
	void disVisible();	//设置控件不可见
	////virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	_XToolBar(const _XToolBar &temp);
	_XToolBar& operator = (const _XToolBar& temp);

};
#include "XToolBar.inl"
#endif