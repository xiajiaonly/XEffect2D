#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XRect.h"
namespace XE{
XRect::XRect(const XRectEx& r)
	:left(r.getLeft())
	, right(r.getRight())
	, top(r.getTop())
	, bottom(r.getBottom())
{}
#if !WITH_INLINE_FILE
#include "XRect.inl"
#endif
}