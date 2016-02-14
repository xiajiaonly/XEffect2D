#ifndef _JIA_XSKYBOX_
#define _JIA_XSKYBOX_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
#include "XBasicFun.h"
#include "XTextureInformation.h"
#include "XCam.h"
namespace XE{
//这是一个天空盒子的类
class XSkyBox
{
private:
	XBool m_isInited;				//是否完成初始化
	XTextureData m_texture[6];		//天空盒子的6个面
	float m_top;					//天空的范围
	float m_bottom;
	float m_front;
	float m_back;
	float m_left;
	float m_right;
public:
	void setBox(float top,float bottom,float left,float right,float front,float back)
	{
		m_top = top;
		m_bottom = bottom;
		m_left = left;
		m_right = right;
		m_front = front;
		m_back = back;
	}
	//传入的文件名大致为：....\top.png
	XBool init(const char *filename,XResourcePosition resourcePosition = RESOURCE_SYSTEM_DEFINE);
	void draw();

	XSkyBox()
		:m_isInited(XFalse)
		,m_top(-100.0f)					//天空的范围
		,m_bottom(100.0f)
		,m_front(100.0f)
		,m_back(-100.0f)
		,m_left(-100.0f)
		,m_right(100.0f)
	{}
	~XSkyBox() {release();}
	void release()
	{
		if(!m_isInited) return;
		m_isInited = XFalse;
	}
};
}
#endif	//_JIA_XSKYBOX_
