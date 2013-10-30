//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XControlBasic.h"

_XControlBasic::_XControlBasic()
:m_mouseRect(0.0f,0.0f,1.0f,1.0f)		//控件的鼠标响应范围
,m_size(1.0f,1.0f)		//控件的大小
,m_position(0.0f,0.0f)	//控件的位置
,m_color(1.0f,1.0f,1.0f,1.0f)		//控件的颜色
,m_isEnable(0)		//控件是否有效，有效的物件才能设置下面的属性
,m_isVisiable(0)		//控件是否可见，可见的物件才能设置下面的属性
,m_isActive(0)		//控件是否处于激活状态，激活的物件才能接收控制信号
,m_isBeChoose(0)
{
	static int controlOrder = 0;
	controlOrder ++;
	m_objectID = controlOrder;
}

int _XControlBasic::setACopy(const _XControlBasic & temp)
{
	m_mouseRect = temp.m_mouseRect;		//控件的鼠标响应范围
	m_size = temp.m_size;		//控件的大小
	m_position = temp.m_position;	//控件的位置
	m_color = temp.m_color;		//控件的颜色

	m_isEnable = temp.m_isEnable;		//控件是否有效，有效的物件才能设置下面的属性
	m_isVisiable = temp.m_isVisiable;		//控件是否可见，可见的物件才能设置下面的属性
	m_isActive = temp.m_isActive;		//控件是否处于激活状态，激活的物件才能接收控制信号
	m_isBeChoose = temp.m_isBeChoose;
	return 1;
}