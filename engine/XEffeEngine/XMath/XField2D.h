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
	int m_sum;
	XVec2 *m_pVectors;
	bool m_isInited;
	XField2DType m_type;
public:
	XField2D()
		:m_isInited(false)
		, m_pVectors(NULL)
		, m_type(FIELD2D_NULL)
	{}
	~XField2D() { Release(); }
	XBool Init(int w, int h);
	XField2DType GetType() const { return m_type; }
	void SetField2DType(XField2DType type);
	void Draw(const XVec2& size, float scale);	//描绘场
	void Release();
	const XVec2& GetVector(int x, int y) const;
	const XVec2& GetVectorFast(int index) const;
	void SetVector(int x, int y, const XVec2& v);
	void SetVectorFast(int x, int y, const XVec2& v);
	void BlendVector(int index, const XVec2& v, float rate);
	void ScaleVector(float scale)
	{
		for (int i = 0; i < m_sum; ++i)
		{
			m_pVectors[i] *= scale;
		}
	}
	void Reset();
	bool Save2File(const char* filename);
	bool LoadFromFile(const char* filename);
};
#if WITH_INLINE_FILE
#include "XField2D.inl"
#endif
}
#endif