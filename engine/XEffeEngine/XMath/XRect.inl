//INLINE 函数的定义
INLINE void XRect::set(const XVec2& leftTop,const XVec2& rightBottom)
{
    //set(leftTop.x,leftTop.y,rightBottom.x,rightBottom.y);
	left = leftTop.x;
	top = leftTop.y;
	right = rightBottom.x;
	bottom = rightBottom.y;
}
INLINE XBool XRect::setLeft(float xl)
{
    left = xl;
    return (xl <= right);
}
INLINE XBool XRect::setRight(float xr)
{
    right = xr;
    return (xr >= left);
}
INLINE XBool XRect::setTop(float yt)
{
    top = yt;
    return (yt <= bottom);
}
INLINE XBool XRect::setBottom(float yb)
{
    bottom = yb;
    return (yb >= top);
}
INLINE float XRect::getLeft() const
{
    return left;
}
INLINE float XRect::getRight() const
{
    return right;
}
INLINE float XRect::getTop() const
{
    return top;
}
INLINE float XRect::getBottom() const
{
    return bottom;
}
INLINE XBool XRect::isCrash(const XRect& R0) const
{
	//x,y双向投影都有交集则有公共区域
//	if((((left >= R0.left && left < R0.right) || (right <= R0.right && right > R0.left))
//		|| (((R0.left >= left && R0.left < right) || (R0.right <= right && R0.right > left))))
//		&& (((top >= R0.top && top < R0.bottom) || (bottom <= R0.bottom && bottom > R0.top))
//		|| (((R0.top >= top && R0.top < bottom) || (R0.bottom <= bottom && R0.bottom > top))))) return XTrue;
//	return XFalse;
	//与上面的结果一致，优化了一下
	return !(right <= R0.left || left >= R0.right || bottom <= R0.top || top >= R0.bottom);
}
INLINE XBool XRect::isCrash(const XVec2& leftTop,const XVec2& rightBottom) const
{
	//XRect R0(leftTop,rightBottom);
	return isCrash(XRect(leftTop,rightBottom));
}
INLINE XBool XRect::isCrash(float xl,float yt,float xr,float yb) const
{
	//XRect R0(xl,yt,xr,yb);
	return isCrash(XRect(xl,yt,xr,yb));
}
INLINE XBool XRect::isInRect(const XVec2& p0) const
{
	return XMath::isInAreaEx(p0.x, right, left) && XMath::isInAreaEx(p0.y, bottom, top);
}
INLINE XBool XRect::isInRect(float x,float y) const
{
//    if(x >= left && x <= right && y >= top && y <= bottom)
    return XMath::isInArea(x,left,right) && XMath::isInArea(y, top, bottom);
}
INLINE float XRect::getArea() const
{
    float area = (right - left) * (bottom - top);
    if(area < 0.0f) return -area;
    else return area;
}
INLINE XVec2 XRect::getCenter() const
{
    //XVec2 temp;
    //temp.set((float)((left + right)/ 2.0), (float)((top + bottom)/ 2.0));
    //return temp;
    return XVec2(left + right,top + bottom) * 0.5f;
}
INLINE XVec2 XRect::getSize() const
{
	return XVec2(getWidth(),getHeight());
}
INLINE float XRect::getWidth() const
{
	return abs(right - left);
	//float temp = right - left;
	//if(temp < 0) return -temp;
	//else return temp;
}
INLINE float XRect::getHeight() const
{
	return abs(bottom - top);
	//float temp = bottom - top;
	//if(temp < 0) return -temp;
	//else return temp;
}
//INLINE void XRect::setCenter(float x,float y)
//{
//    setCenter(XVec2(x,y));
//}
INLINE void XRect::setCenter(const XVec2& p)
{
    XVec2 temp = p - getCenter();
	set(left + temp.x, top + temp.y, right + temp.x, bottom + temp.y);
}