//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XEchoParticles.h"

_XEchoParticles::_XEchoParticles()
:m_glListOrder(-1)
,m_isInited(0)
,m_isEnd(1)
,m_isSetEnd(0)
,m_particles(NULL)
,m_haveGetmemory(0)
,m_blendTypeScr(4)
,m_blendTypeDst(5)
{
	m_parentParticle.m_isEnable = 0;
}

int _XEchoParticles::init(const _XTexture *texture,int insertSum,int density,const _XVector2& position,float dSize,float dAlpha,int atomSum,float initSize)
{
	if(m_isInited != 0) return 0;
	if(insertSum < 0) insertSum = 0;
	m_insertSum = insertSum;
	if(density < 1) density = 1;
	m_density = density;
	m_nowPoint = 0;
//	m_parentParticle.m_texture = tex;
	m_parentParticle.m_initAngle = 0;
	m_parentParticle.m_initColor.setColor(1.0f,1.0f,1.0f,1.0f); //(1.0f,1.0f,1.0f,0.5f); 
	m_parentParticle.m_initPosition = position;
	m_parentParticle.m_initSize.set(initSize,initSize);
	m_parentParticle.m_dSize.x = dSize;
	m_parentParticle.m_dColor.setA(dAlpha);
	m_parentParticle.m_isEnable = 1;
	m_parentParticle.m_nowPosition = position;

	if(texture == NULL || !glIsTexture(texture->m_texture)) return 0;
	m_texture = texture;
	halfW = (m_texture->m_w) >> 1;
	halfH = (m_texture->m_h) >> 1;
	if(m_glListOrder <= 0) 
	{
		m_glListOrder = glGenLists(1); 
		if(m_glListOrder < 0) return 0;
	}
	glNewList(m_glListOrder,GL_COMPILE);  
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex2d(-halfW, -halfH);
	glTexCoord2d(m_texture->m_w, 0);
	glVertex2d(halfW, -halfH);
	glTexCoord2d(m_texture->m_w, m_texture->m_h);
	glVertex2d(halfW, halfH);
	glTexCoord2d(0, m_texture->m_h);
	glVertex2d(-halfW, halfH);
	glEnd();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEndList();
	
	if(m_haveGetmemory == 0)
	{//第一次进入的时候分配内存空间
		if(atomSum > 0)
		{//需要分配内存空间
			if(getMemory(atomSum) == 0) 
			{
				glDeleteLists(m_glListOrder,1);
				return 0;
			}
		}else
		{
			if(getMemory(MAX_ECHO_PARTICLES_SUM) == 0)
			{
				glDeleteLists(m_glListOrder,1);
				return 0;
			}
		}
		m_haveGetmemory = 1;
	}

	m_isSetEnd = 0;
	m_isInited = 1;
	m_isEnd = 0;
	return 1;
}

void _XEchoParticles::move(int timeDelay)
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return ;
	int flag = 0;
	int temp;
	float tempSizeX = pow(m_parentParticle.m_dSize.x,(timeDelay * 0.05f));
	float tempSizeY = pow(m_parentParticle.m_dSize.y,(timeDelay * 0.05f));
	float tempAlpha = m_parentParticle.m_dColor.fA * timeDelay;
	for(int i = 0;i < m_atomSum;++ i)
	{
		temp = m_nowPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			if(flag == 0) flag = 1;
			//尺寸变化
			m_particles[temp].m_nowSize.x *= tempSizeX;
			m_particles[temp].m_nowSize.y *= tempSizeY;

		//	m_particles[temp].m_nowColor.setColor(m_parentParticle.m_initColor.fR,
		//		m_parentParticle.m_initColor.fG,
		//		m_parentParticle.m_initColor.fB,
		//		m_particles[temp].m_nowColor.fA);
			//透明度变化
			m_particles[temp].m_nowColor.fA -= tempAlpha;
			if(m_particles[temp].m_nowColor.getA() <= 0.0f)
			{
				m_particles[temp].m_nowColor.setA(0.0f);
				m_particles[temp].m_isEnable = 0;
			}
		}else
		{
			break;
		}
	}
	if(flag == 0 && m_isSetEnd != 0)
	{
		m_isEnd = 1;
	}
}

