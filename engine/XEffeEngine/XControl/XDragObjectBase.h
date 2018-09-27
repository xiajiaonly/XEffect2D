#ifndef _JIA_XDRAGOBJECTBASE_
#define _JIA_XDRAGOBJECTBASE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.9
//--------------------------------
//这个是可拖拽图物体的基类，这类物体可以被鼠标拖动
#include "../XOSDefine.h"
//#include "../XMouseAndKeyBoardDefine.h"
#include "../XMath/XVector2.h"
namespace XE{
class XDragObjectBase
{
protected:
	XBool m_isMouseBtnDown;
	int m_oldPosX;
	int m_oldPosY;
public:
	XDragObjectBase()
		:m_isMouseBtnDown(false)
		,m_oldPosX(0)
		,m_oldPosY(0)
	{}
	virtual ~XDragObjectBase(){}
	virtual void setPosition(const XVec2& pos) = 0;
	virtual const XVec2& getPosition()const = 0;
protected:
	virtual XBool getIsInDragRect(const XVec2& p) = 0;	//判断位置是否在拖拽区域内
	virtual XVec2 correctPos(const XVec2& pos) = 0;	//更具传入的拖拽位置，修正该位置，当位置出于非法状态时，修正成合法的位置
	void mouseEventProc(const XVec2& p,XMouseState mouseState);
};
}
#endif