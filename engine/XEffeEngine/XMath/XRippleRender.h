#ifndef _JIA_XRIPPLERENDER_
#define _JIA_XRIPPLERENDER_

#include "X3D/XCubeMap.h"
#include "XShaderGLSL.h"
#include "XMath\XMatrix4x4.h"

namespace XE {
class XRippleRender
{
private:
	bool m_isInited;

	XShaderGLSL m_rendShader;

	XCubeMap m_cubeMap;	//»·¾³ÌùÍ¼

	unsigned int m_colorTex;
	unsigned int m_cubeTex;
public:
	XRippleRender()
		:m_isInited(false)
	{}

	bool init(const char* cubeFilename, XResPos resPos = RES_SYS_DEF);
	void draw(unsigned int rTex, unsigned int tex, int w, int h);
};
}
#endif