#ifndef _JIA_XMESH2DBZ_
#define _JIA_XMESH2DBZ_
#include "XMesh2D.h"
//这是一个经过贝塞尔插值的2D网格
#define OPTIMIZATION
namespace XE{
#define DEFAULT_BD_FILENAME "bdData"
class XMesh2DBz
{
private:
	bool m_isInited;
	XMesh2D m_mesh;
	int m_vSum;//控制点之间的顶点数量
	int m_handSumX;				//控制点的数量
	int m_handSumY;

	//bool handleFlag[HAND_SUM];	//调节点的状态是否发生变化,标记变化范围，更新时只更新变化范围内的数据，（！！目前功能尚未实现，性能可以提升一倍）
	std::vector<XVec2> m_handlePoint;	//用于调节的点
	std::vector<XVec2> m_tmpHandlePoint;

public:
	int getNearestPointIndex(const XVec2& pos, float distance = 10.0f)
	{
		int ret = -1;
		float nDis = 0.0f;
		float dis = distance * distance;
		float tmpDis;
		for (int i = 0; i < m_handSumX * m_handSumY; ++i)
		{
			tmpDis = m_handlePoint[i].getLengthSqure(pos);
			if (tmpDis > dis) continue;
			if (ret < 0 || tmpDis < nDis)
			{
				ret = i;
				nDis = tmpDis;
			}
		}
		return ret;
	}
	//	int getNearestPointIndex(float x,float y,float distance = 10.0f)
	//	{
	//		return getNearestPointIndex(XVec2(x, y), distance);
	//	} 
private:
	int m_w;
	int m_h;
	void release()
	{
		if (!m_isInited) return;
		m_isInited = false;
	}
public:
	XVec2 getPixelSize()const { return XVec2(m_w, m_h); }
	void updateMesh();
	void setHandlePos(const XVec2& pos, int index)
	{
		if (index < 0 || index >= m_handlePoint.size()) return;
		m_handlePoint[index] = pos;
	}
	XMesh2DBz()
		:m_isInited(false)
		, m_specialIndex(-1)
		, m_specialLine(-1)
		, m_specialRow(-1)
	{}
	~XMesh2DBz() { release(); }
	//w像素宽度，h像素高度
	//handX,水平方向的控制点的数量
	//handY,垂直方向的控制点的数量
	//vSum，控制点之间的顶点数量
	bool init(int w, int h, int handX, int handY, int vSum);
	bool initFromFile(const char *filename = NULL);
	bool initFromFileB(const char *filename = NULL);
	void draw(unsigned int tex, XShaderGLSL *pShader = NULL)const
	{
		if (!m_isInited) return;
		m_mesh.drawTexture(tex, pShader);
	}
	void drawWireframeEx(bool special = false, bool onlyHandle = false)const;	//描绘网格和点
	void drawWireframeEx(const XVec2& base, float scale, bool withWireframe, bool withHandle)const;	//描绘网格和点
	//以文本的方式操作数据
	bool save(const char *filename = NULL)const;//将数据保存到文件
	bool load(const char *filename = NULL); //从文件中读取数据
	//以二进制的方式操作数据
	bool saveB(const char *filename = NULL)const;//将数据保存到文件
	bool loadB(const char *filename = NULL); //从文件中读取数据

	void reset();	//重置所有点

	int getHandleSumX()const { return m_handSumX; }
	int getHandleSumY()const { return m_handSumY; }
	int getHandleSum()const { return m_handSumX * m_handSumY; }
	XVec2 getHandlePos(int index)const
	{
		if (index < 0 || index >= m_handlePoint.size()) return XVec2::zero;
		return m_handlePoint[index];
	}
	std::vector<XVec2> getHandlePoses()const { return m_handlePoint; }
	void setTextureInfo(float sx, float sy, float w, float h)
	{
		if (!m_isInited) return;
		m_mesh.setTextureInfo(sx, sy, w, h);
	}
private:
	int m_specialIndex;	//特殊点的编号
	int m_specialLine;	//特殊的行
	int m_specialRow;	//特殊的列
	XVec2 m_position;
public:
	void setSpecialIndex(int index) { m_specialIndex = index; }
	int getSpecialIndex()const { return m_specialIndex; }
	void setSpecialLine(int index) { m_specialLine = index; }
	int getSpecialLine()const { return m_specialLine; }
	void setSpecialRow(int index) { m_specialRow = index; }
	int getSpecialRow()const { return m_specialRow; }

	bool mapVertex2Tex(const XVec2& pos, XVec2& out)const { return m_mesh.mapVertex2Tex(pos, out); }
	bool mapTex2Vertex(const XVec2& pos, XVec2& out)const { return m_mesh.mapTex2Vertex(pos, out); }
	void setColor(const XFColor& c) { m_mesh.setColor(c); }

	void setPosition(const XVec2& pos)
	{
		XVec2 offset = pos - m_position;
		m_position = pos;
		for (auto it = m_handlePoint.begin(); it != m_handlePoint.end(); ++it)
		{
			(*it) += offset;
		}
		updateMesh();
	}
};
}
#endif