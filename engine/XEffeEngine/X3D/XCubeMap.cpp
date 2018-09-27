#include "XStdHead.h"
#include "XCubeMap.h"
#include "XResourcePack.h"
namespace XE{
bool XCubeMap::loadPic(const std::string& name,int index,XResPos resPos)
{
	XPixels<XCurPixel> tmp;
	if(!tmp.load(name,resPos)) return false;
	switch(tmp.getColorMode())
	{
	case COLOR_RGBA:
		glTexImage2D(index, 0, GL_RGBA,tmp.getWidth(),tmp.getHeight(),
			0, GL_RGBA, GL_UNSIGNED_BYTE,tmp.getPixel());
		break;
	case COLOR_BGRA:
		glTexImage2D(index, 0, GL_RGBA,tmp.getWidth(),tmp.getHeight(),
			0, GL_BGRA, GL_UNSIGNED_BYTE,tmp.getPixel());
		break;
	case COLOR_RGB:
		glTexImage2D(index, 0, GL_RGB,tmp.getWidth(),tmp.getHeight(),
			0, GL_RGB, GL_UNSIGNED_BYTE,tmp.getPixel());
		break;
	case COLOR_BGR:
		glTexImage2D(index, 0, GL_RGB,tmp.getWidth(),tmp.getHeight(),
			0, GL_BGR, GL_UNSIGNED_BYTE,tmp.getPixel());
		break;
	default:
		return false;
	}
	tmp.release();
	return true;
}

bool XCubeMap::init(const std::string& path, XResPos resPos)
{
	if (m_isInited) return true;

	switch (resPos)
	{
	case RES_LOCAL_PACK:
		if (!loadFromPacker(path)) return false;
		break;
	case RES_LOCAL_FOLDER:
		if (!loadFromFolder(path)) return false;
		break;
	case RES_WEB:
		if (!loadFromWeb(path)) return false;
		break;
	case RES_AUTO:
		if (!loadFromPacker(path) && !loadFromFolder(path) &&
			!loadFromWeb(path)) return false;
		break;
	}

	m_isInited = true;
	return true;
}
bool XCubeMap::loadFromFolder(const std::string& path)	//从文件夹中载入资源
{
	FILE *fp = NULL;
	std::string cfg = path + "/config.txt";
	if ((fp = fopen(cfg.c_str(), "r")) == NULL) return false;
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	char name[1024];
	if (fscanf(fp, "%s\n", name) != 1) return false;
	if (!loadPic(path + "/" + name, GL_TEXTURE_CUBE_MAP_POSITIVE_X, RES_LOCAL_FOLDER)) return false;
	if (fscanf(fp, "%s\n", name) != 1) return false;
	if (!loadPic(path + "/" + name, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, RES_LOCAL_FOLDER)) return false;
	if (fscanf(fp, "%s\n", name) != 1) return false;
	if (!loadPic(path + "/" + name, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, RES_LOCAL_FOLDER)) return false;
	if (fscanf(fp, "%s\n", name) != 1) return false;
	if (!loadPic(path + "/" + name, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, RES_LOCAL_FOLDER)) return false;
	if (fscanf(fp, "%s\n", name) != 1) return false;
	if (!loadPic(path + "/" + name, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, RES_LOCAL_FOLDER)) return false;
	if (fscanf(fp, "%s\n", name) != 1) return false;
	if (!loadPic(path + "/" + name, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, RES_LOCAL_FOLDER)) return false;
#if WITH_ENGINE_STATISTICS
	XEG.addStaticsticsTexInfo(m_texture, 0, GL_TEXTURE_CUBE_MAP);
#endif
	glDisable(GL_TEXTURE_CUBE_MAP);
	fclose(fp);
	return true;
}
bool XCubeMap::loadFromPacker(const std::string& path)	//从压缩包中载入资源
{
	std::string cfg = path + "/config.txt";
	unsigned char *p = XResPack.getFileData(cfg.c_str());
	if (p == NULL) return XFalse;

	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	char name[1024];
	int offset = 0;
	int imgType[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X,GL_TEXTURE_CUBE_MAP_NEGATIVE_X ,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y ,GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z ,GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };
	for (int i = 0; i < 6; ++i)
	{
		if (sscanf((char *)(p + offset), "%s\n", name) != 1 ||
			!loadPic(path + "/" + name, imgType[i], RES_LOCAL_PACK))
		{
			XMem::XDELETE_ARRAY(p);
			return false;
		}
		offset += XString::getCharPosition((char *)(p + offset), '\n') + 1;
	}
#if WITH_ENGINE_STATISTICS
	XEG.addStaticsticsTexInfo(m_texture, 0, GL_TEXTURE_CUBE_MAP);
#endif
	glDisable(GL_TEXTURE_CUBE_MAP);
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XCubeMap::loadFromWeb(const std::string& path)		//从网页中读取资源
{
	return false;
}
void XCubeMap::draw()
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
void XCubeMap::release()
{
	if(glIsTexture(m_texture)) 
	{
#if WITH_ENGINE_STATISTICS
		XEG.decStaticsticsTexInfo(m_texture,0,GL_TEXTURE_CUBE_MAP);
#endif
		LogNull("delete texture:%d\n",m_texture);
		glDeleteTextures(1, &m_texture);
	}
}
}