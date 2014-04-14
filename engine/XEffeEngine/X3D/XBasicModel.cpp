//++++++++++++++++++++++++++++++++
//Author:	¼ÖÊ¤»ª(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBasicModel.h"

void _XBasicModel::draw()
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
	glScalef(m_size.x,m_size.y,m_size.z);

//	_X3DWorld::GetInstance().m_worldMaterial.usetMaterial();
//	_X3DWorld::GetInstance().useShadow(false,SHADER_SHADOW);

	m_vbo.use(false);
	glDrawElements(GL_TRIANGLES,m_vbo.getIndexSize() * 3,GL_UNSIGNED_INT,0);
	m_vbo.disuse();

	glPopMatrix();
	glPopAttrib();
}