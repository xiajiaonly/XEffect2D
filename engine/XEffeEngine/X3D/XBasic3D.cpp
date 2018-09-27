#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBasic3D.h"
//#include "glut.h"
#include "X3DWorld.h"

//注意：这里需要优化掉
//glPushAttrib(GL_ALL_ATTRIB_BITS);
//glMatrixMode(GL_MODELVIEW);
//glPushMatrix();
//这样的代码
namespace XE{
namespace XRender
{
	void drawLine(const XVec3& ps,const XVec3& pe,const XFColor& color,float w)
	{
		//XGL::DisableTexture2D();	//应该要加这一行，需要检查为什么没有加
		XGL::setBlendAlpha();

		glColor4fv(color);
		glLineWidth(w);
		glBegin(GL_LINES);
			glVertex3fv(ps);
			glVertex3fv(pe);
		glEnd();
		XGL::DisableBlend();
	}
	void drawLine(const XVec3& ps,const XVec3& n,float len,const XFColor& color,float w)
	{
		//XGL::DisableTexture2D();	//应该要加这一行，需要检查为什么没有加
		XGL::setBlendAlpha();

		glColor4fv(color);
		glLineWidth(w);
		glBegin(GL_LINES);
			glVertex3fv(ps);
			glVertex3fv(ps + (n * len));
		glEnd();

		XGL::DisableBlend();
	}
	void drawTriangle(const XVec3& p0,const XVec3& p1,const XVec3& p2,const XFColor& color)
	{
		glDisable(GL_CULL_FACE);	//这样孤立的调用会有造成bug。需要检查

		//XGL::DisableTexture2D();	//应该要加这一行，需要检查为什么没有加
		XGL::setBlendAlpha();

		glColor4fv(color);
		glBegin(GL_TRIANGLES);
			glNormal3fv(XMath::getNormal(p0,p1,p2));
			glVertex3fv(p0);
			glVertex3fv(p1);
			glVertex3fv(p2);
		glEnd();

		XGL::DisableBlend();
	}
	void drawTriangle(const XVec3& p0,const XVec3& p1,const XVec3& p2,
					  const XVec3& n,const XFColor& color)
	{
		//XGL::DisableTexture2D();	//应该要加这一行，需要检查为什么没有加
		XGL::setBlendAlpha();

		glColor4fv(color);
		glBegin(GL_TRIANGLES);
			glNormal3fv(n);
			glVertex3fv(p0);
			glVertex3fv(p1);
			glVertex3fv(p2);
		glEnd();

		XGL::DisableBlend();
	}
	void drawPlane(const XVec3& p0,const XVec3& p1,const XVec3& p2,const XVec3& p3,
						  const XVec3& n,const XFColor& color)
	{
		//XGL::DisableTexture2D();	//应该要加这一行，需要检查为什么没有加
		XGL::setBlendAlpha();

		glColor4fv(color);
		//glLineWidth(1);
		glBegin(GL_QUADS);
			glNormal3fv(n);
			glVertex3fv(p0);
			glVertex3fv(p1);
			glVertex3fv(p2);
			glVertex3fv(p3);
		glEnd();

		XGL::DisableBlend();
	}
	static const GLfloat n[6][3] = 
	{
		{ -1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, -1.0f }
	};
	static const GLint faces[6][4] =
	{
		{ 0, 1, 2, 3 },
		{ 3, 2, 6, 7 },
		{ 7, 6, 5, 4 },
		{ 4, 5, 1, 0 },
		{ 5, 6, 2, 1 },
		{ 7, 4, 0, 3 }
	};
	void drawCuboidBase()
	{
		static GLfloat v[8][3] =
		{
			{ -0.5f, -0.5f, -0.5f },
			{ -0.5f, -0.5f, 0.5f },
			{ -0.5f, 0.5f, 0.5f },
			{ -0.5f, 0.5f, -0.5f },
			{ 0.5f, -0.5f, -0.5f },
			{ 0.5f, -0.5f, 0.5f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.5f, 0.5f,-0.5f }
		};
		GLint i;
		glBegin(GL_QUADS);
		for (i = 5; i >= 0; --i)
		{
			glNormal3fv(&n[i][0]);
			glVertex3fv(&v[faces[i][0]][0]);
			glVertex3fv(&v[faces[i][1]][0]);
			glVertex3fv(&v[faces[i][2]][0]);
			glVertex3fv(&v[faces[i][3]][0]);
		}
		glEnd();
	}
	void drawCuboidWithTexBase()
	{
		glBegin(GL_QUADS);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2fv(XVec2::zero);		//u
		glVertex3fv(-XVec3(0.5f));		//0
		glTexCoord2fv(XVec2::zeroOne);	//u
		glVertex3f(-0.5f, -0.5f, 0.5f); //1
		glTexCoord2fv(XVec2::one);		//u
		glVertex3f(-0.5f, 0.5f, 0.5f);	//2
		glTexCoord2fv(XVec2::oneZero);	//u
		glVertex3f(-0.5f, 0.5f, -0.5f); //3
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2fv(XVec2::zero);		//u
		glVertex3f(-0.5f, 0.5f, -0.5f); //3
		glTexCoord2fv(XVec2::zeroOne);	//u
		glVertex3f(-0.5f, 0.5f, 0.5f);	//2
		glTexCoord2fv(XVec2::one);		//u
		glVertex3fv(XVec3(0.5f));		//6
		glTexCoord2fv(XVec2::oneZero);	//u
		glVertex3f(0.5f, 0.5f, -0.5f);	//7
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2fv(XVec2::oneZero);	//u
		glVertex3f(0.5f, 0.5f, -0.5f);	//7
		glTexCoord2fv(XVec2::one);		//u
		glVertex3fv(XVec3(0.5f));		//6
		glTexCoord2fv(XVec2::zeroOne);	//u
		glVertex3f(0.5f, -0.5f, 0.5f);	//5
		glTexCoord2fv(XVec2::zero);		//u
		glVertex3f(0.5f, -0.5f, -0.5f); //4
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2fv(XVec2::oneZero);	//u
		glVertex3f(0.5f, -0.5f, -0.5f); //4
		glTexCoord2fv(XVec2::one);		//u
		glVertex3f(0.5f, -0.5f, 0.5f);	//5
		glTexCoord2fv(XVec2::zeroOne);	//u
		glVertex3f(-0.5f, -0.5f, 0.5f); //1
		glTexCoord2fv(XVec2::zero);		//u
		glVertex3fv(-XVec3(0.5f));		//0
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2fv(XVec2::oneZero);	//u
		glVertex3f(0.5f, -0.5f, 0.5f);	//5
		glTexCoord2fv(XVec2::one);		//u
		glVertex3fv(XVec3(0.5f));		//6
		glTexCoord2fv(XVec2::zeroOne);	//u
		glVertex3f(-0.5f, 0.5f, 0.5f);	//2
		glTexCoord2fv(XVec2::zero);		//u
		glVertex3f(-0.5f, -0.5f, 0.5f); //1
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2fv(XVec2::one);		//u
		glVertex3f(0.5f, 0.5f, -0.5f);	//7
		glTexCoord2fv(XVec2::oneZero);	//u
		glVertex3f(0.5f, -0.5f, -0.5f); //4
		glTexCoord2fv(XVec2::zero);		//u
		glVertex3fv(-XVec3(0.5f));		//0
		glTexCoord2fv(XVec2::zeroOne);	//u
		glVertex3f(-0.5f, 0.5f, -0.5f); //3
		glEnd();
	}
	void drawCuboidBase(const XVec3& size)
	{
		GLfloat v[8][3];

		v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size.x;
		v[4][0] = v[5][0] = v[6][0] = v[7][0] = size.x;
		v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size.y;
		v[2][1] = v[3][1] = v[6][1] = v[7][1] = size.y;
		v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size.x;
		v[1][2] = v[2][2] = v[5][2] = v[6][2] = size.x;

		glBegin(GL_QUADS);
		for (int i = 5; i >= 0; --i)
		{
			glNormal3fv(&n[i][0]);
			glVertex3fv(&v[faces[i][0]][0]);
			glVertex3fv(&v[faces[i][1]][0]);
			glVertex3fv(&v[faces[i][2]][0]);
			glVertex3fv(&v[faces[i][3]][0]);
		}
		glEnd();
	}
	void drawCuboidBase(const XVec3& pos, const XVec3& size)
	{
		GLfloat v[8][3];

		v[0][0] = v[1][0] = v[2][0] = v[3][0] = pos.x - size.x;
		v[4][0] = v[5][0] = v[6][0] = v[7][0] = pos.x + size.x;
		v[0][1] = v[1][1] = v[4][1] = v[5][1] = pos.y - size.y;
		v[2][1] = v[3][1] = v[6][1] = v[7][1] = pos.y + size.y;
		v[0][2] = v[3][2] = v[4][2] = v[7][2] = pos.z - size.x;
		v[1][2] = v[2][2] = v[5][2] = v[6][2] = pos.z + size.x;

		glBegin(GL_QUADS);
		for (int i = 5; i >= 0; --i)
		{
			glNormal3fv(&n[i][0]);
			glVertex3fv(&v[faces[i][0]][0]);
			glVertex3fv(&v[faces[i][1]][0]);
			glVertex3fv(&v[faces[i][2]][0]);
			glVertex3fv(&v[faces[i][3]][0]);
		}
		glEnd();
	}
	void drawCuboid(const XVec3& pos, const XVec3& size)
	{
		XGL::DisableTexture2D();
		if(gFrameworkData.p3DWorld != NULL) 
		{
			//gFrameworkData.p3DWorld->m_worldMaterial.setAmblient(0.0f,0.0f,1.0f,0.5f);
			//gFrameworkData.p3DWorld->m_worldMaterial.setDiffuse(0.0f,0.0f,1.0f,0.5f);
			//gFrameworkData.p3DWorld->m_worldMaterial.setSpecular(0.0f,0.0f,1.0f,0.5f);
			gFrameworkData.p3DWorld->m_worldMaterial.bindMaterial();
			gFrameworkData.p3DWorld->useShadow(XFalse);
		}
		glColor4fv(XFColor::white);
		drawCuboidBase(pos, size);
		if (gFrameworkData.p3DWorld != NULL)
		{
			gFrameworkData.p3DWorld->removeShadow();
			gFrameworkData.p3DWorld->m_worldMaterial.unbindMaterial();
		}
	}
	void drawCuboid(const XVec3& center, const XVec3& size, const XVec3& angle, const XFColor& color)
	{
		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		XGL::DisableTexture2D();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
	//	glLoadIdentity();

		glTranslatef(center.x, center.y, center.z);
		glRotatef(angle.x, 1.0f, 0.0f, 0.0f);
		glRotatef(angle.y, 0.0f, 1.0f, 0.0f);
		glRotatef(angle.z, 0.0f, 0.0f, 1.0f);
		glScalef(size.x * 2.0f, size.y * 2.0f, size.z * 2.0f);
		if (gFrameworkData.p3DWorld != NULL)
		{
			//gFrameworkData.p3DWorld->m_worldMaterial.setAmblient(0.0f,0.0f,1.0f,0.5f);
			//gFrameworkData.p3DWorld->m_worldMaterial.setDiffuse(0.0f,0.0f,1.0f,0.5f);
			//gFrameworkData.p3DWorld->m_worldMaterial.setSpecular(0.0f,0.0f,1.0f,0.5f);
			gFrameworkData.p3DWorld->m_worldMaterial.bindMaterial();
			gFrameworkData.p3DWorld->useShadow(XFalse);
		}
		//glEnable(GL_DEPTH_TEST)
		//glDepthFunc(GL_NICEST)
		XGL::setBlendAlpha();
		glColor4fv(color);
		drawCuboidBase();
		if (gFrameworkData.p3DWorld != NULL)
		{
			gFrameworkData.p3DWorld->removeShadow();
			gFrameworkData.p3DWorld->m_worldMaterial.unbindMaterial();
		}

		XGL::DisableBlend();

		glPopMatrix();
		//glPopAttrib();
	}
	void drawCuboid(const XVec3& center, const XVec3& size, const XVec3& angle, unsigned int tex)
	{
		if (tex == 0) return;	//无效的贴图
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(center.x, center.y, center.z);
		glRotatef(angle.x, 1.0f, 0.0f, 0.0f);
		glRotatef(angle.y, 0.0f, 1.0f, 0.0f);
		glRotatef(angle.z, 0.0f, 0.0f, 1.0f);
		glScalef(size.x * 2.0f, size.y * 2.0f, size.z * 2.0f);
		if (gFrameworkData.p3DWorld != NULL)
		{
			gFrameworkData.p3DWorld->m_worldMaterial.bindMaterial();
			gFrameworkData.p3DWorld->useShadow(XFalse);
		}
		XGL::setBlendAlpha();
		XGL::EnableTexture2D();
		XGL::BindTexture2D(tex);
		glColor4fv(XFColor::white);
		drawCuboidWithTexBase();
		if (gFrameworkData.p3DWorld != NULL)
		{
			gFrameworkData.p3DWorld->removeShadow();
			gFrameworkData.p3DWorld->m_worldMaterial.unbindMaterial();
		}

		XGL::DisableBlend();

		glPopMatrix();
	}
	void drawCuboidEx(const XVec3& pos,
		const XVec3& size,	//长方体的尺寸，长宽高
		const XVec3& angle,//长方体的角度
		const XFColor& color)
	{
		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		//	glLoadIdentity();

		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(angle.x, 1.0f, 0.0f, 0.0f);
		glRotatef(angle.y, 0.0f, 1.0f, 0.0f);
		glRotatef(angle.z, 0.0f, 0.0f, 1.0f);
		glScalef(size.x, size.y, size.z);

		if (gFrameworkData.p3DWorld != NULL)
		{
			//gFrameworkData.p3DWorld->m_worldMaterial.setAmblient(0.0f,0.0f,1.0f,0.5f);
			//gFrameworkData.p3DWorld->m_worldMaterial.setDiffuse(0.0f,0.0f,1.0f,0.5f);
			//gFrameworkData.p3DWorld->m_worldMaterial.setSpecular(0.0f,0.0f,1.0f,0.5f);
			gFrameworkData.p3DWorld->m_worldMaterial.bindMaterial();
			gFrameworkData.p3DWorld->useShadow(XFalse);
		}
		//glEnable(GL_DEPTH_TEST)
		//glDepthFunc(GL_NICEST)
		XGL::setBlendAlpha();
		glColor4fv(color);
		float x = 1;
		float y = 0.5f;
		float z = 0.5f;
		glBegin(GL_QUADS);
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, -y, -z); //0
			glVertex3f(0.0f, -y, z); //1
			glVertex3f(0.0f, y, z); //2
			glVertex3f(0.0f, y, -z); //3
			glNormal3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, y, -z); //3
			glVertex3f(0.0f, y, z); //2
			glVertex3f(x, y, z); //6
			glVertex3f(x, y, -z); //7
			glNormal3f(1.0f, 0.0f, 0.0f);
			glVertex3f(x, y, -z); //7
			glVertex3f(x, y, z); //6
			glVertex3f(x, -y, z); //5
			glVertex3f(x, -y, -z); //4
			glNormal3f(0.0f, -1.0f, 0.0f);
			glVertex3f(x, -y, -z); //4
			glVertex3f(x, -y, z); //5
			glVertex3f(0.0f, -y, z); //1
			glVertex3f(0.0f, -y, -z); //0
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3f(x, -y, z); //5
			glVertex3f(x, y, z); //6
			glVertex3f(0.0f, y, z); //2
			glVertex3f(0.0f, -y, z); //1
			glNormal3f(0.0f, 0.0f, -1.0f);
			glVertex3f(x, y, -z); //7
			glVertex3f(x, -y, -z); //4
			glVertex3f(0.0f, -y, -z); //0
			glVertex3f(0.0f, y, -z); //3
		glEnd();
		if (gFrameworkData.p3DWorld != NULL)
		{
			gFrameworkData.p3DWorld->removeShadow();
			gFrameworkData.p3DWorld->m_worldMaterial.unbindMaterial();
		}

