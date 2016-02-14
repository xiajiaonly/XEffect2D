#ifndef _JIA_XSIMPLELINE_
#define _JIA_XSIMPLELINE_
//这个是分割线控件的类
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.14
//--------------------------------
#include "XControlBasic.h"
namespace XE{
enum XSimpleLineType
{
	SIMPLELINE_TYPE_VERTICAL,		//垂直的
	SIMPLELINE_TYPE_HORIZONTAL,		//水平的
};
class XSimpleLine:public XControlBasic
{
private:
	XBool m_isInited;
	float m_lineLen;
	float m_lineWidth;
	XSimpleLineType m_type;
public:
	XBool init(float lineLen,float lineWidth = 0.5f,
		XSimpleLineType type = SIMPLELINE_TYPE_HORIZONTAL);

	void setLineLen(float lineLen){m_lineLen = lineLen;}
	float getLineLen() const {return m_lineLen;}
	void setLineWidth(float lineWidth){m_lineWidth = lineWidth;}
	float getLineWidth() const {return m_lineWidth;}
	void setLineType(XSimpleLineType type){m_type = type;}
	XSimpleLineType getLineType() const {return m_type;}

	XSimpleLine()
		:m_isInited(XFalse)
		,m_lineLen(100.0f)
		,m_lineWidth(1.0f)
		,m_type(SIMPLELINE_TYPE_HORIZONTAL)
	{
		m_ctrlType = CTRL_OBJ_SIMPLELINE;
	}
	~XSimpleLine(){release();}
	void release();
	//下面是通用接口
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(float x,float y);

	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(float x,float y);

	void setTextColor(const XFColor&){;}	//设置字体的颜色
	XFColor getTextColor() const {return XFColor::white;}	//获取控件字体的颜色

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(float r,float g,float b,float a){m_color.setColor(r,g,b,a);}
	void setAlpha(float a){m_color.setA(a);}
protected:
	void draw();								//描绘按钮
	void drawUp(){};							//do nothing
	XBool mouseProc(float,float,XMouseState){return XFalse;}		//对于鼠标动作的响应函数
	XBool keyboardProc(int,XKeyState){return XFalse;}			//返回是否触发按键动作
	void insertChar(const char *,int){;}
	XBool canGetFocus(float,float){return XFalse;}				//用于判断当前物件是否可以获得焦点
	XBool canLostFocus(float,float){return XTrue;}	//应该是可以随时失去焦点的
	void setLostFocus();	//设置失去焦点
	XBool canBeChoose(){return XFalse;}
public:
	XBool isInRect(float x,float y);		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	XVector2 getBox(int order);			//获取四个顶点的坐标，目前先不考虑旋转和缩放
private://下面为了防止错误，重载赋值操作符，复制构造函数
	XSimpleLine(const XSimpleLine &temp);
	XSimpleLine& operator = (const XSimpleLine& temp);
};
#if WITH_INLINE_FILE
#include "XSimpleLine.inl"
#endif
}
#endif