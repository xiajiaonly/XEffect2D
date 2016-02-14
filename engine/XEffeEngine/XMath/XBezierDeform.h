//这个是基于贝塞尔的平面变形的类
#ifndef _JIA_XBEZIERDEFORM_
#define _JIA_XBEZIERDEFORM_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.4.3
//--------------------------------
#include "XMesh2D.h"

//没有加入但是可以考虑加入的调整:疏密调整(以水平或者垂直的某个点为基准调整齐周围的疏密)
//网格等距曲线调整
//四个角位置变形调整

//这种方式是将整个图片分为一个一个的小单元，然后每个单元内部进行贝塞尔变形，从而试图实现图片的整体变形
//实际上这种方法是错误的，此种方法会造成图片中每个贝塞尔单元交界处的图像不连续，正确的做法应该是对整个
//图像进行统一的贝塞尔变形，请具体实现请参看Ex的例子

//贝塞尔曲面变形，功能已经基本实现，但是性能很差
#define OPTIMIZATION

namespace XE{
#define DEFAULT_BD_FILENAME "bdData.xml"
enum XBDChangeType
{
	BD_CHANGE_NULL,		//没有变形动作
	BD_CHANGE_MOVE,		//移动
	BD_CHANGE_SCALE,	//缩放
	BD_CHANGE_ROTATE,	//旋转
};
struct XInputEvent;
class XBezierDeform
{
private:
	bool m_isInited;
	XMesh2D m_mesh;
	int m_vSum;//控制点之间的顶点数量
	int m_handSumX;				//控制点的数量
	int m_handSumY;
	int m_mouseTouchDistance;	//鼠标触碰点的距离

	//bool handleFlag[HAND_SUM];	//调节点的状态是否发生变化,标记变化范围，更新时只更新变化范围内的数据，（！！目前功能尚未实现，性能可以提升一倍）
	std::vector<XVector2> m_handlePoint;	//用于调节的点
	std::vector<XVector2> m_tmpHandlePoint;
	int m_curMouseIndex;
	XVector2 m_mouseDownPos;	//鼠标按下的位置
	std::vector<XVector2> m_changeOldPoint;

	bool m_isMouseDown;
	int getCurMouseChoose(float x,float y)
	{
		for(int i = 0;i < m_handSumX * m_handSumY;++ i)
		{
			if(m_handlePoint[i].getLengthSqure(x,y) < m_mouseTouchDistance * m_mouseTouchDistance) return i;	//判断生效的距离
		}
		return -1;
	} 
	void updateNet();
	bool m_needDrawHandle;	//是否需要描绘控制点
	XBDChangeType m_changeType;	//是否处于变形状态
	bool load(const char *filename = NULL); //从文件中读取数据
public:
	XBezierDeform()
		:m_isInited(false)
		,m_handlePoint(NULL)
		,m_isMouseDown(false)
		,m_needDrawHandle(false)
		,m_curMouseIndex(-1)
		,m_mouseTouchDistance(10)
		,m_changeType(BD_CHANGE_NULL)
	{}
	//w像素宽度，h像素高度
	//handX,水平方向的控制点的数量
	//handY,垂直方向的控制点的数量
	//vSum，控制点之间的顶点数量
	bool init(int w,int h,int handX,int handY,int vSum);
	bool initFromFile(int w,int h,const char *filename = NULL);
	void draw(unsigned int tex);
	bool save(const char *filename = NULL)const;//将数据保存到文件
	void inputProc(const XInputEvent &event);
};
}
#endif