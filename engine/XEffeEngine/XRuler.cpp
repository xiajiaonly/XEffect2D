#include "XStdHead.h"
#include "XRuler.h"
#define XRULER_DRAW_OPTIM
namespace XE {
bool XRuler::init()
{
	if (m_isInited) return false;
	m_position = XEG.getSceneCenter();
	m_pixelSize = XEG.getSceneSize();
	m_size = m_pixelSize;
	m_scale.set(1.0f);

	m_font.setACopy(getDefaultFont());
	m_font.setScale(0.5f);

	m_isInited = true;
	return true;
}
bool XRuler::init(const XVec2&pos, const XVec2&size)
{
	if (m_isInited ||
		size.x <= 0.0f || size.y <= 0.0f) return false;
	m_position = pos;
	m_pixelSize = size;
	m_size = m_pixelSize;
	m_scale.set(1.0f);

	m_font.setACopy(getDefaultFont());
	m_font.setScale(0.5f);

	m_isInited = true;
	return true;
}
bool XRuler::init(const XVec2&pos, const XVec2&size, const XVec2&scale)
{
	if (m_isInited ||
		size.x <= 0.0f || size.y <= 0.0f ||
		scale.x <= 0.0f || scale.y <= 0.0f) return false;
	m_position = pos;
	m_pixelSize = size;
	m_size = m_pixelSize / scale;
	m_scale = scale;

	m_font.setACopy(getDefaultFont());
	m_font.setScale(0.5f);

	m_isInited = true;
	return true;
}
//void XRuler::update()
//{
//	if (!m_isInited) return;
//}
void XRuler::draw()
{
	if (!m_isInited) return;
	XVec2 halfPixelSize = m_size * m_scale * 0.5f;
	double startX = m_size.x * 0.5 + m_posOffset.x;
	double startY = m_size.y * 0.5 + m_posOffset.y;
	if (m_style.m_withCross)
	{//描绘十字基准线
		XFColor tmpColor = XFColor::darkGray;
		tmpColor.a = 0.45f;
		float offset = m_posOffset.y * m_scale.y;
		if (offset <= halfPixelSize.y && offset >= -halfPixelSize.y)
		{
			XRender::drawLine(m_position - XVec2(halfPixelSize.x, -offset),
				m_position + XVec2(halfPixelSize.x, offset), 2.0f, tmpColor);
		}

		offset = m_posOffset.x * m_scale.x;
		if (offset <= halfPixelSize.x && offset >= -halfPixelSize.x)
		{
			XRender::drawLine(m_position - XVec2(-offset, halfPixelSize.y),
				m_position + XVec2(offset, halfPixelSize.y), 2.0f, tmpColor);
		}
	}
	
	//计算网格基准
	int fData;
	double minNet = XMath::getTheFirstData((std::max)(m_size.x, m_size.y), fData);
	if (fData >= 2) minNet /= static_cast<double>(fData);
	else minNet = minNet / static_cast<double>(fData) * 0.1f;

	if (m_style.m_withNet)
	{//描绘网格
		XFColor tmpColor = XFColor::darkGray;
		tmpColor.a = 0.25f;
		int sum = floor(m_size.x / minNet);	//计算需要画多少线
		double start = startX - floor(startX / minNet) * minNet;
		if(start < 0.0) start = start + minNet - startX;
		else start -= startX;
		start += m_posOffset.x;
		double step = minNet * m_scale.x;
		start *= m_scale.x;
#ifdef XRULER_DRAW_OPTIM
		XGL::setBlendAlpha();
		glColor4fv(tmpColor);
		glLineWidth(0.5f);
		XGL::DisableTexture2D();
		glBegin(GL_LINES);
		for (int i = 0; i <= sum; ++i, start += step)
		{
			if (start > halfPixelSize.x) continue;
			glVertex2fv(m_position + XVec2(start, -halfPixelSize.y));
			glVertex2fv(m_position + XVec2(start, halfPixelSize.y));
		}
		glEnd();
#else
		for (int i = 0; i <= sum; ++i, start += step)
		{
			if (start > halfPixelSize.x) continue;
			XRender::drawLine(m_position + XVec2(start, -halfPixelSize.y),
				m_position + XVec2(start, halfPixelSize.y), 0.5f, tmpColor);
		}
#endif
		sum = floor(m_size.y / minNet);	//计算需要画多少线
		start = startY - floor(startY / minNet) * minNet;
		if(start < 0.0) start = start + minNet - startY;
		else start -= startY;
		start += m_posOffset.y;
		step = minNet * m_scale.y;
		start *= m_scale.y;
#ifdef XRULER_DRAW_OPTIM
		glBegin(GL_LINES);
		for (int i = 0; i <= sum; ++i, start += step)
		{
			if (start > halfPixelSize.y) continue;
			glVertex2fv(m_position + XVec2(-halfPixelSize.x, start));
			glVertex2fv(m_position + XVec2(halfPixelSize.x, start));
		}
		glEnd();
		XGL::DisableBlend();
#else
		for (int i = 0; i <= sum; ++i, start += step)
		{
			if (start > halfPixelSize.y) continue;
			XRender::drawLine(m_position + XVec2(-halfPixelSize.x, start),
				m_position + XVec2(halfPixelSize.x, start), 0.5f, tmpColor);
		}
#endif
	}
	if (m_style.m_withEdgeLine)
	{//描绘边框
		XRender::drawRect(m_position, halfPixelSize, 1.0f, XFColor::darkGray);
		//XRender::drawLine(m_position - halfPixelSize,
		//	m_position + XVec2(halfPixelSize.x, -halfPixelSize.y), 1.0f, XFColor::darkGray);
		//XRender::drawLine(m_position - halfPixelSize,
		//	m_position + XVec2(-halfPixelSize.x, halfPixelSize.y), 1.0f, XFColor::darkGray);
		//XRender::drawLine(m_position + halfPixelSize,
		//	m_position + XVec2(halfPixelSize.x, -halfPixelSize.y), 1.0f, XFColor::darkGray);
		//XRender::drawLine(m_position + halfPixelSize,
		//	m_position + XVec2(-halfPixelSize.x, halfPixelSize.y), 1.0f, XFColor::darkGray);
	}
	if (m_style.m_withEdgeRule)
	{//描绘边标尺
		double dis = minNet * 0.2f;
		XFColor tmpColor = XFColor::darkGray;
		tmpColor.a = 0.75f;

		double tmpLen;
		int index;
		int sum = floor(m_size.x / dis);	//计算需要画多少线
		index = floor(startX / dis);
		double start = startX - static_cast<double>(index) * dis;
		if(start < 0.0) start = start + dis - startX;
		else start -= startX;
		start += m_posOffset.x;
		index = 5 - index % 5;
		double step = dis * m_scale.x;
		start *= m_scale.x;
#ifdef XRULER_DRAW_OPTIM
		XGL::setBlendAlpha();
		glColor4fv(tmpColor);
		glLineWidth(0.5f);
		XGL::DisableTexture2D();
		glBegin(GL_LINES);
		for (int i = 0; i <= sum; ++i,++index, start += step)
		{
			if (start > halfPixelSize.x) continue;
			if (index % 5 == 0) tmpLen = 6.0f;
			else tmpLen = 4.0f;

			glVertex2fv(m_position + XVec2(start, -halfPixelSize.y));
			glVertex2fv(m_position + XVec2(start, -halfPixelSize.y + tmpLen));

			glVertex2fv(m_position + XVec2(start, halfPixelSize.y));
			glVertex2fv(m_position + XVec2(start, halfPixelSize.y - tmpLen));
		}
		glEnd();
#else
		for (int i = 0; i <= sum; ++i,++index, start += step)
		{
			if (start > halfPixelSize.x) continue;
			if (index % 5 == 0) tmpLen = 6.0f;
			else tmpLen = 4.0f;

			XRender::drawLine(m_position + XVec2(start, -halfPixelSize.y),
				m_position + XVec2(start, -halfPixelSize.y + tmpLen), 0.5f, tmpColor);
			XRender::drawLine(m_position + XVec2(start, halfPixelSize.y),
				m_position + XVec2(start, halfPixelSize.y - tmpLen), 0.5f, tmpColor);
		}
#endif
		sum = floor(m_size.y / dis);	//计算需要画多少线
		index = floor(startY / dis);
		start = startY - static_cast<double>(index) * dis;
		if(start < 0.0) start = start + dis - startY;
		else start -= startY;
		start += m_posOffset.y;
		index = 5 - index % 5;
		step = dis * m_scale.y;
		start *= m_scale.y;
#ifdef XRULER_DRAW_OPTIM
		glBegin(GL_LINES);
		for (int i = 0; i <= sum; ++i, ++index, start += step)
		{
			if (start > halfPixelSize.x) continue;
			if (index % 5 == 0) tmpLen = 6.0f;
			else tmpLen = 4.0f;

			glVertex2fv(m_position + XVec2(-halfPixelSize.x, start));
			glVertex2fv(m_position + XVec2(-halfPixelSize.x + tmpLen, start));

			glVertex2fv(m_position + XVec2(halfPixelSize.x, start));
			glVertex2fv(m_position + XVec2(halfPixelSize.x - tmpLen, start));
		}
		glEnd();
		XGL::DisableBlend();
#else
		for (int i = 0; i <= sum; ++i, ++index, start += step)
		{
			if (start > halfPixelSize.y) continue;
			if (index % 5 == 0) tmpLen = 6.0f;
			else tmpLen = 4.0f;

			XRender::drawLine(m_position + XVec2(-halfPixelSize.x, start),
				m_position + XVec2(-halfPixelSize.x + tmpLen, start), 0.5f, tmpColor);
			XRender::drawLine(m_position + XVec2(halfPixelSize.x, start),
				m_position + XVec2(halfPixelSize.x - tmpLen, start), 0.5f, tmpColor);
		}
#endif
	}
	char tmpStr[32];
	if (m_style.m_withFont)
	{//显示边文字
		m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE);
		m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
		int sum = floor(m_size.x / minNet);	//计算需要画多少线
		double start = fmod(startX, minNet);
		if(start < 0.0) start = start + minNet - startX;
		else start -= startX;
		start += m_posOffset.x;
		for (int i = 0; i <= sum; ++i, start += minNet)
		{
			if (start * m_scale.x > halfPixelSize.x) continue;
#ifdef XRULER_DRAW_OPTIM
			sprintf_s(tmpStr, "%g", start - m_posOffset.x);
			m_font.setString(tmpStr);
#else
			m_font.setString(XString::toString(start - m_posOffset.x).c_str());
#endif
			m_font.setPosition(m_position + XVec2(start * m_scale.x, halfPixelSize.y - 14.0f));
			m_font.draw();
		}
		m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
		sum = floor(m_size.y / minNet);	//计算需要画多少线
		start = fmod(startY, minNet);
		if(start < 0.0) start = start + minNet - startY;
		else start -= startY;
		start += m_posOffset.y;
		//如果字太靠近边框则不显示(尚未添加)
		for (int i = 0;i <= sum;++ i, start += minNet)
		{
			if (start * m_scale.y > halfPixelSize.y - 16.0f ||
				start * m_scale.y < 16.0f - halfPixelSize.y) continue;
#ifdef XRULER_DRAW_OPTIM
			sprintf_s(tmpStr, "%g", start - m_posOffset.y);
			m_font.setString(tmpStr);
#else
			m_font.setString(XString::toString(start - m_posOffset.y).c_str());
#endif
			m_font.setPosition(m_position + XVec2(-halfPixelSize.x + 8.0f, start * m_scale.y));
			m_font.draw();
		}
	}	
	if (m_style.m_withMousePos)
	{//显示鼠标交点信息
		XVec2 pos = (getMousePos() - m_position) / m_scale;
		if (pos.x >= -m_size.x * 0.5f && pos.x <= m_size.x * 0.5f &&
			pos.y >= -m_size.y * 0.5f && pos.y <= m_size.y * 0.5f)
		{//鼠标点在有效范围内
			XFColor tmpColor = XFColor::darkGray;
			tmpColor.a = 0.5f;
			XRender::drawLine(m_position + XVec2(pos.x * m_scale.x, -halfPixelSize.y),
				m_position + XVec2(pos.x * m_scale.x, halfPixelSize.y), 0.5f, tmpColor);
			XRender::drawLine(m_position + XVec2(-halfPixelSize.x, pos.y * m_scale.y),
				m_position + XVec2(halfPixelSize.x, pos.y * m_scale.y), 0.5f, tmpColor);
			//显示鼠标位置的坐标细节
#ifdef XRULER_DRAW_OPTIM
			sprintf_s(tmpStr, "%g", pos.x - m_posOffset.x);
			m_font.setString(tmpStr);
#else
			m_font.setString(XString::toString(pos.x - m_posOffset.x).c_str());
#endif
			if (getMousePos().x >= m_position.x)
			{
				m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_RIGHT);
				m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
				//if (getMousePos().y < m_position.y)
				//{
				//	m_font.setPosition(m_position + XVec2(pos.x * m_scale.x - 8.0f,14.0f));
				//}
				//else
				{
					m_font.setPosition(m_position + XVec2(pos.x * m_scale.x - 8.0f, -halfPixelSize.y + 14.0f));
				}
			}
			else
			{
				m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
				m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
				m_font.setPosition(m_position + XVec2(pos.x * m_scale.x + 8.0f, -halfPixelSize.y + 14.0f));
			}
			m_font.draw();
#ifdef XRULER_DRAW_OPTIM
			sprintf_s(tmpStr, "%g", pos.y - m_posOffset.y);
			m_font.setString(tmpStr);
#else
			m_font.setString(XString::toString(pos.y - m_posOffset.y).c_str());
#endif
			if (getMousePos().y >= m_position.y)
			{
				m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_RIGHT);
				m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
				m_font.setPosition(m_position + XVec2(halfPixelSize.x - 8.0f, pos.y * m_scale.y - 8.0f));
			}
			else
			{
				m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_RIGHT);
				m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
				//if (getMousePos().x >= m_position.x)
				//{
				//	m_font.setPosition(m_position + XVec2(-8.0f, pos.y * m_scale.y + 8.0f));
				//}
				//else
				{
					m_font.setPosition(m_position + XVec2(halfPixelSize.x - 8.0f, pos.y * m_scale.y + 8.0f));
				}
			}
			m_font.draw();
		}
	}
}
}