#ifndef _JIA_XSTAGEOBJECT_
#define _JIA_XSTAGEOBJECT_
#include "XMath\XVector3.h"
#include "XMath\XVector2.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
namespace XE{
//舞台上的物件
enum XStageObjectType
{//场景中的物件类型
	STG_OBJ_NULL,	//无效的物件
	STG_OBJ_MAX,	//用于统计物件的类型
};
class XStageObject
{
public:
	std::string m_name;		//可以使用也可以不适用
	int m_id;				//唯一
	XVec3 m_position;		//物件的位置
	XVec2 m_scale;			//物件的缩放比例
	XStageObjectType m_type;	//物件的类型
	bool m_visible;				//物件是否可见

	XVec2 m_originOffset;

	virtual void setPosition(const XVec2& pos) = 0;
	virtual void setPosition(float x, float y) { setPosition(XVec2(x, y)); }
	virtual void setScale(const XVec2& scale) = 0;
	virtual void setScale(float x, float y) { setScale(XVec2(x, y)); }
	virtual void draw() = 0;
	virtual void update(float stepTime) = 0;
	XStageObject()
		:m_scale(1.0f)
		,m_visible(true)
		,m_type(STG_OBJ_NULL)
	{
		static int tmpID = 0;
		m_id = tmpID ++;
	}
};
}
#endif