#include "XStdHead.h"
#include "XMesh2D.h"
#include "XMath\XMatrix2x2.h"
namespace XE{
void XMesh2D::drawWireframe()const	//描绘线框
{
	if(!m_isInited) return;
	if(m_v.size() <= 0) return;	//如果没有数据则直接返回
	XGL::DisableTexture2D();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_position.x + m_center.x,m_position.y + m_center.y, 0.0f);

	if(m_angle != 0.0f) glRotatef(m_angle, 0.0f, 0.0f, 1.0f);	//设置未指定的角度
	if(m_scale.x != 1.0f || m_scale.y != 1.0f) glScalef(m_scale.x, m_scale.y, 0.0f);	//设置缩放的比例

	int index;
	glLineWidth(1.0f);
	glColor4fv(m_color);	//设置为指定的颜色
	glLineWidth(1.0f); 
	glBegin(GL_LINES);
	for(int i = 0;i < m_sumY;++ i)
	{
		index = i * m_sumX;
		for(int j = 0;j < m_sumX;++ j,++ index)
		{
			if(j + 1 < m_sumX)
			{
				glVertex2fv(m_v[index] - m_center);
				glVertex2fv(m_v[index + 1] - m_center);
			}
			if(i + 1 < m_sumY)
			{
				glVertex2fv(m_v[index] - m_center);
				glVertex2fv(m_v[index + m_sumX] - m_center);
			}
		}
	}
	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void XMesh2D::drawWireframeEx(bool special,bool withPoint)const	//描绘线框
{
	if(!m_isInited) return;
	if(m_v.size() <= 0) return;	//如果没有数据则直接返回
	XGL::DisableTexture2D();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_position.x + m_center.x,m_position.y + m_center.y, 0.0f);

	if(m_angle != 0.0f) glRotatef(m_angle, 0.0f, 0.0f, 1.0f);	//设置未指定的角度
	if(m_scale.x != 1.0f || m_scale.y != 1.0f) glScalef(m_scale.x, m_scale.y, 0.0f);	//设置缩放的比例

