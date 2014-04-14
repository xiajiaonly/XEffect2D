//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBasic3D.h"
#include "glut.h"
#include "X3DWorld.h"
#include "XBasicWindow.h"
//注意：这里需要优化掉
//glPushAttrib(GL_ALL_ATTRIB_BITS);
//glMatrixMode(GL_MODELVIEW);
//glPushMatrix();
//这样的代码

void drawLine(const _XVector3 &ps,const _XVector3 &pe,const _XFColor & color)
{
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
//	glLoadIdentity();

	//glDisable(GL_TEXTURE_2D);
	if(XEE::isLineSmooth != 0)
	{
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glColor4fv(color);
	glLineWidth(1);
	glBegin(GL_LINES);
		glVertex3f(ps.x,ps.y,ps.z);
		glVertex3f(pe.x,pe.y,pe.z);
	glEnd();

	glDisable(GL_BLEND);
//	glEnable(GL_TEXTURE_2D);

	//glPopMatrix();
	//glPopAttrib();
}
void drawLine(const _XVector3 &ps,const _XVector3 &n,float len,const _XFColor &color)
{
	_XVector3 pe = ps + (n * len);
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
//	glLoadIdentity();

	//glDisable(GL_TEXTURE_2D);
	if(XEE::isLineSmooth != 0)
	{
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glColor4fv(color);
	glLineWidth(1);
	glBegin(GL_LINES);
		glVertex3f(ps.x,ps.y,ps.z);
		glVertex3f(pe.x,pe.y,pe.z);
	glEnd();

	glDisable(GL_BLEND);
//	glEnable(GL_TEXTURE_2D);

	//glPopMatrix();
	//glPopAttrib();
}
void drawTriangle(const _XVector3 &p0,const _XVector3 &p1,const _XVector3 &p2,const _XFColor & color)
{
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_CULL_FACE);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
//	glLoadIdentity();

//	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glColor4fv(color);
	//glLineWidth(1);
	_XVector3 normal = getNormal(p0,p1,p2);
	glBegin(GL_TRIANGLES);
		glNormal3f(normal.x,normal.y,normal.z);
		glVertex3f(p0.x,p0.y,p0.z);
		glVertex3f(p1.x,p1.y,p1.z);
		glVertex3f(p2.x,p2.y,p2.z);
	glEnd();

	glDisable(GL_BLEND);
//	glEnable(GL_TEXTURE_2D);

	//glPopMatrix();
	//glPopAttrib();
}
void drawTriangle(const _XVector3 &p0,const _XVector3 &p1,const _XVector3 &p2,
				  const _XVector3 &n,const _XFColor & color)
{
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
//	glLoadIdentity();

//	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glColor4fv(color);
	//glLineWidth(1);
	glBegin(GL_TRIANGLES);
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(p0.x,p0.y,p0.z);
		glVertex3f(p1.x,p1.y,p1.z);
		glVertex3f(p2.x,p2.y,p2.z);
	glEnd();

	glDisable(GL_BLEND);
//	glEnable(GL_TEXTURE_2D);

	//glPopMatrix();
	//glPopAttrib();
}
void drawPlane(const _XVector3 &p0,const _XVector3 &p1,const _XVector3 &p2,const _XVector3 &p3,
					  const _XVector3 &n,const _XFColor & color)
{
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
//	glLoadIdentity();

//	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glColor4fv(color);
	//glLineWidth(1);
	glBegin(GL_QUADS);
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(p0.x,p0.y,p0.z);
		glVertex3f(p1.x,p1.y,p1.z);
		glVertex3f(p2.x,p2.y,p2.z);
		glVertex3f(p3.x,p3.y,p3.z);
	glEnd();

	glDisable(GL_BLEND);
//	glEnable(GL_TEXTURE_2D);

	//glPopMatrix();
	//glPopAttrib();
}
void drawBox(GLfloat size)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
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

  for(i = 5;i >= 0;--i) 
  {
    glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);
    glVertex3fv(&v[faces[i][0]][0]);
    glVertex3fv(&v[faces[i][1]][0]);
    glVertex3fv(&v[faces[i][2]][0]);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}
