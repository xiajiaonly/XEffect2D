#include "XStdHead.h"
#include "XGPURipples.h"

namespace XE {
	std::string rippleVs = std::string("#version 130\n") + STRINGIFY(varying vec2 TexCoord;
	void main(void)
	{
		TexCoord = gl_MultiTexCoord0.st;
		gl_Position = ftransform();
	});

	std::string rippleFs = std::string("#version 130\n") + STRINGIFY(uniform sampler2D Texture_00;
	uniform sampler2D Texture_01;	//mask
	uniform sampler2D Texture_02;	//drop
	varying vec2 TexCoord;

	void main()
	{
		vec2 size = textureSize(Texture_00, 0);
		vec4 colorx = texture2D(Texture_01, TexCoord).rgba;
		if (colorx.r <= 0.5)
		{
			gl_FragColor = vec4(0.5, 0.5, 0.5, 0.5);
			return;
		}
		float offset = 0.5 / size.x;
		vec4 color0 = texture2D(Texture_00, TexCoord).rgba;
		vec4 color1 = texture2D(Texture_00, TexCoord + vec2(-offset, -offset)).rgba;
		vec4 color2 = texture2D(Texture_00, TexCoord + vec2(-offset, offset)).rgba;
		vec4 color3 = texture2D(Texture_00, TexCoord + vec2(offset, -offset)).rgba;
		vec4 color4 = texture2D(Texture_00, TexCoord + vec2(offset, offset)).rgba;
		float f = color1.r + color2.r + color3.r + color4.r - 4.0 * color0.r;
		color0 -= 0.5;
		color0.g += 0.1 * f;
		color0.r += 0.6 * color0.g;
		color0 *= 0.992;	//涟漪衰减的系数

		color0.b = 0.25 * (color1.r + color2.r - color3.r - color4.r);
		color0.a = 0.25 * (color1.r - color2.r + color3.r - color4.r);

		color0 += 0.5;

		//drop
		float colorD = texture2D(Texture_02, TexCoord).r;
		if (colorD <= 0.5) color0 = vec4(0.8, 0.5, 0.5, 0.5);

		gl_FragColor = color0;
	});

bool XGPURipples::init(int w, int h,const char* maskFilename, XResPos resPos)
{
	if (m_isInited || w <= 0 || h <= 0) return false;
	m_w = w;
	m_h = h;

	if (!m_fbo.init(m_w, m_h, COLOR_RGBA_F)) return false;
	m_fbo.addOneTexture(m_w, m_h, COLOR_RGBA_F);
	m_fbo.addOneTexture(m_w, m_h, COLOR_RGB);
	m_fbo.useFBO();
	m_fbo.attachTex(0);
	XEG.clearScreen(XFColor(0.5f, 0.5f, 0.5f, 0.5f));
	m_fbo.attachTex(1);
	XEG.clearScreen(XFColor(0.5f, 0.5f, 0.5f, 0.5f));
	m_fbo.attachTex(2);
	XEG.clearScreen(XFColor::white);
	m_fbo.removeFBO();

	if (!m_maskTex.load(maskFilename, resPos)) return false;
	m_maskTexID = m_maskTex.texture.m_texture;
	m_dropTexID = m_fbo.getTexture(2);

	if (!m_calShader.initFromStr(rippleVs.c_str(), rippleFs.c_str())) return false;
	m_calTex = m_fbo.getTexture(0);
	m_calShader.connectTexture("Texture_00", nullptr);
	m_calShader.connectTexture("Texture_01", &m_maskTexID);
	m_calShader.connectTexture("Texture_02", &m_dropTexID);

	m_isInited = true;
	return true;
}
void XGPURipples::update()
{
	if (!m_isInited) return;
	if (m_dropsPos.size() > 0)
	{//填入点的位置信息
		m_fbo.useFBO(true, m_w, m_h);
		m_fbo.attachTex(2);
		XEG.clearScreen(XFColor::white);
		for (int i = 0; i < m_dropsPos.size(); ++i)
		{
			XRender::drawPoints(m_dropsPos, 2.0, XFColor::black);
		}
		m_fbo.removeFBO();
	}

	if (m_texFlag)
	{
		XGL::DisableBlend();	//注意这里需要关闭混合才行，否则会造成问题
		m_fbo.useFBO(true, m_w, m_h);
		m_fbo.attachTex(0);
		XRender::drawBlankPlane(m_w, m_h, m_fbo.getTexture(1), &m_calShader);
		m_fbo.removeFBO();
		m_calTex = m_fbo.getTexture(0);
	}
	else
	{
		XGL::DisableBlend();	//注意这里需要关闭混合才行，否则会造成问题
		m_fbo.useFBO(true, m_w, m_h);
		m_fbo.attachTex(1);
		XRender::drawBlankPlane(m_w, m_h, m_fbo.getTexture(0), &m_calShader);
		m_fbo.removeFBO();
		m_calTex = m_fbo.getTexture(1);
	}
	m_texFlag = !m_texFlag;
	if (m_dropsPos.size() > 0)
	{//还原点的信息
		m_dropsPos.clear();
		m_fbo.useFBO(true, m_w, m_h);
		m_fbo.attachTex(2);
		XEG.clearScreen(XFColor::white);
		m_fbo.removeFBO();
	}
}
}