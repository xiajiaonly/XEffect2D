#ifndef _JIA_XGRAPHELEMENT_
#define _JIA_XGRAPHELEMENT_
#include "XMath\XVector2.h"
#include <vector>
namespace XE{
extern int gXGraphIndex;
//元素信息
class XGraphElement
{
private:
public:
	int m_id;
	XVec2 m_position;	//当前图元所在的位置
	std::vector<XGraphElement *> m_neighbourElement;	//临近的图元
	XGraphElement()
	{
		m_id = gXGraphIndex;
		++ gXGraphIndex;
	}
	XGraphElement(const XVec2& pos)
		:m_position(pos)
	{
		m_id = gXGraphIndex;
		++ gXGraphIndex;
	}
	//XGraphElement(float x,float y)
	//	:m_position(x,y)
	//{
	//	m_id = gXGraphIndex;
	//	++ gXGraphIndex;
	//}
	std::vector<float> m_distance;	//与其他所有元素的距离
};
//边的信息
class XGraphEdge
{
private:
public:
	XGraphElement *m_a;		//边的一个点
	XGraphElement *m_b;		//边的另一个点
	int m_counter;	//计数器
	XGraphEdge()
		:m_a(NULL)
		, m_b(NULL)
		, m_counter(0)
	{}
	XGraphEdge(XGraphElement *A,XGraphElement *B)
		:m_a(A)
		,m_b(B)
		,m_counter(0)
	{}
	bool isTheSame(const XGraphElement *A,const XGraphElement *B)const
	{
		return (m_a == A && m_b == B) || (m_b == A && m_a == B);
	}
};
}
#endif