void drawBox(const _XVector3 &center,const _XVector3 &size,const _XVector3 &angle,const _XFColor & color)
{
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
//	glLoadIdentity();

	glTranslatef(center.x,center.y,center.z);
    glRotatef(angle.x,1,0,0);
    glRotatef(angle.y,0,1,0);
    glRotatef(angle.z,0,0,1);

	//_X3DWorld::GetInstance().m_worldMaterial.setAmblient(0.0f,0.0f,1.0f,0.5f);
	//_X3DWorld::GetInstance().m_worldMaterial.setDiffuse(0.0f,0.0f,1.0f,0.5f);
	//_X3DWorld::GetInstance().m_worldMaterial.setSpecular(0.0f,0.0f,1.0f,0.5f);
	_X3DWorld::GetInstance().m_worldMaterial.usetMaterial();
	_X3DWorld::GetInstance().useShadow(XFalse);
	//glEnable(GL_DEPTH_TEST)
	//glDepthFunc(GL_NICEST)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	_X3DWorld::GetInstance().removeShadow();

	glDisable(GL_BLEND);

	glPopMatrix();
	//glPopAttrib();
}
void drawBall(const _XVector3 &center,float r,const _XVector3 &angle,const _XFColor & color)
{
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_MODELVIEW);  
	glPushMatrix();
