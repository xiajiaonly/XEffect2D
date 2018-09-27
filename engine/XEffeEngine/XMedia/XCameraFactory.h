#ifndef _JIA_XCAMERAFACTORY
#define _JIA_XCAMERAFACTORY
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
#include "XCameraBasic.h"
namespace XE{
class XPixelsInputFactory
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//下面需要将其设计为Singleton模式
protected:
	XPixelsInputFactory(){}
	XPixelsInputFactory(const XPixelsInputFactory&);
	XPixelsInputFactory &operator= (const XPixelsInputFactory&);
	virtual ~XPixelsInputFactory(){} 
public:
	static XPixelsInputFactory& GetInstance()
	{
		static XPixelsInputFactory m_instance;
		return m_instance;
	}
	//-------------------------------------------
public:
	template<typename T>
	T * create(XPixelsInputInfo & data)
	{
		T *pPixelsInput = NULL;
		pPixelsInput = XMem::createMem<T>();
		if(pPixelsInput != NULL && !pPixelsInput->init(data))
			XMem::XDELETE(pPixelsInput);//初始化失败
		return pPixelsInput;
	}
};
}
#endif //_JIA_XCAMERAFACTORY
