#include "XStdHead.h"
#include "X2DGeometry.h"
#include "XVector2.h"
#include "Miniball.hpp"
namespace XE{
namespace XMath{
void getMinCircle(const XVec2*points,int sum,XVec2& center,float &r)
{
	//方案1：结果不理想
	//if(sum <= 0 || points == NULL) return;	//非法的输入参数
 //   float r2;                    // radius and radius squared
 //   float xmin, xmax, ymin, ymax;       // bounding box extremes
 //   int   Pxmin, Pxmax, Pymin, Pymax;   // index of  P[] at box extreme

 //   // find a large diameter to start with
 //   // first get the bounding box and P[] extreme points for it
 //   xmin = xmax = points[0].x;
 //   ymin = ymax = points[0].y;
 //   Pxmin = Pxmax = Pymin = Pymax = 0;
 //   for (int i = 1;i < sum;++ i) 
	//{
 //       if (points[i].x < xmin) 
	//	{
 //           xmin = points[i].x;
 //           Pxmin = i;
 //       }else 
	//	if (points[i].x > xmax) 
	//	{
 //           xmax = points[i].x;
 //           Pxmax = i;
 //       }
 //       if (points[i].y < ymin) 
	//	{
 //           ymin = points[i].y;
 //           Pymin = i;
 //       }else 
	//	if (points[i].y > ymax) 
	//	{
 //           ymax = points[i].y;
 //           Pymax = i;
 //       }
 //   }
 //   // select the largest extent as an initial diameter for the  ball
	//float dx2 = (points[Pxmax] - points[Pxmin]).getLengthSqure(); // Px diff squared
	//float dy2 = (points[Pymax] - points[Pymin]).getLengthSqure(); // Py diff squared
 //   if (dx2 >= dy2) 
	//{                      // x direction is largest extent
 //       center = (points[Pxmax] + points[Pxmin]) * 0.5f;          // Center = midpoint of extremes
	//	r2 = (points[Pxmax] - center).getLengthSqure();          // radius squared
 //   }else 
	//{                                 // y direction is largest extent
 //       center = (points[Pymax] + points[Pymin]) * 0.5f;          // Center = midpoint of extremes
	//	r2 = (points[Pymax] - center).getLengthSqure();          // radius squared
 //   }
 //   r = sqrt(r2);

 //   // now check that all points P[i] are in the ball
 //   // and if not, expand the ball just enough to include them
 //   XVec2 dP;
 //   float dist, dist2;
 //   for (int i = 0;i < sum;++ i) 
	//{
 //       dP = points[i] - center;
	//	dist2 = dP.getLengthSqure();
 //       if (dist2 <= r2)     // P[i] is inside the ball already
 //           continue;
 //       // P[i] not in ball, so expand ball  to include it
 //       dist = sqrt(dist2);
 //       r = (r + dist) * 0.5;          // enlarge radius just enough
 //       r2 = r * r;
 //       center += ((dist - r)/dist) * dP;    // shift Center toward P[i]
 //   }
    //方案2：使用miniball,结果比方案1更理想
	typedef float* const* PointIterator; 
	typedef const float* CoordIterator;
	typedef Miniball::Miniball <Miniball::CoordAccessor<PointIterator,CoordIterator>> MB;
	//下面开始准备数据
	float **ap = XMem::createArrayMem<float*>(sum);
	for(int i = 0;i < sum;++ i)
	{
		ap[i] = points[i];
	}
	MB mb(2,ap,ap + sum);
	const float* centerX = mb.center(); 
	center.x = centerX[0];
	center.y = centerX[1];
	r = sqrt(mb.squared_radius());
	XMem::XDELETE_ARRAY(ap);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(XVec2 p, XVec2 q, XVec2 r)
{
    float val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);
 
    if (val == 0.0f) return 0;  // colinear
    return (val > 0.0f)? 1: 2; // clock or counterclock wise
}
//方案1：
//// A function used by library function qsort() to sort an array of
//// points with respect to the first point
//XVec2 p0;
//int compare(const void *vp1, const void *vp2)
//{
//   XVec2 *p1 = (XVec2 *)vp1;
//   XVec2 *p2 = (XVec2 *)vp2;
// 
//   // Find orientation
//   int o = orientation(p0, *p1, *p2);
//   if (o == 0)
//	   return ((p0 - *p2).getLengthSqure() >= (p0 - *p1).getLengthSqure())? -1 : 1;
// 
//   return (o == 2)? -1: 1;
//}
void convexHull(const XVec2*points,int sum,std::vector<XVec2> &convexHull)
{
	if(sum <= 2 || points == NULL) return;	//非法的输入参数
	//方案1：
	//std::vector<XVec2> tmp;
	//tmp.resize(sum);
	//for(int i = 0;i < sum;++ i)
	//	tmp[i] = points[i];
	//// Find the bottommost point
	//float ymin = tmp[0].y;
	//int minIndex = 0;
	//for (int i = 1; i < tmp.size(); i++)
	//{ 
	//	// Pick the bottom-most or chose the left
	//	// most point in case of tie
	//	if ((tmp[i].y < ymin) || 
	//		(ymin == tmp[i].y && tmp[i].x < tmp[minIndex].x))
	//	{
	//		ymin = tmp[i].y;
	//		minIndex = i;
	//	}
	//}
 //
	//// Place the bottom-most point at first position
	//std::swap(tmp[0], tmp[minIndex]);
 //
	//// Sort n-1 points with respect to the first point.
	//// A point p1 comes before p2 in sorted ouput if p2
	//// has larger polar angle (in counterclockwise
	//// direction) than p1
	//p0 = tmp[0];
	//std::qsort(&tmp[1], tmp.size() - 1, sizeof(XVec2), compare);
 //
	//// If two or more points make same angle with p0,
	//// Remove all but the one that is farthest from p0
	//// Remember that, in above sorting, our criteria was
	//// to keep the farthest point at the end when more than
	//// one points have same angle.
	//int m = 1; // Initialize size of modified array
	//for (int i = 1; i < tmp.size();++ i)
	//{
	//	// Keep removing i while angle of i and i+1 is same
	//	// with respect to p0
	//	while (i < tmp.size() - 1 && 
	//		orientation(p0, tmp[i],tmp[i+1]) == 0)
	//		++i; 
 //
	//	tmp[m] = tmp[i];
	//	m++;  // Update size of modified array
	//}
 //
	//// If modified array of points has less than 3 points,
	//// convex hull is not possible
	//if (m < 3) return;
 //
	//// Create an empty stack and push first three points
	//// to it.
	//convexHull.push_back(tmp[0]);
	//convexHull.push_back(tmp[1]);
	//convexHull.push_back(tmp[2]);
 //
	//// Process remaining n-3 points
	//for (int i = 3; i < m; i++)
	//{
	//	// Keep removing top while the angle formed by
	//	// points next-to-top, top, and points[i] makes
	//	// a non-left turn
	//	while (orientation(convexHull[convexHull.size() - 2], convexHull[convexHull.size() - 1], tmp[i]) != 2)
	//		convexHull.pop_back();
	//	convexHull.push_back(tmp[i]);
	//}
	//方案2
	// Find the leftmost point
    int l = 0;
    for (int i = 1; i < sum; i++)
        if (points[i].x < points[l].x)
            l = i;
 
    // Start from leftmost point, keep moving counterclockwise
    // until reach the start point again.  This loop runs O(h)
    // times where h is number of points in result or output.
    int p = l, q;
    do
    {
        // Add current point to result
        convexHull.push_back(points[p]);
 
        // Search for a point 'q' such that orientation(p, x,
        // q) is counterclockwise for all points 'x'. The idea
        // is to keep track of last visited most counterclock-
        // wise point in q. If any point 'i' is more counterclock-
        // wise than q, then update q.
        q = (p+1) % sum;
        for (int i = 0; i < sum; i++)
        {
           // If i is more counterclockwise than current q, then
           // update q
           if (orientation(points[p], points[i], points[q]) == 2)
               q = i;
        }
 
        // Now q is the most counterclockwise with respect to p
        // Set p as q for next iteration, so that q is added to
        // result 'hull'
        p = q;
 
    } while (p != l);  // While we don't come to first point
}
void getMinAreaRectangle(std::vector<XVec2> &ch,XRectangle &rect)
{
	rect.area = FLT_MAX;	//这里使用浮点数的最大值
	for(int i0 = ch.size() - 1, i1 = 0;i1 < ch.size() ;i0 = i1++)
	{
		XVec2 origin = ch[i0];
		XVec2 U0 = ch[i1] - origin;
		U0.normalize() ; //length of U0 is 1
		//XVec2 U1 = −Perp(U0) ;
		XVec2 U1(-U0.y,U0.x);
		float min0 = 0.0f,max0 = 0.0f ; //projectiononto U0 − axisis[min0 , max0]
		float max1 = 0.0f; //projection onto U1 − axisis[0 , max1], min1 = 0 is guaranteed
		for(int j = 0 ; j < ch.size(); ++j)
		{
			XVec2 D = ch[j] - origin;
			float dot = U0.dot(D);
			if(dot < min0) min0 = dot; 
			else
				if(dot > max0) max0 = dot;

			dot = U1.dot(D);
			if(dot > max1) max1 = dot;
		}
		float area = (max0 - min0) * max1;
		if(area < rect.area)
		{
			rect.center = origin + ((min0 + max0) * 0.5f) * U0 + (max1 * 0.5f) * U1;
			rect.axis[0] = U0;
			rect.axis[1] = U1;
			rect.extent[0] = (max0 - min0) * 0.5f;
			rect.extent[1] = max1 * 0.5f;
			rect.area = area;
		}
	}
}
}
}