//	glLoadIdentity();  

	glTranslatef(center.x,center.y,center.z);
    glRotatef(angle.x,1,0,0);
    glRotatef(angle.y,0,1,0);
    glRotatef(angle.z,0,0,1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_X3DWorld::GetInstance().useShadow(XFalse);
	glColor4fv(color);
	glutSolidSphere(r,30,30); 
	//GLUquadricObj *quad;
	//quad = gluNewQuadric();
	//gluQuadricTexture( quad, true );	//enable TexCoord generation
	//gluSphere(quad,r,30,30);

	_X3DWorld::GetInstance().removeShadow();
	glDisable(GL_BLEND);

	glPopMatrix();
	//glPopAttrib();
}
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

	_X3DWorld::GetInstance().useShadow(XFalse);
	int x,z;
	_XVector3 p0,p1,p2,p3; //顺时针
	_XVector3 normal;
	glBegin(GL_TRIANGLES);
	for(x = 0;x < w;++ x)
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

			normal = getNormal(p0,p1,p2);
			glNormal3f(normal.x,normal.y,normal.z);
			glVertex3f(p0.x,p0.y,p0.z);
			glVertex3f(p1.x,p1.y,p1.z);
			glVertex3f(p2.x,p2.y,p2.z);

			normal = getNormal(p0,p2,p3);
			glNormal3f(normal.x,normal.y,normal.z);
			glVertex3f(p0.x,p0.y,p0.z);
			glVertex3f(p2.x,p2.y,p2.z);
			glVertex3f(p3.x,p3.y,p3.z);
		}
	}
	glEnd();
	_X3DWorld::GetInstance().removeShadow();
	//glDisable(GL_BLEND);
   // glEnable(GL_TEXTURE_2D);

	glPopMatrix();
}
//未测试
void drawArray(const float *p,int w,int h,unsigned int tex)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
//	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
//	glDisable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);	//光照与纹理进行混合
	glBindTexture(GL_TEXTURE_2D,tex);

	int x,z;
	_XVector2 u0,u1,u2,u3;
	_XVector3 p0,p1,p2,p3; //顺时针
	_XVector3 normal;
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

			normal = getNormal(p0,p1,p2);
			glNormal3f(normal.x,normal.y,normal.z);
			glTexCoord2f(u0.x,u0.y);
			glVertex3f(p0.x,p0.y,p0.z);
			glTexCoord2f(u1.x,u1.y);
			glVertex3f(p1.x,p1.y,p1.z);
			glTexCoord2f(u2.x,u2.y);
			glVertex3f(p2.x,p2.y,p2.z);

			normal = getNormal(p0,p2,p3);
			glNormal3f(normal.x,normal.y,normal.z);
			glTexCoord2f(u0.x,u0.y);
			glVertex3f(p0.x,p0.y,p0.z);
			glTexCoord2f(u2.x,u2.y);
			glVertex3f(p2.x,p2.y,p2.z);
			glTexCoord2f(u3.x,u3.y);
			glVertex3f(p3.x,p3.y,p3.z);
		}
	}
	glEnd();

	//glDisable(GL_BLEND);
    //glEnable(GL_TEXTURE_2D);

	glPopMatrix();
}
//实现gluLookAt的功能
_XMatrix4x4 calLookAtMatrix(const _XVector3 &eyePos,const _XVector3 &LookAtPos,const _XVector3 &up)
{
	_XVector3 zaxis = normalize(eyePos - LookAtPos);
	_XVector3 xaxis = normalize(up * zaxis);
	_XVector3 yaxis = zaxis * xaxis;

	return _XMatrix4x4(xaxis.x, yaxis.x, zaxis.x, 0.0f,
					xaxis.y, yaxis.y, zaxis.y, 0.0f,
					xaxis.z, yaxis.z, zaxis.z, 0.0f,
					-(xaxis.dot(eyePos)), -(yaxis.dot(eyePos)), -(zaxis.dot(eyePos)),1.0f);
}
_XMatrix4x4 calPerspectiveMatrix(float fovy,float rat,float zNear,float zFar)
{
	float tan_fovy = tan(fovy * 0.5f * ANGLE_TO_RADIAN);
	float right = tan_fovy * rat * zNear;
	float left = -right;
	float top = tan_fovy * zNear;
	float bottom = -top;
	return _XMatrix4x4(	2.0*zNear/(right-left),	0.0f,					(right+left)/(right-left),		0.0f,
						0.0f,					2.0*zNear/(top-bottom), (top+bottom)/(top-bottom),		0.0f,
						0.0f,					0.0f,					-(zFar+zNear)/(zFar-zNear),		-1.0f,
						0.0f,					0.0f,					-2.0*zFar*zNear/(zFar-zNear),	0.0f);
	//根据网上的资料正确的应该是下面的，但是实际测试中好像结果应该是上面的
	//return _XMatrix4x4(	2.0*zNear/(right-left),	0.0f,					(right+left)/(right-left),		0.0f,
	//					0.0f,					2.0*zNear/(top-bottom), (top+bottom)/(top-bottom),		0.0f,
	//					0.0f,					0.0f,					-(zFar+zNear)/(zFar-zNear),		-2.0*zFar*zNear/(zFar-zNear),
	//					0.0f,					0.0f,					-1.0f,							0.0f);
}
_XMatrix4x4 calOrthoMatrix(float left,float right,float bottom,float top,float zNear,float zFar)
{//没有经过测试
 	return _XMatrix4x4(	2.0f/(right-left),	0.0f,				0.0f,				-(right+left)/(right-left),
						0.0f,				2.0f/(top-bottom),	0.0f,				-(top+bottom)/(top-bottom),
						0.0f,				0.0f,				-2.0/(zFar-zNear),	-(zFar+zNear)/(zFar-zNear),
						0.0f,				0.0f,				0.0f,				1.0f);
}
_XMatrix4x4 calOrtho2DMatrix(float left,float right,float bottom,float top)
{//没有经过测试
	return calOrthoMatrix(left, right, bottom, top, -1.0, 1.0);
}
_XMatrix4x4 calFrustumMatrix(float left,float right,float bottom,float top,float zNear,float zFar)
{//没有经过测试
	return _XMatrix4x4(	2.0*zNear/(right-left),	0.0f,					(right+left)/(right-left),		0.0f,
						0.0f,					2.0*zNear/(top-bottom), (top+bottom)/(top-bottom),		0.0f,
						0.0f,					0.0f,					-(zFar+zNear)/(zFar-zNear),		-2.0*zFar*zNear/(zFar-zNear),
						0.0f,					0.0f,					-1.0f,							0.0f);

}
bool getOrtho(const _XMatrix4x4& mtrx,float& left,float& right,float& bottom,float& top,float& zNear,float& zFar)
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
bool getFrustum(const _XMatrix4x4& mtrx,float& left,float& right,float& bottom,float& top,float& zNear,float& zFar)
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
bool getPerspective(const _XMatrix4x4& mtrx,float& fovy,float& aspectRatio,float& zNear, float& zFar)
{
	float right  =  0.0f;
    float left   =  0.0f;
    float top    =  0.0f;
    float bottom =  0.0f;
    if (getFrustum(mtrx,left,right,bottom,top,zNear,zFar))
    {
        fovy = (atan(top/zNear)-atan(bottom/zNear))*RADIAN_TO_ANGLE;
        aspectRatio = (right-left)/(top-bottom);
        return true;
    }
    return false;
}
void getLookAt(const _XMatrix4x4& mtrx,_XVector3& eye,_XVector3& center,_XVector3& up,float lookDistance)
{
	_XMatrix4x4 inv = mtrx;
	inv = inv.inverse();
    eye = inv * _XVector3(0.0f,0.0f,0.0f);
    up = transform4x4(mtrx,_XVector3(0.0,1.0,0.0));
    center = transform4x4(mtrx,_XVector3(0.0,0.0,-1));
    normalize(center);
    center = eye + center * lookDistance;
}
_XMatrix4x4 getRotate(float angle,_XVector3 v)
{
	float sinA = sin(angle * ANGLE_TO_RADIAN);
	float cosA = cos(angle * ANGLE_TO_RADIAN);
	float tCosA = 1 - cosA;
	v = normalize(v);
	return _XMatrix4x4( tCosA * v.x * v.x + cosA,		tCosA * v.x * v.y + v.z * sinA, tCosA * v.x * v.z - sinA * v.y, 0,
						tCosA * v.x * v.y - sinA * v.z,	tCosA * v.y * v.y + cosA,		tCosA * v.y * v.z + sinA * v.x, 0,
						tCosA * v.x * v.z + sinA * v.y,	tCosA * v.y * v.z - v.x * sinA,	tCosA * v.z * v.z + cosA,		0,
						0,0,0,1);
}
_XMatrix4x4 getRotate(const _XVector3 &a)
{
	_XMatrix4x4 ret = getRotateX(a.x);
	ret = ret * getRotateY(a.y);
	ret = ret * getRotateZ(a.z);
	return ret;
}
_XMatrix4x4 getRotateZ(float angle)
{
	float sinA = sin(angle * ANGLE_TO_RADIAN);
	float cosA = cos(angle * ANGLE_TO_RADIAN);
	return _XMatrix4x4(	cosA,sinA,0,0,
						-sinA,cosA,0,0,
						0,0,1,0,
						0,0,0,1);
}
_XMatrix4x4 getRotateX(float angle)
{
	float sinA = sin(angle * ANGLE_TO_RADIAN);
	float cosA = cos(angle * ANGLE_TO_RADIAN);
	return _XMatrix4x4(	1,0,0,0,
						0,cosA,sinA,0,
						0,-sinA,cosA,0,
						0,0,0,1);
}
_XMatrix4x4 getRotateY(float angle)
{
	float sinA = sin(angle * ANGLE_TO_RADIAN);
	float cosA = cos(angle * ANGLE_TO_RADIAN);
	return _XMatrix4x4(	cosA,0,-sinA,0,
						0,1,0,0,
						sinA,0,cosA,0,
						0,0,0,1);
}
_XMatrix4x4 getTranslate(const _XVector3 &p)
{
	return _XMatrix4x4(	1,0,0,0,
						0,1,0,0,
						0,0,1,0,
						p.x,p.y,p.z,1);
}
_XMatrix4x4 getScale(const _XVector3 &s)
{
	return _XMatrix4x4( s.x,0,	0,	0,
						0,	s.y,0,	0,
						0,	0,	s.z,0,
						0,	0,	0,	1);
}
void drawOrigin()
{
	drawLine(_XVector3(0.0f,0.0f,0.0f),
		_XVector3(100.0f,0.0f,0.0f),_XFColor(1.0f,0.0f,0.0f,1.0f));
	drawLine(_XVector3(0.0f,0.0f,0.0f),
		_XVector3(0.0f,100.0f,0.0f),_XFColor(0.0f,1.0f,0.0f,1.0f));
	drawLine(_XVector3(0.0f,0.0f,0.0f),
		_XVector3(0.0f,0.0f,100.0f),_XFColor(0.0f,0.0f,1.0f,1.0f));
}
float canPickingByRay(const _XVector3 &nearP,const _XVector3 &farP,_XVector3 *v,const _XVector3 &n,_XVector3 &out)
{
	//static _XVector3 l;
	//l = farP - nearP;
	//float nDotL = n ^ l;
	//if(nDotL >= 0.0f) return -1.0f;
	//_XVector3 a = v[0];
	//float d = (n ^ (a - nearP)) / nDotL;
	//if(d < 0.0f || d > 1.0f) return -1.0f;
	//_XVector3 p = nearP + l * d;	//射线与平面的焦点(d 是距离)
	//_XVector3 b = v[1];
	//_XVector3 c = v[2];

	//_XVector3 n1 = (b - a) * (p - a);
	//_XVector3 n2 = (c - b) * (p - b);
	//_XVector3 n3 = (a - c) * (p - c);
	//if ((n ^ n1) >= 0.0f &&
 //       (n ^ n2) >= 0.0f &&
 //       (n ^ n3) >= 0.0f)
 //   {//三角面与射线相交
	//	out = p;	//交点
	//	return d;
	//}
	//return -1.0f;
	//对上面的算法进行优化
	static _XVector3 l;
	l = farP - nearP;
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
int getUnProject(float x,float y,float z,const _XMatrix4x4& modelview,const _XMatrix4x4 &projection,
				  const _XVector4 &viewport,_XVector3 &out)
{
	_XMatrix4x4 m = (projection * modelview).inverse();
	_XVector4 in;
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
int getUnProject(const _XVector2 &pos,_XVector3 &nearP,_XVector3 &farP)
{
	//_XMatrix4x4 m = (_X3DWorld::GetInstance().m_worldCam.getProjectMatrix() * _X3DWorld::GetInstance().m_worldCam.getViewMatrix()).inverse();
	_XMatrix4x4 m = (_X3DWorld::GetInstance().m_worldCam.getProjXView()).inverse();
	_XVector4 inN,inF;
	inN.x = inF.x = pos.x / XEE::windowData.w * 2.0f - 1.0f;
	inN.y = inF.y = pos.y / XEE::windowData.h * 2.0f - 1.0f;
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