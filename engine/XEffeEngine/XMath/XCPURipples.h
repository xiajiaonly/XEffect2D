#ifndef _JIA_XCPURIPPLES_
#define _JIA_XCPURIPPLES_
#include "XMath\XVector4.h"
#include <vector>
namespace XE {
class XCPURipples
{
private:
	int m_w;
	int m_h;
	float m_attenuation;
	float m_startStrength;
	bool m_isInited;
	std::vector<XVec4> m_data0;
	std::vector<XVec4> m_data1;
	std::vector<unsigned char> m_heightMap;
	bool m_flag;
public:
	int getWidth()const { return m_w; }
	int getHeight()const { return m_h; }
	//attenuation 0.0 - 1.0f
	bool init(int w, int h, float attenuation = 0.985f, float startStrength = 0.45f);
	void update();
	void drop(int x, int y)
	{
		if (!m_isInited || !XMath::isInArea(x, 0, m_w) ||
			!XMath::isInArea(y, 0, m_h)) return;
		if (m_flag) m_data0[x + y * m_w].set(m_startStrength + 0.5f, 0 + 0.5f, 0 + 0.5f, 0 + 0.5f);
		else m_data1[x + y * m_w].set(m_startStrength + 0.5f, 0 + 0.5f, 0 + 0.5f, 0 + 0.5f);
	}
	XCPURipples()
		:m_isInited(false)
		, m_flag(false)
		, m_attenuation(0.985f)
		, m_startStrength(0.45f)
	{}
	const void* getHeightMap()const { return &m_heightMap[0]; }
};
}
#endif