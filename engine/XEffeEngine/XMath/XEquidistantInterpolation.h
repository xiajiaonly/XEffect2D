#ifndef _JIA_XEQUIDISTANTITERPOLATION_
#define _JIA_XEQUIDISTANTITERPOLATION_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
//#include "XMath.h"
#include "XVector2.h"
//这是一个2D等距插值的类，用于在节点曲线中通过固定的距离进行插值，插入有效的点(尚未完成)
#include <vector>
namespace XE{
class XEquidistantIterpolation
{
private:
	bool m_isFirstData;		//是否为插入的第一个点
	float m_upLength;		//上一次的长度
	XVec2 m_upPoint;	//上一次的点
	float m_distance;		//插值的距离
public:
	XEquidistantIterpolation()
		:m_isFirstData(true)
		, m_upLength(0.0f)
		, m_distance(10.0f)
	{}
	XVec2 getCurPosition()const{return m_upPoint;}	//获取当前的位置
	void setDistance(float d)
	{
		if (d <= 0) return;	//非法的数据
		m_distance = d; 
	}
	float getDistance() const { return m_distance; }
	void reset()
	{
		m_isFirstData = true;
		m_upLength = 0.0f;
	}
	void reset(const XVec2& p)	//以指定点为起点
	{
		m_upLength = 0.0f;
		m_isFirstData = false;
		m_upPoint = p;
	}
	//根据新插入的点计算中间需要插入的点
	std::vector<XVec2> calPoint(const XVec2& p)
	{
		std::vector<XVec2> ret;
		calPoint(p,ret);
		return ret;
	}
	int calPoint(const XVec2& p,std::vector<XVec2> &data);
};
}
#endif