void _XEchoParticles::moveEx(float sizeX,float sizeY,float alpha)
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return ;
	int flag = 0;
	int temp;
	for(int i = 0;i < m_atomSum;++ i)
	{
		temp = m_nowPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			if(flag == 0) flag = 1;
			//尺寸变化
			m_particles[temp].m_nowSize.x *= sizeX;
			m_particles[temp].m_nowSize.y *= sizeY;

		//	m_particles[temp].m_nowColor.setColor(m_parentParticle.m_initColor.fR,
		//		m_parentParticle.m_initColor.fG,
		//		m_parentParticle.m_initColor.fB,
		//		m_particles[temp].m_nowColor.fA);
			//透明度变化
			m_particles[temp].m_nowColor.fA -= alpha;
			if(m_particles[temp].m_nowColor.getA() <= 0.0f)
			{
				m_particles[temp].m_nowColor.setA(0.0f);
				m_particles[temp].m_isEnable = 0;
			}
		}else
		{
			break;
		}
	}
	if(flag == 0 && m_isSetEnd != 0)
	{
		m_isEnd = 1;
	}
}

//unsigned int srcBlendModeEP[9] = {GL_ZERO,GL_ONE,GL_DST_COLOR,GL_ONE_MINUS_DST_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,
//		GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA,GL_SRC_ALPHA_SATURATE};
//unsigned int dstBlendModeEP[8] = {GL_ZERO,GL_ONE,GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,	
//		GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA};

void _XEchoParticles::draw() const
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return ;
	int temp;

	glEnable(GL_TEXTURE_2D);		
	glBlendFunc(srcBlendMode[m_blendTypeScr], dstBlendMode[m_blendTypeDst]);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,m_texture->m_texture);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,m_texture->m_w << 1, 0,m_texture->m_h << 1, -1, 1);

	//下面是两种画图方式，可以根据需要选择
/*	for(int i = 0;i < m_atomSum;++ i)
	{
		temp = m_nowPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glTranslatef(m_particles[temp].m_nowPosition.x + halfW, m_particles[temp].m_nowPosition.y + halfH, 0);

			glColor4f(m_particles[temp].m_nowColor.fR, m_particles[temp].m_nowColor.fG, m_particles[temp].m_nowColor.fB,m_particles[temp].m_nowColor.fA);
			glRotatef(m_particles[temp].m_nowAngle, 0, 0, 1);
			glScalef(m_particles[temp].m_nowSize.x, m_particles[temp].m_nowSize.x, 0);

			glCallList(m_glListOrder);																
		}else
		{
			break;
		}
	}*/
	for(int i = m_atomSum - 1;i >= 0;-- i)
	{
		temp = m_nowPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glTranslatef(m_particles[temp].m_nowPosition.x + halfW, m_particles[temp].m_nowPosition.y + halfH, 0);

			glColor4f(m_particles[temp].m_nowColor.fR, m_particles[temp].m_nowColor.fG, m_particles[temp].m_nowColor.fB,m_particles[temp].m_nowColor.fA);
			glRotatef(m_particles[temp].m_nowAngle, 0.0f, 0.0f, 1.0f);
			glScalef(m_particles[temp].m_nowSize.x, m_particles[temp].m_nowSize.x, 0.0f);

			//glCallList(m_glListOrder);	
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0);
			glVertex2d(-halfW, -halfH);
			glTexCoord2d(m_texture->m_w, 0);
			glVertex2d(halfW, -halfH);
			glTexCoord2d(m_texture->m_w, m_texture->m_h);
			glVertex2d(halfW, halfH);
			glTexCoord2d(0, m_texture->m_h);
			glVertex2d(-halfW, halfH);
			glEnd();
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
	}
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glDisable(GL_BLEND);
}

void _XEchoParticles::drawExBegin() const
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return;

	glEnable(GL_TEXTURE_2D);		
	glBlendFunc(srcBlendMode[m_blendTypeScr], dstBlendMode[m_blendTypeDst]);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,m_texture->m_texture);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,m_texture->m_w << 1, 0,m_texture->m_h << 1, -1, 1);
}

void _XEchoParticles::drawExBody() const
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return ;
	int temp;
	for(int i = m_atomSum - 1;i >= 0;-- i)
	{
		temp = m_nowPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glTranslatef(m_particles[temp].m_nowPosition.x + halfW, m_particles[temp].m_nowPosition.y + halfH, 0);

			glColor4f(m_particles[temp].m_nowColor.fR, m_particles[temp].m_nowColor.fG, m_particles[temp].m_nowColor.fB,m_particles[temp].m_nowColor.fA);
			glRotatef(m_particles[temp].m_nowAngle, 0.0f, 0.0f, 1.0f);
			glScalef(m_particles[temp].m_nowSize.x, m_particles[temp].m_nowSize.x, 0.0f);

			//glCallList(m_glListOrder);	
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0);
			glVertex2d(-halfW, -halfH);
			glTexCoord2d(m_texture->m_w, 0);
			glVertex2d(halfW, -halfH);
			glTexCoord2d(m_texture->m_w, m_texture->m_h);
			glVertex2d(halfW, halfH);
			glTexCoord2d(0, m_texture->m_h);
			glVertex2d(-halfW, halfH);
			glEnd();
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
	}
}

