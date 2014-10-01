//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XControlBasic.h"

_XControlBasic::_XControlBasic()
:m_mouseRect(0.0f,0.0f,1.0f,1.0f)		//控件的鼠标响应范围
,m_ctrlType(CTRL_OBJ_NULL)
,m_size(1.0f,1.0f)		//控件的大小
,m_position(0.0f,0.0f)	//控件的位置
,m_color(1.0f,1.0f,1.0f,1.0f)		//控件的颜色
,m_isEnable(XFalse)		//控件是否有效，有效的物件才能设置下面的属性
,m_isVisible(XFalse)		//控件是否可见，可见的物件才能设置下面的属性
,m_isActive(XFalse)		//控件是否处于激活状态，激活的物件才能接收控制信号
,m_isBeChoose(XFalse)
,m_withAction(XTrue)
,m_isInAction(XFalse)
,m_isMouseInRect(XFalse)
{
	static int controlOrder = 0;
	controlOrder ++;
	m_objectID = controlOrder;
	m_objType = OBJ_CONTROL;
}
_XBool _XControlBasic::setACopy(const _XControlBasic & temp)
{
	m_comment.setACopy(temp.m_comment);
	m_isMouseInRect = temp.m_isMouseInRect;		//控件的鼠标响应范围
	m_mouseRect = temp.m_mouseRect;		//控件的鼠标响应范围
	m_nowMouseRect = temp.m_nowMouseRect;		//控件的鼠标响应范围
	m_size = temp.m_size;		//控件的大小
	m_position = temp.m_position;	//控件的位置
	m_color = temp.m_color;		//控件的颜色

	m_isEnable = temp.m_isEnable;		//控件是否有效，有效的物件才能设置下面的属性
	m_isVisible = temp.m_isVisible;		//控件是否可见，可见的物件才能设置下面的属性
	m_isActive = temp.m_isActive;		//控件是否处于激活状态，激活的物件才能接收控制信号
	m_isBeChoose = temp.m_isBeChoose;
	m_withAction = temp.m_withAction;
	m_isInAction = temp.m_isInAction;
	return XTrue;
}