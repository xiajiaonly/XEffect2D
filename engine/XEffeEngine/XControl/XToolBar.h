#ifndef _JIA_XTOOLBAR_
#define _JIA_XTOOLBAR_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.7.31
//--------------------------------
#include "XControlBasic.h"
namespace XE{
//这是一个工具栏的类，目前尚未实现功能
//工具栏是控件的集合
struct XToolBarUnit
{
	XBool enable;	//是否有效
	XControlBasic *obj;
	XVec2 scale;	//原始大小
	int width;
	XToolBarUnit()
		:obj(NULL)
		,enable(XTrue)
		,scale(1.0f)
		,width(0)
	{}
};
class XToolBar:public XControlBasic
{
private:
	XBool m_isInited;	//是否已经初始化
//	XVec2 m_position;
//	XVec2 m_scale;
	float m_insertPoint;	//插入点的位置

	int m_height;
	std::vector<XToolBarUnit> m_objects;

	XBool m_isMouseDown;	//鼠标是否按下
	XVec2 m_oldMousePosition;	//鼠标按下的位置
public:
	XBool initWithoutSkin(int height = 32);
protected:
	void draw();
	void drawUp();
	XBool mouseProc(const XVec2& p,XMouseState mouseState);		//对于鼠标动作的响应函数
	XBool keyboardProc(int keyOrder,XKeyState keyState);		//返回是否触发按键动作
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);				//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(const XVec2&){return XTrue;}	//应该是可以随时失去焦点的
	//void setLostFocus(){return XTrue;}	//设置失去焦点
public:
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p);
	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(const XVec2& s);

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(const XFColor& c){m_color = c;}
	void setAlpha(float a){m_color.setA(a);}

	//这个类不会对资源进行管理，所以不会自动释放资源，如果需要外部没有
	//主动释放资源的话需要手动调用资源释放函数
	void addAObject(XControlBasic *obj);
	XControlBasic *getObjByIndex(unsigned int index);
	int getIndexByP(const XControlBasic *obj);
	void releaseMem();
	XToolBar()
		:m_isInited(XFalse)
		,m_insertPoint(10.0f)
	{
		m_ctrlType = CTRL_OBJ_TOOLBAR;
	}
	~XToolBar(){release();}
	void release();

	XBool isInRect(const XVec2& p);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVec2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	void setVisible();
	void disVisible();	//设置控件不可见
	////virtual void justForTest() {;}
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XToolBar(const XToolBar &temp);
	XToolBar& operator = (const XToolBar& temp);

};
#if WITH_INLINE_FILE
#include "XToolBar.inl"
#endif
}
#endif