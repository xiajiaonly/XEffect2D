#ifndef _JIA_XMODELSTL_
#define _JIA_XMODELSTL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.10.1
//--------------------------------
#include "X3DWorld.h"
#include "XShaderGLSL.h"
namespace XE{
//这是一个Stl模型操作的类
/*
二进制STL文件用固定的字节数来给出三角面片的几何信息。文件起始的80个字节是文件头，
用于存贮文件名；紧接着用4个字节的整数来描述模型的三角面片个数，后面逐个给出每个三
角面片的几何信息。每个三角面片占用固定的50个字节，依次是3个4字节浮点数(角面片的法
矢量)3个4字节浮点数(1个顶点的坐标)3个4字节浮点数(2个顶点的坐标)3个4字节浮点数(3个
顶点的坐标)个三角面片的最后2个字节用来描述三角面片的属性信息。一个完整二进制STL文
件的大小为三角形面片数乘以50再加上84个字节，总共134个字节。

文件名		80Bytes
faceSum		4Bytes
faceInfo	50Bytes
	normal	3 * 4Bytes
	vector	3 * 4 * 3Bytes	
	属性	2Byts
	……
*/

class XModelStl:public XBasic3DObject
{
private:
	bool m_isInited;

	unsigned char m_modelName[80];	//模型的名字
	int m_faceSum;
	float *m_normalData;	//法线数据
	float *m_vectorData;	//顶点数据
	XVBO vbo;
public:
	bool load(const char * filename);
	void draw();
	void release()
	{
		if(!m_isInited) return;
		XMem::XDELETE_ARRAY(m_normalData);
		XMem::XDELETE_ARRAY(m_vectorData);
		vbo.release();
		m_isInited = false;
	}
	XModelStl()
		:m_isInited(false)
		,m_normalData(NULL)
		,m_vectorData(NULL)
	{}
	~XModelStl(){release();}
};
}
#endif