void _XEchoParticles::drawExEnd() const
{
	if(m_isInited == 0) return;
	if(m_isEnd != 0) return ;
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glDisable(GL_BLEND);
}

void _XEchoParticles::setPosition(float x,float y)
{
	if(m_isEnd != 0) return;
	if(m_isSetEnd != 0) return ;
	int length = m_parentParticle.m_nowPosition.getLengthSqure(x,y);
	if(length < m_density * m_density) return;
	float angle = m_parentParticle.m_nowPosition.getAngle(x,y) * RADIAN_TO_ANGLE;
	length = sqrtf(length) / m_density;
	if(length > m_insertSum)
	{//是用最大差值
		float tempX = (x - m_parentParticle.m_nowPosition.x) / (m_insertSum + 1);
		float tempY = (y - m_parentParticle.m_nowPosition.y) / (m_insertSum + 1);
		for(int i = 0;i < m_insertSum;++ i)
		{
			m_particles[m_nowPoint].m_nowSize.x = m_parentParticle.m_initSize.x;
			m_particles[m_nowPoint].m_nowColor = m_parentParticle.m_initColor;
			m_particles[m_nowPoint].m_nowAngle = angle;
			m_particles[m_nowPoint].m_nowPosition.set(m_parentParticle.m_nowPosition.x + tempX * i,
				m_parentParticle.m_nowPosition.y + tempY * i);
			m_particles[m_nowPoint].m_isEnable = 1;
			++m_nowPoint;
			if(m_nowPoint >= m_atomSum) m_nowPoint = 0;
		}
	}else
	{//是用密度
		float tempX = (x - m_parentParticle.m_nowPosition.x) / length;
		float tempY = (y - m_parentParticle.m_nowPosition.y) / length;
		for(int i = 0;i < length;++ i)
		{
			m_particles[m_nowPoint].m_nowSize.x = m_parentParticle.m_initSize.x;
			m_particles[m_nowPoint].m_nowColor = m_parentParticle.m_initColor;
			m_particles[m_nowPoint].m_nowAngle = angle;
			m_particles[m_nowPoint].m_nowPosition.set(m_parentParticle.m_nowPosition.x + tempX * i,
				m_parentParticle.m_nowPosition.y + tempY * i);
			m_particles[m_nowPoint].m_isEnable = 1;
			++m_nowPoint;
			if(m_nowPoint >= m_atomSum) m_nowPoint = 0;
		}
	}
	m_parentParticle.m_nowPosition.set(x,y);
}

void _XEchoParticles::setPosition(float x,float y,float size)
{
	if(size <= 0.0f) return;
	if(m_isEnd != 0) return;
	if(m_isSetEnd != 0) return ;
	int length = m_parentParticle.m_nowPosition.getLengthSqure(x,y);
	if(length < m_density * m_density) return;
	float angle = m_parentParticle.m_nowPosition.getAngle(x,y) * RADIAN_TO_ANGLE;
	length = sqrtf(length) / m_density;
	if(length > m_insertSum)
	{//是用最大差值
		float tempX = (x - m_parentParticle.m_nowPosition.x) / (m_insertSum + 1);
		float tempY = (y - m_parentParticle.m_nowPosition.y) / (m_insertSum + 1);
		for(int i = 0;i < m_insertSum;++ i)
		{
			m_particles[m_nowPoint].m_nowSize.x = m_parentParticle.m_initSize.x * size;
			m_particles[m_nowPoint].m_nowColor = m_parentParticle.m_initColor;
			m_particles[m_nowPoint].m_nowAngle = angle;
			m_particles[m_nowPoint].m_nowPosition.set(m_parentParticle.m_nowPosition.x + tempX * i,
				m_parentParticle.m_nowPosition.y + tempY * i);
			m_particles[m_nowPoint].m_isEnable = 1;
			++m_nowPoint;
			if(m_nowPoint >= m_atomSum) m_nowPoint = 0;
		}
	}else
	{//是用密度
		float tempX = (x - m_parentParticle.m_nowPosition.x) / length;
		float tempY = (y - m_parentParticle.m_nowPosition.y) / length;
		for(int i = 0;i < length;++ i)
		{
			m_particles[m_nowPoint].m_nowSize.x = m_parentParticle.m_initSize.x * size;
			m_particles[m_nowPoint].m_nowColor = m_parentParticle.m_initColor;
			m_particles[m_nowPoint].m_nowAngle = angle;
			m_particles[m_nowPoint].m_nowPosition.set(m_parentParticle.m_nowPosition.x + tempX * i,
				m_parentParticle.m_nowPosition.y + tempY * i);
			m_particles[m_nowPoint].m_isEnable = 1;
			++m_nowPoint;
			if(m_nowPoint >= m_atomSum) m_nowPoint = 0;
		}
	}
	m_parentParticle.m_nowPosition.set(x,y);
}

