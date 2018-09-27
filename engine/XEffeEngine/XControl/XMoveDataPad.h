#ifndef _JIA_XMOVEDATAPAD_
#define _JIA_XMOVEDATAPAD_

#include "XControlBasic.h"
namespace XE{
class XMoveDataPad:public XControlBasic
{
private:
	float m_pixelsSize;
	bool m_isInited;
	XMoveData m_md;
	XFontUnicode m_caption;	
	XMoveData m_p1MD;	//用于参数控制的
	XMoveData m_p2MD;	//用于参数控制的
	XMoveData m_p3MD;	//用于参数控制的
	XMoveData m_p4MD;	//用于参数控制的
	void initParam(XMoveDataMode mode);
public:
	bool initWithoutSkin(XMoveDataMode mode);
	XMoveDataPad()
		:m_pixelsSize(256.0f)
		,m_isInited(false)
	{
		m_ctrlType = CTRL_OBJ_MOVEDATAPAD;
	}
	~XMoveDataPad(){release();}
	void release();
	using XObjectBasic::setPosition;	//避免覆盖的问题
	void setPosition(const XVec2& p)
	{
		m_position = p;
		m_curMouseRect.set(m_position,m_position + m_scale * m_pixelsSize);
		m_caption.setPosition(m_curMouseRect.getLB());
		m_caption.setString(m_md.getModeStr().c_str());
	}
	using XObjectBasic::setScale;	//避免覆盖的问题
	void setScale(const XVec2& s)
	{
		m_scale = s;
		m_curMouseRect.set(m_position,m_position + m_scale * m_pixelsSize);
		m_caption.setPosition(m_curMouseRect.getLB());
		m_caption.setString(m_md.getModeStr().c_str());
		m_caption.setScale(m_scale);
	}

	using XObjectBasic::setColor;	//避免覆盖的问题
	void setColor(const XFColor& c)
	{
		m_color = c;
		m_caption.setAlpha(c.a);
		updateChildColor();
	}
	void setAlpha(float a)
	{
		m_color.setA(a);
		m_caption.setAlpha(a);
		updateChildAlpha();
	}
protected:
	void draw();
	void drawUp(){}
	void update(float steptime);
	XBool mouseProc(const XVec2& ,XMouseState){return XFalse;}		//对于鼠标动作的响应函数
	XBool keyboardProc(int,XKeyState){return XFalse;}			//返回是否触发按键动作
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p)				//用于判断当前物件是否可以获得焦点
	{
		if(!m_isInited ||		//如果没有初始化直接退出
			!m_isActive ||		//没有激活的控件不接收控制
			!m_isVisible ||		//如果不可见直接退出
			!m_isEnable) return XFalse;		//如果无效则直接退出
		return m_curMouseRect.isInRect(p);
	}
	XBool canLostFocus(const XVec2& ){return XTrue;}	//应该是可以随时失去焦点的
	void setLostFocus()	//设置失去焦点
	{
		if(!m_isInited ||		//如果没有初始化直接退出
			!m_isActive ||		//没有激活的控件不接收控制
			!m_isVisible ||		//如果不可见直接退出
			!m_isEnable) return;		//如果无效则直接退出
		m_isBeChoose = XFalse;	//控件处于焦点状态
	}

private://下面为了防止错误，重载赋值操作符，复制构造函数
	XMoveDataPad(const XMoveDataPad &temp);
	XMoveDataPad& operator = (const XMoveDataPad& temp);
public:
	//为了支持物件管理器管理控件，这里提供下面两个接口的支持
	XBool isInRect(const XVec2& p)		//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
	{
		if(!m_isInited) return XFalse;
		return m_curMouseRect.isInRect(p);
	}
	XVec2 getBox(int order)			//获取四个顶点的坐标，目前先不考虑旋转和缩放
	{
		if(!m_isInited) return XVec2::zero;
		switch(order)
		{
		case 0:return m_curMouseRect.getLT();
		case 1:return m_curMouseRect.getRT();
		case 2:return m_curMouseRect.getRB();
		case 3:return m_curMouseRect.getLB();
		}
		return XVec2::zero;
	}

	void setMode(XMoveDataMode mode)
	{
		if(!m_isInited) return;
		m_md.setEx(0.0f,1.0f,5.0f,mode,-1);
		initParam(mode);
		m_caption.setString(m_md.getModeStr().c_str());
	}
};
}
#endif