		XGL::DisableBlend();

		glPopMatrix();
		//glPopAttrib();
	}
#ifdef WITH_GLUT
	void drawBall(const XVec3& center,float r,const XVec3& angle,const XFColor& color)
	{
		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		glMatrixMode(GL_MODELVIEW);  
		glPushMatrix();
	//	glLoadIdentity();  

		glTranslatef(center.x,center.y,center.z);
		glRotatef(angle.x,1,0,0);
		glRotatef(angle.y,0,1,0);
		glRotatef(angle.z,0,0,1);

		XGL::setBlendAlpha();

		X3DWld.useShadow(XFalse);
		glColor4fv(color);
		//注意这里是使用glut进行画球，这样会造成glut的dll的引入,这样会造成不便
		glutSolidSphere(r,30,30);
	//	glutSolidCube(r);
	//	glutSolidTeapot(r);
		//GLUquadricObj *quad;
		//quad = gluNewQuadric();
		//gluQuadricTexture( quad, true );	//enable TexCoord generation
		//gluSphere(quad,r,30,30);

		X3DWld.removeShadow();
		XGL::DisableBlend();

		glPopMatrix();
		//glPopAttrib();
	}
#endif
	//计算法线的方程
	//osg::Vec3 COSG_Wnd::CalNormal(osg::Vec3 p1,osg::Vec3 p2,osg::Vec3 p3)
	//{
	// double   w0,w1,w2,v0,v1,v2,nr,nx,ny,nz; 
	// w0=p2._v[0]-p1._v[0];   w1=p2._v[1]-p1._v[1];   w2=p2._v[2]-p1._v[2];
	// v0=p3._v[0]-p1._v[0];  v1=p3._v[1]-p1._v[1]; v2=p3._v[2]-p1._v[2];
	// nx=(w1*v2-w2*v1); 
	// ny=(w2*v0-w0*v2); 
	// nz=(w0*v1-w1*v0); 
	// nr=sqrt(nx*nx+ny*ny+nz*nz); 
	// osg::Vec3 returnVec;
	// returnVec._v[0]=nx/nr;   returnVec._v[1]=ny/nr;   returnVec._v[2]=nz/nr;
	// return returnVec;
	//}
	//目前尚未实现
	void drawArray(const float *p,int w,int h)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
	//	glLoadIdentity();

		if(gFrameworkData.p3DWorld != NULL) 
			gFrameworkData.p3DWorld->useShadow(XFalse);
		int x,z;
		XVec3 p0,p1,p2,p3; //顺时针
		//XVec3 normal;
		glBegin(GL_TRIANGLES);
		for(x = 0;x < w;++ x)	//这里可以对数据进行横纵向的优化
		{
			p1.set(p[x * 3],p[x * 3 + 1],p[x * 3 + 2]);
			p2.set(p[(x + 1) * 3],p[(x + 1) * 3 + 1],p[(x + 1) * 3 + 2]);
			for(z = 0;z < h;++ z)
			{
				p0 = p1;
				p3 = p2;
				p1.set(p[(x + z * w) * 3],p[(x + z * w) * 3 + 1],p[(x + z * w) * 3 + 2]);
				//p2.set(x + 1,getHeight(x + 1,z + 1),z + 1);
				p2.set(p[(x + 1 + z * w) * 3],p[(x + 1 + z * w) * 3 + 1],p[(x + 1 + z * w) * 3 + 2]);
				//这里忽略掉从原点出发的点，目前只有kinect有这个需求
				if(p0.x == 0.0f && p0.y == 0.0f && p0.z == 0.0f) continue;
				if(p1.x == 0.0f && p1.y == 0.0f && p1.z == 0.0f) continue;
				if(p2.x == 0.0f && p2.y == 0.0f && p2.z == 0.0f) continue;
				if(p3.x == 0.0f && p3.y == 0.0f && p3.z == 0.0f) continue;

				//normal = getNormal(p0,p1,p2);
				//glNormal3fv(normal);
				glNormal3fv(XMath::getNormal(p0,p1,p2));
				glVertex3fv(p0);
				glVertex3fv(p1);
				glVertex3fv(p2);

				//normal = getNormal(p0,p2,p3);
				//glNormal3fv(normal);
				glNormal3fv(XMath::getNormal(p0,p2,p3));
				glVertex3fv(p0);
				glVertex3fv(p2);
				glVertex3fv(p3);
			}
		}
		glEnd();
		if(gFrameworkData.p3DWorld != NULL) 
			gFrameworkData.p3DWorld->removeShadow();

		glPopMatrix();
	}
	//未测试
	void drawArray(const float *p,int w,int h,unsigned int tex)
	{
		if (tex == 0) return;	//无效的贴图
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
	//	glLoadIdentity();

		XGL::EnableTexture2D();
	//	XGL::DisableBlend();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);	//光照与纹理进行混合
		XGL::BindTexture2D(tex);

		int x,z;
		XVec2 u0,u1,u2,u3;
		XVec3 p0,p1,p2,p3; //顺时针
		//XVec3 normal;
		glBegin(GL_TRIANGLES);
		for(x = 0;x < w;++ x)
		{
			u1.set((float)(x + 10.0f)/(float)w,0.0f);
			u2.set((float)(x + 11.0f)/(float)w,0.0f);
			p1.set(p[x * 3],p[x * 3 + 1],p[x * 3 + 2]);
			p2.set(p[(x + 1) * 3],p[(x + 1) * 3 + 1],p[(x + 1) * 3 + 2]);
			for(z = 0;z < h;++ z)
			{
				u0 = u1;u3 = u2;
				p0 = p1;p3 = p2;
				u1.set((float)(x + 10.0f)/(float)w,(float)z/(float)h);
				u2.set((float)(x + 11.0f)/(float)w,(float)z/(float)h);
				p1.set(p[(x + z * w) * 3],p[(x + z * w) * 3 + 1],p[(x + z * w) * 3 + 2]);
				//p2.set(x + 1,getHeight(x + 1,z + 1),z + 1);
				p2.set(p[(x + 1 + z * w) * 3],p[(x + 1 + z * w) * 3 + 1],p[(x + 1 + z * w) * 3 + 2]);
				//这里忽略掉从原点出发的点，目前只有kinect有这个需求
				if(p0.x == 0.0f && p0.y == 0.0f && p0.z == 0.0f) continue;
				if(p1.x == 0.0f && p1.y == 0.0f && p1.z == 0.0f) continue;
				if(p2.x == 0.0f && p2.y == 0.0f && p2.z == 0.0f) continue;
				if(p3.x == 0.0f && p3.y == 0.0f && p3.z == 0.0f) continue;

				//normal = getNormal(p0,p1,p2);
				//glNormal3fv(normal);
				glNormal3fv(XMath::getNormal(p0,p1,p2));
				glTexCoord2fv(u0);
				glVertex3fv(p0);
				glTexCoord2fv(u1);
				glVertex3fv(p1);
				glTexCoord2fv(u2);
				glVertex3fv(p2);

				//normal = getNormal(p0,p2,p3);
				//glNormal3fv(normal);
				glNormal3fv(XMath::getNormal(p0,p2,p3));
				glTexCoord2fv(u0);
				glVertex3fv(p0);
				glTexCoord2fv(u2);
				glVertex3fv(p2);
				glTexCoord2fv(u3);
				glVertex3fv(p3);
			}
		}
		glEnd();

		glPopMatrix();
	}
	void drawOrigin()
	{
		drawLine(XVec3::zero, XVec3(100.0f, 0.0f, 0.0f), XFColor::red, 2.0f);
		drawLine(XVec3::zero, XVec3(0.0f, 100.0f, 0.0f), XFColor::green, 2.0f);
		drawLine(XVec3::zero, XVec3(0.0f, 0.0f, 100.0f), XFColor::blue, 2.0f);
	}
	void drawFloor(float distance, float width)
	{
		float len = 100.0f * distance;
		XGL::DisableTexture2D();
		XGL::setBlendAlpha();

		glColor4fv(XFColor::gray);
		glLineWidth(width);
		glBegin(GL_LINES);
		XVec3 tmpPos(-100.0f * distance, 0.0f, -len);
		XVec3 tmpPos1(-100.0f * distance, 0.0f, len);
		for (int w = -100; w < 101; ++w)
		{
		//	glVertex3fv(XVec3(w * distance,0.0f,-len ));	//空间换时间的优化
		//	glVertex3fv(XVec3(w * distance,0.0f,len));
			glVertex3fv(tmpPos);
			glVertex3fv(tmpPos1);
			tmpPos.x += distance;
			tmpPos1.x += distance;
		}
		tmpPos.set(-len, 0.0f, -100.0f * distance);
		tmpPos1.set(len, 0.0f, -100.0f * distance);
		for (int h = -100; h < 101; ++h)
		{
		//	glVertex3fv(XVec3(-len,0.0f,h * distance));	//空间换时间的优化
		//	glVertex3fv(XVec3(len,0.0f,h * distance));
			glVertex3fv(tmpPos);
			glVertex3fv(tmpPos1);
			tmpPos.z += distance;
			tmpPos1.z += distance;
		}
		glEnd();

		XGL::DisableBlend();
	}
}
namespace XMath
{
	//实现gluLookAt的功能
	XMat4 calLookAtMatrix(const XVec3& eyePos, const XVec3& LookAtPos, const XVec3& up)
	{
		XVec3 zaxis = normalize(eyePos - LookAtPos);
		XVec3 xaxis = normalize(up * zaxis);
		XVec3 yaxis = zaxis * xaxis;

		return XMat4(xaxis.x, yaxis.x, zaxis.x, 0.0f,
					 xaxis.y, yaxis.y, zaxis.y, 0.0f,
					 xaxis.z, yaxis.z, zaxis.z, 0.0f,
					 -(xaxis.dot(eyePos)), -(yaxis.dot(eyePos)), -(zaxis.dot(eyePos)), 1.0f);
	}
	XMat4 calPerspectiveMatrix(float fovy,float rat,float zNear,float zFar)
	{
		////方案1：理论算法
		//float tan_fovy = tan(fovy * 0.5f * DEGREE2RADIAN);
		//float right = tan_fovy * rat * zNear;
		//float left = -right;
		//float top = tan_fovy * zNear;
		//float bottom = -top;
		//return XMat4(	2.0*zNear/(right-left),	0.0f,					(right+left)/(right-left),		0.0f,
		//				0.0f,					2.0*zNear/(top-bottom), (top+bottom)/(top-bottom),		0.0f,
		//				0.0f,					0.0f,					-(zFar+zNear)/(zFar-zNear),		-1.0f,
		//				0.0f,					0.0f,					-2.0*zFar*zNear/(zFar-zNear),	0.0f);
		////根据网上的资料正确的应该是下面的，但是实际测试中好像结果应该是上面的
		////return XMat4(	2.0*zNear/(right-left),	0.0f,					(right+left)/(right-left),		0.0f,
		////				0.0f,					2.0*zNear/(top-bottom), (top+bottom)/(top-bottom),		0.0f,
		////				0.0f,					0.0f,					-(zFar+zNear)/(zFar-zNear),		-2.0*zFar*zNear/(zFar-zNear),
		////				0.0f,					0.0f,					-1.0f,							0.0f);
		//方案2：对方案1的优化
		float tan_fovy = tan(fovy * 0.5f * DEGREE2RADIAN);
		float right = tan_fovy * rat * zNear;
		float top = tan_fovy * zNear;
		return XMat4(	zNear / right,	0.0f,			0.0f,								0.0f,
						0.0f,			zNear / top,	0.0f,								0.0f,
						0.0f,			0.0f,			-(zFar + zNear) / (zFar - zNear),	 -1.0f,
						0.0f,			0.0f,			-2.0*zFar*zNear / (zFar - zNear),	 0.0f);
	}
	bool getOrtho(const XMat4& mtrx, float& left, float& right, float& bottom, float& top, float& zNear, float& zFar)
	{
		if (mtrx.data[3] != 0.0 || mtrx.data[7] != 0.0 || mtrx.data[11] != 0.0 || mtrx.data[15] != 1.0) return false;

		zNear = (mtrx.data[14] + 1.0) / mtrx.data[10];
		zFar = (mtrx.data[14] - 1.0) / mtrx.data[10];

		left = -(1.0 + mtrx.data[12]) / mtrx.data[0];
		right = (1.0 - mtrx.data[12]) / mtrx.data[0];

		bottom = -(1.0 + mtrx.data[13]) / mtrx.data[5];
		top = (1.0 - mtrx.data[13]) / mtrx.data[5];

		return true;
	}
	bool getFrustum(const XMat4& mtrx, float& left, float& right, float& bottom, float& top, float& zNear, float& zFar)
	{
		if (mtrx.data[3] != 0.0 || mtrx.data[7] != 0.0 || mtrx.data[11] != 0.0 || mtrx.data[15] != 1.0) return false;

		zNear = mtrx.data[14] / (mtrx.data[10] - 1.0);
		zFar = mtrx.data[14] / (1.0 + mtrx.data[10]);

		left = zNear * (mtrx.data[8] - 1.0) / mtrx.data[0];
		right = zNear * (1.0 + mtrx.data[8]) / mtrx.data[0];

		top = zNear * (1.0 + mtrx.data[9]) / mtrx.data[5];
		bottom = zNear * (mtrx.data[9] - 1.0) / mtrx.data[5];

		return true;
	}
	bool getPerspective(const XMat4& mtrx, float& fovy, float& aspectRatio, float& zNear, float& zFar)
	{
		float right, left, top, bottom;
		if (getFrustum(mtrx, left, right, bottom, top, zNear, zFar))
		{
			fovy = (atan2(top, zNear) - atan2(bottom, zNear))*RADIAN2DEGREE;
			aspectRatio = (right - left) / (top - bottom);
			return true;
		}
		return false;
	}
	void getLookAt(const XMat4& mtrx, XVec3& eye, XVec3& center, XVec3& up, float lookDistance)
	{
		XMat4 inv = mtrx;
		inv = inv.inverse();
		eye = inv * XVec3::zero;
		up = transform4x4(mtrx, XVec3(0.0f, 1.0f, 0.0f));
		center = transform4x4(mtrx, XVec3(0.0f, 0.0f, -1.0f));
		normalize(center);
		center = eye + center * lookDistance;
	}
	XMat4 getRotate(float angle, XVec3 v)
	{
		float sinA = sin(angle * DEGREE2RADIAN);
		float cosA = cos(angle * DEGREE2RADIAN);
		float tCosA = 1.0f - cosA;
		v = normalize(v);
		return XMat4(tCosA * v.x * v.x + cosA, tCosA * v.x * v.y + v.z * sinA, tCosA * v.x * v.z - sinA * v.y, 0.0f,
			tCosA * v.x * v.y - sinA * v.z, tCosA * v.y * v.y + cosA, tCosA * v.y * v.z + sinA * v.x, 0.0f,
			tCosA * v.x * v.z + sinA * v.y, tCosA * v.y * v.z - v.x * sinA, tCosA * v.z * v.z + cosA, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float canPickingByRay(const XVec3& nearP,const XVec3& farP,XVec3 *v,const XVec3& n,XVec3& out)
	{
		//XVec3 l;
		//l = farP - nearP;
		//float nDotL = n ^ l;
		//if(nDotL >= 0.0f) return -1.0f;
		//XVec3 a = v[0];
		//float d = (n ^ (a - nearP)) / nDotL;
		//if(d < 0.0f || d > 1.0f) return -1.0f;
		//XVec3 p = nearP + l * d;	//射线与平面的焦点(d 是距离)
		//XVec3 b = v[1];
		//XVec3 c = v[2];

		//XVec3 n1 = (b - a) * (p - a);
		//XVec3 n2 = (c - b) * (p - b);
		//XVec3 n3 = (a - c) * (p - c);
		//if ((n ^ n1) >= 0.0f &&
	 //       (n ^ n2) >= 0.0f &&
	 //       (n ^ n3) >= 0.0f)
	 //   {//三角面与射线相交
		//	out = p;	//交点
		//	return d;
		//}
		//return -1.0f;
		//对上面的算法进行优化
		XVec3 l = farP - nearP;
		float nDotL = n.dot(l);
		if(nDotL >= 0.0f) return -1.0f;
		float d = (n.dot(v[0] - nearP)) / nDotL;
		if(d < 0.0f || d > 1.0f) return -1.0f;
		out = nearP + l * d;	//射线与平面的焦点(d 是距离)

		if ((n.dot((v[1] - v[0]) * (out - v[0]))) >= 0.0f &&
			(n.dot((v[2] - v[1]) * (out - v[1]))) >= 0.0f &&
			(n.dot((v[0] - v[2]) * (out - v[2]))) >= 0.0f)
		{//三角面与射线相交
			return d;
		}
		return -1.0f;
	}
	bool getUnProject(float x, float y, float z, const XMat4& modelview, const XMat4 &projection,
		const XVec4& viewport, XVec3& out)
	{
		//GL官方的方案
		////Transformation matrices
		//float m[16], A[16];
		//float in[4], out[4];
		////Calculation for inverting a matrix, compute projection x modelview
		////and store in A[16]
		//MultiplyMatrices4by4OpenGL_FLOAT(A, projection, modelview);
		////Now compute the inverse of matrix A
		//if (glhInvertMatrixf2(A, m) == 0)
		//	return 0;
		////Transformation of normalized coordinates between -1 and 1
		//in[0] = (winx - (float)viewport[0]) / (float)viewport[2] * 2.0 - 1.0;
		//in[1] = (winy - (float)viewport[1]) / (float)viewport[3] * 2.0 - 1.0;
		//in[2] = 2.0*winz - 1.0;
		//in[3] = 1.0;
		////Objects coordinates
		//MultiplyMatrixByVector4by4OpenGL_FLOAT(out, m, in);
		//if (out[3] == 0.0)
		//	return 0;
		//out[3] = 1.0 / out[3];
		//objectCoordinate[0] = out[0] * out[3];
		//objectCoordinate[1] = out[1] * out[3];
		//objectCoordinate[2] = out[2] * out[3];
		//return 1;
		//自己的方案
		XMat4 m = (projection * modelview).inverse();
		XVec4 in;
		in.x = (x - viewport.x) / viewport.z * 2.0f - 1.0f;
		in.y = (y - viewport.y) / viewport.w * 2.0f - 1.0f;
		in.z = 2.0f * z - 1.0f;
		in.w = 1.0f;
		in = in * m;
		if (in.w == 0.0f) return false;
		in.w = 1.0f / in.w;
		out.set(in.x * in.w, in.y * in.w, in.z * in.w);
		return true;
	}
	XVec3 worldToScreen(const XVec3& pos, const XMat4& modelview, const XMat4 &projection,
		const XRect& view)
	{
		XVec3 screenXY;
		//方案1:
		//float fTempo[8];
		////Modelview transform
		//fTempo[0] = modelview.data[0] * pos.x + modelview.data[4] * pos.y + modelview.data[8] * pos.z + modelview.data[12];  //w is always 1
		//fTempo[1] = modelview.data[1] * pos.x + modelview.data[5] * pos.y + modelview.data[9] * pos.z + modelview.data[13];
		//fTempo[2] = modelview.data[2] * pos.x + modelview.data[6] * pos.y + modelview.data[10] * pos.z + modelview.data[14];
		//fTempo[3] = modelview.data[3] * pos.x + modelview.data[7] * pos.y + modelview.data[11] * pos.z + modelview.data[15];
		////Projection transform, the final row of projection matrix is always [0 0 -1 0]
		////so we optimize for that.
		//fTempo[4] = projection.data[0] * fTempo[0] + projection.data[4] * fTempo[1] + projection.data[8] * fTempo[2] + projection.data[12] * fTempo[3];
		//fTempo[5] = projection.data[1] * fTempo[0] + projection.data[5] * fTempo[1] + projection.data[9] * fTempo[2] + projection.data[13] * fTempo[3];
		//fTempo[6] = projection.data[2] * fTempo[0] + projection.data[6] * fTempo[1] + projection.data[10] * fTempo[2] + projection.data[14] * fTempo[3];
		//fTempo[7] = -fTempo[2];
		////The result normalizes between -1 and 1
		//if (fTempo[7] == 0.0)	//The w value
		//	return 0;
		//fTempo[7] = 1.0 / fTempo[7];
		////Perspective division
		//fTempo[4] *= fTempo[7];
		//fTempo[5] *= fTempo[7];
		//fTempo[6] *= fTempo[7];
		////Window coordinates
		////Map x, y to range 0-1
		//screenXY.x = (fTempo[4] * 0.5 + 0.5)*view.getWidth() + view.left;
		//screenXY.y = (fTempo[5] * 0.5 + 0.5)*view.getHeight() + view.top;
		////This is only correct when glDepthRange(0.0, 1.0)
		//screenXY.z = (1.0 + fTempo[6])*0.5;	//Between 0 and 1
		//方案2：
		XVec3 tmpV = pos * (projection * modelview);
		//XVec4 tmpV = XVec4(pos.x, pos.y, pos.z, 1.0f) * (projection * modelview);
		//if (tmpV.w == 0.0f) return XVec2::zero;
		//tmpV.w = 1.0f / tmpV.w;
		//tmpV.x *= tmpV.w;
		//tmpV.y *= tmpV.w;
		//tmpV.z *= tmpV.w;

		screenXY.x = (tmpV.x + 1.0f) * 0.5f * view.getWidth() + view.left;
		screenXY.y = (1.0f - tmpV.y) * 0.5f * view.getHeight() + view.top;
		screenXY.z = (1.0 + tmpV.z) * 0.5f;
		return screenXY;
	}
	bool getUnProject(const XVec2& pos, XVec3& nearP, XVec3& farP)
	{
		if (gFrameworkData.p3DWorld == NULL) return true;
		//XMat4 m = (X3DWld.m_worldCam.getProjectMatrix() * X3DWld.m_worldCam.getViewMatrix()).inverse();
		XMat4 m = (gFrameworkData.p3DWorld->m_worldCam.getProjXView()).inverse();
		XVec4 inN, inF;
		inN.x = inF.x = pos.x / XEG.getSceneW() * 2.0f - 1.0f;
		inN.y = inF.y = pos.y / XEG.getSceneH() * 2.0f - 1.0f;
		inN.z = -1.0f;
		inF.z = 1.0f;
		inN.w = inF.w = 1.0f;
		inN = inN * m;
		inF = inF * m;
		if (inN.w == 0.0f || inF.w == 0.0f) return false;
		inN.w = 1.0f / inN.w;
		inF.w = 1.0f / inF.w;
		nearP.set(inN.x * inN.w, inN.y * inN.w, inN.z * inN.w);
		farP.set(inF.x * inF.w, inF.y * inF.w, inF.z * inF.w);
		return true;
	}
}
}