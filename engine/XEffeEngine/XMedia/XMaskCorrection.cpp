#include "XStdHead.h"
#include "XMaskCorrection.h"
#include "XFX\XFXCommon.h"

namespace XE {
bool XMaskCorrection::init(const char* maskFilename, const char* saveFilename, XResPos respos)
{
	if (m_isInietd) return false;

	m_maskCorrection.setConfigFilename(saveFilename);
	//m_maskCorrection.init(true,PC_MODE_TEXTURE_FILL);
	m_maskCorrection.init();

	XEG.setTexNeedFit2N(false);
	if(!m_maskTex.load(maskFilename, respos)) return false;
	XEG.setTexNeedFit2N(true);

	if (!m_totallFbo.init(XEG.getSceneW(), XEG.getSceneH(), COLOR_RGB)) return false;
	m_totallFbo.addOneTexture(XEG.getSceneW(), XEG.getSceneH(), COLOR_RGB);
	m_totallFbo.addOneTexture(XEG.getSceneW(), XEG.getSceneH(), COLOR_RGB);
	char tmpFrg[] = STRINGIFY(uniform sampler2D Texture00;
	uniform sampler2D Texture01;
	varying vec2 TexCoord;
	void main()
	{//多纹理混合的shader，尚未实现
		vec2 size = textureSize(Texture00, 0);
		vec4 colorG = texture2D(Texture00, TexCoord).rgba;
		float a = 0.0;
		a += 1.0 - texture2D(Texture01, TexCoord).r;
		a += 1.0 - texture2D(Texture01, TexCoord + vec2(1.0 / size.x, 0.0)).r;
		a += 1.0 - texture2D(Texture01, TexCoord + vec2(-1.0 / size.x, 0.0)).r;
		a += 1.0 - texture2D(Texture01, TexCoord + vec2(0.0, 1.0 / size.y)).r;
		a += 1.0 - texture2D(Texture01, TexCoord + vec2(0.0, -1.0 / size.x)).r;
		a += 1.0 - texture2D(Texture01, TexCoord + vec2(2.0 / size.x, 0.0)).r;
		a += 1.0 - texture2D(Texture01, TexCoord + vec2(-2.0 / size.x, 0.0)).r;
		a += 1.0 - texture2D(Texture01, TexCoord + vec2(0.0, 2.0 / size.y)).r;
		a += 1.0 - texture2D(Texture01, TexCoord + vec2(0.0, -2.0 / size.x)).r;
		a = a / 9.0;
		gl_FragColor = vec4(colorG.r * a, colorG.g * a, colorG.b * a, 1.0);
	});
	//m_maskShader.initFromStr(tmpVrt, tmpFrg);
	m_maskShader.initFromStr(XFX::gCommonVrt.c_str(), tmpFrg);
	m_maskShader.connectTexture("Texture00", NULL);
	m_maskShader.connectTexture("Texture01", &m_maskTexID);
	m_maskTexID = m_totallFbo.getTexture(1);

	m_isInietd = true;
	return true;
}
void XMaskCorrection::begin(bool withMask)
{
	if (!m_isInietd) return;
	XGL::EnableBlend();
	m_maskCorrection.begin();
	XEG.clearScreen(XFColor::white);
	if(withMask)
		XRender::drawBlankPlane(XEG.getSceneSize(), m_maskTex.getTexGLID());
	m_maskCorrection.end();

	m_totallFbo.useFBO();
	m_totallFbo.attachTex(0);
}
void XMaskCorrection::end()
{
	if (!m_isInietd) return;
	m_totallFbo.attachTex(1);
	XEG.clearScreen(XFColor::white);
	m_maskCorrection.draw(false);
	m_totallFbo.removeFBO();
}
void XMaskCorrection::draw()
{
	if (!m_isInietd) return;
	XRender::drawBlankPlane(XEG.getSceneSize(), m_totallFbo.getTexture(0), &m_maskShader);
}
}