#ifndef _JIA_XBASICCLASS_
#define _JIA_XBASICCLASS_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:        2011.4.9
//--------------------------------
#include "XBasicFun.h"
//#include <math.h>
//#include "stdio.h"

#include "XMath/XVector2.h"
#include "XMath/XVector3.h" 
#include "XMath/XVector4.h" 
#include "XMath/XLine.h"
#include "XMath/XMatrix2x2.h"
#include "XMath/XMatrix3x3.h"
#include "XMath/XMatrix4x4.h"
#include "XMath/XRect.h"
#include "XMath/XCColor.h"
#include "XMath/XFColor.h"
#include "XCritical.h" 
#include "XThread.h" 
//整形索引
class _XVectorIndex3
{
public:
    int x;
    int y;
    int z;

	_XVectorIndex3()
		:x(0),y(0),z(0)
	{}
	_XVectorIndex3(int a,int b,int c)
		:x(a),y(b),z(c)
	{}
};
////整形2D点，考虑到整形数的运算速度较快，所以才特别定义的
//class _XVector2I
//{
//public:
//    int x;
//    int y;
//
//public:
//    void set(int a,int b);
//
//    float getLength(const _XVector2I& P0) const;
//    float getAngle(const _XVector2I& P0) const;
//
//	_XVector2I& operator = (const _XVector2 & temp);
//	_XVector2I(const _XVector2 & temp);
//
//    _XVector2I(int a,int b);
//    _XVector2I();
//};
//
////整形矩形
//class _XIRect
//{
//private:
//public:
//    int left;
//    int top;
//    int right;
//    int bottom;
//public:
//    void set(int xl =0,int yt =0,int xr =0,int yb =0);
//
//    _XBool isCrash(_XIRect& R0);
//
//    _XIRect(int xl =0,int yt =0,int xr =0,int yb =0);
//    _XIRect(const _XVector2I& p0,const _XVector2I& p1);
//   //_XIRect()
//   //     :left(0)
//   //     ,top(0)
//   //     ,right(0)
//   //     ,bottom(0)
//   // {
//   // }
//    void set(const _XVector2I& p0,const _XVector2I& p1);
//    void set(const _XVector2& p0,const _XVector2& p1);
//    _XBool isInRect(const _XVector2I& p0) const;
//    int getArea() const;
//    _XVector2 getCenter() const;
//    int getWidth() const;
//    int getHeight() const;
//    void setCenter(int x,int y);
//    void setCenter(const _XVector2I& p);
//};

#include "XOSDefine.h"

#ifdef XEE_OS_WINDOWS
#include "windows.h"
//extern void close(int temp);
extern int initSocket();
#endif

#include "XBasicClass.inl"
#endif