void _XEchoParticles::setPosition(float x,float y,float size,float udR,float udG,float udB)//,float udA)
{
	if(size <= 0.0f) return;
	if(m_isEnd != 0) return;
	if(m_isSetEnd != 0) return ;
	int length = m_parentParticle.m_nowPosition.getLengthSqure(x,y);
	if(length < m_density * m_density) return;
	float angle = m_parentParticle.m_nowPosition.getAngle(x,y) * RADIAN_TO_ANGLE;
	length = sqrtf(length) / m_density;
	if(length > m_insertSum)
	{//是用最大差值
		float tempX = (x - m_parentParticle.m_nowPosition.x) / (m_insertSum + 1);
		float tempY = (y - m_parentParticle.m_nowPosition.y) / (m_insertSum + 1);
		for(int i = 0;i < m_insertSum;++ i)
		{
			m_particles[m_nowPoint].m_nowSize.x = m_parentParticle.m_initSize.x * size;
			m_particles[m_nowPoint].m_nowColor = m_parentParticle.m_initColor;
			m_particles[m_nowPoint].m_nowColor.fR = udR;
			m_particles[m_nowPoint].m_nowColor.fG = udG;
			m_particles[m_nowPoint].m_nowColor.fB = udB;
			//m_particles[m_nowPoint].m_nowColor.fA = udA;
			m_particles[m_nowPoint].m_nowAngle = angle;
			m_particles[m_nowPoint].m_nowPosition.set(m_parentParticle.m_nowPosition.x + tempX * i,
				m_parentParticle.m_nowPosition.y + tempY * i);
			m_particles[m_nowPoint].m_isEnable = 1;
			++m_nowPoint;
			if(m_nowPoint >= m_atomSum) m_nowPoint = 0;
		}
	}else
	{//是用密度
		float tempX = (x - m_parentParticle.m_nowPosition.x) / length;
		float tempY = (y - m_parentParticle.m_nowPosition.y) / length;
		for(int i = 0;i < length;++ i)
		{
			m_particles[m_nowPoint].m_nowSize.x = m_parentParticle.m_initSize.x * size;
			m_particles[m_nowPoint].m_nowColor = m_parentParticle.m_initColor;
			m_particles[m_nowPoint].m_nowColor.fR = udR;
			m_particles[m_nowPoint].m_nowColor.fG = udG;
			m_particles[m_nowPoint].m_nowColor.fB = udB;
			//m_particles[m_nowPoint].m_nowColor.fA = udA;
			m_particles[m_nowPoint].m_nowAngle = angle;
			m_particles[m_nowPoint].m_nowPosition.set(m_parentParticle.m_nowPosition.x + tempX * i,
				m_parentParticle.m_nowPosition.y + tempY * i);
			m_particles[m_nowPoint].m_isEnable = 1;
			++m_nowPoint;
			if(m_nowPoint >= m_atomSum) m_nowPoint = 0;
		}
	}
	m_parentParticle.m_nowPosition.set(x,y);
}

void _XEchoParticles::release()
{
	if(m_isInited == 0) return;
	if(m_glListOrder >=0) glDeleteLists(m_glListOrder,1);
	XDELETE_ARRAY(m_particles);
	m_haveGetmemory = 0;
	m_parentParticle.m_isEnable = 0;
	m_isInited = 0;		//防止release之后调用其它成员函数而造成错误
}

int _XEchoParticles::getMemory(int atomSum)
{
	XDELETE_ARRAY(m_particles);
	m_atomSum = atomSum;
	m_particles = NULL;
	try
	{
		m_particles = new _XBasicParticle[m_atomSum];
		if(m_particles == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	for(int i = 0;i < m_atomSum;++ i)
	{
		m_particles[i].m_isEnable = 0;
	}
	return 1;
}