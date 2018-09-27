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
class XRectEx;
//单精度矩形
class XRect
{
public:
    float left;
    float top;
    float right;
    float bottom;
public:
	void setAndOrder(float xl, float yt, float xr, float yb)
	{//设置并排序，满足屏幕坐标系的要求
		if (xl > xr) XMath::swapTwoSum(xl, xr);
		if (yt > yb) XMath::swapTwoSum(yt, yb);
		left = xl;
		top = yt;
		right = xr;
		bottom = yb;
	}
	void set(float xl, float yt, float xr, float yb)
	{
		//if(xl > xr) XMath::swapTwoSum(&xl,&xr);
		//if(yt > yb) XMath::swapTwoSum(&yt,&yb);
		left = xl;
		top = yt;
		right = xr;
		bottom = yb;
	}
    void set(float ltD, float rbD)
	{
		//if(xl > xr) XMath::swapTwoSum(&xl,&xr);
		//if(yt > yb) XMath::swapTwoSum(&yt,&yb);
		left = ltD;
		top = ltD;
		right = rbD;
		bottom = rbD;
	}
	void set(const XVec2& leftTop, const XVec2& rightBottom);
	//左上角+全尺寸
	void set(float _left, float _top, const XVec2& size)
	{
		left = _left;
		top = _top;
		right = left + size.x;
		bottom = top + size.y;
	}
    XBool setLeft(float xl);
    XBool setRight(float xr);
    XBool setTop(float yt);
    XBool setBottom(float yb);
    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
	XRect()
		:left(0.0f),top(0.0f),right(0.0f),bottom(0.0f)
	{}
	//四条边
	XRect(float xl,float yt,float xr,float yb)
		:left(xl),top(yt),right(xr),bottom(yb)
	{}
	//特殊形式的矩形
	XRect(float ltD,float rbD)
		:left(ltD),top(ltD),right(rbD),bottom(rbD)
	{}
	//左上角+右下角
	XRect(const XVec2& leftTop,const XVec2& rightBottom)
		:left(leftTop.x),top(leftTop.y),right(rightBottom.x),bottom(rightBottom.y)
	{}
	//左上角+全尺寸
	XRect(float _left,float _top,const XVec2& size)
		:left(_left),top(_top),right(_left + size.x),bottom(_top + size.y)
	{}
	//中心点+全尺寸
	XRect(const XVec2& c,float w,float h)
		:left(c.x - w * 0.5f),top(c.y - h * 0.5f),right(c.x + w * 0.5f),bottom(c.y + h * 0.5f)
	{}
	XRect(const XRectEx& r);
	//注意下面的函数式针对于不会旋转的矩形的碰撞判断，如果是旋转的需要进一步修改代码
    XBool isCrash(const XRect& R0) const;
    XBool isCrash(const XVec2& leftTop,const XVec2& rightBottom) const;
    XBool isCrash(float xl,float yt,float xr,float yb) const;
    XBool isInRect(const XVec2& p0) const;
    XBool isInRect(float x,float y) const;
    float getArea() const;
    XVec2 getCenter() const;
    XVec2 getSize() const;
	//获取左上角点
	XVec2 getLT() const { return XVec2(left, top); }
	//获取右下角点
	XVec2 getRB() const { return XVec2(right, bottom); }
	//获取左下角点
	XVec2 getLB() const { return XVec2(left, bottom); }
	//获取右上角点
	XVec2 getRT() const { return XVec2(right, top); }
	float getXCenter() const { return (left + right) * 0.5f; }
	float getYCenter() const { return (top + bottom) * 0.5f; }
    float getWidth() const;
    float getHeight() const;
//    void setCenter(float x,float y);
    void setCenter(const XVec2& p);
	void scaleCenter(float scale)	//以中心为基准点进行缩放
	{
		XVec2 c = getCenter();
		XVec2 s(getWidth() * scale * 0.5f, getHeight() * scale * 0.5f);
		left = c.x - s.x;
		right = c.x + s.x;
		top = c.y - s.y;
		bottom = c.y + s.y;
	}
	void scaleLeftTop(float scale)	//以左上角为基准点进行缩放
	{
		right = (right - left) * scale + left;
		bottom = (bottom - top) * scale + top;
	}
};
//这里是使用中心点和大小来描绘一个矩形的类
class XRectEx
{
private:
public:
	XRectEx()
		:x(0), y(0), width(0), height(0)
	{}
	XRectEx(float posx, float posy, float w, float h)
		:x(posx), y(posy), width(w), height(h)
	{}
	XRectEx(const XRect& r)
		:x(r.getXCenter()), y(r.getYCenter()), width(r.getWidth()), height(r.getHeight())
	{}
	float x;
	float y;
	float width;
	float height;
	XVec2 getCenter()const { return XVec2(x + width * 0.5f, y + height * 0.5f); }
	float getLeft()const {return x - width * 0.5f; }
	float getRight()const { return x + width * 0.5f; }
	float getTop()const { return y - height * 0.5f; }
	float getBottom()const { return y + height * 0.5f; }
};
#if WITH_INLINE_FILE
#include "XRect.inl"
#endif
}
#endif