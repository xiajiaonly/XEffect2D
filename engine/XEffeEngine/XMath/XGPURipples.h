#ifndef _JIA_XGPURIPPLES_
#define _JIA_XGPURIPPLES_

#include "XShaderGLSL.h"
#include "XTextureInformation.h"
#include <deque>

namespace XE {
class XGPURipples
{
private:
	bool m_isInited;
	XFBO m_fbo;
	int m_w;
	int m_h;
	XShaderGLSL m_calShader;
	unsigned int m_calTex;
	XTextureData m_maskTex;
	unsigned int m_maskTexID;
	unsigned int m_dropTexID;
	XVec2 m_dropPos;
	int m_isDrop;	//滴下是否有效
	std::vector<XVec2> m_dropsPos;
	bool m_texFlag;
public:
	void setMaskTexID(unsigned int id) { m_maskTexID = id; }
	unsigned int getCalTex()const { return m_calTex; }
	bool init(int w, int h,const char* maskFilename, XResPos resPos = RES_SYS_DEF);
	int getWidth()const { return m_w; }
	int getHeight()const { return m_h; }
	void update();
	void drop(const XVec2& pos)
	{
		if (!m_isInited || pos.x < 0 || pos.y < 0 ||
			pos.x > m_w || pos.y > m_h) return;
		m_dropsPos.push_back(pos);
	}

	XGPURipples()
		:m_isInited(false)
		, m_isDrop(0)
		, m_texFlag(true)
	{}
};
}
#endif