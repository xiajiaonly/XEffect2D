//这个是基于贝塞尔的平面变形的类
#ifndef _JIA_XBEZIERDEFORM_
#define _JIA_XBEZIERDEFORM_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.4.3
//--------------------------------
#include "../XMesh2DBz.h"
#include "../XFont/XFontUnicode.h"
#include "XMoveData.h"

//没有加入但是可以考虑加入的调整:疏密调整(以水平或者垂直的某个点为基准调整齐周围的疏密)
//网格等距曲线调整
//四个角位置变形调整

//这种方式是将整个图片分为一个一个的小单元，然后每个单元内部进行贝塞尔变形，从而试图实现图片的整体变形
//实际上这种方法是错误的，此种方法会造成图片中每个贝塞尔单元交界处的图像不连续，正确的做法应该是对整个
//图像进行统一的贝塞尔变形，请具体实现请参看Ex的例子

//贝塞尔曲面变形，功能已经基本实现，但是性能很差

namespace XE{
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
	bool m_withFilename;
	std::string m_filename;
	XMesh2DBz m_mesh;
	bool m_showHandle;
	bool m_onlyHandle;	//是否只显示点
	int m_mouseTouchDistance;	//鼠标触碰点的距离

	XVec2 m_mouseDownPos;	//鼠标按下的位置
	XVec2 m_oldMousePos;
	std::vector<XVec2> m_changeOldPoint;

	bool m_isMouseDown;
	XBDChangeType m_changeType;	//是否处于变形状态
	void release();
	//鼠标的坐标
	XVec2 m_mouseOffset;
	XVec2 m_mouseSacle;
public:
	void setMouseParam(const XVec2& offset, const XVec2& scale)	//设置鼠标的参数
	{
		m_mouseOffset = offset;
		m_mouseSacle = scale;
	}
	XBezierDeform()
		:m_isInited(false)
		, m_isMouseDown(false)
		, m_mouseTouchDistance(20)
		, m_changeType(BD_CHANGE_NULL)
		, m_showHandle(false)
		, m_onlyHandle(false)
		, m_withScale(false)
		, m_mouseOffset(0.0f)
		, m_mouseSacle(1.0f)
		, m_withFilename(false)
	{}
	void setFilename(const std::string& filename)
	{
		m_withFilename = true;
		m_filename = filename;
	}
	~XBezierDeform() { release(); }
	//w像素宽度，h像素高度
	//handX,水平方向的控制点的数量
	//handY,垂直方向的控制点的数量
	//vSum，控制点之间的顶点数量
	bool init(int w, int h, int handX = 4, int handY = 4, int vSum = 10);
	bool initFromFile(const char *filename = NULL);
	void draw(unsigned int tex,bool withWireframe = true)
	{
		if (!m_isInited) return;
		m_mesh.draw(tex);
		if (!m_showHandle) return;
		if (withWireframe)
		{
			if (m_withScale)
				m_mesh.drawWireframeEx(m_basePosition, m_scale, true, false);
			else
				m_mesh.drawWireframeEx();
		}
	}
	void drawHandle()
	{
		if (!m_isInited || !m_showHandle) return;
		if (m_withScale)
			m_mesh.drawWireframeEx(m_basePosition, m_scale, false, true);
		else
			m_mesh.drawWireframeEx(true, true);
	}
	void inputProc(const XInputEvent &event);
	void reset()	//重置所有点
	{
		if (!m_isInited) return;
		m_mesh.reset();
	}
	void setOnlyHandle(bool flag) { m_onlyHandle = flag; }
	bool getIsShowHandle()const { return m_showHandle; }
	XVec2 mapWorld2Local(const XVec2& pos)	//将世界坐标映射到局部坐标
	{
		XVec2 ret;
		if (m_mesh.mapVertex2Tex(pos, ret)) return ret * m_mesh.getPixelSize();
		return XVec2::zero;
	}
	XVec2 mapLocal2World(const XVec2& pos)	//将世界坐标映射到局部坐标
	{
		XVec2 ret;
		if (m_mesh.mapTex2Vertex(pos / m_mesh.getPixelSize(), ret)) return ret;
		return XVec2::zero;
	}
private:
	bool m_isInputEnable;
	bool m_withScale;
	float m_scale;
	XVec2 m_basePosition;
public:
	bool getEnableInput()const { return m_isInputEnable; }
	void setEnableInput(bool flag) { m_isInputEnable = flag; }
	void setScale(const XVec2& base, float scale)
	{
		m_basePosition = base;
		m_scale = scale;
		m_withScale = true;
	}
	XVec2 getScalePos(const XVec2& pos)const	//返回缩放之前的比例
	{
		if (m_withScale) return (pos - m_basePosition) / m_scale + m_basePosition;
		else return pos;
	}
};
}
#endif