//inline 函数的定义
//inline void XVector2I::set(int a,int b)
//{
//    x = a;
//    y = b;
//}
//inline void XIRect::set(const XVector2I& p0,const XVector2I& p1)
//{
//    set(p0.x,p0.y,p1.x,p1.y);
//}
//inline void XIRect::set(const XVector2& p0,const XVector2& p1)
//{
//    set((int)p0.x,(int)p0.y,(int)p1.x,(int)p1.y);
//}
//inline XBool XIRect::isInRect(const XVector2I& p0) const
//{
//    if(p0.x >= left && p0.x <= right
//        && p0.y >= top && p0.y <= bottom) return XTrue;
//    else return XFalse;
//}
//inline int XIRect::getArea() const
//{
//    int area = (right - left) * (bottom - top);
//    if(area < 0) return -area;
//    else return area;
//}
//inline XVector2 XIRect::getCenter() const
//{
//    //XVector2 temp;
//    //temp.set((float)((left + right)/ 2.0f), (float)((top + bottom)/ 2.0f));
//    //return temp;
//    return XVector2((float)((left + right)/ 2.0f), (float)((top + bottom)/ 2.0f));
//}
//inline int XIRect::getWidth() const
//{
//    int temp = right - left;
//    if(temp < 0) return -temp;
//    else return temp;
//}
//inline int XIRect::getHeight() const
//{
//    int temp = bottom - top;
//    if(temp < 0) return -temp;
//    else return temp;
//}
//inline void XIRect::setCenter(int x,int y)
//{
//    XVector2 temp = getCenter();
//    temp.set(x - temp.x,y - temp.y);
//    set((int)(left + temp.y),(int)(top + temp.y),(int)(right + temp.x),(int)(bottom + temp.y));
//}
//inline void XIRect::setCenter(const XVector2I& p)
//{
//    setCenter(p.x,p.y);
//}
//inline float XVector2I::getLength(const XVector2I& P0) const
//{
//    return (float)(sqrt((double)((P0.x - x) * (P0.x - x) + (P0.y - y) * (P0.y - y))));
//}
//
//inline float XVector2I::getAngle(const XVector2I& P0) const
//{
//	return XMath::getAngleXY(P0.x - x,P0.y - y);
//}