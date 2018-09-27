#include "XStdHead.h"
#include "XMoveDataPad.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"

namespace XE {
void XMoveDataPad::initParam(XMoveDataMode mode)
{
	switch (mode)
	{
	case MD_MODE_QBP:
		m_p1MD.setEx(0.0f, 1.0f, 3.0f, MD_MODE_SIN, -1);
		m_p2MD.setEx(0.0f, 1.0f, 3.0f, MD_MODE_COS, -1);
		break;
	case MD_MODE_ESP:
		m_p1MD.setEx(0.0f, 1.0f, 5.0f, MD_MODE_SIN, -1);
		break;
	case MD_MODE_CBP:
		m_p1MD.setEx(0.0f, 1.0f, 3.0f, MD_MODE_SIN, -1);
		m_p2MD.setEx(0.0f, 1.0f, 3.0f, MD_MODE_COS, -1);
		m_p3MD.setEx(0.0f, 1.0f, 6.0f, MD_MODE_SIN, -1);
		m_p4MD.setEx(0.0f, 1.0f, 6.0f, MD_MODE_COS, -1);
		break;
	case MD_MODE_CB2P:
		break;
	case MD_MODE_SG:
		break;
	case MD_MODE_OD:
		break;
	case MD_MODE_EIB:
	case MD_MODE_EOB:
	case MD_MODE_EOIB:
	case MD_MODE_EIOB:
		m_p1MD.setEx(-1.5f, 1.5f, 3.0f, MD_MODE_SIN, -1);
		break;
	case MD_MODE_EIE:
	case MD_MODE_EOE:
	case MD_MODE_EIOE:
	case MD_MODE_EOIE:
		m_p1MD.setEx(0.5f, 1.5f, 3.0f, MD_MODE_SIN, -1);
		m_p2MD.setEx(0.5f, 1.5f, 3.0f, MD_MODE_COS, -1);
		m_p3MD.setEx(0.25f, 0.75f, 3.0f, MD_MODE_SIN, -1);
		break;
	case MD_MODE_BIC:
	case MD_MODE_BOC:
		m_p1MD.setEx(1.0f, 7.0f, 3.0f, MD_MODE_SIN, -1);
		m_p2MD.setEx(0.2f, 0.7f, 3.0f, MD_MODE_COS, -1);
		break;
	case MD_MODE_MAX:
		break;
	}
}
bool XMoveDataPad::initWithoutSkin(XMoveDataMode mode)
{
	if (m_isInited) return false;
	m_md.setEx(0.0f, 1.0f, 5.0f, mode, -1);
	initParam(mode);
	m_curMouseRect.set(m_position, m_position + m_scale * m_pixelsSize);

	m_caption.setACopy(getDefaultFont());
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体居中对齐
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP); //设置字体居中对齐
	m_caption.setPosition(m_curMouseRect.getLB());
	m_caption.setString(m_md.getModeStr().c_str());
	m_caption.setColor(XFColor::black);

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = true;
	return true;
}
void XMoveDataPad::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = false;
}
void XMoveDataPad::update(float steptime)
{
	if (!m_isInited) return;
	m_md.move(steptime);
	m_p1MD.move(steptime);
	m_p2MD.move(steptime);
	m_p3MD.move(steptime);
	m_p4MD.move(steptime);
	switch (m_md.getMoveMode())
	{
	case MD_MODE_QBP:
		m_md.m_param.p1 = m_p1MD.getCurData();
		m_md.m_param.p2 = m_p2MD.getCurData();
		break;
	case MD_MODE_ESP:
		m_md.m_param.p1 = m_p1MD.getCurData();
		break;
	case MD_MODE_CBP:
		m_md.m_param.p1 = m_p1MD.getCurData();
		m_md.m_param.p2 = m_p2MD.getCurData();
		m_md.m_param.p3 = m_p3MD.getCurData();
		m_md.m_param.p4 = m_p4MD.getCurData();
		break;
	case MD_MODE_EIB:
	case MD_MODE_EOB:
	case MD_MODE_EOIB:
	case MD_MODE_EIOB:
		m_md.m_param.p1 = m_p1MD.getCurData();
		break;
	case MD_MODE_EIE:
	case MD_MODE_EOE:
	case MD_MODE_EIOE:
	case MD_MODE_EOIE:
		m_md.m_param.p1 = m_p1MD.getCurData();
		m_md.m_param.p2 = m_p2MD.getCurData();
		m_md.m_param.p3 = m_p3MD.getCurData();
		m_md.m_param.p4 = m_p4MD.getCurData();
		break;
	case MD_MODE_BIC:
	case MD_MODE_BOC:
		m_md.m_param.p1 = XMath::clamp<float>(m_p1MD.getCurData(), 1.0f, BOUNCES_MAX - 1);
		m_md.m_param.p2 = 0.0f;
		m_md.setBounceElast(m_p2MD.getCurData());
		break;
	}
}
void XMoveDataPad::draw()
{
	if (!m_isInited) return;
	XVec2 tmpSize = m_scale * m_pixelsSize;
	//描绘底
	XRender::drawFillRect(m_position, tmpSize, XFColor::black * m_color);
	//描绘辅助线
	XRender::drawLine(m_position, m_position + XVec2(0.0f, tmpSize.y), 2.0f, XFColor::yellow * m_color);
	XRender::drawLine(m_position + XVec2(0.0f, tmpSize.y), m_position + tmpSize, 2.0f, XFColor::yellow * m_color);
	XRender::drawLine(m_position + XVec2(0.0f, tmpSize.y), m_position + XVec2(tmpSize.x, 0.0f), 1.0f, XFColor::green * m_color);
	//下面描绘曲线
	XVec2 pos, posOld(0.0f, (1.0f - m_md.getValueEx(0.0f)) * m_pixelsSize);
	posOld = m_position + posOld * m_scale;
	float rate = m_pixelsSize / 128.0f;
	for (int i = 1; i <= 128; ++i)
	{
		pos.x = i * rate;
		pos.y = (1.0f - m_md.getValueEx(i / 128.0f)) * m_pixelsSize;
		pos = m_position + pos * m_scale;
		XRender::drawLine(posOld, pos, 2.0f, XFColor::blue);
		posOld = pos;
	}
	if (m_md.getCurTimer() <= 1.0f)
		XRender::drawPoint(m_position + XVec2(m_md.getCurTimer(), 1.0f - m_md.getCurData()) *
			XVec2(m_pixelsSize) * m_scale, 2, XFColor::red * m_color);
	else
		XRender::drawPoint(m_position + XVec2(2.0f - m_md.getCurTimer(), 1.0f - m_md.getCurData()) *
			XVec2(m_pixelsSize) * m_scale, 2, XFColor::red * m_color);

	m_caption.draw();
}
}