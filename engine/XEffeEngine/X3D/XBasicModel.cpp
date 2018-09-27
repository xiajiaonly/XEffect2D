#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBasicModel.h"
namespace XE{
void XBasicModel::draw()
{
	if(!m_isInited) return;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_CULL_FACE);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(m_position.x,m_position.y,m_position.z);
    glRotatef(m_angle.x,1,0,0);
    glRotatef(m_angle.y,0,1,0);
    glRotatef(m_angle.z,0,0,1);
	glScalef(m_scale.x,m_scale.y,m_scale.z);

//	X3DWld.m_worldMaterial.usetMaterial();
//	X3DWld.useShadow(false,SHADER_SHADOW);

	m_vbo.drawByIndex(GL_TRIANGLES,XFalse);

	glPopMatrix();
	glPopAttrib();
}
}