#include "XStdHead.h"
#include "XRippleRender.h"

namespace XE
{
std::string rippleRenderVs = std::string("#version 130\n") + STRINGIFY(varying vec2 texcoord;
void main()
{
	gl_Position = ftransform();
	vec4 Position = gl_Vertex;//gl_Position * view_proj_matrix;
	texcoord = gl_MultiTexCoord0.st;
});

std::string rippleRenderFs = std::string("#version 130\n") + STRINGIFY(uniform sampler2D tex0;		//起伏贴图
uniform sampler2D tex1;		//地贴图
uniform samplerCube envMap;
varying vec2 texcoord;

void main()
{
	vec4 s = texture2D(tex0, texcoord) - 0.5;
	vec3 normal = vec3(s.z, 0.04, s.w);
	normal = normalize(normal);
	vec3 tmpVec = normalize(vec3(0.0,1.0,0.0));
	vec3 reflVec = reflect(tmpVec, normal);		//反射
	vec4 refl = textureCube(envMap, reflVec) * 0.9;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//下面使用2D折射替换3D折射
	//方案1
	//float tmpSize = 0.75f;
	//vec3 refrVec = refract(tmpVec, normal, 0.85);
	//vec2 tmp = vec2(refrVec.x/refrVec.y + 0.5 / tmpSize,refrVec.z/refrVec.y + 0.5 / tmpSize) * tmpSize;
	//方案2
	//最后的参数为折射的强度，建议值为0.5
	vec2 tmp = vec2(reflVec.x / reflVec.y, reflVec.z / reflVec.y) * 0.03125;
	//-----------------------------------------------------------
	vec4 refr = texture2D(tex1, tmp + texcoord);// texcoord);	//折射

	float rate = sqrt(normal.x * normal.x + normal.z * normal.z) * 8.0f;
	vec4 tmpC = refr * (1.0 + refl.r * rate);
	tmpC.a = 1.0;

	gl_FragColor = tmpC;
});
bool XRippleRender::init(const char* cubeFilename, XResPos resPos)
{
	if (m_isInited || cubeFilename == nullptr ||
		!m_cubeMap.init(cubeFilename, resPos)) return false;
	if (!m_rendShader.initFromStr(rippleRenderVs.c_str(), rippleRenderFs.c_str())) return false;
	m_rendShader.connectTexture("tex0", NULL);
	m_rendShader.connectTexture("tex1", &m_colorTex);
	m_cubeTex = m_cubeMap.getTexture();
	m_rendShader.connectTexture("envMap", &m_cubeTex, TEX_TYPE_CUBE);

	m_isInited = true;
	return true;
}
void XRippleRender::draw(unsigned int rTex, unsigned int tex,int w,int h)
{
	if (!m_isInited) return;
	m_colorTex = tex;
	XGL::setBlendAlpha();
	XRender::drawBlankPlane(w, h, rTex, &m_rendShader);
	XGL::DisableBlend();
}
}