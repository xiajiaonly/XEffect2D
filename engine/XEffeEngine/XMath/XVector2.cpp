#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XVector2.h"
namespace XE {
const XVec2 XVec2::zero(0.0f);
const XVec2 XVec2::one(1.0f);
const XVec2 XVec2::zeroOne(0.0f, 1.0f);
const XVec2 XVec2::oneZero(1.0f, 0.0f);
#if !WITH_INLINE_FILE
#include "XVector2.inl"
#endif
//单精度2D点
//XVec2& XVec2::operator = (const XVector2I & temp)
//{
//	x = temp.x;
//	y = temp.y;
//	return *this;
//}
//XVec2::XVec2(const XVector2I & temp)
//{
//	x = temp.x;
//	y = temp.y;
//}
}