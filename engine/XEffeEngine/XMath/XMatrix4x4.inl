//INLINE 函数的定义
INLINE void XMatrix4x4::loadIdentity(void)
{
	memset(data,0,16 * sizeof(float));
	data[0] = 1.0f;
	data[5] = 1.0f;
	data[10] = 1.0f;
	data[15] = 1.0f;
}
INLINE XVector3 XMatrix4x4::operator*(const XVector3 &v) const 
{
	float d = 1.0f / (data[3] * v.x + data[7] * v.y + data[11] * v.z + data[15]) ;
	return XVector3((data[0] * v.x + data[4] * v.y + data[8] * v.z + data[12]) * d,
					(data[1] * v.x + data[5] * v.y + data[9] * v.z + data[13]) * d,
					(data[2] * v.x + data[6] * v.y + data[10] * v.z + data[14]) * d);
}
INLINE XMatrix4x4 XMatrix4x4::getRotate(const XMatrix3x3 &mxtr)
{
	float scale = sqrt(((data[0] * data[0]) + (data[1] * data[1]) + (data[2] * data[2]) + 
                  (data[4] * data[4]) + (data[5] * data[5]) + (data[6] * data[6]) +
                  (data[8] * data[8]) + (data[9] * data[9]) + (data[10] * data[10])) / 3.0f);
	return XMatrix4x4(mxtr.data[0] * scale,mxtr.data[1] * scale,mxtr.data[2] * scale,data[3],
		mxtr.data[3] * scale,mxtr.data[4] * scale,mxtr.data[5] * scale,data[7],
		mxtr.data[6] * scale,mxtr.data[7] * scale,mxtr.data[8] * scale,data[11],
		data[12],data[13],data[14],data[15]);
}
INLINE float& XMatrix4x4::operator () (int x,int y) 
{
	if(x < 0 || x >= 4 || y < 0 || y >= 4)
	{
		printf("matrix access out of range\n");
		exit(1);
	}
	return data[x + y * 4];
}
INLINE float XMatrix4x4::operator () (int x,int y) const
{
	if(x < 0 || x >= 4 || y < 0 || y >= 4)
	{
		printf("matrix access out of range\n");
		exit(1);
	}
	return data[x + y * 4];
}