#ifndef _JIA_XSIMPLELINE_
#define _JIA_XSIMPLELINE_
//这个是分割线控件的类
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.14
//--------------------------------
#include "XControlBasic.h"

enum _XSimpleLineType
{
	SIMPLELINE_TYPE_VERTICAL,		//垂直的
	SIMPLELINE_TYPE_HORIZONTAL,		//水平的
};
class _XSimpleLine:public _XControlBasic
{
private:
	_XBool m_isInited;
	float m_lineLen;
	float m_lineWidth;
	_XSimpleLineType m_type;
public:
	_XBool init(float lineLen,float lineWidth = 0.5f,
		_XSimpleLineType type = SIMPLELINE_TYPE_HORIZONTAL);

	void setLineLen(float lineLen){m_lineLen = lineLen;}
	float getLineLen() const {return m_lineLen;}
	void setLineWidth(float lineWidth){m_lineWidth = lineWidth;}
	float getLineWidth() const {return m_lineWidth;}
	void setLineType(_XSimpleLineType type){m_type = type;}
	_XSimpleLineType getLineType() const {return m_type;}

	_XSimpleLine()
		:m_isInited(XFalse)
		,m_lineLen(100.0f)
		,m_lineWidth(1.0f)
		,m_type(SIMPLELINE_TYPE_HORIZONTAL)
	{
		m_ctrlType = CTRL_OBJ_XSIMPLELINE;
	}
	~_XSimpleLine(){release();}
	void release();
	//下面是通用接口
	using _XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//避免覆盖的问题
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color){;}	//设置字体的颜色
	_XFColor getTextColor() const {return _XFColor::white;}	//获取控件字体的颜色

	using _XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a){m_color.setColor(r,g,b,a);}
	void setAlpha(float a){m_color.setA(a);}
protected:
	void draw();								//描绘按钮
	void drawUp(){};							//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState){return XFalse;}		//对于鼠标动作的响应函数
	_XBool keyboardProc(int keyOrder,_XKeyState keyState){return XFalse;}			//返回是否触发按键动作
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y){return XFalse;}				//用于判断当前物件是否可以获得焦点
	_XBool canLostFocus(float,float){return XTrue;}	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点
	_XBool canBeChoose(){return XFalse;}
public:
	_XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	_XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
private://下面为了防止错误，重载赋值操作符，复制构造函数
	_XSimpleLine(const _XSimpleLine &temp);
	_XSimpleLine& operator = (const _XSimpleLine& temp);
};
#include "XSimpleLine.inl"
#endif