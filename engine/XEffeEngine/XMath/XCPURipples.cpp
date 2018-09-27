#include "XStdHead.h"
#include "XCPURipples.h"
namespace XE {
bool XCPURipples::init(int w, int h, float attenuation, float startStrength)
{
	if (m_isInited || w <= 0 || h <= 0) return false;
	m_w = w;
	m_h = h;
	m_attenuation = XMath::clamp(attenuation, 0.0f, 1.0f);
	m_startStrength = startStrength;
	m_data0.resize(m_w * m_h);
	m_data1.resize(m_data0.size());
	m_heightMap.resize(m_data0.size());
	for (int i = 0; i < m_data0.size(); ++i)
	{
		m_data0[i].set(0.5f, 0.5f, 0.5f, 0.5f);
		m_data1[i].set(0.5f, 0.5f, 0.5f, 0.5f);
		m_heightMap[i] = 0.5;
	}

	m_isInited = true;
	return true;
}
void XCPURipples::update()
{
	if (!m_isInited) return;
	XVec4 *src = NULL;
	XVec4 *dst = NULL;
	if (m_flag)
	{
		src = &m_data0[0];
		dst = &m_data1[0];
	}
	else
	{
		src = &m_data1[0];
		dst = &m_data0[0];
	}
	float v1, v2, v3, v4, f;
	int index = 0;
	for (int h = 0; h < m_h; ++h)
	{
		for (int w = 0; w < m_w; ++w, ++index)
		{
			v1 = (h > 0 && w > 0) ?			src[index - m_w - 1].x : 0.5f;
			v2 = (h < m_h - 1 && w > 0) ?	src[index + m_w - 1].x : 0.5f;
			v3 = (h > 0 && w < m_w - 1) ?	src[index - m_w + 1].x : 0.5f;
			v4 = (h < m_h - 1 && w < m_w - 1) ? src[index + m_w + 1].x : 0.5f;

			f = v1 + v2 + v3 + v4 - 4.0f * src[index].x;
			XVec4& ref = dst[index];
			ref = src[index];
			ref -= 0.5f;

			ref.y += 0.1 * f;		//这两个系数？
			ref.x += 0.6 * ref.y;

			ref *= m_attenuation;	//衰减？

			ref.z = 0.25 * (v1 + v2 - v3 - v4); // sobelX
			ref.w = 0.25 * (v1 - v2 + v3 - v4); // sobelY

			ref += 0.5f;
			m_heightMap[index] = ref.x * 256.0f;
			//m_heightMap[index] = (int)(ref.x * 512.0f) % 256;
			//m_heightMap[index] = (int)(ref.x * 10000) % 256;
		}
	}
	m_flag = !m_flag;
}
}