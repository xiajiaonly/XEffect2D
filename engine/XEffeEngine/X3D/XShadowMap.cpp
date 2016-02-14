#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XShadowMap.h"
#include "X3DWorld.h"
#include "../XGameBasic.h"
namespace XE{
XBool XShadowMap::init()
{
	if(m_isInited) return XFalse;
	if(!m_fbo.init(SHADOW_MAP_TEXTURE_SIZE,SHADOW_MAP_TEXTURE_SIZE,
		COLOR_DEPTH)) return XFalse;
	m_isInited = XTrue;
	return XTrue;
}
void XShadowMap::updateShadowMap()
{
	if(!m_isInited) return;
	if(gFrameworkData.p3DWorld == NULL || gFrameworkData.pFrameWork == NULL) return;
	if(gFrameworkData.p3DWorld->m_drawFun == NULL && gFrameworkData.pFrameWork->m_pGame == NULL) return;

	m_fbo.bind();
	m_fbo.attachTex();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixf(gFrameworkData.p3DWorld->m_worldLight.getProjectMatrix());
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(gFrameworkData.p3DWorld->m_worldLight.getViewMatrix());

	glViewport(0,0,SHADOW_MAP_TEXTURE_SIZE,SHADOW_MAP_TEXTURE_SIZE);
	glCullFace(GL_FRONT);
	glShadeModel(GL_FLAT);
	glColorMask(0,0,0,0);
	//这里描绘场景
	if(gFrameworkData.p3DWorld->m_drawFun != NULL) gFrameworkData.p3DWorld->m_drawFun(XFalse);
	else if(gFrameworkData.pFrameWork->m_pGame != NULL) ((XGameBasic3D *)gFrameworkData.pFrameWork->m_pGame)->draw3D(XFalse);
	//回复参数
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glColorMask(1,1,1,1);
	m_fbo.unbind();
}
void XShadowMap::release()
{
	if(!m_isInited) return;
	m_fbo.release();
	m_isInited = XFalse;
}
}