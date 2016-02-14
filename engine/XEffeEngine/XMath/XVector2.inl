//INLINE 函数的定义
INLINE void XVector2::reset()    //归零这个点
{
    x = 0.0f;
    y = 0.0f;
}
INLINE void XVector2::set(const XVector2 &temp)
{
	x = temp.x;
	y = temp.y;
}
INLINE void XVector2::set(float a,float b)    //设置当前点的值
{
    x = a;
    y = b;
}
INLINE void XVector2::add(float a,float b)    //加上某个值
{
    x += a;
    y += b;
}
INLINE XVector2 XVector2::operator + (const XVector2& temp) const
{
    return XVector2(x + temp.x,y + temp.y);
}
INLINE XVector2 XVector2::operator - (const XVector2& temp) const
{
    return XVector2(x - temp.x,y - temp.y);
}
INLINE XVector2 XVector2::operator * (const XVector2& temp) const
{
    return XVector2(x * temp.x,y * temp.y);
}
INLINE XVector2 XVector2::operator / (const XVector2& temp) const
{
    if(temp.x == 0.0f || temp.y == 0.0f) return XVector2(x,y);
    return XVector2(x / temp.x,y / temp.y);
}
INLINE XVector2 XVector2::operator + (const float& temp) const
{
    return XVector2(x + temp,y + temp);
}
INLINE XVector2 XVector2::operator - (const float& temp) const
{
    return XVector2(x - temp,y - temp);
}
INLINE XVector2 XVector2::operator * (const float& temp) const
{
    return XVector2(x * temp,y * temp);
}
INLINE XVector2 XVector2::operator / (const float& temp) const
{
    if(temp == 0.0f) return *this;
    //return XVector2(x / temp,y / temp);
	return operator * (1.0f/temp);
}
INLINE void XVector2::operator += (const XVector2& temp)
{
    x += temp.x;
    y += temp.y;
}
INLINE void XVector2::operator -= (const XVector2& temp)
{
    x -= temp.x;
    y -= temp.y;
}
INLINE void XVector2::operator *= (const XVector2& temp)
{
    x *= temp.x;
    y *= temp.y;
}
INLINE void XVector2::operator /= (const XVector2& temp)
{
    if(temp.x == 0.0f || temp.y == 0.0f) return;
    x /= temp.x;
    y /= temp.y;
}
INLINE void XVector2::operator += (const float & temp)
{
    x += temp;
    y += temp;
}
INLINE void XVector2::operator -= (const float & temp)
{
    x -= temp;
    y -= temp;
}
INLINE void XVector2::operator *= (const float & temp)
{
    x *= temp;
    y *= temp;
}
INLINE void XVector2::operator /= (const float & temp)
{
    if(temp == 0.0f) return;
	//x /= temp;
	//y /= temp;
	operator *=(1.0f/temp);
}
INLINE float XVector2::getLength(const XVector2& P0) const
{
	return _hypotf((P0.x - x),(P0.y - y));
	//return hypot((P0.x - x),(P0.y - y));
    //return sqrt((P0.x - x) * (P0.x - x) + (P0.y - y) * (P0.y - y));
}
INLINE float XVector2::getLength(float a,float b) const
{
	return _hypotf((a - x),(b - y));
 	//return hypot((a - x),(b - y));
    //return sqrt((a - x) * (a - x) + (b - y) * (b - y));
}
INLINE float XVector2::getLengthSqure(const XVector2& P0) const
{
	return XMath::squareFloat(P0.x - x) + XMath::squareFloat(P0.y - y);
}
INLINE float XVector2::getLengthSqure(float a,float b) const
{
	return XMath::squareFloat(a - x) + XMath::squareFloat(b - y);
}
INLINE float XVector2::getLengthSqure() const
{
	return x * x + y * y;
}
INLINE float XVector2::getLength() const
{
	return _hypotf(x,y);
	//return hypot(x,y);
	//return sqrt(x * x + y * y);
}
INLINE float XVector2::getAngle(const XVector2& P0) const
{
	return XMath::getAngleXY(P0.x - x,P0.y - y);
}
INLINE float XVector2::getAngle(float a,float b) const
{
	return XMath::getAngleXY(a - x,b - y);
}
INLINE float XVector2::getAngle() const
{
	return XMath::getAngleXY(x,y);
}
INLINE XVector2 &XVector2::operator = (const float* temp)	//这种逻辑不严密
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
INLINE XVector2 XVector2::operator - () const
{
	return XVector2(-x,-y);
}
//INLINE XVector2 &XVector2::operator = (const XVector2 &temp)
//{
//	if(this == &temp) return *this;
//	x = temp.x;
//	y = temp.y;
//	return *this;
//}
INLINE void XVector2::normalize()
{
	float t = getLengthSqure();
	if(t != 0.0f)
	{
		t = 1.0f / sqrt(t);
		x *= t;
		y *= t;
	}
}