#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XEchoParticles.h"
namespace XE{
XEchoParticles::XEchoParticles()
:m_isInited(0)
#ifdef WITH_DLIST
,m_glListOrder(0)
#endif
,m_isEnd(1)
,m_isSetEnd(0)
,m_particles(NULL)
,m_haveGetmemory(0)
,m_blendTypeScr(4)
,m_blendTypeDst(5)
,m_blendType(XGL::BLEND_TWO_DATA)
{
	m_parentParticle.m_isEnable = 0;
}
int XEchoParticles::init(const XTexture *texture,int insertSum,int density,const XVector2& position,float dSize,float dAlpha,int atomSum,float initSize)
{
	if(m_isInited != 0) return 0;
	if(insertSum < 0) insertSum = 0;
	m_insertSum = insertSum;
	if(density < 1) density = 1;
	m_density = density;
	m_curPoint = 0;
//	m_parentParticle.m_texture = tex;
	m_parentParticle.m_initAngle = 0;
	m_parentParticle.m_initColor.setColor(1.0f,1.0f,1.0f,1.0f); //(1.0f,1.0f,1.0f,0.5f); 
	m_parentParticle.m_initPosition = position;
	m_parentParticle.m_initSize.set(initSize,initSize);
	m_parentParticle.m_dSize.x = dSize;
	m_parentParticle.m_dColor.setA(dAlpha);
	m_parentParticle.m_isEnable = 1;
	m_parentParticle.m_curPosition = position;

	if(texture == NULL || !glIsTexture(texture->m_texture)) return 0;
	m_texture = texture;
	w = m_texture->m_w;
	h = m_texture->m_h;
	halfW = (m_texture->m_w) >> 1;
	halfH = (m_texture->m_h) >> 1;
#ifdef WITH_DLIST
	if(m_glListOrder == 0) 
	{
		m_glListOrder = glGenLists(1); 
		if(m_glListOrder == 0) return 0;
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
	glEndList();
#endif
	
	if(m_haveGetmemory == 0)
	{//第一次进入的时候分配内存空间
		if(atomSum > 0)
		{//需要分配内存空间
			if(getMemory(atomSum) == 0) 
			{
#ifdef WITH_DLIST
				glDeleteLists(m_glListOrder,1);
#endif
				return 0;
			}
		}else
		{
			if(getMemory(m_maxEchoParticlesSum) == 0)
			{
#ifdef WITH_DLIST
				glDeleteLists(m_glListOrder,1);
#endif
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
void XEchoParticles::move(float timeDelay)
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return ;
	int flag = 0;
	int temp;
	XVector2 tmpSize(pow(m_parentParticle.m_dSize.x,(timeDelay * 0.05f)),pow(m_parentParticle.m_dSize.y,(timeDelay * 0.05f)));
	//float tempSizeX = pow(m_parentParticle.m_dSize.x,(timeDelay * 0.05f));
	//float tempSizeY = pow(m_parentParticle.m_dSize.y,(timeDelay * 0.05f));
	float tempAlpha = m_parentParticle.m_dColor.fA * timeDelay;
	for(int i = 0;i < m_atomSum;++ i)
	{
		temp = m_curPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable == 0) break;
		if(flag == 0) flag = 1;
		//尺寸变化
		//m_particles[temp].m_curSize.x *= tempSizeX;
		//m_particles[temp].m_curSize.y *= tempSizeY;
		m_particles[temp].m_curSize *= tmpSize;

	//	m_particles[temp].m_curColor.setColor(m_parentParticle.m_initColor.fR,
	//		m_parentParticle.m_initColor.fG,
	//		m_parentParticle.m_initColor.fB,
	//		m_particles[temp].m_curColor.fA);
		//透明度变化
		m_particles[temp].m_curColor.fA -= tempAlpha;
		if(m_particles[temp].m_curColor.getA() <= 0.0f)
		{
			m_particles[temp].m_curColor.setA(0.0f);
			m_particles[temp].m_isEnable = 0;
		}
	}
	if(flag == 0 && m_isSetEnd != 0) m_isEnd = 1;
}
void XEchoParticles::moveEx(float sizeX,float sizeY,float alpha)
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return ;
	int flag = 0;
	int temp;
	for(int i = 0;i < m_atomSum;++ i)
	{
		temp = m_curPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable == 0) break;

		if(flag == 0) flag = 1;
		XBasicParticle &tmp = m_particles[temp];
		//尺寸变化
		tmp.m_curSize.x *= sizeX;
		tmp.m_curSize.y *= sizeY;
		//透明度变化
		tmp.m_curColor.fA -= alpha;
		if(tmp.m_curColor.getA() <= 0.0f)
		{
			tmp.m_curColor.setA(0.0f);
			tmp.m_isEnable = 0;
		}
	}
	if(flag == 0 && m_isSetEnd != 0) m_isEnd = 1;
}
void XEchoParticles::draw() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return ;
	int temp;

	XGL::EnableTexture2D();	
	XGL::EnableBlend();
	if(m_blendType == XGL::BLEND_TWO_DATA) XGL::SetBlendFunc(XGL::srcBlendMode[m_blendTypeScr],XGL::dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(XGL::srcBlendMode[m_blendRGBScr],XGL::dstBlendMode[m_blendRGBDst],XGL::srcBlendMode[m_blendAScr],XGL::dstBlendMode[m_blendADst]);
	XGL::BindTexture2D(m_texture->m_texture);
	//下面是两种画图方式，可以根据需要选择
/*	for(int i = 0;i < m_atomSum;++ i)
	{
		temp = m_curPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glTranslatef(m_particles[temp].m_curPosition.x + halfW, m_particles[temp].m_curPosition.y + halfH, 0);

			glColor4fv(m_particles[temp].m_curColor);
			glRotatef(m_particles[temp].m_curAngle, 0, 0, 1);
			glScalef(m_particles[temp].m_curSize.x, m_particles[temp].m_curSize.x, 0);

			glCallList(m_glListOrder);																
		}else
		{
			break;
		}
	}*/
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	for(int i = m_atomSum - 1;i >= 0;-- i)
	{
		temp = m_curPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			glLoadIdentity();

			glTranslatef(m_particles[temp].m_curPosition.x + halfW, m_particles[temp].m_curPosition.y + halfH, 0);

			glColor4fv(m_particles[temp].m_curColor);
			glRotatef(m_particles[temp].m_curAngle, 0.0f, 0.0f, 1.0f);
			glScalef(m_particles[temp].m_curSize.x, m_particles[temp].m_curSize.x, 0.0f);

#ifdef WITH_DLIST
			glCallList(m_glListOrder);	
#else
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
#endif
			//glMatrixMode(GL_MODELVIEW);
		}
	}
	glPopMatrix();

	XGL::DisableBlend();
}
void XEchoParticles::drawAsLine() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return ;
	int temp;

