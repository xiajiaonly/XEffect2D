//inline 函数的定义
inline void _XVector2::reset()    //归零这个点
{
    x = 0.0f;
    y = 0.0f;
}
inline void _XVector2::set(const _XVector2 &temp)
{
	x = temp.x;
	y = temp.y;
}
inline void _XVector2::set(float a,float b)    //设置当前点的值
{
    x = a;
    y = b;
}
inline void _XVector2::add(float a,float b)    //加上某个值
{
    x += a;
    y += b;
}
inline _XVector2 _XVector2::operator + (const _XVector2& temp) const
{
    return _XVector2(x + temp.x,y + temp.y);
}
inline _XVector2 _XVector2::operator - (const _XVector2& temp) const
{
    return _XVector2(x - temp.x,y - temp.y);
}
inline _XVector2 _XVector2::operator * (const _XVector2& temp) const
{
    return _XVector2(x * temp.x,y * temp.y);
}
inline _XVector2 _XVector2::operator / (const _XVector2& temp) const
{
    if(temp.x == 0.0f || temp.y == 0.0f) return _XVector2(x,y);
    return _XVector2(x / temp.x,y / temp.y);
}
inline _XVector2 _XVector2::operator + (const float& temp) const
{
    return _XVector2(x + temp,y + temp);
}
inline _XVector2 _XVector2::operator - (const float& temp) const
{
    return _XVector2(x - temp,y - temp);
}
inline _XVector2 _XVector2::operator * (const float& temp) const
{
    return _XVector2(x * temp,y * temp);
}
inline _XVector2 _XVector2::operator / (const float& temp) const
{
    if(temp == 0.0f) return _XVector2(x,y);
    return _XVector2(x / temp,y / temp);
}
inline void _XVector2::operator += (const _XVector2& temp)
{
    x += temp.x;
    y += temp.y;
}
inline void _XVector2::operator -= (const _XVector2& temp)
{
    x -= temp.x;
    y -= temp.y;
}
inline void _XVector2::operator *= (const _XVector2& temp)
{
    x *= temp.x;
    y *= temp.y;
}
inline void _XVector2::operator /= (const _XVector2& temp)
{
    if(temp.x == 0.0f || temp.y == 0.0f) return;
    x /= temp.x;
    y /= temp.y;
}
inline void _XVector2::operator += (const float & temp)
{
    x += temp;
    y += temp;
}
inline void _XVector2::operator -= (const float & temp)
{
    x -= temp;
    y -= temp;
}
inline void _XVector2::operator *= (const float & temp)
{
    x *= temp;
    y *= temp;
}
inline void _XVector2::operator /= (const float & temp)
{
    if(temp == 0.0f) return;
    x /= temp;
    y /= temp;
}
inline float _XVector2::getLength(const _XVector2& P0) const
{
	return _hypotf((P0.x - x),(P0.y - y));
	//return hypot((P0.x - x),(P0.y - y));
    //return sqrt((P0.x - x) * (P0.x - x) + (P0.y - y) * (P0.y - y));
}
inline float _XVector2::getLength(float a,float b) const
{
	return _hypotf((a - x),(b - y));
 	//return hypot((a - x),(b - y));
    //return sqrt((a - x) * (a - x) + (b - y) * (b - y));
}
inline float _XVector2::getLengthSqure(const _XVector2& P0) const
{
	return (P0.x - x) * (P0.x - x) + (P0.y - y) * (P0.y - y);
}
inline float _XVector2::getLengthSqure(float a,float b) const
{
	return (a - x) * (a - x) + (b - y) * (b - y);
}
inline float _XVector2::getLengthSqure() const
{
	return x * x + y * y;
}
inline float _XVector2::getLength() const
{
	return _hypotf(x,y);
	//return hypot(x,y);
	//return sqrt(x * x + y * y);
}
inline float _XVector2::getAngle(const _XVector2& P0) const
{
	return getAngleXY(P0.x - x,P0.y - y);
}
inline float _XVector2::getAngle(float a,float b) const
{
	return getAngleXY(a - x,b - y);
}
inline float _XVector2::getAngle() const
{
	return getAngleXY(x,y);
}
inline _XVector2 &_XVector2::operator = (const float* temp)	//这种逻辑不严密
{
	if(temp == NULL)
	{
		x = 0.0f;
		y = 0.0f;
	}else
	{
		x = temp[0];
		y = temp[1];
	}
	return * this;
}
inline _XVector2 _XVector2::operator - () const
{
	return _XVector2(-x,-y);
}
inline _XVector2 &_XVector2::operator = (const _XVector2 &temp)
{
	if(this == &temp) return *this;
	x = temp.x;
	y = temp.y;
	return *this;
}
inline void _XVector2::normalize()
{
	float t = getLengthSqure();
	if(t != 0.0f)
	{
		t = 1.0f / sqrt(t);
		x *= t;
		y *= t;
	}
}
inline _XVector2 _XVector2::normalize() const
{
	_XVector2 ret(0.0f,0.0f);
	float r = getLength();
	if(r != 0) ret.set(x/r,y/r);
	return ret;
}