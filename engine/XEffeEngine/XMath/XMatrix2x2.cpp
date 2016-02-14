#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMatrix2x2.h"
namespace XE{
const XMatrix2x2 XMatrix2x2::identity(1.0f,0.0f,0.0f,1.0f);	//µ¥Î»¾ØÕó

XMatrix2x2 XMatrix2x2::inverse()
{
	XMatrix2x2 ret;
	XMatrix2x2& A = * this;
	double determinant = getValue();
	if(determinant == 0.0f) return ret;
	double invdet = 1/determinant;
	ret(0,0) =  A(1,1)*invdet;
	ret(1,0) = -A(0,1)*invdet;
	ret(0,1) = -A(1,0)*invdet;
	ret(1,1) =  A(0,0)*invdet;
	return ret;
}
#if !WITH_INLINE_FILE
#include "XMatrix2x2.inl"
#endif
}