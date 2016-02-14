//INLINE 函数的定义
INLINE void XVector3::reset()//重置这个点
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}
INLINE void XVector3::add(float a,float b,float c)//设置这个点的值
{
    x += a;
    y += b;
    z += c;
}
INLINE void XVector3::set(float a,float b,float c)//这个点的值加上一个点的值
{
    x = a;
    y = b;
    z = c;
}
INLINE XVector3 XVector3::operator + (const XVector3& temp) const
{
	return XVector3(x + temp.x,y + temp.y,z + temp.z);
}
INLINE XVector3 XVector3::operator - (const XVector3& temp) const
{
	return XVector3(x - temp.x,y - temp.y,z - temp.z);
}
INLINE XVector3 XVector3::operator * (const XVector3& temp) const
{
	return XVector3(y * temp.z - z * temp.y,
			z * temp.x - x * temp.z,
			x * temp.y - y * temp.x);
}
INLINE XVector3 XVector3::operator / (const XVector3& temp) const
{
	if(temp.x == 0.0f || temp.y == 0.0f || temp.z == 0.0f) return XVector3(x,y,z);
	return XVector3(x / temp.x,y / temp.y,z / temp.z);
}
INLINE XVector3 XVector3::operator + (const float& temp) const
{
	return XVector3(x + temp,y + temp,z + temp);
}
INLINE XVector3 XVector3::operator - (const float& temp) const
{
	return XVector3(x - temp,y - temp,z - temp);
}
INLINE XVector3 XVector3::operator * (const float& temp) const
{
	return XVector3(x * temp,y * temp,z * temp);
}
INLINE XVector3 XVector3::operator / (const float& temp) const
{
	if(temp == 0.0f) return *this;
	return operator *(1.0f / temp);
}
INLINE void XVector3::operator += (const XVector3& temp)
{
	x += temp.x;
	y += temp.y;
	z += temp.z;
}
INLINE void XVector3::operator -= (const XVector3& temp)
{
	x -= temp.x;
	y -= temp.y;
	z -= temp.z;
}
INLINE void XVector3::operator *= (const XVector3& temp)
{
	float tx = x,ty = y,tz = z;
	x = ty * temp.z - tz * temp.y;
	y = tz * temp.x - tx * temp.z;
	z = tx * temp.y - ty * temp.x;
}
INLINE void XVector3::operator /= (const XVector3& temp)
{
	if(temp.x == 0.0f || temp.y == 0.0f || temp.z == 0.0f) return;
	x /= temp.x;
	y /= temp.y;
	z /= temp.z;
}
INLINE void XVector3::operator += (const float& temp)
{
	x += temp;
	y += temp;
	z += temp;
}
INLINE void XVector3::operator -= (const float& temp)
{
	x -= temp;
	y -= temp;
	z -= temp;
}
INLINE void XVector3::operator *= (const float& temp)
{
	x *= temp;
	y *= temp;
	z *= temp;
}
INLINE void XVector3::operator /= (const float& temp)
{
	if(temp == 0.0f) return;
	operator *=(1.0f/temp);
}
INLINE float XVector3::getLength(const XVector3& P0) const
{
	return sqrtf(XMath::squareFloat(x - P0.x) + XMath::squareFloat(y - P0.y) + XMath::squareFloat(z - P0.z));
}
INLINE float XVector3::getLength(float a,float b,float c) const
{
	return sqrtf(XMath::squareFloat(x - a) + XMath::squareFloat(y - b) + XMath::squareFloat(z - c));
}
INLINE float XVector3::getLength() const
{
	return sqrtf(x * x + y * y + z * z);
}
INLINE float XVector3::getLengthSqure(const XVector3& P0) const
{
	return XMath::squareFloat(x - P0.x) + XMath::squareFloat(y - P0.y) + XMath::squareFloat(z - P0.z);
}
INLINE float XVector3::getLengthSqure(float a,float b,float c) const
{
	return XMath::squareFloat(x - a) + XMath::squareFloat(y - b) + XMath::squareFloat(z - c);
}
INLINE float XVector3::getLengthSqure() const
{
	return x * x + y * y + z * z;
}
INLINE XVector3 XVector3::operator - () const
{
	return XVector3(-x,-y,-z);
}
INLINE XVector3& XVector3::operator = (const float* temp)
{
	if(temp == NULL)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}else
	{
		x = temp[0];
		y = temp[1];
		z = temp[2];
	}
	return * this;
}
INLINE XVector3 &XVector3::operator = (const XVector3 &temp)
{
	if(this == &temp) return *this;
	x = temp.x;
	y = temp.y;
	z = temp.z;
	return *this;
}
INLINE void XVector3::normalize()
{
	float t = getLengthSqure();
	if(t != 0.0f)
	{
		operator *= (1.0f / sqrt(t));
	}
}
INLINE float XVector3::dot(const XVector3& temp) const
{//点乘，集合意义:a.dot(b) = |a|*|b|*cos(夹角)
	return x * temp.x + y * temp.y + z * temp.z;
}