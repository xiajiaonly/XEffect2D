#ifndef _JIA_XMESH2D_
#define _JIA_XMESH2D_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------

#include "XMath\XVector2.h"
#include "XMath\XFColor.h"
#include <vector>
//这里是对2Dmesh的封装
//暂时先不使用VBO进行优化，之后再考虑这步优化

//1、混合方式控制
//2、支持shader
namespace XE{
class XMesh2D
{
public:
	std::vector<XVector2> m_v;		//顶点坐标组
	std::vector<XVector2> m_t;		//贴图坐标组
	std::vector<XFColor> m_c;		//颜色坐标组
private:
	bool m_isInited;
	bool m_withTexture;
	bool m_withColor;

	XVector2 m_position;	//当前的位置
	XVector2 m_scale;		//缩放的比例
	XVector2 m_center;		//旋转缩放的相对位置，不进行归一化
	XFColor m_color;
	float m_angle;			//旋转角度
	int m_sumX;
	int m_sumY;
public:
	int getWidthSum()const{return m_sumX;}
	int getHeightSum()const{return m_sumY;}
	void setPosition(float x,float y){m_position.set(x,y);}
	void setPosition(const XVector2 &p){m_position = p;}
	void setScale(float x,float y){m_scale.set(x,y);}
	void setColor(float r,float g,float b,float a){m_color.setColor(r,g,b,a);}
	void setAngle(float a){m_angle = a;}	//0 - 360,单位角度
	//w:mesh的像素宽度
	//h:mesh的像素高度
	//sumX:mesh的水平密度
	//sumY:mesh的垂直密度
	//withT:是否包含贴图数据
	//withC:是否包含颜色数据
	bool init(int w,int h,int sumX,int sumY,bool withT = false,bool withC = false);
	bool init(const char *filename)
	{
		if(m_isInited) return false;
		if(!loadFromFile(filename)) return false;
		m_isInited = true;
		return true;
	}

	bool saveToFile(const char *filename);		//将数据保存到文件，建议使用OBJ的数据格式
	bool loadFromFile(const char *filename);	//从文件中读取数据

	void drawWireframe();	//描绘线框
	void drawTexture(unsigned int tex);

	XMesh2D()
		:m_isInited(false)
		,m_position(0.0f,0.0f)
		,m_scale(1.0f,1.0f)
		,m_center(64.0f,64.0f)
//		,m_center(0.0f,0.0f)
		,m_color(1.0f,1.0f,1.0f,1.0f)
		,m_angle(0.0f)
	{}
	//下面是为了便于鼠标拖拽而提供的接口
	int getNearestVertex(const XVector2 &pos,float maxD = 10.0f);	//获取离pos最近的顶点的编号，距离小于maxD
	void setVertexPos(const XVector2 &pos,int index);	//设置顶点的位置
	void clearAllData()
	{
		m_v.clear();
		m_t.clear();
		m_c.clear();
	}
};
}
#endif