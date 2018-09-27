#include "XStdHead.h"
#include "XPicRectEx.h"
namespace XE {
bool XPicRectEx::init(const XVec2& rectSize,	//板子的大小
	const XVec2& windowsPosition,		//板子的位置
	const XVec2& texturePosition,		//贴图的位置
	GLuint *texture, const XVec2& textureSize,
	GLuint *textureShader)
{
	if (m_isInited) return false;

	m_size = rectSize * 0.5f;
	m_position = windowsPosition + m_size;
	m_v.resize(6);
	//下面计算U的相关信息(尚未完成)
	m_u.push_back(windowsPosition / textureSize);
	m_u.push_back((windowsPosition + XVec2(m_size.x * 2.0f, 0.0f)) / textureSize);
	m_u.push_back((windowsPosition + XVec2(0.0f, m_size.y * 2.0f)) / textureSize);
	m_u.push_back((windowsPosition + m_size * 2.0f) / textureSize);

	m_su.push_back(XVec2::zero);
	m_su.push_back(XVec2::oneZero);
	m_su.push_back(XVec2::zeroOne);
	m_su.push_back(XVec2::one);

	m_texturePosition = texturePosition;
	m_texture = texture;

	m_textureShader = textureShader;

	m_turnSpeed = 1.0f;
	m_state = PIC_STA_NORMAL;

//	m_isChangeTexture = false;
	setCurAngle(0.0f);

	m_isInited = true;
	return true;
}
void XPicRectEx::draw(bool withS)
{
	if (!m_isInited) return;
	std::vector<XVec2> v(4), u(4), su(4);
	float turnRate, maxAlpha = 0.5f;
	switch (m_state)
	{
	case PIC_STA_NORMAL:
	case PIC_STA_UP_TURN_ENTER:
	case PIC_STA_LEFT_TURN_ENTER:
	case PIC_STA_DOWN_TURN_ENTER:
	case PIC_STA_RIGHT_TURN_ENTER:
		v[0].set(m_v[0]);
		v[1].set(m_v[1]);
		v[2].set(m_v[3]);
		v[3].set(m_v[4]);
		u = m_u;
		if (m_texture != nullptr)
			XRender::drawTexture(*m_texture, &v[0], &m_u[0], 4, true);
		break;
	case PIC_STA_UP_TURN_IN:
	case PIC_STA_LEFT_TURN_IN:
		turnRate = -m_turnAngle / 120.0f;
		if (m_turnAngle > -60.0f)
		{
			if (m_turnAngle < -30.0f)
			{
				v[0].set(m_v[0]);
				v[1].set(m_v[1]);
				v[2].set(m_v[3]);
				v[3].set(m_v[4]);
				if (m_state == PIC_STA_LEFT_TURN_ENTER || m_state == PIC_STA_LEFT_TURN_IN)
				{//翻起来
					if (m_textureSecond != nullptr)
					{
						XRender::drawTexture(*m_textureSecond, &v[0], &m_u[0], 4, true,
							XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
					}
					if (withS)
					{
						XRender::drawTexture(m_textureShader[TEXTURE_LEFT], &v[0], &m_su[0], 4, true,
							XFColor(1.0f, cos(turnRate * PI_HALF)));
					}
				}
				else
				{
					if (m_textureSecond != nullptr)
					{
						u[0] = m_u[0]; u[1] = m_u[2]; u[2] = m_u[1]; u[3] = m_u[3];
						XRender::drawTexture(*m_textureSecond, &v[0], &u[0], 4, true,
							XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
					}
					if (withS)
					{
						su[0] = m_su[0]; su[1] = m_su[2]; su[2] = m_su[1]; su[3] = m_su[3];
						XRender::drawTexture(m_textureShader[TEXTURE_UP], &v[0], &su[0], 4, true,
							XFColor(1.0f, cos(turnRate * PI_HALF)));
					}
				}
			}
			v[0].set(m_v[1]);
			v[1].set(m_v[2]);
			v[2].set(m_v[4]);
			v[3].set(m_v[5]);
			if (m_state == PIC_STA_LEFT_TURN_ENTER || m_state == PIC_STA_LEFT_TURN_IN)
			{//翻起来
				if (m_texture != nullptr)
				{
					XRender::drawTexture(*m_texture, &v[0], &m_u[0], 4, true,
						XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
				}
				if (withS)
				{
					XRender::drawTexture(m_textureShader[TEXTURE_RIGHT], &v[0], &m_su[0], 4, true,
						XFColor(1.0f, sin(turnRate * PI_HALF)));
				}
			}
			else
			{
				if (m_texture != nullptr)
				{
					u[0] = m_u[0]; u[1] = m_u[2]; u[2] = m_u[1]; u[3] = m_u[3];
					XRender::drawTexture(*m_texture, &v[0], &u[0], 4, true,
						XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
				}
				if (withS)
				{
					su[0] = m_su[0]; su[1] = m_su[2]; su[2] = m_su[1]; su[3] = m_su[3];
					XRender::drawTexture(m_textureShader[TEXTURE_DOWN], &v[0], &su[0], 4, true,
						XFColor(1.0f, sin(turnRate * PI_HALF)));
				}
			}
		}
		else
		{
			if (m_turnAngle > -90.0f)
			{
				v[0].set(m_v[1]);
				v[1].set(m_v[2]);
				v[2].set(m_v[4]);
				v[3].set(m_v[5]);
				if (m_state == PIC_STA_LEFT_TURN_ENTER || m_state == PIC_STA_LEFT_TURN_IN)
				{//翻起来
					if (m_texture != nullptr)
					{
						XRender::drawTexture(*m_texture, &v[0], &m_u[0], 4, true,
							XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
					}
					if (withS)
					{
						XRender::drawTexture(m_textureShader[TEXTURE_RIGHT], &v[0], &m_su[0], 4, true,
							XFColor(1.0f, sin(turnRate * PI_HALF)));
					}
				}
				else
				{
					if (m_texture != nullptr)
					{
						u[0] = m_u[0]; u[1] = m_u[2]; u[2] = m_u[1]; u[3] = m_u[3];
						XRender::drawTexture(*m_texture, &v[0], &u[0], 4, true,
							XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
					}
					if (withS)
					{
						su[0] = m_su[0]; su[1] = m_su[2]; su[2] = m_su[1]; su[3] = m_su[3];
						XRender::drawTexture(m_textureShader[TEXTURE_DOWN], &v[0], &su[0], 4, true,
							XFColor(1.0f, sin(turnRate * PI_HALF)));
					}
				}
			}
			v[0].set(m_v[0]);
			v[1].set(m_v[1]);
			v[2].set(m_v[3]);
			v[3].set(m_v[4]);
			if (m_state == PIC_STA_LEFT_TURN_ENTER || m_state == PIC_STA_LEFT_TURN_IN)
			{//翻起来
				if (m_textureSecond != nullptr)
				{
					XRender::drawTexture(*m_textureSecond, &v[0], &m_u[0], 4, true,
						XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
				}
				if (withS)
				{
					XRender::drawTexture(m_textureShader[TEXTURE_LEFT], &v[0], &m_su[0], 4, true,
						XFColor(1.0f, cos(turnRate * PI_HALF)));
				}
			}
			else
			{
				if (m_textureSecond != nullptr)
				{
					u[0] = m_u[0]; u[1] = m_u[2]; u[2] = m_u[1]; u[3] = m_u[3];
					XRender::drawTexture(*m_textureSecond, &v[0], &u[0], 4, true,
						XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
				}
				if (withS)
				{
					su[0] = m_su[0]; su[1] = m_su[2]; su[2] = m_su[1]; su[3] = m_su[3];
					XRender::drawTexture(m_textureShader[TEXTURE_UP], &v[0], &su[0], 4, true,
						XFColor(1.0f, cos(turnRate * PI_HALF)));
				}
			}
		}
		break;
	case PIC_STA_DOWN_TURN_IN:
	case PIC_STA_RIGHT_TURN_IN:
		turnRate = m_turnAngle / 120.0f;
		if (m_turnAngle < 60.0f)
		{
			if (m_turnAngle > 30.0f)
			{
				v[0].set(m_v[1]);
				v[1].set(m_v[2]);
				v[2].set(m_v[4]);
				v[3].set(m_v[5]);
				if (m_state == PIC_STA_RIGHT_TURN_ENTER || m_state == PIC_STA_RIGHT_TURN_IN)
				{//翻起来
					if (m_textureSecond != nullptr)
					{
						XRender::drawTexture(*m_textureSecond, &v[0], &m_u[0], 4, true,
							XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
					}
					if (withS)
					{
						XRender::drawTexture(m_textureShader[TEXTURE_RIGHT], &v[0], &m_su[0], 4, true,
							XFColor(1.0f, cos(turnRate * PI_HALF)));
					}
				}
				else
				{
					if (m_textureSecond != nullptr)
					{
						u[0] = m_u[0]; u[1] = m_u[2]; u[2] = m_u[1]; u[3] = m_u[3];
						XRender::drawTexture(*m_textureSecond, &v[0], &u[0], 4, true,
							XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
					}
					if (withS)
					{
						su[0] = m_su[0]; su[1] = m_su[2]; su[2] = m_su[1]; su[3] = m_su[3];
						XRender::drawTexture(m_textureShader[TEXTURE_DOWN], &v[0], &su[0], 4, true,
							XFColor(1.0f, cos(turnRate * PI_HALF)));
					}
				}
			}
			v[0].set(m_v[0]);
			v[1].set(m_v[1]);
			v[2].set(m_v[3]);
			v[3].set(m_v[4]);
			if (m_state == PIC_STA_RIGHT_TURN_ENTER || m_state == PIC_STA_RIGHT_TURN_IN)
			{//翻起来
				if (m_texture != nullptr)
				{
					XRender::drawTexture(*m_texture, &v[0], &m_u[0], 4, true,
						XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
				}
				if (withS)
				{
					XRender::drawTexture(m_textureShader[TEXTURE_LEFT], &v[0], &m_su[0], 4, true,
						XFColor(1.0f, sin(turnRate * PI_HALF)));
				}
			}
			else
			{
				if (m_texture != nullptr)
				{
					u[0] = m_u[0]; u[1] = m_u[2]; u[2] = m_u[1]; u[3] = m_u[3];
					XRender::drawTexture(*m_texture, &v[0], &u[0], 4, true,
						XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
				}
				if (withS)
				{
					su[0] = m_su[0]; su[1] = m_su[2]; su[2] = m_su[1]; su[3] = m_su[3];
					XRender::drawTexture(m_textureShader[TEXTURE_UP], &v[0], &su[0], 4, true,
						XFColor(1.0f, sin(turnRate * PI_HALF)));
				}
			}
		}
		else
		{
			if (m_turnAngle < 90.0f)
			{
				v[0].set(m_v[0]);
				v[1].set(m_v[1]);
				v[2].set(m_v[3]);
				v[3].set(m_v[4]);
				if (m_state == PIC_STA_RIGHT_TURN_ENTER || m_state == PIC_STA_RIGHT_TURN_IN)
				{//翻起来
					if (m_texture != nullptr)
					{
						XRender::drawTexture(*m_texture, &v[0], &m_u[0], 4, true,
							XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
					}
					if (withS)
					{
						XRender::drawTexture(m_textureShader[TEXTURE_LEFT], &v[0], &m_su[0], 4, true,
							XFColor(1.0f, sin(turnRate * PI_HALF)));
					}
				}
				else
				{
					if (m_texture != nullptr)
					{
						u[0] = m_u[0]; u[1] = m_u[2]; u[2] = m_u[1]; u[3] = m_u[3];
						XRender::drawTexture(*m_texture, &v[0], &u[0], 4, true,
							XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
					}
					if (withS)
					{
						su[0] = m_su[0]; su[1] = m_su[2]; su[2] = m_su[1]; su[3] = m_su[3];
						XRender::drawTexture(m_textureShader[TEXTURE_UP], &v[0], &su[0], 4, true,
							XFColor(1.0f, sin(turnRate * PI_HALF)));
					}
				}
			}
			v[0].set(m_v[1]);
			v[1].set(m_v[2]);
			v[2].set(m_v[4]);
			v[3].set(m_v[5]);
			if (m_state == PIC_STA_RIGHT_TURN_ENTER || m_state == PIC_STA_RIGHT_TURN_IN)
			{//翻起来
				if (m_textureSecond != nullptr)
				{
					XRender::drawTexture(*m_textureSecond, &v[0], &m_u[0], 4, true,
						XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
				}
				if (withS)
				{
					XRender::drawTexture(m_textureShader[TEXTURE_RIGHT], &v[0], &m_su[0], 4, true,
						XFColor(1.0f, cos(turnRate * PI_HALF)));
				}
			}
			else
			{
				if (m_textureSecond != nullptr)
				{
					u[0] = m_u[0]; u[1] = m_u[2]; u[2] = m_u[1]; u[3] = m_u[3];
					XRender::drawTexture(*m_textureSecond, &v[0], &u[0], 4, true,
						XFColor(1.0f, 1.0f - sin(turnRate * PI) * maxAlpha));
				}
				if (withS)
				{
					su[0] = m_su[0]; su[1] = m_su[2]; su[2] = m_su[1]; su[3] = m_su[3];
					XRender::drawTexture(m_textureShader[TEXTURE_DOWN], &v[0], &su[0], 4, true,
						XFColor(1.0f, cos(turnRate * PI_HALF)));
				}
			}
		}
		break;
	}
}
void XPicRectEx::setCurAngle(float angle)
{
	m_turnAngle = angle;
	float tmpAngle = m_turnAngle;// +120.0f;
	float r;

	switch (m_state)
	{
	case PIC_STA_NORMAL:
		r = m_size.x / sin(60.0f * DEGREE2RADIAN);
		m_v[0].set(m_position.x - r * sin((60.0f + tmpAngle) * DEGREE2RADIAN),
			m_position.y - m_size.y,
			r * cos((60.0f + tmpAngle) * DEGREE2RADIAN));

		m_v[1].set(m_position.x + r * sin((60.0f - tmpAngle) * DEGREE2RADIAN),
			m_position.y - m_size.y,
			r * cos((tmpAngle - 60.0f) * DEGREE2RADIAN));

		m_v[2].set(m_position.x + r * sin(tmpAngle * DEGREE2RADIAN),
			m_position.y - m_size.y,
			r * cos((180.0f - tmpAngle) * DEGREE2RADIAN));

		m_v[3] = m_v[0];
		m_v[3].y = m_position.y + m_size.y;

		m_v[4] = m_v[1];
		m_v[4].y = m_position.y + m_size.y;

		m_v[5] = m_v[2];
		m_v[5].y = m_position.y + m_size.y;
		//下面计算景深
		for (int i = 0; i < 6; ++i)
		{
			if (m_v[i].z < 0.0f)	//防止冲突的伪景深方式
				m_v[i].y = m_position.y + (m_v[i].y - m_position.y)* (1.0f + m_v[i].z / r * 0.2f);
		}
		break;
	case PIC_STA_LEFT_TURN_ENTER:
	case PIC_STA_LEFT_TURN_IN:
		r = m_size.x / sin(60.0f * DEGREE2RADIAN);
		m_v[0].set(m_position.x + r * sin(tmpAngle * DEGREE2RADIAN),
			m_position.y - m_size.y,
			r * cos((180.0f - tmpAngle) * DEGREE2RADIAN));

		m_v[1].set(m_position.x - r * sin((60.0f + tmpAngle) * DEGREE2RADIAN),
			m_position.y - m_size.y,
			r * cos((60.0f + tmpAngle) * DEGREE2RADIAN));

		m_v[2].set(m_position.x + r * sin((60.0f - tmpAngle) * DEGREE2RADIAN),
			m_position.y - m_size.y,
			r * cos((tmpAngle - 60.0f) * DEGREE2RADIAN));

		m_v[3] = m_v[0];
		m_v[3].y = m_position.y + m_size.y;

		m_v[4] = m_v[1];
		m_v[4].y = m_position.y + m_size.y;

		m_v[5] = m_v[2];
		m_v[5].y = m_position.y + m_size.y;
		//下面计算景深
		for (int i = 0; i < 6; ++i)
		{
			if (m_v[i].z < 0.0f)	//防止冲突的伪景深方式
				m_v[i].y = m_position.y + (m_v[i].y - m_position.y)* (1.0f + m_v[i].z / r * 0.2f);
		}
		break;
	case PIC_STA_UP_TURN_ENTER:
	case PIC_STA_UP_TURN_IN:
		r = m_size.y / sin(60.0f * DEGREE2RADIAN);
		m_v[0].set(m_position.x - m_size.x,
			m_position.y + r * sin(tmpAngle * DEGREE2RADIAN),
			r * cos((180.0f - tmpAngle) * DEGREE2RADIAN));

		m_v[1].set(m_position.x - m_size.x,
			m_position.y - r * sin((60.0f + tmpAngle) * DEGREE2RADIAN),
			r * cos((60.0f + tmpAngle) * DEGREE2RADIAN));

		m_v[2].set(m_position.x - m_size.x,
			m_position.y + r * sin((60.0f - tmpAngle) * DEGREE2RADIAN),
			r * cos((tmpAngle - 60.0f) * DEGREE2RADIAN));

		m_v[3] = m_v[0];
		m_v[3].x = m_position.x + m_size.x;

		m_v[4] = m_v[1];
		m_v[4].x = m_position.x + m_size.x;

		m_v[5] = m_v[2];
		m_v[5].x = m_position.x + m_size.x;
		//下面计算景深
		for (int i = 0; i < 6; ++i)
		{
			if (m_v[i].z < 0.0f)	//防止冲突的伪景深方式
				m_v[i].x = m_position.x + (m_v[i].x - m_position.x) * (1.0f + m_v[i].z / r * 0.2f);
		}
		break;
	case PIC_STA_DOWN_TURN_ENTER:
	case PIC_STA_DOWN_TURN_IN:
		r = m_size.y / sin(60.0f * DEGREE2RADIAN);
		m_v[0].set(m_position.x - m_size.x,
			m_position.y - r * sin((60.0f + tmpAngle) * DEGREE2RADIAN),
			r * cos((60.0f + tmpAngle) * DEGREE2RADIAN));

		m_v[1].set(m_position.x - m_size.x,
			m_position.y + r * sin((60.0f - tmpAngle) * DEGREE2RADIAN),
			r * cos((tmpAngle - 60.0f) * DEGREE2RADIAN));

		m_v[2].set(m_position.x - m_size.x,
			m_position.y + r * sin(tmpAngle * DEGREE2RADIAN),
			r * cos((180.0f - tmpAngle) * DEGREE2RADIAN));

		m_v[3] = m_v[0];
		m_v[3].x = m_position.x + m_size.x;

		m_v[4] = m_v[1];
		m_v[4].x = m_position.x + m_size.x;

		m_v[5] = m_v[2];
		m_v[5].x = m_position.x + m_size.x;
		//下面计算景深
		for (int i = 0; i < 6; ++i)
		{
			if (m_v[i].z < 0.0f)	//防止冲突的伪景深方式
				m_v[i].x = m_position.x + (m_v[i].x - m_position.x)* (1.0f + m_v[i].z / r * 0.2f);
		}
		break;
	case PIC_STA_RIGHT_TURN_ENTER:
	case PIC_STA_RIGHT_TURN_IN:
		r = m_size.x / sin(60.0f * DEGREE2RADIAN);
		m_v[0].set(m_position.x - r * sin((60.0f + tmpAngle) * DEGREE2RADIAN),
			m_position.y - m_size.y,
			r * cos((60.0f + tmpAngle) * DEGREE2RADIAN));

		m_v[1].set(m_position.x + r * sin((60.0f - tmpAngle) * DEGREE2RADIAN),
			m_position.y - m_size.y,
			r * cos((tmpAngle - 60.0f) * DEGREE2RADIAN));

		m_v[2].set(m_position.x + r * sin(tmpAngle * DEGREE2RADIAN),
			m_position.y - m_size.y,
			r * cos((180.0f - tmpAngle) * DEGREE2RADIAN));

		m_v[3] = m_v[0];
		m_v[3].y = m_position.y + m_size.y;

		m_v[4] = m_v[1];
		m_v[4].y = m_position.y + m_size.y;

		m_v[5] = m_v[2];
		m_v[5].y = m_position.y + m_size.y;
		//下面计算景深
		for (int i = 0; i < 6; ++i)
		{
			if (m_v[i].z < 0.0f)	//防止冲突的伪景深方式
				m_v[i].y = m_position.y + (m_v[i].y - m_position.y)* (1.0f + m_v[i].z / r * 0.2f);
		}
		break;
	}
}
void XPicRectEx::move(float stepTime)
{
	if (!m_isInited) return;
	switch (m_state)
	{
	case PIC_STA_LEFT_TURN_ENTER:
		m_turnAngle = 0.0f;
		m_state = PIC_STA_LEFT_TURN_IN;
		m_isIntTurn = false;
		setCurAngle(m_turnAngle);
		break;
	case PIC_STA_UP_TURN_ENTER:
		m_turnAngle = 0.0f;
		m_state = PIC_STA_UP_TURN_IN;
		m_isIntTurn = false;
		setCurAngle(m_turnAngle);
		break;
	case PIC_STA_LEFT_TURN_IN:
	case PIC_STA_UP_TURN_IN:
		m_turnAngle -= m_turnSpeed * stepTime;
		if (m_turnAngle <= -120.0f)
		{
			m_turnAngle = 0.0f;
			m_state = PIC_STA_NORMAL;
			m_texture = m_textureSecond;
		}
		setCurAngle(m_turnAngle);
		break;

	case PIC_STA_RIGHT_TURN_ENTER:
		m_turnAngle = 0.0f;
		m_state = PIC_STA_RIGHT_TURN_IN;
		m_isIntTurn = false;
		setCurAngle(m_turnAngle);
		break;
	case PIC_STA_DOWN_TURN_ENTER:
		m_turnAngle = 0.0f;
		m_state = PIC_STA_DOWN_TURN_IN;
		m_isIntTurn = false;
		setCurAngle(m_turnAngle);
		break;
	case PIC_STA_DOWN_TURN_IN:
	case PIC_STA_RIGHT_TURN_IN:
		m_turnAngle += m_turnSpeed * stepTime;
		if (m_turnAngle >= 120.0f)
		{
			m_turnAngle = 0.0f;
			m_state = PIC_STA_NORMAL;
			m_texture = m_textureSecond;
		}
		setCurAngle(m_turnAngle);
		break;
	}
}
}