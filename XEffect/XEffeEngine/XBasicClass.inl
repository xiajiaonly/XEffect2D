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

inline float _XVector2::getLength(const _XVector2& P0) const
{
    return sqrt((P0.x - x) * (P0.x - x) + (P0.y - y) * (P0.y - y));
}

inline float _XVector2::getLength(float a,float b) const
{
    return sqrt((a - x) * (a - x) + (b - y) * (b - y));
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
	return sqrt(x * x + y * y);
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
    x = c;
}

inline void _XRect::set(const _XVector2& leftTop,const _XVector2& rightBottom)
{
    set(leftTop.x,leftTop.y,rightBottom.x,rightBottom.y);
}

inline bool _XRect::setLeft(float xl)
{
    left = xl;
    if(xl <= right) return true;
    else return false;
}

inline bool _XRect::setRight(float xr)
{
    right = xr;
    if(xr >= left) return true;
    else return false;
}

inline bool _XRect::setTop(float yt)
{
    top = yt;
    if(yt <= bottom) return true;
    else return false;
}

inline bool _XRect::setBottom(float yb)
{
    bottom = yb;
    if(yb >= top) return true;
    else return false;
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

inline bool _XRect::isCrash(const _XRect& R0) const
{
	//x,y双向投影都有交集则有公共区域
//	if((((left >= R0.left && left < R0.right) || (right <= R0.right && right > R0.left))
//		|| (((R0.left >= left && R0.left < right) || (R0.right <= right && R0.right > left))))
//		&& (((top >= R0.top && top < R0.bottom) || (bottom <= R0.bottom && bottom > R0.top))
//		|| (((R0.top >= top && R0.top < bottom) || (R0.bottom <= bottom && R0.bottom > top))))) return true;
//	return false;
	//与上面的结果一致，优化了一下
	if(right <= R0.left || left >= R0.right || bottom <= R0.top || top >= R0.bottom) return false;
	return true;
}

inline bool _XRect::isCrash(const _XVector2& leftTop,const _XVector2& rightBottom) const
{
	_XRect R0(leftTop,rightBottom);
	return isCrash(R0);
}

inline bool _XRect::isCrash(float xl,float yt,float xr,float yb) const
{
	_XRect R0(xl,yt,xr,yb);
	return isCrash(R0);
}

inline bool _XRect::isInRect(const _XVector2& p0) const
{
    if(p0.x >= left && p0.x <= right && p0.y >= top && p0.y <= bottom) 
    {
        return true;
    }else 
    {
        return false;
    }
}

inline bool _XRect::isInRect(float x,float y) const
{
//    if(x >= left && x <= right && y >= top && y <= bottom)
    if(x > left && x < right && y > top && y < bottom)
    {
        return true;
    }else 
    {
        return false;
    }
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
    return _XVector2((float)((left + right)/ 2.0), (float)((top + bottom)/ 2.0));
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

inline void _XVector2I::set(int a,int b)
{
    x = a;
    y = b;
}

inline void _XIRect::set(const _XVector2I& p0,const _XVector2I& p1)
{
    set(p0.x,p0.y,p1.x,p1.y);
}

inline void _XIRect::set(const _XVector2& p0,const _XVector2& p1)
{
    set((int)p0.x,(int)p0.y,(int)p1.x,(int)p1.y);
}

inline bool _XIRect::isInRect(const _XVector2I& p0) const
{
    if(p0.x >= left && p0.x <= right
        && p0.y >= top && p0.y <= bottom) return true;
    else return false;
}

inline int _XIRect::getArea() const
{
    int area = (right - left) * (bottom - top);
    if(area < 0) return -area;
    else return area;
}

inline _XVector2 _XIRect::getCenter() const
{
    //_XVector2 temp;
    //temp.set((float)((left + right)/ 2.0f), (float)((top + bottom)/ 2.0f));
    //return temp;
    return _XVector2((float)((left + right)/ 2.0f), (float)((top + bottom)/ 2.0f));
}

inline int _XIRect::getWidth() const
{
    int temp = right - left;
    if(temp < 0) return -temp;
    else return temp;
}

inline int _XIRect::getHeight() const
{
    int temp = bottom - top;
    if(temp < 0) return -temp;
    else return temp;
}

inline void _XIRect::setCenter(int x,int y)
{
    _XVector2 temp = getCenter();
    temp.set(x - temp.x,y - temp.y);
    set((int)(left + temp.y),(int)(top + temp.y),(int)(right + temp.x),(int)(bottom + temp.y));
}

inline void _XIRect::setCenter(const _XVector2I& p)
{
    setCenter(p.x,p.y);
}

inline void _XCColor::setColor(unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
    cR = r;
    cG = g;
    cB = b;
    cA = a;
}

#ifdef OS_WINDOWS
inline void _XCritical::Lock()
{ 
    EnterCriticalSection( &m_sec ); 
} 

inline void _XCritical::Unlock()
{ 
    LeaveCriticalSection( &m_sec ); 
} 
#endif

inline float _XLine::getLineLength() const
{
	return m_startPosition.getLength(m_endPosition);
}

inline float _XLine::getLineAngle() const
{
	return m_angle;
}

inline _XVector2 _XLine::getStartPosition() const
{
	return m_startPosition;
}

inline _XVector2 _XLine::getEndPosition() const
{
	return m_endPosition;
}

inline float _XVector2I::getLength(const _XVector2I& P0) const
{
    return (float)(sqrt((double)((P0.x - x) * (P0.x - x) + (P0.y - y) * (P0.y - y))));
}

inline float _XVector2I::getAngle(const _XVector2I& P0) const
{
	return getAngleXY(P0.x - x,P0.y - y);
}
