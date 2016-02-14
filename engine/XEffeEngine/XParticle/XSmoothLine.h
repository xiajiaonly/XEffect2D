#ifndef _JIA_XSMOOTHLINE_
#define _JIA_XSMOOTHLINE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.10
//--------------------------------
//这是一个平滑曲线的类，设计方式类似于节点引导曲线与拖尾粒子效果的集合
//#include "XBasicClass.h"
#include "../XMath/XVector2.h"
#include <deque>
namespace XE{
class XSmoothPoint
{
public:
	XVector2 m_position;	//当前点的位置
	float m_length;			//当前节点与上一节点的距离
	float m_percentage;		//当前节点所处在全线中的百分比
};
class XSmoothLine
{
protected:
	bool m_isInited;
	std::deque<XSmoothPoint *> m_points;	//曲线上的所有点
	int m_insertSum;						//差值数量
	float m_insertLength;					//差值距离	//与差值数量存在权衡关系，在满足距离的条件下使用最小差值数量

	virtual void release();	//释放资源
	virtual void pushAPoint(const XVector2 &p);	//这个点是已经经过插值运算了的
	virtual void updateData();	//更新内部数据
public:
	XSmoothLine()
		:m_isInited(false)
		,m_insertSum(1)
		,m_insertLength(1.0f)
	{}
	virtual ~XSmoothLine() {release();}
	virtual bool init(int insertSum,float insertLength);
	virtual void draw();
	virtual void addAPoint(const XVector2 & p);	//插入一个点
	//void update(float stepTime);
	virtual int getPointSum() const {return m_points.size();}
	virtual void clear() 
	{
		for(unsigned int i = 0;i < m_points.size();++ i)
		{
			XMem::XDELETE(m_points[i]);
		}
		m_points.clear();
	}
	virtual bool getIsEnd(){return m_points.size() <= 0;}
};
}
#endif