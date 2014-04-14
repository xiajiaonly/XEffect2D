#include "glew.h"
#include "XCubeMap.h"

bool _XCubeMap::loadPic(const std::string & name,int index,_XResourcePosition resPos)
{
	_XPixels<_XCurPixel> tmp;
	if(!tmp.load(name,resPos)) return false;
	switch(tmp.getColorMode())
	{
	case COLOR_RGBA:
		glTexImage2D(index, 0, 3,tmp.getWidth(),tmp.getHeight(),
			0, GL_RGBA, GL_UNSIGNED_BYTE,tmp.getPixel());
		break;
	case COLOR_BGRA:
		glTexImage2D(index, 0, 3,tmp.getWidth(),tmp.getHeight(),
			0, GL_BGRA, GL_UNSIGNED_BYTE,tmp.getPixel());
		break;
	case COLOR_RGB:
		glTexImage2D(index, 0, 3,tmp.getWidth(),tmp.getHeight(),
			0, GL_RGB, GL_UNSIGNED_BYTE,tmp.getPixel());
		break;
	case COLOR_BGR:
		glTexImage2D(index, 0, 3,tmp.getWidth(),tmp.getHeight(),
			0, GL_BGR, GL_UNSIGNED_BYTE,tmp.getPixel());
		break;
	default:
		return false;
	}
	tmp.release();
	return true;
}

bool _XCubeMap::init(const std::string &path,_XResourcePosition resPos)
{
	if(m_isInited) return true;
	FILE *fp = NULL;
	std::string cfg = path + "/config.txt";
	if((fp = fopen(cfg.c_str(),"r")) == NULL) return false;
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1,&m_texture);
	glBindTexture (GL_TEXTURE_CUBE_MAP,m_texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	char name[1024];
	fscanf(fp,"%s\n",name);
	if(!loadPic(path + "/" + name,GL_TEXTURE_CUBE_MAP_POSITIVE_X,resPos)) return false;
	fscanf(fp,"%s\n",name);
	if(!loadPic(path + "/" + name,GL_TEXTURE_CUBE_MAP_NEGATIVE_X,resPos)) return false;
	fscanf(fp,"%s\n",name);
	if(!loadPic(path + "/" + name,GL_TEXTURE_CUBE_MAP_POSITIVE_Y,resPos)) return false;
	fscanf(fp,"%s\n",name);
	if(!loadPic(path + "/" + name,GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,resPos)) return false;
	fscanf(fp,"%s\n",name);
	if(!loadPic(path + "/" + name,GL_TEXTURE_CUBE_MAP_POSITIVE_Z,resPos)) return false;
	fscanf(fp,"%s\n",name);
	if(!loadPic(path + "/" + name,GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,resPos)) return false;
	glDisable(GL_TEXTURE_CUBE_MAP);
	fclose(fp);

	m_isInited = true;
	return true;
}
void _XCubeMap::draw()
{
	if(!m_isInited) return;
	static const GLfloat vertex[4*6][3] = 
	{
		//Positive X face.
		{ 3000,-3000, 3000},{ 3000, 3000, 3000},{ 3000, 3000,-3000},{ 3000,-3000,-3000},
		//Negative X face.
		{-3000,-3000,-3000},{-3000, 3000,-3000},{-3000, 3000, 3000},{-3000,-3000, 3000},
		//Positive Y face.
		{-3000, 3000,-3000},{ 3000, 3000,-3000},{ 3000, 3000, 3000},{-3000, 3000, 3000},
		//Negative Y face.
		{-3000,-3000, 3000},{ 3000,-3000, 3000},{ 3000,-3000,-3000},{-3000,-3000,-3000},
		//Positive Z face.
		{-3000, 3000, 3000},{ 3000, 3000, 3000},{ 3000,-3000, 3000},{-3000,-3000, 3000},
		//Negative Z face.
		{-3000,-3000,-3000},{ 3000,-3000,-3000},{ 3000, 3000,-3000},{-3000, 3000,-3000},
	};
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP,m_texture);
//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	     
	glBegin(GL_QUADS);
	for(int i = 0;i < 4*6;++ i)
	{
		glTexCoord3fv(vertex[i]);
		glVertex3fv(vertex[i]);
	}
	glEnd();
	 
	glDisable(GL_TEXTURE_CUBE_MAP);
}
void _XCubeMap::release()
{
	if(glIsTexture(m_texture)) 
	{
		printf("delete texture:%d\n",m_texture);
		glDeleteTextures(1, &m_texture);
	}
}