//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMatrix4x4.h"
#include "XVector4.h"
_XMatrix4x4 _XMatrix4x4::operator*(const _XMatrix4x4 & rhs) const
{
	//Optimise for matrices in which bottom row is (0, 0, 0, 1) in both matrices
	if(data[3] == 0.0f && data[7] == 0.0f && data[11] == 0.0f && data[15] == 1.0f &&
		rhs.data[3] == 0.0f && rhs.data[7] == 0.0f &&
		rhs.data[11] == 0.0f && rhs.data[15] == 1.0f)
	{
		return _XMatrix4x4(	data[0]*rhs.data[0]+data[4]*rhs.data[1]+data[8]*rhs.data[2],
							data[1]*rhs.data[0]+data[5]*rhs.data[1]+data[9]*rhs.data[2],
							data[2]*rhs.data[0]+data[6]*rhs.data[1]+data[10]*rhs.data[2],
							0.0f,
							data[0]*rhs.data[4]+data[4]*rhs.data[5]+data[8]*rhs.data[6],
							data[1]*rhs.data[4]+data[5]*rhs.data[5]+data[9]*rhs.data[6],
							data[2]*rhs.data[4]+data[6]*rhs.data[5]+data[10]*rhs.data[6],
							0.0f,
							data[0]*rhs.data[8]+data[4]*rhs.data[9]+data[8]*rhs.data[10],
							data[1]*rhs.data[8]+data[5]*rhs.data[9]+data[9]*rhs.data[10],
							data[2]*rhs.data[8]+data[6]*rhs.data[9]+data[10]*rhs.data[10],
							0.0f,
							data[0]*rhs.data[12]+data[4]*rhs.data[13]+data[8]*rhs.data[14]+data[12],
							data[1]*rhs.data[12]+data[5]*rhs.data[13]+data[9]*rhs.data[14]+data[13],
							data[2]*rhs.data[12]+data[6]*rhs.data[13]+data[10]*rhs.data[14]+data[14],
							1.0f);
	}

	//Optimise for when bottom row of 1st matrix is (0, 0, 0, 1)
	if(data[3] == 0.0f && data[7] == 0.0f && data[11] == 0.0f && data[15] == 1.0f)
	{
		return _XMatrix4x4(	data[0]*rhs.data[0]+data[4]*rhs.data[1]+data[8]*rhs.data[2]+data[12]*rhs.data[3],
							data[1]*rhs.data[0]+data[5]*rhs.data[1]+data[9]*rhs.data[2]+data[13]*rhs.data[3],
							data[2]*rhs.data[0]+data[6]*rhs.data[1]+data[10]*rhs.data[2]+data[14]*rhs.data[3],
							rhs.data[3],
							data[0]*rhs.data[4]+data[4]*rhs.data[5]+data[8]*rhs.data[6]+data[12]*rhs.data[7],
							data[1]*rhs.data[4]+data[5]*rhs.data[5]+data[9]*rhs.data[6]+data[13]*rhs.data[7],
							data[2]*rhs.data[4]+data[6]*rhs.data[5]+data[10]*rhs.data[6]+data[14]*rhs.data[7],
							rhs.data[7],
							data[0]*rhs.data[8]+data[4]*rhs.data[9]+data[8]*rhs.data[10]+data[12]*rhs.data[11],
							data[1]*rhs.data[8]+data[5]*rhs.data[9]+data[9]*rhs.data[10]+data[13]*rhs.data[11],
							data[2]*rhs.data[8]+data[6]*rhs.data[9]+data[10]*rhs.data[10]+data[14]*rhs.data[11],
							rhs.data[11],
							data[0]*rhs.data[12]+data[4]*rhs.data[13]+data[8]*rhs.data[14]+data[12]*rhs.data[15],
							data[1]*rhs.data[12]+data[5]*rhs.data[13]+data[9]*rhs.data[14]+data[13]*rhs.data[15],
							data[2]*rhs.data[12]+data[6]*rhs.data[13]+data[10]*rhs.data[14]+data[14]*rhs.data[15],
							rhs.data[15]);
	}

	//Optimise for when bottom row of 2nd matrix is (0, 0, 0, 1)
	if(	rhs.data[3] == 0.0f && rhs.data[7] == 0.0f &&
		rhs.data[11] == 0.0f && rhs.data[15] == 1.0f)
	{
		return _XMatrix4x4(	data[0]*rhs.data[0]+data[4]*rhs.data[1]+data[8]*rhs.data[2],
							data[1]*rhs.data[0]+data[5]*rhs.data[1]+data[9]*rhs.data[2],
							data[2]*rhs.data[0]+data[6]*rhs.data[1]+data[10]*rhs.data[2],
							data[3]*rhs.data[0]+data[7]*rhs.data[1]+data[11]*rhs.data[2],
							data[0]*rhs.data[4]+data[4]*rhs.data[5]+data[8]*rhs.data[6],
							data[1]*rhs.data[4]+data[5]*rhs.data[5]+data[9]*rhs.data[6],
							data[2]*rhs.data[4]+data[6]*rhs.data[5]+data[10]*rhs.data[6],
							data[3]*rhs.data[4]+data[7]*rhs.data[5]+data[11]*rhs.data[6],
							data[0]*rhs.data[8]+data[4]*rhs.data[9]+data[8]*rhs.data[10],
							data[1]*rhs.data[8]+data[5]*rhs.data[9]+data[9]*rhs.data[10],
							data[2]*rhs.data[8]+data[6]*rhs.data[9]+data[10]*rhs.data[10],
							data[3]*rhs.data[8]+data[7]*rhs.data[9]+data[11]*rhs.data[10],
							data[0]*rhs.data[12]+data[4]*rhs.data[13]+data[8]*rhs.data[14]+data[12],
							data[1]*rhs.data[12]+data[5]*rhs.data[13]+data[9]*rhs.data[14]+data[13],
							data[2]*rhs.data[12]+data[6]*rhs.data[13]+data[10]*rhs.data[14]+data[14],
							data[3]*rhs.data[12]+data[7]*rhs.data[13]+data[11]*rhs.data[14]+data[15]);
	}	
	
	return _XMatrix4x4(	data[0]*rhs.data[0]+data[4]*rhs.data[1]+data[8]*rhs.data[2]+data[12]*rhs.data[3],
						data[1]*rhs.data[0]+data[5]*rhs.data[1]+data[9]*rhs.data[2]+data[13]*rhs.data[3],
						data[2]*rhs.data[0]+data[6]*rhs.data[1]+data[10]*rhs.data[2]+data[14]*rhs.data[3],
						data[3]*rhs.data[0]+data[7]*rhs.data[1]+data[11]*rhs.data[2]+data[15]*rhs.data[3],
						data[0]*rhs.data[4]+data[4]*rhs.data[5]+data[8]*rhs.data[6]+data[12]*rhs.data[7],
						data[1]*rhs.data[4]+data[5]*rhs.data[5]+data[9]*rhs.data[6]+data[13]*rhs.data[7],
						data[2]*rhs.data[4]+data[6]*rhs.data[5]+data[10]*rhs.data[6]+data[14]*rhs.data[7],
						data[3]*rhs.data[4]+data[7]*rhs.data[5]+data[11]*rhs.data[6]+data[15]*rhs.data[7],
						data[0]*rhs.data[8]+data[4]*rhs.data[9]+data[8]*rhs.data[10]+data[12]*rhs.data[11],
						data[1]*rhs.data[8]+data[5]*rhs.data[9]+data[9]*rhs.data[10]+data[13]*rhs.data[11],
						data[2]*rhs.data[8]+data[6]*rhs.data[9]+data[10]*rhs.data[10]+data[14]*rhs.data[11],
						data[3]*rhs.data[8]+data[7]*rhs.data[9]+data[11]*rhs.data[10]+data[15]*rhs.data[11],
						data[0]*rhs.data[12]+data[4]*rhs.data[13]+data[8]*rhs.data[14]+data[12]*rhs.data[15],
						data[1]*rhs.data[12]+data[5]*rhs.data[13]+data[9]*rhs.data[14]+data[13]*rhs.data[15],
						data[2]*rhs.data[12]+data[6]*rhs.data[13]+data[10]*rhs.data[14]+data[14]*rhs.data[15],
						data[3]*rhs.data[12]+data[7]*rhs.data[13]+data[11]*rhs.data[14]+data[15]*rhs.data[15]);
}
_XVector4 _XMatrix4x4::getRow(int position) const
{
	switch(position)
	{
	case 0:
		return _XVector4(data[0],data[4],data[8],data[12]);
		break;
	case 1:
		return _XVector4(data[1],data[5],data[9],data[13]);
		break;
	case 2:
		return _XVector4(data[2],data[6],data[10],data[14]);
		break;
	case 3:
		return _XVector4(data[3],data[7],data[11],data[15]);
		break;
	default:
		return _XVector4();
		break;
	}
}
_XVector4 _XMatrix4x4::operator*(const _XVector4& v) const 
{
	return _XVector4((data[0]*v.x + data[1]*v.y + data[2]*v.z + data[3]*v.w),
				 (data[4]*v.x + data[5]*v.y + data[6]*v.z + data[7]*v.w),
				 (data[8]*v.x + data[9]*v.y + data[10]*v.z + data[11]*v.w),
				 (data[12]*v.x + data[13]*v.y + data[14]*v.z + data[15]*v.w)) ;
}
inline float det2x2sub(const float *m, int i0, int i1, int i2, int i3)
{
	return m[i0] * m[i3] - m[i2] * m[i1];
}
inline float det3x3sub(const float *m, int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8)
{
	float det = 0.0f;

	det += m[i0] * det2x2sub(m, i4, i5, i7, i8);
	det -= m[i3] * det2x2sub(m, i1, i2, i7, i8);
	det += m[i6] * det2x2sub(m, i1, i2, i4, i5);

	return det;
}
_XMatrix4x4 _XMatrix4x4::inverse()
{
	float *m = data;
	float det = 0.0f;
	
	det += m[0] * det3x3sub(m, 5, 6, 7, 9, 10, 11, 13, 14, 15);
	det -= m[4] * det3x3sub(m, 1, 2, 3, 9, 10, 11, 13, 14, 15);
	det += m[8] * det3x3sub(m, 1, 2, 3, 5, 6, 7, 13, 14, 15);
	det -= m[12] * det3x3sub(m, 1, 2, 3, 5, 6, 7, 9, 10, 11);

	static _XMatrix4x4 inverse;
	det = 1.0f / det; 

	inverse.data[0] = det3x3sub(m, 5, 6, 7, 9, 10, 11, 13, 14, 15) * det;
	inverse.data[1] = -det3x3sub(m, 1, 2, 3, 9, 10, 11, 13, 14, 15) * det;
	inverse.data[2] = det3x3sub(m, 1, 2, 3, 5, 6, 7, 13, 14, 15) * det;
	inverse.data[3] = -det3x3sub(m, 1, 2, 3, 5, 6, 7, 9, 10, 11) * det;
	inverse.data[4] = -det3x3sub(m, 4, 6, 7, 8, 10, 11, 12, 14, 15) * det;
	inverse.data[5] = det3x3sub(m, 0, 2, 3, 8, 10, 11, 12, 14, 15) * det;
	inverse.data[6] = -det3x3sub(m, 0, 2, 3, 4, 6, 7, 12, 14, 15) * det;
	inverse.data[7] = det3x3sub(m, 0, 2, 3, 4, 6, 7, 8, 10, 11) * det;
	inverse.data[8] = det3x3sub(m, 4, 5, 7, 8, 9, 11, 12, 13, 15) * det;
	inverse.data[9] = -det3x3sub(m, 0, 1, 3, 8, 9, 11, 12, 13, 15) * det;
	inverse.data[10] = det3x3sub(m, 0, 1, 3, 4, 5, 7, 12, 13, 15) * det;
	inverse.data[11] = -det3x3sub(m, 0, 1, 3, 4, 5, 7, 8, 9, 11) * det;
	inverse.data[12] = -det3x3sub(m, 4, 5, 6, 8, 9, 10, 12, 13, 14) * det;
	inverse.data[13] = det3x3sub(m, 0, 1, 2, 8, 9, 10, 12, 13, 14) * det;
	inverse.data[14] = -det3x3sub(m, 0, 1, 2, 4, 5, 6, 12, 13, 14) * det;
	inverse.data[15] = det3x3sub(m, 0, 1, 2, 4, 5, 6, 8, 9, 10) * det;

	return inverse;
}
inline float getDeta(const _XVector3 &v0,const _XVector3 &v1,const _XVector3 &v2)
{
	return v0.x * v1.y * v2.z + v1.x * v2.y * v0.z + v2.x * v0.y * v1.z
		- v2.x * v1.y * v0.z - v0.x * v2.y * v1.z - v1.x * v0.y * v2.z;
}
//计算三个平面的交点
_XVector3 getPoint(const _XVector4& v0,const _XVector4& v1,const _XVector4& v2)
{
	_XVector3 ret;
	_XVector3 x0(v0.x,v0.y,v0.z);
	_XVector3 x1(v1.x,v1.y,v1.z);
	_XVector3 x2(v2.x,v2.y,v2.z);
	_XVector3 b(v0.w,v1.w,v2.w);
	float d = -getDeta(x0,x1,x2);
	x0 = _XVector3(v0.w,v0.y,v0.z);
	x1 = _XVector3(v1.w,v1.y,v1.z);
	x2 = _XVector3(v2.w,v2.y,v2.z);
	ret.x = getDeta(x0,x1,x2)/d;
	x0 = _XVector3(v0.x,v0.w,v0.z);
	x1 = _XVector3(v1.x,v1.w,v1.z);
	x2 = _XVector3(v2.x,v2.w,v2.z);
	ret.y = getDeta(x0,x1,x2)/d;
	x0 = _XVector3(v0.x,v0.y,v0.w);
	x1 = _XVector3(v1.x,v1.y,v1.w);
	x2 = _XVector3(v2.x,v2.y,v2.w);
	ret.z = getDeta(x0,x1,x2)/d;
	return ret;
}
_XVector3 _XMatrix4x4::resolveEquation()
{
	_XVector3 ret;
	_XMatrix3x3 m;
	m.data[0] = data[0];m.data[1] = data[1];m.data[2] = data[2];
	m.data[3] = data[4];m.data[4] = data[5];m.data[5] = data[6];
	m.data[6] = data[8];m.data[7] = data[9];m.data[8] = data[10];
	float d = m.getValue();
	if(d == 0.0f) return ret;
	m.data[0] = data[3];//m.data[1] = data[1];m.data[2] = data[2];
	m.data[3] = data[7];//m.data[4] = data[5];m.data[5] = data[6];
	m.data[6] = data[11];//m.data[7] = data[9];m.data[8] = data[10];
	ret.x = m.getValue() / d;
	m.data[0] = data[0];m.data[1] = data[3];//m.data[2] = data[2];
	m.data[3] = data[4];m.data[4] = data[7];//m.data[5] = data[6];
	m.data[6] = data[8];m.data[7] = data[11];//m.data[8] = data[10];
	ret.y = m.getValue() / d;
	/*m.data[0] = data[0];*/m.data[1] = data[1];m.data[2] = data[3];
	/*m.data[3] = data[4];*/m.data[4] = data[5];m.data[5] = data[7];
	/*m.data[6] = data[8];*/m.data[7] = data[9];m.data[8] = data[11];
	ret.z = m.getValue() / d;
	return ret;
}