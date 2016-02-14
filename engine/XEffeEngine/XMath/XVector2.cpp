#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XVector2.h"
namespace XE{
const XVector2 XVector2::zero(0.0f,0.0f);
#if !WITH_INLINE_FILE
#include "XVector2.inl"
#endif
//单精度2D点
//XVector2& XVector2::operator = (const XVector2I & temp)
//{
//	x = temp.x;
//	y = temp.y;
//	return *this;
//}
//XVector2::XVector2(const XVector2I & temp)
//{
//	x = temp.x;
//	y = temp.y;
//}
}