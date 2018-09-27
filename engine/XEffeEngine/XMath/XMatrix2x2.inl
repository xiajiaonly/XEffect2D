INLINE void XMat2::loadIdentity()
{
	data[0] = 1.0f;
	data[1] = 0.0f;
	data[2] = 0.0f;
	data[3] = 1.0f;
}
INLINE XMat2 XMat2::anti()		//求反置矩阵
{
	return XMat2(data[0], data[3], data[2], data[1]);
}
//INLINE void XMat2::anti()
//{
//	float temp = data[3];
//	data[3] = data[1];
//	data[1] = temp;
//}
INLINE float XMat2::operator () (int x,int y) const
{
	if(x < 0 || x >= 2 || y < 0 || y >= 2)
	{
		printf("matrix access out of range\n");
		exit(1);
	}
	return data[x + y * 2];
}
INLINE float& XMat2::operator () (int x,int y) 
{
	if(x < 0 || x >= 2 || y < 0 || y >= 2)
	{
		printf("matrix access out of range\n");
		exit(1);
	}
	return data[x + y * 2];
}
INLINE XVec2 XMat2::operator *(const XVec2& v)	//v必须是列向量，否则没有意义，OpenGL中默认为列向量
{//应用与坐标系变化中的缩放和旋转
	return XVec2(v.x * data[0] + v.y * data[1], v.x * data[2] + v.y * data[3]);
}
INLINE XMat2 XMat2::operator*(const XMat2 & rhs) const
{
	return XMat2(data[0] * rhs.data[0] + data[1] * rhs.data[2],
		data[0] * rhs.data[1] + data[1] * rhs.data[3],

		data[2] * rhs.data[0] + data[3] * rhs.data[2],
		data[2] * rhs.data[1] + data[3] * rhs.data[3]);
}
INLINE float XMat2::getValue() 
{
	return data[0] * data[3] - data[1] * data[2];
}