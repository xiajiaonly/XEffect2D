#include "XStdHead.h"
#include "XSliderRing.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XMath\XColorSpace.h"

namespace XE
{
XSliderRing::XSliderRing()
	:m_isInited(false)
	, m_withFont(false)
	, m_textColor(0.0f, 1.0f)
	, m_mode(SLDR_MODE_HORIZONTAL)
	, m_minValue(0.0f)
	, m_maxValue(100.0f)
	, m_curValue(0.0f)
	, m_curSliderState(SLDR_STATE_NORMAL)
	, m_mouseFlyArea(50.0f)	//这个值使用一个系统设定，这个值不能在代码中进行修改，这是不理想的，但是考虑到它存在的无形性，所以默认允许这样操作
	, m_dataChanged(false)
	, m_isClockwise(-1)
	, m_trimValue(0.0f)
{
	m_ctrlType = CTRL_OBJ_SLIDERRING;
}
void XSliderRing::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = false;
}
bool XSliderRing::initWithoutSkin(const XVec2& size, float maxValue, float minValue,
	XSliderRingMode mode)
{
	if (m_isInited) return false;
	if (size.x <= 0.0f || size.y <= 0.0f) return false;
	if (minValue >= maxValue) XMath::swapTwoSum(minValue, maxValue);
	m_mode = mode;
	m_size = size;
	m_minValue = minValue;
	m_maxValue = maxValue;
	m_curValue = minValue;
	m_upValue = minValue;
	m_dataChanged = XFalse;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;

	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
	case SLDR_MODE_HORIZONTAL:
		m_curMouseRect.set(m_position, m_position + m_size * m_scale);
		break;
	case SLDR_MODE_RING:
		m_curMouseRect.set(m_position - m_scale * m_size.x,
			m_position + m_scale * m_size.x);
		break;
	}
	m_mouseRect = m_curMouseRect;

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	m_isInited = true;
	return true;
}
XBool XSliderRing::setFont(const char* caption, const XFontUnicode& font)
{
	if (!m_isInited) return false;
	if (caption != NULL) m_fontString = caption;
	else m_fontString = "";
	if (!m_caption.setACopy(font)) return XFalse;
	m_caption.setColor(m_textColor * m_color);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
															   //m_textColor.setColor(XFColor::black);
	m_caption.setColor(m_textColor * m_color);
	m_caption.setScale(m_scale);
	updateFontStr();
	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
		m_caption.setAngle(90.0f);
		m_caption.setPosition(m_position + XVec2(m_caption.getTextSize().x * 0.5f + 0.0f) * m_scale);
		m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
		m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
		break;
	case SLDR_MODE_HORIZONTAL:
		m_caption.setAngle(0.0f);
		m_caption.setPosition(m_position + XVec2(0.0f, m_caption.getTextSize().y * 0.5f) * m_scale);
		m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //设置字体左对齐
		m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
		break;
	case SLDR_MODE_RING:
		m_caption.setAngle(0.0f);
		m_caption.setPosition(m_position);
		m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //设置字体左对齐
		m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //设置字体居中对齐
		break;
	}

	m_withFont = XTrue;
	return XTrue;
}
void XSliderRing::draw()
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isVisible) return;	//如果不可见直接退出
	float w, step, offset;
	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
		switch (m_curSliderState)
		{
		case SLDR_STATE_NORMAL:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::downColor * m_color, true);
			break;
		case SLDR_STATE_DOWN:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::mouseColor * m_color, true);
			break;
		case SLDR_STATE_ON:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::specialColor * m_color, true);
			break;
		case SLDR_STATE_DISABLE:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::blackDownColor * m_color, true);
			break;
		}
		w = m_mouseRect.getHeight();
		step = 0.0f;
		offset = (m_curValue - m_minValue) / (m_maxValue - m_minValue);
		for (int i = 1; i < w; i += step)
		{
			float rate = static_cast<float>(i) / w;
			step = 3.0f + 5.0f * sin(rate * PI);
			rate -= offset;
			XFColor color = XColorSpace::getWheelColor(rate * 360.0f);
			XRender::drawLine(XVec2(m_mouseRect.left + 1.0f, m_mouseRect.top + i),
				XVec2(m_mouseRect.right - 1.0f, m_mouseRect.top + i), 2.0f, color);
		}
		break;
	case SLDR_MODE_HORIZONTAL:
		switch (m_curSliderState)
		{
		case SLDR_STATE_NORMAL:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::downColor * m_color, true);
			break;
		case SLDR_STATE_DOWN:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::mouseColor * m_color, true);
			break;
		case SLDR_STATE_ON:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::specialColor * m_color, true);
			break;
		case SLDR_STATE_DISABLE:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::blackDownColor * m_color, true);
			break;
		}
		w = m_mouseRect.getWidth();
		step = 0.0f;
		offset = (m_curValue - m_minValue) / (m_maxValue - m_minValue);
		for (int i = 1; i < w; i += step)
		{
			float rate = static_cast<float>(i) / w;
			step = 3.0f + 5.0f * sin(rate * PI);
			rate -= offset;
			XFColor color = XColorSpace::getWheelColor(rate * 360.0f);
			XRender::drawLine(XVec2(i + m_mouseRect.left, m_mouseRect.top + 1.0f),
				XVec2(i + m_mouseRect.left, m_mouseRect.bottom - 1.0f), 2.0f, color);
		}
		break;
	case SLDR_MODE_RING:
		switch (m_curSliderState)
		{
		case SLDR_STATE_NORMAL:
			XRender::drawFillRing(m_position, m_size.x * m_scale.x, m_size.y * m_scale.x,
				0.0f, 360.0f, 60, XCCS::downColor * m_color);
			break;
		case SLDR_STATE_DOWN:
			XRender::drawFillRing(m_position, m_size.x * m_scale.x, m_size.y * m_scale.x,
				0.0f, 360.0f, 60, XCCS::mouseColor * m_color);
			break;
		case SLDR_STATE_ON:
			XRender::drawFillRing(m_position, m_size.x * m_scale.x, m_size.y * m_scale.x,
				0.0f, 360.0f, 60, XCCS::specialColor * m_color);
			break;
		case SLDR_STATE_DISABLE:
			XRender::drawFillRing(m_position, m_size.x * m_scale.x, m_size.y * m_scale.x,
				0.0f, 360.0f, 60, XCCS::blackDownColor * m_color);
			break;
		}
		w = m_mouseRect.getWidth();
		step = 360.0f / (PI * m_size.x * m_scale.x * 2.0f / 5.0f);
		offset = (m_curValue - m_minValue) / (m_maxValue - m_minValue);
		for (float i = 0.0f; i < 360.0f; i += step)
		{
			XVec2 e = m_scale.x * XMath::getRotateRate(i * DEGREE2RADIAN);
			XVec2 s = m_position + m_size.y * e;
			e = m_position + m_size.x * e;
			XFColor color = XColorSpace::getWheelColor(i + offset * 360.0f);
			XRender::drawLine(s, e, 2.0f, color);
		}
		if (m_curSliderState == SLDR_STATE_DOWN && m_isClockwise != -1)
		{//描绘滑过的区间
			float sAngle,eAngle;
			if (m_isClockwise == 0)
			{//逆时针方向
				sAngle = m_position.getAngle(m_oldMousePos);
				eAngle = m_position.getAngle(getMousePos());
				if (eAngle > sAngle) eAngle -= PI2;
				std::swap(eAngle, sAngle);
			}
			else
			{//顺时针方向
				sAngle = m_position.getAngle(m_oldMousePos);
				eAngle = m_position.getAngle(getMousePos());
				if (eAngle < sAngle) eAngle += PI2;
			}

			XRender::drawFillFan(m_position, m_size.y * m_scale.x, (sAngle + PI) * RADIAN2DEGREE,
				(eAngle + PI) * RADIAN2DEGREE, 60, XFColor(1.0f, 0.25f));
		}
		break;
	}
	if (m_withFont) m_caption.draw();
}
float XSliderRing::getDistanceValue()const
{
	if (!m_isInited) return 0.0f;
	if (m_curSliderState != SLDR_STATE_DOWN) return 0.0f;
	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
		if (getMousePos().y < m_oldMousePos.y)
		{
			if (m_curValue > m_upValue) return m_curValue - m_upValue - m_maxValue;
			else return m_curValue - m_upValue;
		}
		else
		{
			if (m_curValue < m_upValue) return m_curValue - m_upValue + m_maxValue;
			else return m_curValue - m_upValue;
		}
		break;
	case SLDR_MODE_HORIZONTAL:
		if (getMousePos().x < m_oldMousePos.x)
		{
			if (m_curValue > m_upValue) return m_curValue - m_upValue - m_maxValue;
			else return m_curValue - m_upValue;
		}
		else
		{
			if (m_curValue < m_upValue) return m_curValue - m_upValue + m_maxValue;
			else return m_curValue - m_upValue;
		}
		break;
	case SLDR_MODE_RING:
		if (m_isClockwise == -1) return 0.0f;
		if (m_isClockwise == 0)
		{
			if (m_curValue < m_upValue) return m_curValue - m_upValue + m_maxValue;
			else return m_curValue - m_upValue;
		}
		else
		{
			if (m_curValue > m_upValue) return m_curValue - m_upValue - m_maxValue;
			else return m_curValue - m_upValue;
		}
		break;
	}
	return 0.0f;
}
XBool XSliderRing::mouseProc(const XVec2& p, XMouseState mouseState)
{
	if (!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	if (m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if (m_isSilent) return XFalse;

	if (m_curSliderState == SLDR_STATE_DOWN)
	{//注意这里是滑动条拖动时，为了是超出范围的拖动仍然有效，所以这个这个特殊处理
		XRect tempFly(m_curMouseRect.getLT() - m_mouseFlyArea, m_curMouseRect.getRB() + m_mouseFlyArea);
		if (tempFly.isInRect(p))
		{//拖动(注意如果这里拖动超出范围，则使用最后一次的有效值)
			switch (mouseState)
			{
			case MOUSE_MOVE://重新计算Slider的当前值
				updateCurValue(p);
				m_dataChanged = XTrue;	//标记数值发生改变
				if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, SLDR_MOUSE_MOVE);
				else XCtrlManager.eventProc(m_objectID, SLDR_MOUSE_MOVE);
				break;
			case MOUSE_LEFT_BUTTON_UP://这个弹起事件是在按钮上按下的弹起事件
				m_curSliderState = SLDR_STATE_ON;
				if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, SLDR_MOUSE_UP);
				else XCtrlManager.eventProc(m_objectID, SLDR_MOUSE_UP);
				if (m_dataChanged)
				{
					if (m_eventProc != NULL)
						m_eventProc(m_pClass, m_objectID, SLDR_VALUE_CHANGE);
					else
						XCtrlManager.eventProc(m_objectID, SLDR_VALUE_CHANGE);
					m_dataChanged = XFalse;
				}
				break;
			}
			m_isBeChoose = XTrue;
		}
		else
		{
			m_curSliderState = SLDR_STATE_NORMAL;
			if (m_dataChanged)
			{
				if (m_eventProc != NULL)
					m_eventProc(m_pClass, m_objectID, SLDR_VALUE_CHANGE);
				else
					XCtrlManager.eventProc(m_objectID, SLDR_VALUE_CHANGE);
				m_dataChanged = XFalse;
			}
		}
	}
	if (m_curMouseRect.isInRect(p))
	{//鼠标动作在范围内
		switch (mouseState)
		{
		case MOUSE_MOVE:
			if (m_curSliderState != SLDR_STATE_NORMAL) break; //悬浮
			m_curSliderState = SLDR_STATE_ON;
			if (m_withAction)
			{//这里测试一个动态效果
			//	m_isInAction = XTrue;
			}
			break;
		case MOUSE_LEFT_BUTTON_DOWN:
		case MOUSE_LEFT_BUTTON_DCLICK:
			if (m_curSliderState != SLDR_STATE_NORMAL && m_curSliderState != SLDR_STATE_ON) break;

			m_upValue = m_curValue;
			m_oldMousePos = p;
			m_isClockwise = -1;

			m_curSliderState = SLDR_STATE_DOWN;
			if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, SLDR_MOUSE_DOWN);
			else XCtrlManager.eventProc(m_objectID, SLDR_MOUSE_DOWN);
			break;
		case MOUSE_LEFT_BUTTON_UP:
			if (m_curSliderState != SLDR_STATE_DOWN) break;
			//这里会触发数值改变事件(这个弹起事件应该是在线上按下的弹起)(*这里貌似不会发生*)
			m_curSliderState = SLDR_STATE_ON;
			if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, SLDR_MOUSE_UP);
			else XCtrlManager.eventProc(m_objectID, SLDR_MOUSE_UP);
			if (m_dataChanged)
			{
				if (m_eventProc != NULL)
					m_eventProc(m_pClass, m_objectID, SLDR_VALUE_CHANGE);
				else
					XCtrlManager.eventProc(m_objectID, SLDR_VALUE_CHANGE);
				m_dataChanged = XFalse;
			}
			break;
		case MOUSE_WHEEL_UP_DOWN:
			if (m_curSliderState != SLDR_STATE_ON) break;
			m_trimValue = m_keyOneValue * 0.1f;
			m_curValue += m_trimValue;
			if (m_curValue > m_maxValue) m_curValue = m_minValue + m_curValue - m_maxValue;
			updateFontStr();
			if (m_eventProc != NULL)
			{
				m_eventProc(m_pClass, m_objectID, SLDR_VALUE_CHANGE);
				m_eventProc(m_pClass, m_objectID, SLDR_VALUE_TRIM);
			}
			else
			{
				XCtrlManager.eventProc(m_objectID, SLDR_VALUE_CHANGE);
				XCtrlManager.eventProc(m_objectID, SLDR_VALUE_TRIM);
			}
			m_dataChanged = XFalse;
			break;
		case MOUSE_WHEEL_DOWN_DOWN:
			if (m_curSliderState != SLDR_STATE_ON) break;
			m_trimValue = -m_keyOneValue * 0.1f;
			m_curValue += m_trimValue;
			if (m_curValue < m_minValue) m_curValue = m_maxValue - m_minValue + m_curValue;
			updateFontStr();
			if (m_eventProc != NULL)
			{
				m_eventProc(m_pClass, m_objectID, SLDR_VALUE_CHANGE);
				m_eventProc(m_pClass, m_objectID, SLDR_VALUE_TRIM);
			}
			else
			{
				XCtrlManager.eventProc(m_objectID, SLDR_VALUE_CHANGE);
				XCtrlManager.eventProc(m_objectID, SLDR_VALUE_TRIM);
			}
			m_dataChanged = XFalse;
			break;
		}
	}
	else
	{
		if (m_curSliderState == SLDR_STATE_ON) m_curSliderState = SLDR_STATE_NORMAL;
	}

	return XTrue;
}
void XSliderRing::updateCurValue(const XVec2& pos)
{
	float changeRate, tmpAngle;
	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
		changeRate = (pos.y - m_oldMousePos.y) / m_mouseRect.getHeight() * (m_maxValue - m_minValue);
		m_curValue = m_upValue + changeRate;
		while (m_curValue < m_minValue)
		{
			m_curValue = m_maxValue - m_minValue + m_curValue;
		}
		while (m_curValue > m_maxValue)
		{
			m_curValue = m_minValue + m_curValue - m_maxValue;
		}
		break;
	case SLDR_MODE_HORIZONTAL:
		changeRate = (pos.x - m_oldMousePos.x) / m_mouseRect.getWidth() * (m_maxValue - m_minValue);
		m_curValue = m_upValue + changeRate;
		while (m_curValue < m_minValue)
		{
			m_curValue = m_maxValue - m_minValue + m_curValue;
		}
		while (m_curValue > m_maxValue)
		{
			m_curValue = m_minValue + m_curValue - m_maxValue;
		}
		break;
	case SLDR_MODE_RING:
		tmpAngle = m_position.getAngle(m_oldMousePos);
		changeRate = m_position.getAngle(pos);
		if (m_isClockwise == -1)
		{//先判断方向
			if (XMath::angleDistance(tmpAngle * RADIAN2DEGREE, changeRate * RADIAN2DEGREE) <= 0)
				m_isClockwise = 1;
			else
				m_isClockwise = 0;
		}
		if (m_isClockwise == 0)
		{//逆时针方向
			tmpAngle = m_position.getAngle(m_oldMousePos);
			changeRate = m_position.getAngle(pos);
			if (changeRate > tmpAngle) changeRate -= PI2;
			changeRate = changeRate - tmpAngle;
		}
		else
		{//顺时针方向
			tmpAngle = m_position.getAngle(m_oldMousePos);
			changeRate = m_position.getAngle(pos);
			if (changeRate < tmpAngle) changeRate += PI2;
			changeRate = changeRate - tmpAngle;
		}
		changeRate = changeRate / PI2 * (m_maxValue - m_minValue);

		m_curValue = m_upValue - changeRate;
		while (m_curValue < m_minValue)
		{
			m_curValue = m_maxValue - m_minValue + m_curValue;
		}
		while (m_curValue > m_maxValue)
		{
			m_curValue = m_minValue + m_curValue - m_maxValue;
		}
		break;
	}
	updateFontStr();
}
void XSliderRing::setPosition(const XVec2& p)
{
	if (!m_isInited) return;
	m_position = p;
	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
	case SLDR_MODE_HORIZONTAL:
		m_curMouseRect.set(m_position, m_position + m_size * m_scale);
		break;
	case SLDR_MODE_RING:
		m_curMouseRect.set(m_position - m_scale * m_size.x,
			m_position + m_scale * m_size.x);
		break;
	}
	m_mouseRect = m_curMouseRect;

	if (m_withFont)
	{
		m_caption.setScale(m_scale);
		switch (m_mode)
		{
		case SLDR_MODE_VERTICAL:
			m_caption.setPosition(m_position + XVec2(m_caption.getTextSize().x * 0.5f + 0.0f) * m_scale);
			break;
		case SLDR_MODE_HORIZONTAL:
			m_caption.setPosition(m_position + XVec2(0.0f, m_caption.getTextSize().y * 0.5f) * m_scale);
			break;
		case SLDR_MODE_RING:
			m_caption.setPosition(m_position);
			break;
		}
	}
	updateChildPos();
}
void XSliderRing::setScale(const XVec2& s)
{
	if (!m_isInited ||
		s.x <= 0 || s.y <= 0) return;
	m_scale = s;
	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
	case SLDR_MODE_HORIZONTAL:
		m_curMouseRect.set(m_position, m_position + m_size * m_scale);
		break;
	case SLDR_MODE_RING:
		m_curMouseRect.set(m_position - m_scale * m_size.x,
			m_position + m_scale * m_size.x);
		break;
	}
	m_mouseRect = m_curMouseRect;
	if (m_withFont)
	{
		m_caption.setScale(m_scale);
		switch (m_mode)
		{
		case SLDR_MODE_VERTICAL:
			m_caption.setPosition(m_position + XVec2(m_caption.getTextSize().x * 0.5f + 0.0f) * m_scale);
			break;
		case SLDR_MODE_HORIZONTAL:
			m_caption.setPosition(m_position + XVec2(0.0f, m_caption.getTextSize().y * 0.5f) * m_scale);
			break;
		case SLDR_MODE_RING:
			m_caption.setPosition(m_position);
			break;
		}
	}
	updateChildScale();
}
#if !WITH_INLINE_FILE
#include "XSliderRing.inl"
#endif
}