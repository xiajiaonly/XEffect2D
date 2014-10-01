//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "glew.h"
#include "XEchoParticles.h"

_XEchoParticles::_XEchoParticles()
:m_isInited(0)
//,m_glListOrder(-1)
,m_isEnd(1)
,m_isSetEnd(0)
,m_particles(NULL)
,m_haveGetmemory(0)
,m_blendTypeScr(4)
,m_blendTypeDst(5)
,m_blendType(BLEND_TWO_DATA)
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
	w = m_texture->m_w;
	h = m_texture->m_h;
	halfW = (m_texture->m_w) >> 1;
	halfH = (m_texture->m_h) >> 1;
	//if(m_glListOrder <= 0) 
	//{
	//	m_glListOrder = glGenLists(1); 
	//	if(m_glListOrder < 0) return 0;
	//}
	//glNewList(m_glListOrder,GL_COMPILE);  
	//glBegin(GL_QUADS);
	//glTexCoord2f(0, 0);
	//glVertex2f(-halfW, -halfH);
	//glTexCoord2f(m_texture->m_w, 0);
	//glVertex2f(halfW, -halfH);
	//glTexCoord2f(m_texture->m_w, m_texture->m_h);
	//glVertex2f(halfW, halfH);
	//glTexCoord2f(0, m_texture->m_h);
	//glVertex2f(-halfW, halfH);
	//glEnd();
	//glMatrixMode(GL_MODELVIEW);
	//glPopMatrix();
	//glEndList();
	
	if(m_haveGetmemory == 0)
	{//第一次进入的时候分配内存空间
		if(atomSum > 0)
		{//需要分配内存空间
			if(getMemory(atomSum) == 0) 
			{
			//	glDeleteLists(m_glListOrder,1);
				return 0;
			}
		}else
		{
			if(getMemory(MAX_ECHO_PARTICLES_SUM) == 0)
			{
			//	glDeleteLists(m_glListOrder,1);
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
	if(m_isInited == 0 ||
		m_isEnd != 0) return ;
	int flag = 0;
	int temp;
	float tempSizeX = pow(m_parentParticle.m_dSize.x,(timeDelay * 0.05f));
	float tempSizeY = pow(m_parentParticle.m_dSize.y,(timeDelay * 0.05f));
	float tempAlpha = m_parentParticle.m_dColor.fA * timeDelay;
	for(int i = 0;i < m_atomSum;++ i)
	{
		temp = m_nowPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable == 0) break;
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
	}
	if(flag == 0 && m_isSetEnd != 0) m_isEnd = 1;
}
void _XEchoParticles::moveEx(float sizeX,float sizeY,float alpha)
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return ;
	int flag = 0;
	int temp;
	for(int i = 0;i < m_atomSum;++ i)
	{
		temp = m_nowPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable == 0) break;

		if(flag == 0) flag = 1;
		_XBasicParticle &tmp = m_particles[temp];
		//尺寸变化
		tmp.m_nowSize.x *= sizeX;
		tmp.m_nowSize.y *= sizeY;
		//透明度变化
		tmp.m_nowColor.fA -= alpha;
		if(tmp.m_nowColor.getA() <= 0.0f)
		{
			tmp.m_nowColor.setA(0.0f);
			tmp.m_isEnable = 0;
		}
	}
	if(flag == 0 && m_isSetEnd != 0) m_isEnd = 1;
}
void _XEchoParticles::draw() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return ;
	int temp;

	glEnable(GL_TEXTURE_2D);		
	if(m_blendType == BLEND_TWO_DATA) glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(srcBlendMode[m_blendRGBScr],dstBlendMode[m_blendRGBDst],srcBlendMode[m_blendAScr],dstBlendMode[m_blendADst]);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,m_texture->m_texture);
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

			glColor4fv(m_particles[temp].m_nowColor);
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

			glColor4fv(m_particles[temp].m_nowColor);
			glRotatef(m_particles[temp].m_nowAngle, 0.0f, 0.0f, 1.0f);
			glScalef(m_particles[temp].m_nowSize.x, m_particles[temp].m_nowSize.x, 0.0f);

			//glCallList(m_glListOrder);	
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
			//glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
	}

	glDisable(GL_BLEND);
}
void _XEchoParticles::drawAsLine() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return ;
	int temp;

	glDisable(GL_TEXTURE_2D);		
	if(m_blendType == BLEND_TWO_DATA) glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(srcBlendMode[m_blendRGBScr],dstBlendMode[m_blendRGBDst],srcBlendMode[m_blendAScr],dstBlendMode[m_blendADst]);
	glEnable(GL_BLEND);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	for(int i = m_atomSum - 1;i >= 0;-- i)
	{
		temp = m_nowPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			glColor4fv(m_particles[temp].m_nowColor);
			glVertex2fv(m_particles[temp].m_nowPosition);
		}
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
}
void _XEchoParticles::drawExBegin() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;

	glEnable(GL_TEXTURE_2D);		
	if(m_blendType == BLEND_TWO_DATA) glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(srcBlendMode[m_blendRGBScr],dstBlendMode[m_blendRGBDst],srcBlendMode[m_blendAScr],dstBlendMode[m_blendADst]);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,m_texture->m_texture);
}
void _XEchoParticles::drawExBeginEx() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;

	glEnable(GL_TEXTURE_2D);		
	if(m_blendType == BLEND_TWO_DATA) glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(srcBlendMode[m_blendRGBScr],dstBlendMode[m_blendRGBDst],srcBlendMode[m_blendAScr],dstBlendMode[m_blendADst]);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,m_texture->m_texture);
	glBegin(GL_QUADS);
}
void _XEchoParticles::drawExBody() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	int temp;
	for(int i = m_atomSum - 1;i >= 0;-- i)
	{
		temp = m_nowPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			_XBasicParticle &tmp = m_particles[temp];
			
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glTranslatef(tmp.m_nowPosition.x + halfW, tmp.m_nowPosition.y + halfH, 0);

			glColor4fv(tmp.m_nowColor);
			glRotatef(tmp.m_nowAngle, 0.0f, 0.0f, 1.0f);
			glScalef(tmp.m_nowSize.x, tmp.m_nowSize.x, 0.0f);

			//glCallList(m_glListOrder);	
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
			//glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
	}
}
void _XEchoParticles::drawExBodyEx() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	int temp;
	int tmpW = w,tmpWO = 0;
	for(int i = m_atomSum - 1;i >= 0;-- i)
	{
		temp = m_nowPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			_XBasicParticle &tmp = m_particles[temp];

			glColor4fv(tmp.m_nowColor);
		//	tmpW = w * tmp.m_nowSize.x;
		//	tmpWO = (w - tmpW) >> 1;
		//	tmpW += tmpWO;

			glTexCoord2f(0, 0);
			glVertex2f(tmp.m_nowPosition.x + tmpWO,tmp.m_nowPosition.y + tmpWO);
			glTexCoord2f(1, 0);
			glVertex2f(tmp.m_nowPosition.x + tmpW, tmp.m_nowPosition.y + tmpWO);
			glTexCoord2f(1, 1);
			glVertex2f(tmp.m_nowPosition.x + tmpW, tmp.m_nowPosition.y + tmpW);
			glTexCoord2f(0, 1);
			glVertex2f(tmp.m_nowPosition.x + tmpWO, tmp.m_nowPosition.y + tmpW);
		}
	}
}
void _XEchoParticles::setPosition(float x,float y)
{
	if(m_isEnd != 0 ||
		m_isSetEnd != 0) return ;
	int length = m_parentParticle.m_nowPosition.getLengthSqure(x,y);
	if(length < m_density * m_density) return;
	float angle = m_parentParticle.m_nowPosition.getAngle(x,y) * RADIAN2DEGREE;
	length = XEE_Min(sqrtf(length) / m_density,m_insertSum);

	float tempX = (x - m_parentParticle.m_nowPosition.x) / length;
	float tempY = (y - m_parentParticle.m_nowPosition.y) / length;
	for(int i = 1;i < length + 1;++ i)
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
	m_parentParticle.m_nowPosition.set(x,y);
}
void _XEchoParticles::setPosition(float x,float y,float size)
{
	if(size <= 0.0f ||
		m_isEnd != 0 ||
		m_isSetEnd != 0) return ;
	int length = m_parentParticle.m_nowPosition.getLengthSqure(x,y);
	if(length < m_density * m_density) return;
	float angle = m_parentParticle.m_nowPosition.getAngle(x,y) * RADIAN2DEGREE;
	length = XEE_Min(sqrtf(length) / m_density,m_insertSum);
	float tempX = (x - m_parentParticle.m_nowPosition.x) / length;
	float tempY = (y - m_parentParticle.m_nowPosition.y) / length;
	for(int i = 1;i < length + 1;++ i)
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
	m_parentParticle.m_nowPosition.set(x,y);
}
void _XEchoParticles::setPosition(float x,float y,float size,float udR,float udG,float udB)//,float udA)
{
	if(size <= 0.0f ||
		m_isEnd != 0 ||
		m_isSetEnd != 0) return ;
	int length = m_parentParticle.m_nowPosition.getLengthSqure(x,y);
	if(length < m_density * m_density) return;
	float angle = m_parentParticle.m_nowPosition.getAngle(x,y) * RADIAN2DEGREE;
	length = XEE_Min(sqrtf(length) / m_density,m_insertSum);
	
	float tempX = (x - m_parentParticle.m_nowPosition.x) / length;
	float tempY = (y - m_parentParticle.m_nowPosition.y) / length;
	for(int i = 1;i < length + 1;++ i)
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
	m_parentParticle.m_nowPosition.set(x,y);
}
void _XEchoParticles::release()
{
	if(m_isInited == 0) return;
	//if(m_glListOrder >=0) glDeleteLists(m_glListOrder,1);
	XDELETE_ARRAY(m_particles);
	m_haveGetmemory = 0;
	m_parentParticle.m_isEnable = 0;
	m_isInited = 0;		//防止release之后调用其它成员函数而造成错误
}
int _XEchoParticles::getMemory(int atomSum)
{
	XDELETE_ARRAY(m_particles);
	m_atomSum = atomSum;
	m_particles = createArrayMem<_XBasicParticle>(m_atomSum);
	if(m_particles == NULL) return 0;

	for(int i = 0;i < m_atomSum;++ i)
	{
		m_particles[i].m_isEnable = 0;
	}
	return 1;
}