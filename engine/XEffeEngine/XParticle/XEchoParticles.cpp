#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XEchoParticles.h"
namespace XE{
XEchoParticles::XEchoParticles()
	:m_isInited(false)
#ifdef WITH_DLIST
	, m_glListOrder(0)
#endif
	, m_isEnd(true)
	, m_isSetEnd(false)
	, m_haveGetmemory(false)
	, m_particles(NULL)
//	, m_blendTypeScr(4)
//	, m_blendTypeDst(5)
//	, m_blendType(XGL::BLEND_TWO_DATA)
	, m_blendModel(XGL::BLEND_ALPHA)
{
	m_parentParticle.m_isEnable = false;
}
bool XEchoParticles::init(const XTexture *texture,int insertSum,int density,const XVec2& position,float dSize,float dAlpha,int atomSum,float initSize)
{
	if(m_isInited != 0) return false;
	if(insertSum < 0) m_insertSum = 0;
	else m_insertSum = insertSum;
	if(density < 1) m_density = 1;
	else m_density = density;
	m_curPoint = 0;
//	m_parentParticle.m_texture = tex;
	m_parentParticle.m_initAngle = 0;
	m_parentParticle.m_initColor.set(1.0f,1.0f); //(1.0f,1.0f,1.0f,0.5f); 
	m_parentParticle.m_initPosition = position;
	m_parentParticle.m_initSize.set(initSize);
	m_parentParticle.m_dSize.x = dSize;
	m_parentParticle.m_dColor.setA(dAlpha);
	m_parentParticle.m_isEnable = true;
	m_parentParticle.m_curPosition = position;

	if (texture == NULL || !glIsTexture(texture->m_texture)) return false;
	m_texture = texture;
	w = m_texture->m_w;
	h = m_texture->m_h;
	halfW = (m_texture->m_w) >> 1;
	halfH = (m_texture->m_h) >> 1;
#ifdef WITH_DLIST
	if(m_glListOrder == 0) 
	{
		m_glListOrder = glGenLists(1); 
		if(m_glListOrder == 0) return false;
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
	
	if (!m_haveGetmemory)
	{//第一次进入的时候分配内存空间
		if(atomSum > 0)
		{//需要分配内存空间
			if (!getMemory(atomSum))
			{
#ifdef WITH_DLIST
				glDeleteLists(m_glListOrder,1);
#endif
				return false;
			}
		}else
		{
			if (!getMemory(m_maxEchoParticlesSum))
			{
#ifdef WITH_DLIST
				glDeleteLists(m_glListOrder,1);
#endif
				return false;
			}
		}
		m_haveGetmemory = true;
	}

	m_isSetEnd = false;
	m_isInited = true;
	m_isEnd = false;
	return true;
}
void XEchoParticles::_moveEx(float sizeX,float alpha)
{
	int temp = m_curPoint - 1;
	if (temp < 0) temp += m_atomSum;
	if (!m_particles[temp].m_isEnable && m_isSetEnd)
	{
		m_isEnd = true;
		return;
	}
	for (int i = 0; i < m_atomSum; ++i, --temp)
	{
		if(temp < 0) temp += m_atomSum;
		if(!m_particles[temp].m_isEnable) break;
		XBasicParticle& ref = m_particles[temp];
		if(ref.m_curColor.a <= alpha)
		{
			ref.m_isEnable = false;
		}
		else
		{
			ref.m_curSize.x *= sizeX;		//尺寸变化
			ref.m_curColor.a -= alpha;		//透明度变化
		}
	}
}
void XEchoParticles::draw() const
{
	if (!m_isInited || m_isEnd) return;

	XGL::EnableTexture2D();	
	XGL::setBlendModel(m_blendModel);

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

			glTranslatef(m_particles[temp].m_curPosition.x + halfW, m_particles[temp].m_curPosition.y + halfH, 0.0f);

			glColor4fv(m_particles[temp].m_curColor);
			glRotatef(m_particles[temp].m_curAngle, 0.0f, 0.0f, 1.0f);
			glScalef(m_particles[temp].m_curSize.x, m_particles[temp].m_curSize.x, 0.0f);

			glCallList(m_glListOrder);																
		}else
		{
			break;
		}
	}*/
	int temp = m_curPoint - 1;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	for (int i = 0; i < m_atomSum; ++i, --temp)
	{
		if(temp < 0) temp += m_atomSum;
		if (!m_particles[temp].m_isEnable) break;
		XBasicParticle& ref = m_particles[temp];

		glLoadIdentity();
		glTranslatef(ref.m_curPosition.x + halfW, ref.m_curPosition.y + halfH, 0.0f);
		glColor4fv(ref.m_curColor);
		glRotatef(ref.m_curAngle, 0.0f, 0.0f, 1.0f);
		glScalef(ref.m_curSize.x, ref.m_curSize.x, 0.0f);

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
	glPopMatrix();

	XGL::DisableBlend();
}
void XEchoParticles::drawAsLine() const
{
	if (!m_isInited || m_isEnd) return;

	XGL::EnableTexture2D();	
	XGL::setBlendModel(m_blendModel);

	int temp = m_curPoint - 1;
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < m_atomSum; ++i, --temp)
	{
		if(temp < 0) temp += m_atomSum;
		if (!m_particles[temp].m_isEnable) break;
		glColor4fv(m_particles[temp].m_curColor);
		glVertex2fv(m_particles[temp].m_curPosition);
	}
	glEnd();
}
void XEchoParticles::drawExBegin() const
{
	if (!m_isInited || m_isEnd) return;

	XGL::EnableTexture2D();		
	XGL::setBlendModel(m_blendModel);

	XGL::BindTexture2D(m_texture->m_texture);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}
