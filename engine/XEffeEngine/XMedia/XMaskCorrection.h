#ifndef _JIA_XMASKCORRECTION_
#define _JIA_XMASKCORRECTION_

#include "XProjectionCorrection.h"
namespace XE {
class XMaskCorrection
{
private:
	XProjectionCorrection m_maskCorrection;
	XTextureData m_maskTex;
	XFBO m_totallFbo;
	XShaderGLSL m_maskShader;
	unsigned int m_maskTexID;
	bool m_isInietd;
public:
	XMaskCorrection()
		:m_isInietd(false)
	{}
	bool init(const char* maskFilename,const char* saveFilename, XResPos respos = RES_AUTO);
	void begin(bool withMask = true);
	void end();
	void draw();
	void setEnableInput(bool flag) { m_maskCorrection.setEnableInput(flag); }
	bool inputProc(const XInputEvent &event) { return m_maskCorrection.inputProc(event);}

	bool getIsShowCtrl()const { return m_maskCorrection.getIsShowCtrl(); }
	void setIsShowCtrl(bool flag) { m_maskCorrection.setIsShowCtrl(flag); }
};
}
#endif