	int index;
	if(special) glColor4fv(XFColor::yellow);	//设置为指定的颜色
	else glColor4fv(XFColor::blue);
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for(int i = 0;i < m_sumY;++ i)
	{
		index = i * m_sumX;
		for(int j = 0;j < m_sumX;++ j,++ index)
		{
			if(j + 1 < m_sumX)
			{
				glVertex2fv(m_v[index] - m_center);
				glVertex2fv(m_v[index + 1] - m_center);
			}
			if(i + 1 < m_sumY)
			{
				glVertex2fv(m_v[index] - m_center);
				glVertex2fv(m_v[index + m_sumX] - m_center);
			}
		}
	}
	glEnd();
	if(special && withPoint)
	{
		//描绘节点
		glColor4fv(XFColor::green);	//设置为指定的颜色
		glPointSize(7.0f);
		glBegin(GL_POINTS);
		for(auto it = m_v.begin();it != m_v.end();++ it)
		{
			glVertex2fv((*it) - m_center);
		}
		glEnd();
		//描绘特殊的节点以及行或者列
		if(m_specialLine >= 0 && m_specialLine < m_sumY)
		{
			glColor4fv(XFColor::blue);	//设置为指定的颜色
			glPointSize(20.0f);
			glBegin(GL_POINTS);
			for(int i = 0;i < m_sumX;++ i)
			{
				glVertex2fv(m_v[m_specialLine * m_sumX + i] - m_center);
			}
			glEnd();
		}
		if(m_specialRow >= 0 && m_specialRow < m_sumX)
		{
			glColor4fv(XFColor::blue);	//设置为指定的颜色
			glPointSize(20.0f);
			glBegin(GL_POINTS);
			for(int i = 0;i < m_sumY;++ i)
			{
				glVertex2fv(m_v[m_specialRow + i * m_sumX] - m_center);
			}
			glEnd();
		}
		if(m_specialIndex >= 0 && m_specialIndex < m_v.size())
		{
			glColor4fv(XFColor::green);	//设置为指定的颜色
			glPointSize(30.0f);
			glBegin(GL_POINTS);
				glVertex2fv(m_v[m_specialIndex] - m_center);
			glEnd();
		}
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void XMesh2D::drawTexture(unsigned int tex,XShaderGLSL *pShader)const
{
	if(!m_isInited || tex == 0) return;
	if(m_v.size() <= 0) return;	//如果没有数据则直接返回
	if(!m_withTexture)
	{
		drawWireframe();
		return;
	}
	if(pShader == NULL)
	{
		XGL::EnableTexture2D();
		XGL::BindTexture2D(tex);
	}else
	{
		pShader->useShaderEx(tex);
	}
	XGL::setBlendAlpha();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_position.x + m_center.x,m_position.y + m_center.y, 0.0f);
//	glTranslatef(m_position.x,m_position.y, 0.0f);

	if(m_angle != 0.0f) glRotatef(m_angle, 0.0f, 0.0f, 1.0f);	//设置未指定的角度
	if(m_scale.x != 1.0f || m_scale.y != 1.0f) glScalef(m_scale.x, m_scale.y, 0.0f);	//设置缩放的比例

	glColor4fv(m_color);	//设置为指定的颜色
	//下面开始描绘
	int index;
	for(int i = 0;i < m_sumY - 1;++ i)
	{
		index = i * m_sumX;
		glBegin(GL_TRIANGLE_STRIP);
		for(int j = 0;j < m_sumX;++ j,++ index)
		{
			if(m_withColor) 
				glColor4fv(m_c[index]);
			glTexCoord2fv(m_t[index]);
			glVertex2fv(m_v[index] - m_center);
			if(m_withColor) 
				glColor4fv(m_c[index + m_sumX]);
			glTexCoord2fv(m_t[index + m_sumX]);
			glVertex2fv(m_v[index + m_sumX] - m_center);
		}
		glEnd();
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	XGL::DisableBlend();
	if(pShader != NULL)
		pShader->disShader();
}
bool XMesh2D::saveToFile(const char *filename)const		//将数据保存到文件，建议使用OBJ的数据格式
{
	if(!m_isInited || filename == NULL) return false;
	FILE *fp;
	if((fp = fopen(filename,"w")) == NULL) return false;
	fprintf(fp,"%d,%d,\n",m_sumX,m_sumY);
	if(m_withTexture) fprintf(fp,"1,");
	else fprintf(fp,"0,");
	if(m_withColor) fprintf(fp,"1,\n");
	else fprintf(fp,"0,\n");
	//一些特性
	fprintf(fp,"%f,%f,\n",m_position.x,m_position.y);
	fprintf(fp,"%f,\n",m_angle);
	fprintf(fp,"%f,%f,\n",m_scale.x,m_scale.y);
	fprintf(fp,"%f,%f,\n",m_center.x,m_center.y);
	//写入顶点数据
	for(auto it = m_v.begin();it != m_v.end();++ it)
	{
		fprintf(fp,"%f,%f,\n",it->x,it->y);
	}
	if(m_withTexture)
	{
		for (auto it = m_t.begin(); it != m_t.end(); ++it)
		{
			fprintf(fp,"%f,%f,\n",it->x,it->y);
		}
	}
	if(m_withColor)
	{
		for (auto it = m_c.begin(); it != m_c.end(); ++it)
		{
			fprintf(fp,"%f,%f,%f,%f,\n",it->r, it->g, it->b, it->a);
		}
	}
	fclose(fp);
	return true;
}
bool XMesh2D::saveToFileB(const char *filename)const
{
	if (!m_isInited || filename == NULL) return false;
	FILE *fp;
	if ((fp = fopen(filename, "wb")) == NULL) return false;
	fwrite(&m_sumX, sizeof(m_sumX), 1, fp);
	fwrite(&m_sumY, sizeof(m_sumY), 1, fp);
	fwrite(&m_withTexture, sizeof(m_withTexture), 1, fp);
	fwrite(&m_withColor, sizeof(m_withColor), 1, fp);
	//一些特性
	fwrite(&m_position.x, sizeof(m_position.x), 1, fp);
	fwrite(&m_position.y, sizeof(m_position.y), 1, fp);
	fwrite(&m_angle, sizeof(m_angle), 1, fp);
	fwrite(&m_scale.x, sizeof(m_scale.x), 1, fp);
	fwrite(&m_scale.y, sizeof(m_scale.y), 1, fp);
	fwrite(&m_center.x, sizeof(m_center.x), 1, fp);
	fwrite(&m_center.y, sizeof(m_center.y), 1, fp);
	//写入顶点数据
	for (auto it = m_v.begin(); it != m_v.end(); ++it)
	{
		fwrite(&it->x, sizeof(it->x), 1, fp);
		fwrite(&it->y, sizeof(it->y), 1, fp);
	}
	if (m_withTexture)
	{
		for (auto it = m_t.begin(); it != m_t.end(); ++it)
		{
			fwrite(&it->x, sizeof(it->x), 1, fp);
			fwrite(&it->y, sizeof(it->y), 1, fp);
		}
	}
	if (m_withColor)
	{
		for (auto it = m_c.begin(); it != m_c.end(); ++it)
		{
			fwrite(&it->r, sizeof(it->r), 1, fp);
			fwrite(&it->g, sizeof(it->g), 1, fp);
			fwrite(&it->b, sizeof(it->b), 1, fp);
			fwrite(&it->a, sizeof(it->a), 1, fp);
		}
	}
	fclose(fp);
	return true;
}
bool XMesh2D::loadFromFile(const char *filename)	//从文件中读取数据
{
	if(filename == NULL) return false;
	FILE *fp;
	int tmpFlag[2] = {0,0};
	if((fp = fopen(filename,"r")) == NULL) return false;
	clearAllData();
	if(fscanf(fp,"%d,%d,\n",&m_sumX,&m_sumY) != 2) return false;
	if(fscanf(fp,"%d,%d,\n",&tmpFlag[0],&tmpFlag[1]) != 2) return false;
	m_withTexture = (tmpFlag[0] != 0);
	m_withColor = (tmpFlag[1] != 0);
	if(fscanf(fp,"%f,%f,\n",&m_position.x,&m_position.y) != 2) return false;
	if(fscanf(fp,"%f,\n",&m_angle) != 1) return false;
	if(fscanf(fp,"%f,%f,\n",&m_scale.x,&m_scale.y) != 2) return false;
	if(fscanf(fp,"%f,%f,\n",&m_center.x,&m_center.y) != 2) return false;
	//写入顶点数据
	XVec2 tmp;
	for(int i = 0;i < m_sumX * m_sumY;++ i)
	{
		if(fscanf(fp,"%f,%f,\n",&tmp.x,&tmp.y) != 2) return false;
		m_v.push_back(tmp);
	}
	if(m_withTexture)
	{
		for(int i = 0;i < m_sumX * m_sumY;++ i)
		{
			if(fscanf(fp,"%f,%f,\n",&tmp.x,&tmp.y) != 2) return false;
			m_t.push_back(tmp);
		}
	}
	if(m_withColor)
	{
		XFColor tmpC;
		for(int i = 0;i < m_sumX * m_sumY;++ i)
		{
			if(fscanf(fp,"%f,%f,%f,%f,\n",&tmpC.r,&tmpC.g,&tmpC.b,&tmpC.a) != 4) return false;
			m_c.push_back(tmpC);
		}
	}
	fclose(fp);
	return true;
}
bool XMesh2D::loadFromFileB(const char *filename)	//从文件中读取数据
{
	if (filename == NULL) return false;
	FILE *fp;
//	int tmpFlag[2] = { 0,0 };
	if ((fp = fopen(filename, "rb")) == NULL) return false;
	clearAllData();
	fread(&m_sumX, sizeof(m_sumX), 1, fp);
	fread(&m_sumY, sizeof(m_sumY), 1, fp);
	fread(&m_withTexture, sizeof(m_withTexture), 1, fp);
	fread(&m_withColor, sizeof(m_withColor), 1, fp);
	fread(&m_position.x, sizeof(m_position.x), 1, fp);
	fread(&m_position.y, sizeof(m_position.y), 1, fp);
	fread(&m_angle, sizeof(m_angle), 1, fp);
	fread(&m_scale.x, sizeof(m_scale.x), 1, fp);
	fread(&m_scale.y, sizeof(m_scale.y), 1, fp);
	fread(&m_center.x, sizeof(m_center.x), 1, fp);
	fread(&m_center.y, sizeof(m_center.y), 1, fp);
	//写入顶点数据
	XVec2 tmp;
	m_v.resize(m_sumX * m_sumY);
	for (int i = 0; i < m_v.size(); ++i)
	{
		fread(&m_v[i].x, sizeof(m_v[i].x), 1, fp);
		fread(&m_v[i].y, sizeof(m_v[i].y), 1, fp);
	}
	if (m_withTexture)
	{
		m_t.resize(m_sumX * m_sumY);
		for (int i = 0; i < m_t.size(); ++i)
		{
			fread(&m_t[i].x, sizeof(m_t[i].x), 1, fp);
			fread(&m_t[i].y, sizeof(m_t[i].y), 1, fp);
		}
	}
	if (m_withColor)
	{
		m_c.resize(m_sumX * m_sumY);
		for (int i = 0; i < m_c.size(); ++i)
		{
			fread(&m_c[i].r, sizeof(m_c[i].r), 1, fp);
			fread(&m_c[i].g, sizeof(m_c[i].g), 1, fp);
			fread(&m_c[i].b, sizeof(m_c[i].b), 1, fp);
			fread(&m_c[i].a, sizeof(m_c[i].a), 1, fp);
		}
	}
	fclose(fp);
	return true;
}
bool XMesh2D::init(const XMesh2DInfo &info)
{
	if(m_isInited) return false;
	if(info.w <= 0 || info.h <= 0 || info.sumX <= 1 || info.sumY <= 1) return false;	//输入数据不合法
	m_sumX = info.sumX;
	m_sumY = info.sumY;
	m_withTexture = info.withT;
	m_withColor = info.withC;
	clearAllData();
	XVec2 size = XVec2(info.w,info.h) / XVec2(m_sumX - 1,m_sumY - 1);
	XVec2 sizeT = XVec2(info.tw,info.th) / XVec2(m_sumX - 1,m_sumY - 1);
	for(int i = 0;i < m_sumY;++ i)
	{
		for(int j = 0;j < m_sumX;++ j)
		{
			m_v.push_back(size * XVec2(j,i));
			if(m_withTexture)
				m_t.push_back(XVec2(info.tSx + sizeT.x * j,info.tSy + sizeT.y * i));
			if(m_withColor)
				m_c.push_back(XFColor::white);
		}
	}
	m_isInited = true;
	return true;
}
void XMesh2D::setTextureInfo(float sx,float sy,float w,float h)
{
	if(!m_isInited || !m_withTexture) return;
	XVec2 sizeT = XVec2(w,h) / XVec2(m_sumX - 1,m_sumY - 1);
	m_t.clear();
	for(int i = 0;i < m_sumY;++ i)
	{
		for(int j = 0;j < m_sumX;++ j)
		{
			m_t.push_back(XVec2(sx + sizeT.x * j,sy + sizeT.y * i));
		}
	}
}
int XMesh2D::getNearestVertex(const XVec2& pos,float maxD)const	//获取离pos最近的顶点的编号，距离小于maxD
{
	//从local空间到world空间
//	XMat2 tmp = XMath::getMatrix2D(m_angle * DEGREE2RADIAN) * XMath::getMatrix2D(m_scale);
//	XVec2 ret = (pos - m_center) * tmp + m_position + m_center;
//	XRender::drawPoint(ret,4,1,1,1,1);
	//从world空间到local空间
//	XMat2 tmp = XMath::getMatrix2D(m_angle * DEGREE2RADIAN) * XMath::getMatrix2D(m_scale);
//	XVec2 ret = tmp.inverse() * (pos - m_center - m_position) + m_center;
//	printf("%f,%f\n",ret.x,ret.y);
	if(m_v.size() <= 0) return -1;	//无效的数据
	//将pos从世界空间映射到mesh的local空间
	XVec2 local = world2Local(pos);
	//遍历所有的顶点寻找距离最近的顶点
	float minD = m_v[0].getLengthSqure(local);
	int index = 0;
	float tmpD;
	for(int i = 1;i < m_v.size();++ i)
	{
		tmpD = m_v[i].getLengthSqure(local);
		if(tmpD < minD)
		{
			minD = tmpD;
			index = i;
		}
	}
	if(minD <= maxD * maxD) return index;
	else return -1;
}
void XMesh2D::setVertexPos(const XVec2& pos,int index)	//设置顶点的位置
{
	if(index < 0 || index >= m_v.size()) return;	//非法的数据
	m_v[index] = world2Local(pos);
}
XVec2 XMesh2D::getVertexPos(int index)const
{
	if(index < 0 || index >= m_v.size()) return XVec2::zero;	//非法的数据
	return local2World(m_v[index]);
}
void XMesh2D::setVertexColor(const XFColor& color,int index)
{
	if(index < 0 || index >= m_c.size()) return;	//非法的数据
	m_c[index] = color;
}
void XMesh2D::setVertexTex(const XVec2& t,int index)
{
	if(index < 0 || index >= m_t.size()) return;	//非法的数据
	m_t[index] = t;
}
bool XMesh2D::mapVertex2Tex(const XVec2& pos,XVec2& out)const
{
	if(!m_withTexture) return false;
	XVec2 inPos = world2Local(pos);
	XVec2 inRect[4];
	XVec2 outRect[4];
	for(int h = 0;h < m_sumY - 1;++ h)
	{
		int index = h * m_sumX;
		for(int w = 0;w < m_sumX - 1;++ w,++index)
		{
			//方案1：四边形比较
			//inRect[0] = m_v[index];
			//inRect[1] = m_v[index + 1];
			//inRect[2] = m_v[index + m_sumX + 1];
			//inRect[3] = m_v[index + m_sumX];
			//if(!XMath::getIsInPolygon(inPos,inRect,4)) continue;
			//outRect[0] = m_t[index];
			//outRect[1] = m_t[index + 1];
			//outRect[2] = m_t[index + m_sumX + 1];
			//outRect[3] = m_t[index + m_sumX];
			//out = XMath::mapping2DEx(inRect,outRect,inPos);
			//return true;
			//方案2：三角形计算
			inRect[0] = m_v[index];
			inRect[1] = m_v[index + 1];
			inRect[2] = m_v[index + m_sumX + 1];
			if (XMath::getIsInTriangle(inPos, inRect))
			{
				outRect[0] = m_t[index];
				outRect[1] = m_t[index + 1];
				outRect[2] = m_t[index + m_sumX + 1];
				out = XMath::mapping2DTriangle(inRect, outRect, inPos);
				return true;
			}
			inRect[1] = m_v[index + m_sumX];
			if (XMath::getIsInTriangle(inPos, inRect))
			{
				outRect[0] = m_t[index];
				outRect[1] = m_t[index + m_sumX];
				outRect[2] = m_t[index + m_sumX + 1];
				out = XMath::mapping2DTriangle(inRect, outRect, inPos);
				return true;
			}
		}
	}
	return false;
}
bool XMesh2D::mapTex2Vertex(const XVec2& pos, XVec2& out)const
{
	if (!m_withTexture) return false;
	XVec2 inPos = pos;// world2Local(pos);
	XVec2 inRect[4];
	XVec2 outRect[4];
	for (int h = 0; h < m_sumY - 1; ++h)
	{
		int index = h * m_sumX;
		for (int w = 0; w < m_sumX - 1; ++w,++index)
		{
			//方案1：四边形比较
			//inRect[0] = m_t[index];
			//inRect[1] = m_t[index + 1];
			//inRect[2] = m_t[index + m_sumX + 1];
			//inRect[3] = m_t[index + m_sumX];
			//if (!XMath::getIsInPolygon(inPos, inRect, 4)) continue;
			//outRect[0] = m_v[index];
			//outRect[1] = m_v[index + 1];
			//outRect[2] = m_v[index + m_sumX + 1];
			//outRect[3] = m_v[index + m_sumX];
			//out = XMath::mapping2DEx(inRect, outRect, inPos);
			//return true;
			//方案2：三角形计算
			inRect[0] = m_t[index];
			inRect[1] = m_t[index + 1];
			inRect[2] = m_t[index + m_sumX + 1];
			if (XMath::getIsInTriangle(inPos, inRect))
			{
				outRect[0] = m_v[index];
				outRect[1] = m_v[index + 1];
				outRect[2] = m_v[index + m_sumX + 1];
				out = XMath::mapping2DTriangle(inRect, outRect, inPos);
				return true;
			}
			inRect[1] = m_t[index + m_sumX];
			if (XMath::getIsInTriangle(inPos, inRect))
			{
				outRect[0] = m_v[index];
				outRect[1] = m_v[index + m_sumX];
				outRect[2] = m_v[index + m_sumX + 1];
				out = XMath::mapping2DTriangle(inRect, outRect, inPos);
				return true;
			}
		}
	}
	return false;
}
XVec2 XMesh2D::world2Local(const XVec2& pos)const
{
	return (XMath::getMatrix2D(m_angle * DEGREE2RADIAN) * XMath::getMatrix2D(m_scale)).inverse() * (pos - m_center - m_position) + m_center;
}
XVec2 XMesh2D::local2World(const XVec2& pos)const
{
	return (XMath::getMatrix2D(m_angle * DEGREE2RADIAN) * XMath::getMatrix2D(m_scale)) * (pos - m_center) + m_center + m_position;
}
}