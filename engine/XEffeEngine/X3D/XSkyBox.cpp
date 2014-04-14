//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XSkyBox.h"

int _XSkyBox::init(const char *filename,_XResourcePosition resourcePosition)
{
	if(m_isInited != 0) return 1;
	if(filename == NULL) return 0;
	char tempFilename[256];
	strcpy(tempFilename,filename);
	char fileType[10];
	strcpy(fileType,filename + strlen(filename) - 4);
	//下面载入图片资源
	sprintf(tempFilename + strlen(filename) - 7,"top%s",fileType);
	if(m_texture[0].load(tempFilename,resourcePosition) == 0) return 0;
	sprintf(tempFilename + strlen(filename) - 7,"bottom%s",fileType);
	if(m_texture[1].load(tempFilename,resourcePosition) == 0) return 0;
	sprintf(tempFilename + strlen(filename) - 7,"left%s",fileType);
	if(m_texture[2].load(tempFilename,resourcePosition) == 0) return 0;
	sprintf(tempFilename + strlen(filename) - 7,"right%s",fileType);
	if(m_texture[3].load(tempFilename,resourcePosition) == 0) return 0;
	sprintf(tempFilename + strlen(filename) - 7,"front%s",fileType);
	if(m_texture[4].load(tempFilename,resourcePosition) == 0) return 0;
	sprintf(tempFilename + strlen(filename) - 7,"back%s",fileType);
	if(m_texture[5].load(tempFilename,resourcePosition) == 0) return 0;
	m_isInited = 1;
	return 1;
}
void _XSkyBox::draw()
{
	if(m_isInited == 0) return;
	GLboolean lp;
	glGetBooleanv(GL_LIGHTING,&lp);	//记录原来灯光的设置

	glPushAttrib(GL_ALL_ATTRIB_BITS);	//GL_ALL_ATTRIB_BITS GL_ENABLE_BIT
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
//	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(XFalse);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	//bottom
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D, m_texture[1].texture.m_texture);
	glBegin(GL_QUADS);	
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_left,m_bottom,m_front);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_right,m_bottom,m_front);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_right,m_bottom,m_back);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_left,m_bottom,m_back); 
	glEnd();
	//top
	glBindTexture(GL_TEXTURE_2D, m_texture[0].texture.m_texture);
	glBegin(GL_QUADS);		
		glNormal3f(0.0f,-1.0f,0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_left,m_top,m_front);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_left,m_top,m_back); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_right,m_top,m_back);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_right,m_top,m_front);		
	glEnd();
	//left
	glBindTexture(GL_TEXTURE_2D, m_texture[3].texture.m_texture);
	glBegin(GL_QUADS);
		glNormal3f(1.0f,0.0f,0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_left,m_bottom,m_front);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_left,m_bottom,m_back);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_left,m_top,m_back);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_left,m_top,m_front);
	glEnd();
	//right
	glBindTexture(GL_TEXTURE_2D, m_texture[2].texture.m_texture);
	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_right,m_bottom,m_front);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_right,m_top,m_front); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_right,m_top,m_back);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_right,m_bottom,m_back);		
	glEnd();
	//front
	glBindTexture(GL_TEXTURE_2D, m_texture[4].texture.m_texture);
	glBegin(GL_QUADS);		
		glNormal3f(0.0f,0.0f,-1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_left,m_bottom,m_front);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_left,m_top,m_front);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_right,m_top,m_front); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_right,m_bottom,m_front);
	glEnd();
	//back
	glBindTexture(GL_TEXTURE_2D, m_texture[5].texture.m_texture);
	glBegin(GL_QUADS);	
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_right,m_bottom,m_back);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_right,m_top,m_back); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_left,m_top,m_back);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_left,m_bottom,m_back);		
	glEnd();

	glPopMatrix();
	glPopAttrib();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDepthMask(XTrue);
	glClear(GL_DEPTH_BUFFER_BIT);

	if(lp) glEnable(GL_LIGHTING);	//恢复光照状态  
	glDisable(GL_TEXTURE_2D);
}