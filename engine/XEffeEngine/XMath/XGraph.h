#ifndef _JIA_XGRAPH_
#define _JIA_XGRAPH_
#include "XGraphElement.h"
#include <deque>
//注意:
//1、可以优化全覆盖算法来提升性能
//2、可以优化为遍历边的方式拆分路以达到较大的性能提升
//3、道路连接时以角度小的优先，可以使得结果更好
namespace XE{
class XGraph
{
private:
	std::vector<XGraphElement *> m_graphElements;	//图元的集合
	std::vector<XGraphEdge> m_edges;	//所有的边
	void createEdge(XGraphElement *A, XGraphElement * B)	//给两个元素建立边
	{
		A->m_neighbourElement.push_back(B);
		B->m_neighbourElement.push_back(A);
	}
	//寻找岛A与B中最近的两个点，并返回距离
	bool getIslandDis(const std::vector<int> &A, const std::vector<int> &B, 
		int &indexA, int &indexB, float &dis);
	std::vector<std::deque<XGraphElement *>> m_tmpPlanRoads;	//临时的路，储存中间结果
public:
	int getElementsSum()const { return m_graphElements.size(); }
	const XGraphElement *getElement(int index)const
	{
		if (index >= 0 && index < m_graphElements.size()) return m_graphElements[index];
		return NULL;
	}
	XVec2 getElementsPos(int index)const
	{
		if (index >= 0 && index < m_graphElements.size()) return m_graphElements[index]->m_position;
		return XVec2::zero;
	}
	bool getIsRoadEndpoint(int index)const
	{//判断节点是否为路面的端点
		if (index < 0 || index >= m_graphElements.size()) return false;
		for (auto it = m_roads.begin(); it != m_roads.end(); ++it)
		{
			if (*(it->begin()) == m_graphElements[index] ||
				*(it->end() - 1) == m_graphElements[index]) return true;
		}
		return false;
	}
public:
	const std::vector<XGraphEdge> &getEdges()const { return m_edges; }
	bool getIsExist(const XGraphElement *A) const
	{//首先判断两个点是否存在
		for (int i = 0; i < m_graphElements.size(); ++i)
		{
			if (m_graphElements[i] == A) return true;
		}
		return false;
	}
	static bool getIsEdge(const XGraphElement *A, const XGraphElement *B)	//返回是否存在A到B的边
	{
		//其次判断边是否存在
		if (A->m_neighbourElement.size() <= B->m_neighbourElement.size())
		{
			for (std::vector<XGraphElement *>::const_iterator it = A->m_neighbourElement.begin(); it != A->m_neighbourElement.end(); ++it)
			{
				if (*it == B) return true;
			}
		}
		else
		{
			for (std::vector<XGraphElement *>::const_iterator it = B->m_neighbourElement.begin(); it != B->m_neighbourElement.end(); ++it)
			{
				if (*it == A) return true;
			}
		}
		return false;
	}
	void addAElement(const XVec2& pos)		//向图中增加一元素，只是增加一个元素并不生成相应的边
	{
		XGraphElement *tmp = XMem::createMem<XGraphElement>();
		if (tmp == NULL) return;
		tmp->m_position = pos;
		m_graphElements.push_back(tmp);
	}
	void changeElement(int index, const XVec2& pos)	//改变某一个元素所在的位置(尚未完成)
	{
		if (index < 0 || index >= m_graphElements.size()) return;
		m_graphElements[index]->m_position = pos;
	}
	void addAElementAndConnect(const XVec2& pos);	//向图中增加一元素并于原来的元素进行最近连接
	void draw();

	void clearAllEdge();	//清除所有的边
	void clearAllElement();	//清除所有的元
	void clearOneElement(int index);	//清除指定的元
	int getCloseElement(const XVec2& pos);	//获取离指定位置最近的元的索引

	void calculateEdge();	//根据点计算边，使得所有点连在一起并路径和最短，还可优化性能
	//原则：1、边全覆盖
	//2、无重复的边
	//3、路的数量尽量少
	//4、路的拐角小于指定值，并在此前提下尽量小
	//enableCross 是否允许交叉
	//withEx，是否使用优化的算法，优化的算法在性能上有绝对优势
	void roadPlanning(bool enableCross, bool withEx);	//将图拆分为路的算法
	std::vector<std::deque<XGraphElement *>> getRoads()const { return m_roads; }
	const std::vector<std::deque<XGraphElement *>> &getRoads() { return m_roads; }
private:
	int m_roadsMaxLen;	//路中最长的路的长度
	std::vector<std::deque<XGraphElement *>> m_roads;	//拆分路之后最终的结果
	std::vector<std::deque<XGraphElement *>> m_tmpRoads;	//拆分路之后最终的结果
	//根据第order中可能将节点index加入到roads的队列中，返回是否成功
	void addElementToRoads(int index, std::vector<std::deque<XGraphElement *>> &roads, bool enableCross);
	//返回路是否已经全覆盖
	bool isAllCover(const std::vector<std::deque<XGraphElement *>> &roads);
	//判断节点index是否已经在路中
	bool isInRoads(int index, const std::vector<std::deque<XGraphElement *>> &roads)
	{
		return isInMiddle(m_graphElements[index], roads);
	}
	//判断边A-B是否在路中存在
	bool isEdgeInRoads(const XGraphElement *A, const XGraphElement *B, const std::vector<std::deque<XGraphElement *>> &roads);
	void optimizationRoad(std::vector<std::deque<XGraphElement *>> &roads, bool enableCross);	//优化路径
	bool isAngleOK(const XGraphElement *A,
		const XGraphElement *B, const XGraphElement *C)	//连线的角度是否符合要求
	{
		float angle = XMath::angleAbbreviation(A->m_position.getAngle(B->m_position) -
			B->m_position.getAngle(C->m_position));
		if (angle > PI) angle = PI2 - angle;
		return (angle < PI * 0.3333f);	//60度
	}
	bool isInMiddle(const XGraphElement *A, const std::vector<std::deque<XGraphElement *>> &roads);	//判断点A是否出现在其他路的中间，用于判断是否两条路产生相交
	int getMaxRoadLen(const std::vector<std::deque<XGraphElement *>> &roads);
	void updateRoadPlanning(std::vector<std::deque<XGraphElement *>> &roads, bool enableCross);
	//下面是强化的实现
	void addEdgeToRoads(int index, std::vector<std::deque<XGraphElement *>> &roads, bool enableCross);
	//下面的算法是对上面算法的优化,新的算法完全优于旧的算法
	void addEdgeToRoadsEx(int index, bool enableCross);
	void tryMergeRoad(int index, std::vector<std::deque<XGraphElement *>> &roads, bool enableCross);
};
}
#endif