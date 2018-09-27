#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XParticle.h"
namespace XE{
XParentParticle::XParentParticle()
:m_stage(STAGE_SLEEP)
{}
int XAloneParticles::init(const XTexture *texture)
{
	if(m_isInited != 0) return 0;
	if(texture == NULL || !glIsTexture(texture->m_texture)) return 0;
	m_texture = texture;
	halfW = m_texture->m_w >> 1;
	halfH = m_texture->m_h >> 1;

	//初始化一些基本数值
	m_isEnable = false;			

	m_initPosition.set(0.0f);	
	m_initSize.set(1.0f);		
	m_initAngle = 0;				
	m_initColor.set(1.0f, 1.0f);

	m_dPosition.set(0.0f);
	m_dSize.set(0.0f);
	m_dAngle = 0;			
	m_dColor.set(1.0f, 1.0f);

	m_curPosition.set(0.0f);
	m_curSize.set(1.0f);		
	m_curAngle = 0;		
	m_curColor.set(1.0f, 1.0f);

	if(m_glListOrder <= 0)
	{
		m_glListOrder = glGenLists(1); 
		if(m_glListOrder < 0) return 0;
	}
	glNewList(m_glListOrder,GL_COMPILE);  
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(-halfW, -halfH);
	glTexCoord2f(1, 0);
	glVertex2f(halfW, -halfH);
	glTexCoord2f(1, 1);
	glVertex2f(halfW, halfH);
	glTexCoord2f(0, 1);
	glVertex2f(-halfW, halfH);
	glEnd();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
//	glMatrixMode(GL_TEXTURE);
//	glPopMatrix();
//#if WITHXSPRITE_EX
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//#endif
	XGL::DisableBlend();
	glEndList();

	m_isInited = 1;
	return 1;
}
void XAloneParticles::draw() const
{
	if(m_isInited == 0) return;
	if(m_stage == STAGE_SLEEP) return;
	XGL::EnableTexture2D();
	XGL::BindTexture2D(m_texture->m_texture);
	XGL::setBlendAlpha();
//#if WITHXSPRITE_EX
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
//#endif
//	glMatrixMode(GL_TEXTURE);
//	glPushMatrix();
//	glLoadIdentity();
//	glOrtho(0,m_texture->m_w << 1, 0,m_texture->m_h << 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_curPosition.x + halfW,m_curPosition.y + halfH, 0.0f);

	glScalef(m_curSize.x,m_curSize.x, 0.0f);
	glColor4fv(m_curColor);

	glCallList(m_glListOrder);																
}
XAloneParticles::XAloneParticles()
:m_isInited(0)
,m_glListOrder(0)
,m_texture(NULL)
{}
}