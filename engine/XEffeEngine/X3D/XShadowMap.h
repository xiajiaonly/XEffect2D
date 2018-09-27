#ifndef _JIA_XSHADOWMAP_
#define _JIA_XSHADOWMAP_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.10.1
//--------------------------------
#include "XShaderGLSL.h"
namespace XE{
//这是一个用于生成阴影的类，使用的方法是Shadowmap，这个方法速度快，
//但是只能对于射灯比较有效，对于点光源不适用，还需要处理边缘锯齿问题

#define SHADOW_MAP_TEXTURE_SIZE (1024)

class XShadowMap
{
private:
	XBool m_isInited;	//是否初始化
	XFBO m_fbo;	//暂时不使用FBO
public:
	unsigned int getShadowMapTexture(){return m_fbo.getTexture(0);}
	XBool init();
	void updateShadowMap();	//根据参数刷新shadowmap的内容
	//void draw();

	void release();

	XShadowMap()
		:m_isInited(XFalse)
	{}
	~XShadowMap(){release();}
};
}
#endif