#include "XStdHead.h"
#include "XMath\XColorSpace.h"
#include "XConfigManager.h"
#include "XFirefly.h"
#include "XRandBG.h"
namespace XE {
bool XFirefly::init(int maxObjSum, const char* texfilename, int w, int h, bool withUI)
{
	if (m_isInited || maxObjSum <= 0 || texfilename == nullptr ||
		!m_objTxt.load(texfilename)) return false;
	m_allObjs.resize(maxObjSum);
	for (int i = 0; i < m_allObjs.size(); ++i)
	{
		m_freeObj.push_back(&m_allObjs[i]);
	}
	resetParameter();
	if (withUI)
	{
		XCFGManager.addGroup("物件参数");
		XCFGManager.addAItem<float>(&m_minSpeed, CFG_DATA_FLOAT, "最小速度", 100.0f, 0.01f, 5.0f, nullptr, "物件参数");
		XCFGManager.addAItem<float>(&m_maxSpeed, CFG_DATA_FLOAT, "最大速度", 100.0f, 0.01f, 10.0f, nullptr, "物件参数");
	//	XCFGManager.addAItem<float>(&m_minScale, CFG_DATA_FLOAT, "最小缩放", 10.0f, 0.01f, 0.5f, nullptr, "物件参数");
	//	XCFGManager.addAItem<float>(&m_maxScale, CFG_DATA_FLOAT, "最大缩放", 10.0f, 0.01f, 1.25f, nullptr, "物件参数");
		XCFGManager.addAItem<float>(&m_minEnergy, CFG_DATA_FLOAT, "最小能量", 10000.0f, 1.0f, 400.0f, nullptr, "物件参数");
		XCFGManager.addAItem<float>(&m_maxEnergy, CFG_DATA_FLOAT, "最大能量", 10000.0f, 1.0f, 800.0f, nullptr, "物件参数");
		XCFGManager.addAItem<float>(&m_minCycleSpeed, CFG_DATA_FLOAT, "最小闪速", 100.0f, 0.1f, 5.0f, nullptr, "物件参数");
		XCFGManager.addAItem<float>(&m_maxCycleSpeed, CFG_DATA_FLOAT, "最大闪速", 100.0f, 0.1f, 10.0f, nullptr, "物件参数");
		XCFGManager.addAItem<float>(&m_minRecoveryRate, CFG_DATA_FLOAT, "最小恢复", 1000.0f, 1.0f, 150.0f, nullptr, "物件参数");
		XCFGManager.addAItem<float>(&m_maxRecoveryRate, CFG_DATA_FLOAT, "最大恢复", 1000.0f, 1.0f, 250.0f, nullptr, "物件参数");
		XCFGManager.addAItem<float>(&m_colorBase, CFG_DATA_FLOAT, "颜色基准", 360.0f, 0.0f, 0.0f, nullptr, "物件参数");
		XCFGManager.addAItem<float>(&m_colorArea, CFG_DATA_FLOAT, "色域", 360.0f, 20.0f, 30.0f, nullptr, "物件参数");
		XCFGManager.addAItem<float>(&m_minLight, CFG_DATA_FLOAT, "最小亮度", 100.0f, 1.0f, 5.0f, nullptr, "物件参数");
		XCFGManager.addAItem<float>(&m_maxLight, CFG_DATA_FLOAT, "最大亮度", 100.0f, 1.0f, 10.0f, nullptr, "物件参数");
		XCFGManager.addAItem<float>(&m_nearRate, CFG_DATA_FLOAT, "近率", 100.0f, 0.1f, 20.0f, nullptr, "物件参数");
		XCFGManager.addAItem<float>(&m_bornRate, CFG_DATA_FLOAT, "出生率", 16384.0f, 100.0f, 2048.0f, nullptr, "物件参数");
	}

	m_w = w;
	m_h = h;

	m_isInited = true;
	return true;
}
void XFirefly::resetParameter()
{
	m_minEnergy = 50.0f;
	m_maxEnergy = 100.0f;
	m_minCycleSpeed = 5.0f;
	m_maxCycleSpeed = 10.0f;
	m_colorBase = 0.0f;
	m_colorArea = 30.0f;
	m_minLight = 5.0f;
	m_maxLight = 10.0f;
	m_minRecoveryRate = 150.0f;
	m_maxRecoveryRate = 250.0f;
	m_minSpeed = 5.0f;
	m_maxSpeed = 10.0f;
	m_nearRate = 20.0f;
}
void XFirefly::update(float steptime, const XRandBG& bg)
{
	if (!m_isInited) return;
	m_bornSum += m_bornRate * steptime * 0.001f;
	while (m_bornSum > 1.0f)
	{
		if (m_freeObj.size() == 0) break;

		XObject* tmp = m_freeObj[0];
		XVec2 pos(XRand::randomf(1.0f), XRand::randomf(1.0f));
		if (XRand::random(256) <= bg.getPixel(pos))
		{
			tmp->m_pos = pos * XVec2(m_w, m_h);
			tmp->m_maxEnergy = XRand::randomf(m_minEnergy, m_maxEnergy);
			tmp->m_energy = tmp->m_maxEnergy;
			tmp->m_light = 0.0f;
			tmp->m_cycle = 0.0f;
			tmp->m_cycleSpeed = XRand::randomf(m_minCycleSpeed, m_maxCycleSpeed);
			tmp->m_color = XColorSpace::getWheelColor(m_colorBase + m_colorArea * XRand::randomf(1.0f, -1.0f));
			tmp->m_scale = (1.0f + XRand::gaussRand()) * 0.5f * XRand::randomf(0.05f, 0.2f);

			tmp->m_maxLigth = XRand::randomf(m_minLight, m_maxLight);
			tmp->m_lightRecoveryRate = XRand::randomf(m_minRecoveryRate, m_maxRecoveryRate);

			tmp->m_curSpeed.reset();
			tmp->m_maxSpeed = XMath::getRotateRate(XRand::randomf(PI2)) * XRand::randomf(m_minSpeed, m_maxSpeed);

			tmp->m_isNear = XRand::randomf(100.0f) < m_nearRate;

			m_usedObj.push_back(tmp);
			m_freeObj.pop_front();
		}
		m_bornSum -= 1.0f;
	}
	for (auto it = m_usedObj.begin(); it != m_usedObj.end();)
	{
		if (!(*it)->update(steptime))
		{
			++it;
			continue;
		}
		m_freeObj.push_back(*it);
		it = m_usedObj.erase(it);
	}
}
void XFirefly::draw()
{
	if (!m_isInited) return;
	//XGL::setBlendAlpha();
	XGL::setBlendAdd();
#ifdef WITH_DRAW_OP
	glActiveTexture(GL_TEXTURE0);
	XGL::EnableTexture2D();
	XGL::BindTexture2D(m_objTxt.getTexGLID());
	glBegin(GL_QUADS);
	for (auto it = m_usedObj.begin(); it != m_usedObj.end(); ++it)
	{
		(*it)->draw();
	}
#else
	for (auto it = m_usedObj.begin(); it != m_usedObj.end(); ++it)
	{
		(*it)->draw(m_objTxt.getTexGLID());
	}
#endif
#ifdef WITH_DRAW_OP
	glEnd();
#endif
}
bool XObject::update(float steptime)
{
	float t = steptime * 0.001f;
//	if (m_isNear)
//		m_pos += m_curSpeed * m_scale * t * 20.0f;	//越小飞的越慢
//	else
	m_pos += m_curSpeed * m_scale * t;	//越小飞的越慢
//基本能量消耗
	if (m_light < m_maxLigth)
	{//能量恢复
		m_light += m_maxLigth * t * m_lightRecoveryRate;
		if (m_light > m_maxLigth) m_light = m_maxLigth;
	}
	m_curSpeed = (m_light / m_maxLigth) * m_maxSpeed;
	//下面计算能量消耗
	float tmpLight = m_light;
	//加上速度加成
	m_cycle += m_cycleSpeed * t;
	tmpLight *= 0.8f + 0.3f * sin(m_cycle);	//闪烁效果
	//加上周期消耗
	tmpLight *= (1.0f + XMath::clamp(m_curSpeed.getLength() * 0.01f, 0.0f, 1.0f) * 0.5f);
	m_color.a = 0.8f * tmpLight / m_maxLigth * sin(m_energy / m_maxEnergy * PI) * (0.01 + m_scale) * 6.0f;
	m_energy -= tmpLight * t;

	return m_energy <= 0.0f;
}
#ifdef WITH_DRAW_OP
void XObject::draw()
{
	XVec2 s;
	if (m_isNear)
	{
		s.set(32.0f * m_scale * 16.0f);
		m_color.a *= 0.05f;
	}
	else
	{
		s.set(32.0f * m_scale);
	}
	glColor4fv(m_color);

	glTexCoord2fv(XVec2::zero);
	glVertex2fv(m_pos - s);
	glTexCoord2fv(XVec2::zeroOne);
	glVertex2f(m_pos.x - s.x, m_pos.y + s.y);
	glTexCoord2fv(XVec2::one);
	glVertex2fv(m_pos + s);
	glTexCoord2fv(XVec2::oneZero);
	glVertex2f(m_pos.x + s.x, m_pos.y - s.y);
}
#else
void XObject::draw(unsigned int tex)
{
	XVec2 s;
	if (m_isNear)
	{
		s.set(32.0f * m_scale * 16.0f);
		m_color.a *= 0.05f;
	}
	else
	{
		s.set(32.0f * m_scale);
	}

	XRender::drawBlankPlane(m_pos - s, s * 2.0f, tex, nullptr, m_color);
}
#endif
}