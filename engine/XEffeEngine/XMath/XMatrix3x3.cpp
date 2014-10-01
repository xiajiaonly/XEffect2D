//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMatrix3x3.h"
#include "XMatrix2x2.h"
const _XMatrix3x3 _XMatrix3x3::identity(1.0f,0.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f);	//单位矩阵
_XMatrix3x3 _XMatrix3x3::inverse()
{
	_XMatrix3x3 ret;
	_XMatrix3x3& A = * this;
	double determinant = getValue();
	if(determinant == 0.0f) return ret;
	double invdet = 1/determinant;
	ret(0,0) =  (A(1,1)*A(2,2)-A(2,1)*A(1,2))*invdet;
	ret(1,0) = -(A(0,1)*A(2,2)-A(0,2)*A(2,1))*invdet;
	ret(2,0) =  (A(0,1)*A(1,2)-A(0,2)*A(1,1))*invdet;
	ret(0,1) = -(A(1,0)*A(2,2)-A(1,2)*A(2,0))*invdet;
	ret(1,1) =  (A(0,0)*A(2,2)-A(0,2)*A(2,0))*invdet;
	ret(2,1) = -(A(0,0)*A(1,2)-A(1,0)*A(0,2))*invdet;
	ret(0,2) =  (A(1,0)*A(2,1)-A(2,0)*A(1,1))*invdet;
	ret(1,2) = -(A(0,0)*A(2,1)-A(2,0)*A(0,1))*invdet;
	ret(2,2) =  (A(0,0)*A(1,1)-A(1,0)*A(0,1))*invdet;
	return ret;
}
_XVector2 _XMatrix3x3::resolveEquation()
{
	_XVector2 ret;
	_XMatrix2x2 m;
	m.data[0] = data[0];m.data[1] = data[1];
	m.data[2] = data[3];m.data[3] = data[4];
	float d = m.getValue();
	if(d == 0.0f) return ret;
	m.data[0] = data[2];//m.data[1] = data[1];
	m.data[2] = data[5];//m.data[3] = data[4];
	ret.x = m.getValue() / d;
	m.data[0] = data[0];m.data[1] = data[2];
	m.data[2] = data[3];m.data[3] = data[5];
	ret.y = m.getValue() / d;
	return ret;
}
_XVector4 toVector4(const _XMatrix3x3 &m)
{//从旋转矩阵到四元数的转换
	_XVector4 ret;
	ret.w = m(0,0) + m(1,1) + m(2,2);
	ret.x = m(0,0) - m(1,1) - m(2,2);
	ret.y = m(1,1) - m(0,0) - m(2,2);
	ret.z = m(2,2) - m(0,0) - m(1,1);
	int index = 0;
	float big = ret.w;
	if(ret.x > big)
	{
		big = ret.x;
		index = 1;
	}
	if(ret.y > big)
	{
		big = ret.y;
		index = 2;
	}
	if(ret.z > big)
	{
		big = ret.z;
		index = 3;
	}
	float bigV = sqrt(big + 1.0f) * 0.5f;
	float mult = 0.25f / bigV;
	switch(index)
	{
	case 0:
		ret.set((m(1,2) - m(2,1)) * mult,
			(m(2,0) - m(0,2)) * mult,
			(m(0,1) - m(1,0)) * mult,bigV);
		break;
	case 1:
		ret.set(bigV,(m(0,1) + m(1,0)) * mult,
			(m(2,0) + m(0,2)) * mult,
			(m(1,2) - m(2,1)) * mult);
		break;
	case 2:
		ret.set((m(0,1) + m(1,0)) * mult,bigV,
			(m(1,2) + m(2,1)) * mult,
			(m(2,0) - m(0,2)) * mult);
		break;
	case 3:
		ret.set((m(2,0) + m(0,2)) * mult,
			(m(1,2) + m(2,1)) * mult,bigV,
			(m(0,1) - m(1,0)) * mult);
		break;
	}
	return ret;
}