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
	void drawLine(const XVector3 &ps,const XVector3 &pe,const XFColor & color,float w)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
		glColor4fv(color);
		glLineWidth(w);
		glBegin(GL_LINES);
			glVertex3fv(ps);
			glVertex3fv(pe);
		glEnd();
		XGL::DisableBlend();
	}
	void drawLine(const XVector3 &ps,const XVector3 &n,float len,const XFColor &color,float w)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
		glColor4fv(color);
		glLineWidth(w);
		glBegin(GL_LINES);
			glVertex3fv(ps);
			glVertex3fv(ps + (n * len));
		glEnd();

		XGL::DisableBlend();
	}
	void drawTriangle(const XVector3 &p0,const XVector3 &p1,const XVector3 &p2,const XFColor & color)
	{
		glDisable(GL_CULL_FACE);

		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
		glColor4fv(color);
		glBegin(GL_TRIANGLES);
			glNormal3fv(XMath::getNormal(p0,p1,p2));
			glVertex3fv(p0);
			glVertex3fv(p1);
			glVertex3fv(p2);
		glEnd();

		XGL::DisableBlend();
	}
	void drawTriangle(const XVector3 &p0,const XVector3 &p1,const XVector3 &p2,
					  const XVector3 &n,const XFColor & color)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
		glColor4fv(color);
		glBegin(GL_TRIANGLES);
			glNormal3fv(n);
			glVertex3fv(p0);
			glVertex3fv(p1);
			glVertex3fv(p2);
		glEnd();

		XGL::DisableBlend();
	}
	void drawPlane(const XVector3 &p0,const XVector3 &p1,const XVector3 &p2,const XVector3 &p3,
						  const XVector3 &n,const XFColor & color)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
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
	void drawBox(GLfloat size)
	{
		static const GLfloat n[6][3] =
		{
			{-1.0, 0.0, 0.0},
			{0.0, 1.0, 0.0},
			{1.0, 0.0, 0.0},
			{0.0, -1.0, 0.0},
			{0.0, 0.0, 1.0},
			{0.0, 0.0, -1.0}
		};
		static const GLint faces[6][4] =
		{
			{0, 1, 2, 3},
			{3, 2, 6, 7},
			{7, 6, 5, 4},
			{4, 5, 1, 0},
			{5, 6, 2, 1},
			{7, 4, 0, 3}
		};
		GLfloat v[8][3];
		GLint i;

		v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
		v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
		v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
		v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
		v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
		v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

		glBegin(GL_QUADS);
		for(i = 5;i >= 0;--i) 
		{
			glNormal3fv(&n[i][0]);
			glVertex3fv(&v[faces[i][0]][0]);
			glVertex3fv(&v[faces[i][1]][0]);
			glVertex3fv(&v[faces[i][2]][0]);
			glVertex3fv(&v[faces[i][3]][0]);
		}
		glEnd();
	}
	void drawBox(const XVector3 &center,const XVector3 &size,const XVector3 &angle,const XFColor & color)
	{
		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
	//	glLoadIdentity();

		glTranslatef(center.x,center.y,center.z);
		glRotatef(angle.x,1,0,0);
		glRotatef(angle.y,0,1,0);
		glRotatef(angle.z,0,0,1);
		if(gFrameworkData.p3DWorld != NULL) 
		{
			//gFrameworkData.p3DWorld->m_worldMaterial.setAmblient(0.0f,0.0f,1.0f,0.5f);
			//gFrameworkData.p3DWorld->m_worldMaterial.setDiffuse(0.0f,0.0f,1.0f,0.5f);
			//gFrameworkData.p3DWorld->m_worldMaterial.setSpecular(0.0f,0.0f,1.0f,0.5f);
			gFrameworkData.p3DWorld->m_worldMaterial.usetMaterial();
			gFrameworkData.p3DWorld->useShadow(XFalse);
		}
		//glEnable(GL_DEPTH_TEST)
		//glDepthFunc(GL_NICEST)
		//XGL::EnableBlend();
		//XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(color);
		glBegin(GL_QUADS);
			glNormal3f(-1.0f,0.0f,0.0f);
			glVertex3f(-size.x,-size.y,-size.z); //0
			glVertex3f(-size.x,-size.y, size.z); //1
			glVertex3f(-size.x, size.y, size.z); //2
			glVertex3f(-size.x, size.y,-size.z); //3
			glNormal3f(0.0f,1.0f,0.0f);
			glVertex3f(-size.x, size.y,-size.z); //3
			glVertex3f(-size.x, size.y, size.z); //2
			glVertex3f( size.x, size.y, size.z); //6
			glVertex3f( size.x, size.y,-size.z); //7
			glNormal3f(1.0f,0.0f,0.0f);
			glVertex3f( size.x, size.y,-size.z); //7
			glVertex3f( size.x, size.y, size.z); //6
			glVertex3f( size.x,-size.y, size.z); //5
			glVertex3f( size.x,-size.y,-size.z); //4
			glNormal3f(0.0f,-1.0f,0.0f);
			glVertex3f( size.x,-size.y,-size.z); //4
			glVertex3f( size.x,-size.y, size.z); //5
			glVertex3f(-size.x,-size.y, size.z); //1
			glVertex3f(-size.x,-size.y,-size.z); //0
			glNormal3f(0.0f,0.0f,1.0f);
			glVertex3f( size.x,-size.y, size.z); //5
			glVertex3f( size.x, size.y, size.z); //6
			glVertex3f(-size.x, size.y, size.z); //2
			glVertex3f(-size.x,-size.y, size.z); //1
			glNormal3f(0.0f,0.0f,-1.0f);
			glVertex3f( size.x, size.y,-size.z); //7
			glVertex3f( size.x,-size.y,-size.z); //4
			glVertex3f(-size.x,-size.y,-size.z); //0
			glVertex3f(-size.x, size.y,-size.z); //3
		glEnd();
		if(gFrameworkData.p3DWorld != NULL) 
			gFrameworkData.p3DWorld->removeShadow();

		//XGL::DisableBlend();

		glPopMatrix();
		//glPopAttrib();
	}
	void drawBox(const XVector3 &center,const XVector3 &size,const XVector3 &angle,unsigned int tex)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(center.x,center.y,center.z);
		glRotatef(angle.x,1,0,0);
		glRotatef(angle.y,0,1,0);
		glRotatef(angle.z,0,0,1);
		if(gFrameworkData.p3DWorld != NULL) 
		{
			gFrameworkData.p3DWorld->m_worldMaterial.usetMaterial();
			gFrameworkData.p3DWorld->useShadow(XFalse);
		}
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		XGL::EnableTexture2D();
		XGL::BindTexture2D(tex);
		glColor4fv(XFColor::white);
		glBegin(GL_QUADS);
			glNormal3f(-1.0f,0.0f,0.0f);
			glTexCoord2f(0.0f,0.0f);	//u
			glVertex3f(-size.x,-size.y,-size.z); //0
			glTexCoord2f(0.0f,1.0f);	//u
			glVertex3f(-size.x,-size.y, size.z); //1
			glTexCoord2f(1.0f,1.0f);	//u
			glVertex3f(-size.x, size.y, size.z); //2
			glTexCoord2f(1.0f,0.0f);	//u
			glVertex3f(-size.x, size.y,-size.z); //3
			glNormal3f(0.0f,1.0f,0.0f);
			glTexCoord2f(0.0f,0.0f);	//u
			glVertex3f(-size.x, size.y,-size.z); //3
			glTexCoord2f(0.0f,1.0f);	//u
			glVertex3f(-size.x, size.y, size.z); //2
			glTexCoord2f(1.0f,1.0f);	//u
			glVertex3f( size.x, size.y, size.z); //6
			glTexCoord2f(1.0f,0.0f);	//u
			glVertex3f( size.x, size.y,-size.z); //7
			glNormal3f(1.0f,0.0f,0.0f);
			glTexCoord2f(1.0f,0.0f);	//u
			glVertex3f( size.x, size.y,-size.z); //7
			glTexCoord2f(1.0f,1.0f);	//u
			glVertex3f( size.x, size.y, size.z); //6
			glTexCoord2f(0.0f,1.0f);	//u
			glVertex3f( size.x,-size.y, size.z); //5
			glTexCoord2f(0.0f,0.0f);	//u
			glVertex3f( size.x,-size.y,-size.z); //4
			glNormal3f(0.0f,-1.0f,0.0f);
			glTexCoord2f(1.0f,0.0f);	//u
			glVertex3f( size.x,-size.y,-size.z); //4
			glTexCoord2f(1.0f,1.0f);	//u
			glVertex3f( size.x,-size.y, size.z); //5
			glTexCoord2f(0.0f,1.0f);	//u
			glVertex3f(-size.x,-size.y, size.z); //1
			glTexCoord2f(0.0f,0.0f);	//u
			glVertex3f(-size.x,-size.y,-size.z); //0
			glNormal3f(0.0f,0.0f,1.0f);
			glTexCoord2f(1.0f,0.0f);	//u
			glVertex3f( size.x,-size.y, size.z); //5
			glTexCoord2f(1.0f,1.0f);	//u
			glVertex3f( size.x, size.y, size.z); //6
			glTexCoord2f(0.0f,1.0f);	//u
			glVertex3f(-size.x, size.y, size.z); //2
			glTexCoord2f(0.0f,0.0f);	//u
			glVertex3f(-size.x,-size.y, size.z); //1
			glNormal3f(0.0f,0.0f,-1.0f);
			glTexCoord2f(1.0f,1.0f);	//u
			glVertex3f( size.x, size.y,-size.z); //7
			glTexCoord2f(1.0f,0.0f);	//u
			glVertex3f( size.x,-size.y,-size.z); //4
			glTexCoord2f(0.0f,0.0f);	//u
			glVertex3f(-size.x,-size.y,-size.z); //0
			glTexCoord2f(0.0f,1.0f);	//u
			glVertex3f(-size.x, size.y,-size.z); //3
		glEnd();
		if(gFrameworkData.p3DWorld != NULL) 
			gFrameworkData.p3DWorld->removeShadow();

		XGL::DisableBlend();

		glPopMatrix();
	}
	void drawCuboid(const XVector3 &pos,
		const XVector3 &size,	//长方体的尺寸，长宽高
		const XVector3 &angle,//长方体的角度
		const XFColor &color)
	{
		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
	//	glLoadIdentity();

		glTranslatef(pos.x,pos.y,pos.z);
		glRotatef(angle.x,1,0,0);
		glRotatef(angle.y,0,1,0);
		glRotatef(angle.z,0,0,1);

		if(gFrameworkData.p3DWorld != NULL) 
		{
			//gFrameworkData.p3DWorld->m_worldMaterial.setAmblient(0.0f,0.0f,1.0f,0.5f);
			//gFrameworkData.p3DWorld->m_worldMaterial.setDiffuse(0.0f,0.0f,1.0f,0.5f);
			//gFrameworkData.p3DWorld->m_worldMaterial.setSpecular(0.0f,0.0f,1.0f,0.5f);
			gFrameworkData.p3DWorld->m_worldMaterial.usetMaterial();
			gFrameworkData.p3DWorld->useShadow(XFalse);
		}
		//glEnable(GL_DEPTH_TEST)
		//glDepthFunc(GL_NICEST)
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4fv(color);
		float x = size.x;// * 0.5f;
		float y = size.y * 0.5f;
		float z = size.z * 0.5f;
		glBegin(GL_QUADS);
			glNormal3f(-1.0f,0.0f,0.0f);
			glVertex3f(0.0f,-y,-z); //0
			glVertex3f(0.0f,-y, z); //1
			glVertex3f(0.0f,y, z); //2
			glVertex3f(0.0f,y,-z); //3
			glNormal3f(0.0f,1.0f,0.0f);
			glVertex3f(0.0f,y,-z); //3
			glVertex3f(0.0f,y, z); //2
			glVertex3f(x,y, z); //6
			glVertex3f(x,y,-z); //7
			glNormal3f(1.0f,0.0f,0.0f);
			glVertex3f(x,y,-z); //7
			glVertex3f(x,y, z); //6
			glVertex3f(x,-y, z); //5
			glVertex3f(x,-y,-z); //4
			glNormal3f(0.0f,-1.0f,0.0f);
			glVertex3f(x,-y,-z); //4
			glVertex3f(x,-y, z); //5
			glVertex3f(0.0f,-y, z); //1
			glVertex3f(0.0f,-y,-z); //0
			glNormal3f(0.0f,0.0f,1.0f);
			glVertex3f(x,-y, z); //5
			glVertex3f(x,y, z); //6
			glVertex3f(0.0f,y, z); //2
			glVertex3f(0.0f,-y, z); //1
			glNormal3f(0.0f,0.0f,-1.0f);
			glVertex3f(x,y,-z); //7
			glVertex3f(x,-y,-z); //4
			glVertex3f(0.0f,-y,-z); //0
			glVertex3f(0.0f,y,-z); //3
		glEnd();
		if(gFrameworkData.p3DWorld != NULL) 
			gFrameworkData.p3DWorld->removeShadow();

		XGL::DisableBlend();

		glPopMatrix();
		//glPopAttrib();
	}
	//void drawBall(const XVector3 &center,float r,const XVector3 &angle,const XFColor & color)
	//{
	//	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//	glMatrixMode(GL_MODELVIEW);  
	//	glPushMatrix();
	////	glLoadIdentity();  

	//	glTranslatef(center.x,center.y,center.z);
	//	glRotatef(angle.x,1,0,0);
	//	glRotatef(angle.y,0,1,0);
	//	glRotatef(angle.z,0,0,1);

	//	XGL::EnableBlend();
	//	XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//	X3DWld.useShadow(XFalse);
	//	glColor4fv(color);
	//	//注意这里是使用glut进行画球，这样会造成glut的dll的引入,这样会造成不便
	//	glutSolidSphere(r,30,30);
	////	glutSolidCube(r);
	////	glutSolidTeapot(r);
	//	//GLUquadricObj *quad;
	//	//quad = gluNewQuadric();
	//	//gluQuadricTexture( quad, true );	//enable TexCoord generation
	//	//gluSphere(quad,r,30,30);

	//	X3DWld.removeShadow();
	//	XGL::DisableBlend();

	//	glPopMatrix();
	//	//glPopAttrib();
	//}
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
		XVector3 p0,p1,p2,p3; //顺时针
		//XVector3 normal;
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
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
	//	glLoadIdentity();

		XGL::EnableTexture2D();
	//	XGL::DisableBlend();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);	//光照与纹理进行混合
		XGL::BindTexture2D(tex);

		int x,z;
		XVector2 u0,u1,u2,u3;
		XVector3 p0,p1,p2,p3; //顺时针
		//XVector3 normal;
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
		drawLine(XVector3::zero,
			XVector3(100.0f,0.0f,0.0f),XFColor::red,2.0f);
		drawLine(XVector3::zero,
			XVector3(0.0f,100.0f,0.0f),XFColor::green,2.0f);
		drawLine(XVector3::zero,
			XVector3(0.0f,0.0f,100.0f),XFColor::blue,2.0f);
	}
	void drawFloor(float distance,float width)
	{
		float len = 100.0f * distance;
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
		glColor4fv(XFColor::gray);
		glLineWidth(width);
		glBegin(GL_LINES);
		XVector3 tmpPos(-100.0f * distance,0.0f,-len);
		XVector3 tmpPos1(-100.0f * distance,0.0f,len);
		for(int w = -100;w < 101;++ w)
		{
		//	glVertex3fv(XVector3(w * distance,0.0f,-len ));	//空间换时间的优化
		//	glVertex3fv(XVector3(w * distance,0.0f,len));
			glVertex3fv(tmpPos);
			glVertex3fv(tmpPos1);
			tmpPos.x += distance;
			tmpPos1.x += distance;
		}
		tmpPos.set(-len,0.0f,-100.0f * distance);
		tmpPos1.set(len,0.0f,-100.0f * distance);
		for(int h = -100;h < 101;++ h)
		{
		//	glVertex3fv(XVector3(-len,0.0f,h * distance));	//空间换时间的优化
		//	glVertex3fv(XVector3(len,0.0f,h * distance));
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
	XMatrix4x4 calLookAtMatrix(const XVector3 &eyePos,const XVector3 &LookAtPos,const XVector3 &up)
	{
		XVector3 zaxis = normalize(eyePos - LookAtPos);
		XVector3 xaxis = normalize(up * zaxis);
		XVector3 yaxis = zaxis * xaxis;

		return XMatrix4x4(xaxis.x, yaxis.x, zaxis.x, 0.0f,
						xaxis.y, yaxis.y, zaxis.y, 0.0f,
						xaxis.z, yaxis.z, zaxis.z, 0.0f,
						-(xaxis.dot(eyePos)), -(yaxis.dot(eyePos)), -(zaxis.dot(eyePos)),1.0f);
	}
	XMatrix4x4 calPerspectiveMatrix(float fovy,float rat,float zNear,float zFar)
	{
		float tan_fovy = tan(fovy * 0.5f * DEGREE2RADIAN);
		float right = tan_fovy * rat * zNear;
		float left = -right;
		float top = tan_fovy * zNear;
		float bottom = -top;
		return XMatrix4x4(	2.0*zNear/(right-left),	0.0f,					(right+left)/(right-left),		0.0f,
							0.0f,					2.0*zNear/(top-bottom), (top+bottom)/(top-bottom),		0.0f,
							0.0f,					0.0f,					-(zFar+zNear)/(zFar-zNear),		-1.0f,
							0.0f,					0.0f,					-2.0*zFar*zNear/(zFar-zNear),	0.0f);
		//根据网上的资料正确的应该是下面的，但是实际测试中好像结果应该是上面的
		//return XMatrix4x4(	2.0*zNear/(right-left),	0.0f,					(right+left)/(right-left),		0.0f,
		//					0.0f,					2.0*zNear/(top-bottom), (top+bottom)/(top-bottom),		0.0f,
		//					0.0f,					0.0f,					-(zFar+zNear)/(zFar-zNear),		-2.0*zFar*zNear/(zFar-zNear),
		//					0.0f,					0.0f,					-1.0f,							0.0f);
	}
	XMatrix4x4 calOrthoMatrix(float left,float right,float bottom,float top,float zNear,float zFar)
	{//没有经过测试
 		return XMatrix4x4(	2.0f/(right-left),	0.0f,				0.0f,				-(right+left)/(right-left),
							0.0f,				2.0f/(top-bottom),	0.0f,				-(top+bottom)/(top-bottom),
							0.0f,				0.0f,				-2.0/(zFar-zNear),	-(zFar+zNear)/(zFar-zNear),
							0.0f,				0.0f,				0.0f,				1.0f);
	}
	XMatrix4x4 calOrtho2DMatrix(float left,float right,float bottom,float top)
	{//没有经过测试
		return calOrthoMatrix(left, right, bottom, top, -1.0, 1.0);
	}
	XMatrix4x4 calFrustumMatrix(float left,float right,float bottom,float top,float zNear,float zFar)
	{//没有经过测试
		return XMatrix4x4(	2.0*zNear/(right-left),	0.0f,					(right+left)/(right-left),		0.0f,
							0.0f,					2.0*zNear/(top-bottom), (top+bottom)/(top-bottom),		0.0f,
							0.0f,					0.0f,					-(zFar+zNear)/(zFar-zNear),		-2.0*zFar*zNear/(zFar-zNear),
							0.0f,					0.0f,					-1.0f,							0.0f);

	}
	bool getOrtho(const XMatrix4x4& mtrx,float& left,float& right,float& bottom,float& top,float& zNear,float& zFar)
	{
		if(mtrx.data[3]!=0.0 || mtrx.data[7]!=0.0 || mtrx.data[11]!=0.0 || mtrx.data[15]!=1.0) return false;

		zNear = (mtrx.data[14]+1.0) / mtrx.data[10];
		zFar = (mtrx.data[14]-1.0) / mtrx.data[10];

		left = -(1.0+mtrx.data[12]) / mtrx.data[0];
		right = (1.0-mtrx.data[12]) / mtrx.data[0];

		bottom = -(1.0+mtrx.data[13]) / mtrx.data[5];
		top = (1.0-mtrx.data[13]) / mtrx.data[5];

		return true;
	}
	bool getFrustum(const XMatrix4x4& mtrx,float& left,float& right,float& bottom,float& top,float& zNear,float& zFar)
	{
		if(mtrx.data[3]!=0.0 || mtrx.data[7]!=0.0 || mtrx.data[11]!=0.0 || mtrx.data[15]!=1.0) return false;

		zNear = mtrx.data[14] / (mtrx.data[10]-1.0);
		zFar = mtrx.data[14] / (1.0+mtrx.data[10]);

		left = zNear * (mtrx.data[8]-1.0) / mtrx.data[0];
		right = zNear * (1.0+mtrx.data[8]) / mtrx.data[0];

		top = zNear * (1.0+mtrx.data[9]) / mtrx.data[5];
		bottom = zNear * (mtrx.data[9]-1.0) / mtrx.data[5];

		return true;
	}
	bool getPerspective(const XMatrix4x4& mtrx,float& fovy,float& aspectRatio,float& zNear, float& zFar)
	{
		float right  =  0.0f;
		float left   =  0.0f;
		float top    =  0.0f;
		float bottom =  0.0f;
		if (getFrustum(mtrx,left,right,bottom,top,zNear,zFar))
		{
			fovy = (atan(top/zNear)-atan(bottom/zNear))*RADIAN2DEGREE;
			aspectRatio = (right-left)/(top-bottom);
			return true;
		}
		return false;
	}
	void getLookAt(const XMatrix4x4& mtrx,XVector3& eye,XVector3& center,XVector3& up,float lookDistance)
	{
		XMatrix4x4 inv = mtrx;
		inv = inv.inverse();
		eye = inv * XVector3::zero;
		up = transform4x4(mtrx,XVector3(0.0,1.0,0.0));
		center = transform4x4(mtrx,XVector3(0.0,0.0,-1));
		normalize(center);
		center = eye + center * lookDistance;
	}
	XMatrix4x4 getRotate(float angle,XVector3 v)
	{
		float sinA = sin(angle * DEGREE2RADIAN);
		float cosA = cos(angle * DEGREE2RADIAN);
		float tCosA = 1 - cosA;
		v = normalize(v);
		return XMatrix4x4( tCosA * v.x * v.x + cosA,		tCosA * v.x * v.y + v.z * sinA, tCosA * v.x * v.z - sinA * v.y, 0,
							tCosA * v.x * v.y - sinA * v.z,	tCosA * v.y * v.y + cosA,		tCosA * v.y * v.z + sinA * v.x, 0,
							tCosA * v.x * v.z + sinA * v.y,	tCosA * v.y * v.z - v.x * sinA,	tCosA * v.z * v.z + cosA,		0,
							0,0,0,1);
	}
	XMatrix4x4 getRotate(const XVector3 &a)
	{
		XMatrix4x4 ret = getRotateX(a.x);
		ret = ret * getRotateY(a.y);
		ret = ret * getRotateZ(a.z);
		return ret;
	}
	XMatrix4x4 getRotateZ(float angle)
	{
		float sinA = sin(angle * DEGREE2RADIAN);
		float cosA = cos(angle * DEGREE2RADIAN);
		return XMatrix4x4(	cosA,sinA,0,0,
							-sinA,cosA,0,0,
							0,0,1,0,
							0,0,0,1);
	}
	XMatrix4x4 getRotateX(float angle)
	{
		float sinA = sin(angle * DEGREE2RADIAN);
		float cosA = cos(angle * DEGREE2RADIAN);
		return XMatrix4x4(	1,0,0,0,
							0,cosA,sinA,0,
							0,-sinA,cosA,0,
							0,0,0,1);
	}
	XMatrix4x4 getRotateY(float angle)
	{
		float sinA = sin(angle * DEGREE2RADIAN);
		float cosA = cos(angle * DEGREE2RADIAN);
		return XMatrix4x4(	cosA,0,-sinA,0,
							0,1,0,0,
							sinA,0,cosA,0,
							0,0,0,1);
	}
	XMatrix4x4 getTranslate(const XVector3 &p)
	{
		return XMatrix4x4(	1,0,0,0,
							0,1,0,0,
							0,0,1,0,
							p.x,p.y,p.z,1);
	}
	XMatrix4x4 getScale(const XVector3 &s)
	{
		return XMatrix4x4( s.x,0,	0,	0,
							0,	s.y,0,	0,
							0,	0,	s.z,0,
							0,	0,	0,	1);
	}
	float canPickingByRay(const XVector3 &nearP,const XVector3 &farP,XVector3 *v,const XVector3 &n,XVector3 &out)
	{
		//XVector3 l;
		//l = farP - nearP;
		//float nDotL = n ^ l;
		//if(nDotL >= 0.0f) return -1.0f;
		//XVector3 a = v[0];
		//float d = (n ^ (a - nearP)) / nDotL;
		//if(d < 0.0f || d > 1.0f) return -1.0f;
		//XVector3 p = nearP + l * d;	//射线与平面的焦点(d 是距离)
		//XVector3 b = v[1];
		//XVector3 c = v[2];

		//XVector3 n1 = (b - a) * (p - a);
		//XVector3 n2 = (c - b) * (p - b);
		//XVector3 n3 = (a - c) * (p - c);
		//if ((n ^ n1) >= 0.0f &&
	 //       (n ^ n2) >= 0.0f &&
	 //       (n ^ n3) >= 0.0f)
	 //   {//三角面与射线相交
		//	out = p;	//交点
		//	return d;
		//}
		//return -1.0f;
		//对上面的算法进行优化
		XVector3 l = farP - nearP;
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
	int getUnProject(float x,float y,float z,const XMatrix4x4& modelview,const XMatrix4x4 &projection,
					  const XVector4 &viewport,XVector3 &out)
	{
		XMatrix4x4 m = (projection * modelview).inverse();
		XVector4 in;
		in.x = (x - viewport.x) / viewport.z * 2.0f - 1.0f;
		in.y = (y - viewport.y) / viewport.w * 2.0f - 1.0f;
		in.z = 2.0f * z - 1.0f;
		in.w = 1.0f;
		in = in * m;
		if(in.w == 0.0f) return 0;
		in.w = 1.0f / in.w;
		out.set(in.x * in.w,in.y * in.w,in.z * in.w);
		return 1;
	}
	int getUnProject(const XVector2 &pos,XVector3 &nearP,XVector3 &farP)
	{
		if(gFrameworkData.p3DWorld == NULL) return 1; 
		//XMatrix4x4 m = (X3DWld.m_worldCam.getProjectMatrix() * X3DWld.m_worldCam.getViewMatrix()).inverse();
		XMatrix4x4 m = (gFrameworkData.p3DWorld->m_worldCam.getProjXView()).inverse();
		XVector4 inN,inF;
		inN.x = inF.x = pos.x / getWindowWidth() * 2.0f - 1.0f;
		inN.y = inF.y = pos.y / getWindowHeight() * 2.0f - 1.0f;
		inN.z = -1.0f;
		inF.z = 1.0f;
		inN.w = inF.w = 1.0f;
		inN = inN * m;
		inF = inF * m;
		if(inN.w == 0.0f || inF.w == 0.0f) return 0;
		inN.w = 1.0f / inN.w;
		inF.w = 1.0f / inF.w;
		nearP.set(inN.x * inN.w,inN.y * inN.w,inN.z * inN.w);
		farP.set(inF.x * inF.w,inF.y * inF.w,inF.z * inF.w);
		return 1;
	}
}
}