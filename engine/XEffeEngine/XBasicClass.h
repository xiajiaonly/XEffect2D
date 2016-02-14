#ifndef _JIA_XBASICCLASS_
#define _JIA_XBASICCLASS_
//++++++++++++++++++++++++++++++++
//Author:    贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:        2011.4.9
//--------------------------------
//#include "XBasicFun.h"

//#include "XMath/XVector2.h"
//#include "XMath/XVector3.h" 
//#include "XMath/XVector4.h" 
//#include "XMath/XLine.h"
//#include "XMath/XMatrix2x2.h"
//#include "XMath/XMatrix3x3.h"
//#include "XMath/XMatrix4x4.h"
//#include "XMath/XRect.h"
//#include "XMath/XCColor.h"
//#include "XMath/XFColor.h"
//#include "XMath/XKalman.h"
//#include "XCritical.h" 
//#include "XThread.h" 
namespace XE{
//整形索引
class XVectorIndex3
{
public:
    int x;
    int y;
    int z;

	XVectorIndex3()
		:x(0),y(0),z(0)
	{}
	XVectorIndex3(int a,int b,int c)
		:x(a),y(b),z(c)
	{}
};

class XTexture;
class XTextureInfo;
class XFrame;
class XFrameEx;
class XNodeLine;
class XFontUnicode;
class XSCounter	//智能计数器的类,这个类需要用std::shared_ptr替换
{
private:
	friend XTexture;
	friend XTextureInfo;
	friend XFrame;
	friend XFrameEx;
	friend XNodeLine;
	friend XFontUnicode;

	int m_counter;
	XSCounter()
		:m_counter(1)
	{}
};
////整形2D点，考虑到整形数的运算速度较快，所以才特别定义的
//class XVector2I
//{
//public:
//    int x;
//    int y;
//
//public:
//    void set(int a,int b);
//
//    float getLength(const XVector2I& P0) const;
//    float getAngle(const XVector2I& P0) const;
//
//	XVector2I& operator = (const XVector2 & temp);
//	XVector2I(const XVector2 & temp);
//
//    XVector2I(int a,int b);
//    XVector2I();
//};
//
////整形矩形
//class XIRect
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
//    XBool isCrash(XIRect& R0);
//
//    XIRect(int xl =0,int yt =0,int xr =0,int yb =0);
//    XIRect(const XVector2I& p0,const XVector2I& p1);
//   //XIRect()
//   //     :left(0)
//   //     ,top(0)
//   //     ,right(0)
//   //     ,bottom(0)
//   // {
//   // }
//    void set(const XVector2I& p0,const XVector2I& p1);
//    void set(const XVector2& p0,const XVector2& p1);
//    XBool isInRect(const XVector2I& p0) const;
//    int getArea() const;
//    XVector2 getCenter() const;
//    int getWidth() const;
//    int getHeight() const;
//    void setCenter(int x,int y);
//    void setCenter(const XVector2I& p);
//};
#if WITH_INLINE_FILE
#include "XBasicClass.inl"
#endif
}
#endif