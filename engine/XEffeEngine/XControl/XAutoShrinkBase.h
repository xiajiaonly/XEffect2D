#ifndef _JIA_XAUTOSHRINKBASE_
#define _JIA_XAUTOSHRINKBASE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.9
//--------------------------------
#include "../XOSDefine.h"
//#include "../XMouseAndKeyBoardDefine.h"
#include "../XMath/XVector2.h"
#include "../XMath/XMoveData.h"
namespace XE{
//具有自动收缩功能的物件的基类
class XAutoShrinkBase
{
protected:
	bool m_withAutoShrink;	//是否会自动收缩
	bool m_isInShrink;		//当前是否处于收缩状态
	bool m_oldIsInRect;
	float m_shrinkTimer;	//收缩计时器
	float m_shrinkRate;		//收缩的百分比0.0f - 1.0f;
	XVec2 m_curMousePosition;	//当前鼠标的位置
	XMoveData m_shrinkMD;
	enum XShrinkDirection
	{
		DIR_NULL,	//未发生收缩动作
		DIR_UP,		//向上收缩
		DIR_LEFT,	//向左收缩
		DIR_RIGHT,	//向右收缩
		DIR_DOWN,	//向下收缩
	};
	XShrinkDirection m_curShrinkDir;	//当前的收所方向
	virtual XShrinkDirection calShrinkDir() = 0;	//计算当前可能的收缩方向
	void startShrink(XShrinkDirection dir)	//开始收缩
	{
		m_shrinkMD.set(m_shrinkRate,1.0f,0.01f,MD_MODE_COS_MULT);
		m_curShrinkDir = dir;
		m_isInShrink = true;
	}
	void disShrink()	//从收缩状态复原
	{
		m_shrinkMD.set(m_shrinkRate,0.0f,0.01f,MD_MODE_SIN_MULT);
		m_isInShrink = false;
	}
	void updateShrinkState(float stepTime);	//更新收缩的状态
public:
	void setAutoShrink(bool flag)
	{
		//这里需要根据当前状态修改窗口的状态
		if(m_withAutoShrink && !flag && m_isInShrink)
		{//这里需要设置为非收缩状态(尚未完成)
		}
		m_withAutoShrink = flag;
	}
	bool getAutoShrink()const{return m_withAutoShrink;}
public:
	XAutoShrinkBase()
		:m_withAutoShrink(false)
		, m_shrinkTimer(0.0f)
		, m_isInShrink(false)
		, m_shrinkRate(0.0f)
		, m_oldIsInRect(false)
	{}
	virtual ~XAutoShrinkBase(){}
protected:
	virtual void updateShrinkPosition() = 0;	//更新收缩的位置
	virtual XBool getIsInShrinkRect() = 0;		//判断位置是否在拖拽区域内
};
}
#endif