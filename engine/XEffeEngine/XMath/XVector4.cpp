#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XVector4.h"
namespace XE{
const XVector4 XVector4::zero(0.0f,0.0f,0.0f,0.0f);

XVector4 slerp(const XVector4& v0,const XVector4& v1,float r)
{//尚未经过验证
	if(r <= 0.0f) return v0;
	if(r >= 1.0f) return v1;
	XVector4 ret;
	float cosOmega = v0.dot(v1);
	if(cosOmega < 0.0f)
	{
		ret.set(-v1.x,-v1.y,-v1.z,-v1.w);
		cosOmega = -cosOmega;
	}
	float k0,k1;
	if(cosOmega > 0.9999f)
	{
		k0 = 1.0f - r;
		k1 = r;
	}else
	{
		float sinOmega = sqrt(1.0f - cosOmega *cosOmega);
		float omega = atan2(sinOmega,cosOmega);
		float oneOverSinOmega = 1.0f / sinOmega;
		k0 = sin((1.0f - r) * omega) * oneOverSinOmega;
		k1 = sin(r * omega) * oneOverSinOmega;
	}
	ret.set(v0.x * k0 + ret.x * k1,
		v0.y * k0 + ret.y * k1,
		v0.z * k0 + ret.z * k1,
		v0.w * k0 + ret.w * k1);
	return ret;
}
#if !WITH_INLINE_FILE
#include "XVector4.inl"
#endif
}