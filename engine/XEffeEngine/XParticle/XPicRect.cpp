#include "XStdHead.h"
#include "XPicRect.h"
namespace XE {
bool XPicRect::init(const XVec2& rectSize, const XVec2& windowsPosition,
	const XVec2& texturePosition, GLuint *texture, const XVec2& textureSize,
	const XVec2& shaderSize, GLuint *textureShader)
{
	if (m_isInited) return false;
	m_rectSize = rectSize;
	m_windowsPosition = windowsPosition;
	m_texturePosition = texturePosition;
	m_texture = texture;
	m_sprite.init(textureSize.x, textureSize.y, 0);
	m_sprite.setIsTransformCenter(POINT_CENTER);
	m_sprite.setClipRect(XRect(m_texturePosition, m_texturePosition + m_rectSize));
	m_sprite.setPosition(m_windowsPosition.x, m_windowsPosition.y);
	m_textureShader = textureShader;
	m_shaderSize = shaderSize;
	m_spriteShader.init(m_shaderSize.x, m_shaderSize.y, 0);
	m_spriteShader.setPosition(m_windowsPosition - (m_shaderSize - m_rectSize) * 0.5f);
	m_turnSpeed = 1.0f;
	m_state = PIC_STA_NORMAL;
	m_sprite.setScale(1.0f);
	//m_lastPoint.set(0, 0);
	m_isChangeTexture = false;

//	m_sprite.setIsUseSolidify(1);	//设置使用显示列表

	m_isInited = true;
	return true;
}
void XPicRect::draw(bool withS)
{
	if (!m_isInited) return;
	switch (m_state)
	{
	case PIC_STA_NORMAL:
		//m_sprite.setSize(_XPoint(1.0,1.0));
		m_sprite.draw(m_texture);
		break;
	case PIC_STA_UP_TURN_ENTER:
	case PIC_STA_UP_TURN_IN:
	case PIC_STA_DOWN_TURN_ENTER:
	case PIC_STA_DOWN_TURN_IN:
		if (m_isChangeTexture)
		{
			m_sprite.setScale(1.0f, cos(m_turnAngle * DEGREE2RADIAN));
			if (!m_isIntTurn) m_sprite.draw(m_texture);
			else m_sprite.draw(m_textureSecond);
		}
		else
		{
			m_sprite.setScale(1.0f, cos(m_turnAngle * DEGREE2RADIAN));
			m_sprite.draw(m_texture);
		}
		if (m_turnAngle > 0.0f)
		{
			m_spriteShader.setAlpha(1.0f - cos(m_turnAngle * DEGREE2RADIAN));
			m_spriteShader.setScale(XVec2(1.0f, cos(m_turnAngle * DEGREE2RADIAN)) * m_rectSize / m_shaderSize);
			if(withS)
				m_spriteShader.draw(&m_textureShader[TEXTURE_DOWN]);
		}
		else
		{
			m_spriteShader.setAlpha(1.0f - cos(m_turnAngle * DEGREE2RADIAN));
			m_spriteShader.setScale(XVec2(1.0f, cos(m_turnAngle * DEGREE2RADIAN)) * m_rectSize / m_shaderSize);
			if (withS)
				m_spriteShader.draw(&m_textureShader[TEXTURE_UP]);
		}
		break;
	case PIC_STA_LEFT_TURN_ENTER:
	case PIC_STA_LEFT_TURN_IN:
	case PIC_STA_RIGHT_TURN_ENTER:
	case PIC_STA_RIGHT_TURN_IN:
		if (m_isChangeTexture)
		{
			m_sprite.setScale(cos(m_turnAngle * DEGREE2RADIAN), 1.0f);
			if (!m_isIntTurn) m_sprite.draw(m_texture);
			else m_sprite.draw(m_textureSecond);
		}
		else
		{
			m_sprite.setScale(cos(m_turnAngle * DEGREE2RADIAN), 1.0f);
			m_sprite.draw(m_texture);
		}
		if (m_turnAngle > 0.0f)
		{
			m_spriteShader.setAlpha(1.0 - cos(m_turnAngle * DEGREE2RADIAN));
			m_spriteShader.setScale(XVec2(cos(m_turnAngle * DEGREE2RADIAN), 1.0f) * m_rectSize / m_shaderSize);
			if (withS)
				m_spriteShader.draw(&m_textureShader[TEXTURE_RIGHT]);
		}
		else
		{
			m_spriteShader.setAlpha(1.0f - cos(m_turnAngle * DEGREE2RADIAN));
			m_spriteShader.setScale(XVec2(cos(m_turnAngle * DEGREE2RADIAN), 1.0f) * m_rectSize / m_shaderSize);
			if (withS)
				m_spriteShader.draw(&m_textureShader[TEXTURE_LEFT]);
		}
		break;
	}
}
void XPicRect::move(float timeDelay)
{
	if (!m_isInited) return;
	switch (m_state)
	{
	case PIC_STA_LEFT_TURN_ENTER:
		m_turnAngle = 0.0f;
		m_state = PIC_STA_LEFT_TURN_IN;
		m_isIntTurn = false;
		break;
	case PIC_STA_UP_TURN_ENTER:
		m_turnAngle = 0.0f;
		m_state = PIC_STA_UP_TURN_IN;
		m_isIntTurn = false;
		break;
	case PIC_STA_UP_TURN_IN:
	case PIC_STA_LEFT_TURN_IN:
		m_turnAngle += m_turnSpeed * timeDelay;
		if (m_turnAngle >= 90.0f && !m_isIntTurn)
		{
			m_turnAngle = -90.0f;
			m_isIntTurn = true;
		}
		if (m_turnAngle >= 0.0f && m_isIntTurn)
		{
			m_turnAngle = 0.0f;
			m_state = PIC_STA_NORMAL;
			m_sprite.setScale(1.0f);
			if (m_isChangeTexture)
			{
				m_texture = m_textureSecond;
				m_isChangeTexture = false;
			}
		}
		break;
	case PIC_STA_RIGHT_TURN_ENTER:
		m_turnAngle = 0.0f;
		m_state = PIC_STA_RIGHT_TURN_IN;
		m_isIntTurn = false;
		break;
	case PIC_STA_DOWN_TURN_ENTER:
		m_turnAngle = 0.0f;
		m_state = PIC_STA_DOWN_TURN_IN;
		m_isIntTurn = false;
		break;
	case PIC_STA_DOWN_TURN_IN:
	case PIC_STA_RIGHT_TURN_IN:
		m_turnAngle -= m_turnSpeed * timeDelay;
		if (m_turnAngle <= -90.0f && !m_isIntTurn)
		{
			m_turnAngle = 90.0f;
			m_isIntTurn = true;
		}
		if (m_turnAngle <= 0.0f && m_isIntTurn)
		{
			m_turnAngle = 0.0f;
			m_state = PIC_STA_NORMAL;
			m_sprite.setScale(1.0f);
			if (m_isChangeTexture)
			{
				m_texture = m_textureSecond;
				m_isChangeTexture = false;
			}
		}
		break;
	}
}
}