#ifndef _JIA_XSMOOTHCURVE_
#define _JIA_XSMOOTHCURVE_
#include "XMath\XMath.h"
#include "XMath\XVector2.h"
//在两个点之间产生一条光滑的曲线，以便于可以让物体跟随运动
namespace XE{
struct XSmoothCurveInfo
{
	float m_cycleSum;	//半周期的数量
	float m_maxOffset;	//最大的偏移量
	bool m_isAdd;
};
class XSmoothCurve
{
private:
	XVec2 m_start;		//曲线的起点
	XVec2 m_end;			//曲线的终点
	std::vector<XSmoothCurveInfo> m_infos;
	bool m_isInited;
public:
	XSmoothCurve()
		:m_isInited(false)
	{}
	//s：线的起点
	//e：线的终点
	//minS:线的最小阶
	//maxS:线的最大阶段
	//offsetRate:偏移率
	//阶越大，线越曲折
	void initCurve(const XVec2& s,const XVec2& e,int minS = 1,int maxS = 1,float offsetRate = 1.0f);
	//[0.0f - 1.0f]
	XVec2 getPosition(float rate);
	//density曲线的密度，数值越大曲线越平滑
	void draw(int density = 1000);
};
}
#endif