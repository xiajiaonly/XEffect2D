#ifndef _JIA_XCUBEMAP_
#define _JIA_XCUBEMAP_
#include "XPixelCommon.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.1
//--------------------------------
namespace XE{
class XCubeMap
{
private:
	bool m_isInited;	//是否初始化
	unsigned int m_texture;

	bool loadPic(const std::string& name,int index,XResPos resPos);
	void release();

	bool loadFromFolder(const std::string& filename);	//从文件夹中载入资源
	bool loadFromPacker(const std::string& filename);	//从压缩包中载入资源
	bool loadFromWeb(const std::string& filename);		//从网页中读取资源
public:
	bool init(const std::string& path,XResPos resPos = RES_SYS_DEF);
	void draw();
	unsigned int getTexture() const {return m_texture;}

	XCubeMap()
		:m_isInited(false)
		,m_texture(0)
	{}
	~XCubeMap(){release();}
};
}
#endif