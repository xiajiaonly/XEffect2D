//INLINE 函数的定义
INLINE void XVec2::reset()    //归零这个点
{
    x = y = 0.0f;
}
INLINE void XVec2::set(const XVec2& temp)
{
	x = temp.x;
	y = temp.y;
}
INLINE void XVec2::set(float a,float b)    //设置当前点的值
{
    x = a;
    y = b;
}
INLINE void XVec2::set(float* v)    //加上某个值
{
    x = *v;
    y = *(v+ 1);
}
INLINE void XVec2::add(float a,float b)    //加上某个值
{
    x += a;
    y += b;
}
INLINE XVec2 XVec2::operator + (const XVec2& temp) const
{
	return XVec2(x + temp.x, y + temp.y);
}
INLINE XVec2 XVec2::operator - (const XVec2& temp) const
{
	return XVec2(x - temp.x, y - temp.y);
}
INLINE XVec2 XVec2::operator * (const XVec2& temp) const
{
	return XVec2(x * temp.x, y * temp.y);
}
INLINE XVec2 XVec2::operator / (const XVec2& temp) const
{
    if(temp.x == 0.0f || temp.y == 0.0f) return *this;
	return XVec2(x / temp.x, y / temp.y);
}
INLINE XVec2 XVec2::operator + (const float& temp) const
{
	return XVec2(x + temp, y + temp);
}
INLINE XVec2 XVec2::operator - (const float& temp) const
{
	return XVec2(x - temp, y - temp);
}
INLINE XVec2 XVec2::operator * (const float& temp) const
{
	return XVec2(x * temp, y * temp);
}
INLINE XVec2 XVec2::operator / (const float& temp) const
{
    if(temp == 0.0f) return *this;
    //return XVec2(x / temp,y / temp);
	return operator * (1.0f / temp);
}
INLINE void XVec2::operator += (const XVec2& temp)
{
    x += temp.x;
    y += temp.y;
}
INLINE void XVec2::operator -= (const XVec2& temp)
{
    x -= temp.x;
    y -= temp.y;
}
INLINE void XVec2::operator *= (const XVec2& temp)
{
    x *= temp.x;
    y *= temp.y;
}
INLINE void XVec2::operator /= (const XVec2& temp)
{
	if (temp.x == 0.0f || temp.y == 0.0f) return;
    x /= temp.x;
    y /= temp.y;
}
INLINE void XVec2::operator += (const float & temp)
{
    x += temp;
    y += temp;
}
INLINE void XVec2::operator -= (const float & temp)
{
    x -= temp;
    y -= temp;
}
INLINE void XVec2::operator *= (const float & temp)
{
    x *= temp;
    y *= temp;
}
INLINE void XVec2::operator /= (const float & temp)
{
    if(temp == 0.0f) return;
	//x /= temp;
	//y /= temp;
	operator *=(1.0f / temp);
}
INLINE float XVec2::getLength(const XVec2& P0) const
{
	return _hypotf((P0.x - x), (P0.y - y));
	//return hypot((P0.x - x),(P0.y - y));
    //return sqrt((P0.x - x) * (P0.x - x) + (P0.y - y) * (P0.y - y));
}
INLINE float XVec2::getLength(float a,float b) const
{
	return _hypotf((a - x), (b - y));
 	//return hypot((a - x),(b - y));
    //return sqrt((a - x) * (a - x) + (b - y) * (b - y));
}
INLINE float XVec2::getLengthSqure(const XVec2& P0) const
{
	return XMath::squareFloat(P0.x - x) + XMath::squareFloat(P0.y - y);
}
INLINE float XVec2::getLengthSqure(float a,float b) const
{
	return XMath::squareFloat(a - x) + XMath::squareFloat(b - y);
}
INLINE float XVec2::getLengthSqure() const
{
	return x * x + y * y;
}
INLINE float XVec2::getLength() const
{
	return _hypotf(x, y);
	//return hypot(x,y);
	//return sqrt(x * x + y * y);
}
INLINE float XVec2::getAngle(const XVec2& P0) const
{
	return XMath::getAngleXY(P0.x - x, P0.y - y);
}
INLINE float XVec2::getAngle(float a,float b) const
{
	return XMath::getAngleXY(a - x, b - y);
}
INLINE float XVec2::getAngle() const
{
	return XMath::getAngleXY(x, y);
}
INLINE XVec2& XVec2::operator = (const float* temp)	//这种逻辑不严密
{
	if(temp == NULL)
	{
		x = y = 0.0f;
	}else
	{
		x = temp[0];
		y = temp[1];
	}
	return * this;
}
INLINE XVec2 XVec2::operator - () const
{
	return XVec2(-x, -y);
}
//INLINE XVec2& XVec2::operator = (const XVec2& temp)
//{
//	if(this == &temp) return *this;
//	x = temp.x;
//	y = temp.y;
//	return *this;
//}
INLINE void XVec2::normalize()
{
	float t = getLengthSqure();
	if(t != 0.0f)
	{
		t = 1.0f / sqrt(t);
		x *= t;
		y *= t;
	}
}