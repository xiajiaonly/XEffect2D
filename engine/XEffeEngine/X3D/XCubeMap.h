#ifndef _JIA_XCUBEMAP_
#define _JIA_XCUBEMAP_
#include "XPixelCommon.h"

class _XCubeMap
{
private:
	bool m_isInited;	// «∑Ò≥ı ºªØ
	unsigned int m_texture;

	bool loadPic(const std::string & name,int index,_XResourcePosition resPos);
	void release();
public:
	bool init(const std::string &path,_XResourcePosition resPos = RESOURCE_SYSTEM_DEFINE);
	void draw();
	unsigned int getTexture() const {return m_texture;}

	_XCubeMap()
		:m_isInited(false)
		,m_texture(0)
	{}
	~_XCubeMap(){release();}
};

#endif