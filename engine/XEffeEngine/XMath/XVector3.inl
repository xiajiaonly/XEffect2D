//inline 函数的定义
inline void _XVector3::reset()//重置这个点
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}
inline void _XVector3::add(float a,float b,float c)//设置这个点的值
{
    x += a;
    y += b;
    z += c;
}
inline void _XVector3::set(float a,float b,float c)//这个点的值加上一个点的值
{
    x = a;
    y = b;
    z = c;
}
inline _XVector3 _XVector3::operator + (const _XVector3& temp) const
{
	return _XVector3(x + temp.x,y + temp.y,z + temp.z);
}
inline _XVector3 _XVector3::operator - (const _XVector3& temp) const
{
	return _XVector3(x - temp.x,y - temp.y,z - temp.z);
}
inline _XVector3 _XVector3::operator * (const _XVector3& temp) const
{
	return _XVector3(y * temp.z - z * temp.y,
			z * temp.x - x * temp.z,
			x * temp.y - y * temp.x);
}
inline _XVector3 _XVector3::operator / (const _XVector3& temp) const
{
	if(temp.x == 0.0f || temp.y == 0.0f || temp.z == 0.0f) return _XVector3(x,y,z);
	return _XVector3(x / temp.x,y / temp.y,z / temp.z);
}
inline _XVector3 _XVector3::operator + (const float& temp) const
{
	return _XVector3(x + temp,y + temp,z + temp);
}
inline _XVector3 _XVector3::operator - (const float& temp) const
{
	return _XVector3(x - temp,y - temp,z - temp);
}
inline _XVector3 _XVector3::operator * (const float& temp) const
{
	return _XVector3(x * temp,y * temp,z * temp);
}
inline _XVector3 _XVector3::operator / (const float& temp) const
{
	if(temp == 0.0f) return *this;
	return operator *(1.0f / temp);
}
inline void _XVector3::operator += (const _XVector3& temp)
{
	x += temp.x;
	y += temp.y;
	z += temp.z;
}
inline void _XVector3::operator -= (const _XVector3& temp)
{
	x -= temp.x;
	y -= temp.y;
	z -= temp.z;
}
inline void _XVector3::operator *= (const _XVector3& temp)
{
	float tx = x,ty = y,tz = z;
	x = ty * temp.z - tz * temp.y;
	y = tz * temp.x - tx * temp.z;
	z = tx * temp.y - ty * temp.x;
}
inline void _XVector3::operator /= (const _XVector3& temp)
{
	if(temp.x == 0.0f || temp.y == 0.0f || temp.z == 0.0f) return;
	x /= temp.x;
	y /= temp.y;
	z /= temp.z;
}
inline void _XVector3::operator += (const float& temp)
{
	x += temp;
	y += temp;
	z += temp;
}
inline void _XVector3::operator -= (const float& temp)
{
	x -= temp;
	y -= temp;
	z -= temp;
}
inline void _XVector3::operator *= (const float& temp)
{
	x *= temp;
	y *= temp;
	z *= temp;
}
inline void _XVector3::operator /= (const float& temp)
{
	if(temp == 0.0f) return;
	operator *=(1.0f/temp);
}
inline float _XVector3::getLength(const _XVector3& P0) const
{
	return sqrtf(squareFloat(x - P0.x) + squareFloat(y - P0.y) + squareFloat(z - P0.z));
}
inline float _XVector3::getLength(float a,float b,float c) const
{
	return sqrtf(squareFloat(x - a) + squareFloat(y - b) + squareFloat(z - c));
}
inline float _XVector3::getLength() const
{
	return sqrtf(x * x + y * y + z * z);
}
inline float _XVector3::getLengthSqure(const _XVector3& P0) const
{
	return squareFloat(x - P0.x) + squareFloat(y - P0.y) + squareFloat(z - P0.z);
}
inline float _XVector3::getLengthSqure(float a,float b,float c) const
{
	return squareFloat(x - a) + squareFloat(y - b) + squareFloat(z - c);
}
inline float _XVector3::getLengthSqure() const
{
	return x * x + y * y + z * z;
}
inline _XVector3 _XVector3::operator - () const
{
	return _XVector3(-x,-y,-z);
}
inline _XVector3& _XVector3::operator = (const float* temp)
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
inline _XVector3 &_XVector3::operator = (const _XVector3 &temp)
{
	if(this == &temp) return *this;
	x = temp.x;
	y = temp.y;
	z = temp.z;
	return *this;
}
inline void _XVector3::normalize()
{
	float t = getLengthSqure();
	if(t != 0.0f)
	{
		operator *= (1.0f / sqrt(t));
	}
}
inline float _XVector3::dot(const _XVector3& temp) const
{//点乘，集合意义:a.dot(b) = |a|*|b|*cos(夹角)
	return x * temp.x + y * temp.y + z * temp.z;
}