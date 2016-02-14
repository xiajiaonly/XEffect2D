//INLINE 函数的定义
INLINE void XVector4::reset()//重置这个点
{
	x = y = z = w = 0.0f;
}
INLINE void XVector4::add(float a,float b,float c,float d)//设置这个点的值
{
	x += a;y += b;z += c;w += d;
}
INLINE void XVector4::set(float a,float b,float c,float d)//这个点的值加上一个点的值
{
	x = a;y = b;z = c;w = d;
}
INLINE XVector4 XVector4::operator - (const XVector4& temp) const
{
	return XVector4(x - temp.x,y - temp.y,z - temp.z,w - temp.w);
}
INLINE XVector4 XVector4::operator + (const XVector4& temp) const
{
	return XVector4(x + temp.x,y + temp.y,z + temp.z,w + temp.w);
}
INLINE void XVector4::getRotate(float angle,const XVector3 &v)	//向量v转动角度angle之后的四元素
{
	float sinA = sin(angle * 0.5f * DEGREE2RADIAN);
	x = sinA * v.x;
	y = sinA * v.y;
	z = sinA * v.z;
	w = cos(angle * 0.5f * DEGREE2RADIAN);
}
INLINE void XVector4::getFromPlane(const XVector3 &normal,const XVector3 &point)
{//没有经过验证
	x = normal.x;
	y = normal.y;
	z = normal.z;
	w = -(point.x * x + point.y * y + point.z * z);
}
INLINE void XVector4::getFromPlane(const XVector3 &p0,const XVector3 &p1,const XVector3 &p2)
{
	x = p0.y*(p1.z - p2.z) + p1.y*(p2.z - p0.z) + p2.y*(p0.z-p1.z);
	y = p0.z*(p1.x - p2.x) + p1.z*(p2.x - p0.x) + p2.z*(p0.x-p1.x);
	z = p0.x*(p1.y - p2.y) + p1.x*(p2.y - p0.y) + p2.x*(p0.y-p1.y);
	w = -(p0.x*(p1.y*p2.z - p2.y*p1.z) +
		p1.x*(p2.y*p0.z - p0.y*p2.z) +
		p2.x*(p0.y*p1.z - p1.y*p0.z));
}
INLINE float XVector4::getLength() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}
INLINE float XVector4::getLengthSqure() const
{
	return x * x + y * y + z * z + w * w;
}
INLINE XVector4 XVector4::operator * (const XVector4& v) const	//CROSS product
{
	return XVector4(w * v.x + x * v.w + z * v.y - y * v.z,
		w * v.y + y * v.w + x * v.z - z * v.x,
		w * v.z + z * v.w + y * v.x - x * v.y,
		w * v.w - x * v.x - y * v.y - z * v.z);
}
INLINE XVector4 XVector4::operator + (const float& temp) const
{
	return XVector4(x + temp,y + temp,z + temp,w + temp);
}
INLINE XVector4 XVector4::operator - (const float& temp) const
{
	return XVector4(x - temp,y - temp,z - temp,w - temp);
}
INLINE XVector4 XVector4::operator * (const float& temp) const
{
	return XVector4(x * temp,y * temp,z * temp,w * temp);
}
INLINE XVector4 XVector4::operator / (const float& temp) const
{
	if(temp == 0.0f) return *this;
	return operator * (1.0f / temp);
}
INLINE void XVector4::operator += (const float& temp)
{
	x += temp;
	y += temp;
	z += temp;
	w += temp;
}
INLINE void XVector4::operator -= (const float& temp)
{
	x -= temp;
	y -= temp;
	z -= temp;
	w -= temp;
}
INLINE void XVector4::operator *= (const float& temp)
{
	x *= temp;
	y *= temp;
	z *= temp;
	w *= temp;
}
INLINE void XVector4::operator /= (const float& temp)
{
	if(temp == 0.0f) return;
	operator *= (1.0f / temp);
}
INLINE XVector4 XVector4::conjugate() const
{
	return XVector4(-x,-y,-z,w);
}
INLINE XVector4 XVector4::inverse() const
{
	return conjugate()/getLength();
}
INLINE XVector4& XVector4::operator = (const float* temp) //这种逻辑不严密
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
INLINE void XVector4::getRotateX(float ax)
{
	ax = ax * 0.5f;
	w = cos(ax);
	x = sin(ax);
	y = 0.0f;
	z = 0.0f;
}
INLINE void XVector4::getRotateY(float ay)
{
	ay = ay * 0.5f;
	w = cos(ay);
	x = 0.0f;
	y = sin(ay);
	z = 0.0f;
}
INLINE void XVector4::getRotateZ(float az)
{
	az = az * 0.5f;
	w = cos(az);
	x = 0.0f;
	y = 0.0f;
	z = sin(az);
}