void XEchoParticles::drawExBeginEx() const
{
	if (!m_isInited || m_isEnd) return;

	XGL::EnableTexture2D();	
	XGL::setBlendModel(m_blendModel);

	XGL::BindTexture2D(m_texture->m_texture);
	glBegin(GL_QUADS);
}
void XEchoParticles::drawExBody() const
{
	if (!m_isInited || m_isEnd) return;
	int temp = m_curPoint - 1;
	for (int i = 0; i < m_atomSum; ++i, --temp)
	{
		if(temp < 0) temp += m_atomSum;
		if (!m_particles[temp].m_isEnable) break;
		XBasicParticle& ref = m_particles[temp];
			
		glLoadIdentity();
		glTranslatef(ref.m_curPosition.x + halfW, ref.m_curPosition.y + halfH, 0.0f);
		glColor4fv(ref.m_curColor);
//暂时优化掉这个旋转
//		glRotatef(tmp.m_curAngle, 0.0f, 0.0f, 1.0f);
		glScalef(ref.m_curSize.x, ref.m_curSize.x, 0.0f);

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
void XEchoParticles::getDrawData(std::vector<float> &v,std::vector<float> &t,std::vector<float> &c) const
{
	if (!m_isInited || m_isEnd) return;
	int temp,temp1 = m_curPoint - 1;
	float tw, th, x, y;
	for (int i = 0; i < m_atomSum; ++i, --temp1)
	{
		if(temp1 < 0) temp1 += m_atomSum;
		if (!m_particles[temp1].m_isEnable) break;
		XBasicParticle& ref = m_particles[temp1];
		//t的数据存在特殊性可以优化
		t.push_back(0.0f);t.push_back(0.0f);
		t.push_back(1.0f);t.push_back(0.0f);
		t.push_back(1.0f);t.push_back(1.0f);
		t.push_back(0.0f);t.push_back(1.0f);

		float *tmpF = ref.m_curColor;
		c.insert(c.end(), tmpF, tmpF + 4);
		c.insert(c.end(), tmpF, tmpF + 4);
		c.insert(c.end(), tmpF, tmpF + 4);
		c.insert(c.end(), tmpF, tmpF + 4);

		tw = halfW * ref.m_curSize.x;
		th = halfH * ref.m_curSize.x;
		x = ref.m_curPosition.x + halfW;
		y = ref.m_curPosition.y + halfH;
		temp = v.size();
		v.resize(temp + 8);
		v[temp++] = x - tw;
		v[temp++] = y - th;
		v[temp++] = x + tw;
		v[temp++] = y - th;
		v[temp++] = x + tw;
		v[temp++] = y + th;
		v[temp++] = x - tw;
		v[temp++] = y + th;
	}
}
void XEchoParticles::getDrawData(std::vector<float> &v,std::vector<float> &c)const	//这个不考虑旋转，使用vbo绘图
{
	if (!m_isInited || m_isEnd) return;
	int temp, temp1 = m_curPoint - 1;
	float tw,th,x,y;
	for (int i = 0; i < m_atomSum; ++i, --temp1)
	{
		if(temp1 < 0) temp1 += m_atomSum;
		if(!m_particles[temp1].m_isEnable) continue;
		XBasicParticle& ref = m_particles[temp1];
		float *tmpF = ref.m_curColor;
		c.insert(c.end(), tmpF, tmpF + 4);
		c.insert(c.end(), tmpF, tmpF + 4);
		c.insert(c.end(), tmpF, tmpF + 4);
		c.insert(c.end(), tmpF, tmpF + 4);

		tw = halfW * ref.m_curSize.x;
		th = halfH * ref.m_curSize.x;
		x = ref.m_curPosition.x + halfW;
		y = ref.m_curPosition.y + halfH;
		temp = v.size();
		v.resize(temp + 8);
		v[temp++] = x - tw;
		v[temp++] = y - th;
		v[temp++] = x + tw;
		v[temp++] = y - th;
		v[temp++] = x + tw;
		v[temp++] = y + th;
		v[temp++] = x - tw;
		v[temp++] = y + th;
	}
}
void XEchoParticles::getDrawData(float* v, float* c, int& sum) const
{
	if (!m_isInited || m_isEnd) return;
	int temp = m_curPoint - 1;
	float tw, th, x, y;
	for (int i = 0; i < m_atomSum; ++i, --temp)
	{
		if (temp < 0) temp += m_atomSum;
		if (!m_particles[temp].m_isEnable) continue;
		XBasicParticle& ref = m_particles[temp];

		memcpy(c, ref.m_curColor, 4 * sizeof(float)); c += 4;
		memcpy(c, ref.m_curColor, 4 * sizeof(float)); c += 4;
		memcpy(c, ref.m_curColor, 4 * sizeof(float)); c += 4;
		memcpy(c, ref.m_curColor, 4 * sizeof(float)); c += 4;

		tw = halfW * ref.m_curSize.x;
		th = halfH * ref.m_curSize.x;
		x = ref.m_curPosition.x + halfW;
		y = ref.m_curPosition.y + halfH;

		*(v++) = x - tw;
		*(v++) = y - th;
		*(v++) = x + tw;
		*(v++) = y - th;
		*(v++) = x + tw;
		*(v++) = y + th;
		*(v++) = x - tw;
		*(v++) = y + th;

		++sum;
	}
}
void XEchoParticles::drawExBodyEx() const
{
	if (!m_isInited || m_isEnd) return;
	int temp = m_curPoint - 1, tmpW = w;
	for (int i = 0; i < m_atomSum; ++i, --temp)
	{
		if (temp < 0) temp += m_atomSum;
		if (!m_particles[temp].m_isEnable) break;
		XBasicParticle& ref = m_particles[temp];

		tmpW = w * ref.m_curSize.x;
		glColor4fv(ref.m_curColor);

		glTexCoord2fv(XVec2::zero);
		glVertex2fv(ref.m_curPosition);
		glTexCoord2fv(XVec2::oneZero);
		glVertex2f(ref.m_curPosition.x + tmpW, ref.m_curPosition.y);
		glTexCoord2fv(XVec2::one);
		glVertex2f(ref.m_curPosition.x + tmpW, ref.m_curPosition.y + tmpW);
		glTexCoord2fv(XVec2::zeroOne);
		glVertex2f(ref.m_curPosition.x, ref.m_curPosition.y + tmpW);
	}
}
void XEchoParticles::setPosition(float x,float y)
{
	if (m_isEnd || m_isSetEnd) return;
	int length = m_parentParticle.m_curPosition.getLengthSqure(x,y);
	if(length < m_density * m_density) return;
	float angle = m_parentParticle.m_curPosition.getAngle(x,y) * RADIAN2DEGREE;
	length = (std::min)(sqrtf(length) / m_density,(float)m_insertSum);

	XVec2 pos = (XVec2(x,y) - m_parentParticle.m_curPosition) / length;
	for(int i = 1;i < length + 1;++ i)
	{
		XBasicParticle& ref = m_particles[m_curPoint];
		ref.m_curSize.x = m_parentParticle.m_initSize.x;
		ref.m_curColor = m_parentParticle.m_initColor;
		ref.m_curAngle = angle;
		ref.m_curPosition.set(m_parentParticle.m_curPosition + pos * i);
		ref.m_isEnable = true;
		++m_curPoint;
		if(m_curPoint >= m_atomSum) m_curPoint = 0;
	}
	m_parentParticle.m_curPosition.set(x,y);
}
void XEchoParticles::setPosition(float x,float y,float size)
{
	if (size <= 0.0f || m_isEnd || m_isSetEnd) return;
	int length = m_parentParticle.m_curPosition.getLengthSqure(x,y);
	if(length < m_density * m_density) return;
	float angle = m_parentParticle.m_curPosition.getAngle(x,y) * RADIAN2DEGREE;
	length = (std::min)(sqrtf(length) / m_density,(float)m_insertSum);
	XVec2 pos = (XVec2(x, y) - m_parentParticle.m_curPosition) / length;
	for(int i = 1;i < length + 1;++ i)
	{
		XBasicParticle& ref = m_particles[m_curPoint];
		ref.m_curSize.x = m_parentParticle.m_initSize.x * size;
		ref.m_curColor = m_parentParticle.m_initColor;
		ref.m_curAngle = angle;
		ref.m_curPosition.set(m_parentParticle.m_curPosition + pos * i);
		ref.m_isEnable = true;
		++m_curPoint;
		if(m_curPoint >= m_atomSum) m_curPoint = 0;
	}
	m_parentParticle.m_curPosition.set(x,y);
}
void XEchoParticles::setPosition(float x,float y,float size,float udR,float udG,float udB)//,float udA)
{
	if (size <= 0.0f || m_isEnd || m_isSetEnd) return;

	int length = m_parentParticle.m_curPosition.getLengthSqure(x,y);
	if(length < m_density * m_density) return;
	float angle = m_parentParticle.m_curPosition.getAngle(x,y) * RADIAN2DEGREE;
	length = (std::min)(sqrtf(length) / m_density,(float)m_insertSum);
	
	XVec2 pos = (XVec2(x, y) - m_parentParticle.m_curPosition) / length;
	for(int i = 1;i < length + 1;++ i)
	{
		XBasicParticle& ref = m_particles[m_curPoint];
		ref.m_curSize.x = m_parentParticle.m_initSize.x * size;
		ref.m_curColor = m_parentParticle.m_initColor;
		ref.m_curColor.r = udR;
		ref.m_curColor.g = udG;
		ref.m_curColor.b = udB;
		//ref.m_curColor.a = udA;
		ref.m_curAngle = angle;
		ref.m_curPosition.set(m_parentParticle.m_curPosition + pos * i);
		ref.m_isEnable = true;
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
	m_parentParticle.m_isEnable = false;
	m_isInited = 0;		//防止release之后调用其它成员函数而造成错误
}
bool XEchoParticles::getMemory(int atomSum)
{
	XMem::XDELETE_ARRAY(m_particles);
	m_atomSum = atomSum;
	m_particles = XMem::createArrayMem<XBasicParticle>(m_atomSum);
	if(m_particles == NULL) return  false;

	for(int i = 0;i < m_atomSum;++ i)
	{
		m_particles[i].m_isEnable = false;
	}
	return true;
}
}