#include "XStdHead.h"
#include "XGraph.h"
namespace XE {
void XGraph::addAElementAndConnect(const XVec2& pos)
{
	XGraphElement *tmp = XMem::createMem<XGraphElement>();
	if (tmp == NULL) return;
	tmp->m_position = pos;
	int index = getCloseElement(pos);
	m_graphElements.push_back(tmp);
	if (index >= 0)
		createEdge(tmp, m_graphElements[index]);
}
void XGraph::draw()
{
	//描绘图元中的点
	for (std::vector<XGraphElement *>::iterator it = m_graphElements.begin(); it != m_graphElements.end(); ++it)
	{
		XRender::drawPoint((*it)->m_position, 3, XFColor::red);
	}
	//描绘图元中的边
	for (std::vector<XGraphElement *>::iterator it = m_graphElements.begin(); it != m_graphElements.end(); ++it)
	{
		for (std::vector<XGraphElement *>::iterator jt = (*it)->m_neighbourElement.begin(); jt != (*it)->m_neighbourElement.end(); ++jt)
		{
			if ((*it)->m_id >= (*jt)->m_id)
				continue;	//放置重复描绘，否则会造成一条路径描绘两次
			XRender::drawLine((*it)->m_position, (*jt)->m_position);
		}
	}
}
void XGraph::clearAllEdge()	//清除所有的边
{
	for (std::vector<XGraphElement *>::iterator it = m_graphElements.begin(); it != m_graphElements.end(); ++it)
	{
		(*it)->m_neighbourElement.clear();
	}
	m_edges.clear();
}
void XGraph::clearAllElement()	//清除所有的元
{
	for (int i = 0; i < m_graphElements.size(); ++i)
	{
		XMem::XDELETE(m_graphElements[i]);
	}
	m_graphElements.clear();
	m_edges.clear();
	m_roads.clear();
}
void XGraph::clearOneElement(int index)	//清除指定的元
{
	if (index < 0 || index >= m_graphElements.size()) return;
	//清除所有边种与该点构成的边
	for (int i = 0; i < m_graphElements.size(); ++i)
	{
		if (i == index) continue;
		for (int j = 0; j < m_graphElements[i]->m_neighbourElement.size(); ++j)
		{
			//方案1：遍历所有的边，如果相同则清除掉
			//if(m_graphElements[i]->m_neighbourElement[j] == m_graphElements[index])
			//{
			//	m_graphElements[i]->m_neighbourElement.erase(m_graphElements[i]->m_neighbourElement.begin() + j);
			//	-- j;
			//}
			//方案2：边是不会重复的所以只需要清除一次
			if (m_graphElements[i]->m_neighbourElement[j] != m_graphElements[index]) continue;
			m_graphElements[i]->m_neighbourElement.erase(m_graphElements[i]->m_neighbourElement.begin() + j);
			break;
		}
	}
	XMem::XDELETE(m_graphElements[index]);
	m_graphElements.erase(m_graphElements.begin() + index);
}
int XGraph::getCloseElement(const XVec2& pos)	//获取离指定位置最近的元的索引
{
	if (m_graphElements.size() == 0) return -1;
	int index = 0;
	float dis = m_graphElements[0]->m_position.getLengthSqure(pos);
	float tmpDis;
	for (int i = 1; i < m_graphElements.size(); ++i)
	{
		tmpDis = m_graphElements[i]->m_position.getLengthSqure(pos);
		if (tmpDis < dis)
		{
			dis = tmpDis;
			index = i;
		}
	}
	return index;
}
//下面这个算法可以优化，可以进行排序，然后利用排序结果进行优化
bool XGraph::getIslandDis(const std::vector<int> &A,
	const std::vector<int> &B,
	int &indexA, int &indexB, float &dis)
{
	if (A.size() <= 0 || B.size() <= 0) return false;
	float tmpDis;
	//	bool isFirst = true;
	indexA = A[0];
	indexB = B[0];
	dis = m_graphElements[indexA]->m_distance[indexB];

	for (std::vector<int>::const_iterator it = A.begin(); it != A.end(); ++it)
	{
		for (std::vector<int>::const_iterator jt = B.begin(); jt != B.end(); ++jt)
		{
			tmpDis = m_graphElements[*it]->m_distance[*jt];
			if (tmpDis < dis)
			{
				indexA = *it;
				indexB = *jt;
				dis = tmpDis;
			}
		}
	}
	return true;
}
void XGraph::calculateEdge()
{//从算法上实现，不考虑性能，方案2：每个点都选用最短路径相连，直到所有的点都被连接为止（尚未实现，需要保证最终形成一个图）
	clearAllEdge();
	if (m_graphElements.size() <= 0) return;	//数据不够，直接返回
	std::vector<std::vector<int>> islands;
	islands.resize(m_graphElements.size());
	for (int i = 0; i < m_graphElements.size(); ++i)
	{
		islands[i].push_back(i);
	}
	//使用空间换时间的方式对算法进行优化，先准备数据
	for (int i = 0; i < m_graphElements.size(); ++i)
	{
		m_graphElements[i]->m_distance.clear();
		for (int j = 0; j < m_graphElements.size(); ++j)
		{
			if (j == i)
			{
				m_graphElements[i]->m_distance.push_back(0.0f);
			}
			else
				if (j < i)
				{
					m_graphElements[i]->m_distance.push_back(m_graphElements[j]->m_distance[i]);
				}
				else
				{
					m_graphElements[i]->m_distance.push_back(m_graphElements[i]->m_position.getLengthSqure(m_graphElements[j]->m_position));
				}
		}
	}

	int indexA, indexB;
	float dis;
	int minIndexA = 0, minIndexB = 0;
	float minDis = 0.0f;
	int minIslandIndexA = 0, minIslandIndexB = 0;
	while (islands.size() > 1)
	{
		//寻找到最近的两个岛
		for (int i = 0; i < int(islands.size()) - 1; ++i)
		{
			for (int j = i + 1; j < islands.size(); ++j)
			{
				if (i == 0 && j == 1)
				{
					getIslandDis(islands[i], islands[j], minIndexA, minIndexB, minDis);
					minIslandIndexA = i;
					minIslandIndexB = j;
				}
				else
				{
					getIslandDis(islands[i], islands[j], indexA, indexB, dis);
					if (dis < minDis)
					{
						minDis = dis;
						minIndexA = indexA;
						minIndexB = indexB;
						minIslandIndexA = i;
						minIslandIndexB = j;
					}
				}
			}
		}
		//建立边,并推入队列中
		createEdge(m_graphElements[minIndexA], m_graphElements[minIndexB]);
		if (m_graphElements[minIndexA]->m_id >= m_graphElements[minIndexB]->m_id)
			m_edges.push_back(XGraphEdge(m_graphElements[minIndexB], m_graphElements[minIndexA]));
		else
			m_edges.push_back(XGraphEdge(m_graphElements[minIndexA], m_graphElements[minIndexB]));
		//合并两个岛
		islands[minIslandIndexA].insert(islands[minIslandIndexA].end(),
			islands[minIslandIndexB].begin(), islands[minIslandIndexB].end());
		islands.erase(islands.begin() + minIslandIndexB);
	}
}
void XGraph::roadPlanning(bool enableCross, bool withEx)
{
	m_roads.clear();
	m_roadsMaxLen = 0;
	if (m_graphElements.size() <= 1)//没有足够的数据直接返回
		return;
	m_tmpPlanRoads.clear();
	if (withEx) addEdgeToRoadsEx(0, enableCross);	//按边的方式加入
	else addElementToRoads(0, m_tmpPlanRoads, enableCross);	//由于没有考虑线合并的算法会造成结果不最优，改进方法为使用遍历边的方式（尚未实现）
}
void XGraph::addElementToRoads(int index, std::vector<std::deque<XGraphElement *>> &roads, bool enableCross)
{//下面尝试插入点
	for (int i = 0; i < m_graphElements[index]->m_neighbourElement.size(); ++i)
	{
		if (isEdgeInRoads(m_graphElements[index], m_graphElements[index]->m_neighbourElement[i], roads))
		{
			//	if(index < int(m_graphElements.size()) - 1)
			//		addElementToRoads(index + 1,roads,enableCross);
			continue;
		}
		//尝试新建立一条路径
		std::deque<XGraphElement *> tmp;
		tmp.push_back(m_graphElements[index]);
		tmp.push_back(m_graphElements[index]->m_neighbourElement[i]);
		roads.push_back(tmp);
		if (isAllCover(roads))
		{//如果所有边已经全覆盖，则开始计算是否为最优的路
			updateRoadPlanning(roads, enableCross);
		}
		else
		{//如果数量已经超过当前最优解，就不用继续了
			//if((roads.size() < m_roads.size() || m_roads.size() == 0) && index < int(m_graphElements.size()) - 1)
			if (index < int(m_graphElements.size()) - 1)
				addElementToRoads(index + 1, roads, enableCross);
		}
		roads.pop_back();	//还原数据
		//尝试将该点插入到现有数据中
		for (int j = 0; j < roads.size(); ++j)
		{
			if (roads[j][0] == m_graphElements[index]->m_neighbourElement[i])
			{//加入到头以前	
				if (isAngleOK(roads[j][1], roads[j][0], m_graphElements[index]) &&
					(enableCross || isInMiddle(m_graphElements[index]->m_neighbourElement[i], roads)))
				{//检查角度是否合适
					roads[j].push_front(m_graphElements[index]);
					if (isAllCover(roads))
					{//如果所有边已经全覆盖，则开始计算是否为最优的路
						updateRoadPlanning(roads, enableCross);
					}
					else
					{//如果数量已经超过当前最优解，就不用继续了
						//if((roads.size() < m_roads.size() || m_roads.size() == 0) && index < int(m_graphElements.size()) - 1)
						if (index < int(m_graphElements.size()) - 1)
							addElementToRoads(index + 1, roads, enableCross);
					}
					roads[j].pop_front();	//还原数据
				}
			}
			if (*(roads[j].end() - 1) == m_graphElements[index]->m_neighbourElement[i])
			{//加入到尾以后
				if (isAngleOK(*(roads[j].end() - 2), *(roads[j].end() - 1), m_graphElements[index]) &&
					(enableCross || isInMiddle(m_graphElements[index]->m_neighbourElement[i], roads)))
				{//检查角度是否合适
					roads[j].push_back(m_graphElements[index]);
					if (isAllCover(roads))
					{//如果所有边已经全覆盖，则开始计算是否为最优的路
						updateRoadPlanning(roads, enableCross);
					}
					else
					{//如果数量已经超过当前最优解，就不用继续了
						//if((roads.size() < m_roads.size() || m_roads.size() == 0) && index < int(m_graphElements.size()) - 1)
						if (index < int(m_graphElements.size()) - 1)
							addElementToRoads(index + 1, roads, enableCross);
					}
					roads[j].pop_back();	//还原数据
				}
			}
			if (roads[j][0] == m_graphElements[index])
			{//加入到头以前	
				if (isAngleOK(roads[j][1], roads[j][0], m_graphElements[index]->m_neighbourElement[i]) &&
					(enableCross || isInMiddle(m_graphElements[index], roads)))	//60度
				{//检查角度是否合适
					roads[j].push_front(m_graphElements[index]->m_neighbourElement[i]);
					if (isAllCover(roads))
					{//如果所有边已经全覆盖，则开始计算是否为最优的路
						updateRoadPlanning(roads, enableCross);
					}
					else
					{//如果数量已经超过当前最优解，就不用继续了
						//if((roads.size() < m_roads.size() || m_roads.size() == 0) && index < int(m_graphElements.size()) - 1)
						if (index < int(m_graphElements.size()) - 1)
							addElementToRoads(index + 1, roads, enableCross);
					}
					roads[j].pop_front();	//还原数据						
				}
			}
			if (*(roads[j].end() - 1) == m_graphElements[index])
			{//加入到尾以后
				if (isAngleOK(*(roads[j].end() - 2), *(roads[j].end() - 1), 
					m_graphElements[index]->m_neighbourElement[i]) &&
					(enableCross || isInMiddle(m_graphElements[index], roads)))
				{//检查角度是否合适
					roads[j].push_back(m_graphElements[index]->m_neighbourElement[i]);
					if (isAllCover(roads))
					{//如果所有边已经全覆盖，则开始计算是否为最优的路
						updateRoadPlanning(roads, enableCross);
					}
					else
					{//如果数量已经超过当前最优解，就不用继续了
						//if((roads.size() < m_roads.size() || m_roads.size() == 0) && index < int(m_graphElements.size()) - 1)
						if (index < int(m_graphElements.size()) - 1)
							addElementToRoads(index + 1, roads, enableCross);
					}
					roads[j].pop_back();	//还原数据
				}
			}
		}
	}
}
bool XGraph::isAllCover(const std::vector<std::deque<XGraphElement *>> &roads)
{//所有边都已经在路中
	for (auto it = m_graphElements.begin(); it != m_graphElements.end(); ++it)
	{
		for (auto jt = (*it)->m_neighbourElement.begin(); jt != (*it)->m_neighbourElement.end(); ++jt)
		{
			if ((*it)->m_id >= (*jt)->m_id)
				continue;	//放置重复描绘，否则会造成一条路径描绘两次
			if (!isEdgeInRoads((*it), (*jt), roads)) return false;
		}
	}
	return true;
}
bool XGraph::isEdgeInRoads(const XGraphElement *A, const XGraphElement *B,
	const std::vector<std::deque<XGraphElement *>> &roads)
{
	for (auto it = roads.begin(); it != roads.end(); ++it)
	{
		if (it->size() == 0) continue;
		for (auto jt = it->begin(); jt != it->end() - 1; ++jt)
		{
			if ((*jt == A && *(jt + 1) == B) ||
				(*jt == B && *(jt + 1) == A))
				return true;
		}
	}
	return false;
}
bool XGraph::isInMiddle(const XGraphElement *A, const std::vector<std::deque<XGraphElement *>> &roads)
{
	for (auto it = roads.begin(); it != roads.end(); ++it)
	{
		if (it->size() < 2) continue;
		for (auto jt = it->begin() + 1; jt != it->end() - 1; ++jt)
		{
			if (*jt == A) return true;
		}
	}
	return false;
}
void XGraph::optimizationRoad(std::vector<std::deque<XGraphElement *>> &roads, bool enableCross)	//优化路径
{//将能链接的路连接起来
	for (int i = 0; i < int(roads.size()) - 1; ++i)
	{
		for (int j = i + 1; j < roads.size(); ++j)
		{
			if (roads[i][0] == roads[j][0] && isAngleOK(roads[i][1], roads[i][0], roads[j][1]) &&
				(enableCross || !isInMiddle(roads[i][0], roads)))
			{//头头
			//	for(int k = 1;k < roads[j].size();++ k)
			//	{
			//		roads[i].push_front(roads[j][k]);
			//	}
				if (roads[j].size() > 0)
				{
					for (auto it = roads[j].begin() + 1; it != roads[j].end(); ++it)
					{
						m_tmpPlanRoads[i].push_front(*it);
					}
				}
				roads.erase(roads.begin() + j);
				i = -1;
				break;
			}
			else
				if (*(roads[i].end() - 1) == roads[j][0] &&
					isAngleOK(roads[j][1], roads[j][0], *(roads[i].end() - 2)) &&
					(enableCross || !isInMiddle(roads[j][0], roads)))
				{//尾头
					roads[i].insert(roads[i].end(), roads[j].begin() + 1, roads[j].end());
					roads.erase(roads.begin() + j);
					i = -1;
					break;
				}
				else
					if (roads[i][0] == *(roads[j].end() - 1) &&
						isAngleOK(roads[i][1], roads[i][0], *(roads[j].end() - 2)) &&
						(enableCross || !isInMiddle(roads[i][0], roads)))
					{//头尾
						roads[i].insert(roads[i].begin(), roads[j].begin(), roads[j].end() - 1);
						roads.erase(roads.begin() + j);
						i = -1;
						break;
					}
					else
						if (*(roads[i].end() - 1) == *(roads[j].end() - 1) &&
							isAngleOK(*(roads[i].end() - 2),
								*(roads[i].end() - 1), *(roads[j].end() - 2)) &&
								(enableCross || !isInMiddle(*(roads[i].end() - 1), roads)))
						{//尾尾
						//	for(int k = int(roads[j].size()) - 2;k >= 0;-- k)
						//	{
						//		roads[i].push_back(roads[j][k]);
						//	}
							if (roads[j].size() >= 2)
							{
								for (auto it = roads[j].end() - 2;; --it)
								{
									roads[i].push_back(*it);
									if (it == roads[j].begin()) break;
								}
							}
							roads.erase(roads.begin() + j);
							i = -1;
							break;
						}
		}
	}
}
int XGraph::getMaxRoadLen(const std::vector<std::deque<XGraphElement *>> &roads)
{
	int ret = roads[0].size();
	for (int i = 1; i < roads.size(); ++i)
	{
		if (roads[i].size() > ret) ret = roads[i].size();
	}
	return ret;
}
void XGraph::updateRoadPlanning(std::vector<std::deque<XGraphElement *>> &roads, bool enableCross)
{
	m_tmpRoads = roads;
	optimizationRoad(m_tmpRoads, enableCross);
	if (m_tmpRoads.size() < m_roads.size() || m_roads.size() == 0)
	{//长度优先
		m_roadsMaxLen = getMaxRoadLen(m_tmpRoads);
		m_roads = m_tmpRoads;	//记录最优的结果
	}
	else
		if (m_tmpRoads.size() == m_roads.size())
		{//等长时以最大长度为标准
			int maxLen = getMaxRoadLen(m_tmpRoads);
			if (maxLen > m_roadsMaxLen)
			{
				m_roadsMaxLen = getMaxRoadLen(m_tmpRoads);
				m_roads = m_tmpRoads;	//记录最优的结果
			}
		}
}
void XGraph::addEdgeToRoads(int index, std::vector<std::deque<XGraphElement *>> &roads, bool enableCross)
{//递归遍历所有可能的情况，找到符合条件的最优解，可以优化
	//尝试与现有的线连接
	for (int j = 0; j < roads.size(); ++j)
	{
		if (roads[j][0] == m_edges[index].m_a)
		{//加入到头以前	
			if (isAngleOK(roads[j][1], roads[j][0], m_edges[index].m_b) &&
				(enableCross || isInMiddle(m_edges[index].m_a, roads)))
			{//检查角度是否合适
				roads[j].push_front(m_edges[index].m_b);
				if (index >= int(m_edges.size()) - 1)//插入完成
					updateRoadPlanning(roads, enableCross);
				else
					addEdgeToRoads(index + 1, roads, enableCross);
				roads[j].pop_front();	//还原数据
			}
		}
		if (*(roads[j].end() - 1) == m_edges[index].m_a)
		{//加入到尾以后
			if (isAngleOK(*(roads[j].end() - 2),
				*(roads[j].end() - 1), m_edges[index].m_b) &&
				(enableCross || isInMiddle(m_edges[index].m_a, roads)))
			{//检查角度是否合适
				roads[j].push_back(m_edges[index].m_b);
				if (index >= int(m_edges.size()) - 1)//插入完成
					updateRoadPlanning(roads, enableCross);
				else
					addEdgeToRoads(index + 1, roads, enableCross);
				roads[j].pop_back();	//还原数据
			}
		}
		if (roads[j][0] == m_edges[index].m_b)
		{//加入到头以前	
			if (isAngleOK(roads[j][1], roads[j][0], m_edges[index].m_a) &&
				(enableCross || isInMiddle(m_edges[index].m_b, roads)))	//60度
			{//检查角度是否合适
				roads[j].push_front(m_edges[index].m_a);
				if (index >= int(m_edges.size()) - 1)//插入完成
					updateRoadPlanning(roads, enableCross);
				else
					addEdgeToRoads(index + 1, roads, enableCross);
				roads[j].pop_front();	//还原数据						
			}
		}
		if (*(roads[j].end() - 1) == m_edges[index].m_b)
		{//加入到尾以后
			if (isAngleOK(*(roads[j].end() - 2),
				*(roads[j].end() - 1), m_edges[index].m_a) &&
				(enableCross || isInMiddle(m_edges[index].m_b, roads)))
			{//检查角度是否合适
				roads[j].push_back(m_edges[index].m_a);
				if (index >= int(m_edges.size()) - 1)//插入完成
					updateRoadPlanning(roads, enableCross);
				else
					addEdgeToRoads(index + 1, roads, enableCross);
				roads[j].pop_back();	//还原数据
			}
		}
	}
	//建立一条线	
	std::deque<XGraphElement *> tmp;
	tmp.push_back(m_edges[index].m_a);
	tmp.push_back(m_edges[index].m_b);
	roads.push_back(tmp);
	if (index >= int(m_edges.size()) - 1)//插入完成
		updateRoadPlanning(roads, enableCross);
	else
		addEdgeToRoads(index + 1, roads, enableCross);
	roads.pop_back();
}
void XGraph::addEdgeToRoadsEx(int index, bool enableCross)
{//使用岛链合并的方式
	m_tmpPlanRoads.resize(m_edges.size());
	for (int i = 0; i < m_tmpPlanRoads.size(); ++i)
	{
		m_tmpPlanRoads[i].push_back(m_edges[i].m_a);
		m_tmpPlanRoads[i].push_back(m_edges[i].m_b);
	}
	//将必然合并的路线合并：1、只有一条路且可以结合则必然结合，2、有多条路但是只有一条路可以结合则必然结合（尚未实现）
	for (int i = 0; i < m_graphElements.size(); ++i)
	{
		if (m_graphElements[i]->m_neighbourElement.size() == 2 &&
			isAngleOK(m_graphElements[i]->m_neighbourElement[0], m_graphElements[i], m_graphElements[i]->m_neighbourElement[1]))
		{//如果该点只有一条通路，而且满足通路的角度符合要求则将两条线路合并
			int indexA = -1, indexB = -1;
			bool isHeadA = false, isHeadB = false;
			for (int j = 0; j < m_tmpPlanRoads.size(); ++j)
			{
				if (indexA < 0)
				{
					if (m_tmpPlanRoads[j][0] == m_graphElements[i])
					{
						indexA = j;
						isHeadA = true;
					}
					else
						if (*(m_tmpPlanRoads[j].end() - 1) == m_graphElements[i])
						{
							indexA = j;
							isHeadA = false;
						}
				}
				else
					if (indexB < 0)
					{
						if (m_tmpPlanRoads[j][0] == m_graphElements[i])
						{
							indexB = j;
							isHeadB = true;
							break;
						}
						else
							if (*(m_tmpPlanRoads[j].end() - 1) == m_graphElements[i])
							{
								indexB = j;
								isHeadB = false;
								break;
							}
					}
			}
			//合并两条线路
			assert(indexA >= 0 && indexB >= 0);
			if (isHeadA)
			{
				if (isHeadB)
				{//头头
					if (m_tmpPlanRoads[indexB].size() > 0)
					{
						for (auto it = m_tmpPlanRoads[indexB].begin() + 1; it != m_tmpPlanRoads[indexB].end(); ++it)
						{
							m_tmpPlanRoads[indexA].push_front(*it);
						}
					}
				}
				else
				{//头尾
					m_tmpPlanRoads[indexA].insert(m_tmpPlanRoads[indexA].begin(),
						m_tmpPlanRoads[indexB].begin(), m_tmpPlanRoads[indexB].end() - 1);
				}
			}
			else
			{
				if (isHeadB)
				{//尾头
					m_tmpPlanRoads[indexA].insert(m_tmpPlanRoads[indexA].end(),
						m_tmpPlanRoads[indexB].begin() + 1, m_tmpPlanRoads[indexB].end());
				}
				else
				{//尾尾
					if (m_tmpPlanRoads[indexB].size() >= 2)
					{
						for (std::deque<XGraphElement *>::iterator it = m_tmpPlanRoads[indexB].end() - 2;; --it)
						{
							m_tmpPlanRoads[indexA].push_back(*it);
							if (it == m_tmpPlanRoads[indexB].begin()) break;
						}
					}
				}
			}
			m_tmpPlanRoads.erase(m_tmpPlanRoads.begin() + indexB);
		}
		else
			if (m_graphElements[i]->m_neighbourElement.size() > 2)
			{//如果存在多个通路，但是只有一条可行的通路，则必须结合
				int sum = 0, indexNA = 0, indexNB = 0;
				for (int j = 0; j < int(m_graphElements[i]->m_neighbourElement.size()) - 1; ++j)
				{
					for (int k = j + 1; k < m_graphElements[i]->m_neighbourElement.size(); ++k)
					{
						if (!isAngleOK(m_graphElements[i]->m_neighbourElement[j],
							m_graphElements[i], m_graphElements[i]->m_neighbourElement[k])) continue;
						indexNA = j;
						indexNB = k;
						++sum;
						if (sum > 1) break;
					}
					if (sum > 1) break;
				}
				if (sum == 1)
				{//只有一条可行的通路，则必然联通
					int indexA = -1, indexB = -1;
					bool isHeadA = false, isHeadB = false;
					for (int j = 0; j < m_tmpPlanRoads.size(); ++j)
					{
						if (m_tmpPlanRoads[j][0] == m_graphElements[i] &&
							(m_tmpPlanRoads[j][1] == m_graphElements[i]->m_neighbourElement[indexNA] ||
								m_tmpPlanRoads[j][1] == m_graphElements[i]->m_neighbourElement[indexNB]))
						{
							if (indexA < 0)
							{
								indexA = j;
								isHeadA = true;
							}
							else
							{
								indexB = j;
								isHeadB = true;
								break;
							}
						}
						else
							if (*(m_tmpPlanRoads[j].end() - 1) == m_graphElements[i] &&
								(*(m_tmpPlanRoads[j].end() - 2) == m_graphElements[i]->m_neighbourElement[indexNA] ||
									*(m_tmpPlanRoads[j].end() - 2) == m_graphElements[i]->m_neighbourElement[indexNB]))
							{
								if (indexA < 0)
								{
									indexA = j;
									isHeadA = false;
								}
								else
								{
									indexB = j;
									isHeadB = false;
									break;
								}
							}
					}
					//合并两条线路
					assert(indexA >= 0 && indexB >= 0);
					if (isHeadA)
					{
						if (isHeadB)
						{//头头
							if (m_tmpPlanRoads[indexB].size() > 0)
							{
								for (auto it = m_tmpPlanRoads[indexB].begin() + 1; it != m_tmpPlanRoads[indexB].end(); ++it)
								{
									m_tmpPlanRoads[indexA].push_front(*it);
								}
							}
						}
						else
						{//头尾
							m_tmpPlanRoads[indexA].insert(m_tmpPlanRoads[indexA].begin(),
								m_tmpPlanRoads[indexB].begin(), m_tmpPlanRoads[indexB].end() - 1);
						}
					}
					else
					{
						if (isHeadB)
						{//尾头
							m_tmpPlanRoads[indexA].insert(m_tmpPlanRoads[indexA].end(),
								m_tmpPlanRoads[indexB].begin() + 1, m_tmpPlanRoads[indexB].end());
						}
						else
						{//尾尾
							if (m_tmpPlanRoads[indexB].size() >= 2)
							{
								for (std::deque<XGraphElement *>::iterator it = m_tmpPlanRoads[indexB].end() - 2;; --it)
								{
									m_tmpPlanRoads[indexA].push_back(*it);
									if (it == m_tmpPlanRoads[indexB].begin()) break;
								}
							}
						}
					}
					m_tmpPlanRoads.erase(m_tmpPlanRoads.begin() + indexB);
				}
			}
	}
	//下面遍历所有非必然合并的线路
	tryMergeRoad(0, m_tmpPlanRoads, enableCross);
}
void XGraph::tryMergeRoad(int index, std::vector<std::deque<XGraphElement *>> &roads, bool enableCross)
{
	bool canMerge = false;
	//std::vector<std::deque<XGraphElement *>> tmpRoads;
	for (int i = index; i < int(roads.size()) - 1; ++i)
	{
		XGraphElement *end = *(roads[i].end() - 1);
		XGraphElement *end1 = *(roads[i].end() - 2);
		for (int j = i + 1; j < roads.size(); ++j)
		{
			if (roads[i][0] == roads[j][0] && isAngleOK(roads[i][1], roads[i][0], roads[j][1]) &&
				(enableCross || !isInMiddle(roads[i][0], roads)))
			{//头头
				std::deque<XGraphElement *> tmp = roads[j];	//这里会造成内存复制的开销，可以优化掉
				if (tmp.size() > 0)
				{
					for (auto it = tmp.begin() + 1; it != tmp.end(); it++)
					{
						roads[i].push_front(*it);
					}
				}
				roads.erase(roads.begin() + j);
				tryMergeRoad(index, roads, enableCross);
				roads[i].erase(roads[i].begin(), roads[i].begin() + (tmp.size() - 1));
				roads.insert(roads.begin() + j, tmp);

				canMerge = true;
			}
			else
				if (end == roads[j][0] &&
					isAngleOK(roads[j][1], roads[j][0], end1) &&
					(enableCross || !isInMiddle(roads[j][0], roads)))
				{//尾头
					std::deque<XGraphElement *> tmp = roads[j];	//这里会造成内存复制的开销，可以优化掉
					roads[i].insert(roads[i].end(), tmp.begin() + 1, tmp.end());
					roads.erase(roads.begin() + j);
					tryMergeRoad(index, roads, enableCross);
					//还原现场
					roads[i].erase(roads[i].begin() + (roads[i].size() - tmp.size() + 1), roads[i].end());
					roads.insert(roads.begin() + j, tmp);

					canMerge = true;
				}
				else
					if (roads[i][0] == *(roads[j].end() - 1) &&
						isAngleOK(roads[i][1], roads[i][0], *(roads[j].end() - 2)) &&
						(enableCross || !isInMiddle(roads[i][0], roads)))
					{//头尾
						std::deque<XGraphElement *> tmp = roads[j];	//这里会造成内存复制的开销，可以优化掉
						roads[i].insert(roads[i].begin(), tmp.begin(), tmp.end() - 1);
						roads.erase(roads.begin() + j);
						tryMergeRoad(index, roads, enableCross);
						roads[i].erase(roads[i].begin(), roads[i].begin() + tmp.size() - 1);
						roads.insert(roads.begin() + j, tmp);

						canMerge = true;
					}
					else
						if (end == *(roads[j].end() - 1) &&
							isAngleOK(end1, end, *roads[j].end() - 2) &&
							(enableCross || !isInMiddle(end, roads)))
						{//尾尾
							std::deque<XGraphElement *> tmp = roads[j];	//这里会造成内存复制的开销，可以优化掉
							if (tmp.size() >= 2)
							{
								for (auto it = tmp.end() - 2;; --it)
								{
									roads[i].push_back(*it);
									if (it == tmp.begin()) break;
								}
							}
							roads.erase(roads.begin() + j);
							tryMergeRoad(index, roads, enableCross);
							roads[i].erase(roads[i].begin() + (roads[i].size() - tmp.size() + 1), roads[i].end());
							roads.insert(roads.begin() + j, tmp);
							canMerge = true;
						}
		}
		if (canMerge) return;	//如果能结合而不结合则绝对不是最优解
	}
	//如果没有可以合并的，则这里判断结果是否为最优
	if (canMerge) return;
	if (roads.size() < m_roads.size() || m_roads.size() == 0)
	{//长度优先
		m_roadsMaxLen = getMaxRoadLen(roads);
		m_roads = roads;	//记录最优的结果
	}
	else
		if (roads.size() == m_roads.size())
		{//等长时以最大长度为标准
			int maxLen = getMaxRoadLen(roads);
			if (maxLen > m_roadsMaxLen)
			{
				m_roadsMaxLen = maxLen;
				m_roads = roads;	//记录最优的结果
			}
		}
}
}