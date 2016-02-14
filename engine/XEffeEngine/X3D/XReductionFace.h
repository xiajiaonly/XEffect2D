#ifndef _JIA_XREDUCTIONFACE_
#define _JIA_XREDUCTIONFACE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.10.1
//--------------------------------
//这是一个模型自动减面的算法的类
//(尚未完成)
#include "XBasic3D.h"

#include <vector>
#include <list>
namespace XE{
class XReductionFace
{
private:
	XBool m_isInited;
public:
//	void reductionFace(std::vector<XVector3> &v,	//顶点的数据
//		std::vector<XFaceInfo> &f,
//		std::vector<int> &map,
//		std::vector<int> &permutation);

	XReductionFace()
		:m_isInited(XFalse)
	{}
};
}
#endif