#ifndef _JIA_XRECT_
#define _JIA_XRECT_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "..\XOSDefine.h"
#include "..\XCommonDefine.h"
#include "XVector2.h"
namespace XE{
//单精度矩形
class XRect
{
public:
    float left;
    float top;
    float right;
    float bottom;
public:
	void setAndOrder(float xl = 0.0f,float yt = 0.0f,float xr = 0.0f,float yb = 0.0f)
	{//设置并排序，满足屏幕坐标系的要求
		if(xl > xr) XMath::swapTwoSum(xl,xr);
		if(yt > yb) XMath::swapTwoSum(yt,yb);
		left = xl;
		top = yt;
		right = xr;
		bottom = yb;
	}
    void set(float xl = 0.0f,float yt = 0.0f,float xr = 0.0f,float yb = 0.0f)
	{
		//if(xl > xr) XMath::swapTwoSum(&xl,&xr);
		//if(yt > yb) XMath::swapTwoSum(&yt,&yb);
		left = xl;
		top = yt;
		right = xr;
		bottom = yb;
	}
    void set(const XVector2& leftTop,const XVector2& rightBottom);
    XBool setLeft(float xl);
    XBool setRight(float xr);
    XBool setTop(float yt);
    XBool setBottom(float yb);
    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
	XRect(float xl =0.0f,float yt =0.0f,float xr =0.0f,float yb =0.0f)
	:left(xl),top(yt),right(xr),bottom(yb)
	{}
	XRect(const XVector2& leftTop,const XVector2& rightBottom)
	:left(leftTop.x),top(leftTop.y),right(rightBottom.x),bottom(rightBottom.y)
	{}
    //XRect()
    //    :left(0.0f)
    //    ,top(0.0f)
    //    ,right(0.0f)
    //    ,bottom(0.0f)
    //{
    //}
	//注意下面的函数式针对于不会旋转的矩形的碰撞判断，如果是旋转的需要进一步修改代码
    XBool isCrash(const XRect& R0) const;
    XBool isCrash(const XVector2& leftTop,const XVector2& rightBottom) const;
    XBool isCrash(float xl,float yt,float xr,float yb) const;
    XBool isInRect(const XVector2& p0) const;
    XBool isInRect(float x,float y) const;
    float getArea() const;
    XVector2 getCenter() const;
    XVector2 getSize() const;
	float getXCenter() const {return (left + right) * 0.5f;}
	float getYCenter() const {return (top + bottom) * 0.5f;}
    float getWidth() const;
    float getHeight() const;
    void setCenter(float x,float y);
    void setCenter(const XVector2& p);
};
//这里是使用中心点和大小来描绘一个矩形的类
class XRectEx
{
private:
public:
	XRectEx()
		:x(0),y(0),width(0),height(0)
	{}
	XRectEx(float posx,float posy,float w,float h)
		:x(posx),y(posy),width(w),height(h)
	{}
	float x;
	float y;
	float width;
	float height;
};
#if WITH_INLINE_FILE
#include "XRect.inl"
#endif
}
#endif