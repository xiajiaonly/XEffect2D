//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XShadowMap.h"
#include "XEffeEngine.h"
#include "X3DWorld.h"

_XBool _XShadowMap::init()
{
	if(m_isInited) return XFalse;
	if(!m_fbo.init(SHADOW_MAP_TEXTURE_SIZE,SHADOW_MAP_TEXTURE_SIZE,
		TEXTURE_DEEPTH)) return XFalse;
	m_isInited = XTrue;
	return XTrue;
}
void _XShadowMap::updateShadowMap()
{
	if(!m_isInited) return;
	if(_X3DWorld::GetInstance().m_drawFun == NULL) return;

	m_fbo.bind();
	m_fbo.attachTex();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixf(_X3DWorld::GetInstance().m_worldLight.getProjectMatrix());
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(_X3DWorld::GetInstance().m_worldLight.getViewMatrix());

	glViewport(0,0,SHADOW_MAP_TEXTURE_SIZE,SHADOW_MAP_TEXTURE_SIZE);
	glCullFace(GL_FRONT);
	glShadeModel(GL_FLAT);
	glColorMask(0,0,0,0);
	//这里描绘场景
	_X3DWorld::GetInstance().m_drawFun(XFalse);
	//回复参数
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glColorMask(1,1,1,1);
	m_fbo.unbind();
}
void _XShadowMap::release()
{
	if(!m_isInited) return;
	m_fbo.release();
	m_isInited = XFalse;
}