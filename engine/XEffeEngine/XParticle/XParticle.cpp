//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "glew.h"
#include "XParticle.h"

_XParentParticle::_XParentParticle()
:m_stage(STAGE_SLEEP)
{}
int _XAloneParticles::init(const _XTexture *texture)
{
	if(m_isInited != 0) return 0;
	if(texture == NULL || !glIsTexture(texture->m_texture)) return 0;
	m_texture = texture;
	halfW = m_texture->m_w >> 1;
	halfH = m_texture->m_h >> 1;

	//初始化一些基本数值
	m_isEnable = 0;			

	m_initPosition.set(0,0);	
	m_initSize.set(1,1);		
	m_initAngle = 0;				
	m_initColor.setColor(1.0,1.0,1.0,1.0);	

	m_dPosition.set(0,0);	
	m_dSize.set(0,0);		
	m_dAngle = 0;			
	m_dColor.setColor(1.0,1.0,1.0,1.0);		

	m_nowPosition.set(0,0);	
	m_nowSize.set(1.0,1.0);		
	m_nowAngle = 0;		
	m_nowColor.setColor(1.0,1.0,1.0,1.0);	

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
//#if WITH_XSPRITE_EX
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//#endif
	glDisable(GL_BLEND);	
	glEndList();

	m_isInited = 1;
	return 1;
}
void _XAloneParticles::draw() const
{
	if(m_isInited == 0) return;
	if(m_stage == STAGE_SLEEP) return;
	glEnable(GL_TEXTURE_2D);		
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,m_texture->m_texture);
//#if WITH_XSPRITE_EX
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

	glTranslatef(m_nowPosition.x + halfW,m_nowPosition.y + halfH, 0);

	glScalef(m_nowSize.x,m_nowSize.x, 0);
	glColor4fv(m_nowColor);

	glCallList(m_glListOrder);																
}
_XAloneParticles::_XAloneParticles()
:m_isInited(0)
,m_glListOrder(0)
,m_texture(NULL)
{}
