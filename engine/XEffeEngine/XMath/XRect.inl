//inline 函数的定义
inline void _XRect::set(const _XVector2& leftTop,const _XVector2& rightBottom)
{
    set(leftTop.x,leftTop.y,rightBottom.x,rightBottom.y);
}
inline _XBool _XRect::setLeft(float xl)
{
    left = xl;
    if(xl <= right) return XTrue;
    else return XFalse;
}
inline _XBool _XRect::setRight(float xr)
{
    right = xr;
    if(xr >= left) return XTrue;
    else return XFalse;
}
inline _XBool _XRect::setTop(float yt)
{
    top = yt;
    if(yt <= bottom) return XTrue;
    else return XFalse;
}
inline _XBool _XRect::setBottom(float yb)
{
    bottom = yb;
    if(yb >= top) return XTrue;
    else return XFalse;
}
inline float _XRect::getLeft() const
{
    return left;
}
inline float _XRect::getRight() const
{
    return right;
}
inline float _XRect::getTop() const
{
    return top;
}
inline float _XRect::getBottom() const
{
    return bottom;
}
inline _XBool _XRect::isCrash(const _XRect& R0) const
{
	//x,y双向投影都有交集则有公共区域
//	if((((left >= R0.left && left < R0.right) || (right <= R0.right && right > R0.left))
//		|| (((R0.left >= left && R0.left < right) || (R0.right <= right && R0.right > left))))
//		&& (((top >= R0.top && top < R0.bottom) || (bottom <= R0.bottom && bottom > R0.top))
//		|| (((R0.top >= top && R0.top < bottom) || (R0.bottom <= bottom && R0.bottom > top))))) return XTrue;
//	return XFalse;
	//与上面的结果一致，优化了一下
	if(right <= R0.left || left >= R0.right || bottom <= R0.top || top >= R0.bottom) return XFalse;
	return XTrue;
}
inline _XBool _XRect::isCrash(const _XVector2& leftTop,const _XVector2& rightBottom) const
{
	//_XRect R0(leftTop,rightBottom);
	return isCrash(_XRect(leftTop,rightBottom));
}
inline _XBool _XRect::isCrash(float xl,float yt,float xr,float yb) const
{
	//_XRect R0(xl,yt,xr,yb);
	return isCrash(_XRect(xl,yt,xr,yb));
}
inline _XBool _XRect::isInRect(const _XVector2& p0) const
{
    if(p0.x >= left && p0.x <= right && p0.y >= top && p0.y <= bottom) return XTrue;
    else return XFalse;
}
inline _XBool _XRect::isInRect(float x,float y) const
{
//    if(x >= left && x <= right && y >= top && y <= bottom)
    if(x > left && x < right && y > top && y < bottom) return XTrue;
    else return XFalse;
}
inline float _XRect::getArea() const
{
    float area = (right - left) * (bottom - top);
    if(area < 0) return -area;
    else return area;
}
inline _XVector2 _XRect::getCenter() const
{
    //_XVector2 temp;
    //temp.set((float)((left + right)/ 2.0), (float)((top + bottom)/ 2.0));
    //return temp;
    return _XVector2((left + right) * 0.5f,(top + bottom) * 0.5f);
}
inline float _XRect::getWidth() const
{
    float temp = right - left;
    if(temp < 0) return -temp;
    else return temp;
}
inline float _XRect::getHeight() const
{
    float temp = bottom - top;
    if(temp < 0) return -temp;
    else return temp;
}
inline void _XRect::setCenter(float x,float y)
{
    _XVector2 temp = getCenter();
    temp.set(x - temp.x,y - temp.y);
    set(left + temp.x,top + temp.y,right + temp.x,bottom + temp.y);
}
inline void _XRect::setCenter(const _XVector2& p)
{
    setCenter(p.x,p.y);
}