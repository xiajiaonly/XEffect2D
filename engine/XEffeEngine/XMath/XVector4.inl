//inline 函数的定义
inline void _XVector4::reset()//重置这个点
{
	x = 0.0f;y = 0.0f;z = 0.0f;w = 0.0f;
}
inline void _XVector4::add(float a,float b,float c,float d)//设置这个点的值
{
	x += a;y += b;z += c;w += d;
}
inline void _XVector4::set(float a,float b,float c,float d)//这个点的值加上一个点的值
{
	x = a;y = b;z = c;w = d;
}
inline _XVector4 _XVector4::operator - (const _XVector4& temp) const
{
	return _XVector4(x - temp.x,y - temp.y,z - temp.z,w - temp.w);
}
inline _XVector4 _XVector4::operator + (const _XVector4& temp) const
{
	return _XVector4(x + temp.x,y + temp.y,z + temp.z,w + temp.w);
}
inline void _XVector4::getRotate(float angle,const _XVector3 &v)	//向量v转动角度angle之后的四元素
{
	float sinA = sin(angle * 0.5f * ANGLE_TO_RADIAN);
	x = sinA * v.x;
	y = sinA * v.y;
	z = sinA * v.z;
	w = cos(angle * 0.5f * ANGLE_TO_RADIAN);
}
inline void _XVector4::getFromPlane(const _XVector3 &normal,const _XVector3 &point)
{//没有经过验证
	x = normal.x;
	y = normal.y;
	z = normal.z;
	w = -(point.x * x + point.y * y + point.z * z);
}
inline float _XVector4::getLength() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}
inline float _XVector4::getLengthSqure() const
{
	return x * x + y * y + z * z + w * w;
}
inline _XVector4 _XVector4::operator * (const _XVector4& v) const	//CROSS product
{
	return _XVector4(w * v.x + x * v.w + z * v.y - y * v.z,
		w * v.y + y * v.w + x * v.z - z * v.x,
		w * v.z + z * v.w + y * v.x - x * v.y,
		w * v.w - x * v.x - y * v.y - z * v.z);
}
inline _XVector4 _XVector4::operator + (const float& temp) const
{
	return _XVector4(x + temp,y + temp,z + temp,w + temp);
}
inline _XVector4 _XVector4::operator - (const float& temp) const
{
	return _XVector4(x - temp,y - temp,z - temp,w - temp);
}
inline _XVector4 _XVector4::operator * (const float& temp) const
{
	return _XVector4(x * temp,y * temp,z * temp,w * temp);
}
inline _XVector4 _XVector4::operator / (const float& temp) const
{
	return _XVector4(x / temp,y / temp,z / temp,w / temp);
}
inline void _XVector4::operator += (const float& temp)
{
	x += temp;
	y += temp;
	z += temp;
	w += temp;
}
inline void _XVector4::operator -= (const float& temp)
{
	x -= temp;
	y -= temp;
	z -= temp;
	w -= temp;
}
inline void _XVector4::operator *= (const float& temp)
{
	x *= temp;
	y *= temp;
	z *= temp;
	w *= temp;
}
inline void _XVector4::operator /= (const float& temp)
{
	x /= temp;
	y /= temp;
	z /= temp;
	w /= temp;
}
inline _XVector4 _XVector4::conjugate() const
{
	return _XVector4(-x,-y,-z,w);
}
inline _XVector4 _XVector4::inverse() const
{
	return conjugate()/getLength();
}
inline _XVector4& _XVector4::operator = (const float* temp) //这种逻辑不严密
{
	if(temp == NULL)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}else
	{
		x = temp[0];
		y = temp[1];
		z = temp[2];
		w = temp[3];
	}
	return * this;
}
inline void _XVector4::getRotateX(float ax)
{
	ax = ax * 0.5f;
	w = cos(ax);
	x = sin(ax);
	y = 0.0f;
	z = 0.0f;
}
inline void _XVector4::getRotateY(float ay)
{
	ay = ay * 0.5f;
	w = cos(ay);
	x = 0.0f;
	y = sin(ay);
	z = 0.0f;
}
inline void _XVector4::getRotateZ(float az)
{
	az = az * 0.5f;
	w = cos(az);
	x = 0.0f;
	y = 0.0f;
	z = sin(az);
}