#ifndef _JIA_XFIELD2D_
#define _JIA_XFIELD2D_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include "XMath\XVector2.h"
#include "XMemory.h"
namespace XE{
//这是一个用于描述2D空间的势场的类
class XField2D
{
public:
	enum XField2DType	//几种基本类型的场
	{
		FIELD2D_NULL,
		FIELD2D_DOWN,	//下落的场
		FIELD2D_WHIRL,	//漩涡的场
		FIELD2D_RAND,	//随机场
	};
private:
	int m_w;
	int m_h;
	XVector2 *m_pVectors;
	bool m_isInited;
	XField2DType m_type;
public:
	XField2D()
		:m_isInited(false)
		,m_pVectors(NULL)
		,m_type(FIELD2D_NULL)
	{}
	~XField2D(){Release();}
	XBool Init(int w,int h)
	{
		if(m_isInited) return XFalse;
		if(w <= 0 || h <= 0) return XFalse;
		m_w = w;
		m_h = h;
		m_pVectors = XMem::createArrayMem<XVector2>(w * h);
		if(m_pVectors == NULL) return XFalse;
		m_type = FIELD2D_NULL;
		m_isInited = XTrue;
		return XTrue;
	}
	XField2DType GetType() const {return m_type;}
	void SetField2DType(XField2DType type);
	void Draw(const XVector2 &size);	//描绘场
	void Release()
	{
		if(!m_isInited) return;
		XMem::XDELETE_ARRAY(m_pVectors);
		m_isInited = XFalse;
	}
	XVector2 GetVector(int x,int y) const
	{
		if(!m_isInited || x < 0 || y < 0 || x >= m_w || y >= m_h) return XVector2::zero;
		return m_pVectors[x + m_w * y];
	}
};
}
#endif