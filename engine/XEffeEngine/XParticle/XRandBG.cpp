#include "XStdHead.h"
#include "XRandBG.h"
#include "XConfigManager.h"
namespace XE {
bool XRandBG::init(int w, int h, const char* filename, int objsSum, bool m_withUI)
{
	if (m_isInited || w <= 0 || h <= 0) return false;

	resetParameter();
	if (m_withUI)
	{
		XCFGManager.addGroup("背景参数");
		XCFGManager.addAItem<float>(&m_minSpeed, CFG_DATA_FLOAT, "最小速度", 100.0f, 0.01f, 5.0f, nullptr, "背景参数");
		XCFGManager.addAItem<float>(&m_maxSpeed, CFG_DATA_FLOAT, "最大速度", 100.0f, 0.01f, 10.0f, nullptr, "背景参数");
		XCFGManager.addAItem<float>(&m_minScale, CFG_DATA_FLOAT, "最小缩放", 10.0f, 0.01f, 0.5f, nullptr, "背景参数");
		XCFGManager.addAItem<float>(&m_maxScale, CFG_DATA_FLOAT, "最大缩放", 10.0f, 0.01f, 1.25f, nullptr, "背景参数");
		XCFGManager.addAItem<float>(&m_minEnergy, CFG_DATA_FLOAT, "最小能量", 10000.0f, 1.0f, 400.0f, nullptr, "背景参数");
		XCFGManager.addAItem<float>(&m_maxEnergy, CFG_DATA_FLOAT, "最大能量", 10000.0f, 1.0f, 800.0f, nullptr, "背景参数");
		XCFGManager.addAItem<float>(&m_minStrength, CFG_DATA_FLOAT, "最小强度", 500.0f, 1.0f, 20.0f, nullptr, "背景参数");
		XCFGManager.addAItem<float>(&m_maxStrength, CFG_DATA_FLOAT, "最大强度", 500.0f, 1.0f, 40.0f, nullptr, "背景参数");
		XCFGManager.addAItem<float>(&m_minRecoveryRate, CFG_DATA_FLOAT, "最小恢复", 1000.0f, 1.0f, 150.0f, nullptr, "背景参数");
		XCFGManager.addAItem<float>(&m_maxRecoveryRate, CFG_DATA_FLOAT, "最大恢复", 1000.0f, 1.0f, 250.0f, nullptr, "背景参数");
	}

	m_w = w;
	m_h = h;
	m_buff.resize(m_w * m_h);

	if (!m_fbo.init(m_w, m_h, COLOR_GRAY)) return false;
	if (!m_tex.load(filename)) return false;

	m_avgBornTimer = 5.0f;
	m_bornTime = XRand::randomf(0.75f, 1.25f) * m_avgBornTimer;
	m_objs.resize(objsSum + 2);
	for (int i = 0; i < m_objs.size(); ++i)
	{
		m_freeObjs.push_back(&m_objs[i]);
	}
	for (int i = 0; i < objsSum; ++i)	//初始化5个粒子
		born();

	m_isInited = true;
	return true;
}
void XRandBG::resetParameter()
{
	m_minSpeed = 5.0f;
	m_maxSpeed = 10.0f;
	m_minScale = 0.5f;
	m_maxScale = 1.25f;
	m_minEnergy = 400.0f;
	m_maxEnergy = 800.0f;
	m_minStrength = 20.0f;
	m_maxStrength = 40.0f;
	m_minRecoveryRate = 150.0f;
	m_maxRecoveryRate = 250.0f;
}
void XRandBG::update(float steptime)
{
	if (!m_isInited) return;
	float curTime = steptime * 0.001f;
	//出生的处理
	//m_bornTime -= curTime;
	//if (m_bornTime <= 0.0f)
	//{
	//	born();
	//	m_bornTime = XRand::randomf(0.75f, 1.25f) * m_avgBornTimer;
	//}
	//状态更新
	//方案1：
	//for (int i = 0; i < m_usedObjs.size(); ++i)
	//{
	//	if (!m_usedObjs[i]->update(curTime)) continue;

	//	m_freeObjs.push_back(m_usedObjs[i]);
	//	m_usedObjs.erase(m_usedObjs.begin() + i);
	//	--i;
	//	born();
	//}
	//方案2：
	int deadSum = 0;
	for (auto it = m_usedObjs.begin(); it != m_usedObjs.end();)
	{
		if (!(*it)->update(curTime))
		{
			++it;
			continue;
		}
		m_freeObjs.push_back(*it);
		it = m_usedObjs.erase(it);
		++deadSum;
	}
	for (int i = 0; i < deadSum; ++i)
	{//不分开处理会crash
		born();
	}
	//显示
	XVec2 texSize(m_tex.getOpWidth(), m_tex.getOpHeight());
	m_fbo.useFBO();
	XEG.clearScreen(XFColor::black);
	//XGL::setBlendAlpha();
	XGL::setBlendAdd();
	XVec2 s, pos;
#ifdef WITH_DRAW_OP
	glActiveTexture(GL_TEXTURE0);
	XGL::EnableTexture2D();
	XGL::BindTexture2D(m_tex.getTexGLID());
	glBegin(GL_QUADS);

	for (auto it = m_usedObjs.begin(); it != m_usedObjs.end(); ++it)
	{
		s = texSize * (*it)->m_scale * 0.5f;
		pos = (*it)->m_pos;

		glColor4fv(XFColor(1.0f, (*it)->m_alpha));
		glTexCoord2fv(XVec2::zero);
		glVertex2fv(pos - s);
		glTexCoord2fv(XVec2::zeroOne);
		glVertex2f(pos.x - s.x, pos.y + s.y);
		glTexCoord2fv(XVec2::one);
		glVertex2fv(pos + s);
		glTexCoord2fv(XVec2::oneZero);
		glVertex2f(pos.x + s.x, pos.y - s.y);
	}

	glEnd();
#else
	for (auto it = m_usedObjs.begin(); it != m_usedObjs.end(); ++it)
	{
		tmpSize = texSize * (*it)->m_scale;
		XRender::drawBlankPlane((*it)->m_pos - tmpSize * 0.5f,
			tmpSize, m_tex.getTexGLID(), nullptr, XFColor(1.0f, (*it)->m_alpha));
	}
#endif
	glReadPixels(0, 0, m_w, m_h, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_buff.getBuffer());
	m_fbo.removeFBO();
}
void XRandBG::draw()
{
	if (!m_isInited) return;
	XRender::drawBlankPlane(m_w, m_h, m_fbo.getTexture());
}
void XRandBG::born()
{//随机产生一个粒子
	if (m_freeObjs.size() <= 0) return;
	XBGBaseObjInfo* tmp = m_freeObjs[0];
	m_freeObjs.pop_front();
	tmp->m_maxSpeed = XMath::getRotateRate(XRand::randomf(PI2)) * XRand::randomf(m_minSpeed, m_maxSpeed);
	tmp->m_curSpeed.reset();
	tmp->m_pos = XVec2(XRand::randomf(0.0f, m_w), XRand::randomf(0.0f, m_h));
	tmp->m_scale = XRand::randomf(m_minScale, m_maxScale);

	tmp->m_maxEnergy = XRand::randomf(m_minEnergy, m_maxEnergy);
	tmp->m_maxStrength = XRand::randomf(m_minStrength, m_maxStrength);
	tmp->m_releaseEnergy = tmp->m_maxEnergy;
	tmp->m_curEnergy = 0.0f;

	tmp->m_recoveryRate = XRand::randomf(m_minRecoveryRate, m_maxRecoveryRate);

	tmp->m_life = 0.0f;
	tmp->m_alpha = 0.0f;
	m_usedObjs.push_back(tmp);
}
bool XBGBaseObjInfo::update(float steptime)
{
	m_releaseEnergy -= m_curEnergy * steptime;
	if (m_releaseEnergy <= 0.0f) return true;

	m_life += steptime;
	m_pos += m_curSpeed * steptime;
	if (m_curEnergy < m_maxStrength)
	{//成长或者恢复阶段
		m_curEnergy += m_maxStrength * steptime * m_recoveryRate;
		if (m_curEnergy > m_maxStrength)
			m_curEnergy = m_maxStrength;
	}
	m_alpha = sin(m_releaseEnergy / m_maxEnergy * PI);
	m_curSpeed = m_alpha * m_maxSpeed;
	return false;
}
}