	XGL::EnableTexture2D();	
	XGL::EnableBlend();
	if(m_blendType == XGL::BLEND_TWO_DATA) XGL::SetBlendFunc(XGL::srcBlendMode[m_blendTypeScr],XGL::dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(XGL::srcBlendMode[m_blendRGBScr],XGL::dstBlendMode[m_blendRGBDst],
		XGL::srcBlendMode[m_blendAScr],XGL::dstBlendMode[m_blendADst]);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	for(int i = m_atomSum - 1;i >= 0;-- i)
	{
		temp = m_curPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			glColor4fv(m_particles[temp].m_curColor);
			glVertex2fv(m_particles[temp].m_curPosition);
		}
	}
	glEnd();
}
void XEchoParticles::drawExBegin() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;

	XGL::EnableTexture2D();		
	XGL::EnableBlend();
	if(m_blendType == XGL::BLEND_TWO_DATA) XGL::SetBlendFunc(XGL::srcBlendMode[m_blendTypeScr],XGL::dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(XGL::srcBlendMode[m_blendRGBScr],XGL::dstBlendMode[m_blendRGBDst],
		XGL::srcBlendMode[m_blendAScr],XGL::dstBlendMode[m_blendADst]);
	XGL::BindTexture2D(m_texture->m_texture);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}
