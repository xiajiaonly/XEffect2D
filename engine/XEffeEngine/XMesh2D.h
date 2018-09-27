#ifndef _JIA_XMESH2D_
#define _JIA_XMESH2D_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------

#include "XMath\XVector2.h"
#include "XMath\XFColor.h"
#include "XShaderGLSL.h"
#include <vector>
//这里是对2Dmesh的封装
//暂时先不使用VBO进行优化，之后再考虑这步优化

//1、混合方式控制
//2、支持shader
namespace XE{
struct XMesh2DInfo
{
	int w;
	int h;
	int sumX;
	int sumY;
	bool withT;
	float tSx;	//贴图的x起点
	float tSy;
	float tw;
	float th;
	bool withC;
	XMesh2DInfo()
		:w(0)
		, h(0)
		, sumX(0)
		, sumY(0)
		, withT(false)
		, withC(false)
		, tSx(0.0f)
		, tSy(0.0f)
		, tw(1.0f)
		, th(1.0f)
	{}
};
class XMesh2D
{
public:
	std::vector<XVec2> m_v;		//顶点坐标组
	std::vector<XVec2> m_t;		//贴图坐标组
	std::vector<XFColor> m_c;		//颜色坐标组
private:
	bool m_isInited;
	bool m_withTexture;
	bool m_withColor;

	XVec2 m_position;	//当前的位置
	XVec2 m_scale;		//缩放的比例
	XVec2 m_center;		//旋转缩放的相对位置，不进行归一化
	XFColor m_color;
	float m_angle;			//旋转角度
	int m_sumX;
	int m_sumY;
	//XTurnOverMode m_turnOverMode;	//翻转模式
	XVec2 world2Local(const XVec2& pos)const;	//世界空间到自身空间的转换
	XVec2 local2World(const XVec2& pos)const;	//自身空间到世界空间的转换
public:
	bool getIsInited()const { return m_isInited; }
	int getWidthSum()const { return m_sumX; }
	int getHeightSum()const { return m_sumY; }
	//	void setPosition(float x, float y) { m_position.set(x, y); }
	void setPosition(const XVec2& p) { m_position = p; }
	void setScale(const XVec2& s) { m_scale = s; }
	//	void setColor(float r, float g, float b, float a) { m_color.set(r, g, b, a); }
	void setColor(const XFColor&color) { m_color = color; }
	void setAngle(float a) { m_angle = a; }	//0 - 360,单位角度
	//w:mesh的像素宽度
	//h:mesh的像素高度
	//sumX:mesh的水平密度
	//sumY:mesh的垂直密度
	//withT:是否包含贴图数据
	//withC:是否包含颜色数据
	bool init(const XVec2& size, const XVec2& sum,
		bool withT = false, bool withC = false)
	{
		XMesh2DInfo info;
		info.w = size.x;
		info.h = size.y;
		info.sumX = sum.x;
		info.sumY = sum.y;
		info.withT = withT;
		info.withC = withC;
		return init(info);
	}
	bool init(int w, int h, int sumX, int sumY,
		bool withT = false, bool withC = false)
	{
		XMesh2DInfo info;
		info.w = w;
		info.h = h;
		info.sumX = sumX;
		info.sumY = sumY;
		info.withT = withT;
		info.withC = withC;
		return init(info);
	}
	bool init(const XMesh2DInfo &info);
	bool init(const char *filename)
	{
		if (m_isInited) return false;
		if (!loadFromFile(filename)) return false;
		m_isInited = true;
		return true;
	}
	bool initB(const char *filename)
	{
		if (m_isInited) return false;
		if (!loadFromFileB(filename)) return false;
		m_isInited = true;
		return true;
	}
	void setTextureInfo(float sx, float sy, float w, float h);

	//以文本的方式操作数据
	bool saveToFile(const char *filename)const;		//将数据保存到文件，建议使用OBJ的数据格式
	bool loadFromFile(const char *filename);		//从文件中读取数据
	//以二进制的方式操作数据
	bool saveToFileB(const char *filename)const;		//将数据保存到文件，建议使用OBJ的数据格式
	bool loadFromFileB(const char *filename);		//从文件中读取数据

	void drawWireframe()const;	//描绘线框
	void drawTexture(unsigned int tex, XShaderGLSL *pShader = NULL)const;
	//+++++++++++++++++++++++++++++++++++++++++++++++++
	//下面是为了网格调整而定义的一些接口
private:
	int m_specialIndex;	//特殊点的编号
	int m_specialLine;	//特殊的行
	int m_specialRow;	//特殊的列
public:
	void setSpecialIndex(int index) { m_specialIndex = index; }
	int getSpecialIndex()const { return m_specialIndex; }
	void setSpecialLine(int index) { m_specialLine = index; }
	int getSpecialLine()const { return m_specialLine; }
	void setSpecialRow(int index) { m_specialRow = index; }
	int getSpecialRow()const { return m_specialRow; }
	void drawWireframeEx(bool special = false, bool withPoint = true)const;	//描绘线框,描绘特诊等一些信息
	//结果是归一化的结果
	bool mapVertex2Tex(const XVec2& pos, XVec2& out)const;	//映射顶点坐标到贴图空间
	bool mapTex2Vertex(const XVec2& pos, XVec2& out)const;	//映射顶点坐标到贴图空间
//-------------------------------------------------
	XMesh2D()
		:m_isInited(false)
		, m_position(0.0f)
		, m_scale(1.0f)
		, m_center(64.0f)
//		,m_center(0.0f)
		, m_color(1.0f, 1.0f)
		, m_angle(0.0f)
		, m_specialIndex(-1)
		, m_specialLine(-1)
		, m_specialRow(-1)
	{}
	//下面是为了便于鼠标拖拽而提供的接口
	int getNearestVertex(const XVec2& pos, float maxD = 10.0f)const;	//获取离pos最近的顶点的编号，距离小于maxD
	void setVertexPos(const XVec2& pos, int index);	//设置顶点的位置
	XVec2 getVertexPos(int index)const;
	void setVertexColor(const XFColor& color, int index);
	void setVertexTex(const XVec2& t, int index);
	void clearAllData()
	{
		m_v.clear();
		m_t.clear();
		m_c.clear();
	}
	//尚未实现的接口
	void addALine();	//增加一行
	void addARow();		//增加一列
};
}
#endif