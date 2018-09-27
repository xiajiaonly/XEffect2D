#ifndef _JIA_X2DGEOMETRY_
#define _JIA_X2DGEOMETRY_
//这里包含松所有2D几何的相关函数接口
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#include <vector>
namespace XE{
class XVec2;
namespace XMath{
//计算若干点的最小外接圆，返回圆心和半径
extern void getMinCircle(const XVec2* points,int sum,XVec2& center,float &r);
//计算N个点的凸包
extern void convexHull(const XVec2*points,int sum,std::vector<XVec2> &convexHull);
//计算N个点的最小包围矩形
struct XRectangle
{
	XVec2 center,axis[2];	//中点以及两个轴的向量
	float extent[2];			//两个半轴长
	float area; //area = 4 * extent[0] * extent[1]	//面积
};
extern void getMinAreaRectangle(std::vector<XVec2> &convexHull,XRectangle &rect);
inline void getMinAreaRectangle(const XVec2*points,int sum,XRectangle &rect)
{
	if(sum <= 2 || points == NULL) return;	//非法的输入参数
	std::vector<XVec2> ch;
	XMath::convexHull(points,sum,ch);
	getMinAreaRectangle(ch,rect);
}
//计算N个点的最小包围椭圆(尚未实现,目前没有找到较好的方法)
}
}
#endif