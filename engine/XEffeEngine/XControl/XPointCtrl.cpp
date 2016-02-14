#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XPointCtrl.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
XBool XPointCtrl::init(const XVector2& position,const XFontUnicode *font)
{
	if(m_isInited) return XFalse;
	m_position = position;
	if(font == NULL)
	{
		m_withFont = false;
	}else
	{
		m_withFont = true;
		m_font.setACopy(*font);
		m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_font);
#endif
	}
	XCtrlManager.addACtrl(this);
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	updateData();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	m_isInited = XTrue;
	return XTrue;
}
void XPointCtrl::draw()
{
	if(!m_isInited ||
		!m_isVisible) return;	//如果不可见直接退出

	switch(m_ctrlMode)
	{
	case CTRL_MODE_COMPLETE:
		//描绘网格
		if(m_withRange)
		{
			float w = m_range.getWidth() / m_pointCtrlLineSum;
			float h = m_range.getHeight() / m_pointCtrlLineSum;
			for(int i = 1;i < m_pointCtrlLineSum;++ i)
			{
				XRender::drawLine(m_range.left + w * i,m_range.top + 12.0f,m_range.left + w * i,m_range.bottom - 12.0f,1.0f,
					m_color.fR, m_color.fG, m_color.fB, 0.5f * m_color.fA, XRender::LS_DASHES);
				XRender::drawLine(m_range.left + 12.0f,m_range.top + h * i,m_range.right - 12.0f,m_range.top + h * i,1.0f,
					m_color.fR, m_color.fG, m_color.fB, 0.5f * m_color.fA, XRender::LS_DASHES);
			}
		}
	case CTRL_MODE_NORMAL:
		//描绘边框和标尺
		if(m_withRange) 
		{
			XRender::drawRect(m_range);
			float w = m_range.getWidth() / m_pointCtrlLineSum;
			float h = m_range.getHeight() / m_pointCtrlLineSum;
			for(int i = 1;i < m_pointCtrlLineSum;++ i)
			{
				XRender::drawLine(m_range.left + w * i,m_range.top,m_range.left + w * i,m_range.top + 10.0f,1.0f,
					m_color);
				XRender::drawLine(m_range.left + w * i,m_range.bottom,m_range.left + w * i,m_range.bottom - 10.0f,1.0f,
					m_color);
				XRender::drawLine(m_range.left,m_range.top + h * i,m_range.left + 10.0f,m_range.top + h * i,1.0f,
					m_color);
				XRender::drawLine(m_range.right,m_range.top + h * i,m_range.right - 10.0f,m_range.top + h * i,1.0f,
					m_color);
			}
		}
	case CTRL_MODE_SIMPLE:
		break;
	}
	//描绘点原的位置
	if(m_isDown)
	{
	//	XRender::drawLine(m_position.x - m_truePixelSize.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y,2,
	//		m_color.fR,0.0f,0.0f,m_color.fA);
	//	XRender::drawLine(m_position.x,m_position.y - m_truePixelSize.y,m_position.x,m_position.y + m_truePixelSize.y,2,
	//		m_color.fR,0.0f,0.0f,m_color.fA);
		XRender::drawLineAntiColor(m_position.x - m_truePixelSize.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y,2);
		XRender::drawLineAntiColor(m_position.x,m_position.y - m_truePixelSize.y,m_position.x,m_position.y + m_truePixelSize.y,2);
	}else
	{
		XRender::drawLine(m_position.x - m_truePixelSize.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y,1,
			m_color);
		XRender::drawLine(m_position.x,m_position.y - m_truePixelSize.y,m_position.x,m_position.y + m_truePixelSize.y,1,
			m_color);
	}
	if(m_withFont) m_font.draw();	//显示文字提示
}
XBool XPointCtrl::mouseProc(float x,float y,XMouseState mouseState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable) return XFalse;		//如果无效则直接退出
	switch(mouseState)
	{
	case MOUSE_MOVE:
		if(!m_isDown) break;
		setPosition(x,y);
		if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,PITCTRL_DATA_CHANGE);
		else XCtrlManager.eventProc(m_objectID,PITCTRL_DATA_CHANGE);
		break;
	case MOUSE_LEFT_BUTTON_DOWN:
	case MOUSE_LEFT_BUTTON_DCLICK:
		//鼠标按下
		if(m_position.getLengthSqure(x,y) < 100.0f)
		{
			m_isDown = true;
			m_isBeChoose = XTrue;
		}
		break;
	case MOUSE_LEFT_BUTTON_UP:
		if(m_isDown) m_isDown = false;
		break;
	}
	return XTrue;
}
XBool XPointCtrl::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//如果没有初始化直接退出
		!m_isActive ||		//没有激活的控件不接收控制
		!m_isVisible ||	//如果不可见直接退出
		!m_isEnable ||
		!m_isBeChoose) return XFalse;		//如果无效则直接退出
	if(m_withAction && m_isInAction) return XFalse;	//如果支持动作播放而且正在播放动画那么不接受鼠标控制
	if(keyState == KEY_STATE_UP && !m_isDown)
	{
		switch(keyOrder)
		{
		case XKEY_UP:
			setPosition(m_position.x,m_position.y - 1);
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,PITCTRL_DATA_CHANGE);
			else XCtrlManager.eventProc(m_objectID,PITCTRL_DATA_CHANGE);
			break;
		case XKEY_DOWN:
			setPosition(m_position.x,m_position.y + 1);
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,PITCTRL_DATA_CHANGE);
			else XCtrlManager.eventProc(m_objectID,PITCTRL_DATA_CHANGE);
			break;
		case XKEY_LEFT:
			setPosition(m_position.x - 1,m_position.y);
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,PITCTRL_DATA_CHANGE);
			else XCtrlManager.eventProc(m_objectID,PITCTRL_DATA_CHANGE);
			break;
		case XKEY_RIGHT:
			setPosition(m_position.x + 1,m_position.y);
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,PITCTRL_DATA_CHANGE);
			else XCtrlManager.eventProc(m_objectID,PITCTRL_DATA_CHANGE);
			break;
		}
	}
	return XFalse;
}
XBool XPointCtrl::setACopy(const XPointCtrl & temp)
{
	if(& temp == this) return XTrue;	//防止自身赋值
	if(!temp.m_isInited) return XFalse;
	if(!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;
	m_position = temp.m_position;	//控件的位置
	m_scale = temp.m_scale;		//大小
	m_pixelSize = temp.m_pixelSize;	//像素大小
	m_truePixelSize = temp.m_truePixelSize;	//真实的像素尺寸
	if(!m_font.setACopy(temp.m_font))	 return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_font);
#endif
	memcpy(m_textStr,temp.m_textStr,64);		//显示的字符串

	m_withFont = temp.m_withFont;
	m_isDown = temp.m_isDown;		//是否被鼠标拾取
	m_withRange = temp.m_withRange;	//范围设置是否有效
	m_range = temp.m_range;			//可以移动的范围
	m_withMap = temp.m_withMap;		//是否进行映射
	m_mapRange = temp.m_mapRange;	//映射的范围
	m_mapValue = temp.m_mapValue;	//映射的范围

	m_ctrlMode = temp.m_ctrlMode;	//控件模式

	if(temp.m_withFont)
	{
		m_font.setACopy(temp.m_font);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_font);
#endif
	}

	return XTrue;
}
void XPointCtrl::release()
{
	XCtrlManager.decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
}
void XPointCtrl::updateData()
{
	m_truePixelSize = m_pixelSize * m_scale;
	if(m_withRange)
	{//控制在范围内
		if(m_position.x < m_range.left) m_position.x = m_range.left;
		if(m_position.x > m_range.right) m_position.x = m_range.right;
		if(m_position.y < m_range.top) m_position.y = m_range.top;
		if(m_position.y > m_range.bottom) m_position.y = m_range.bottom;
	}
	if(m_withMap)
	{//这里进行映射
		m_mapValue.x = XMath::maping1D(m_position.x,m_range.left,m_range.right,m_mapRange.left,m_mapRange.right);
		m_mapValue.y = XMath::maping1D(m_position.y,m_range.top,m_range.bottom,m_mapRange.top,m_mapRange.bottom);
		if(m_withFont)
		{
			if(m_autoString)
			{
				sprintf(m_textStr,"(%.0f,%.0f)",m_mapValue.x,m_mapValue.y);
				m_font.setString(m_textStr);
			}
			m_font.setPosition(m_position);
		}
	}else
	{
		if(m_withFont)
		{
			if(m_autoString)
			{
				sprintf(m_textStr,"(%.0f,%.0f)",m_position.x,m_position.y);
				m_font.setString(m_textStr);
			}
			m_font.setPosition(m_position);
		}
	}
}
XBool XPointCtrl::setFontString(const char *str)
{
	if(!m_withFont) return false;
	if(str == NULL)
	{
		m_autoString = true;
		return true;
	}else
	{
		m_autoString = false;
		m_font.setString(str);
		return true;
	}
}
#if !WITH_INLINE_FILE
#include "XPointCtrl.inl"
#endif
}