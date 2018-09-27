#include "XStdHead.h"
#include "XBowyerWatson.h"
#include "XMath\XVector2.h"
namespace XE{
void XBowyerWatson::clearBowyerWatson()
{
	m_points.clear();
	m_lstAddPointList.clear();				//Point
}
void XBowyerWatson::createHelperPoint(const XVec2& pt1, const XVec2& pt2, const XVec2& pt3, const XVec2& pt4)
{
	//加入辅助点4个
	int p0 = addBowyerWatsonPoint(pt1);
	int p1 = addBowyerWatsonPoint(pt2);
	int p2 = addBowyerWatsonPoint(pt3);
	int p3 = addBowyerWatsonPoint(pt4);

	mHelperPoints[0] = p0;
	mHelperPoints[1] = p1;
	mHelperPoints[2] = p2;
	mHelperPoints[3] = p3;
	//加入辅助窗体的5条边
	addBowyerWatsonLine(p1, p0);
	addBowyerWatsonLine(p2, p1);
	addBowyerWatsonLine(p3, p2);
	addBowyerWatsonLine(p3, p0);
	addBowyerWatsonLine(p3, p1);
	//加入辅助三角形2个
	addBowyerWatsonTriangle(p0, p1, p3);
	addBowyerWatsonTriangle(p1, p2, p3);
}
void XBowyerWatson::addNewPoint(const XVec2& pt)
{
	for (auto it = m_lstAddPointList.begin(); it != m_lstAddPointList.end(); ++it)
	{
		if (it->x == pt.x && it->y == pt.y)
			return;//存在重复的点，这里为什么要排除重复的点呢?
	}
	m_lstAddPointList.push_back(pt);
}
void XBowyerWatson::updateNewPoint()
{
	//处理所有的点
	for (auto it = m_lstAddPointList.begin(); it != m_lstAddPointList.end(); ++it)
	{
		processNewPoint(*it);
	}
	m_lstAddPointList.clear();
}
int XBowyerWatson::addBowyerWatsonPoint(const XVec2& pt)
{//整理完成
	for (int i = 0; i < m_points.size(); ++i)
	{
		if (pt.x == m_points[i].m_position.x &&
			pt.y == m_points[i].m_position.y)
			return i;
	}
	m_points.push_back(XPointInfo(pt));
	return int(m_points.size()) - 1;
}
//下面优化该函数试衣镜排好序的py > px
void XBowyerWatson::addBowyerWatsonLine(int py, int px)
{
	std::list<int>::iterator iterLine = m_points[px].m_lines.begin();
	for (; iterLine != m_points[px].m_lines.end(); ++iterLine)
	{
		if (*iterLine == py) return;
	}
	m_points[px].m_lines.push_back(py);
}
//void XBowyerWatson::addBowyerWatsonLine(int p0,int p1)
//{
//	int px,py;
//	if(p0 < p1)
//	{
//		px = p0;py = p1;
//	}else
//	{
//		px = p1;py = p0;
//	}
//	std::list<int>::iterator iterLine = m_points[px].m_lines.begin();
//	for(;iterLine != m_points[px].m_lines.end();++ iterLine)
//	{
//		if(*iterLine == py) return;
//	}
//	m_points[px].m_lines.push_back(py);
//}
void XBowyerWatson::delBowyerWatsonLine(int p0, int p1)
{
	int px, py;
	if (p0 < p1)
	{
		px = p0; py = p1;
	}
	else
	{
		px = p1; py = p0;
	}
	std::list<int>::iterator iterLine = m_points[px].m_lines.begin();
	while (iterLine != m_points[px].m_lines.end())
	{
		if (*iterLine == py)
		{
			m_points[px].m_lines.erase(iterLine);
			break;
		}
		++iterLine;
	}
	//下面三角形的剔除会存在问题
	std::list<TInfo>::iterator iterTriangle = m_points[px].m_triangles.begin();
	while (iterTriangle != m_points[px].m_triangles.end())
	{
		if ((*iterTriangle).p0 == py || (*iterTriangle).p1 == py)
		{//这里只剔除了一个三角形
			m_points[px].m_triangles.erase(iterTriangle);
			return;
		}
		++iterTriangle;
	}
	for (int i = 0; i < px; ++i)
	{
		iterTriangle = m_points[i].m_triangles.begin();
		while (iterTriangle != m_points[i].m_triangles.end())
		{
			if (((*iterTriangle).p0 == p0 && (*iterTriangle).p1 == p1) ||
				((*iterTriangle).p0 == p1 && (*iterTriangle).p1 == p0))
			{//这里只剔除了一个三角形(可能会存在问题)
				m_points[i].m_triangles.erase(iterTriangle);
				return;
			}
			++iterTriangle;
		}
	}
}
void XBowyerWatson::addBowyerWatsonTriangle(int p0, int p1, int p2)
{
	int px, py, pz;
	if (p0 < p1 && p0 < p2)
	{
		px = p0; py = p1; pz = p2;
	}
	else
	if (p1 < p0 && p1 < p2)
	{
		px = p1; py = p0; pz = p2;
	}
	else
	{
		px = p2; py = p0; pz = p1;
	}
	std::list<TInfo>::iterator iterTriangle = m_points[px].m_triangles.begin();
	for (; iterTriangle != m_points[px].m_triangles.end(); ++iterTriangle)
	{
		if (((*iterTriangle).p0 == py && (*iterTriangle).p1 == pz) ||
			((*iterTriangle).p0 == pz && (*iterTriangle).p1 == py))
			return;
	}
	float r;
	XVec2 c;
	calTriangleInfo(px, py, pz, r, c);	//这个数据丢失了
	//将三角形信息写入
	m_points[px].m_triangles.push_back(TInfo(py, pz, r, c));
}
void XBowyerWatson::calTriangleInfo(int px, int py, int pz, float &r, XVec2& c)	//计算三角形的相关数据
{
	const XVec2& p1 = m_points[px].m_position;
	const XVec2& p2 = m_points[py].m_position;
	const XVec2& p3 = m_points[pz].m_position;
	r = sqrt(p1.getLengthSqure(p2)*p2.getLengthSqure(p3)*p3.getLengthSqure(p1)) / fabs(p1.x * p2.y + p2.x * p3.y
		+ p3.x * p1.y - p2.x * p1.y - p3.x * p2.y - p1.x * p3.y) * 0.5f;
	r *= r;

	float c1, c2;
	float xA, yA, xB, yB, xC, yC;

	xA = p1.x; yA = p1.y;
	xB = p2.x; yB = p2.y;
	xC = p3.x; yC = p3.y;
	c1 = (xA * xA + yA * yA - xB * xB - yB * yB) * 0.5f;
	c2 = (xA * xA + yA * yA - xC * xC - yC * yC) * 0.5f;

	c.x = (c1 * (yA - yC) - c2 * (yA - yB)) /
		((xA - xB) * (yA - yC) - (xA - xC) * (yA - yB));
	c.y = (c1 * (xA - xC) - c2 * (xA - xB)) /
		((yA - yB) * (xA - xC) - (yA - yC) * (xA - xB));
}
void XBowyerWatson::delBowyerWatsonTriangle(int p0, int p1, int p2)
{
	int px, py, pz;
	if (p0 < p1 && p0 < p2)
	{
		px = p0; py = p1; pz = p2;
	}
	else
	if (p1 < p0 && p1 < p2)
	{
		px = p1; py = p0; pz = p2;
	}
	else
	{
		px = p2; py = p0; pz = p1;
	}
	std::list<TInfo>::iterator iterTriangle = m_points[px].m_triangles.begin();
	for (; iterTriangle != m_points[px].m_triangles.end(); ++iterTriangle)
	{
		if (((*iterTriangle).p0 == py && (*iterTriangle).p1 == pz) ||
			((*iterTriangle).p0 == pz && (*iterTriangle).p1 == py))
		{
			m_points[px].m_triangles.erase(iterTriangle);
			break;
		}
	}
}
void XBowyerWatson::processNewPoint(const XVec2&pt)
{
	int pIndex = addBowyerWatsonPoint(pt);

	m_commonTriangleVector.clear();
	std::list<TInfo>::iterator iterTriangle;
	for (int i = 0; i < m_points.size(); ++i)
	{
		iterTriangle = m_points[i].m_triangles.begin();
		for (; iterTriangle != m_points[i].m_triangles.end(); ++iterTriangle)
		{
			if ((*iterTriangle).checkInCircle(pt))
				m_commonTriangleVector.push_back(XTriangle(i, (*iterTriangle).p0, (*iterTriangle).p1));
		}
	}
	if (m_commonTriangleVector.size() == 1)
	{
		std::vector<XTriangle>::iterator iter_v = m_commonTriangleVector.begin();

		////////////////////////////////
		//删除三角形
		delBowyerWatsonTriangle((*iter_v).m_p0, (*iter_v).m_p1, (*iter_v).m_p2);

		/////////////////////////////////
		//连接三角形三点
		addBowyerWatsonLine(pIndex, (*iter_v).m_p0);
		addBowyerWatsonLine(pIndex, (*iter_v).m_p1);
		addBowyerWatsonLine(pIndex, (*iter_v).m_p2);

		//加入新三角形
		if (checkTriangleLinesExist((*iter_v).m_p0, (*iter_v).m_p1))
			addBowyerWatsonTriangle(pIndex, (*iter_v).m_p0, (*iter_v).m_p1);
		if (checkTriangleLinesExist((*iter_v).m_p1, (*iter_v).m_p2))
			addBowyerWatsonTriangle(pIndex, (*iter_v).m_p1, (*iter_v).m_p2);
		if (checkTriangleLinesExist((*iter_v).m_p2, (*iter_v).m_p0))
			addBowyerWatsonTriangle(pIndex, (*iter_v).m_p2, (*iter_v).m_p0);
	}
	if (m_commonTriangleVector.size() > 1)
	{
		int lineIndex;
		for (int i = 0; i < int(m_commonTriangleVector.size()) - 1; ++i)
		{
			XTriangle &trg1 = *(m_commonTriangleVector.begin() + i);
			for (int j = i + 1; j < m_commonTriangleVector.size(); ++j)
			{
				XTriangle &trg2 = *(m_commonTriangleVector.begin() + j);

				if (trg1.findCommonLine(trg2, lineIndex))
				{
					////////////////////////////////
					//删除影响三角形
					delBowyerWatsonTriangle(trg1.m_p0, trg1.m_p1, trg1.m_p2);
					delBowyerWatsonTriangle(trg2.m_p0, trg2.m_p1, trg2.m_p2);

					//删除公共边
					//delBowyerWatsonLine(commonLine.m_p0,commonLine.m_p1);	//(这里重复了上面的操作)

					/////////////////////////////////
					//连接三角形三点，(这里应该只需要增加4条线，因为2条是重复的)
					switch (lineIndex)
					{
					case 1:
						delBowyerWatsonLine(trg1.m_p1, trg1.m_p2);	//(这里重复了上面的操作)
						addBowyerWatsonLine(pIndex, trg1.m_p0);
						break;
					case 2:
						delBowyerWatsonLine(trg1.m_p0, trg1.m_p2);	//(这里重复了上面的操作)
						addBowyerWatsonLine(pIndex, trg1.m_p1);
						break;
					case 0:
						delBowyerWatsonLine(trg1.m_p0, trg1.m_p1);	//(这里重复了上面的操作)
						addBowyerWatsonLine(pIndex, trg1.m_p2);
						break;
					}
					addBowyerWatsonLine(pIndex, trg2.m_p0);
					addBowyerWatsonLine(pIndex, trg2.m_p1);
					addBowyerWatsonLine(pIndex, trg2.m_p2);

					//加入新三角形(必须要三条线都已经存在才能加入,为什么?)
					switch (lineIndex)
					{
					case 1:
						if (checkTriangleLinesExist(trg1.m_p0, trg1.m_p1))
							addBowyerWatsonTriangle(pIndex, trg1.m_p0, trg1.m_p1);
						if (checkTriangleLinesExist(trg1.m_p2, trg1.m_p0))
							addBowyerWatsonTriangle(pIndex, trg1.m_p2, trg1.m_p0);
						break;
					case 2:
						if (checkTriangleLinesExist(trg1.m_p0, trg1.m_p1))
							addBowyerWatsonTriangle(pIndex, trg1.m_p0, trg1.m_p1);
						if (checkTriangleLinesExist(trg1.m_p1, trg1.m_p2))
							addBowyerWatsonTriangle(pIndex, trg1.m_p1, trg1.m_p2);
						break;
					case 0:
						if (checkTriangleLinesExist(trg1.m_p1, trg1.m_p2))
							addBowyerWatsonTriangle(pIndex, trg1.m_p1, trg1.m_p2);
						if (checkTriangleLinesExist(trg1.m_p2, trg1.m_p0))
							addBowyerWatsonTriangle(pIndex, trg1.m_p2, trg1.m_p0);
						break;
					}

					if (checkTriangleLinesExist(trg2.m_p0, trg2.m_p1))
						addBowyerWatsonTriangle(pIndex, trg2.m_p0, trg2.m_p1);
					if (checkTriangleLinesExist(trg2.m_p1, trg2.m_p2))
						addBowyerWatsonTriangle(pIndex, trg2.m_p1, trg2.m_p2);
					if (checkTriangleLinesExist(trg2.m_p2, trg2.m_p0))
						addBowyerWatsonTriangle(pIndex, trg2.m_p2, trg2.m_p0);
				}
			}
		}
	}
}
bool XBowyerWatson::checkTriangleLinesExist(const int &p0, const int &p1)
{
	int px, py;
	if (p0 < p1)
	{
		px = p0; py = p1;
	}
	else
	{
		px = p1; py = p0;
	}
	std::list<int>::iterator iterLine = m_points[px].m_lines.begin();
	for (; iterLine != m_points[px].m_lines.end(); ++iterLine)
	{
		if ((*iterLine) == py) return true;
	}
	return false;
}
bool XBowyerWatson::checkTriangleLinesExist(const int &pt1, const int &pt2, const int &pt3)
{
	return checkTriangleLinesExist(pt1,pt2) && checkTriangleLinesExist(pt1,pt3) && checkTriangleLinesExist(pt2,pt3);
}
}