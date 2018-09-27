#ifndef _JIA_XVEGETATIONSYSTEM_
#define _JIA_XVEGETATIONSYSTEM_
#include "XMath\XVector2.h"
#include <vector>
namespace XE{
struct XVegetationCore	//植被核心点
{
	XVec2 pos;	//核心点所在的位置
	float r;		//核心的半径
};
struct XVegetation
{
	XVec2 pos;	//植被的位置
	float size;			//植被的大小
};
class XVegetationSystem
{
private:
	std::vector<XVegetationCore> m_cores;	//核心
	std::vector<XVegetation> m_vegetations;	//植被
	float m_maxVegetationSize;

	XVec2 m_area;
	int m_coreSum;
	float m_coreSize;
	int m_maxVegetationsSum;
public:
	void setMaxVegetationSize(float s){m_maxVegetationSize = s;}
	XVegetationSystem()
		:m_maxVegetationSize(10.0f)
	{}
	void init(const XVec2& area,int coreSum,float coreSize,int maxVegetationsSum);
	void draw();
	void clear()
	{
		m_cores.clear();
		m_vegetations.clear();
	}
	void reset();
	std::vector<XVegetation> getVegetationsInfo()const{return m_vegetations;}
};
}
#endif