void XEchoParticles::drawExBeginEx() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;

	XGL::EnableTexture2D();	
	XGL::EnableBlend();
	if(m_blendType == XGL::BLEND_TWO_DATA) XGL::SetBlendFunc(XGL::srcBlendMode[m_blendTypeScr],XGL::dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(XGL::srcBlendMode[m_blendRGBScr],XGL::dstBlendMode[m_blendRGBDst],
		XGL::srcBlendMode[m_blendAScr],XGL::dstBlendMode[m_blendADst]);
	XGL::BindTexture2D(m_texture->m_texture);
	glBegin(GL_QUADS);
}
void XEchoParticles::drawExBody() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	int temp;
	for(int i = m_atomSum - 1;i >= 0;-- i)
	{
		temp = m_curPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			XBasicParticle &tmp = m_particles[temp];
			
			glLoadIdentity();

			glTranslatef(tmp.m_curPosition.x + halfW, tmp.m_curPosition.y + halfH, 0);

			glColor4fv(tmp.m_curColor);
//暂时优化掉这个旋转
//			glRotatef(tmp.m_curAngle, 0.0f, 0.0f, 1.0f);
			glScalef(tmp.m_curSize.x, tmp.m_curSize.x, 0.0f);

#ifdef WITH_DLIST
			glCallList(m_glListOrder);	
#else
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
#endif
			//glMatrixMode(GL_MODELVIEW);
		}
	}
}
void XEchoParticles::getDrawData(std::vector<float> &v,std::vector<float> &t,std::vector<float> &c) const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	int temp;
	float w,h;
	for(int i = m_atomSum - 1;i >= 0;-- i)
	{
		temp = m_curPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			XBasicParticle &tmp = m_particles[temp];
			t.push_back(0.0f);t.push_back(0.0f);
			t.push_back(1.0f);t.push_back(0.0f);
			t.push_back(1.0f);t.push_back(1.0f);
			t.push_back(0.0f);t.push_back(1.0f);

			c.push_back(tmp.m_curColor.fR);
			c.push_back(tmp.m_curColor.fG);
			c.push_back(tmp.m_curColor.fB);
			c.push_back(tmp.m_curColor.fA);
			c.push_back(tmp.m_curColor.fR);
			c.push_back(tmp.m_curColor.fG);
			c.push_back(tmp.m_curColor.fB);
			c.push_back(tmp.m_curColor.fA);
			c.push_back(tmp.m_curColor.fR);
			c.push_back(tmp.m_curColor.fG);
			c.push_back(tmp.m_curColor.fB);
			c.push_back(tmp.m_curColor.fA);
			c.push_back(tmp.m_curColor.fR);
			c.push_back(tmp.m_curColor.fG);
			c.push_back(tmp.m_curColor.fB);
			c.push_back(tmp.m_curColor.fA);

			w = halfW * tmp.m_curSize.x;
			h = halfH * tmp.m_curSize.x;
			v.push_back(tmp.m_curPosition.x + halfW - w);
			v.push_back(tmp.m_curPosition.y + halfH - h);
			v.push_back(tmp.m_curPosition.x + halfW + w);
			v.push_back(tmp.m_curPosition.y + halfH - h);
			v.push_back(tmp.m_curPosition.x + halfW + w);
			v.push_back(tmp.m_curPosition.y + halfH + h);
			v.push_back(tmp.m_curPosition.x + halfW - w);
			v.push_back(tmp.m_curPosition.y + halfH + h);
		}
	}
}
void XEchoParticles::drawExBodyEx() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	int temp;
	int tmpW = w,tmpWO = 0;
	for(int i = m_atomSum - 1;i >= 0;-- i)
	{
		temp = m_curPoint - i - 1;
		if(temp < 0) temp += m_atomSum;
		if(m_particles[temp].m_isEnable != 0)
		{
			XBasicParticle &tmp = m_particles[temp];

			glColor4fv(tmp.m_curColor);
		//	tmpW = w * tmp.m_curSize.x;
		//	tmpWO = (w - tmpW) >> 1;
		//	tmpW += tmpWO;

			glTexCoord2f(0, 0);
			glVertex2f(tmp.m_curPosition.x + tmpWO,tmp.m_curPosition.y + tmpWO);
			glTexCoord2f(1, 0);
			glVertex2f(tmp.m_curPosition.x + tmpW, tmp.m_curPosition.y + tmpWO);
			glTexCoord2f(1, 1);
			glVertex2f(tmp.m_curPosition.x + tmpW, tmp.m_curPosition.y + tmpW);
			glTexCoord2f(0, 1);
			glVertex2f(tmp.m_curPosition.x + tmpWO, tmp.m_curPosition.y + tmpW);
		}
	}
}
void XEchoParticles::setPosition(float x,float y)
{
	if(m_isEnd != 0 ||
		m_isSetEnd != 0) return ;
	int length = m_parentParticle.m_curPosition.getLengthSqure(x,y);
	if(length < m_density * m_density) return;
	float angle = m_parentParticle.m_curPosition.getAngle(x,y) * RADIAN2DEGREE;
	length = XEE_Min(sqrtf(length) / m_density,m_insertSum);

	float tempX = (x - m_parentParticle.m_curPosition.x) / length;
	float tempY = (y - m_parentParticle.m_curPosition.y) / length;
	for(int i = 1;i < length + 1;++ i)
	{
		m_particles[m_curPoint].m_curSize.x = m_parentParticle.m_initSize.x;
		m_particles[m_curPoint].m_curColor = m_parentParticle.m_initColor;
		m_particles[m_curPoint].m_curAngle = angle;
		m_particles[m_curPoint].m_curPosition.set(m_parentParticle.m_curPosition.x + tempX * i,
			m_parentParticle.m_curPosition.y + tempY * i);
		m_particles[m_curPoint].m_isEnable = 1;
		++m_curPoint;
		if(m_curPoint >= m_atomSum) m_curPoint = 0;
	}
	m_parentParticle.m_curPosition.set(x,y);
}
void XEchoParticles::setPosition(float x,float y,float size)
{
	if(size <= 0.0f ||
		m_isEnd != 0 ||
		m_isSetEnd != 0) return ;
	int length = m_parentParticle.m_curPosition.getLengthSqure(x,y);
	if(length < m_density * m_density) return;
	float angle = m_parentParticle.m_curPosition.getAngle(x,y) * RADIAN2DEGREE;
	length = XEE_Min(sqrtf(length) / m_density,m_insertSum);
	float tempX = (x - m_parentParticle.m_curPosition.x) / length;
	float tempY = (y - m_parentParticle.m_curPosition.y) / length;
	for(int i = 1;i < length + 1;++ i)
	{
		m_particles[m_curPoint].m_curSize.x = m_parentParticle.m_initSize.x * size;
		m_particles[m_curPoint].m_curColor = m_parentParticle.m_initColor;
		m_particles[m_curPoint].m_curAngle = angle;
		m_particles[m_curPoint].m_curPosition.set(m_parentParticle.m_curPosition.x + tempX * i,
			m_parentParticle.m_curPosition.y + tempY * i);
		m_particles[m_curPoint].m_isEnable = 1;
		++m_curPoint;
		if(m_curPoint >= m_atomSum) m_curPoint = 0;
	}
	m_parentParticle.m_curPosition.set(x,y);
}
void XEchoParticles::setPosition(float x,float y,float size,float udR,float udG,float udB)//,float udA)
{
	if(size <= 0.0f ||
		m_isEnd != 0 ||
		m_isSetEnd != 0) return ;
	int length = m_parentParticle.m_curPosition.getLengthSqure(x,y);
	if(length < m_density * m_density) return;
	float angle = m_parentParticle.m_curPosition.getAngle(x,y) * RADIAN2DEGREE;
	length = XEE_Min(sqrtf(length) / m_density,m_insertSum);
	
	float tempX = (x - m_parentParticle.m_curPosition.x) / length;
	float tempY = (y - m_parentParticle.m_curPosition.y) / length;
	for(int i = 1;i < length + 1;++ i)
	{
		m_particles[m_curPoint].m_curSize.x = m_parentParticle.m_initSize.x * size;
		m_particles[m_curPoint].m_curColor = m_parentParticle.m_initColor;
		m_particles[m_curPoint].m_curColor.fR = udR;
		m_particles[m_curPoint].m_curColor.fG = udG;
		m_particles[m_curPoint].m_curColor.fB = udB;
		//m_particles[m_curPoint].m_curColor.fA = udA;
		m_particles[m_curPoint].m_curAngle = angle;
		m_particles[m_curPoint].m_curPosition.set(m_parentParticle.m_curPosition.x + tempX * i,
			m_parentParticle.m_curPosition.y + tempY * i);
		m_particles[m_curPoint].m_isEnable = 1;
		++m_curPoint;
		if(m_curPoint >= m_atomSum) m_curPoint = 0;
	}
	m_parentParticle.m_curPosition.set(x,y);
}
void XEchoParticles::release()
{
	if(m_isInited == 0) return;
#ifdef WITH_DLIST	
	if(m_glListOrder > 0) glDeleteLists(m_glListOrder,1);
#endif
	XMem::XDELETE_ARRAY(m_particles);
	m_haveGetmemory = 0;
	m_parentParticle.m_isEnable = 0;
	m_isInited = 0;		//防止release之后调用其它成员函数而造成错误
}
int XEchoParticles::getMemory(int atomSum)
{
	XMem::XDELETE_ARRAY(m_particles);
	m_atomSum = atomSum;
	m_particles = XMem::createArrayMem<XBasicParticle>(m_atomSum);
	if(m_particles == NULL) return 0;

	for(int i = 0;i < m_atomSum;++ i)
	{
		m_particles[i].m_isEnable = 0;
	}
	return 1;
}
}