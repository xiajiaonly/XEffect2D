#ifndef _JIA_XCONTROLMANAGER_
#define _JIA_XCONTROLMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.10
//--------------------------------
#include "XCtrlManagerBase.h"
namespace XE{
//这个类需要使用到单子系统
class XControlManager:public XCtrlManagerBase
{
public:
	XControlManager(){}
	virtual ~XControlManager(){;}
protected:
	XControlManager(const XControlManager&);
	XControlManager &operator= (const XControlManager&);
};
}
#endif