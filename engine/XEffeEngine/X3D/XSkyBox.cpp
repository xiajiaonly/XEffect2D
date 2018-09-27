#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XSkyBox.h"
#include "XLight.h"
namespace XE{
XBool XSkyBox::init(const char *filename,XResPos resourcePosition)
{
	if(m_isInited) return XTrue;
	if(filename == NULL) return XFalse;
	char tempFilename[256];
	strcpy_s(tempFilename,256,filename);
	char fileType[10];
	strcpy_s(fileType,10,filename + strlen(filename) - 4);
	//下面载入图片资源
	sprintf_s(tempFilename + strlen(filename) - 7,256 - (strlen(filename) - 7),"top%s",fileType);
	if(!m_texture[0].load(tempFilename,resourcePosition)) return XFalse;
	sprintf_s(tempFilename + strlen(filename) - 7,256 - (strlen(filename) - 7),"bottom%s",fileType);
	if(!m_texture[1].load(tempFilename,resourcePosition)) return XFalse;
	sprintf_s(tempFilename + strlen(filename) - 7,256 - (strlen(filename) - 7),"left%s",fileType);
	if(!m_texture[2].load(tempFilename,resourcePosition)) return XFalse;
	sprintf_s(tempFilename + strlen(filename) - 7,256 - (strlen(filename) - 7),"right%s",fileType);
	if(!m_texture[3].load(tempFilename,resourcePosition)) return XFalse;
	sprintf_s(tempFilename + strlen(filename) - 7,256 - (strlen(filename) - 7),"front%s",fileType);
	if(!m_texture[4].load(tempFilename,resourcePosition)) return XFalse;
	sprintf_s(tempFilename + strlen(filename) - 7,256 - (strlen(filename) - 7),"back%s",fileType);
	if(!m_texture[5].load(tempFilename,resourcePosition)) return XFalse;
	m_isInited = XTrue;
	return XTrue;
}
void XSkyBox::draw()
{
	if(!m_isInited) return;
	GLboolean lp;
	glGetBooleanv(GL_LIGHTING,&lp);	//记录原来灯光的设置

	glPushAttrib(GL_ALL_ATTRIB_BITS);	//GL_ALL_ATTRIB_BITS GL_ENABLE_BIT
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
//	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDepthMask(XFalse);
	XGL::DisableBlend();
	XLight::disAllLight();
	//bottom
	glColor4fv(XFColor::white);
	glActiveTexture(GL_TEXTURE0);
	XGL::EnableTexture2D();
	XGL::BindTexture2D(m_texture[1].texture.m_texture);
	glBegin(GL_QUADS);	
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_left,m_bottom,m_front);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_right,m_bottom,m_front);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_right,m_bottom,m_back);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_left,m_bottom,m_back); 
	glEnd();
	//top
	XGL::BindTexture2D(m_texture[0].texture.m_texture);
	glBegin(GL_QUADS);		
		glNormal3f(0.0f,-1.0f,0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_left,m_top,m_front);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_left,m_top,m_back); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_right,m_top,m_back);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_right,m_top,m_front);		
	glEnd();
	//left
	XGL::BindTexture2D(m_texture[3].texture.m_texture);
	glBegin(GL_QUADS);
		glNormal3f(1.0f,0.0f,0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_left,m_bottom,m_front);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_left,m_bottom,m_back);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_left,m_top,m_back);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_left,m_top,m_front);
	glEnd();
	//right
	XGL::BindTexture2D(m_texture[2].texture.m_texture);
	glBegin(GL_QUADS);
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_right,m_bottom,m_front);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_right,m_top,m_front); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_right,m_top,m_back);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_right,m_bottom,m_back);		
	glEnd();
	//front
	XGL::BindTexture2D(m_texture[4].texture.m_texture);
	glBegin(GL_QUADS);		
		glNormal3f(0.0f,0.0f,-1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_left,m_bottom,m_front);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_left,m_top,m_front);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_right,m_top,m_front); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_right,m_bottom,m_front);
	glEnd();
	//back
	XGL::BindTexture2D(m_texture[5].texture.m_texture);
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
	XGL::EnableBlend();
	glDepthMask(XTrue);
	glClear(GL_DEPTH_BUFFER_BIT);

	if(lp)
	{
		glEnable(GL_LIGHTING);	//恢复光照状态  
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL_EXT, GL_SEPARATE_SPECULAR_COLOR_EXT);
	}
}
}